#ifndef STARTMENU_H
# define STARTMENU_H

# include <stdint.h>
# include "velum/gfx.h"

# define SM_ITEMS 6
# define SM_NO_ITEM -1
# define SM_WIDTH 380
# define SM_HEADER_H 54
# define SM_FOOT_H 38
# define SM_FOOT_ITEM_W 150
# define SM_FOOT_MARGIN 8
# define SM_IDX_LOGOFF 4
# define SM_IDX_SHUTDOWN 5
# define SM_PAD 6
# define SM_ROWS 4
# define SM_ITEM_H_MIN 12
# define SM_ITEM_H_MAX 64
# define SM_LEVEL_TOP 0
# define SM_LEVEL_PROGRAMS 1

typedef enum e_smcmd
{
	SMC_NONE = 0,
	SMC_OPEN_PROGRAMS,
	SMC_BACK,
	SMC_LAUNCH_HELLO,
	SMC_RUN,
	SMC_LOGOFF,
	SMC_SHUTDOWN
}	t_smcmd;

typedef enum e_smcol
{
	SMCOL_LEFT = 0,
	SMCOL_RIGHT,
	SMCOL_FOOT
}	t_smcol;

typedef struct s_smitem
{
	uint32_t	cmd;
	uint32_t	col;
	uint32_t	slot;
	uint32_t	levels;
	uint32_t	icon;
	const char	*label;
}	t_smitem;

typedef struct s_startmenu
{
	int			open;
	uint32_t	level;
	int32_t		hot;
}	t_startmenu;

typedef struct s_smlayout
{
	t_rect	panel;
	t_rect	header;
	t_rect	left;
	t_rect	right;
	t_rect	foot;
	t_rect	item[SM_ITEMS];
}	t_smlayout;

const t_smitem	*sm_item(uint32_t index);
int				sm_visible(uint32_t index, uint32_t level);
void			sm_init(t_startmenu *m);
void			sm_open(t_startmenu *m);
void			sm_close(t_startmenu *m);
int32_t			sm_hover(t_startmenu *m, int32_t idx);
int32_t			sm_move(t_startmenu *m, int dir);
int				sm_activate(t_startmenu *m, int32_t idx);
int				sm_key(t_startmenu *m, uint32_t vk);
int				sm_layout(int32_t item_h, uint32_t level, t_smlayout *out);
int				sm_set_footer(t_smlayout *l, t_rect logoff, t_rect poweroff);
int32_t			sm_hit(const t_smlayout *l, uint32_t level, int32_t x,
					int32_t y);
t_rect			sm_place(int32_t screen_h, int32_t bar_h, const t_smlayout *l);

#endif
