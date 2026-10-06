#include <stdint.h>
#include "harness.h"
#include "velum/err.h"
#include "velum/libk.h"
#include "startmenu.h"

static const char	*g_bad[] = {
	"",
	"\n",
	"Bonjour\n",
	"Bonjour;\n",
	";/system/bin/hello\n",
	"A;/system/bin/a;b\n",
	"A;B;/system/bin/a\n",
	"A;system/bin/a\n",
	"A;/bin/a\n",
	"A;/system/\n",
	"A;/systemx/a\n",
	"A;/system/../bin/a\n",
	"A;/system/bin/..\n",
	"A;/system/./a\n",
	"A;/system//a\n",
	"A;/system/bin/\n",
	"A;/system/bin/a b\n",
	"A;/system/bin/a\r\n",
	"A\x01;/system/bin/a\n",
	"A\x7f;/system/bin/a\n",
	"A\xff;/system/bin/a\n",
	"A\xc3;/system/bin/a\n",
	"A;/system/bin/\xc3\xa9\n",
	"A;/system/a\n\nB;/system/b\n",
	"\nA;/system/a\n",
	"A;/system/a\nB\n",
	"A;/system/a\nB;/system/b\nC;/system/c\nD;/system/d\n"
};

static size_t	put_line(char *dst, size_t name_n, size_t path_n)
{
	memset(dst, 'N', name_n);
	dst[name_n] = ';';
	memset(dst + name_n + 1, 'p', path_n);
	memcpy(dst + name_n + 1, SM_PROG_ROOT, SM_PROG_ROOT_LEN);
	dst[name_n + 1 + path_n] = '\n';
	return (name_n + path_n + 2);
}

static void	programmes_fichier_hostile(void)
{
	t_smprogs	p;
	size_t		i;

	i = 0;
	while (i < sizeof(g_bad) / sizeof(g_bad[0]))
	{
		p.count = 9;
		h_eq_i64(g_bad[i], sm_progs_parse(g_bad[i], strlen(g_bad[i]), &p),
			E_INVAL);
		h_eq_u64("table videe", p.count, 0);
		i++;
	}
	h_eq_i64("tampon nul", sm_progs_parse(NULL, 4, &p), E_INVAL);
	h_eq_i64("zero au milieu", sm_progs_parse("A\0;/system/a\n", 13, &p),
		E_INVAL);
}

static void	programmes_valeurs_limites(void)
{
	char		buf[SM_PROGS_FILE_MAX + 8];
	t_smprogs	p;
	size_t		n;

	h_eq_i64("nom 31", sm_progs_parse(buf, put_line(buf, 31, 63), &p), 1);
	h_eq_u64("nom 31 copie", strlen(p.list[0].name), 31);
	h_eq_u64("chemin 63 copie", strlen(p.list[0].path), 63);
	h_eq_i64("nom 32", sm_progs_parse(buf, put_line(buf, 32, 63), &p),
		E_INVAL);
	h_eq_i64("chemin 64", sm_progs_parse(buf, put_line(buf, 31, 64), &p),
		E_INVAL);
	h_eq_i64("nom 1 chemin 9", sm_progs_parse(buf, put_line(buf, 1, 9), &p), 1);
	h_eq_i64("chemin 8", sm_progs_parse(buf, put_line(buf, 1, 8), &p), E_INVAL);
	n = put_line(buf, 31, 63);
	n += put_line(buf + n, 31, 63);
	n += put_line(buf + n, 31, 63);
	h_eq_u64("taille maximale", n, SM_PROGS_FILE_MAX);
	h_eq_i64("fichier plein", sm_progs_parse(buf, n, &p), SM_PROGS_MAX);
	buf[n] = '\n';
	h_eq_i64("un octet de trop", sm_progs_parse(buf, n + 1, &p), E_INVAL);
	h_eq_i64("sans fin de ligne", sm_progs_parse(buf, n - 1, &p), 3);
	h_eq_i64("deux lignes", sm_progs_parse("A;/system/a\nB;/system/b", 23, &p),
		2);
	h_eq_str("nom", p.list[1].name, "B");
	h_eq_str("chemin", p.list[1].path, "/system/b");
}

static void	programmes_menu(void)
{
	t_smprogs	p;

	h_eq_i64("utf-8", sm_progs_parse("\xc3\x89" "dit;/system/bin/e\nB;/system/b"
			"\nC;/system/c\n", 44, &p), 3);
	h_eq_i64("pose", sm_set_programs(&p), 0);
	h_eq_str("premier", sm_item(SM_IDX_PROG)->label, "\xc3\x89" "dit");
	h_eq_str("troisieme", sm_item(7)->label, "C");
	h_true(sm_visible(6, SM_LEVEL_PROGRAMS), "deuxieme visible");
	h_true(!sm_visible(7, SM_LEVEL_TOP), "cache au premier niveau");
	h_eq_u64("retour en dernier", sm_item(SM_IDX_BACK)->slot, 3);
	h_eq_i64("commande C", sm_prog_of(sm_item(7)->cmd), 2);
	h_eq_i64("commande autre", sm_prog_of(SMC_RUN), -1);
	p.count = SM_PROGS_MAX + 1;
	h_eq_i64("trop d'entrees", sm_set_programs(&p), E_INVAL);
	p.count = 0;
	h_eq_i64("aucune entree", sm_set_programs(&p), E_INVAL);
	sm_progs_default(&p);
	h_eq_i64("repli", sm_set_programs(&p), 0);
	h_eq_str("repli nom", sm_item(SM_IDX_PROG)->label, "Bonjour");
	h_eq_str("repli chemin", p.list[0].path, "/system/bin/hello");
	h_true(!sm_visible(6, SM_LEVEL_PROGRAMS), "deuxieme cache");
	h_eq_u64("retour remonte", sm_item(SM_IDX_BACK)->slot, 1);
}

int	main(void)
{
	h_begin("a20/programmes");
	h_run("programmes: fichier hostile", programmes_fichier_hostile);
	h_run("programmes: valeurs limites", programmes_valeurs_limites);
	h_run("programmes: menu", programmes_menu);
	return (h_end());
}
