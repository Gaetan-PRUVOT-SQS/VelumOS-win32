#include "luna_int.h"

static const t_lstop	g_norm[] = {{0, 0xff5b96f8}, {8, 0xff3e80f2},
{20, 0xff3074ea}, {25, 0xff2c6ae0}};
static const t_lstop	g_down[] = {{0, 0xff1a48a8}, {6, 0xff1f55bc},
{23, 0xff2a62cc}};

static t_color	task_style(t_lunastate st, t_lgrad *g)
{
	t_color	ring;

	g->st = g_norm;
	g->n = 4;
	g->tint = 0;
	g->tint_t = 0;
	ring = 0xff1e4fc0;
	if (st == LS_PRESSED || st == LS_DEFAULT || st == LS_FOCUSED)
	{
		g->st = g_down;
		g->n = 3;
		ring = 0xff153c94;
	}
	if (st == LS_HOT)
	{
		g->tint = LC_WHITE;
		g->tint_t = 34;
	}
	return (ring);
}

void	luna_task_button(t_surface *s, t_rect r, t_lunastate st)
{
	t_lbox	b;
	t_lgrad	g;
	t_color	ring;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w <= 0 || r.h <= 0)
		return ;
	ring = task_style(st, &g);
	b.r = r;
	b.rad = 3;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
}
