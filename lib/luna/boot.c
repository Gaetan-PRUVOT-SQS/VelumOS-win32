#include "luna_int.h"

static void	boot_layout(const t_surface *s, t_lboot *b)
{
	int32_t	e;
	int32_t	mh;
	int32_t	bw;
	int32_t	bh;

	e = lp_clamp(lp_min(s->w, s->h) / 4, 32, 192);
	mh = lp_max(e / 5, 8);
	bh = lp_clamp(e / 9, 10, 18);
	bw = lp_clamp(s->w * 26 / 100, 96, 320);
	b->emblem = lp_rect((s->w - e) / 2, s->h * 34 / 100 - e / 2, e, e);
	b->mark = lp_rect((s->w - lp_wordmark_w(mh)) / 2,
			b->emblem.y + e + e / 6, lp_wordmark_w(mh), mh);
	b->bar = lp_rect((s->w - bw) / 2, s->h * 68 / 100, bw, bh);
	b->text = lp_rect(0, b->bar.y + bh + 14, s->w, 14);
}

static void	boot_text(t_surface *s, const t_lboot *b)
{
	t_ltext	t;

	t.font = FONT_UI;
	t.color = 0xff7f8ba8;
	t.shadow = 0;
	t.text = "D\xc3\xa9marrage en cours";
	t.box = b->text;
	t.align = LA_CENTER;
	lp_text(s, &t);
}

void	luna_boot_draw(t_surface *s, uint32_t pct, uint32_t tick)
{
	t_lboot	b;

	if (!lp_ok(s))
		return ;
	lp_fill(s, lp_rect(0, 0, s->w, s->h), LC_BLACK);
	boot_layout(s, &b);
	lp_emblem(s, b.emblem);
	lp_wordmark(s, b.mark, LC_WHITE);
	lp_boot_bar(s, &b.bar, pct, tick);
	boot_text(s, &b);
}
