PROJECT_NAME     := nrfdoom_net
TARGETS          := nrfdoom_net
OUTPUT_DIRECTORY := _build
MAKE := make

SDK_ROOT := ../nRF5_SDK
PROJ_DIR := .


$(OUTPUT_DIRECTORY)/nrfdoom_net.out: \
  LINKER_SCRIPT  := nrf5340_xxaa_network.ld

SRC_FILES =
INC_FOLDERS =


# Source files common to all targets
SRC_FILES += \
  $(PROJ_DIR)/main.c \
  $(PROJ_DIR)/../uart_common/uart.c \
  $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf5340_network.S \
  $(SDK_ROOT)/modules/nrfx/mdk/system_nrf5340_network.c

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
  $(SDK_ROOT)/modules/nrfx/drivers/include \
  $(PROJ_DIR)/../uart_common \

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
# CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF5340_XXAA_NETWORK
CFLAGS += -DNRF_MCU_NETWORK
CFLAGS += -mcpu=cortex-m33+nodsp
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
ASMFLAGS += -mcpu=cortex-m33+nodsp
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=soft
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
# ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF5340_XXAA_NETWORK

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=soft
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

nrfdoom_net: CFLAGS += -D__HEAP_SIZE=8096
nrfdoom_net: CFLAGS += -D__STACK_SIZE=1024
nrfdoom_net: ASMFLAGS += -D__HEAP_SIZE=8096
nrfdoom_net: ASMFLAGS += -D__STACK_SIZE=1024

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: nrfdoom_net

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrfdoom_net
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

SNR = 
# SNR = -s 960192176

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/nrfdoom_net.hex
	nrfjprog -f nrf53 --coprocessor CP_NETWORK --program $(OUTPUT_DIRECTORY)/nrfdoom_net.hex --sectorerase --log
	nrfjprog -f nrf53 --reset

reset:
	nrfjprog  -f nrf53 --reset

erase:
	nrfjprog -f nrf53 --eraseall

halt:
	nrfjprog -f nrf53 --coprocessor CP_APPLICATION --halt
	nrfjprog -f nrf53 --coprocessor CP_NETWORK     --halt

run:
	nrfjprog -f nrf53 --coprocessor CP_APPLICATION --run
	nrfjprog -f nrf53 --coprocessor CP_NETWORK     --run

.PHONY: debug
