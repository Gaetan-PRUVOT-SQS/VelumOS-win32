#include "luna_int.h"

static t_color	grain(t_color c, int32_t u, int32_t v)
{
	int32_t	g;

	g = (int32_t)(lp_hash((uint32_t)u, (uint32_t)v, 77) & 15) - 8;
	if (g >= 0)
		return (lp_mix(c, LC_WHITE, (uint32_t)g * 2));
	return (lp_mix(c, LC_BLACK, (uint32_t)(-g) * 2));
}

static int32_t	hill_depth(const t_lwall *c, int32_t v, int32_t ridge)
{
	int32_t	span;

	span = lp_max(c->h - ridge / 16, 1);
	return (lp_clamp((v * 16 + 8 - ridge) / 16 * 255 / span, 0, 255));
}

t_color	lp_wall_hillcol(const t_lwall *c, t_point uv, int32_t i, int32_t ridge)
{
	t_chill	p;
	int32_t	t;
	int32_t	lit;
	t_color	col;

	p = lp_wall_hill(i);
	t = hill_depth(c, uv.y, ridge);
	col = lp_mix(p->top, p->bot, (uint32_t)(t * t / 255));
	lit = lp_clamp((c->w - uv.x) * 60 / c->w, 0, 60);
	col = lp_mix(col, 0xfff0ffb0, (uint32_t)lit / 3);
	if (p->haze > 0)
		col = lp_mix(col, 0xffbfdcf0, (uint32_t)(p->haze * (255 - t) / 255));
	if (i == LP_HILLS - 1)
		col = lp_mix(col, LC_WHITE, (uint32_t)(lp_noise2(uv.x * 3, uv.y * 40,
						9) / 24));
	return (grain(col, uv.x, uv.y));
}
