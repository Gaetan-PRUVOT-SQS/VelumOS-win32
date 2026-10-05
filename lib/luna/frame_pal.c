#include "luna_int.h"

static const t_lstop	g_act_st[] = {{0, 0xff2e86fb}, {2, 0xff1e73f5},
{6, 0xff0d63ee}, {12, 0xff0a5ce8}, {22, 0xff0453de}, {26, 0xff0a47c8},
{28, 0xff0b50d8}};
static const t_lstop	g_off_st[] = {{0, 0xffa9bdec}, {3, 0xff95abe5},
{8, 0xff88a2e1}, {22, 0xff7e99dd}, {26, 0xff7490d6}, {28, 0xff7a95db}};
static const t_lgrad	g_act = {g_act_st, 7, 0, 0};
static const t_lgrad	g_off = {g_off_st, 6, 0, 0};
static const t_color	g_ring_act[2] = {0xff0a33c0, 0xff3c86f7};
static const t_color	g_ring_off[2] = {0xff6b83c8, 0xffb3c3f0};

void	lp_frame_pal(bool active, t_lfpal *out)
{
	if (active)
	{
		out->ring[0] = g_ring_act[0];
		out->ring[1] = g_ring_act[1];
		out->fill = &g_act;
		out->text = LC_WHITE;
		out->shadow = 0xff0a1e78;
		return ;
	}
	out->ring[0] = g_ring_off[0];
	out->ring[1] = g_ring_off[1];
	out->fill = &g_off;
	out->text = 0xffd8e4f8;
	out->shadow = 0;
}
