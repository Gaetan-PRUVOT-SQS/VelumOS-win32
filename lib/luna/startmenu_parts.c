#include "luna_int.h"

void	lp_sm_frame(t_surface *s, t_rect r)
{
	t_llayers	l;
	t_lgrad		g;
	t_lstop		st;
	t_color		ring;

	ring = 0xff1a3fa6;
	lp_flat(&g, &st, LC_WHITE);
	l.r = r;
	l.rad[0] = 8;
	l.rad[1] = 8;
	l.rad[2] = 0;
	l.rad[3] = 0;
	l.ring = &ring;
	l.nring = 1;
	l.fill = &g;
	l.hole = lp_rect(0, 0, 0, 0);
	l.skip = 0;
	lp_layers(s, &l);
}

void	lp_sm_columns(t_surface *s, const t_startlayout *lay)
{
	lp_fill(s, lp_rect(lay->left.x + 1, lay->left.y, lay->left.w - 1,
			lay->left.h), LC_WHITE);
	lp_fill(s, lp_rect(lay->right.x, lay->right.y, lay->right.w - 1,
			lay->right.h), 0xffd3e5fa);
	lp_fill(s, lp_rect(lay->right.x, lay->right.y, 1, lay->right.h),
		0xffa7c5ec);
}
