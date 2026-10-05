#include "harness.h"
#include "fake.h"

void	fake_begin(t_fakeui *u, int w, int h)
{
	fake_mem_reset(0);
	h_eq_i64("ouverture de l'interface", fake_ui_open(u, w, h), 0);
}

void	fake_done(t_fakeui *u)
{
	fake_ui_close(u);
	h_eq_i64("aucune fuite", fake_mem_live(), 0);
}
