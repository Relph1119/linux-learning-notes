# 扩展阅读：什么是软中断

## 1 软中断和硬中断对比

宏观上是一致：
- 硬中断包括中断、异常及INT指令，中断机制是纯硬件实现。
- 软中断是纯由软件实现的，打断当前正在允许的程序，转到执行中断处理程序，执行完成之后再返回原始程序。

微观上是类似的：
- 硬中断：CPU在每一个指令周期的最后一个周期检查是否有中断，如果有，将中断号取出，到中断向量表中寻找中断处理程序，然后跳转过去。
- 软中断：一个单独的守护进程，不断轮询一组标志位，如果标志位有值，到这个标志位对应的软中断向量表数组的相应位置，找到中断处理程序，然后跳转过去。

## 2 开启内核软中断处理的守护进程

以Linux2.6.0内核为例，代码路径：`init/main.c`

```c
asmlinkage void __init start_kernel(void)
{
    ...
    trap_init();
	rcu_init();
	init_IRQ();
    ...
    rest_init()
}

static void do_pre_smp_initcalls(void)
{
	extern int spawn_ksoftirqd(void);
#ifdef CONFIG_SMP
	extern int migration_init(void);

	migration_init();
#endif
	node_nr_running_init();
	spawn_ksoftirqd();
}
```

跟踪`spawn_ksoftirqd`方法，可以找到`kernel/softirq.c`文件下的具体方法：

```c
// 软中断处理函数表（软中断向量表）
static struct softirq_action softirq_vec[32] __cacheline_aligned_in_smp;

asmlinkage void do_softirq(void)
{
    // 获取所有的软中断标志位
	pending = local_softirq_pending();

	if (pending) {
		struct softirq_action *h;
        // 软中断向量表起始地址指针
		h = softirq_vec;

		do {
            // 如果软中断标志位有值
			if (pending & 1)
                // 在对应的软中断向量表执行对应的处理函数
				h->action(h);
            // 遍历下一个标志位
			h++;
			pending >>= 1;
		} while (pending);
	}
}
```

## 3 注册软中断向量表

以Linux2.6.0内核的网络子系统的初始化为例，需要注册网络的软中断处理函数，代码路径：`net/core/dev.c`

```c
static int __init net_dev_init(void)
{   
    ...
    // 网络发包的处理函数 
    open_softirq(NET_TX_SOFTIRQ, net_tx_action, NULL);
    // 网络收包的处理函数
	open_softirq(NET_RX_SOFTIRQ, net_rx_action, NULL);
	...
}
```

代码路径：`kernel/softirq.c`

```c
void open_softirq(int nr, void (*action)(struct softirq_action*), void *data)
{
	softirq_vec[nr].data = data;
    // 注册软中断向量表
	softirq_vec[nr].action = action;
}
```

## 4 触发一次软中断

将`local_softirq_pending()`对应的标志位改成1，就可以触发软中断。


代码路径：`include/linux/netdevice.h`

```c
// 将对应的标志位修改成1
#define __raise_softirq_irqoff(nr) do { local_softirq_pending() |= 1UL << (nr); } while (0)

static inline void __netif_rx_schedule(struct net_device *dev)
{
	unsigned long flags;

	local_irq_save(flags);
	dev_hold(dev);
	list_add_tail(&dev->poll_list, &__get_cpu_var(softnet_data).poll_list);
	if (dev->quota < 0)
		dev->quota += dev->weight;
	else
		dev->quota = dev->weight;
    // 触发软中断
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
	local_irq_restore(flags);
}
```

## 5 总结

- 软中断就是一组软中断标志位，对应软中断向量表中的中断处理函数，然后有一个内核守护进程不断去循环判断调用。然后各个子系统调用`open_softirq`为软中断向量表赋值。如果需要触发软中断，则调用`raise_softirq_irqoff`修改中断标志位的值。
- 软中断是Linux处理一个中断的下半部分的主要方式：上半部分处理简单的逻辑，下半部分交给软中断异步处理。