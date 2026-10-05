#include "luna_int.h"

static const t_lstop	g_thumb[] = {{0, 0xffd7e2fd}, {5, 0xffbad0fb},
{10, 0xffa8c0f5}, {16, 0xff94afef}};

static void	thumb_grips(t_surface *s, const t_rect *r, bool vert)
{
	int32_t	i;
	t_rect	c;

	i = -3;
	while (i <= 1)
	{
		c = lp_rect(r->x + 5, r->y + r->h / 2 + i, r->w - 10, 1);
		if (!vert)
			c = lp_rect(r->x + r->w / 2 + i, r->y + 5, 1, r->h - 10);
		lp_fill(s, c, 0xff8aa3e0);
		c.x += !vert;
		c.y += vert;
		lp_fill(s, c, 0xffeef3ff);
		i += 3;
	}
}

void	lp_sthumb(t_surface *s, const t_rect *r, t_lunastate st, bool vert)
{
	t_lbox	b;
	t_lgrad	g;
	t_color	ring;

	if (r->w <= 0 || r->h <= 0)
		return ;
	g.st = g_thumb;
	g.n = 4;
	g.tint = LC_WHITE;
	g.tint_t = 0;
	if (st == LS_HOT)
		g.tint_t = 45;
	ring = 0xff7b93d6;
	b.r = *r;
	b.rad = 2;
	b.ring = &ring;
	b.nring = 1;
	b.fill = &g;
	lp_box(s, &b);
	if (r->w >= 8 && r->h >= 8)
		thumb_grips(s, r, vert);
}
