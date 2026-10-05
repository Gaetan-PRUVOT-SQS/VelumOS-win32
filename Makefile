SHELL := /bin/sh
.SUFFIXES:
export LC_ALL := C
export SOURCE_DATE_EPOCH := 1760000000

B ?= build/main
PROFIL ?= debug
O := $(B)/$(PROFIL)
CMDLINE ?= quiet
MODE ?= 1024x768
LOT ?= skel
V ?= 0
Q := @
ifeq ($(V),1)
Q :=
endif
HOSTCC ?= gcc

CC := x86_64-elf-gcc
LD := x86_64-elf-gcc
AR := x86_64-elf-ar
INC := -Iinclude -Ithird_party/limine -Iarch/x86_64/limine

KCFLAGS_COMMON := -std=gnu11 -ffreestanding -fno-pic -fno-pie -mcmodel=kernel \
	-mno-red-zone -mgeneral-regs-only -fno-omit-frame-pointer \
	-fno-strict-aliasing -fwrapv -fno-common -fno-delete-null-pointer-checks \
	-fstack-protector-strong -mstack-protector-guard=global \
	-ftrivial-auto-var-init=zero -fzero-call-used-regs=used-gpr \
	-ffunction-sections -fdata-sections -Wall -Wextra -Werror \
	-ffile-prefix-map=$(CURDIR)=. -DVELUM_KERNEL
KCFLAGS_debug := -O1 -g -fsanitize=undefined -fsanitize-undefined-trap-on-error \
	-DVELUM_DEBUG
KCFLAGS_release := -O2 -g -DNDEBUG
KCFLAGS := $(KCFLAGS_COMMON) $(KCFLAGS_$(PROFIL)) $(INC)
KASFLAGS := -ffreestanding -mno-red-zone -ffile-prefix-map=$(CURDIR)=. $(INC)

LOTS :=
KSRC :=
UAPPS :=
ULIBS :=
ROOTFS_FILES :=

include mk/skel.mk
include $(sort $(wildcard mk/lot-*.mk))
include mk/kernel.mk
include mk/user.mk
include mk/host.mk
include mk/test.mk

.DEFAULT_GOAL := all
.PHONY: all clean help

all: $(O)/kernel.elf $(O)/disk.img

clean:
	rm -rf $(B)

help:
	@printf '%s\n' 'make [B=build/x] [PROFIL=debug|release]' \
	'  all           noyau, applis et image disque' \
	'  ONLY="skel aNN"  ne construit que ces lots (noyau, libs, applis)' \
	'  norme         norminette sur tout le C' \
	'  lot LOT=aNN   compile, norme et tests hôte du lot' \
	'  test-host     tests unitaires hôte (ASan, UBSan)' \
	'  test-qemu     scénarios QEMU (BIOS et UEFI)' \
	'  test          norme, hôte, QEMU' \
	'  run           lance QEMU avec fenêtre (run-uefi en UEFI)'
