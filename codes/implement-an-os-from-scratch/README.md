# 代码介绍

本代码来自王柏生老师的[《穿越操作系统迷雾》](https://book.douban.com/subject/36560814/)一书中的项目代码。

## 项目结构

<pre>
implement-an-os-from-scratch------------代码
+---c3--------------------------------------第3章机器语言程序设计
+---c4--------------------------------------第4章汇编语言程序设计
+---c5--------------------------------------第5章C语言程序设计
+---c6--------------------------------------第6章32位引导过程
+---c7--------------------------------------第7章64位引导过程
+---c8--------------------------------------第8章内存管理
+---c9--------------------------------------第9章进程
+---c10-------------------------------------第10章中断和异常
+---c11-------------------------------------第11章进程调度
+---c12-------------------------------------第12章系统调用
    +---c12-1-------------------------------------第12.5节之前
    +---c12-2-------------------------------------第12.5节之后
+---c13-------------------------------------第13章进程间通信
+---c14-------------------------------------第14章显示及输入
</pre>

## 项目环境

1. 准备“物理”计算机

```shell
cd ~
mkdir -p LearniongMore/codes
git clone https://git.kernel.org/pub/scm/linux/kernel/git/maz/kvmtool.git
```

2. 安装`SDL`库
```shell
sudo apt-get install libsdl1.2-dev
```

3. 修改串口打印格式

代码路径：`kvmtool/hw/serial.c`
```c
static void serial8250_flush_tx(struct kvm *kvm, struct serial8250_device *dev)
{
	dev->lsr |= UART_LSR_TEMT | UART_LSR_THRE;

	if (dev->txcnt) {
		//term_putc(dev->txbuf, dev->txcnt, dev->id);
		dev->txcnt = 0;
	}
}
```

```c
static bool serial8250_out(struct ioport *ioport, struct kvm_cpu *vcpu, u16 port,
			   void *data, int size)
{
	...

	switch (offset) {
	case UART_TX:
        fprintf(stderr, "output: %lx\n", *((unsigned long *) data));
		fprintf(stderr, "%s\n", ((char *) data));

		if (dev->lcr & UART_LCR_DLAB) {
			dev->dll = ioport__read8(data);
			break;
		}
    
    ...
```

4. 编译`kvmtool`

```shell
cd ~/kvmtool
make
```

## 本书勘误

勘误地址：https://book.douban.com/subject/36560814/discussion/637560822/