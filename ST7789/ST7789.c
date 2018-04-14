#include "ST7789.h"
#include "espressif/esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "esp8266.h"
#include <stdio.h>
#include "esp/spi.h"
#include "esp/gpio.h"
#include "font.h"

uint16_t BACK_COLOR=0xffff;
uint16_t FRONT_COLOR=0x0000;
uint8_t DIRE;

/*
* biref : init gpio rs and rst to output
* para  : NONE
* return: NONE 
*/
void LCD_GPIO_Init(void)
{
    int res=1;
    gpio_enable(LCD_RS, GPIO_OUTPUT);       //enable cmd and data pin (output)
    gpio_enable(LCD_RST, GPIO_OUTPUT);      //enable reset pin (output)

    gpio_write(LCD_RS, 1);      //set rs=1  data mode
    gpio_write(LCD_RST,1);      //set reset=1 not reset
    res = spi_init(SPI_BUS, SPI_MODE0, SPI_FREQ_DIV_8M, 1, SPI_BIG_ENDIAN, false); // init SPI module
    if(res)
    {
        printf("SPI Init OK!\r\n");
    }
    else
    {
        printf("SPI Init Error\r\n");
    }
}
/*
* biref : sent command to lcd
* para  : cmd command will be send
* return: NONE 
*/
void writecmd(uint8_t cmd)
{
    uint8_t num = 0;
    gpio_write(LCD_RS, CMD_MODE);  //RS line output low
    num = spi_transfer_8(SPI_BUS,cmd);   //send cmd to lcd
    gpio_write(LCD_RS, DATA_MODE);  //RS line output high
    //LCD_delay(5);                   //delay some time after cmd sent

}
/*
* biref : send 8bit data to lcd
* para  : data sent data
* return: NONE 
*/
void writedata(uint8_t data)
{
    gpio_write(LCD_RS, DATA_MODE);  //set as data transfer mode
    spi_transfer_8(SPI_BUS,data);  //send data
    gpio_write(LCD_RS,DATA_MODE);   //set as data transfermode
    //LCD_delay(1);                   //delay some time after data sent
}
/*
* biref : send 16bit data to lcd
* para  : data sent data
* return: NONE 
*/
void writedata16(uint16_t data)
{
    gpio_write(LCD_RS, DATA_MODE);  //set as data transfer mode
    spi_transfer_16(SPI_BUS,data);  //send data
    gpio_write(LCD_RS,DATA_MODE);   //set as data transfermode

}
/*
* biref : init lcd
* para  : NONE
* return: NONE 
*/
void LCD_Init(void)
{
	/*
	gpio_write(LCD_RST,0);
	LCD_delay(100);
	gpio_write(LCD_RST,1);
	LCD_delay(1000);*/
	LCD_GPIO_Init();
    writecmd(0x11);         //wake up st7789
    LCD_delay(150);         //delay 150ms
    writecmd(0x36);         //page data access control
    writedata(0x00);        
    writecmd(0x3a);         //interface pixel format 
    writedata(0x55);        //65k rgb,16bit/pixel
    writecmd(0xb2);         //porch setting
    writedata(0x0c);        //
    writedata(0x0c);
    writedata(0x00);
    writedata(0x33);
    writedata(0x33);
    writecmd(0xb7);         //gate control
    writedata(0x35);
    writecmd(0xbb);         //vcom setting
    writedata(0x30);        
    writecmd(0xc0);         //LCM control
    writedata(0x2c);
    writecmd(0xc2);         //VDV and VRH command Eanble
    writedata(0x01);
    //writedata(0xff);

    writecmd(0xc3);         //vrh set
    writedata(0x10);        
    writecmd(0xc4);         //VDV set
    writedata(0x20);
    writecmd(0xc6);         //Frame Rate Control in Normal Mode
    writedata(0x0f);
    writecmd(0xd0);         //Power Control
    writedata(0xa4);
    writedata(0xa1);
    writecmd(0xe0);         //positive voltage gamma control
    writedata(0xd0);
    writedata(0x00);
    writedata(0x02);        //
    writedata(0x07);        //
    writedata(0x0a);
    writedata(0x28);
    writedata(0x32);
    writedata(0x44);
    writedata(0x42);
    writedata(0x06);
    writedata(0x0e);
    writedata(0x12);
    writedata(0x14);
    writedata(0x17);
    writecmd(0xe1);         //negative voltage gamma control
    writedata(0xd0);
    writedata(0x00);
    writedata(0x02);        //
    writedata(0x07);        //
    writedata(0x0a);
    writedata(0x28);
    writedata(0x31);
    writedata(0x54);
    writedata(0x47);
    writedata(0x0e);
    writedata(0x1c);
    writedata(0x17);
    writedata(0x1b);
    writedata(0x1e);
    writecmd(0x29);
    writecmd(0x2c);

}

