#!/bin/sh

DIR_IMAGE="root"
DIR_IMAGE_BIN="root/bin"
DIR_IMAGE_TXT="root/txt"
DIR_SOURCE_ARCH_KERNEL="../arch/x86/kernel"
DIR_SOURCE_ARCH_LIB="../arch/x86/lib"
DIR_SOURCE_ARCH_MODULES="../arch/x86/modules"
DIR_SOURCE_BIN="../bin"
DIR_SOURCE_KERNEL="../kernel"
DIR_SOURCE_LIB="../lib"
DIR_SOURCE_MODULES="../modules"
DIR_SOURCE_TOOLS="../tools"

ASM="nasm"
ASMFLAGS="-f elf"
GCC="gcc"
GCCFLAGS="-c -O2 -I../include -Wall -Wextra -ffreestanding -nostdlib -nostartfiles -nodefaultlibs"
LD="ld"
LDFLAGS=-"T./linker-x86.ld"

$GCC $GCCFLAGS $DIR_SOURCE_KERNEL/dev.c -o $DIR_SOURCE_KERNEL/dev.o
$GCC $GCCFLAGS $DIR_SOURCE_KERNEL/initrd.c -o $DIR_SOURCE_KERNEL/initrd.o
$GCC $GCCFLAGS $DIR_SOURCE_KERNEL/kernel.c -o $DIR_SOURCE_KERNEL/kernel.o
$GCC $GCCFLAGS $DIR_SOURCE_KERNEL/shell.c -o $DIR_SOURCE_KERNEL/shell.o
$GCC $GCCFLAGS $DIR_SOURCE_KERNEL/vfs.c -o $DIR_SOURCE_KERNEL/vfs.o
$GCC $GCCFLAGS $DIR_SOURCE_MODULES/elf/elf.c -o $DIR_SOURCE_MODULES/elf/elf.o
$GCC $GCCFLAGS $DIR_SOURCE_MODULES/tty/tty.c -o $DIR_SOURCE_MODULES/tty/tty.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/arch.c -o $DIR_SOURCE_ARCH_KERNEL/arch.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/cr.s -o $DIR_SOURCE_ARCH_KERNEL/cr.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/fpu.c -o $DIR_SOURCE_ARCH_KERNEL/fpu.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/fpus.s -o $DIR_SOURCE_ARCH_KERNEL/fpus.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/gdt.c -o $DIR_SOURCE_ARCH_KERNEL/gdt.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/gdts.s -o $DIR_SOURCE_ARCH_KERNEL/gdts.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/idt.c -o $DIR_SOURCE_ARCH_KERNEL/idt.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/idts.s -o $DIR_SOURCE_ARCH_KERNEL/idts.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/init.s -o $DIR_SOURCE_ARCH_KERNEL/init.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/io.c -o $DIR_SOURCE_ARCH_KERNEL/io.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/ios.s -o $DIR_SOURCE_ARCH_KERNEL/ios.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/irq.c -o $DIR_SOURCE_ARCH_KERNEL/irq.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/irqs.s -o $DIR_SOURCE_ARCH_KERNEL/irqs.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/isr.c -o $DIR_SOURCE_ARCH_KERNEL/isr.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_KERNEL/isrs.s -o $DIR_SOURCE_ARCH_KERNEL/isrs.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/mmu.c -o $DIR_SOURCE_ARCH_KERNEL/mmu.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_KERNEL/syscall.c -o $DIR_SOURCE_ARCH_KERNEL/syscall.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_MODULES/ata/ata.c -o $DIR_SOURCE_ARCH_MODULES/ata/ata.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_MODULES/kbd/kbd.c -o $DIR_SOURCE_ARCH_MODULES/kbd/kbd.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_MODULES/pit/pit.c -o $DIR_SOURCE_ARCH_MODULES/pit/pit.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_MODULES/rtc/rtc.c -o $DIR_SOURCE_ARCH_MODULES/rtc/rtc.o
$GCC $GCCFLAGS $DIR_SOURCE_ARCH_MODULES/vga/vga.c -o $DIR_SOURCE_ARCH_MODULES/vga/vga.o
$GCC $GCCFLAGS $DIR_SOURCE_LIB/file.c -o $DIR_SOURCE_LIB/file.o
$GCC $GCCFLAGS $DIR_SOURCE_LIB/memory.c -o $DIR_SOURCE_LIB/memory.o
$GCC $GCCFLAGS $DIR_SOURCE_LIB/session.c -o $DIR_SOURCE_LIB/session.o
$GCC $GCCFLAGS $DIR_SOURCE_LIB/string.c -o $DIR_SOURCE_LIB/string.o
$ASM $ASMFLAGS $DIR_SOURCE_ARCH_LIB/calls.s -o $DIR_SOURCE_ARCH_LIB/calls.o

