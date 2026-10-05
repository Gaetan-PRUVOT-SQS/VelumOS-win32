#ifndef LOGON_LAYOUT_H
# define LOGON_LAYOUT_H

# include <stdint.h>
# include "velum/gfx.h"

# define LOGON_MIN_W 640
# define LOGON_MIN_H 480
# define LOGON_TILES_MAX 6
# define LOGON_CARD_W_MAX 360
# define LOGON_CARD_W_MIN 200
# define LOGON_MARGIN 16
# define LOGON_TILE_H 56
# define LOGON_TILE_GAP 8
# define LOGON_AVATAR 48
# define LOGON_BAND_TOP 96
# define LOGON_BAND_BOTTOM 104
# define LOGON_PW_H 24
# define LOGON_GO_W 28
# define LOGON_MSG_H 32
# define LOGON_SHUT_W 150
# define LOGON_SHUT_H 28
# define LOGON_DLG_TEXT_H 60
# define LOGON_DLG_BTN_W 96
# define LOGON_DLG_BTN_H 28

typedef struct s_logonlayout
{
	uint32_t	count;
	t_rect		prompt;
	t_rect		card;
	t_rect		tile[LOGON_TILES_MAX];
	t_rect		password;
	t_rect		go;
	t_rect		message;
	t_rect		shutdown;
	t_rect		dlg_text;
	t_rect		dlg_ok;
	t_rect		dlg_cancel;
}	t_logonlayout;

int32_t	logon_card_h(uint32_t count);
int		logon_layout(int32_t w, int32_t h, uint32_t count, t_logonlayout *out);

#endif
