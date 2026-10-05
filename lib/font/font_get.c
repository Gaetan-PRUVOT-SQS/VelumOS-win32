#include "font_int.h"

const t_font	*font_get(t_fontid id)
{
	if (id == FONT_UI)
		return (&g_font_ui);
	if (id == FONT_UI_BOLD)
		return (&g_font_ui_bold);
	if (id == FONT_TITLE)
		return (&g_font_title);
	if (id == FONT_MONO)
		return (&g_font_mono);
	return (NULL);
}
