#include "../common/uitext.h"
#include "shell.h"

void	sh_menu_geo(t_shell *sh)
{
	t_startlayout	ul;

	sm_layout(sh->lm.menu_item_h, sh->sm.level, &sh->sml);
	luna_startmenu_layout(sh->sml.panel, &ul);
	sm_set_footer(&sh->sml, ul.logoff, ul.poweroff);
}

static t_color	text_color(const t_smitem *it, int hot)
{
	if (hot)
		return (UI_WHITE);
	if (it->col == SMCOL_RIGHT)
		return (UI_MENU_TEXT_RIGHT);
	return (UI_BLACK);
}

static void	paint_item(t_shell *sh, uint32_t i)
{
	const t_smitem	*it;
	t_rect			r;
	t_uitext		st;
	int				hot;

	it = sm_item(i);
	r = sh->sml.item[i];
	hot = (sh->sm.hot == (int32_t)i);
	if (hot)
		luna_menu_item(&sh->menu.surface, r, LS_HOT);
	luna_icon(&sh->menu.surface, (t_point){r.x + 4, r.y + (r.h
			- sh->lm.icon_small) / 2}, (t_iconid)it->icon, sh->lm.icon_small);
	st = (t_uitext){FONT_UI, {r.x + 4 + sh->lm.icon_small + 6, r.y + (r.h
			- ui_font_height(FONT_UI)) / 2}, text_color(it, hot),
		r.w - sh->lm.icon_small - 24};
	ui_text(&sh->menu.surface, &st, it->label);
	if (it->cmd == SMC_OPEN_PROGRAMS)
	{
		st.at.x = r.x + r.w - 14;
		ui_text(&sh->menu.surface, &st, ">");
	}
}

static void	paint_footer_focus(t_shell *sh, uint32_t i)
{
	t_rect	r;

	if (sh->sm.hot != (int32_t)i)
		return ;
	r = sh->sml.item[i];
	gfx_frame(&sh->menu.surface, r, UI_WHITE);
	gfx_frame(&sh->menu.surface, rect_make(r.x + 1, r.y + 1, r.w - 2, r.h - 2),
		UI_WHITE);
}

void	sh_menu_paint(t_shell *sh)
{
	uint32_t	i;

	if (!sh->menu.id)
		return ;
	sh_menu_geo(sh);
	luna_startmenu_named(&sh->menu.surface, sh->sml.panel, sh->user);
	i = 0;
	while (i < SM_ITEMS)
	{
		if (sm_visible(i, sh->sm.level) && sm_item(i)->col == SMCOL_FOOT)
			paint_footer_focus(sh, i);
		else if (sm_visible(i, sh->sm.level))
			paint_item(sh, i);
		i++;
	}
	wmc_present(&sh->menu, NULL, 0);
}
