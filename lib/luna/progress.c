#include "luna_int.h"

static const t_lstop	g_block[] = {{0, 0xffc4f6bd}, {2, 0xff6fe46a},
{4, 0xff2fd22f}, {7, 0xff1fae20}, {10, 0xff188f1a}};

static void	progress_frame(t_surface *s, t_rect r)
{
	t_lbox	b;
	t_lgrad	g;
	t_lstop	st;
	t_color	ring[2];

	ring[0] = 0xff8f9bb0;
	ring[1] = 0xffe6ebf5;
	lp_flat(&g, &st, LC_WHITE);
	b.r = r;
	b.rad = 2;
	b.ring = ring;
	b.nring = 2;
	b.fill = &g;
	lp_box(s, &b);
}

static void	progress_blocks(t_surface *s, const t_rect *in, int32_t count,
				int32_t n)
{
	t_lgrad	g;
	int32_t	step;
	int32_t	i;

	g.st = g_block;
	g.n = 5;
	g.tint = 0;
	g.tint_t = 0;
	step = lm_detail()->progress_block + lm_detail()->progress_gap;
	i = 0;
	while (i < n && i < count)
	{
		lp_vfill(s, lp_rect(in->x + i * step, in->y,
				lm_detail()->progress_block, in->h), &g);
		i++;
	}
}

void	luna_progress(t_surface *s, t_rect r, uint32_t pct)
{
	t_rect	in;
	int32_t	count;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w < 8 || r.h < 8)
		return ;
	progress_frame(s, r);
	in = lp_inset(r, 3);
	count = (in.w + lm_detail()->progress_gap) / (lm_detail()->progress_block
			+ lm_detail()->progress_gap);
	if (pct > 100)
		pct = 100;
	progress_blocks(s, &in, count, count * (int32_t)pct / 100);
}
