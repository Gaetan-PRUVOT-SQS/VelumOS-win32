#include "fake.h"

void	luna_menu_panel(t_surface *s, t_rect r)
{
	fake_log_add(s, FK_MENU_PANEL, r, 0);
	gfx_fill(s, r, 0xfffcfcfc);
}

void	luna_menu_item(t_surface *s, t_rect r, t_lunastate st)
{
	fake_log_add(s, FK_MENU_ITEM, r, (int)st);
	gfx_fill(s, r, 0xff316ac5);
}
