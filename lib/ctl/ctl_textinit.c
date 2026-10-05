#include "ctl_int.h"

const t_font	*ctl_font(uint32_t flags)
{
	if (flags & CTL_BOLD)
		return (font_get(FONT_UI_BOLD));
	return (font_get(FONT_UI));
}

void	ctl_text_init(t_ctltext *tx, const t_ctl *c)
{
	tx->box = c->rect;
	tx->text = c->text;
	tx->flags = c->flags;
	tx->enabled = ctl_enabled(c);
	tx->color = ctl_color_text(tx->enabled);
	tx->mnemonic = true;
}

t_lunastate	ctl_lstate(const t_ctl *c)
{
	if (!ctl_enabled(c))
		return (LS_DISABLED);
	if (c->state & CTL_ST_PRESSED)
		return (LS_PRESSED);
	if (c->state & CTL_ST_HOT)
		return (LS_HOT);
	return (LS_NORMAL);
}
