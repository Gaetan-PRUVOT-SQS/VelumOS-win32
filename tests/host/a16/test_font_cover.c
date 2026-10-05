#include "a16_test.h"

static const uint32_t	g_extra[] = {
	0x20AC, 0x152, 0x153, 0x2039, 0x203A, 0xAB, 0xBB, 0x2026, 0x2013, 0x2014,
	0x2018, 0x2019, 0x201C, 0x201D, 0xFFFD,
};

static int	in_charset(uint32_t cp)
{
	return ((cp >= 0x20 && cp <= 0x7E) || (cp >= 0xA0 && cp <= 0x17F));
}

static void	check_range(const t_font *f)
{
	uint32_t	cp;

	cp = 0;
	while (cp < 0x180)
	{
		if (in_charset(cp))
			h_eq_u64("COUV-ascii, latin-1, latin etendu-A",
				font_glyph(f, cp)->cp, cp);
		else
			h_eq_u64("COUV-controles absents, remplacement",
				font_glyph(f, cp)->cp, 0xFFFD);
		cp++;
	}
}

static void	cover_required_characters(void)
{
	int		id;
	size_t	i;

	id = 0;
	while (id < FONT_IDS)
	{
		check_range(font_get((t_fontid)id));
		i = 0;
		while (i < sizeof(g_extra) / sizeof(g_extra[0]))
		{
			h_eq_u64("COUV-signes en plus", g_extra[i],
				font_glyph(font_get((t_fontid)id), g_extra[i])->cp);
			i++;
		}
		id++;
	}
}

int	main(void)
{
	h_begin("a16/font_cover");
	h_run("couverture exigee par la fiche", cover_required_characters);
	return (h_end());
}
