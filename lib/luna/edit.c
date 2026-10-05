#include "luna_int.h"

void	luna_edit_frame(t_surface *s, t_rect r, bool enabled)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	st;
	t_color	ring;

	if (!lp_ok(s))
		return ;
	r = lp_clean(r);
	ring = 0xff7f9db9;
	lp_flat(&g, &st, LC_WHITE);
	if (!enabled)
	{
		ring = 0xffc9c7ba;
		lp_flat(&g, &st, LC_WINDOW);
	}
	b.r = r;
	b.rad = 0;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
}

static void	group_corners(t_surface *s, t_rect r, t_color c)
{
	t_lcring	k;

	k.rad = 3;
	k.c = c;
	k.x = r.x;
	k.y = r.y;
	k.dx = 1;
	k.dy = 1;
	lp_corner_ring(s, &k);
	k.x = r.x + r.w - 1;
	k.dx = -1;
	lp_corner_ring(s, &k);
	k.y = r.y + r.h - 1;
	k.dy = -1;
	lp_corner_ring(s, &k);
	k.x = r.x;
	k.dx = 1;
	lp_corner_ring(s, &k);
}

static void	group_top(t_surface *s, t_rect r, int32_t label_w, t_color c)
{
	int32_t	gap0;
	int32_t	gap1;

	gap0 = r.x + lm_detail()->group_pad;
	gap1 = gap0 + label_w;
	if (label_w <= 0)
		gap0 = r.x + r.w - 3;
	if (label_w <= 0)
		gap1 = gap0;
	lp_fill(s, lp_rect(r.x + 3, r.y, gap0 - r.x - 3, 1), c);
	lp_fill(s, lp_rect(gap1, r.y, r.x + r.w - 3 - gap1, 1), c);
}

void	luna_groupbox(t_surface *s, t_rect r, int32_t label_w)
{
	t_color	c;
	t_rect	box;

	r = lp_clean(r);
	label_w = lp_clamp(label_w, 0, LP_COORD_MAX);
	if (!lp_ok(s) || r.w < 8 || r.h < 8)
		return ;
	c = 0xffd0d0bf;
	box = lp_rect(r.x, r.y + lm_detail()->group_line_y, r.w,
			r.h - lm_detail()->group_line_y);
	if (box.h < 8)
		return ;
	group_corners(s, box, c);
	group_top(s, box, label_w, c);
	lp_fill(s, lp_rect(box.x + 3, box.y + box.h - 1, box.w - 6, 1), c);
	lp_fill(s, lp_rect(box.x, box.y + 3, 1, box.h - 6), c);
	lp_fill(s, lp_rect(box.x + box.w - 1, box.y + 3, 1, box.h - 6), c);
}
