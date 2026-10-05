#include "harness.h"
#include "velum/abi/abi_input.h"
#include "velum/libk.h"
#include "render.h"
#include "shell.h"

static const char	*g_argv[] = {"shell", "Utilisateur", NULL};

static void	render_demarrage_et_fenetres(void)
{
	t_shell	sh;

	render_reset("build/a20/a20-fixture/users");
	h_eq_i64("demarrage", sh_boot(&sh, 2, (char **)g_argv), 0);
	h_eq_u64("trois fenetres", g_fwm.count, 3);
	h_eq_u64("bureau: style", fwm_find("Bureau")->style, WS_DESKTOP);
	h_eq_u64("barre: style", fwm_find("Barre des tâches")->style, WS_APPBAR);
	h_eq_u64("barre: hauteur", fwm_find("Barre des tâches")->rect.h, 30);
	h_eq_u64("barre: en bas", fwm_find("Barre des tâches")->rect.y,
		SCREEN_H - 30);
	h_eq_u64("menu: cache", fwm_find("Menu Démarrer")->state, WSTATE_HIDDEN);
	h_eq_u64("menu: popup",
		fwm_find("Menu Démarrer")->style & (WS_POPUP | WS_NOACTIVATE),
		WS_POPUP | WS_NOACTIVATE);
	h_eq_i64("abonne a la liste", g_fwm.subscribed, 1);
	h_eq_str("heure", sh.clock, "14:07");
	h_eq_str("nom du compte", sh.user, "Utilisateur");
	render_shot("shell_bureau");
	sh_shutdown(&sh);
	h_eq_u64("fenetres detruites", g_fwm.count, 0);
}

static void	render_couleurs_de_la_barre(void)
{
	t_shell	sh;

	render_reset("build/a20/a20-fixture/users");
	sh_boot(&sh, 2, (char **)g_argv);
	h_true(px_near(&sh.bar.surface, (t_point){500, 15}, 0x245edc, 40),
		"barre bleue");
	h_true(px_near(&sh.bar.surface, (t_point){1000, 25}, 0x0e6cc8, 60),
		"zone de notification");
	h_true(!px_near(&sh.desk.surface, (t_point){500, 300}, 0, 0),
		"papier peint dessine");
	h_true(sh.desk.surface.px[0] != 0, "coin du papier peint");
	sh_shutdown(&sh);
}

int	main(void)
{
	h_begin("a20/render-shell");
	h_run("rendu shell: demarrage et fenetres", render_demarrage_et_fenetres);
	h_run("rendu shell: couleurs", render_couleurs_de_la_barre);
	return (h_end());
}
