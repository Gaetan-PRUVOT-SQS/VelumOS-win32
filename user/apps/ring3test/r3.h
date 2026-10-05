#ifndef R3_H
# define R3_H

# include <stdint.h>
# include <string.h>
# include "velum/velum.h"

# define R3_KPTR 0xffffffff80000000ull
# define R3_HHDM 0xffff800000001000ull
# define R3_BADH 0x7ffffff0ull
# define R3_WAIT_NS 5000000000ull
# define R3_NAP_NS 50000000ull
# define R3_PRIO 8
# define R3_LOG_LEN 200
# define R3_SELF "/system/bin/ring3test"
# define R3_MAGIC 0x1234

typedef struct s_r3
{
	int	ok;
	int	ko;
	int	skip;
}	t_r3;

int64_t	r3_sys(uint64_t num, uint64_t a0, uint64_t a1, uint64_t a2);
void	r3_check(t_r3 *t, const char *name, int64_t got, int64_t want);
void	r3_skip(t_r3 *t, const char *name);
int64_t	r3_spawn_raw(uint64_t path, uint64_t len, uint64_t args,
			uint64_t alen);
int64_t	r3_child(const char *mode, uint32_t flags);
int64_t	r3_reap(int64_t h, t_procinfo *info);
int64_t	r3_thread_raw(uint64_t entry, uint64_t stack, uint64_t prio,
			uint64_t flags);
void	r3_suite_sys(t_r3 *t);
void	r3_suite_spawn(t_r3 *t);
void	r3_suite_obj(t_r3 *t);
void	r3_suite_child(t_r3 *t);
void	r3_suite_thread(t_r3 *t);
int		r3_child_main(const char *mode);
void	r3_do_null(void);
void	r3_do_div0(void);
void	r3_thread_tramp(void);
void	r3_thread_main(uint64_t arg);

#endif
