################################################################################
##
## This file is part of the coreboot project.
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
################################################################################

ppc64_flags = -I$(src)/arch/ppc64/ -mbig-endian -mcpu=power8 -mtune=power8

ppc64_asm_flags =

################################################################################
## bootblock
################################################################################
ifeq ($(CONFIG_ARCH_BOOTBLOCK_PPC64),y)

bootblock-y = bootblock.S stages.c
bootblock-y += boot.c
bootblock-y += rom_media.c
bootblock-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

bootblock-generic-ccopts += $(ppc64_flags)

$(objcbfs)/bootblock.debug: $$(bootblock-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_bootblock) $(LDFLAGS_bootblock) -o $@ -L$(obj) \
		-T $(call src-to-obj,bootblock,src/mainboard/$(MAINBOARDDIR)/memlayout.ld) --whole-archive --start-group $(filter-out %.ld,$(bootblock-objs)) \
		$(LIBGCC_FILE_NAME_bootblock) --end-group $(COMPILER_RT_bootblock)

endif

################################################################################
## romstage
################################################################################
ifeq ($(CONFIG_ARCH_ROMSTAGE_PPC64),y)

romstage-y += boot.c
romstage-y += stages.c
romstage-y += rom_media.c
romstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

romstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

# Build the romstage

$(objcbfs)/romstage.debug: $$(romstage-objs)
	@printf "    LINK       $(subst $(obj)/,,$(@))\n"
	$(LD_romstage) $(LDFLAGS_romstage) -o $@ -L$(obj) -T $(call src-to-obj,romstage,src/mainboard/$(MAINBOARDDIR)/memlayout.ld) --whole-archive --start-group $(filter-out %.ld,$(romstage-objs)) --end-group $(COMPILER_RT_romstage)

romstage-c-ccopts += $(ppc64_flags)
romstage-S-ccopts += $(ppc64_asm_flags)

endif

################################################################################
## ramstage
################################################################################
ifeq ($(CONFIG_ARCH_RAMSTAGE_PPC64),y)

ramstage-y += rom_media.c
ramstage-y += stages.c
ramstage-y += boot.c
ramstage-y += tables.c
ramstage-y += \
	$(top)/src/lib/memchr.c \
	$(top)/src/lib/memcmp.c \
	$(top)/src/lib/memcpy.c \
	$(top)/src/lib/memmove.c \
	$(top)/src/lib/memset.c

$(eval $(call create_class_compiler,rmodules,power8))

ramstage-$(CONFIG_COLLECT_TIMESTAMPS) += timestamp.c

ramstage-srcs += src/mainboard/$(MAINBOARDDIR)/mainboard.c

# Build the ramstage

$(objcbfs)/ramstage.debug: $$(ramstage-objs)
	@printf "    CC         $(subst $(obj)/,,$(@))\n"
	$(LD_ramstage) $(LDFLAGS_ramstage) -o $@ -L$(obj) -T $(call src-to-obj,ramstage,src/mainboard/$(MAINBOARDDIR)/memlayout.ld) --whole-archive --start-group $(filter-out %.ld,$(ramstage-objs)) --end-group $(COMPILER_RT_ramstage)

ramstage-c-ccopts += $(ppc64_flags)
ramstage-S-ccopts += $(ppc64_asm_flags)

endif
