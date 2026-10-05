#include <stdint.h>
#include "harness.h"
#include "a20_test.h"
#include "layout.h"
#include "startmenu.h"

static void	check_items(const t_smlayout *l, uint32_t level)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < SM_ITEMS)
	{
		if (!sm_visible(i, level))
			h_true(l->item[i].w == 0 && l->item[i].h == 0, "masque: vide");
		if (sm_visible(i, level))
			h_true(lay_inside(l->panel, l->item[i]), "dans le panneau");
		j = i + 1;
		while (j < SM_ITEMS)
		{
			h_true(!lay_overlap(l->item[i], l->item[j]), "disjointes");
			j++;
		}
		i++;
	}
}

static void	layout_tailles_et_niveaux(void)
{
	static const int32_t	heights[] = {12, 16, 20, 24, 32, 64};
	t_smlayout				l;
	uint32_t				i;
	uint32_t				level;

	i = 0;
	while (i < sizeof(heights) / sizeof(heights[0]))
	{
		level = 0;
		while (level <= SM_LEVEL_PROGRAMS)
		{
			h_eq_i64("layout", sm_layout(heights[i], level, &l), 0);
			check_items(&l, level);
			h_eq_i64("hauteur du panneau", l.panel.h,
				SM_HEADER_H + SM_ROWS * (heights[i] + 8) + 2 * SM_PAD
				+ SM_FOOT_H);
			level++;
		}
		i++;
	}
}

static void	layout_hit_exhaustif(void)
{
	t_smlayout	l;
	int32_t		x;
	int32_t		y;
	int32_t		hit;
	int32_t		inside;

	h_eq_i64("layout", sm_layout(20, SM_LEVEL_TOP, &l), 0);
	y = -2;
	while (y < l.panel.h + 2)
	{
		x = -2;
		while (x < l.panel.w + 2)
		{
			hit = sm_hit(&l, SM_LEVEL_TOP, x, y);
			inside = hit >= 0 && lay_contains(l.item[hit], x, y);
			h_true(hit == SM_NO_ITEM || inside, "le test de clic est exact");
			h_true(hit != 1 && hit != 2, "pas d'entree masquee");
			x++;
		}
		y++;
	}
}

static void	layout_valeurs_et_placement(void)
{
	t_smlayout	l;
	t_rect		r;

	h_eq_i64("trop petit", sm_layout(11, 0, &l), -34);
	h_eq_i64("trop grand", sm_layout(65, 0, &l), -34);
	h_eq_i64("sortie nulle", sm_layout(20, 0, NULL), -34);
	sm_layout(20, 0, &l);
	r = sm_place(768, 30, &l);
	h_eq_i64("colle a la barre des taches", r.y + r.h, 768 - 30);
	h_eq_i64("a gauche", r.x, 0);
	r = sm_place(100, 30, &l);
	h_eq_i64("ecran trop bas: en haut", r.y, 0);
	h_eq_i64("le centre de Bonjour n'est pas touche au niveau 0",
		sm_hit(&l, SM_LEVEL_TOP, l.item[1].x, l.item[1].y), SM_NO_ITEM);
}

int	main(void)
{
	h_begin("a20/startmenu-layout");
	h_run("menu geometrie: tailles x niveaux", layout_tailles_et_niveaux);
	h_run("menu geometrie: test de clic exhaustif", layout_hit_exhaustif);
	h_run("menu geometrie: valeurs et placement", layout_valeurs_et_placement);
	return (h_end());
}
