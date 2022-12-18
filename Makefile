include toolchain/makefile.conf

PROJECT = self_ballancer
TOOLCHAIN_DIR = toolchain

.PHONY: clean

OBJECTS += main.o
OBJECTS += toolchain/startup_ARMCM4.o

DEFINES=-D__STARTUP_CLEAR_BSS -D__START=main

TOOLCHAIN=arm-none-eabi-
CFLAGS=$(ARCH_FLAGS) $(DEFINES) $(CPU_DEFINES) $(INCLUDES) -Wall -ffunction-sections -fdata-sections -fno-builtin

LFLAGS=--specs=nosys.specs -Wl,--gc-sections -T$(TOOLCHAIN_DIR)/link.ld

%.o: %.S
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(TOOLCHAIN)gcc $(CFLAGS) -c -o $@ $<

$(PROJECT).bin: $(PROJECT).elf
	$(TOOLCHAIN)objcopy -O binary $< $@

$(PROJECT).elf: $(OBJECTS)
	$(TOOLCHAIN)gcc $(LFLAGS) $^ $(CFLAGS) -o $@
	
flash:
	st-flash --reset write $(PROJECT).bin 0x8000000

clean: 
	rm -f *.bin *.elf $(CPUDIR) output.txt
	find . -name '*.o' -delete