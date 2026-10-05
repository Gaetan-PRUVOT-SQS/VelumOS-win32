#include "velum/libk.h"
#include "../common/platform.h"
#include "../common/tbuf.h"
#include "../common/timefmt.h"
#include "hello.h"

static int	add_labels(t_ctlroot *r)
{
	t_ctlspec	s;

	s = (t_ctlspec){CT_LABEL, HELLO_ID_TITLE, CTL_VISIBLE | CTL_ENABLED
		| CTL_BOLD, {16, 16, 300, 20}, "Bonjour de VelumOS !"};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_LABEL, HELLO_ID_UPTIME, CTL_VISIBLE | CTL_ENABLED
		| CTL_ELLIPSIS, {16, 52, 300, 18}, ""};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	s = (t_ctlspec){CT_LABEL, HELLO_ID_NOTE, CTL_VISIBLE | CTL_ENABLED
		| CTL_ELLIPSIS, {16, 84, 300, 18},
		"Application de démonstration de la session."};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	return (0);
}

int	hello_build(t_hello *app)
{
	t_ctlroot	*r;
	t_ctlspec	s;

	r = &app->ui.root;
	if (add_labels(r) < 0)
		return (-1);
	s = (t_ctlspec){CT_BUTTON, CTL_ID_CANCEL, CTL_VISIBLE | CTL_ENABLED
		| CTL_FOCUSABLE | CTL_DEFAULT, {236, 142, 80, 26}, "Fermer"};
	if (!ctl_add(r, r->root, &s))
		return (-1);
	ctl_focus(r, ctl_find(r, CTL_ID_CANCEL));
	return (0);
}

void	hello_on_command(void *c, uint32_t code, void *user)
{
	const t_ctl	*ctl;
	t_hello		*app;

	ctl = c;
	app = user;
	if (app && code == CN_CLICKED && ctl->id == CTL_ID_CANCEL)
		app->quit = 1;
}

void	hello_tick(t_hello *app)
{
	t_sysinfo	si;
	char		text[96];
	char		up[48];
	t_tbuf		b;
	t_ctl		*label;

	label = ctl_find(&app->ui.root, HELLO_ID_UPTIME);
	if (!label || os_sysinfo(&si) < 0)
		return ;
	fmt_uptime(si.uptime_ns, up, sizeof(up));
	tb_init(&b, text, sizeof(text));
	tb_str(&b, "Temps de fonctionnement : ");
	tb_str(&b, up);
	ctl_set_text(&app->ui.root, label, text);
	uiwin_flush(&app->ui);
}
