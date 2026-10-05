#include "velum/err.h"
#include "../common/layout.h"
#include "taskbar.h"

int	tb_regions(const t_tbmetrics *m, t_tbregions *out)
{
	int32_t	strip_x;
	int32_t	strip_w;

	if (!out || m->h <= 2 * TB_MARGIN_Y || m->start_w <= 0 || m->tray_w <= 0
		|| m->gap < 0 || m->gap > TB_SIDE_GAP_MAX)
		return (E_INVAL);
	strip_x = m->start_w + m->gap;
	strip_w = m->w - strip_x - m->tray_w;
	if (strip_w < 0)
		return (E_RANGE);
	out->start = lay_rect(0, 0, m->start_w, m->h);
	out->strip = lay_rect(strip_x, TB_MARGIN_Y, strip_w,
			m->h - 2 * TB_MARGIN_Y);
	out->tray = lay_rect(m->w - m->tray_w, 0, m->tray_w, m->h);
	return (0);
}

static int32_t	fit_count(int32_t width, uint32_t count)
{
	int32_t	shown;

	shown = (width + TB_GAP) / (TB_BTN_MIN_W + TB_GAP);
	if (shown > (int32_t)count)
		shown = (int32_t)count;
	if (shown > TASK_MAX)
		shown = TASK_MAX;
	return (shown);
}

int	tb_layout(t_rect strip, uint32_t count, t_rect *out)
{
	int32_t	shown;
	int32_t	base;
	int32_t	extra;
	int32_t	x;
	int32_t	i;

	shown = fit_count(strip.w, count);
	if (shown <= 0 || strip.h <= 0)
		return (0);
	base = (strip.w - (shown - 1) * TB_GAP) / shown;
	extra = (strip.w - (shown - 1) * TB_GAP) % shown;
	if (base >= TB_BTN_MAX_W)
	{
		base = TB_BTN_MAX_W;
		extra = 0;
	}
	x = strip.x;
	i = 0;
	while (i < shown)
	{
		out[i] = lay_rect(x, strip.y, base + (int32_t)(i < extra), strip.h);
		x += out[i].w + TB_GAP;
		i++;
	}
	return (shown);
}

int32_t	tb_hit(const t_rect *r, uint32_t n, int32_t x, int32_t y)
{
	uint32_t	i;

	i = 0;
	while (i < n)
	{
		if (lay_contains(r[i], x, y))
			return ((int32_t)i);
		i++;
	}
	return (-1);
}
