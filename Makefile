SRC_DIR := src
INC_DIR := include
DOC_DIR := doc
BIN_DIR := bin
LIB_DIR := lib

PRE_LSCRIPT := kernel.lds
LSCRIPT := $(BIN_DIR)/kernel_generated.lds

CROSS := aarch64-none-elf
CC    := $(CROSS)-gcc
CPP   := $(CROSS)-cpp
C++   := $(CROSS)-g++
AS    := $(CROSS)-gcc
AR    := $(CROSS)-ar
LD    := $(CROSS)-gcc
OBJCPY:= $(CROSS)-objcopy
OBJDP := $(CROSS)-objdump

ASFLAGS := 
CFLAGS  := -ffreestanding -Wall -Wextra -O2 -g
CFLAGS  += -I$(INC_DIR)
LDFLAGS := -ffreestanding -O2 -nostdlib -L$(LIB_DIR)
LIBFLAGS:= 
ARFLAGS := -crs
CPYFLAGS:= 
DPFLAGS := -fhDs

KSRC := $(wildcard $(SRC_DIR)/kernel/aarch64/*.c) $(wildcard $(SRC_DIR)/kernel/aarch64/*.S)

KOBJ := $(patsubst %.c,%.o,$(filter %.c,$(KSRC)))
KOBJ += $(patsubst %.S,%.o,$(filter %.S,$(KSRC)))

OBJ := $(KOBJ)
ALL_OBJ := $(OBJ)

DEP := $(OBJ:.o=.d)

KERNEL := $(BIN_DIR)/kernel.elf
KERNEL_IMG := $(patsubst %.elf,%.img,$(KERNEL))

QEMU_SYSTEM_AARCH64= qemu-system-aarch64
QEMU= $(QEMU_SYSTEM_AARCH64) -M raspi3b -nographic

#
# Regeln
#
.PHONY: all
all: $(KERNEL_IMG)

-include $(DEP)

%.o: %.S
	$(AS) $(ASFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

$(LSCRIPT): $(PRE_LSCRIPT)
	$(CPP) $(CFLAGS) $(PRE_LSCRIPT) | grep -v '^#'  > $(LSCRIPT)

$(KERNEL): $(LSCRIPT) $(OBJ) $(LIB)
	$(LD) $(LDFLAGS) -T$(LSCRIPT) -o $@ $(OBJ)

$(KERNEL_IMG): $(KERNEL)
	$(OBJCPY) $< -O binary $@

.PHONY: clean
clean:
	rm -f $(KERNEL_IMG)
	rm -f $(KERNEL)
	rm -f $(LSCRIPT)
	rm -f $(ALL_OBJ)
	rm -f $(DEP)

.PHONY: run
run: $(KERNEL)
	$(QEMU) -kernel $(KERNEL)

.PHONY: dump
dump: $(KERNEL)
	$(OBJDP) $(DPFLAGS) $(KERNEL)

.PHONY: debug
debug: $(KERNEL)
	$(QEMU) -kernel $(KERNEL) -gdb tcp:localhost:1234 -S
