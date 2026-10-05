#include "fmt_int.h"

void	out_init(t_out *o, char *buf, size_t size)
{
	o->buf = buf;
	o->size = size;
	o->pos = 0;
}

void	out_putc(t_out *o, char c)
{
	if (o->size && o->pos < o->size - 1)
		o->buf[o->pos] = c;
	o->pos++;
}

void	out_fill(t_out *o, char c, int32_t n)
{
	while (n > 0)
	{
		out_putc(o, c);
		n--;
	}
}

void	out_write(t_out *o, const char *s, size_t n)
{
	while (n--)
		out_putc(o, *s++);
}

void	out_term(t_out *o)
{
	size_t	i;

	if (!o->size)
		return ;
	i = o->pos;
	if (i >= o->size)
		i = o->size - 1;
	o->buf[i] = '\0';
}
