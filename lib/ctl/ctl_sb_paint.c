#include "ctl_int.h"

static void	paint_part(t_surface *s, t_rect r, t_lunastate st)
{
	t_lunabtn	b;

	b.r = r;
	b.state = st;
	b.is_default = false;
	b.focus = false;
	luna_button(s, &b);
}

static t_lunastate	part_state(const t_scrst *st, uint32_t zone, bool live)
{
	if (!live)
		return (LS_DISABLED);
	if (st->zone == zone)
		return (LS_PRESSED);
	return (LS_NORMAL);
}

void	ctl_sb_paint(t_surface *s, t_rect bar, const t_scrst *st, bool en)
{
	t_sblo	lo;
	t_color	col;
	bool	live;

	ctl_sb_layout(bar, st, &lo);
	live = en && st->max > 0;
	col = ctl_color_text(live);
	gfx_fill(s, lo.track, ctl_color_track());
	paint_part(s, lo.dec, part_state(st, CTL_SBZ_DEC, live));
	paint_part(s, lo.inc, part_state(st, CTL_SBZ_INC, live));
	ctl_sb_arrow(s, lo.dec, !lo.vertical * 2, col);
	ctl_sb_arrow(s, lo.inc, !lo.vertical * 2 + 1, col);
	if (!rect_empty(lo.thumb))
		paint_part(s, lo.thumb, part_state(st, CTL_SBZ_THUMB, live));
}
