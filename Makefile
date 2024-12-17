GCCPARAMS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASPARAMS = 
LDPARAMS = -ffreestanding -O2 -nostdlib

objects = boot.o gdt.o kernel.o

BIN_DIR = iso/boot/

.PHONY : run
run: oasis.iso
	qemu-system-i386 -full-screen -cdrom $<

.PHONY : install
install: oasis.bin
	sudo cp $(BIN_DIR)$< /boot/oasis.bin

.PHONY : clean
clean:
	rm $(objects)
	rm $(BIN_DIR)oasis.bin
	rm oasis.iso

%.o: %.c
	i686-elf-gcc $(GCCPARAMS) -o $@ -c $<

%.o: %.s
	i686-elf-as $(ASPARAMS) -o $@ $<

oasis.bin: linker.ld $(objects)
	i686-elf-gcc $(LDPARAMS) -T $< -o $(BIN_DIR)$@ $(objects) -lgcc

oasis.iso: oasis.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o oasis.iso iso

