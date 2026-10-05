#include "velum/libk.h"
#include "logon.h"
#include "logon_text.h"

static int	add_tiles(t_logon *lg)
{
	t_ctlroot	*r;
	t_ctlspec	s;
	uint32_t	i;

	r = &lg->ui.root;
	i = 0;
	while (i < lg->lay.count)
	{
		s = (t_ctlspec){CT_BUTTON, LOGON_ID_TILE + i, CTL_VISIBLE | CTL_ENABLED
			| CTL_FOCUSABLE, lg->lay.tile[i], lg->set.list[lg->shown[i]].name};
		if (!ctl_add(r, r->root, &s))
			return (-1);
		i++;
	}
	return (0);
}

static int	add_login_row(t_logon *lg)
{
	t_ctlroot	*r;
	t_ctlspec	s;
	t_ctl		*edit;

	r = &lg->ui.root;
	s = (t_ctlspec){CT_EDIT, LOGON_ID_PW, CTL_ENABLED | CTL_FOCUSABLE
		| CTL_PASSWORD, lg->lay.password, ""};
	edit = ctl_add(r, r->root, &s);
	if (!edit)
		return (-1);
	ctl_edit_set_limit(r, edit, LOGON_PW_LIMIT);
	s = (t_ctlspec){CT_BUTTON, LOGON_ID_GO, CTL_ENABLED | CTL_FOCUSABLE
		| CTL_DEFAULT, lg->lay.go, "OK"};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_LABEL, LOGON_ID_MSG, CTL_VISIBLE | CTL_ENABLED
		| CTL_ELLIPSIS, lg->lay.message, ""};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_BUTTON, LOGON_ID_SHUT, CTL_VISIBLE | CTL_ENABLED
		| CTL_FOCUSABLE, lg->lay.shutdown, "Éteindre l'ordinateur"};
	return (ctl_add(r, r->root, &s) != NULL);
}

static int	add_dialog(t_logon *lg)
{
	t_ctlroot	*r;
	t_ctlspec	s;

	r = &lg->ui.root;
	s = (t_ctlspec){CT_LABEL, LOGON_ID_DLG_TEXT, CTL_ENABLED, lg->lay.dlg_text,
		LOGON_SHUTDOWN_ASK};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_BUTTON, CTL_ID_OK, CTL_ENABLED | CTL_FOCUSABLE
		| CTL_DEFAULT, lg->lay.dlg_ok, "Éteindre"};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_BUTTON, CTL_ID_CANCEL, CTL_ENABLED | CTL_FOCUSABLE,
		lg->lay.dlg_cancel, "Annuler"};
	return (ctl_add(r, r->root, &s) != NULL);
}

int	logon_build(t_logon *lg)
{
	if (add_tiles(lg) < 0 || add_login_row(lg) < 0 || add_dialog(lg) < 0)
		return (-1);
	logon_sync(lg);
	return (0);
}
