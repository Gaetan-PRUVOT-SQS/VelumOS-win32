#include "luna_int.h"

static const t_color	g_face[2][4] = {{0xffffffff, 0xfff1f0ea, 0xffe8e6dc,
	0xffd8d2c3}, {0xffcfcabc, 0xffdcd8cb, 0xffe6e3d8, 0xffedebe3}};

static int32_t	btn_rings(const t_lunabtn *b, t_color *ring)
{
	ring[0] = 0xff003c74;
	if (b->state == LS_DISABLED)
	{
		ring[0] = 0xffc9c7ba;
		return (1);
	}
	if (b->state == LS_HOT)
	{
		ring[1] = 0xfffff0cf;
		ring[2] = 0xfff8b330;
		return (3);
	}
	if (b->state == LS_DEFAULT || (b->is_default && b->state != LS_PRESSED))
	{
		ring[1] = 0xffa6c1f5;
		ring[2] = 0xff8aaaf0;
		return (3);
	}
	return (1);
}

static void	btn_fill(const t_lunabtn *b, t_lgrad *g, t_lstop *st)
{
	int32_t			k;
	int32_t			h;
	const t_color	*c;

	h = b->r.h;
	c = g_face[b->state == LS_PRESSED];
	st[0].at = 0;
	st[1].at = h * 55 / 100;
	st[2].at = h * 85 / 100;
	st[3].at = h - 1;
	k = 0;
	while (k < 4)
	{
		st[k].c = c[k];
		k++;
	}
	g->st = st;
	g->n = 4;
	g->tint = 0xfff4f2e8;
	g->tint_t = 0;
	if (b->state == LS_DISABLED)
		g->tint_t = 255;
}

void	luna_button(t_surface *s, const t_lunabtn *src)
{
	t_lunabtn	b;
	t_lbox		box;
	t_color		ring[3];
	t_lgrad		g;
	t_lstop		st[4];

	if (!lp_ok(s) || src == NULL)
		return ;
	b = *src;
	b.r = lp_clean(src->r);
	if (b.r.w <= 0 || b.r.h <= 0)
		return ;
	btn_fill(&b, &g, st);
	box.nring = btn_rings(&b, ring);
	box.ring = ring;
	box.r = b.r;
	box.rad = 3;
	box.fill = &g;
	lp_box(s, &box);
	if (b.focus && b.state != LS_DISABLED)
		lp_dotted(s, lp_inset(b.r, 4), LC_BLACK);
}
