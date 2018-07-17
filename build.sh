PLATFORM="x86"


echo "Building kernel for platform $PLATFORM"
cd src/kernel/

nasm arch/x86/boot.asm -f elf32 -o arch/x86/boot.o

#gcc commands
~/i386elfgcc/bin/i386-elf-gcc start.c -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./libk/include -I./arch/$PLATFORM/include -c -o start.o

echo "Linking object files"
~/i386elfgcc/bin/i386-elf-ld -T arch/x86/link.ld -o kernel.bin arch/x86/boot.o start.o
mv Kernel.bin ../..
rm arch/x86/boot.o
rm start.o

echo "Done!"