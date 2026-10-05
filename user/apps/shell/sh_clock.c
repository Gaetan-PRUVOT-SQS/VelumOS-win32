#include "../common/platform.h"
#include "shell.h"

void	sh_clock_arm(t_shell *sh)
{
	os_timer_after(sh->timer, ns_to_next_minute(os_wall_ns()), 0);
}

void	sh_clock_tick(t_shell *sh)
{
	fmt_clock(os_wall_ns(), sh->clock, sizeof(sh->clock));
	sh_clock_arm(sh);
	if (sh->bar.id != 0)
		sh_bar_paint(sh);
}
