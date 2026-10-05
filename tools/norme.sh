#!/bin/sh
set -eu
export PATH="$HOME/.local/bin:$PATH"
racine=$(cd "$(dirname "$0")/.." && pwd)
cd "$racine"
mkdir -p build
liste=build/norme.$$.liste
if [ "$#" -eq 0 ]; then
	find . \( -name build -o -name .git \) -prune -o \
		\( -name '*.c' -o -name '*.h' \) -print | sort >"$liste"
else
	printf '%s\n' "$@" >"$liste"
fi

exclu()
{
	d=$(dirname "$1")
	while [ "$d" != "." ] && [ "$d" != "/" ]; do
		[ -e "$d/.no-norminette" ] && return 0
		d=$(dirname "$d")
	done
	return 1
}

mauvais=0
total=0
while IFS= read -r f; do
	exclu "$f" && continue
	total=$((total + 1))
	sortie=$(norminette "$f" 2>&1 | grep -E '^Error: ' | grep -v INVALID_HEADER || true)
	if [ -n "$sortie" ]; then
		printf '%s\n%s\n' "$f" "$sortie" >&2
		mauvais=$((mauvais + 1))
	fi
done <"$liste"
rm -f "$liste"
if [ "$mauvais" -ne 0 ]; then
	printf 'norme KO : %s fichier(s) sur %s\n' "$mauvais" "$total" >&2
	exit 1
fi
printf 'norme OK : %s fichier(s)\n' "$total"
