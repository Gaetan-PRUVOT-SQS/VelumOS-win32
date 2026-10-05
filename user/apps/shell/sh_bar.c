#include "../common/uitext.h"
#include "shell.h"

static t_lunastate	start_state(const t_shell *sh)
{
	if (sh->sm.open)
		return (LS_PRESSED);
	if (sh->hot_start)
		return (LS_HOT);
	return (LS_NORMAL);
}

static void	paint_task(t_shell *sh, uint32_t i)
{
	const t_task	*t;
	t_rect			b;
	t_uitext		st;
	t_lunastate		state;
	int32_t			tx;

	t = &sh->tasks.list[i];
	b = sh->btn[i];
	state = LS_NORMAL;
	if (t->active)
		state = LS_PRESSED;
	else if (sh->hot_task == (int32_t)i)
		state = LS_HOT;
	luna_task_button(&sh->bar.surface, b, state);
	luna_icon(&sh->bar.surface, (t_point){b.x + 4, b.y + (b.h
			- sh->lm.icon_small) / 2}, (t_iconid)t->icon, sh->lm.icon_small);
	tx = b.x + 4 + sh->lm.icon_small + 4;
	st = (t_uitext){FONT_UI, {tx, b.y + (b.h - ui_font_height(FONT_UI)) / 2},
		UI_WHITE, b.x + b.w - 4 - tx};
	if (st.max_w > 8)
		ui_text(&sh->bar.surface, &st, t->title);
}

static void	paint_clock(t_shell *sh)
{
	t_uitext	st;
	t_rect		tray;

	tray = sh->reg.tray;
	st.font = FONT_UI;
	st.at.x = tray.x + (tray.w - ui_text_width(FONT_UI, sh->clock)) / 2;
	st.at.y = tray.y + (tray.h - ui_font_height(FONT_UI)) / 2;
	st.color = UI_BLACK;
	st.max_w = tray.w;
	ui_text(&sh->bar.surface, &st, sh->clock);
}

void	sh_bar_paint(t_shell *sh)
{
	t_surface	*s;
	uint32_t	i;

	s = &sh->bar.surface;
	luna_taskbar(s, rect_make(0, 0, s->w, s->h));
	luna_start_button(s, sh->reg.start, start_state(sh));
	i = 0;
	while (i < sh->shown)
	{
		paint_task(sh, i);
		i++;
	}
	luna_tray(s, sh->reg.tray);
	paint_clock(sh);
	wmc_present(&sh->bar, NULL, 0);
}