void LCD_SetDir(uint8_t dir)
{
	if(LANDSCAPE == dir)
	{
		writecmd(0x36);         //page data access control
		writedata(0x00);
		DIRE = 0;
	}
	else
	{
		writecmd(0x36);         //page data access control
		writedata(0x60);
		DIRE = 1;
	}
}
void LCD_SetWindow(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye)//set operate window
{
    writecmd(0x2a);         //column address set
    writedata((xs>>8)&0xff);
    writedata(xs&0xff);
    writedata((xe>>8)&0xff);
    writedata(xe&0xff);
    writecmd(0x2b);         //row address set
    writedata((ys>>8)&0xff);
    writedata(ys&0xff);
    writedata((ye>>8)&0xff);
    writedata(ye&0xff);
    writecmd(0x2c);
}
void LCD_ClrScreen(uint16_t color)       //clear screen
{
    int i=0,j=0;
    LCD_SetWindow(0,0,LCD_WIDTH-1,LCD_HIGH-1); //set window
    for(i=0;i<LCD_HIGH;i++)
    {
        for(j=0;j<LCD_WIDTH;j++)
        {
            writedata16(color);
        }
    }
}

void LCD_Fill(uint16_t xState,uint16_t yState,uint16_t xEnd,uint16_t yEnd,uint16_t color)
{          
	uint16_t temp;

    if((xState > xEnd) || (yState > yEnd))
    {
        return;
    }   
	LCD_SetWindow(xState, yState, xEnd, yEnd); 
    xState = xEnd - xState + 1;
	yState = yEnd - yState + 1;

	while(xState--)
	{
	 	temp = yState;
		while (temp--)
	 	{			
			writedata16(color);	
		}
	}	
} 

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetWindow(x, y, x, y);  //设置点的位置
	writedata16(FRONT_COLOR);	
}

void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetWindow(x, y, x, y);  //设置点的位置
	writedata16(color);	
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  

void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawFRONT_COLOR(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
    }  

}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}
void LCD_DrawRectangle_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    LCD_DrawLine_Color(x1,y1,x2,y1,color);
    LCD_DrawLine_Color(x1,y1,x1,y2,color);
    LCD_DrawLine_Color(x1,y2,x2,y2,color);
    LCD_DrawLine_Color(x2,y1,x2,y2,color);
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=ascii_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=ascii_1608[num][t];	//调用1608字体
		else if(size==24)temp=ascii_2412[num][t];	//调用2412字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,FRONT_COLOR);
			else if(mode==0)LCD_DrawFRONT_COLOR(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_HIGH)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_WIDTH)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
} 

uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t wide, uint16_t high,uint8_t *pic)
{
	uint16_t temp = 0;
	long tmp=0,num=0;
	LCD_SetWindow(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		writedata16(temp);//逐点显示
		tmp += 2;
	}
	while(tmp < num);	
}

void disp_qrcode(QRcode *qrcode,uint8_t xs,uint8_t ys, uint8_t p)
{
    int x,y;
    for(y=0;y<qrcode->width;y++)
    {
        for(x=0;x<qrcode->width;x++)  
        {
            if(qrcode->data[y*qrcode->width+x]&0x01)  
            {
                LCD_Fill(xs+x*p,ys+y*p,xs+x*p+p,ys+y*p+p,BLACK);          
            }
        }
    
    }

}
