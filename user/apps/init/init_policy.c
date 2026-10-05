#include "init.h"

static uint32_t	restarts_recent(const t_restarts *r, uint64_t now_ns)
{
	uint32_t	i;
	uint32_t	recent;

	recent = 0;
	i = 0;
	while (i < r->n)
	{
		if (now_ns >= r->stamp[i] && now_ns - r->stamp[i] < RESTART_WINDOW_NS)
			recent++;
		i++;
	}
	return (recent);
}

int	restart_allowed(t_restarts *r, uint64_t now_ns)
{
	if (r->n > RESTART_MAX || r->next >= RESTART_MAX)
		return (0);
	if (restarts_recent(r, now_ns) >= RESTART_MAX)
		return (0);
	r->stamp[r->next] = now_ns;
	r->next = (r->next + 1) % RESTART_MAX;
	if (r->n < RESTART_MAX)
		r->n++;
	return (1);
}
