#include "luna_int.h"

static const t_lstop	g_act_st[] = {{0, 0xff2e86fb}, {2, 0xff1e73f5},
{6, 0xff0d63ee}, {12, 0xff0a5ce8}, {22, 0xff0453de}, {26, 0xff0a47c8},
{28, 0xff0b50d8}};
static const t_lstop	g_off_st[] = {{0, 0xff8fa3d6}, {2, 0xff5c72ae},
{5, 0xff5a70ac}, {22, 0xff566ca8}, {26, 0xff4e639c}, {28, 0xff546aa4}};
static const t_lgrad	g_act = {g_act_st, 7, 0, 0};
static const t_lgrad	g_off = {g_off_st, 6, 0, 0};
static const t_color	g_ring_act[2] = {0xff0a33c0, 0xff3c86f7};
static const t_color	g_ring_off[2] = {0xff475c96, 0xff8c9fd2};

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
	out->text = LC_WHITE;
	out->shadow = 0;
}
