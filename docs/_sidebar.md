* [目录](README.md)
    * [《Linux源码趣读》](linux-source-code-reading/content.md)
        * 第一部分 进入内核前的苦力活
            * [第1回 Linux开机初始化](linux-source-code-reading/part01/ch01.md)
            * [第2回 从0x7c00到0x90000](linux-source-code-reading/part01/ch02.md)
            * [第3回 做好访问内存的基础准备工作](linux-source-code-reading/part01/ch03.md)
            * [第4回 把全部的操作系统代码从硬盘搬到内存](linux-source-code-reading/part01/ch04.md)
            * [扩展阅读：什么是中断](linux-source-code-reading/part01/references-interrupt.md)
            * [扩展阅读：什么是软中断](linux-source-code-reading/part01/references-software-interrupt.md)
            * [第5回 将重要的操作系统代码放在零地址处](linux-source-code-reading/part01/ch05.md)
            * [第6回 解决段寄存器的历史包袱问题](linux-source-code-reading/part01/ch06.md)
            * [第7回 六行代码进入保护模式](linux-source-code-reading/part01/ch07.md)
            * [第8回 重新设置中断描述符表与全局描述符表](linux-source-code-reading/part01/ch08.md)
            * [第9回 开启分页机制](linux-source-code-reading/part01/ch09.md)
            * [第10回 进入main函数前的最后一跃](linux-source-code-reading/part01/ch10.md)
        * 第二部分 “大战”前期的初始化工作
            * [第11回 整个操作系统就二十几行](linux-source-code-reading/part02/ch11.md)
            * [第12回 管理内存前先划分出三个边界值](linux-source-code-reading/part02/ch12.md)
            * [第13回 主内存初始化mem_init](linux-source-code-reading/part02/ch13.md)
            * [第14回 中断初始化trap_init](linux-source-code-reading/part02/ch14.md)
            * [第15回 块设备请求项初始化blk_dev_init](linux-source-code-reading/part02/ch15.md)
            * [第16回 控制台初始化tty_init](linux-source-code-reading/part02/ch16.md)
            * [第17回 时间初始化time_init](linux-source-code-reading/part02/ch17.md)
            * [第18回 进程调度初始化sched_init](linux-source-code-reading/part02/ch18.md)
            * [第19回 缓冲区初始化buffer_init](linux-source-code-reading/part02/ch19.md)
            * [第20回 硬盘初始化hd_init](linux-source-code-reading/part02/ch20.md)
        * 第三部分 一个新进程的诞生
            * [第21回 第3部分全局概览](linux-source-code-reading/part03/ch21.md)
            * [第22回 从内核态切换到用户态](linux-source-code-reading/part03/ch22.md)
            * [第23回 如果让你来设计进程调度](linux-source-code-reading/part03/ch23.md)
            * [第24回 从一次定时器滴答来看进程调度](linux-source-code-reading/part03/ch24.md)
            * [第25回 通过fork看一次系统调用](linux-source-code-reading/part03/ch25.md)
            * [第26回 fork函数中进程基本信息的复制](linux-source-code-reading/part03/ch26.md)
            * [第27回 透过fork来看进程的内存规划](linux-source-code-reading/part03/ch27.md)
            * [第29回 番外篇：写时复制——就这么几行代码](linux-source-code-reading/part03/ch29.md)
            * [第30回 番外篇：你管这破玩意叫文件系统](linux-source-code-reading/part03/ch30.md)
        * 第四部分 shell程序的由来
            * [第31回 拿到硬盘信息](linux-source-code-reading/part04/ch31.md)     
            * [第32回 加载根文件系统](linux-source-code-reading/part04/ch32.md)     
            * [第33回 打开终端设备文件](linux-source-code-reading/part04/ch33.md)     
            * [第34回 进程2的创建](linux-source-code-reading/part04/ch34.md)     
            * [第35回 execve加载并执行shell程序](linux-source-code-reading/part04/ch35.md)    
            * [第36回 缺页中断](linux-source-code-reading/part04/ch36.md)
        * 第五部分 一条shell命令的执行
            * [第42回 用键盘输入一条命令](linux-source-code-reading/part05/ch42.md)     
            * [第43回 shell程序读取你的命令](linux-source-code-reading/part05/ch43.md)     
            * [第44回 进程的阻塞与唤醒](linux-source-code-reading/part05/ch44.md)     
            * [第45回 解析并执行shell命令](linux-source-code-reading/part05/ch45.md)     
            * [第46回 读硬盘数据全流程](linux-source-code-reading/part05/ch46.md)     
            * [第47回 读取硬盘数据的细节](linux-source-code-reading/part05/ch47.md)
            * [第48回 信号](linux-source-code-reading/part05/ch48.md)
    * [《穿越操作系统迷雾》](implement-an-os-from-scratch/content.md)
        * [第6章 32位引导过程](implement-an-os-from-scratch/ch06.md)
        * [第7章 64位引导过程](implement-an-os-from-scratch/ch07.md)
        * [第8章 内存管理](implement-an-os-from-scratch/ch08.md)
        * [第9章 进程](implement-an-os-from-scratch/ch09.md)
        * [第10章 中断和异常](implement-an-os-from-scratch/ch10.md)
        * [第11章 进程调度](implement-an-os-from-scratch/ch11.md)
        * [第12章 系统调用](implement-an-os-from-scratch/ch12.md)
        * [第13章 进程间通信](implement-an-os-from-scratch/ch13.md)
        * [第14章 显示及输入](implement-an-os-from-scratch/ch14.md)