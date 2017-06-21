/************************************************************************************
*  Copyright (c), 2013, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: LQ12864.h
* Project  : OLED
* Processor: STC89C52
* Compiler : Keil C51 Compiler
* 
* Author : С��
* Version: 1.00
* Date   : 2013.8.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64����OLEDģ�������ļ���������heltec.taobao.com���۲�Ʒ
*
* Others: none;
*
* Function List:
*
* 1. void delay(unsigned int z) -- ��ʱ����,����
* 2. void IIC_Start() -- ����I2C����
* 3. void IIC_Stop() -- �ر�I2C����
* 4. void Write_IIC_Byte(unsigned char IIC_Byte) -- ͨ��I2C����дһ��byte������
* 5. void OLED_WrDat(unsigned char dat) -- ��OLED��д����
* 6. void OLED_WrCmd(unsigned char cmd) -- ��OLED��д����
* 7. void OLED_Set_Pos(unsigned char x, unsigned char y) -- ������ʾ����
* 8. void OLED_Fill(unsigned char bmp_dat) -- ȫ����ʾ(��ʾBMPͼƬʱ�Ż��õ��˹���)
* 9. void OLED_CLS(void) -- ��λ/����
* 10. void OLED_Init(void) -- OLED����ʼ�����򣬴˺���Ӧ�ڲ�����Ļ֮ǰ���ȵ���
* 11. void OLED_P6x8Str(unsigned char x, y,unsigned char ch[]) -- 6x8������������ʾASCII�����С���У���̫����
* 12. void OLED_P8x16Str(unsigned char x, y,unsigned char ch[]) -- 8x16������������ʾASCII�룬�ǳ�����
* 13.void OLED_P16x16Ch(unsigned char x, y, N) -- 16x16������������ʾ���ֵ���С���У������ø������塢�Ӵ֡���б���»��ߵ�
* 14.void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[]) -- ��128x64���ص�BMPλͼ��ȡ�����������ֱ�Ȼ���Ƶ�codetab�У��˺������ü���
*
* History: none;
*
*************************************************************************************/

//#include "reg52.h"


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "buttons.h"
//#include "utils/uartstdio.h"

#define SLAVE_ADDRESS (0x78<<1)

// ------------------------------------------------------------
// IO��ģ��I2Cͨ��
// SCL��P1^3
// SDA��P1^2
// ------------------------------------------------------------
//sbit SCL=P1^3; //����ʱ��
//sbit SDA=P1^2; //��������

//#define SCL=low;
//#define SCL=high;
//#define SDA=high;
//#define SDA=low;


//#define high 1
//#define low 0

#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/*********************OLED���������õ���ʱ����************************************/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=550;y>0;y--);
}

/**********************************************
 *
 * //GPIO INITIATE
**********************************************/
void GPIOINIT()
{
    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7);
	ButtonsInit();

    /**********************************************************
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    // This step is not necessary if your part does not support pin muxing.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
    //I2CMasterSlaveAddrSet(I2C0_BASE, SLAVE_ADDRESS, false);
    ************************************************************/

}
void I2C_INIT()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_6|GPIO_PIN_7);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PA6_I2C1SCL);
	GPIOPinConfigure(GPIO_PA7_I2C1SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
	I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
	I2CSlaveEnable(I2C1_BASE);
	//I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS, false);
}

/*
//IIC Start
**********************************************/
void IIC_Start()
{
/*
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_6);//SDA=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);			//SDA=low;
*/
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;SCL=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7);//SDA=high;SDA=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);			//SDA=low;SDA=low;
    GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;SCL=low;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
/*
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_6);//SDA=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);			//SDA=low;
*/
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);			//SDA=low;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7);//SDA=high;

}

/**********************************************
// ͨ��I2C����дһ���ֽ�
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(IIC_Byte & 0x80)
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7);//SDA=high;SDA=high;
		else
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,0);			//SDA=low;SDA=low;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;SCL=high;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;SCL=low;
		IIC_Byte<<=1;
	}
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_7,GPIO_PIN_7);//SDA=high;SDA=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,GPIO_PIN_6);//SCL=high;SCL=high;
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_6,0);			//SCL=low;SCL=low;


}

/*********************OLEDд����************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	/****************************************************
	IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			//write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop();
	*/////////////////////////////////////////////////////

	/*******************************************************/
	//I2CMasterDataPut(I2C0_BASE, 0x78);
	//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	//I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS, false);
	I2CMasterDataPut(I2C1_BASE, 0x78<<1);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	while(I2CMasterBusy(I2C1_BASE));
	I2CMasterDataPut(I2C1_BASE, 0x40);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy(I2C1_BASE));
    I2CMasterDataPut(I2C1_BASE, IIC_Data);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    /**********************************************************/
}
/*********************OLEDд����************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	/*****************************************************
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			//write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop();
	*/////////////////////////////////////////////////////

	/*******************************************************/
	//I2CMasterDataPut(I2C0_BASE, 0x78);
	//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	I2CMasterSlaveAddrSet(I2C1_BASE, SLAVE_ADDRESS, false);
	//I2CMasterDataPut(I2C1_BASE, 0x78);
	//I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
	//while(I2CMasterBusy(I2C1_BASE));
	I2CMasterDataPut(I2C1_BASE, 0x00);
	I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
	while(I2CMasterBusy(I2C1_BASE));
    I2CMasterDataPut(I2C1_BASE, IIC_Command);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    //*////////////////////////////////////////////////////////

}

/*********************OLED ��������************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
} 
/*********************OLEDȫ��************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}
/*********************OLED��λ************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}
/*********************OLED��ʼ��************************************/
void OLED_Init(void)
{
	I2C_INIT();
	delay(500);//��ʼ��֮ǰ����ʱ����Ҫ��
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //��ʼ����
	OLED_Set_Pos(0,0);
} 
/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void OLED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned char N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}
