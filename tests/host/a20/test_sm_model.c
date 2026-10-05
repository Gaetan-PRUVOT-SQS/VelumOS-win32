#include <stdint.h>
#include "harness.h"
#include "velum/abi/abi_input.h"
#include "a20_test.h"
#include "startmenu.h"

static void	model_ferme_et_ouverture(void)
{
	t_startmenu	m;

	sm_init(&m);
	h_eq_i64("ferme au depart", m.open, 0);
	h_eq_i64("touche ignoree si ferme", sm_key(&m, VK_DOWN), SMC_NONE);
	h_eq_i64("survol ignore si ferme", sm_hover(&m, 3), SM_NO_ITEM);
	h_eq_i64("activation ignoree si ferme", sm_activate(&m, 3), SMC_NONE);
	h_eq_i64("deplacement ignore si ferme", sm_move(&m, 1), SM_NO_ITEM);
	sm_open(&m);
	h_eq_i64("ouvert", m.open, 1);
	h_eq_i64("niveau principal", m.level, SM_LEVEL_TOP);
	h_eq_i64("rien de survole", m.hot, SM_NO_ITEM);
}

static void	model_fleches_au_niveau_principal(void)
{
	t_startmenu	m;

	sm_init(&m);
	sm_open(&m);
	h_eq_i64("bas: premiere entree", sm_move(&m, 1), 0);
	h_eq_i64("bas: Executer", sm_move(&m, 1), 3);
	h_eq_i64("bas: Fermer la session", sm_move(&m, 1), 4);
	h_eq_i64("bas: Eteindre", sm_move(&m, 1), 5);
	h_eq_i64("bas: reboucle", sm_move(&m, 1), 0);
	h_eq_i64("haut: reboucle", sm_move(&m, -1), 5);
	sm_key(&m, VK_HOME);
	h_eq_i64("Debut", m.hot, 0);
	sm_key(&m, VK_END);
	h_eq_i64("Fin", m.hot, 5);
	sm_close(&m);
	sm_open(&m);
	h_eq_i64("haut depuis rien: derniere entree", sm_move(&m, -1), 5);
}

static void	model_sous_menu_programmes(void)
{
	t_startmenu	m;

	sm_init(&m);
	sm_open(&m);
	sm_move(&m, 1);
	h_eq_i64("Entree ouvre les programmes", sm_key(&m, VK_RETURN),
		SMC_OPEN_PROGRAMS);
	h_eq_i64("niveau programmes", m.level, SM_LEVEL_PROGRAMS);
	h_eq_i64("premiere entree du niveau", m.hot, 1);
	h_eq_i64("bas: Retour", sm_move(&m, 1), 2);
	h_eq_i64("haut: reboucle sur Eteindre", sm_move(&m, -1), 1);
	h_eq_i64("haut depuis la premiere", sm_move(&m, -1), 5);
	h_eq_i64("bas: reboucle sur Bonjour", sm_move(&m, 1), 1);
	h_eq_i64("lancer Bonjour", sm_key(&m, VK_SPACE), SMC_LAUNCH_HELLO);
	h_eq_i64("menu ferme apres lancement", m.open, 0);
	h_eq_i64("niveau remis", m.level, SM_LEVEL_TOP);
}

static void	model_decisions_clavier(void)
{
	t_startmenu	m;

	sm_init(&m);
	sm_open(&m);
	sm_hover(&m, 3);
	h_eq_i64("Droite hors sous-menu", sm_key(&m, VK_RIGHT), SMC_NONE);
	h_eq_i64("rien ne change", m.level, SM_LEVEL_TOP);
	sm_hover(&m, 0);
	sm_key(&m, VK_RIGHT);
	h_eq_i64("Droite ouvre", m.level, SM_LEVEL_PROGRAMS);
	sm_key(&m, VK_LEFT);
	h_eq_i64("Gauche revient", m.level, SM_LEVEL_TOP);
	h_eq_i64("Gauche au niveau principal sans effet", sm_key(&m, VK_LEFT), 0);
	sm_key(&m, VK_RIGHT);
	sm_key(&m, VK_ESCAPE);
	h_eq_i64("Echap revient d'un niveau", m.open && m.level == 0, 1);
	sm_key(&m, VK_ESCAPE);
	h_eq_i64("Echap ferme", m.open, 0);
	sm_open(&m);
	sm_key(&m, VK_LWIN);
	h_eq_i64("Windows ferme", m.open, 0);
}

int	main(void)
{
	h_begin("a20/startmenu-model");
	h_run("menu: ferme et ouverture", model_ferme_et_ouverture);
	h_run("menu: fleches niveau principal", model_fleches_au_niveau_principal);
	h_run("menu: sous-menu programmes", model_sous_menu_programmes);
	h_run("menu: decisions clavier", model_decisions_clavier);
	return (h_end());
}
