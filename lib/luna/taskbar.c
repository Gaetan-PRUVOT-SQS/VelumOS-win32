#include "luna_int.h"

static void	bar_stops(int32_t h, t_lstop *st)
{
	static const t_color	col[8] = {0xff5a8df0, 0xff3d79e8, 0xff2f6be0,
		0xff2862dc, 0xff245edc, 0xff2058d2, 0xff1b4fc2, 0xff1544a4};
	int32_t					k;

	st[0].at = 0;
	st[1].at = 1;
	st[2].at = 3;
	st[3].at = 7;
	st[4].at = lp_max(h - 12, 8);
	st[5].at = lp_max(h - 4, 9);
	st[6].at = lp_max(h - 2, 10);
	st[7].at = lp_max(h - 1, 11);
	k = 0;
	while (k < 8)
	{
		st[k].c = col[k];
		k++;
	}
}

void	luna_taskbar(t_surface *s, t_rect r)
{
	t_lstop	st[8];
	t_lgrad	g;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w <= 0 || r.h <= 0)
		return ;
	bar_stops(r.h, st);
	g.st = st;
	g.n = 8;
	g.tint = 0;
	g.tint_t = 0;
	lp_vfill(s, r, &g);
}

void	luna_tray(t_surface *s, t_rect r)
{
	static const t_lstop	st[4] = {{0, 0xff3aa6f4}, {1, 0xff1a9bf2},
	{12, 0xff118ae8}, {29, 0xff0e6cc8}};
	t_lgrad					g;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w <= 2 || r.h <= 0)
		return ;
	g.st = st;
	g.n = 4;
	g.tint = 0;
	g.tint_t = 0;
	lp_vfill(s, r, &g);
	lp_fill(s, lp_rect(r.x, r.y, 1, r.h), 0xff0a4fa8);
	lp_fill(s, lp_rect(r.x + 1, r.y, 1, r.h), 0xff3c9cf0);
}

void	luna_taskbar_layout(t_rect bar, t_taskbarlayout *out)
{
	const t_lunametrics	*m;
	int32_t				tray;

	if (out == NULL)
		return ;
	bar = lp_clean(bar);
	m = lm_metrics();
	tray = lp_min(m->tray_w, lp_max(bar.w - m->start_w, 0));
	out->start = lp_rect(bar.x, bar.y, lp_min(m->start_w, bar.w), bar.h);
	out->tray = lp_rect(bar.x + bar.w - tray, bar.y, tray, bar.h);
	out->tasks = lp_rect(out->start.x + out->start.w + lm_detail()->task_gap,
			bar.y, bar.w - out->start.w - tray - lm_detail()->task_gap, bar.h);
	if (out->tasks.w < 0)
		out->tasks.w = 0;
}
