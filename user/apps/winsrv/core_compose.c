#include "ws_core.h"

static void	content(const t_wtable *t, int slot, t_surface *s)
{
	const t_wwin	*w;
	t_rect			cl;
	t_blit			b;

	w = &t->w[slot];
	cl = wh_client(t, slot);
	if (rect_empty(cl))
		return ;
	if (w->content.px == NULL || w->content.w < cl.w || w->content.h < cl.h)
		gfx_fill(s, cl, luna_color_window());
	if (w->content.px == NULL)
		return ;
	b.dst = s;
	b.src = &w->content;
	b.sr = rect_make(0, 0, wg_clamp(w->content.w, 0, cl.w),
			wg_clamp(w->content.h, 0, cl.h));
	b.dr = rect_make(cl.x, cl.y, b.sr.w, b.sr.h);
	gfx_blit(&b);
}

void	wc_window(const t_wtable *t, int slot, t_surface *s)
{
	t_lunawin	lw;

	if (wh_decorated(t->w[slot].style))
	{
		wh_lunawin(t, slot, &lw);
		luna_window_frame(s, &lw);
	}
	content(t, slot, s);
}

void	wc_compose(const t_wtable *t, t_surface *dst, t_wcursor c, t_rect d)
{
	t_surface	s;
	uint32_t	i;
	int			slot;

	s = *dst;
	gfx_set_clip(&s, d);
	if (rect_empty(s.clip))
		return ;
	gfx_fill(&s, s.clip, WS_DESK_COLOR);
	i = 0;
	while (i < t->nz)
	{
		slot = t->z[i];
		if (wf_visible(&t->w[slot])
			&& !rect_empty(rect_intersect(t->w[slot].rect, s.clip)))
			wc_window(t, slot, &s);
		i++;
	}
	wcur_draw(&s, c);
}
