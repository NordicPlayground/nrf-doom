PROJECT_NAME     := doom_nrf53p3
TARGETS          := doom_nrf53p3
OUTPUT_DIRECTORY := _build
MAKE := make

SDK_ROOT := ../../../nRF5_SDK
PROJ_DIR := ../../source

$(OUTPUT_DIRECTORY)/doom_nrf53p3.out: \
	LINKER_SCRIPT  := nrfdoom_gcc_nrf53.ld

SRC_FILES =
INC_FOLDERS =

# Source files common to all targets
SRC_FILES += \
	$(PROJ_DIR)/main.c \
	$(PROJ_DIR)/retarget.c \
	$(PROJ_DIR)/n_uart.c \
	$(PROJ_DIR)/n_qspi.c \
	$(PROJ_DIR)/n_fs.c \
	$(PROJ_DIR)/n_mem.c \
	$(PROJ_DIR)/n_buttons.c \
 	$(PROJ_DIR)/n_display.c \
	$(PROJ_DIR)/n_rjoy.c \
	$(PROJ_DIR)/n_i2s.c \
	$(PROJ_DIR)/n_i2s_sound.c \
	$(PROJ_DIR)/deh_main.c \
	$(PROJ_DIR)/d_event.c \
	$(PROJ_DIR)/d_iwad.c \
	$(PROJ_DIR)/d_mode.c \
	$(PROJ_DIR)/d_loop.c \
	$(PROJ_DIR)/i_timer.c \
	$(PROJ_DIR)/i_system.c \
	$(PROJ_DIR)/i_input.c \
	$(PROJ_DIR)/i_video.c \
	$(PROJ_DIR)/i_joystick.c \
	$(PROJ_DIR)/i_sound.c \
	$(PROJ_DIR)/m_argv.c \
	$(PROJ_DIR)/m_bbox.c \
	$(PROJ_DIR)/m_cheat.c \
	$(PROJ_DIR)/m_config.c \
	$(PROJ_DIR)/m_fixed.c \
	$(PROJ_DIR)/m_misc.c \
	$(PROJ_DIR)/m_controls.c \
	$(PROJ_DIR)/net_client.c \
	$(PROJ_DIR)/net_server.c \
	$(PROJ_DIR)/tables.c \
	$(PROJ_DIR)/v_diskicon.c \
	$(PROJ_DIR)/v_video.c \
	$(PROJ_DIR)/w_checksum.c \
	$(PROJ_DIR)/w_file.c \
	$(PROJ_DIR)/w_file_fatfs.c \
	$(PROJ_DIR)/w_main.c \
	$(PROJ_DIR)/w_wad.c \
	$(PROJ_DIR)/z_native.c \
	$(PROJ_DIR)/doom/am_map.c \
	$(PROJ_DIR)/doom/doomstat.c \
	$(PROJ_DIR)/doom/d_main.c \
	$(PROJ_DIR)/doom/d_net.c \
	$(PROJ_DIR)/doom/d_items.c \
	$(PROJ_DIR)/doom/dstrings.c \
	$(PROJ_DIR)/doom/f_wipe.c \
	$(PROJ_DIR)/doom/f_finale.c \
	$(PROJ_DIR)/doom/g_game.c \
	$(PROJ_DIR)/doom/hu_lib.c \
	$(PROJ_DIR)/doom/hu_stuff.c \
	$(PROJ_DIR)/doom/info.c \
	$(PROJ_DIR)/doom/m_menu.c \
	$(PROJ_DIR)/doom/m_random.c \
	$(PROJ_DIR)/doom/p_doors.c \
	$(PROJ_DIR)/doom/p_ceilng.c \
	$(PROJ_DIR)/doom/p_enemy.c \
	$(PROJ_DIR)/doom/p_floor.c \
	$(PROJ_DIR)/doom/p_lights.c \
	$(PROJ_DIR)/doom/p_plats.c \
	$(PROJ_DIR)/doom/p_telept.c \
	$(PROJ_DIR)/doom/p_setup.c \
	$(PROJ_DIR)/doom/p_sight.c \
	$(PROJ_DIR)/doom/p_saveg.c \
	$(PROJ_DIR)/doom/p_map.c \
	$(PROJ_DIR)/doom/p_maputl.c \
	$(PROJ_DIR)/doom/p_mobj.c \
	$(PROJ_DIR)/doom/p_inter.c \
	$(PROJ_DIR)/doom/p_tick.c \
	$(PROJ_DIR)/doom/p_spec.c \
	$(PROJ_DIR)/doom/p_switch.c \
	$(PROJ_DIR)/doom/p_pspr.c \
	$(PROJ_DIR)/doom/p_user.c \
	$(PROJ_DIR)/doom/r_bsp.c \
	$(PROJ_DIR)/doom/r_main.c \
	$(PROJ_DIR)/doom/r_data.c \
	$(PROJ_DIR)/doom/r_draw.c \
	$(PROJ_DIR)/doom/r_plane.c \
	$(PROJ_DIR)/doom/r_segs.c \
	$(PROJ_DIR)/doom/r_sky.c \
	$(PROJ_DIR)/doom/r_things.c \
	$(PROJ_DIR)/doom/s_sound.c \
	$(PROJ_DIR)/doom/st_lib.c \
	$(PROJ_DIR)/doom/st_stuff.c \
	$(PROJ_DIR)/doom/statdump.c \
	$(PROJ_DIR)/doom/sounds.c \
	$(PROJ_DIR)/doom/wi_stuff.c \
	$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf5340_application.S \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf5340_application.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_qspi.c  \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_spim.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_spi.c \
	$(SDK_ROOT)/components/libraries/block_dev/sdc/nrf_block_dev_sdc.c \
	$(SDK_ROOT)/components/libraries/sdcard/app_sdcard.c \
	$(SDK_ROOT)/external/fatfs/port/diskio_blkdev.c \
	$(SDK_ROOT)/external/fatfs/src/ff.c