$LD $LDFLAGS \
    $DIR_SOURCE_KERNEL/dev.o \
    $DIR_SOURCE_KERNEL/initrd.o \
    $DIR_SOURCE_KERNEL/kernel.o \
    $DIR_SOURCE_KERNEL/shell.o \
    $DIR_SOURCE_KERNEL/vfs.o \
    $DIR_SOURCE_MODULES/elf/elf.o \
    $DIR_SOURCE_MODULES/tty/tty.o \
    $DIR_SOURCE_ARCH_KERNEL/arch.o \
    $DIR_SOURCE_ARCH_KERNEL/cr.o \
    $DIR_SOURCE_ARCH_KERNEL/fpu.o \
    $DIR_SOURCE_ARCH_KERNEL/fpus.o \
    $DIR_SOURCE_ARCH_KERNEL/gdt.o \
    $DIR_SOURCE_ARCH_KERNEL/gdts.o \
    $DIR_SOURCE_ARCH_KERNEL/idt.o \
    $DIR_SOURCE_ARCH_KERNEL/idts.o \
    $DIR_SOURCE_ARCH_KERNEL/init.o \
    $DIR_SOURCE_ARCH_KERNEL/io.o \
    $DIR_SOURCE_ARCH_KERNEL/ios.o \
    $DIR_SOURCE_ARCH_KERNEL/irqs.o \
    $DIR_SOURCE_ARCH_KERNEL/isrs.o \
    $DIR_SOURCE_ARCH_KERNEL/irq.o \
    $DIR_SOURCE_ARCH_KERNEL/isr.o \
    $DIR_SOURCE_ARCH_KERNEL/mmu.o \
    $DIR_SOURCE_ARCH_KERNEL/syscall.o \
    $DIR_SOURCE_ARCH_MODULES/ata/ata.o \
    $DIR_SOURCE_ARCH_MODULES/kbd/kbd.o \
    $DIR_SOURCE_ARCH_MODULES/pit/pit.o \
    $DIR_SOURCE_ARCH_MODULES/rtc/rtc.o \
    $DIR_SOURCE_ARCH_MODULES/vga/vga.o \
    $DIR_SOURCE_LIB/memory.o \
    $DIR_SOURCE_LIB/file.o \
    $DIR_SOURCE_LIB/session.o \
    $DIR_SOURCE_LIB/string.o \
    $DIR_SOURCE_ARCH_LIB/calls.o \
    -o root/boot/kernel

ASM="nasm"
ASMFLAGS="-f elf"
GCC="gcc"
GCCFLAGS="-c -O2 -I../include/lib -Wall -ffreestanding -nostdlib -nostartfiles -nodefaultlibs"
LD="ld"
LDFLAGS="-e main"

$GCC $GCCFLAGS $DIR_SOURCE_BIN/cat.c -o $DIR_SOURCE_BIN/cat.o
$LD $LDFLAGS $DIR_SOURCE_BIN/cat.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/cat

