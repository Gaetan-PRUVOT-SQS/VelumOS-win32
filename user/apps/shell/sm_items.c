#include "velum/err.h"
#include "velum/libk.h"
#include "velum/luna.h"
#include "startmenu.h"

static t_smitem	g_items[SM_ITEMS] = {
{SMC_OPEN_PROGRAMS, SMCOL_LEFT, 0, 1, ICON_PROGRAM, "Tous les programmes"},
{SMC_LAUNCH_HELLO, SMCOL_LEFT, 0, 2, ICON_PROGRAM, "Bonjour"},
{SMC_BACK, SMCOL_LEFT, 1, 2, ICON_FOLDER, "Retour"},
{SMC_RUN, SMCOL_RIGHT, 0, 3, ICON_RUN, "Exécuter..."},
{SMC_LOGOFF, SMCOL_FOOT, 0, 3, ICON_USER, "Fermer la session"},
{SMC_SHUTDOWN, SMCOL_FOOT, 1, 3, ICON_POWER, "Éteindre l'ordinateur"},
{SMC_LAUNCH_B, SMCOL_LEFT, 1, 0, ICON_PROGRAM, ""},
{SMC_LAUNCH_C, SMCOL_LEFT, 2, 0, ICON_PROGRAM, ""}
};

const t_smitem	*sm_item(uint32_t index)
{
	if (index >= SM_ITEMS)
		return (NULL);
	return (&g_items[index]);
}

int	sm_visible(uint32_t index, uint32_t level)
{
	if (index >= SM_ITEMS || level > SM_LEVEL_PROGRAMS)
		return (0);
	return ((g_items[index].levels >> level) & 1);
}

static t_smitem	*prog_item(uint32_t i)
{
	if (i == 0)
		return (&g_items[SM_IDX_PROG]);
	return (&g_items[SM_ITEMS - SM_PROGS_MAX + i]);
}

int	sm_set_programs(const t_smprogs *p)
{
	static char	names[SM_PROGS_MAX][SM_PROG_NAME_MAX];
	t_smitem	*it;
	uint32_t	i;

	if (!p || p->count == 0 || p->count > SM_PROGS_MAX)
		return (E_INVAL);
	i = 0;
	while (i < SM_PROGS_MAX)
	{
		it = prog_item(i);
		it->levels = 0;
		if (i < p->count)
		{
			strlcpy(names[i], p->list[i].name, sizeof(names[i]));
			it->label = names[i];
			it->levels = 1u << SM_LEVEL_PROGRAMS;
		}
		i++;
	}
	g_items[SM_IDX_BACK].slot = p->count;
	return (0);
}

int32_t	sm_prog_of(uint32_t cmd)
{
	if (cmd == SMC_LAUNCH_HELLO)
		return (0);
	if (cmd == SMC_LAUNCH_B)
		return (1);
	if (cmd == SMC_LAUNCH_C)
		return (2);
	return (-1);
}
