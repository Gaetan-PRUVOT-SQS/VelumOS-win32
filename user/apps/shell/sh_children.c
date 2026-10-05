#include "../common/platform.h"
#include "shell.h"

void	sh_reap(t_shell *sh)
{
	uint32_t	i;

	i = 0;
	while (i < sh->nchildren)
	{
		if (os_exited(sh->children[i]))
		{
			os_close(sh->children[i]);
			sh->nchildren--;
			sh->children[i] = sh->children[sh->nchildren];
		}
		else
			i++;
	}
}

int	sh_launch(t_shell *sh, const char *path)
{
	t_handle	h;
	int			r;

	sh_reap(sh);
	r = os_spawn(path, NULL, 0, &h);
	if (r < 0)
		return (r);
	if (sh->nchildren >= SH_CHILD_MAX)
	{
		os_close(h);
		return (0);
	}
	sh->children[sh->nchildren] = h;
	sh->nchildren++;
	return (0);
}

void	sh_kill_children(t_shell *sh)
{
	uint32_t	i;

	i = 0;
	while (i < sh->nchildren)
	{
		os_kill(sh->children[i]);
		os_close(sh->children[i]);
		i++;
	}
	sh->nchildren = 0;
}
