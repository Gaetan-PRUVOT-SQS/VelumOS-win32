#include "luna_int.h"

static const t_lop	g_tick[] = {{LOP_LINE, 1, {7, 15, 11, 20, 5}, 0xff21a121,
	0xff21a121}, {LOP_LINE, 1, {11, 20, 20, 7, 5}, 0xff21a121, 0xff21a121}};
static const t_lop	g_dot[] = {{LOP_DISC, 0, {13, 13, 5}, 0xff21a121,
	0xff1b8f1b}};
static const t_lop	g_circle[] = {{LOP_DISC, 0, {13, 13, 13}, 0xff1c5180,
	0xff1c5180}, {LOP_DISC, 0, {13, 13, 12}, 0xffffffff, 0xffe3e2d8}};

static void	box_style(t_lunastate st, t_color *border, t_lgrad *g, t_lstop *s2)
{
	*border = 0xff1c5180;
	s2[0].at = 0;
	s2[0].c = 0xfffffffd;
	s2[1].at = 12;
	s2[1].c = 0xffe6e4da;
	if (st == LS_PRESSED)
	{
		s2[0].c = 0xffd5d3c8;
		s2[1].c = 0xffeeede6;
	}
	if (st == LS_DISABLED)
	{
		*border = 0xffc9c7ba;
		s2[0].c = 0xfff4f2e8;
		s2[1].c = 0xfff4f2e8;
	}
	g->st = s2;
	g->n = 2;
	g->tint = 0xfff8b330;
	g->tint_t = 0;
}

static void	check_box(t_surface *s, t_point p, t_lunastate st)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	s2[2];
	t_color	ring[2];

	box_style(st, &ring[0], &g, s2);
	ring[1] = 0xffffe8b0;
	b.nring = 1 + (st == LS_HOT);
	b.ring = ring;
	b.r = lp_rect(p.x, p.y, lm_detail()->check_size, lm_detail()->check_size);
	b.rad = 1;
	b.fill = &g;
	lp_box(s, &b);
}

void	luna_checkbox(t_surface *s, t_point p, t_lunastate st, bool on)
{
	t_lscale	sc;

	if (!lp_ok(s) || !lp_pt_ok(p))
		return ;
	check_box(s, p, st);
	sc.ox = p.x;
	sc.oy = p.y;
	sc.num = 1;
	sc.den = 2;
	if (on)
		lp_ops(s, &sc, g_tick, 2);
}

void	luna_radio(t_surface *s, t_point p, t_lunastate st, bool on)
{
	t_lscale	sc;
	t_lop		ops[2];

	if (!lp_ok(s) || !lp_pt_ok(p))
		return ;
	sc.ox = p.x;
	sc.oy = p.y;
	sc.num = 1;
	sc.den = 2;
	ops[0] = g_circle[0];
	ops[1] = g_circle[1];
	if (st == LS_DISABLED)
	{
		ops[0].c0 = 0xffc9c7ba;
		ops[1].c0 = 0xfff4f2e8;
		ops[1].c1 = 0xfff4f2e8;
	}
	if (st == LS_PRESSED)
		ops[1].c0 = 0xffd5d3c8;
	if (st == LS_HOT)
		ops[0].c0 = 0xfff8b330;
	lp_ops(s, &sc, ops, 2);
	if (on)
		lp_ops(s, &sc, g_dot, 1);
}
