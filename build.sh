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

asm()
{
	nasm $1.asm -f elf32 -o $1.o
	linkfiles="$linkfiles $1.o"
}


echo "Building kernel for platform $PLATFORM"
cd src/kernel/


#NASM commands
asm arch/$PLATFORM/boot
asm arch/$PLATFORM/pic
asm arch/$PLATFORM/idt_helper
asm arch/$PLATFORM/blink


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