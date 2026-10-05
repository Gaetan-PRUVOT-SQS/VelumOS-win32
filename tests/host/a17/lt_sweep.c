#include "lt_hit.h"

static void	ft_zero_counts(t_ltsweep *out)
{
	int	i;

	i = 0;
	while (i < 21)
	{
		out->count[i] = 0;
		i++;
	}
	out->bad = 0;
	out->bad_x = 0;
	out->bad_y = 0;
}

static void	sweep_note(t_ltsweep *out, t_point p, bool ok)
{
	if (ok)
		return ;
	if (out->bad == 0)
	{
		out->bad_x = p.x;
		out->bad_y = p.y;
	}
	out->bad++;
}

static bool	sweep_check(const t_lunawin *w, t_rect cl, t_point p, uint32_t h)
{
	t_point	m;
	bool	framed;

	framed = (w->style & WS_CAPTION) && !(w->style & 0x0e00);
	if (!lt_ht_valid(h))
		return (false);
	if (lt_ht_edge(h))
	{
		m = lp_pt(w->outer.x + w->outer.w - 1 - (p.x - w->outer.x), p.y);
		if (lt_ht_mirror(h) != luna_hit_test(w, m))
			return (false);
		if (!(w->style & WS_SIZEBOX)
			|| (w->maximized && !(w->style & WS_TOOLWINDOW)))
			return (false);
	}
	if (lp_in(cl, p) != (h == HT_CLIENT))
		return (false);
	return (framed || h == HT_CLIENT);
}

static bool	sweep_visit(const t_lunawin *w, t_point p, int32_t step)
{
	int32_t	dx;
	int32_t	dy;
	bool	near_x;
	bool	near_y;

	dx = p.x - w->outer.x;
	dy = p.y - w->outer.y;
	if (step <= 1)
		return (true);
	near_x = dx < LT_BAND_X || dx >= w->outer.w - LT_BAND_BTN;
	near_y = dy < LT_BAND || dy >= w->outer.h - LT_BAND;
	if (near_x && near_y)
		return (true);
	if (near_x)
		return (dy % step == 0);
	if (near_y)
		return (dx % step == 0);
	return (dx % step == 0 && dy % step == 0);
}

void	lt_sweep(const t_lunawin *w, int32_t step, t_ltsweep *out)
{
	t_rect		cl;
	t_point		p;
	uint32_t	h;

	ft_zero_counts(out);
	cl = luna_window_client(w);
	p.y = w->outer.y;
	while (p.y < w->outer.y + w->outer.h)
	{
		p.x = w->outer.x;
		while (p.x < w->outer.x + w->outer.w)
		{
			if (sweep_visit(w, p, step))
			{
				h = luna_hit_test(w, p);
				if (h < 21)
					out->count[h]++;
				sweep_note(out, p, h < 21 && sweep_check(w, cl, p, h));
			}
			p.x++;
		}
		p.y++;
	}
}
