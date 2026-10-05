#include "luna_int.h"

void	luna_startmenu_layout(t_rect r, t_startlayout *out)
{
	const t_lunadetail	*d;
	int32_t				mid;
	int32_t				item;

	if (out == NULL)
		return ;
	r = lp_clean(r);
	d = lm_detail();
	mid = lp_max(r.h - d->menu_header_h - d->menu_footer_h, 0);
	item = lp_min(d->menu_foot_item_w, lp_max(r.w - 16, 0) / 2);
	out->header = lp_rect(r.x, r.y, r.w, d->menu_header_h);
	out->avatar = lp_rect(r.x + 6, r.y + (d->menu_header_h - d->avatar) / 2,
			d->avatar, d->avatar);
	out->left = lp_rect(r.x, r.y + d->menu_header_h, r.w / 2, mid);
	out->right = lp_rect(r.x + r.w / 2, r.y + d->menu_header_h, r.w - r.w / 2,
			mid);
	out->footer = lp_rect(r.x, r.y + r.h - d->menu_footer_h, r.w,
			d->menu_footer_h);
	out->poweroff = lp_rect(r.x + r.w - 8 - item, out->footer.y, item,
			d->menu_footer_h);
	out->logoff = lp_rect(out->poweroff.x - item, out->footer.y, item,
			d->menu_footer_h);
}

void	luna_startmenu_named(t_surface *s, t_rect r, const char *user)
{
	t_startlayout	lay;

	r = lp_clean(r);
	if (!lp_ok(s) || r.w < 40 || r.h < lm_detail()->menu_header_h
		+ lm_detail()->menu_footer_h)
		return ;
	luna_startmenu_layout(r, &lay);
	lp_sm_frame(s, r);
	lp_sm_columns(s, &lay);
	lp_sm_header(s, &lay, user);
	lp_sm_footer(s, &lay);
}

void	luna_startmenu(t_surface *s, t_rect r)
{
	luna_startmenu_named(s, r, "Utilisateur");
}
