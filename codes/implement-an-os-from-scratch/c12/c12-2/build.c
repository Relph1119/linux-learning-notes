#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {   
    int fd, fd_kernel;
    int c;
    char buf[512];

    // 创建kernel.bin文件并以读写方式打开
    fd_kernel = open("kernel.bin", O_WRONLY | O_CREAT, 0664);

    // 将boot16.bin文件的数据写入kernel.bin文件中
    fd = open("boot16.bin", O_RDONLY);
    while (1) {
        c = read(fd, buf, 512);
        if (c > 0) {
            write(fd_kernel, buf, c);
        } else {
            break;
        }
    };
    close(fd);

    // 将内核保护模式部分加载到内存0x20000处
    lseek(fd_kernel, 0x20000 - 0x10000, SEEK_SET);

    // 将boot32.bin文件的数据写入kernel.bin文件中
    fd = open("boot32.bin", O_RDONLY);
    while (1) {
        c = read(fd, buf, 512);
        if (c > 0) {
            write(fd_kernel, buf, c);
        } else {
            break;
        }
    };
    close(fd);

    // 将内核64位部分加载到内存0x100000处
    lseek(fd_kernel, 0x100000 - 0x10000, SEEK_SET);

    // 将system.bin文件的数据写入kernel.bin文件中
    fd = open("system.bin", O_RDONLY);
    while (1) {
        c = read(fd, buf, 512);
        if (c > 0) {
            write(fd_kernel, buf, c);
        } else {
            break;
        }
    };
    close(fd);

    // 将app1程序映像加载到内存0xc800000处
    lseek(fd_kernel, 0xc800000 - 0x10000, SEEK_SET);
    fd = open("app/app1.bin", O_RDONLY);
    while (1) {
        c = read(fd, buf, 512);
        if (c > 0) {
            write(fd_kernel, buf, c);
        } else {
            break;
        }
    };
    close(fd);

    // 将app2程序映像加载到内存0xd000000处
    lseek(fd_kernel, 0xd000000 - 0x10000, SEEK_SET);
    fd = open("app/app2.bin", O_RDONLY);
    while (1) {
        c = read(fd, buf, 512);
        if (c > 0) {
        write(fd_kernel, buf, c);
        } else {
        break;
        }
    };
    close(fd);

    close(fd_kernel);

    return 0;
}
