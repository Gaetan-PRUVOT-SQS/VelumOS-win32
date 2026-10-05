#include "velum/libk.h"
#include "startmenu.h"

void	sm_init(t_startmenu *m)
{
	memset(m, 0, sizeof(*m));
	m->hot = SM_NO_ITEM;
}

void	sm_open(t_startmenu *m)
{
	m->open = 1;
	m->level = SM_LEVEL_TOP;
	m->hot = SM_NO_ITEM;
}

void	sm_close(t_startmenu *m)
{
	m->open = 0;
	m->level = SM_LEVEL_TOP;
	m->hot = SM_NO_ITEM;
}
