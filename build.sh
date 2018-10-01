#!/bin/bash

#Config variables
PLATFORM="x86"
CCPLATFORM="i386"
BINLOC="$HOME/i386elfgcc/bin" #location of compiliation tools
CC="$BINLOC/$CCPLATFORM-elf-gcc"
LD="$BINLOC/$CCPLATFORM-elf-ld"
CCFLAGS="-std=gnu89 -pedantic -Wno-long-long -D ARCH=$PLATFORM -Wall -O3 -fstrength-reduce\
 -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -mno-red-zone\
 -mgeneral-regs-only -I./include -I./libk/include -I./arch/$PLATFORM/include -c"

red=`tput setaf 1`
green=`tput setaf 2`
clr=`tput sgr0`

if [ "$1" = "clean" ]
then
	rm Kernel.bin OS.iso iso/boot/Kernel.bin out.bochs &> /dev/null
	echo "${green}Done cleaning!"
	exit 0
fi

function asm
{
	nasm $1.asm -f elf32 -o $1.o
	linkfiles="$linkfiles $1.o"
}

function buildc
{
	for file in $1/*.c
	do
		object=${file/".c"/}".o"
		echo "$file -> $object"
		$CC $file $CCFLAGS -o $object
		linkfiles="$linkfiles $object"
	done
}


echo "${green}Building kernel for platform $PLATFORM${clr}"
cd src/kernel/


#NASM commands
echo "boot.asm -> boot.o"
asm arch/$PLATFORM/boot
echo "pic.asm -> pic.o"
asm arch/$PLATFORM/pic
echo "idt_helper.asm -> idt_helper.o"
asm arch/$PLATFORM/idt_helper
echo "blink.asm -> blink.o"
asm arch/$PLATFORM/blink


#CC commands
echo -e "${green}\nCompiling base-level files${clr}"
buildc .

echo -e "${green}\nCompiling libk${clr}"
buildc libk

echo -e "${green}\nCompiling HAL${clr}"
buildc arch/$PLATFORM

echo -e "${green}\nLinking object files...${clr}"
$BINLOC/$CCPLATFORM-elf-ld -T arch/$PLATFORM/link.ld -o Kernel.bin $linkfiles -z max-page-size=4096 --nmagic

#Clean up
echo -e "${green}\nCleaning up...${clr}"
mv Kernel.bin ../..
rm $linkfiles

#make iso
echo -e "${green}\nMaking iso...${clr}"
cd ../..
cp -p Kernel.bin iso/boot/
grub-mkrescue -o OS.iso iso &> /dev/null

echo -e "${green}\n*** Done! ***${clr}\n"