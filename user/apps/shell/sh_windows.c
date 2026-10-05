#include "velum/libk.h"
#include "../common/layout.h"
#include "shell.h"

void	sh_layout(t_shell *sh)
{
	t_taskbarlayout	tl;
	t_tbmetrics		m;
	t_rect			area;

	luna_taskbar_layout(lay_rect(0, 0, (int32_t)sh->info.screen_w,
			sh->lm.taskbar_h), &tl);
	m = (t_tbmetrics){(int32_t)sh->info.screen_w, sh->lm.taskbar_h,
		tl.start.w, tl.tray.w, tl.tasks.x - tl.start.w};
	tb_regions(&m, &sh->reg);
	area = lay_rect(0, 0, m.w, (int32_t)sh->info.screen_h - sh->lm.taskbar_h);
	sh->placed = (uint32_t)desk_grid(area, DESK_ICONS, sh->cells);
	sm_layout(sh->lm.menu_item_h, SM_LEVEL_TOP, &sh->sml);
}

static int	make_bar(t_shell *sh)
{
	t_wmcreate	rq;
	int32_t		w;
	int32_t		h;

	w = (int32_t)sh->info.screen_w;
	h = (int32_t)sh->info.screen_h;
	ui_request(&rq, lay_rect(0, h - sh->lm.taskbar_h, w, sh->lm.taskbar_h),
		WS_APPBAR, "Barre des tâches");
	return (wmc_create(&sh->bar, &rq));
}

static int	make_menu(t_shell *sh)
{
	t_wmcreate	rq;
	t_rect		at;

	at = sm_place((int32_t)sh->info.screen_h, sh->lm.taskbar_h, &sh->sml);
	ui_request(&rq, at, WS_POPUP | WS_TOPMOST | WS_NOACTIVATE,
		"Menu Démarrer");
	rq.state = WSTATE_HIDDEN;
	return (wmc_create(&sh->menu, &rq));
}

int	sh_make_windows(t_shell *sh)
{
	t_wmcreate	rq;
	int			r;

	sh_layout(sh);
	ui_request(&rq, lay_rect(0, 0, (int32_t)sh->info.screen_w,
			(int32_t)sh->info.screen_h), WS_DESKTOP, "Bureau");
	r = wmc_create(&sh->desk, &rq);
	if (r == 0)
		r = make_bar(sh);
	if (r == 0)
		r = make_menu(sh);
	return (r);
}
