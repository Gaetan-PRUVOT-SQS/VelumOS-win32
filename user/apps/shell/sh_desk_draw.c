#include "velum/libk.h"
#include "../common/uitext.h"
#include "shell.h"

static size_t	break_at(const char *s, int32_t max_w)
{
	char	buf[SH_LABEL_MAX];
	size_t	i;
	size_t	best;

	best = 0;
	i = 0;
	while (s[i] && i < sizeof(buf) - 1)
	{
		if (s[i] == ' ')
		{
			memcpy(buf, s, i);
			buf[i] = '\0';
			if (ui_text_width(FONT_UI, buf) <= max_w)
				best = i;
		}
		i++;
	}
	return (best);
}

static void	draw_line(const t_cellctx *cx, int32_t y, const char *s)
{
	t_uitext	st;
	int32_t		w;

	w = ui_text_width(FONT_UI, s);
	if (w > cx->cell.w - 6)
		w = cx->cell.w - 6;
	st.font = FONT_UI;
	st.at = (t_point){cx->cell.x + (cx->cell.w - w) / 2, y};
	st.color = UI_WHITE;
	st.max_w = cx->cell.w - 6;
	gfx_fill(&cx->sh->desk.surface, rect_make(st.at.x - 2, y - 1, w + 4,
			ui_font_height(FONT_UI) + 2), UI_LABEL_PLATE);
	if (cx->selected)
		gfx_fill(&cx->sh->desk.surface, rect_make(st.at.x - 2, y - 1, w + 4,
				ui_font_height(FONT_UI) + 2), luna_color_selection());
	ui_text_shadow(&cx->sh->desk.surface, &st, s);
}

static void	draw_label(const t_cellctx *cx, const char *label)
{
	char	first[SH_LABEL_MAX];
	size_t	cut;
	int32_t	y;

	y = cx->cell.y + 6 + cx->sh->lm.icon_large + 4;
	cut = break_at(label, cx->cell.w - 6);
	if (cut == 0 || ui_text_width(FONT_UI, label) <= cx->cell.w - 6)
	{
		draw_line(cx, y, label);
		return ;
	}
	memcpy(first, label, cut);
	first[cut] = '\0';
	draw_line(cx, y, first);
	draw_line(cx, y + ui_font_height(FONT_UI) + 2, label + cut + 1);
}

void	sh_desk_cell_draw(t_shell *sh, int32_t idx)
{
	const t_deskicon	*ic;
	t_cellctx			cx;

	ic = desk_icon((uint32_t)idx);
	if (!ic || (uint32_t)idx >= sh->placed)
		return ;
	cx = (t_cellctx){sh, sh->cells[idx], sh->selected == idx};
	luna_icon(&sh->desk.surface, (t_point){cx.cell.x + (cx.cell.w
			- sh->lm.icon_large) / 2, cx.cell.y + 6}, (t_iconid)ic->icon,
		sh->lm.icon_large);
	draw_label(&cx, ic->label);
}

void	sh_desk_cell(t_shell *sh, int32_t idx)
{
	if (idx < 0 || (uint32_t)idx >= sh->placed)
		return ;
	sh_wall_blit(sh, sh->cells[idx]);
	sh_desk_cell_draw(sh, idx);
	wmc_present(&sh->desk, &sh->cells[idx], 1);
}
