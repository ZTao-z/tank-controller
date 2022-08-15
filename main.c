// 51单片机
#include "reg52.h"

typedef unsigned int uint;
typedef unsigned char uchar;
sbit led1 = P2 ^ 0; //根据各位实际的单片机上的小灯硬件连接，随便选择一个即可
sbit led2 = P2 ^ 1; //根据各位实际的单片机上的小灯硬件连接，随便选择一个即可
uint pass = 0;			//传递数据位

sbit ain1 = P1 ^ 0;
sbit ain2 = P1 ^ 1;

void UART_init() //设置串行通信 本晶振为11.0592MHZ，其他的就自己算一下应该设置多少
{
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd; //波特率9600
	SM0 = 0;
	SM1 = 1; // 串口工作方式1 10位异步
	REN = 1; //串口允许接收
	TR1 = 1;
	EA = 1;
	ES = 1; //串口中断
}

int main()
{
	UART_init(); //波特率9600
	while (1)
	{
		if (pass == 0x31) {
			led1 = 0; //本单片机P2^1为小灯
			led2 = 1;
			ain1 = 1;
			ain2 = 0;
		} else if (pass == 0x32) {
			led1 = 1;
			led2 = 0;
			ain1 = 0;
			ain2 = 1;
		} else {
			led1 = 1;
			led2 = 1;
			ain1 = 0;
			ain2 = 0;
		}
	}
}

void UART_INTERRUPT() interrupt 4
{
	pass = SBUF;
	RI = 0;
	SBUF = pass; //手机和蓝牙模块通讯时，我们可以打开电脑串口调试工具查看我们的发送值）
	while (!TI);
	TI = 0;
}
