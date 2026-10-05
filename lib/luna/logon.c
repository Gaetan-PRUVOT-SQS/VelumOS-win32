#include "luna_int.h"

static t_color	logon_px(const t_lwall *c, int32_t u, int32_t v)
{
	int32_t	top;
	int32_t	bot;
	t_color	col;
	int32_t	fade;

	top = c->h * 15 / 100;
	bot = c->h - c->h * 12 / 100;
	if (v < top)
		return (0xff00309c);
	if (v >= bot)
		return (0xff003399);
	col = lp_mix(0xff82a4f0, 0xff4468cc, (uint32_t)(u * 255 / c->w));
	col = lp_mix(col, 0xff2d50b6, (uint32_t)((v - top) * 90
				/ lp_max(bot - top, 1)));
	fade = (c->w - u) * 190 / c->w;
	if (v - top < 2)
		col = lp_mix(col, LC_WHITE, (uint32_t)fade);
	if (bot - 1 - v < 2)
		col = lp_mix(col, 0xfff9a641, (uint32_t)fade);
	return (col);
}

void	luna_logon_bg(t_surface *s, t_rect r)
{
	t_lwall	c;
	int32_t	x;
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
		x = c.vis.x;
		while (x < c.vis.x + c.vis.w)
		{
			s->px[(size_t)y * (size_t)s->stride + (size_t)x]
				= logon_px(&c, lp_min(x - r.x, c.w - 1),
					lp_min(y - r.y, c.h - 1)) | 0xff000000;
			x++;
		}
		y++;
	}
}
