#include "velum/err.h"
#include "velum/libk.h"
#include "../common/layout.h"
#include "startmenu.h"

static t_rect	slot_rect(const t_smlayout *l, const t_smitem *it,
					int32_t row_h)
{
	int32_t	half;
	int32_t	item;

	half = SM_WIDTH / 2;
	if (it->col == SMCOL_LEFT)
		return (lay_rect(l->left.x + SM_PAD,
				l->left.y + SM_PAD + (int32_t)it->slot * row_h,
				half - 2 * SM_PAD, row_h));
	if (it->col == SMCOL_RIGHT)
		return (lay_rect(l->right.x + SM_PAD,
				l->right.y + SM_PAD + (int32_t)it->slot * row_h,
				half - 2 * SM_PAD, row_h));
	item = lay_clamp((SM_WIDTH - 2 * SM_FOOT_MARGIN) / 2, 0, SM_FOOT_ITEM_W);
	return (lay_rect(SM_WIDTH - SM_FOOT_MARGIN - item
			- (1 - (int32_t)it->slot) * item, l->foot.y, item, SM_FOOT_H));
}

int	sm_layout(int32_t item_h, uint32_t level, t_smlayout *out)
{
	int32_t		row_h;
	int32_t		body_h;
	uint32_t	i;

	if (!out || item_h < SM_ITEM_H_MIN || item_h > SM_ITEM_H_MAX)
		return (E_RANGE);
	memset(out, 0, sizeof(*out));
	row_h = item_h + 8;
	body_h = SM_ROWS * row_h + 2 * SM_PAD;
	out->panel = lay_rect(0, 0, SM_WIDTH, SM_HEADER_H + body_h + SM_FOOT_H);
	out->header = lay_rect(0, 0, SM_WIDTH, SM_HEADER_H);
	out->left = lay_rect(0, SM_HEADER_H, SM_WIDTH / 2, body_h);
	out->right = lay_rect(SM_WIDTH / 2, SM_HEADER_H, SM_WIDTH / 2, body_h);
	out->foot = lay_rect(0, SM_HEADER_H + body_h, SM_WIDTH, SM_FOOT_H);
	i = 0;
	while (i < SM_ITEMS)
	{
		if (sm_visible(i, level))
			out->item[i] = slot_rect(out, sm_item(i), row_h);
		i++;
	}
	return (0);
}

int32_t	sm_hit(const t_smlayout *l, uint32_t level, int32_t x, int32_t y)
{
	uint32_t	i;

	i = 0;
	while (i < SM_ITEMS)
	{
		if (sm_visible(i, level) && lay_contains(l->item[i], x, y))
			return ((int32_t)i);
		i++;
	}
	return (SM_NO_ITEM);
}

int	sm_set_footer(t_smlayout *l, t_rect logoff, t_rect poweroff)
{
	if (!lay_inside(l->panel, logoff) || !lay_inside(l->panel, poweroff)
		|| lay_overlap(logoff, poweroff))
		return (E_INVAL);
	l->item[SM_IDX_LOGOFF] = logoff;
	l->item[SM_IDX_SHUTDOWN] = poweroff;
	return (0);
}

t_rect	sm_place(int32_t screen_h, int32_t bar_h, const t_smlayout *l)
{
	int32_t	y;

	y = lay_clamp(screen_h - bar_h - l->panel.h, 0, screen_h);
	return (lay_rect(0, y, l->panel.w, l->panel.h));
}
