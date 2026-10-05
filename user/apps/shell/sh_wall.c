#include <stdlib.h>
#include "velum/libk.h"
#include "shell.h"

void	sh_wall_make(t_shell *sh)
{
	int32_t	w;
	int32_t	h;

	w = (int32_t)sh->info.screen_w;
	h = (int32_t)sh->info.screen_h;
	sh->wall_px = malloc((size_t)w * (size_t)h * sizeof(uint32_t));
	if (!sh->wall_px)
		return ;
	gfx_surface_init(&sh->wall, sh->wall_px, w, h);
	luna_wallpaper(&sh->wall, rect_make(0, 0, w, h));
}

void	sh_wall_blit(t_shell *sh, t_rect r)
{
	t_blit	b;

	if (!sh->wall_px)
	{
		gfx_fill(&sh->desk.surface, r, SH_WALL_COLOR);
		return ;
	}
	b.dst = &sh->desk.surface;
	b.src = &sh->wall;
	b.dr = r;
	b.sr = r;
	gfx_blit(&b);
}

void	sh_desk_paint(t_shell *sh)
{
	uint32_t	i;

	sh_wall_blit(sh, rect_make(0, 0, sh->desk.surface.w, sh->desk.surface.h));
	i = 0;
	while (i < sh->placed)
	{
		sh_desk_cell_draw(sh, (int32_t)i);
		i++;
	}
	wmc_present(&sh->desk, NULL, 0);
}
