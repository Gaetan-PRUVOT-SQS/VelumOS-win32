#include "font_int.h"

static void	paint_row(const t_pen *pen, const t_glyph *g, const t_cut *cut,
		int32_t row)
{
	const uint8_t	*line;
	int32_t			col;
	t_point			at;

	line = g->bits + (size_t)row * (size_t)((g->w + 7) / 8);
	col = cut->c0;
	at.y = cut->y + row;
	while (col < cut->c1)
	{
		if ((line[col >> 3] >> (7 - (col & 7))) & 1)
		{
			at.x = cut->x + col;
			gfx_put(pen->dst, at, pen->color);
		}
		col++;
	}
}

static void	draw_glyph(const t_pen *pen, const t_glyph *g)
{
	t_cut	cut;
	int32_t	row;

	if (!g->bits || !g->w || !g->h || !font_glyph_cut(pen, g, &cut))
		return ;
	row = cut.r0;
	while (row < cut.r1)
	{
		paint_row(pen, g, &cut, row);
		row++;
	}
}

void	font_draw(t_surface *dst, const t_textreq *rq)
{
	t_pen			pen;
	const char		*text;
	const char		*end;
	const t_glyph	*g;

	if (!dst || !rq || !rq->font || !rq->text)
		return ;
	if (!font_pen_start(&pen, dst, rq))
		return ;
	text = rq->text;
	end = font_text_end(text, rq->len);
	while (text < end && pen.x - FONT_XOFF_MIN < pen.box.right)
	{
		g = font_glyph(rq->font, font_utf8_next(&text, end));
		if (g)
		{
			draw_glyph(&pen, g);
			pen.x += g->advance;
		}
	}
}
