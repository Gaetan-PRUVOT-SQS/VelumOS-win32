#include "ctl_int.h"

void	ctl_btn_paint(t_ctlroot *r, t_ctl *c)
{
	t_lunabtn	b;
	t_ctltext	tx;

	b.r = c->rect;
	b.state = ctl_lstate(c);
	b.is_default = (c->flags & CTL_DEFAULT) != 0;
	b.focus = (r->focus == c);
	luna_button(r->surface, &b);
	ctl_text_init(&tx, c);
	tx.flags |= CTL_ALIGN_CENTER;
	tx.box.x += CTL_PAD;
	tx.box.w -= 2 * CTL_PAD;
	if (tx.box.w < 0)
		tx.box.w = 0;
	if (b.state == LS_PRESSED)
	{
		tx.box.x++;
		tx.box.y++;
	}
	ctl_text_draw(r->surface, &tx);
}

bool	ctl_btn_key(t_ctlroot *r, t_ctl *c, const t_inpevent *ev)
{
	if (ev->type != INP_KEY_DOWN)
		return (false);
	if (ev->code != VK_SPACE && ev->code != VK_RETURN)
		return (false);
	if (ev->mods & (INPM_ALT | INPM_CTRL))
		return (false);
	if (!(ev->mods & INPM_REPEAT))
		ctl_activate(r, c);
	return (true);
}

bool	ctl_btn_activate(t_ctlroot *r, t_ctl *c)
{
	ctl_notify(r, c, CN_CLICKED);
	return (true);
}
