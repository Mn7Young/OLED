/*
 * OLED.C
 *
 *  Created on: 2016年5月2日
 *      Author: Mn
 */


//#include "reg52.h"
//#include "intrins.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "codetab.h"
#include "LQ12864(tiva).h"

void main(void)//23:54//23:58//0:02//NEW0.13
{
	unsigned char i;

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                   SYSCTL_XTAL_16MHZ);
	//GPIOINIT();

	OLED_Init(); //OLED初始化
	while(1)
	{
//		OLED_Fill(0xff); //屏全亮
//		delay(2000);
//		OLED_Fill(0x00); //屏全灭
//		delay(200);//*****caonima
		//



		OLED_P16x16Ch(40,0,2);
		OLED_P16x16Ch(57,0,3);
		OLED_P16x16Ch(74,0,4);
		OLED_P16x16Ch(91,0,5);

		for(i=0; i<8; i++)//通过点整显示汉字 -- i表示字表数组的位置
		{
//			OLED_P16x16Ch(i*16,0,i);
		 	OLED_P16x16Ch(i*16,2,i+8);
		 	OLED_P16x16Ch(i*16,4,i+16);
		 	OLED_P16x16Ch(i*16,6,i+24);
		}
		delay(10000);
		OLED_CLS();//清屏

		OLED_P8x16Str(0,0,"HelTec");//第一行 -- 8x16的显示单元显示ASCII码
		OLED_P8x16Str(0,2,"OLED Display");
		OLED_P8x16Str(0,4,"www.heltec.cn");
		OLED_P6x8Str(0,6,"cn.heltec@gmail.com");
		OLED_P6x8Str(0,7,"heltec.taobao.com");
		delay(14000);
		OLED_CLS();

		Draw_BMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay(8000);
		Draw_BMP(0,0,128,8,BMP2);
		delay(8000);
	}
}




