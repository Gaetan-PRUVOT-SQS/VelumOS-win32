#include "luna_int.h"

static const t_lstop	g_face[] = {{0, 0xffd2defc}, {6, 0xffbdd0fa},
{16, 0xffa5bcf3}};
static const t_lstop	g_trackst[] = {{0, 0xffebe9e0}, {16, 0xfffdfdfa}};

static const t_lstop	g_dis[] = {{0, 0xfff4f3ec}, {16, 0xffe3e2d8}};

static t_color	sc_style(t_lunastate st, t_lgrad *g)
{
	g->st = g_face;
	g->n = 3;
	g->tint = 0;
	g->tint_t = 0;
	if (st == LS_HOT)
	{
		g->tint = LC_WHITE;
		g->tint_t = 50;
	}
	if (st == LS_PRESSED)
	{
		g->tint = 0xff3b5bb8;
		g->tint_t = 70;
	}
	if (st == LS_DISABLED)
	{
		g->st = g_dis;
		g->n = 2;
	}
	if (st == LS_DISABLED)
		return (0xffd2d0c4);
	return (0xff8da6e4);
}

static void	sc_button(t_surface *s, t_rect r, t_lunastate st, int32_t dir)
{
	t_lbox	b;
	t_lgrad	g;
	t_color	ring;
	t_color	arrow;

	ring = sc_style(st, &g);
	arrow = 0xff4d6185;
	if (st == LS_DISABLED)
		arrow = 0xffc3c1b3;
	b.r = r;
	b.rad = 2;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
	lp_arrow(s, r, dir, arrow);
}

void	luna_scrollbar_ex(t_surface *s, const t_lunascroll *src)
{
	t_lunascroll	sb;
	t_scrollparts	p;
	t_lgrad			g;
	bool			vert;

	if (!lp_ok(s) || src == NULL)
		return ;
	sb = *src;
	sb.r = lp_clean(src->r);
	if (sb.r.w <= 0 || sb.r.h <= 0)
		return ;
	luna_scroll_layout(&sb, &p);
	vert = sb.r.h >= sb.r.w;
	g.st = g_trackst;
	g.n = 2;
	g.tint = 0;
	g.tint_t = 0;
	if (vert)
		lp_hfill(s, p.track, &g);
	else
		lp_vfill(s, p.track, &g);
	sc_button(s, p.up, sb.up, 2 - 2 * vert);
	sc_button(s, p.down, sb.down, 3 - 2 * vert);
	lp_sthumb(s, &p.thumb, sb.thumb, vert);
}

void	luna_scrollbar(t_surface *s, t_rect r, t_lunastate st)
{
	t_lunascroll	sb;

	sb.r = r;
	sb.total = 2;
	sb.page = 1;
	sb.pos = 0;
	sb.up = LS_NORMAL;
	sb.down = LS_NORMAL;
	sb.thumb = st;
	if (st == LS_DISABLED)
	{
		sb.total = 0;
		sb.up = LS_DISABLED;
		sb.down = LS_DISABLED;
	}
	luna_scrollbar_ex(s, &sb);
}
