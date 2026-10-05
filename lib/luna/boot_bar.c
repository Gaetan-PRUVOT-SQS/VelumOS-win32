#include "luna_int.h"

static const t_lstop	g_blk[] = {{0, 0xff9fcbff}, {3, 0xff3f7cf2},
{8, 0xff2a5fe0}, {14, 0xff1f4bc4}};

static void	bar_frame(t_surface *s, const t_rect *r)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	st;
	t_color	ring[2];

	ring[0] = 0xff6a7fb4;
	ring[1] = 0xff1c2540;
	lp_flat(&g, &st, LC_BLACK);
	b.r = *r;
	b.rad = 3;
	b.ring = ring;
	b.nring = 2;
	b.fill = &g;
	lp_box(s, &b);
}

static void	bar_blocks(t_surface *s, const t_rect *in, uint32_t tick)
{
	t_lbox	b;
	t_lgrad	g;
	int32_t	slots;
	int32_t	first;
	int32_t	i;

	g.st = g_blk;
	g.n = 4;
	g.tint = 0;
	g.tint_t = 0;
	b.rad = 2;
	b.ring = NULL;
	b.nring = 0;
	b.fill = &g;
	slots = (in->w + LP_BLK_GAP) / (in->h + LP_BLK_GAP);
	first = (int32_t)(tick % (uint32_t)(slots + 3)) - 3;
	i = lp_max(0, -first);
	while (i < 3 && first + i < slots)
	{
		b.r = lp_rect(in->x + (first + i) * (in->h + LP_BLK_GAP), in->y,
				in->h, in->h);
		lp_box(s, &b);
		i++;
	}
}

void	lp_boot_bar(t_surface *s, const t_rect *r, uint32_t pct, uint32_t tick)
{
	t_rect	in;

	bar_frame(s, r);
	in = lp_inset(*r, 3);
	if (in.w < 4 || in.h < 4)
		return ;
	bar_blocks(s, &in, tick);
	if (pct > 100)
		pct = 100;
	lp_fill(s, lp_rect(r->x, r->y + r->h + 3, r->w * (int32_t)pct / 100, 2),
		0xff2d6bef);
}
