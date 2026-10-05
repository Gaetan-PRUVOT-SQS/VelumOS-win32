#include "luna_int.h"

static const t_lstop	g_head[] = {{0, 0xff3a77e6}, {6, 0xff2a66dc},
{40, 0xff1d58cf}, {54, 0xff1646b4}};
static const t_lstop	g_tile[] = {{0, 0xffa9c9f8}, {47, 0xff5a8ae4}};

static void	sm_avatar(t_surface *s, t_rect a)
{
	t_lbox	b;
	t_lgrad	g;
	t_color	ring[2];

	ring[0] = LC_WHITE;
	ring[1] = 0xffc3d6f4;
	g.st = g_tile;
	g.n = 2;
	g.tint = 0;
	g.tint_t = 0;
	b.r = a;
	b.rad = 5;
	b.ring = ring;
	b.nring = 2;
	b.fill = &g;
	lp_box(s, &b);
	luna_icon(s, lp_pt(a.x + 3, a.y + 3), ICON_USER, a.w - 6);
}

static void	sm_title(t_surface *s, const t_startlayout *lay, const char *user)
{
	t_ltext	t;

	if (user == NULL)
		return ;
	t.font = FONT_TITLE;
	t.color = LC_WHITE;
	t.shadow = 0xff0f2f7a;
	t.text = user;
	t.box = lp_rect(lay->avatar.x + lay->avatar.w + 10, lay->header.y,
			lay->header.w - lay->avatar.w - 20, lay->header.h);
	t.align = LA_LEFT;
	lp_text(s, &t);
}

void	lp_sm_header(t_surface *s, const t_startlayout *lay, const char *user)
{
	t_llayers	l;
	t_lgrad		g;

	g.st = g_head;
	g.n = 4;
	g.tint = 0;
	g.tint_t = 0;
	l.r = lp_rect(lay->header.x + 1, lay->header.y + 1, lay->header.w - 2,
			lay->header.h - 1);
	l.rad[0] = 7;
	l.rad[1] = 7;
	l.rad[2] = 0;
	l.rad[3] = 0;
	l.nring = 0;
	l.fill = &g;
	l.hole = lp_rect(0, 0, 0, 0);
	l.skip = 0;
	lp_layers(s, &l);
	sm_avatar(s, lay->avatar);
	sm_title(s, lay, user);
}
