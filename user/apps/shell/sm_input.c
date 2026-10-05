#include "velum/abi/abi_input.h"
#include "startmenu.h"

int	sm_activate(t_startmenu *m, int32_t idx)
{
	const t_smitem	*it;

	if (!m->open || idx < 0 || !sm_visible((uint32_t)idx, m->level))
		return (SMC_NONE);
	it = sm_item((uint32_t)idx);
	if (it->cmd == SMC_OPEN_PROGRAMS)
	{
		m->level = SM_LEVEL_PROGRAMS;
		m->hot = SM_NO_ITEM;
		sm_move(m, 1);
	}
	else if (it->cmd == SMC_BACK)
	{
		m->level = SM_LEVEL_TOP;
		m->hot = 0;
	}
	else
		sm_close(m);
	return ((int)it->cmd);
}

static int	key_back(t_startmenu *m)
{
	if (m->level == SM_LEVEL_PROGRAMS)
	{
		m->level = SM_LEVEL_TOP;
		m->hot = 0;
	}
	else
		sm_close(m);
	return (SMC_NONE);
}

static int	key_enter(t_startmenu *m, uint32_t vk)
{
	const t_smitem	*it;

	if (m->hot < 0)
		return (SMC_NONE);
	it = sm_item((uint32_t)m->hot);
	if (vk == VK_RIGHT && it->cmd != SMC_OPEN_PROGRAMS)
		return (SMC_NONE);
	return (sm_activate(m, m->hot));
}

static int	key_edge(t_startmenu *m, uint32_t vk)
{
	m->hot = SM_NO_ITEM;
	if (vk == VK_HOME)
		sm_move(m, 1);
	else
		sm_move(m, -1);
	return (SMC_NONE);
}

int	sm_key(t_startmenu *m, uint32_t vk)
{
	if (!m->open)
		return (SMC_NONE);
	if (vk == VK_DOWN)
		sm_move(m, 1);
	else if (vk == VK_UP)
		sm_move(m, -1);
	else if (vk == VK_HOME || vk == VK_END)
		return (key_edge(m, vk));
	else if (vk == VK_RETURN || vk == VK_SPACE || vk == VK_RIGHT)
		return (key_enter(m, vk));
	else if (vk == VK_LEFT && m->level == SM_LEVEL_PROGRAMS)
		return (key_back(m));
	else if (vk == VK_ESCAPE)
		return (key_back(m));
	else if (vk == VK_LWIN || vk == VK_RWIN)
		sm_close(m);
	return (SMC_NONE);
}
