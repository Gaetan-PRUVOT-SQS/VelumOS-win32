#include "luna_int.h"

static void	frame_layers(t_surface *s, const t_lgeo *g, const t_lfpal *pal)
{
	t_llayers	l;

	l.fill = pal->fill;
	l.ring = pal->ring;
	l.nring = 2;
	l.hole = g->client;
	l.skip = 0;
	l.r = g->outer;
	l.rad[0] = lm_detail()->title_radius;
	l.rad[1] = l.rad[0];
	l.rad[2] = 0;
	l.rad[3] = 0;
	if (g->flags & LG_MAX)
	{
		l.r = lp_rect(g->outer.x, g->outer.y, g->outer.w, g->cap_end);
		l.nring = 0;
		l.skip = 2;
		l.rad[0] = 0;
		l.rad[1] = 0;
		l.hole = lp_rect(0, 0, 0, 0);
	}
	lp_layers(s, &l);
}

static void	frame_title(t_surface *s, const t_lunawin *w, const t_lgeo *g,
				const t_lfpal *pal)
{
	t_ltext	t;

	if (w->title == NULL || g->title.w <= 0)
		return ;
	t.font = FONT_TITLE;
	if (g->flags & LG_TOOL)
		t.font = FONT_UI_BOLD;
	t.color = pal->text;
	t.shadow = pal->shadow;
	t.text = w->title;
	t.box = g->title;
	t.align = LA_LEFT;
	lp_text(s, &t);
}

static void	frame_buttons(t_surface *s, const t_lunawin *w, const t_lgeo *g)
{
	t_lcb	cb;
	int32_t	i;

	i = 0;
	while (i < LG_NBTN)
	{
		if (g->flags & (LG_SHOWN << i))
		{
			cb.r = g->btn[i];
			cb.kind = lg_btn_ht(i);
			cb.active = w->active;
			cb.maximized = w->maximized;
			cb.st = LS_NORMAL;
			if (!(g->flags & (LG_ENABLED << i)))
				cb.st = LS_DISABLED;
			else if (w->pressed == cb.kind)
				cb.st = LS_PRESSED;
			else if (w->hot == cb.kind)
				cb.st = LS_HOT;
			lp_capbtn(s, &cb);
		}
		i++;
	}
}

void	luna_window_frame(t_surface *s, const t_lunawin *w)
{
	t_lgeo	g;
	t_lfpal	pal;

	if (!lp_ok(s) || w == NULL)
		return ;
	lg_make(w, &g);
	if (!(g.flags & LG_FRAMED) || g.outer.w <= 0 || g.outer.h <= 0)
		return ;
	lp_frame_pal(w->active, &pal);
	frame_layers(s, &g, &pal);
	if (g.icon.w > 0 && w->icon != ICON_NONE)
		luna_icon(s, lp_pt(g.icon.x, g.icon.y), w->icon, g.icon.w);
	frame_title(s, w, &g, &pal);
	frame_buttons(s, w, &g);
}
