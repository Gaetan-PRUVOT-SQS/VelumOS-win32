#include "fake.h"

bool	vmm_query(t_aspace *as, uintptr_t va, t_vminfo *out)
{
	uint32_t	i;
	uint64_t	page;

	page = va & ~(uint64_t)(PAGE_SIZE - 1);
	i = 0;
	while (i < FK_MAPS)
	{
		if (g_fk.maps[i].used && g_fk.maps[i].as == (void *)as
			&& g_fk.maps[i].va == page)
		{
			out->pa = g_fk.maps[i].pa;
			out->flags = g_fk.maps[i].flags;
			return (true);
		}
		i++;
	}
	return (false);
}

uint32_t	fk_count_maps(void *as)
{
	uint32_t	i;
	uint32_t	n;

	i = 0;
	n = 0;
	while (i < FK_MAPS)
	{
		if (g_fk.maps[i].used && (!as || g_fk.maps[i].as == as))
			n++;
		i++;
	}
	return (n);
}

uint64_t	fk_uptr(const void *p)
{
	return ((uint64_t)(uintptr_t)p);
}
