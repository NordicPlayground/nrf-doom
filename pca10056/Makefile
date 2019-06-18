PROJECT_NAME     := fatfs_pca10056
TARGETS          := nrf52840_xxaa
OUTPUT_DIRECTORY := _build
MAKE := make

SDK_ROOT := ../../SDK
PROJ_DIR := ..

$(OUTPUT_DIRECTORY)/nrf52840_xxaa.out: \
  LINKER_SCRIPT  := fatfs_gcc_nrf52.ld

SRC_FILES =
INC_FOLDERS =

#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
#   $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \

# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/retarget.c \
  $(PROJ_DIR)/hardfault_handler_gcc.c \
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
  $(PROJ_DIR)/n_fs.c \
  $(PROJ_DIR)/n_mem.c \
  $(PROJ_DIR)/n_buttons.c \
  $(PROJ_DIR)/n_display.c \
  $(PROJ_DIR)/n_qspi.c \
  $(PROJ_DIR)/n_uart.c \
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
  $(PROJ_DIR)/doom/wi_stuff.c \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
  $(SDK_ROOT)/components/boards/boards.c \
  $(SDK_ROOT)/components/libraries/util/app_error.c \
  $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
  $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
  $(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
  $(SDK_ROOT)/components/libraries/sdcard/app_sdcard.c \
  $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
  $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
  $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
  $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
  $(SDK_ROOT)/components/libraries/block_dev/sdc/nrf_block_dev_sdc.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
  $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
  $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
  $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
  $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
  $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_spi.c \
  $(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_spi.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_spim.c \
  $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_qspi.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
  $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
  $(SDK_ROOT)/external/fatfs/port/diskio_blkdev.c \
  $(SDK_ROOT)/external/fatfs/src/ff.c \
  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR) \
  ../config \
  ../doom \
  config \
  $(SDK_ROOT)/components \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/external/fatfs/port \
  $(SDK_ROOT)/components/libraries/hardfault \
  $(SDK_ROOT)/components/libraries/fifo \
  $(SDK_ROOT)/components/libraries/strerror \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/external/protothreads/pt-1.4 \
  $(SDK_ROOT)/components/libraries/balloc \
  $(SDK_ROOT)/components/libraries/ringbuf \
  $(SDK_ROOT)/external/fatfs/src \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/components/libraries/bsp \
  $(SDK_ROOT)/components/libraries/uart \
  $(SDK_ROOT)/components/libraries/log \
  $(SDK_ROOT)/external/protothreads \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/components/libraries/experimental_section_vars \
  $(SDK_ROOT)/integration/nrfx/legacy \
  $(SDK_ROOT)/components/libraries/delay \
  $(SDK_ROOT)/external/segger_rtt \
  $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd \
  $(SDK_ROOT)/components/libraries/atomic \
  $(SDK_ROOT)/components/boards \
  $(SDK_ROOT)/components/libraries/memobj \
  $(SDK_ROOT)/integration/nrfx \
  $(SDK_ROOT)/components/libraries/block_dev/sdc \
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(SDK_ROOT)/components/libraries/block_dev \
  $(SDK_ROOT)/external/fprintf \
  $(SDK_ROOT)/components/libraries/sdcard \
  $(SDK_ROOT)/components/libraries/log/src \


# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10056
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52840_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-but-set-variable
# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52840_XXAA

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

nrf52840_xxaa: CFLAGS += -D__HEAP_SIZE=90000
nrf52840_xxaa: CFLAGS += -D__STACK_SIZE=2048
nrf52840_xxaa: ASMFLAGS += -D__HEAP_SIZE=90000
nrf52840_xxaa: ASMFLAGS += -D__STACK_SIZE=2048

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: nrf52840_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52840_xxaa
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall


.PHONY: debug

debug:
	arm-none-eabi-gdb -x debug.gdb _build/nrf52840_xxaa.out

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
