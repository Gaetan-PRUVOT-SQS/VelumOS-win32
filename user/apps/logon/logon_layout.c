#include "velum/err.h"
#include "velum/libk.h"
#include "../common/layout.h"
#include "logon_layout.h"

int32_t	logon_card_h(uint32_t count)
{
	return (LOGON_MARGIN + (int32_t)count * (LOGON_TILE_H + LOGON_TILE_GAP) + 4
		+ LOGON_PW_H + 8 + LOGON_MSG_H + 10 + LOGON_SHUT_H + LOGON_MARGIN);
}

static void	place_children(t_logonlayout *o)
{
	int32_t		iw;
	int32_t		y;
	int32_t		pw_x;
	uint32_t	i;

	iw = o->card.w - 2 * LOGON_MARGIN;
	y = LOGON_MARGIN;
	i = 0;
	while (i < o->count)
	{
		o->tile[i] = lay_rect(LOGON_MARGIN, y, iw, LOGON_TILE_H);
		y += LOGON_TILE_H + LOGON_TILE_GAP;
		i++;
	}
	y += 4;
	pw_x = LOGON_MARGIN + LOGON_AVATAR + 12;
	o->password = lay_rect(pw_x, y, iw - (pw_x - LOGON_MARGIN) - 8 - LOGON_GO_W,
			LOGON_PW_H);
	o->go = lay_rect(o->password.x + o->password.w + 8, y, LOGON_GO_W,
			LOGON_PW_H);
	o->message = lay_rect(LOGON_MARGIN, y + LOGON_PW_H + 8, iw, LOGON_MSG_H);
	o->shutdown = lay_rect(LOGON_MARGIN, o->message.y + LOGON_MSG_H + 10,
			LOGON_SHUT_W, LOGON_SHUT_H);
}

static void	place_dialog(t_logonlayout *o)
{
	int32_t	iw;
	int32_t	y;

	iw = o->card.w - 2 * LOGON_MARGIN;
	y = LOGON_MARGIN + LOGON_DLG_TEXT_H + 10;
	o->dlg_text = lay_rect(LOGON_MARGIN, LOGON_MARGIN, iw, LOGON_DLG_TEXT_H);
	o->dlg_ok = lay_rect(LOGON_MARGIN + iw - 2 * LOGON_DLG_BTN_W - 8, y,
			LOGON_DLG_BTN_W, LOGON_DLG_BTN_H);
	o->dlg_cancel = lay_rect(LOGON_MARGIN + iw - LOGON_DLG_BTN_W, y,
			LOGON_DLG_BTN_W, LOGON_DLG_BTN_H);
}

static uint32_t	tiles_that_fit(int32_t h, uint32_t count)
{
	int32_t	avail;
	int32_t	fit;

	avail = h - LOGON_BAND_TOP - LOGON_BAND_BOTTOM;
	fit = (avail - logon_card_h(0) + LOGON_TILE_GAP)
		/ (LOGON_TILE_H + LOGON_TILE_GAP);
	return ((uint32_t)lay_clamp((int32_t)count, 0,
			lay_clamp(fit, 1, LOGON_TILES_MAX)));
}

int	logon_layout(int32_t w, int32_t h, uint32_t count, t_logonlayout *out)
{
	int32_t	card_w;
	int32_t	card_h;
	int32_t	avail;

	if (!out || w < LOGON_MIN_W || h < LOGON_MIN_H)
		return (E_RANGE);
	memset(out, 0, sizeof(*out));
	out->count = tiles_that_fit(h, count);
	card_w = lay_clamp(w / 2 - 24, LOGON_CARD_W_MIN, LOGON_CARD_W_MAX);
	card_h = logon_card_h(out->count);
	avail = h - LOGON_BAND_TOP - LOGON_BAND_BOTTOM;
	out->card = lay_rect(w / 2 + (w / 2 - card_w) / 2,
			LOGON_BAND_TOP + (avail - card_h) / 2, card_w, card_h);
	out->prompt = lay_rect(40, h / 2 - 60, w / 2 - 80, 120);
	place_children(out);
	place_dialog(out);
	return ((int)out->count);
}
