PROJECT_NAME     := mb_ctrl
TARGETS          := mb_ctrl
OUTPUT_DIRECTORY := _build
MAKE := make

SDK_ROOT := ../nRF5_SDK
PROJ_DIR := .

$(OUTPUT_DIRECTORY)/mb_ctrl.out: \
  LINKER_SCRIPT  := nrf51_xxaa.ld

SRC_FILES =
INC_FOLDERS =

# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/main.c \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf51.S \
  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf51.c

# Include folders common to all targets
INC_FOLDERS += \
  $(PROJ_DIR) \
  $(PROJ_DIR)/config \
  $(SDK_ROOT)/config \
  $(SDK_ROOT)/modules/nrfx/mdk \
  $(SDK_ROOT)/components/toolchain/cmsis/Include \
  $(SDK_ROOT)/components/libraries/delay \
  $(SDK_ROOT)/components/libraries/util \
  $(SDK_ROOT)/modules/nrfx \
  $(SDK_ROOT)/modules/nrfx/hal \
  $(SDK_ROOT)/modules/nrfx/drivers/include

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
# CFLAGS += -DBOARD_PCA10056
# CFLAGS += -DBSP_DEFINES_ONLY
# CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF51
CFLAGS += -DNRF51822
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=soft
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
ASMFLAGS += -mcpu=cortex-m0
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=soft
# ASMFLAGS += -DBOARD_PCA10056
# ASMFLAGS += -DBSP_DEFINES_ONLY
# ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
# ASMFLAGS += -DFLOAT_ABI_HARD
# ASMFLAGS += -DNRF5340_XXAA_APPLICATION


# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -mfloat-abi=soft
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

mb_ctrl: CFLAGS += -D__HEAP_SIZE=8096
mb_ctrl: CFLAGS += -D__STACK_SIZE=2048
mb_ctrl: ASMFLAGS += -D__HEAP_SIZE=8096
mb_ctrl: ASMFLAGS += -D__STACK_SIZE=2048

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: mb_ctrl

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		mb_ctrl
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Edit this to match your devkit/debugger
SNR = 
# SNR = -s 1234567

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/mb_ctrl.hex
	nrfjprog $(SNR) -f nrf51 --program $(OUTPUT_DIRECTORY)/mb_ctrl.hex --sectorerase
	nrfjprog $(SNR) -f nrf51 --reset

reset:
	nrfjprog $(SNR) -f nrf51 --reset
halt:
	nrfjprog $(SNR) -f nrf51 --halt
erase:
	nrfjprog -f nrf51 --eraseall

.PHONY: copy

# Copy to micro:bit virtual drive on Windows
copy: default
	cp _build/mb_ctrl.hex D:

.PHONY: debug