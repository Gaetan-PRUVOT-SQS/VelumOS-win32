#include <stdint.h>
#include "harness.h"
#include "a20_test.h"
#include "layout.h"
#include "startmenu.h"

static void	footer_geometrie_par_defaut(void)
{
	t_smlayout	l;
	t_rect		off;
	t_rect		out;

	h_eq_i64("layout", sm_layout(22, SM_LEVEL_TOP, &l), 0);
	off = l.item[SM_IDX_LOGOFF];
	out = l.item[SM_IDX_SHUTDOWN];
	h_eq_i64("Eteindre a droite", out.x + out.w, SM_WIDTH - SM_FOOT_MARGIN);
	h_eq_i64("largeur des entrees", out.w, SM_FOOT_ITEM_W);
	h_eq_i64("Fermer la session juste a gauche", off.x + off.w, out.x);
	h_eq_i64("hauteur du pied", out.h, SM_FOOT_H);
	h_eq_i64("meme ligne", off.y, out.y);
	h_eq_i64("pied en bas du panneau", out.y + out.h, l.panel.h);
	h_true(lay_inside(l.foot, off) && lay_inside(l.foot, out), "dans le pied");
}

static void	footer_adoption_de_la_geometrie_externe(void)
{
	t_smlayout	l;
	t_rect		off;
	t_rect		out;

	sm_layout(22, SM_LEVEL_TOP, &l);
	off = lay_rect(100, l.foot.y, 120, SM_FOOT_H);
	out = lay_rect(230, l.foot.y, 120, SM_FOOT_H);
	h_eq_i64("adoptee", sm_set_footer(&l, off, out), 0);
	h_eq_i64("clic sur Fermer la session", sm_hit(&l, 0, 101, l.foot.y + 1),
		SM_IDX_LOGOFF);
	h_eq_i64("clic sur Eteindre", sm_hit(&l, 0, 349, l.foot.y + 1),
		SM_IDX_SHUTDOWN);
	h_eq_i64("clic entre les deux", sm_hit(&l, 0, 225, l.foot.y + 1), -1);
}

static void	footer_refus_des_rectangles_invalides(void)
{
	t_smlayout	l;
	t_rect		ok;

	sm_layout(22, SM_LEVEL_TOP, &l);
	ok = lay_rect(10, l.foot.y, 100, SM_FOOT_H);
	h_eq_i64("hors du panneau", sm_set_footer(&l, ok, lay_rect(300, l.foot.y,
				200, SM_FOOT_H)), -22);
	h_eq_i64("recouvrement", sm_set_footer(&l, ok, lay_rect(50, l.foot.y, 100,
				SM_FOOT_H)), -22);
	h_eq_i64("rectangle vide", sm_set_footer(&l, ok, lay_rect(200, l.foot.y, 0,
				SM_FOOT_H)), -22);
	h_eq_i64("negatif", sm_set_footer(&l, lay_rect(-5, 0, 10, 10), ok), -22);
	h_eq_i64("rien n'a change", l.item[SM_IDX_SHUTDOWN].w, SM_FOOT_ITEM_W);
}

int	main(void)
{
	h_begin("a20/startmenu-footer");
	h_run("pied: geometrie par defaut", footer_geometrie_par_defaut);
	h_run("pied: adoption de la geometrie externe",
		footer_adoption_de_la_geometrie_externe);
	h_run("pied: refus des rectangles invalides",
		footer_refus_des_rectangles_invalides);
	return (h_end());
}
