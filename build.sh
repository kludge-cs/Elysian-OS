#!/bin/bash

#Config variables
PLATFORM="x86"
CCPLATFORM="i386"
BINLOC="$HOME/i386elfgcc/bin" #location of compiliation tools


if [ "$1" = "clean" ]
then
	rm Kernel.bin OS.iso iso/boot/Kernel.bin out.bochs
	exit
fi


echo "Building kernel for platform $PLATFORM"
cd src/kernel/


#NASM commands
nasm arch/$PLATFORM/boot.asm -f elf32 -o arch/$PLATFORM/boot.o
linkfiles="arch/x86/boot.o"

nasm arch/$PLATFORM/pic.asm -f elf32 -o arch/$PLATFORM/pic.o
linkfiles="$linkfiles arch/x86/pic.o"

nasm arch/$PLATFORM/idt_handler.asm -f elf32 -o arch/$PLATFORM/idt_handler.o
linkfiles="$linkfiles arch/x86/idt_handler.o"


#GCC commands
echo "Compiling kernel"
$BINLOC/$CCPLATFORM-elf-gcc start.c -Wall -O \
	-fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin \
	-I./libk/include -I./arch/$PLATFORM/include -c -o start.o

linkfiles="$linkfiles start.o"


echo "Compiling libk"
for file in libk/string/*.c
do
	object=${file/".c"/}".o"
	$BINLOC/$CCPLATFORM-elf-gcc $file -Wall -O \
		-fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin \
		-I./libk/include -I./arch/$PLATFORM/include -c -o $object
	linkfiles="$linkfiles $object"
done

for file in arch/$PLATFORM/*.c
do
	object=${file/".c"/}".o"
	$BINLOC/$CCPLATFORM-elf-gcc $file -Wall -O \
			-fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -mgeneral-regs-only \
			-I./libk/include -I./arch/$PLATFORM/include -c -o $object
	linkfiles="$linkfiles $object"
done

echo "Linking object files"
$BINLOC/$CCPLATFORM-elf-ld -T arch/$PLATFORM/link.ld -o Kernel.bin $linkfiles

#Clean up
mv Kernel.bin ../..
rm $linkfiles

#make iso
cd ../..
cp -p Kernel.bin iso/boot/
grub-mkrescue -o OS.iso iso &> /dev/null

echo "Done!"