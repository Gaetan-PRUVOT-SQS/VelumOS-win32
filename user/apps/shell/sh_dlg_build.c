#include "velum/libk.h"
#include "shell.h"

static int	add(t_ctlroot *r, t_ctlspec s)
{
	return (ctl_add(r, r->root, &s) != NULL);
}

static int	build_run(t_ctlroot *r)
{
	uint32_t	vis;

	vis = CTL_VISIBLE | CTL_ENABLED;
	return (add(r, (t_ctlspec){CT_LABEL, 10, vis, {16, 16, 340, 18},
			"Tapez le nom d'un programme, puis cliquez sur OK."})
		&& add(r, (t_ctlspec){CT_LABEL, 11, vis, {16, 50, 52, 18}, "Ouvrir :"})
		&& add(r, (t_ctlspec){CT_EDIT, SH_ID_EDIT, vis | CTL_FOCUSABLE,
			{72, 46, 284, 22}, ""})
		&& add(r, (t_ctlspec){CT_BUTTON, CTL_ID_OK, vis | CTL_FOCUSABLE
			| CTL_DEFAULT, {192, 82, 80, 26}, "OK"})
		&& add(r, (t_ctlspec){CT_BUTTON, CTL_ID_CANCEL, vis | CTL_FOCUSABLE,
			{276, 82, 80, 26}, "Annuler"}));
}

static int	build_power(t_ctlroot *r)
{
	uint32_t	vis;

	vis = CTL_VISIBLE | CTL_ENABLED;
	return (add(r, (t_ctlspec){CT_LABEL, 10, vis, {16, 16, 320, 18},
			"Que voulez-vous que l'ordinateur fasse ?"})
		&& add(r, (t_ctlspec){CT_BUTTON, SH_ID_HALT, vis | CTL_FOCUSABLE
			| CTL_DEFAULT, {16, 62, 100, 26}, "Arrêter"})
		&& add(r, (t_ctlspec){CT_BUTTON, SH_ID_REBOOT, vis | CTL_FOCUSABLE,
			{124, 62, 100, 26}, "Redémarrer"})
		&& add(r, (t_ctlspec){CT_BUTTON, CTL_ID_CANCEL, vis | CTL_FOCUSABLE,
			{232, 62, 100, 26}, "Annuler"}));
}

static int	build_note(t_ctlroot *r, const char *text)
{
	uint32_t	vis;
	char		first[SH_NOTE_MAX];
	const char	*cut;
	size_t		n;

	vis = CTL_VISIBLE | CTL_ENABLED | CTL_ELLIPSIS;
	cut = strchr(text, '\n');
	n = strlen(text);
	if (cut)
		n = (size_t)(cut - text);
	memcpy(first, text, n);
	first[n] = '\0';
	if (!add(r, (t_ctlspec){CT_LABEL, 10, vis, {16, 16, 300, 18}, first}))
		return (0);
	if (cut && !add(r, (t_ctlspec){CT_LABEL, 11, vis, {16, 36, 300, 18},
			cut + 1}))
		return (0);
	return (add(r, (t_ctlspec){CT_BUTTON, CTL_ID_CANCEL, vis | CTL_FOCUSABLE
			| CTL_DEFAULT, {236, 62, 80, 26}, "OK"}));
}

int	sh_dlg_build(t_shell *sh, int kind)
{
	t_ctlroot	*r;
	uint32_t	first;

	r = &sh->dlg.root;
	first = CTL_ID_OK;
	if (kind == SH_DLG_RUN && !build_run(r))
		return (-1);
	if (kind == SH_DLG_POWER && !build_power(r))
		return (-1);
	if (kind == SH_DLG_NOTE && !build_note(r, sh->note))
		return (-1);
	if (kind == SH_DLG_RUN)
		first = SH_ID_EDIT;
	if (kind == SH_DLG_POWER)
		first = SH_ID_HALT;
	if (kind == SH_DLG_NOTE)
		first = CTL_ID_CANCEL;
	ctl_focus(r, ctl_find(r, first));
	return (0);
}
