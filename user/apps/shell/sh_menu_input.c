#include "velum/abi/abi_input.h"
#include "../common/layout.h"
#include "shell.h"

void	sh_menu_key(t_shell *sh, uint32_t vk)
{
	sh_menu_run(sh, sm_key(&sh->sm, vk));
}

void	sh_menu_mouse(t_shell *sh, const t_wmmouse *m)
{
	int32_t	item;
	int32_t	before;

	if (!sh->sm.open)
		return ;
	sh_menu_geo(sh);
	item = sm_hit(&sh->sml, sh->sm.level, m->x, m->y);
	if (m->type == INP_MOUSE_MOVE)
	{
		before = sh->sm.hot;
		if (sm_hover(&sh->sm, item) != before)
			sh_menu_paint(sh);
		return ;
	}
	if (m->type != INP_MOUSE_DOWN
		|| ((m->buttons >> WM_BTN_SHIFT) & WM_BTN_MASK) != BTN_LEFT)
		return ;
	if (!lay_contains(sh->sml.panel, m->x, m->y))
		sh_menu_close(sh);
	else
		sh_menu_run(sh, sm_activate(&sh->sm, item));
}
