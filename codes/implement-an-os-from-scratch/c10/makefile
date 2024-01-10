kernel.bin: build boot16.bin boot32.bin system.bin app/app1.bin
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

# 使用CFLAGS给编译器传参
# -I：从哪些目录搜索头文件
# -fon-pic：位置无关代码可以加载在程序地址空间中的任何位置
# -mcmodel=kernel：指示gcc生成使用64位寻址操作数的汇编代码，基于large模型改进，减少指令长度
# -fno-stack-protector：关闭栈溢出检查
# -fcf-protection=none：关闭gcc检查代码的特性
# -nostdinc：不要搜索宿主系统的系统目录下的头文件
# -fno-builtin：不需要使用内置的memset
CFLAGS = -std=c11 -I. -fno-pic -mcmodel=kernel -fno-stack-protector -fcf-protection=none -nostdinc -fno-builtin

SRCS = main.c $(wildcard mm/*.c) $(wildcard lib/*.c) $(wildcard kernel/*.c)
# 将SRCS中的每一个.c替换成.o
OBJS = $(SRCS:.c=.o)

system.bin: head64.o kernel/handler.o $(OBJS)
# 内核映像的虚拟地址起始于0xffffffff8000000,映射物理内存地址0处,64位部分位于物理内存0x100000处
	ld -Ttext=0xffffffff80100000 head64.o kernel/handler.o $(OBJS) -o system.elf
	objcopy -O binary system.elf $@

# 将依赖关系保存到.depend文件中
.depend: $(SRCS)
	@rm -f .depend
	@$(foreach src,$(SRCS), \
		echo -n $(dir $(src)) >> .depend; \
		gcc -I. -MM $(src) >> .depend; \
	)
include .depend

app/app1.bin: app/app1.S
	gcc -c app/app1.S -o app/app1.o
	ld -Ttext=0x100000 app/app1.o -o app/app1.elf
	objcopy -O binary app/app1.elf app/app1.bin

build: build.c
	gcc $< -o $@

.PHONY: clean run

run: kernel.bin
	~/kvmtool/lkvm run -c 1 -k ./kernel.bin

clean:
	find -name "*.o" -o -name "*.elf" -o -name "*.bin" | xargs rm -f
	rm -f build .depend messages.log
