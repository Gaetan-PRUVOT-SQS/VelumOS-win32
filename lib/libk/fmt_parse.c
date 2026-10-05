#include "fmt_int.h"
#include "velum/libk.h"

static int32_t	parse_int(const char **p, va_list *ap)
{
	int32_t	v;

	if (**p == '*')
	{
		(*p)++;
		return (va_arg(*ap, int));
	}
	v = 0;
	while (**p >= '0' && **p <= '9')
	{
		v = v * 10 + (**p - '0');
		(*p)++;
	}
	return (v);
}

static void	parse_flags(const char **p, t_spec *sp)
{
	const char	*pos;

	pos = strchr("-0+ #", **p);
	while (**p && pos)
	{
		sp->flags |= 1u << (pos - "-0+ #");
		(*p)++;
		pos = strchr("-0+ #", **p);
	}
}

static void	parse_len(const char **p, t_spec *sp)
{
	sp->len = 0;
	if (**p == 'h')
		sp->len = -1;
	else if (**p == 'l' || **p == 'z' || **p == 'j' || **p == 't')
		sp->len = 1;
	if (**p == 'h' && (*p)[1] == 'h')
		sp->len = -2;
	if (**p == 'l' && (*p)[1] == 'l')
		sp->len = 2;
	if (sp->len == 2 || sp->len == -2)
		(*p)++;
	if (sp->len)
		(*p)++;
}

void	fmt_parse(const char **p, va_list *ap, t_spec *sp)
{
	sp->flags = 0;
	sp->prec = -1;
	parse_flags(p, sp);
	sp->width = parse_int(p, ap);
	if (sp->width < 0)
	{
		sp->width = -sp->width;
		sp->flags |= F_LEFT;
	}
	if (**p == '.')
	{
		(*p)++;
		sp->prec = parse_int(p, ap);
	}
	parse_len(p, sp);
	sp->conv = **p;
	if (**p)
		(*p)++;
}
