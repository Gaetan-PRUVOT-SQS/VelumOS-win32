#include "luna_int.h"

void	luna_menu_panel(t_surface *s, t_rect r)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	st;
	t_color	ring;

	r = lp_clean(r);
	if (!lp_ok(s))
		return ;
	ring = 0xffaca899;
	lp_flat(&g, &st, LC_WHITE);
	b.r = r;
	b.rad = 0;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
}

void	luna_menu_item(t_surface *s, t_rect r, t_lunastate st)
{
	if (!lp_ok(s))
		return ;
	r = lp_clean(r);
	if (st == LS_HOT || st == LS_PRESSED || st == LS_FOCUSED)
		lp_fill(s, r, LC_SELECTION);
}

void	luna_tooltip(t_surface *s, t_rect r)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	st;
	t_color	ring;

	r = lp_clean(r);
	if (!lp_ok(s))
		return ;
	ring = LC_BLACK;
	lp_flat(&g, &st, 0xffffffe1);
	b.r = r;
	b.rad = 0;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
}
