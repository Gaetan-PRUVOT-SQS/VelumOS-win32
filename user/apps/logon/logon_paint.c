#include "velum/libk.h"
#include "../common/layout.h"
#include "../common/uitext.h"
#include "logon.h"
#include "logon_text.h"

#define LOGON_BORDER 0xff3b5ea8u

void	logon_overlay(void *user, t_rect dirty)
{
	t_logon		*lg;
	t_surface	*s;
	t_rect		card;
	uint32_t	i;

	(void)dirty;
	lg = user;
	s = &lg->ui.win.surface;
	card = lg->lay.card;
	gfx_frame(s, card, LOGON_BORDER);
	if (lg->flow.mode == LM_SHUTDOWN)
		return ;
	i = 0;
	while (i < lg->lay.count)
	{
		luna_icon(s, (t_point){card.x + lg->lay.tile[i].x + 4, card.y
			+ lg->lay.tile[i].y + (LOGON_TILE_H - LOGON_AVATAR) / 2},
			ICON_USER, LOGON_AVATAR);
		i++;
	}
}

static void	draw_prompt(t_logon *lg)
{
	t_uitext	st;
	t_rect		p;
	int32_t		lines;
	int32_t		fh;

	p = lg->lay.prompt;
	fh = ui_font_height(FONT_TITLE);
	lines = 1;
	if (ui_text_width(FONT_TITLE, LOGON_PROMPT) > p.w)
		lines = 2;
	st.font = FONT_TITLE;
	st.at = (t_point){p.x, p.y + p.h / 2 - fh};
	st.color = UI_WHITE;
	st.max_w = p.w;
	gfx_fill(&lg->ui.win.surface, rect_make(p.x - 14, st.at.y - 10, p.w + 28,
			lines * (fh + 4) + 16), UI_PROMPT_PLATE);
	ui_text_wrap(&lg->ui.win.surface, &st, LOGON_PROMPT);
}

static int	place(t_logon *lg)
{
	int32_t	w;
	int32_t	h;

	w = lg->ui.win.surface.w;
	h = lg->ui.win.surface.h;
	if (logon_layout(w, h, lg->nshown, &lg->lay) < 0)
		return (-1);
	return (uiwin_area(&lg->ui, lg->lay.card));
}

int	logon_window(t_logon *lg)
{
	t_wmcreate	rq;
	int			r;

	ui_request(&rq, rect_make(0, 0, (int32_t)lg->info.screen_w,
			(int32_t)lg->info.screen_h), WS_FULLSCREEN, "Ouverture de session");
	r = uiwin_open(&lg->ui, &rq, logon_on_command);
	if (r < 0)
		return (r);
	lg->ui.root.user = lg;
	lg->ui.overlay = logon_overlay;
	lg->ui.overlay_user = lg;
	luna_logon_bg(&lg->ui.win.surface, rect_make(0, 0,
			lg->ui.win.surface.w, lg->ui.win.surface.h));
	if (place(lg) < 0)
		return (-1);
	draw_prompt(lg);
	if (logon_build(lg) < 0)
		return (-1);
	wmc_present(&lg->ui.win, NULL, 0);
	return (0);
}
