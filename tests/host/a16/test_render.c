#include <stdio.h>
#include <stdlib.h>
#include "a16_test.h"

#define PAGE_W 520
#define PAGE_LINES 7
#define PPM_SCALE 3

static const char		*g_lines[PAGE_LINES] = {
	"Démarrer",
	"Poste de travail",
	"Éteindre l'ordinateur",
	"Fichier  Édition  Affichage  Favoris  Outils  ?",
	"ÀÂÇÉÈÊËÎÏÔÙÛÜŸ àâçéèêëîïôùûüÿ œŒ € « » … – — ‘ ’ “ ”",
	"ĀăĄćČďĐēĘěĞĢīİıŁńŇŌőřŚŞŠţŤūůŰŹŻž ĲĳĸŉŊŋſ",
	"The quick brown fox jumps over the lazy dog 0123456789 (){}[]<>",
};

static const uint64_t	g_golden[FONT_IDS] = {
	0x6332754a9f6f3741ull, 0x681eca5b6212e6c3ull, 0x6a0ce746a9a8c353ull,
	0x2698d396da2728eaull,
};
static const char		*g_names[FONT_IDS] = {"ui", "ui_bold", "title", "mono"};

static void	draw_page(t_surface *s, t_fontid id)
{
	t_textreq	rq;
	int32_t		line;

	line = 0;
	while (line < PAGE_LINES)
	{
		req_set(&rq, id, point_of(6, 4 + line * (font_get(id)->height + 2)),
			g_lines[line]);
		font_draw(s, &rq);
		line++;
	}
}

static void	save_ppm(const t_surface *s, const char *name)
{
	char		path[256];
	const char	*dir;

	dir = getenv("A16_OUT");
	if (!dir || !*dir)
		dir = "build/a16/render";
	snprintf(path, sizeof(path), "%s/%s.ppm", dir, name);
	if (fake_write_ppm(s, path, PPM_SCALE) != 0)
		printf("a16/render : ecriture de %s impossible (ignore)\n", path);
}

static void	render_every_font(void)
{
	int			id;
	t_surface	s;
	int32_t		height;

	id = 0;
	while (id < FONT_IDS)
	{
		height = 8 + PAGE_LINES * (font_get((t_fontid)id)->height + 2);
		fake_alloc(&s, PAGE_W, height);
		draw_page(&s, (t_fontid)id);
		h_eq_i64("RENDU-aucun appel hors surface", g_fake.outside, 0);
		save_ppm(&s, g_names[id]);
		printf("a16/render : %s hash 0x%016llx\n", g_names[id],
			(unsigned long long)fake_hash(&s));
		h_eq_u64("RENDU-page francaise figee par hash", fake_hash(&s),
			g_golden[id]);
		free(s.px);
		id++;
	}
}

int	main(void)
{
	h_begin("a16/render");
	h_run("pages d'essai francaises des 4 polices", render_every_font);
	return (h_end());
}
