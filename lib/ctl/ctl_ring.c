#include "ctl_int.h"

static void	dots_h(t_surface *s, t_point p, int32_t len, t_color col)
{
	int32_t	i;

	i = 0;
	while (i < len)
	{
		gfx_put(s, (t_point){p.x + i, p.y}, col);
		i += 2;
	}
}

static void	dots_v(t_surface *s, t_point p, int32_t len, t_color col)
{
	int32_t	i;

	i = 1;
	while (i < len)
	{
		gfx_put(s, (t_point){p.x, p.y + i}, col);
		i += 2;
	}
}

void	ctl_focus_ring(t_surface *s, t_rect r)
{
	t_color	col;

	if (r.w < 2 || r.h < 2)
		return ;
	col = luna_color_text();
	dots_h(s, (t_point){r.x, r.y}, r.w, col);
	dots_h(s, (t_point){r.x, r.y + r.h - 1}, r.w, col);
	dots_v(s, (t_point){r.x, r.y}, r.h - 1, col);
	dots_v(s, (t_point){r.x + r.w - 1, r.y}, r.h - 1, col);
}
