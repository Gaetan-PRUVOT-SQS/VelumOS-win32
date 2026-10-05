.PHONY: norme hdrcheck test-qemu test run run-uefi lot lot-compile norme-lot

norme:
	tools/norme.sh

hdrcheck:
	@for h in $$(find include -name '*.h' | sort); do \
		printf '#include "%s"\n' $${h#include/} | \
		$(CC) $(KCFLAGS) -x c -fsyntax-only - || exit 1; \
	done; echo 'en-têtes autonomes : OK'

LOTOBJ := $(patsubst %,$(O)/obj/%.o,$(KSRC_$(LOT))) $(ULOT_OBJ)

lot-compile: $(LOTOBJ)

norme-lot:
	@files=$$(find $(DIRS_$(LOT)) \( -name '*.c' -o -name '*.h' \) -print | sort); \
		tools/norme.sh $$files

lot: lot-compile host-$(LOT) norme-lot
	@echo 'lot $(LOT) : compilation, tests hôte et norme OK'

test-qemu: $(O)/kernel.elf $(O)/rootfs.stamp
	python3 tools/run_scenarios.py --kernel $(O)/kernel.elf --root $(O)/root \
		--out $(B)/qemu --resolution $(MODE)

test: norme test-host test-qemu

KVM := $(shell test -w /dev/kvm && echo -enable-kvm -cpu host)
UEFI_FLAGS := -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/ovmf/OVMF_CODE.fd \
	-drive if=pflash,format=raw,file=$(O)/ovmf_vars.fd

run: $(O)/disk.img
	qemu-system-x86_64 -machine q35 -m 256M $(KVM) -drive file=$(O)/disk.img,format=raw,snapshot=on \
		-serial stdio -vga std -device isa-debug-exit,iobase=0xf4,iosize=0x04

run-uefi: $(O)/disk.img
	cp /usr/share/edk2/ovmf/OVMF_VARS.fd $(O)/ovmf_vars.fd
	qemu-system-x86_64 -machine q35 -m 256M $(KVM) $(UEFI_FLAGS) \
		-drive file=$(O)/disk.img,format=raw,snapshot=on \
		-serial stdio -vga std -device isa-debug-exit,iobase=0xf4,iosize=0x04
