#include "fake.h"

static bool	ring_dots(const t_fakeui *u, t_rect row)
{
	return (fake_px_count(&u->s, row, 0xff000000) > 0);
}

static bool	button_focus(const t_ctl *c)
{
	int			i;
	t_fakecall	*k;

	i = fake_log_count() - 1;
	while (i >= 0)
	{
		k = fake_log_get(i);
		if (k->kind == FK_BUTTON && fake_rect_eq(k->r, c->rect))
			return ((k->b & 2) != 0);
		i--;
	}
	return (false);
}

static bool	edit_caret(const t_fakeui *u, const t_ctl *c)
{
	t_rect	col;

	col = rect_make(c->rect.x + 3, c->rect.y + (c->rect.h - 11) / 2, 1, 11);
	return (fake_px_count(&u->s, col, 0xff000000) == 11);
}

bool	fake_focus_visible(t_fakeui *u, const t_ctl *c)
{
	t_rect	row;

	ctl_invalidate(&u->r, u->r.root);
	fake_log_clear();
	ctl_paint(&u->r);
	row = rect_make(c->rect.x + 15, c->rect.y + (c->rect.h - 13) / 2, 40, 1);
	if (c->type == CT_BUTTON)
		return (button_focus(c));
	if (c->type == CT_EDIT)
		return (edit_caret(u, c));
	if (c->type == CT_LIST)
		return (ring_dots(u, rect_make(c->rect.x + 2, c->rect.y + 2, 60, 1)));
	if (c->type == CT_SCROLL)
		return (ring_dots(u, rect_make(c->rect.x + 1, c->rect.y + 1, 15, 1)));
	return (ring_dots(u, row));
}
