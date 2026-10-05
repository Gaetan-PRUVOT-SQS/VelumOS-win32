#include "luna_int.h"

static const t_lunametrics	g_metrics = {30, 4, 4, 21, 21, 30, 100, 80, 17,
	22, 16, 32};
static const t_lunadetail	g_detail = {2, 5, 5, 2, 3, 22, 4, 16, 22, 15, 5, 5,
	13, 6, 8, 8, 2, 54, 38, 150, 24, 48, 3, 8, 8};

void	luna_metrics(t_lunametrics *out)
{
	if (out)
		*out = g_metrics;
}

t_cmet	lm_metrics(void)
{
	return (&g_metrics);
}

t_cdet	lm_detail(void)
{
	return (&g_detail);
}
