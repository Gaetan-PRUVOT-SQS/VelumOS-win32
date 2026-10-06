#include "velum/err.h"
#include "velum/libk.h"
#include "../common/utf8.h"
#include "sm_progs.h"

static int	name_ok(const char *s, size_t n)
{
	size_t	i;

	if (n == 0 || n >= SM_PROG_NAME_MAX || !utf8_valid(s, n))
		return (0);
	i = 0;
	while (i < n)
	{
		if ((uint8_t)s[i] < 0x20 || (uint8_t)s[i] == 0x7f)
			return (0);
		i++;
	}
	return (1);
}

static int	path_ok(const char *s, size_t n)
{
	size_t	i;

	if (n <= SM_PROG_ROOT_LEN || n >= SM_PROG_PATH_MAX
		|| memcmp(s, SM_PROG_ROOT, SM_PROG_ROOT_LEN) != 0 || s[n - 1] == '/')
		return (0);
	i = 0;
	while (i < n)
	{
		if ((uint8_t)s[i] <= 0x20 || (uint8_t)s[i] >= 0x7f || s[i] == ';')
			return (0);
		if (s[i] == '/' && i + 1 < n && (s[i + 1] == '/' || s[i + 1] == '.'))
			return (0);
		i++;
	}
	return (1);
}

static int	add_line(t_smprogs *out, const char *s, size_t n)
{
	size_t		name_n;
	size_t		path_n;
	t_smprog	*p;

	name_n = 0;
	while (name_n < n && s[name_n] != ';')
		name_n++;
	if (name_n == n || out->count >= SM_PROGS_MAX)
		return (E_INVAL);
	path_n = n - name_n - 1;
	if (!name_ok(s, name_n) || !path_ok(s + name_n + 1, path_n))
		return (E_INVAL);
	p = &out->list[out->count];
	memcpy(p->name, s, name_n);
	memcpy(p->path, s + name_n + 1, path_n);
	out->count++;
	return (0);
}

int	sm_progs_parse(const char *buf, size_t len, t_smprogs *out)
{
	size_t	start;
	size_t	i;
	int		r;

	memset(out, 0, sizeof(*out));
	if (!buf || len == 0 || len > SM_PROGS_FILE_MAX)
		return (E_INVAL);
	start = 0;
	i = 0;
	r = 0;
	while (i <= len && r == 0)
	{
		if (i == len || buf[i] == '\n')
		{
			if (i > start || i < len)
				r = add_line(out, buf + start, i - start);
			start = i + 1;
		}
		i++;
	}
	if (r < 0)
		memset(out, 0, sizeof(*out));
	if (out->count == 0)
		return (E_INVAL);
	return ((int)out->count);
}

void	sm_progs_default(t_smprogs *out)
{
	memset(out, 0, sizeof(*out));
	strlcpy(out->list[0].name, SM_PROG_DEFAULT_NAME, SM_PROG_NAME_MAX);
	strlcpy(out->list[0].path, SM_PROG_DEFAULT_PATH, SM_PROG_PATH_MAX);
	out->count = 1;
}
