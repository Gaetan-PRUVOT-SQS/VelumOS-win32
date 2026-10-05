#include <stdint.h>
#include "harness.h"
#include "a20_srgb.h"
#include "uicolor.h"

static void	contrast_formule_de_reference(void)
{
	h_eq_u64("noir sur blanc: 21", ratio_x100(0x000000, 0xffffff), 2100);
	h_eq_u64("meme couleur: 1", ratio_x100(0x245edc, 0x245edc), 100);
	h_eq_u64("symetrique", ratio_x100(0xffffff, 0x245edc),
		ratio_x100(0x245edc, 0xffffff));
	h_true(ratio_x100(0x777777, 0xffffff) < 450, "#777 sur blanc sous AA");
	h_true(ratio_x100(0x767676, 0xffffff) >= 450, "#767676 sur blanc: AA");
	h_true(ratio_x100(0xffffff, 0x245edc) >= 560, "blanc sur #245EDC: 5,6");
	h_true(ratio_x100(0xffffff, 0x245edc) <= 570, "blanc sur #245EDC: 5,7");
}

static void	contrast_couples_de_texte_du_shell(void)
{
	h_true(ratio_x100(UI_WHITE & 0xffffff, 0x245edc) >= 450,
		"barre des taches: blanc sur bleu");
	h_true(ratio_x100(UI_WHITE & 0xffffff, 0x316ac5) >= 450,
		"selection: blanc sur #316AC5");
	h_true(ratio_x100(UI_BLACK & 0xffffff, 0xffffff) >= 450,
		"colonne gauche: noir sur blanc");
	h_true(ratio_x100(UI_BLACK & 0xffffff, 0xece9d8) >= 450,
		"fenetre: noir sur #ECE9D8");
	h_true(ratio_x100(UI_MENU_TEXT_RIGHT & 0xffffff, 0xd3e5fa) >= 450,
		"colonne droite: bleu fonce sur bleu clair");
	h_true(ratio_x100(UI_WHITE & 0xffffff, 0x245edc) >= 450,
		"pied de menu: blanc sur bleu");
}

static void	contrast_fonds_variables(void)
{
	uint32_t	label;
	uint32_t	prompt;

	label = blend_over(UI_LABEL_PLATE, 0xffffff);
	prompt = blend_over(UI_PROMPT_PLATE, 0xffffff);
	h_true(ratio_x100(0xffffff, label) >= 450,
		"libelle d'icone: blanc sur plaque, pire fond blanc");
	h_true(ratio_x100(0xffffff, prompt) >= 450,
		"invite de connexion: blanc sur plaque, pire fond blanc");
	h_true(ratio_x100(0x000000, 0x3aa6f4) >= 450, "heure: noir sur le haut");
	h_true(ratio_x100(0x000000, 0x118ae8) >= 450, "heure: noir au milieu");
	h_true(ratio_x100(0x000000, 0x107ad7) >= 450, "heure: noir, ligne 21");
	h_true(ratio_x100(0xffffff, 0x118ae8) < 450, "heure en blanc: sous AA");
}

int	main(void)
{
	h_begin("a20/contrast");
	h_run("contraste: formule de reference", contrast_formule_de_reference);
	h_run("contraste: couples du shell AA", contrast_couples_de_texte_du_shell);
	h_run("contraste: fonds variables", contrast_fonds_variables);
	return (h_end());
}
