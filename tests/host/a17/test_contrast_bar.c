#include "harness.h"
#include "lt_contrast.h"

static t_lt	g_t;

static void	bar_start(void)
{
	static const char	*name[3] = {"demarrer normal", "demarrer survol",
		"demarrer enfonce"};
	int					st;

	st = LS_NORMAL;
	while (st <= LS_PRESSED)
	{
		lk_begin(&g_t, LC_BLACK);
		luna_start_button(&g_t.s, lp_rect(0, 0, 100, 30), (t_lunastate)st);
		lk_check(name[st], lk_spied(&g_t.s));
		st++;
	}
}

static void	bar_task_at(int32_t h, t_lunastate st, const char *what)
{
	t_rect	r;

	r = lp_rect(0, 0, 160, h);
	lk_begin(&g_t, LC_BLACK);
	luna_task_button(&g_t.s, r, st);
	r = lk_band(lp_rect(4, 0, 152, h), LK_UI);
	lk_check(what, lk_worst(&g_t.s, r, LC_WHITE));
}

static void	bar_task(void)
{
	bar_task_at(30, LS_NORMAL, "tache inactive h30");
	bar_task_at(22, LS_NORMAL, "tache inactive h22");
	bar_task_at(30, LS_HOT, "tache survolee h30");
	bar_task_at(22, LS_HOT, "tache survolee h22");
	bar_task_at(30, LS_PRESSED, "tache active h30");
	bar_task_at(22, LS_PRESSED, "tache active h22");
}

static void	bar_clock(void)
{
	t_rect	r;

	r = lp_rect(0, 0, 110, 30);
	lk_begin(&g_t, LC_BLACK);
	luna_taskbar(&g_t.s, lp_rect(0, 0, 200, 30));
	luna_tray(&g_t.s, r);
	r = lk_band(lp_rect(4, 0, 102, 30), LK_UI);
	lk_check("horloge sur la zone de notification",
		lk_worst(&g_t.s, r, LC_BLACK));
}

int	main(void)
{
	if (lt_open(&g_t, 200, 40) != 0)
		return (1);
	h_begin("a17/contrast_bar");
	h_run("P25 libelle du bouton demarrer", bar_start);
	h_run("P25 libelle des boutons de tache", bar_task);
	h_run("horloge", bar_clock);
	lt_close(&g_t);
	return (h_end());
}
