#include "velum/klog.h"
#include "velum/panic.h"
#include "cpu_int.h"

static uint64_t	g_unhandled[IDT_VECTORS / 64];

void	trap_unhandled(const t_regs *regs)
{
	uint64_t	vec;
	uint64_t	bit;

	vec = regs->vec & (IDT_VECTORS - 1);
	bit = 1ull << (vec & 63);
	if (g_unhandled[vec >> 6] & bit)
		return ;
	g_unhandled[vec >> 6] |= bit;
	klog_warn("cpu: vecteur %llu sans gestionnaire (rip %#llx)", vec,
		regs->rip);
}

static void	report_regs(const t_regs *r)
{
	kprintf("rsi=%#llx rdi=%#llx rbp=%#llx\n", r->rsi, r->rdi, r->rbp);
	kprintf("r8=%#llx r9=%#llx r10=%#llx r11=%#llx\n", r->r8, r->r9,
		r->r10, r->r11);
	kprintf("r12=%#llx r13=%#llx r14=%#llx r15=%#llx\n", r->r12, r->r13,
		r->r14, r->r15);
	kprintf("rflags=%#llx ss=%#llx\n", r->rflags, r->ss);
}

static void	report_pf(uint64_t err, uint64_t cr2)
{
	kprintf("faute de page : adresse %#llx, P=%llu W=%llu U=%llu R=%llu "
		"I=%llu\n", cr2, err & 1, (err >> 1) & 1, (err >> 2) & 1,
		(err >> 3) & 1, (err >> 4) & 1);
}

static void	report_trace(const t_regs *regs)
{
	uint64_t	pcs[BT_MAX];
	int			n;
	int			i;

	kprintf("trace : #0 %#llx\n", regs->rip);
	n = cpu_backtrace(regs->rbp, pcs, BT_MAX);
	i = 0;
	while (i < n)
	{
		kprintf("trace : #%d %#llx\n", i + 1, pcs[i]);
		i++;
	}
}

_Noreturn void	trap_kernel_fault(const t_regs *regs, uint64_t cr2)
{
	char	msg[96];

	kprintf("\nexception %s (%s), vecteur %llu, anneau %llu, pile %s\n",
		trap_name(regs->vec), trap_desc(regs->vec), regs->vec, regs->cs & 3,
		cpu_stack_name((uint64_t)regs));
	report_regs(regs);
	if (regs->vec == VEC_PAGE_FAULT)
		report_pf(regs->err, cr2);
	report_trace(regs);
	ksnprintf(msg, sizeof(msg), "%s %s (anneau %llu)", trap_name(regs->vec),
		trap_desc(regs->vec), regs->cs & 3);
	panic_regs(regs, msg);
}
