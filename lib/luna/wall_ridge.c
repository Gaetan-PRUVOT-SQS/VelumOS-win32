#include "luna_int.h"

static const t_lhill	g_hills[LP_HILLS] = {
{53, 4, 640, 11, 60, 80, 4, 0xff92c673, 0xff6aa64e, 70},
{64, 5, 520, 31, 82, 55, 9, 0xff7ebf55, 0xff4e9c36, 25},
{78, 2, 300, 21, 28, 95, 26, 0xffaaeb5e, 0xff2f8a21, 0}};

t_chill	lp_wall_hill(int32_t i)
{
	return (&g_hills[i]);
}

static int32_t	hump(t_chill p, int32_t t1024)
{
	int32_t	dx;
	int32_t	bw;

	dx = t1024 - p->bc * 1024 / 100;
	bw = p->bw * 1024 / 100;
	if (dx <= -bw || dx >= bw)
		return (0);
	return (256 - dx * dx * 256 / (bw * bw));
}

int32_t	lp_wall_ridge(const t_lwall *c, int32_t u, int32_t i)
{
	t_chill	p;
	int32_t	x;
	int32_t	n;
	int32_t	h16;

	p = &g_hills[i];
	h16 = c->h * 16;
	x = (int32_t)((int64_t)u * p->freq / c->w) + 100 * i;
	n = lp_noise1(x, (uint32_t)p->seed) - 128;
	n += (lp_noise1(x * 3, (uint32_t)p->seed + 1) - 128) / 3;
	n = n * h16 * p->amp / (128 * 100);
	return (h16 * p->base / 100 + n - h16 * p->bh / 100
		* hump(p, (int32_t)((int64_t)u * 1024 / c->w)) / 256);
}
