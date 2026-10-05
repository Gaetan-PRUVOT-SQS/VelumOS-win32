#include "proc_int.h"
#include "velum/err.h"
#include "velum/util.h"

static int	tstack_size(uint64_t req, uint64_t *out)
{
	if (req == 0)
		req = TSTACK_DEFAULT;
	if (req < TSTACK_MIN || req > TSTACK_MAX)
		return (E_INVAL);
	*out = align_up(req, PAGE_SIZE);
	return (0);
}

int	proc_tstack_alloc(t_process *p, uint64_t size, uint64_t arg,
		t_pthread *st)
{
	uint64_t	frame[2];
	uint64_t	va;
	int			rc;

	rc = tstack_size(size, &st->stack_len);
	if (rc < 0)
		return (rc);
	va = vmm_find_free(p->aspace, st->stack_len + PAGE_SIZE, ELF_ASLR_LO,
			ELF_ASLR_HI);
	if (va == 0)
		return (E_NOMEM);
	st->stack_va = va + PAGE_SIZE;
	rc = vmm_alloc(p->aspace, st->stack_va, st->stack_len,
			VM_USER | VM_R | VM_W);
	if (rc < 0)
		return (rc);
	frame[0] = arg;
	frame[1] = 0;
	rc = aspace_write(p->aspace, st->stack_va + st->stack_len - sizeof(frame),
			frame, sizeof(frame));
	if (rc < 0)
		vmm_unmap(p->aspace, st->stack_va, st->stack_len);
	return (rc);
}
