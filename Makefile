GCCPARAMS = -std=gnu99 -Iinclude -ffreestanding -Wall -Wextra
ASPARAMS = 
LDPARAMS = -ffreestanding -O2 -nostdlib

BIN_DIR = iso/boot/

src := $(shell find src/ -type f)
# $(info SRC FILES = $(src))

objects := $(addsuffix .o, $(basename $(src:src/%=obj/%)))
# $(info OBJECT FILES = $(objects))

all : run

.PHONY : run
run: oasis.iso
	qemu-system-i386 -full-screen -cdrom $<

.PHONY : install
install: oasis.bin
	sudo cp $(BIN_DIR)$< /boot/oasis.bin

.PHONY : clean
clean:
	rm -rf obj $(BIN_DIR)oasis.bin oasis.iso

obj/%.o: src/%.c
	mkdir -p $(@D)
	i686-elf-gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	i686-elf-as $(ASPARAMS) -o $@ $<

oasis.bin: linker.ld $(objects)
	i686-elf-gcc $(LDPARAMS) -T $< -o $(BIN_DIR)$@ $(objects) -lgcc

oasis.iso: oasis.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o oasis.iso iso

