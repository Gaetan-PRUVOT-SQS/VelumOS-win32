#include <stdint.h>
#include "harness.h"
#include "a20_test.h"
#include "layout.h"
#include "taskbar.h"

static void	taskbar_regions_normales(void)
{
	t_tbmetrics	m;
	t_tbregions	r;

	m = (t_tbmetrics){1024, 30, 100, 80, 3};
	h_eq_i64("regions", tb_regions(&m, &r), 0);
	h_eq_i64("demarrer x", r.start.x, 0);
	h_eq_i64("demarrer largeur", r.start.w, 100);
	h_eq_i64("notification x", r.tray.x, 944);
	h_eq_i64("bande x", r.strip.x, 103);
	h_eq_i64("bande largeur", r.strip.w, 841);
	h_eq_i64("bande contre la zone", r.strip.x + r.strip.w, r.tray.x);
	h_eq_i64("bande hauteur", r.strip.h, 26);
	h_true(!lay_overlap(r.start, r.strip), "Demarrer hors bande");
	h_true(!lay_overlap(r.tray, r.strip), "notification hors bande");
	h_true(lay_inside(lay_rect(0, 0, 1024, 30), r.strip), "bande dans barre");
}

static void	taskbar_regions_invalides(void)
{
	t_tbmetrics	m;
	t_tbregions	r;

	m = (t_tbmetrics){1024, 4, 100, 80, 3};
	h_eq_i64("barre trop basse", tb_regions(&m, &r), -22);
	m = (t_tbmetrics){1024, 30, 0, 80, 3};
	h_eq_i64("Demarrer nul", tb_regions(&m, &r), -22);
	m = (t_tbmetrics){1024, 30, 100, -1, 3};
	h_eq_i64("notification negative", tb_regions(&m, &r), -22);
	m = (t_tbmetrics){150, 30, 100, 80, 3};
	h_eq_i64("ecran trop etroit", tb_regions(&m, &r), -34);
	m = (t_tbmetrics){1024, 30, 100, 80, -1};
	h_eq_i64("interstice negatif", tb_regions(&m, &r), -22);
	m = (t_tbmetrics){1024, 30, 100, 80, 65};
	h_eq_i64("interstice trop grand", tb_regions(&m, &r), -22);
	m = (t_tbmetrics){1024, 30, 100, 80, 0};
	h_eq_i64("interstice nul accepte", tb_regions(&m, &r), 0);
	h_eq_i64("sortie nulle", tb_regions(&m, NULL), -22);
}

static void	taskbar_cas_simples(void)
{
	t_rect	out[TASK_MAX];
	t_rect	strip;

	strip = lay_rect(106, 2, 832, 26);
	h_eq_i64("zero fenetre", tb_layout(strip, 0, out), 0);
	h_eq_i64("une fenetre", tb_layout(strip, 1, out), 1);
	h_eq_i64("largeur maximale", out[0].w, TB_BTN_MAX_W);
	h_eq_i64("une: x", out[0].x, 106);
	h_eq_i64("cinq fenetres", tb_layout(strip, 5, out), 5);
	h_eq_i64("cinq: largeur maximale", out[4].w, TB_BTN_MAX_W);
	h_eq_i64("cinq: pas de recouvrement", out[4].x, 106 + 4 * (160 + 2));
	h_eq_i64("quarante fenetres", tb_layout(strip, 40, out), 40);
	h_eq_i64("bande vide", tb_layout(lay_rect(0, 0, 10, 26), 3, out), 0);
	h_eq_i64("hauteur nulle", tb_layout(lay_rect(0, 0, 832, 0), 3, out), 0);
}

static void	taskbar_test_de_clic(void)
{
	t_rect	out[TASK_MAX];

	tb_layout(lay_rect(106, 2, 832, 26), 3, out);
	h_eq_i64("premier bouton", tb_hit(out, 3, 106, 2), 0);
	h_eq_i64("dernier pixel du premier", tb_hit(out, 3, 265, 27), 0);
	h_eq_i64("interstice", tb_hit(out, 3, 266, 10), -1);
	h_eq_i64("second bouton", tb_hit(out, 3, 268, 10), 1);
	h_eq_i64("au dessus", tb_hit(out, 3, 110, 1), -1);
	h_eq_i64("sous le dernier", tb_hit(out, 3, 600, 10), -1);
	h_eq_i64("aucun bouton", tb_hit(out, 0, 110, 10), -1);
}

int	main(void)
{
	h_begin("a20/taskbar");
	h_run("barre: regions normales", taskbar_regions_normales);
	h_run("barre: regions invalides", taskbar_regions_invalides);
	h_run("barre: cas simples 0, 1, 5, 40", taskbar_cas_simples);
	h_run("barre: test de clic", taskbar_test_de_clic);
	return (h_end());
}
