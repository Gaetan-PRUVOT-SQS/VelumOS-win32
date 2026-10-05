#include "lt_draw.h"

void	lt_d_start(t_surface *s, t_rect r)
{
	luna_start_button(s, r, LS_NORMAL);
	luna_start_button(s, r, LS_HOT);
	luna_start_button(s, r, LS_PRESSED);
}

void	lt_d_task(t_surface *s, t_rect r)
{
	luna_task_button(s, r, LS_NORMAL);
	luna_task_button(s, r, LS_HOT);
	luna_task_button(s, r, LS_PRESSED);
	luna_task_button(s, r, LS_DEFAULT);
}

void	lt_d_startmenu(t_surface *s, t_rect r)
{
	luna_startmenu(s, r);
	luna_startmenu_named(s, r, NULL);
	luna_startmenu_named(s, r, "Un nom d'utilisateur beaucoup trop long");
}

void	lt_d_wallpaper(t_surface *s, t_rect r)
{
	luna_wallpaper(s, r);
}

void	lt_d_logon(t_surface *s, t_rect r)
{
	luna_logon_bg(s, r);
}
