#!/bin/sh
set -eu

racine=$(CDPATH='' cd -- "$(dirname -- "$0")/.." && pwd)
cd -- "$racine"

sortie="${B:-build/main}"
taches=$(nproc 2>/dev/null || echo 2)
lots_hote="a02 a03 a04 a08 a09 a11 a13 d00 d07 d11"
scenarios="a01_boot skel_boot int_session a20_desktop a12_fat32 int_apk_installation"

etape()
{
	printf '\n== %s\n' "$1" >&2
}

lancer_scenario()
{
	python3 tools/run_scenarios.py --kernel "$sortie/debug/kernel.elf" \
		--root "$sortie/debug/root" --out "$sortie/qemu" --filter "$1" --jobs 2
}

etape "Limine"
make limine

etape "Compilation avec le gcc du système"
make CROSS= B="$sortie" -j"$taches" all

etape "Outils et contrôles statiques"
python3 -m pytest -q tests/tools
python3 tools/matrice_couverture.py --check
python3 tools/scan_securite.py

etape "Tests unitaires hôte"
set --
for lot in $lots_hote; do
	set -- "$@" "host-$lot"
done
make CROSS= B="$sortie" -j"$taches" "$@"

etape "Scénarios QEMU de contrôle, sans KVM"
VTEST_SANS_KVM=1
export VTEST_SANS_KVM
for scenario in $scenarios; do
	lancer_scenario "$scenario"
done

etape "Terminé"
