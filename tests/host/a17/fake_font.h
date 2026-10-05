#ifndef FAKE_FONT_H
# define FAKE_FONT_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "velum/font.h"

typedef struct s_fkp
{
	int32_t	adv;
	int32_t	bold;
	int32_t	yoff;
	int32_t	xoff;
	int32_t	height;
	int32_t	ascent;
	int32_t	descent;
}	t_fkp;

typedef struct s_fkg
{
	const t_fkp	*p;
	t_point		at;
	t_color		c;
}	t_fkg;

const char	*fk_rows(int ch);
int32_t		fk_len(const char *s);
const t_fkp	*fk_params(const t_font *f);
void		fk_glyph(t_surface *s, uint32_t cp, const t_fkg *g);
uint32_t	fk_utf8(const char **s, const char *end);
bool		fk_pixel(uint32_t cp, int32_t x, int32_t y);

#endif
