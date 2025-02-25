/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: miscbeepApp.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: MISC驱动框架下的beep测试APP。
其他	   	: 无
使用方法	 ：./miscbeepApp  /dev/miscbeep  0 关闭蜂鸣器
		     ./misdcbeepApp /dev/miscbeep  1 打开蜂鸣器
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/8/20 正点原子Linux团队创建
***************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#define CLOSE_CMD 		(_IO(0XEF, 0x1))	/* 关闭beep */
#define OPEN_CMD		(_IO(0XEF, 0x2))	/* 打开beep */
#define GET_GPIO_CMD    _IOWR(0XEF, 0x3, unsigned int)


#define BEEPOFF	0
#define BEEPON 	1

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd, retvalue;
	char *filename;
	unsigned char databuf[1];
	unsigned int cmd;
	unsigned int gpio_value;
	
	if(argc != 3){
		printf("Error Usage!\r\n");
		return -1;
	}

	filename = argv[1];
	fd = open(filename, O_RDWR);	/* 打开beep驱动 */
	if(fd < 0){
		printf("file %s open failed!\r\n", argv[1]);
		return -1;
	}

	// databuf[0] = atoi(argv[2]);	/* 要执行的操作：打开或关闭 */
	// retvalue = write(fd, databuf, sizeof(databuf));
	// if(retvalue < 0){
	// 	printf("BEEP Control Failed!\r\n");
	// 	close(fd);
	// 	return -1;
	// }

	cmd = atoi(argv[2]);
    if(4 == cmd)				/* 退出APP	 */
        goto out;
    else if(cmd == 1)
    {
        cmd = CLOSE_CMD;
        ioctl(fd, cmd);
    }
    else if(cmd == 2)
    {
        cmd = OPEN_CMD;
        ioctl(fd, cmd);
    }
	else if(cmd == 3)
    {
        cmd = GET_GPIO_CMD;
        ioctl(fd, cmd, &gpio_value);
        printf("GPIO is %d\n", gpio_value);
    }

    close(fd); 
    return 0;
out:
	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file %s close failed!\r\n", argv[1]);
		return -1;
	}
	return 0;
}