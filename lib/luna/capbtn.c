#include "luna_int.h"

static const t_lstop	g_blue_st[] = {{0, 0xff5b97fa}, {5, 0xff3c80f5},
{11, 0xff2a6bec}, {18, 0xff1c57da}, {19, 0xff1a4fcf}};
static const t_lstop	g_red_st[] = {{0, 0xfff0907a}, {5, 0xffe4694b},
{11, 0xffd9502e}, {18, 0xffc03c1c}, {19, 0xffb03213}};
static const t_lstop	g_off_st[] = {{0, 0xffc9d6f5}, {7, 0xffaabeee},
{19, 0xff8ea5df}};
static const t_lstop	g_dis_st[] = {{0, 0xffbfcbed}, {19, 0xffa3b4e3}};
static const t_lgrad	g_blue = {g_blue_st, 5, 0, 0};
static const t_lgrad	g_red = {g_red_st, 5, 0, 0};
static const t_lgrad	g_off = {g_off_st, 3, 0, 0};
static const t_lgrad	g_dis = {g_dis_st, 2, 0, 0};

static void	capbtn_grad(const t_lcb *cb, t_lgrad *g)
{
	*g = g_blue;
	if (cb->st == LS_DISABLED)
		*g = g_dis;
	else if (!cb->active)
		*g = g_off;
	else if (cb->kind == HT_CLOSE)
		*g = g_red;
	if (cb->st == LS_HOT)
	{
		g->tint = LC_WHITE;
		g->tint_t = 56;
	}
	if (cb->st == LS_PRESSED)
	{
		g->tint = LC_BLACK;
		g->tint_t = 64;
	}
}

void	lp_capbtn(t_surface *s, const t_lcb *cb)
{
	t_lbox	b;
	t_lgrad	g;
	t_color	ring;

	capbtn_grad(cb, &g);
	ring = LC_WHITE;
	if (!cb->active || cb->st == LS_DISABLED)
		ring = 0xffe4eaf9;
	b.r = cb->r;
	b.rad = cb->r.w / 7;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
	lp_capglyph(s, cb);
}
