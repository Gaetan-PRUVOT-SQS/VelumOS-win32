#include "velum/luna.h"
#include "startmenu.h"

static const t_smitem	g_items[SM_ITEMS] = {
{SMC_OPEN_PROGRAMS, SMCOL_LEFT, 0, 1, ICON_PROGRAM, "Tous les programmes"},
{SMC_LAUNCH_HELLO, SMCOL_LEFT, 0, 2, ICON_PROGRAM, "Bonjour"},
{SMC_BACK, SMCOL_LEFT, 1, 2, ICON_FOLDER, "Retour"},
{SMC_RUN, SMCOL_RIGHT, 0, 3, ICON_RUN, "Exécuter..."},
{SMC_LOGOFF, SMCOL_FOOT, 0, 3, ICON_USER, "Fermer la session"},
{SMC_SHUTDOWN, SMCOL_FOOT, 1, 3, ICON_POWER, "Éteindre l'ordinateur"}
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
