#include "luna_int.h"

static const t_lstop	g_foot[] = {{0, 0xff5d90e4}, {1, 0xff3f78d8},
{18, 0xff2f67ca}, {37, 0xff2556b8}};

static void	foot_item(t_surface *s, t_rect item, t_iconid id, const char *txt)
{
	t_ltext	t;
	int32_t	ic;

	ic = lm_detail()->menu_foot_icon;
	luna_icon(s, lp_pt(item.x + 4, item.y + (item.h - ic) / 2), id, ic);
	t.font = FONT_UI;
	t.color = LC_WHITE;
	t.shadow = 0;
	t.align = LA_LEFT;
	t.text = txt;
	t.box = lp_rect(item.x + ic + 10, item.y, item.w - ic - 12, item.h);
	lp_text(s, &t);
}

void	lp_sm_footer(t_surface *s, const t_startlayout *lay)
{
	t_lgrad	g;

	g.st = g_foot;
	g.n = 4;
	g.tint = 0;
	g.tint_t = 0;
	lp_vfill(s, lp_rect(lay->footer.x + 1, lay->footer.y, lay->footer.w - 2,
			lay->footer.h - 1), &g);
	foot_item(s, lay->logoff, ICON_USER, "Fermer la session");
	foot_item(s, lay->poweroff, ICON_POWER, "\xc3\x89teindre");
}
