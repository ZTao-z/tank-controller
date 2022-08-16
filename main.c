// 51单片机
#include "reg52.h"

typedef unsigned int uint;
typedef unsigned char uchar;
sbit led1 = P2 ^ 0; //根据各位实际的单片机上的小灯硬件连接，随便选择一个即可
sbit led2 = P2 ^ 1; //根据各位实际的单片机上的小灯硬件连接，随便选择一个即可
uint signal = 0;    //传递数据位
int time;           //定义占空比的变量

sbit ain1 = P1 ^ 0;
sbit ain2 = P1 ^ 1;

sbit PWM = P2 ^ 0; // P2.0输出pwm

void UART_init() // 设置串行通信 本晶振为11.0592MHZ，其他的就自己算一下应该设置多少
{
  TMOD = 0x20;
  TH1 = 0xfd;
  TL1 = 0xfd; // 波特率9600
  SM0 = 0;
  SM1 = 1; // 串口工作方式1 10位异步
  REN = 1; // 串口允许接收
  TR1 = 1;
  EA = 1;
  ES = 1; //串口中断
}

void Timer0_init()
{
  TMOD = TMOD | 0x01;
  TH0 = 0xff;  // 高位赋值 (65536 - 10) / 256 赋初值定时
  TL0 = 0xf7;  // 低位赋值 (65536 - 10) % 256 = 0.01ms
  ET0 = 1;     // 定时器中断允许位
  EA = EA | 1; // 总中断开关
  TR0 = 1;     // 开始计数控制位
}

void delay1ms(uint ms)
{
  uint i, j;
  for (i = ms; i > 0; i--)
    for (j = 110; j > 0; j--)
      ;
}

int main()
{
  UART_init(); //波特率9600
  Timer0_init();
  while (1)
  {
    if (signal == 0x31)
    {
      led1 = 0; //本单片机P2^1为小灯
      led2 = 1;
      ain1 = 1;
      ain2 = 0;
    }
    else if (signal == 0x32)
    {
      led1 = 1;
      led2 = 0;
      ain1 = 0;
      ain2 = 1;
    }
    else
    {
      led1 = 1;
      led2 = 1;
      ain1 = 0;
      ain2 = 0;
    }
  }
}

// interrupt 4 代表串行口中断
void UART_INTERRUPT() interrupt 4
{
  signal = SBUF;
  RI = 0;
  SBUF = signal; //手机和蓝牙模块通讯时，我们可以打开电脑串口调试工具查看我们的发送值）
  while (!TI)
    ;
  TI = 0;
}

// interrupt 1 代表时钟中断
void TIMER_INTERRUPT() interrupt 1
{
  TR0 = 0;    //赋初值时，关闭定时器
  TH0 = 0xff; //(65536-10)/256;//赋初值定时
  TL0 = 0xf7; //(65536-10)%256;//0.01ms
  TR0 = 1;    //打开定时器

  time++;

  if (time >= 100)
    time = 0;

  if (time <= 50) //占空比%50,可改
    PWM = 1;
  else
    PWM = 0;
}
