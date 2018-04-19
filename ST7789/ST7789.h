#ifndef __ST7789_H
#define __ST7789_H
/*
 * Note the setting of the direction and screen height and width,
 * otherwise an error will occur
 * The default direction is portrait,width:240,height:320
 *
 * */

#include "FreeRTOS.h"
#include "task.h"
#include "qrencode.h"

#define LCD_RS  4   //cmd or data
#define LCD_RST 5   //reset lcd

#define LCD_WIDTH   240     //screen width
#define LCD_HIGH    320     //screen hight

#define SPI_BUS 1   //spi bus use user.(0:system,1:user)

#define LANDSCAPE	0		//hengping
#define PORTRAIT	1		//shuping


#define CMD_MODE 0  //command
#define DATA_MODE 1 //data

#define WHITE 	0xffff
#define BLACK 	0x0000
#define RED		0xf800
#define GREEN	0x07E0
#define BLUE	0x001f

extern uint16_t BACK_COLOR;
extern uint16_t FRONT_COLOR;

#define LCD_delay(x) vTaskDelay((x)/portTICK_PERIOD_MS)

void writecmd(uint8_t cmd);    //write cmd to lcd
void writedata(uint8_t data);  //write data to lcd
void writedata16(uint16_t data);//write 16 bit data to lcd
void LCD_GPIO_Init(void);           //init GPIO
void LCD_Init(void);            //init lcd
void LCD_SetDir(uint8_t dir);
void LCD_SetWindow(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye);    //set display windows
void LCD_ClrScreen(uint16_t color);       //clear screen
void LCD_Fill(uint16_t xState,uint16_t yState,uint16_t xEnd,uint16_t yEnd,uint16_t color);  //fill windows with color
void LCD_DrawPoint(uint16_t x,uint16_t y);  
void LCD_DrawFRONT_COLOR(uint16_t x,uint16_t y,uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawLine_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle_Color(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);
void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t wide, uint16_t high,uint8_t *pic);
void disp_qrcode(QRcode *qrcode,uint8_t xs,uint8_t ys, uint8_t p);
#endif
