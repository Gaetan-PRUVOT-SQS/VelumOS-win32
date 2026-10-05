#include "luna_int.h"

static const t_lop	g_min[] = {{LOP_RECT, 0, {6, 13, 14, 16}, 0, 0}};
static const t_lop	g_max[] = {{LOP_RECT, 0, {5, 5, 15, 8}, 0, 0},
{LOP_RECT, 0, {5, 8, 7, 15}, 0, 0}, {LOP_RECT, 0, {13, 8, 15, 15}, 0, 0},
{LOP_RECT, 0, {7, 13, 13, 15}, 0, 0}};
static const t_lop	g_restore[] = {{LOP_RECT, 0, {8, 4, 16, 6}, 0, 0},
{LOP_RECT, 0, {14, 6, 16, 12}, 0, 0}, {LOP_RECT, 0, {5, 8, 13, 11}, 0, 0},
{LOP_RECT, 0, {5, 11, 7, 16}, 0, 0}, {LOP_RECT, 0, {11, 11, 13, 16}, 0, 0},
{LOP_RECT, 0, {7, 14, 11, 16}, 0, 0}};
static const t_lop	g_close[] = {{LOP_LINE, 0, {6, 6, 15, 15, 5}, 0, 0},
{LOP_LINE, 0, {15, 6, 6, 15, 5}, 0, 0}};

static t_loplist	glyph_list(const t_lcb *cb)
{
	t_loplist	l;

	l.ops = g_close;
	l.n = sizeof(g_close) / sizeof(g_close[0]);
	if (cb->kind == HT_MINBUTTON)
	{
		l.ops = g_min;
		l.n = sizeof(g_min) / sizeof(g_min[0]);
	}
	else if (cb->kind == HT_MAXBUTTON && !cb->maximized)
	{
		l.ops = g_max;
		l.n = sizeof(g_max) / sizeof(g_max[0]);
	}
	else if (cb->kind == HT_MAXBUTTON)
	{
		l.ops = g_restore;
		l.n = sizeof(g_restore) / sizeof(g_restore[0]);
	}
	return (l);
}

static t_color	glyph_color(const t_lcb *cb)
{
	if (cb->st == LS_DISABLED)
		return (0xffc3cfee);
	if (!cb->active)
		return (0xfff0f4ff);
	return (LC_WHITE);
}

void	lp_capglyph(t_surface *s, const t_lcb *cb)
{
	t_lscale	sc;
	t_loplist	l;

	l = glyph_list(cb);
	sc.ox = cb->r.x + 1;
	sc.oy = cb->r.y + 1;
	sc.num = cb->r.w;
	sc.den = 21;
	l.flat = 0x60001060;
	lp_ops_flat(s, &sc, &l);
	sc.ox = cb->r.x;
	sc.oy = cb->r.y;
	l.flat = glyph_color(cb);
	lp_ops_flat(s, &sc, &l);
}
