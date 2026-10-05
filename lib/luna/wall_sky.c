#include "luna_int.h"

static const t_lstop	g_sky[4] = {{0, 0xff1457c7}, {90, 0xff3f8be6},
{170, 0xff7db8f2}, {255, 0xffc8e4fb}};

static int32_t	cloud_fbm(const t_lwall *c, int32_t u, int32_t v)
{
	int32_t	x;
	int32_t	y;

	x = (int32_t)((int64_t)u * 384 / c->h);
	y = (int32_t)((int64_t)v * 768 / c->h);
	return (lp_fbm(x, y, 5));
}

static int32_t	cloud_density(const t_lwall *c, int32_t n, int32_t v)
{
	int32_t	d;
	int32_t	alt;

	d = lp_clamp((n - 122) * 7, 0, 255);
	alt = lp_clamp(v * 255 / (c->h * 55 / 100), 0, 255);
	return (d * (255 - alt) / 255);
}

static t_color	cloud_color(const t_lwall *c, int32_t u, int32_t v, int32_t n1)
{
	int32_t	n2;
	int32_t	shade;

	n2 = cloud_fbm(c, u, v + c->h / 40 + 1);
	shade = lp_clamp((n1 - n2) * 9 + 15, 0, 150);
	return (lp_mix(LC_WHITE, 0xff9fb9e2, (uint32_t)shade));
}

t_color	lp_wall_sky(const t_lwall *c, int32_t u, int32_t v)
{
	t_lgrad	g;
	t_color	base;
	int32_t	n;
	int32_t	d;

	g.st = g_sky;
	g.n = 4;
	g.tint = 0;
	g.tint_t = 0;
	base = lp_grad_at(&g, lp_clamp(v * 255 / (c->h * 58 / 100), 0, 255));
	if (v >= c->h * 55 / 100)
		return (base);
	n = cloud_fbm(c, u, v);
	d = cloud_density(c, n, v);
	if (d == 0)
		return (base);
	return (lp_mix(base, cloud_color(c, u, v, n), (uint32_t)d));
}
