#include "harness.h"
#include "velum/abi/abi_input.h"
#include "velum/abi/abi_syscall.h"
#include "velum/libk.h"
#include "render.h"

static const char	*g_argv[] = {"shell", "Utilisateur", NULL};

static void	menu_ouverture_survol_et_echap(void)
{
	t_shell	sh;

	render_reset("build/a20/a20-fixture/users");
	sh_boot(&sh, 2, (char **)g_argv);
	drv_move(&sh, sh.bar.id, rect_center(sh.reg.start));
	h_eq_i64("survol de Demarrer", sh.hot_start, 1);
	drv_click(&sh, sh.bar.id, rect_center(sh.reg.start));
	h_eq_i64("menu ouvert", sh.sm.open, 1);
	h_eq_u64("menu visible", fwm_find("Menu Démarrer")->state, WSTATE_NORMAL);
	h_eq_u64("capture au menu", g_fwm.captured, sh.menu.id);
	render_shot("shell_menu");
	drv_key(&sh, sh.menu.id, VK_ESCAPE);
	h_eq_i64("echap ferme", sh.sm.open, 0);
	h_eq_u64("menu cache", fwm_find("Menu Démarrer")->state, WSTATE_HIDDEN);
	h_eq_u64("capture relachee", g_fwm.captured, 0);
	sh_shutdown(&sh);
}

static void	menu_programmes_et_lancement(void)
{
	t_shell	sh;

	render_reset("build/a20/a20-fixture/users");
	sh_boot(&sh, 2, (char **)g_argv);
	drv_click(&sh, sh.bar.id, rect_center(sh.reg.start));
	drv_click(&sh, sh.menu.id, rect_center(sh.sml.item[0]));
	h_eq_i64("sous-menu ouvert", sh.sm.level, SM_LEVEL_PROGRAMS);
	h_eq_i64("menu toujours ouvert", sh.sm.open, 1);
	render_shot("shell_menu_programmes");
	drv_click(&sh, sh.menu.id, rect_center(sh.sml.item[1]));
	h_eq_i64("menu ferme", sh.sm.open, 0);
	h_eq_u64("un lancement", g_fos.nspawn, 1);
	h_eq_str("programme lance", g_fos.path[0], "/system/bin/hello");
	h_eq_u64("sans privilege", g_fos.flags[0], 0);
	sh_shutdown(&sh);
	h_eq_u64("fils arretes a la fermeture de session", g_fos.killed, 1);
}

static void	menu_fermeture_exterieure_et_touche_windows(void)
{
	t_shell	sh;

	render_reset("build/a20/a20-fixture/users");
	sh_boot(&sh, 2, (char **)g_argv);
	drv_key(&sh, sh.bar.id, VK_LWIN);
	h_eq_i64("Windows ouvre", sh.sm.open, 1);
	drv_key(&sh, sh.bar.id, VK_LWIN);
	h_eq_i64("Windows referme", sh.sm.open, 0);
	drv_key(&sh, sh.bar.id, VK_RWIN);
	drv_click(&sh, sh.menu.id, (t_point){-20, -20});
	h_eq_i64("clic dehors ferme", sh.sm.open, 0);
	drv_click(&sh, sh.bar.id, rect_center(sh.reg.start));
	drv_click(&sh, sh.menu.id, (t_point){50, sh.sml.panel.h + 15});
	h_eq_i64("clic sur Demarrer ouvert referme", sh.sm.open, 0);
	sh_shutdown(&sh);
}

int	main(void)
{
	h_begin("a20/render-menu");
	h_run("rendu menu: ouverture et Echap", menu_ouverture_survol_et_echap);
	h_run("rendu menu: programmes, lancement", menu_programmes_et_lancement);
	h_run("rendu menu: fermetures",
		menu_fermeture_exterieure_et_touche_windows);
	return (h_end());
}
