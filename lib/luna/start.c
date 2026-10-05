#include "luna_int.h"

static const t_lstop	g_green[] = {{0, 0xff7fd66a}, {2, 0xff5bba47},
{8, 0xff46a836}, {16, 0xff3c9b2c}, {24, 0xff38902a}, {28, 0xff2f7d24}};

static void	start_text(t_surface *s, t_rect r)
{
	t_ltext	t;

	t.font = FONT_TITLE;
	t.color = LC_WHITE;
	t.shadow = 0xff1e5a18;
	t.text = "d\xc3\xa9marrer";
	t.box = lp_rect(r.x + 32, r.y, r.w - 33, r.h);
	t.align = LA_LEFT;
	lp_text(s, &t);
}

static void	start_grad(t_lunastate st, t_lgrad *g)
{
	g->st = g_green;
	g->n = 6;
	g->tint = LC_WHITE;
	g->tint_t = 0;
	if (st == LS_HOT)
		g->tint_t = 36;
	if (st == LS_PRESSED)
	{
		g->tint = LC_BLACK;
		g->tint_t = 52;
	}
}

void	luna_start_button(t_surface *s, t_rect r, t_lunastate st)
{
	t_llayers	l;
	t_lgrad		g;
	t_color		ring;
	int32_t		logo;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w <= 0 || r.h <= 0)
		return ;
	start_grad(st, &g);
	ring = 0xff2b6b20;
	l.r = r;
	l.rad[0] = 0;
	l.rad[1] = r.h / 3;
	l.rad[2] = r.h / 3;
	l.rad[3] = 0;
	l.ring = &ring;
	l.nring = 1;
	l.fill = &g;
	l.hole = lp_rect(0, 0, 0, 0);
	l.skip = 0;
	lp_layers(s, &l);
	logo = r.h - 8;
	lp_emblem(s, lp_rect(r.x + 8, r.y + 4, logo, logo));
	start_text(s, r);
}