$GCC $GCCFLAGS $DIR_SOURCE_BIN/cd.c -o $DIR_SOURCE_BIN/cd.o
$LD $LDFLAGS $DIR_SOURCE_BIN/cd.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/cd

$GCC $GCCFLAGS $DIR_SOURCE_BIN/clear.c -o $DIR_SOURCE_BIN/clear.o
$LD $LDFLAGS $DIR_SOURCE_BIN/clear.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/clear

$ASM $ASMFLAGS $DIR_SOURCE_BIN/cpu.s -o $DIR_SOURCE_BIN/cpus.o
$GCC $GCCFLAGS $DIR_SOURCE_BIN/cpu.c -o $DIR_SOURCE_BIN/cpu.o
$LD $LDFLAGS $DIR_SOURCE_BIN/cpu.o $DIR_SOURCE_BIN/cpus.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/cpu

$GCC $GCCFLAGS $DIR_SOURCE_BIN/date.c -o $DIR_SOURCE_BIN/date.o
$LD $LDFLAGS $DIR_SOURCE_BIN/date.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/date

$GCC $GCCFLAGS $DIR_SOURCE_BIN/echo.c -o $DIR_SOURCE_BIN/echo.o
$LD $LDFLAGS $DIR_SOURCE_BIN/echo.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/echo

$GCC $GCCFLAGS $DIR_SOURCE_BIN/elf.c -o $DIR_SOURCE_BIN/elf.o
$LD $LDFLAGS $DIR_SOURCE_BIN/elf.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/elf

$GCC $GCCFLAGS $DIR_SOURCE_BIN/hello.c -o $DIR_SOURCE_BIN/hello.o
$LD $LDFLAGS $DIR_SOURCE_BIN/hello.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/hello

$GCC $GCCFLAGS $DIR_SOURCE_BIN/ls.c -o $DIR_SOURCE_BIN/ls.o
$LD $LDFLAGS $DIR_SOURCE_BIN/ls.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/ls

$GCC $GCCFLAGS $DIR_SOURCE_BIN/reboot.c -o $DIR_SOURCE_BIN/reboot.o
$LD $LDFLAGS $DIR_SOURCE_BIN/reboot.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/reboot

$GCC $GCCFLAGS $DIR_SOURCE_BIN/timer.c -o $DIR_SOURCE_BIN/timer.o
$LD $LDFLAGS $DIR_SOURCE_BIN/timer.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/timer

$GCC $GCCFLAGS $DIR_SOURCE_BIN/vga.c -o $DIR_SOURCE_BIN/vga.o
$LD $LDFLAGS $DIR_SOURCE_BIN/vga.o $DIR_SOURCE_LIB/memory.o $DIR_SOURCE_LIB/session.o $DIR_SOURCE_LIB/string.o $DIR_SOURCE_LIB/file.o $DIR_SOURCE_ARCH_LIB/calls.o -o $DIR_IMAGE_BIN/vga

GCC="gcc"
GCCFLAGS="-O2"

$GCC $GCCFLAGS $DIR_SOURCE_TOOLS/mkinitrd.c -o $DIR_SOURCE_TOOLS/mkinitrd

$DIR_SOURCE_TOOLS/mkinitrd $DIR_IMAGE_BIN/cat cat $DIR_IMAGE_BIN/cd cd $DIR_IMAGE_BIN/clear clear $DIR_IMAGE_BIN/cpu cpu $DIR_IMAGE_BIN/date date $DIR_IMAGE_BIN/echo echo $DIR_IMAGE_BIN/elf elf $DIR_IMAGE_BIN/hello hello $DIR_IMAGE_BIN/ls ls $DIR_IMAGE_BIN/reboot reboot $DIR_IMAGE_BIN/timer timer $DIR_IMAGE_BIN/vga vga $DIR_IMAGE_TXT/about.txt about.txt $DIR_IMAGE_TXT/help.txt help.txt

