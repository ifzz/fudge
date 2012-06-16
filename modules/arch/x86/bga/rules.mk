BUILD+=modules/arch/x86/bga/bga.ko
CLEAN+=$(OBJ_modules/arch/x86/bga/bga.ko) modules/arch/x86/bga/bga.ko

OBJ_modules/arch/x86/bga/bga.ko:=modules/arch/x86/bga/main.o modules/arch/x86/bga/driver.o

modules/arch/x86/bga/bga.ko: lib/memory.o lib/string.o lib/arch/x86/io.o $(OBJ_modules/arch/x86/bga/bga.ko)
	$(LD) $(LDFLAGS) -o $@ $^
