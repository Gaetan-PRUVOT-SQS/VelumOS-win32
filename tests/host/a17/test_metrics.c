#include "harness.h"
#include "luna_int.h"

static void	metrics_positive(void)
{
	t_lunametrics	m;
	const int32_t	*v;
	int				i;

	luna_metrics(&m);
	v = (const int32_t *)&m;
	i = 0;
	while (i < (int)(sizeof(m) / sizeof(int32_t)))
	{
		h_true(v[i] > 0, "champ de metrique strictement positif");
		i++;
	}
}

static void	metrics_parity(void)
{
	t_lunametrics	m;

	luna_metrics(&m);
	h_true(m.btn_w % 2 == 1, "bouton de legende impair : glyphe centre");
	h_true(m.scroll_w % 2 == 1, "ascenseur impair : fleche centree");
	h_true(m.taskbar_h % 2 == 0, "barre des taches paire");
	h_true(m.menu_item_h % 2 == 0, "entree de menu paire");
	h_true((m.taskbar_h - m.icon_small) % 2 == 0, "icone centree en barre");
	h_true(m.icon_large == 2 * m.icon_small, "icones 16 et 32");
	h_true(m.btn_w == m.btn_h, "boutons de legende carres");
	h_true(m.frame_w == m.frame_bottom, "bordures symetriques");
}

static void	metrics_titlebar(void)
{
	t_lunametrics		m;
	const t_lunadetail	*d;

	luna_metrics(&m);
	d = lm_detail();
	h_true(m.caption_h > m.frame_w, "titre plus haut que la bordure");
	h_true(d->btn_top + m.btn_h <= m.caption_h, "bouton dans la barre");
	h_true(d->btn_right >= m.frame_w, "bouton hors de la bordure droite");
	h_true(d->btn_top >= m.frame_w, "bouton sous la bordure haute");
	h_true(d->btn_top_max + m.btn_h <= m.caption_h - m.frame_w, "max");
	h_true(d->tool_btn_top + d->tool_btn <= d->tool_caption_h, "outil");
	h_true(m.icon_small + 2 <= d->sysmenu_w, "icone dans la zone systeme");
	h_true(d->btn_gap > 0 && d->btn_gap < m.btn_w, "ecart de boutons");
}

static void	metrics_stable(void)
{
	t_lunametrics	a;
	t_lunametrics	b;

	luna_metrics(&a);
	luna_metrics(&b);
	h_true(a.caption_h == b.caption_h && a.taskbar_h == b.taskbar_h,
		"deux appels, memes valeurs");
	luna_metrics(NULL);
	h_true(1, "luna_metrics(NULL) sans effet");
	h_eq_i64("barre des taches", a.taskbar_h, 30);
	h_eq_i64("bouton demarrer", a.start_w, 100);
}

int	main(void)
{
	h_begin("a17/metrics");
	h_run("metrique positive", metrics_positive);
	h_run("parite", metrics_parity);
	h_run("boutons dans la barre de titre", metrics_titlebar);
	h_run("stabilite et valeurs publiques", metrics_stable);
	return (h_end());
}
