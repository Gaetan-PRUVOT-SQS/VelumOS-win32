#ifndef INIT_H
# define INIT_H

# include <stdint.h>
# include "velum/abi/abi_types.h"

# define INIT_SERVICES 2
# define RESTART_MAX 5
# define RESTART_WINDOW_NS 30000000000ull
# define INIT_IDLE_NS 1000000000ull

typedef struct s_restarts
{
	uint64_t	stamp[RESTART_MAX];
	uint32_t	n;
	uint32_t	next;
}	t_restarts;

typedef struct s_service
{
	const char	*path;
	uint32_t	flags;
	t_handle	h;
	int			enabled;
	t_restarts	rs;
}	t_service;

int		restart_allowed(t_restarts *r, uint64_t now_ns);
int		service_start(t_service *s);
void	service_died(t_service *s, uint64_t now_ns);

#endif
