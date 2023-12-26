kernel.bin: build boot16.bin boot32.bin
	./build

boot16.bin: boot16.S
	gcc -c boot16.S -o boot16.o
	# 实模式运行时的地址是由“段基址<<4+段内偏移”生成,kvmtool将段寄存器初始化为0x1000,段内偏移为0
	ld -Ttext=0x0 boot16.o -o boot16.elf
	objcopy -O binary boot16.elf boot16.bin

boot32.bin: boot32.S
	gcc -c boot32.S -o boot32.o
	# 保护模式的代码段基址为0,段内偏移为0x20000
	ld -Ttext=0x20000 boot32.o -o boot32.elf
	objcopy -O binary boot32.elf boot32.bin

build: build.c
	gcc $< -o $@

.PHONY: clean run

run: kernel.bin
	~/kvmtool/lkvm run -c 1 -k ./kernel.bin

clean:
	rm -f *.bin *.elf *.o build
