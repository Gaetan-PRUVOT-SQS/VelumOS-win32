#include "fake_font.h"

void	fk_glyph(t_surface *s, uint32_t cp, const t_fkg *g)
{
	int32_t	x;
	int32_t	y;

	y = 0;
	while (y < 11)
	{
		x = 0;
		while (x < 5)
		{
			if (fk_pixel(cp, x, y))
			{
				gfx_put(s, (t_point){g->at.x + g->p->xoff + x,
					g->at.y + g->p->yoff + y}, g->c);
				if (g->p->bold)
					gfx_put(s, (t_point){g->at.x + g->p->xoff + x + 1,
						g->at.y + g->p->yoff + y}, g->c);
			}
			x++;
		}
		y++;
	}
}

void	font_draw(t_surface *dst, const t_textreq *rq)
{
	t_fkg		g;
	const char	*p;
	const char	*end;

	g.p = fk_params(rq->font);
	g.at = rq->at;
	g.c = rq->color;
	p = rq->text;
	end = rq->text + rq->len;
	while (p < end)
	{
		fk_glyph(dst, fk_utf8(&p, end), &g);
		g.at.x += g.p->adv;
	}
}