# Include folders common to all targets
INC_FOLDERS += \
	../config \
	$(PROJ_DIR) \
	$(PROJ_DIR)/config \
	$(SDK_ROOT)/modules/nrfx/mdk \
	$(SDK_ROOT)/components/toolchain/cmsis/Include \
	$(SDK_ROOT)/modules/nrfx \
	$(SDK_ROOT)/modules/nrfx/hal \
	$(SDK_ROOT)/modules/nrfx/drivers/include \
	$(SDK_ROOT)/integration/nrfx/legacy \
	$(SDK_ROOT)/components/libraries/delay \
	$(SDK_ROOT)/components/libraries/util \
	$(SDK_ROOT)/components/libraries/block_dev/sdc \
	$(SDK_ROOT)/components/libraries/sdcard \
	$(SDK_ROOT)/components/libraries/block_dev \
	$(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd \
	$(SDK_ROOT)/external/fatfs/port \
	$(SDK_ROOT)/external/fatfs/src \
	$(SDK_ROOT)/external/protothreads/pt-1.4 \
	$(SDK_ROOT)/external/protothreads 

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF5340_XXAA_APPLICATION
CFLAGS += -mcpu=cortex-m33
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
CFLAGS += -Wno-error=char-subscripts
CFLAGS += -Wno-error=address-of-packed-member
# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m33
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF5340_XXAA_APPLICATION

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m33
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

doom_nrf53p3: CFLAGS += -D__HEAP_SIZE=262144
doom_nrf53p3: CFLAGS += -D__STACK_SIZE=2048
doom_nrf53p3: ASMFLAGS += -D__HEAP_SIZE=262144
doom_nrf53p3: ASMFLAGS += -D__STACK_SIZE=2048

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: doom_nrf53p3

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo   doom_nrf53p3
	@echo   sdk_config - starting external tool for editing sdk_config.h
	@echo   flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/doom_nrf53p3.hex
	nrfjprog -f nrf53 --coprocessor CP_APPLICATION --program $(OUTPUT_DIRECTORY)/doom_nrf53p3.hex --sectorerase
	nrfjprog -f nrf53 --reset

reset:
	nrfjprog -f nrf53 --reset
erase:
	nrfjprog -f nrf53 --eraseall
halt:
	nrfjprog -f nrf53 --halt


.PHONY: debug

debug:
	arm-none-eabi-gdb -x debug.gdb _build/doom_nrf53p3.out
