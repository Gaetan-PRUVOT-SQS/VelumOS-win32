#include "luna_int.h"

static t_color	wall_layer(t_color under, t_color over, int32_t d)
{
	if (d >= 8)
		return (over);
	return (lp_mix(under, over, (uint32_t)((d + 8) * 255 / 16)));
}

static bool	wall_hidden(const int32_t *r, int32_t v16, int32_t i)
{
	while (++i < LP_HILLS)
		if (v16 >= r[i] + 8)
			return (true);
	return (false);
}

static t_color	wall_pixel(const t_lwall *c, int32_t u, int32_t v)
{
	int32_t	r[LP_HILLS];
	int32_t	v16;
	int32_t	i;
	t_color	col;

	v16 = v * 16 + 8;
	i = -1;
	while (++i < LP_HILLS)
		r[i] = lp_wall_ridge(c, u, i);
	col = 0;
	if (v16 < lp_min(r[0], lp_min(r[1], r[2])) + 8)
		col = lp_wall_sky(c, u, v);
	i = -1;
	while (++i < LP_HILLS)
		if (v16 > r[i] - 8 && !wall_hidden(r, v16, i))
			col = wall_layer(col, lp_wall_hillcol(c, lp_pt(u, v), i, r[i]),
					v16 - r[i]);
	return (col | 0xff000000);
}

static void	wall_row(t_surface *s, const t_lwall *c, int32_t y)
{
	int32_t	x;

	x = c->vis.x;
	while (x < c->vis.x + c->vis.w)
	{
		s->px[(size_t)y * (size_t)s->stride + (size_t)x]
			= wall_pixel(c, lp_min(x - c->ox, c->w - 1),
				lp_min(y - c->oy, c->h - 1));
		x++;
	}
}

void	luna_wallpaper(t_surface *s, t_rect r)
{
	t_lwall	c;
	int32_t	y;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w <= 0 || r.h <= 0)
		return ;
	c.w = lp_clamp(r.w, 8, LP_WALL_MAX);
	c.h = lp_clamp(r.h, 8, LP_WALL_MAX);
	c.ox = r.x;
	c.oy = r.y;
	c.vis = lp_visible(s, r);
	y = c.vis.y;
	while (y < c.vis.y + c.vis.h)
	{
		wall_row(s, &c, y);
		y++;
	}
}
