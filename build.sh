echo "Building kernel"
cd src/kernel/arch/x86
nasm boot.asm -f elf32 -o boot.o

echo "Linking object files"
~/i386elfgcc/bin/i386-elf-ld -T link.ld boot.o -o kernel.bin
mv Kernel.bin ../../../..
rm boot.o

echo "Done!"