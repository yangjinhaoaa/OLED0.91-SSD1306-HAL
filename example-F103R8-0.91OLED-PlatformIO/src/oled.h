
#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"


#define OLED_ADDR 0x78

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

void OLED_Write_cmd(uint8_t cmd);
void OLED_Write_data(uint8_t data);
void OLED_ShowPic(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);
void OLED_ShowHanzi(uint8_t x, uint8_t y, uint8_t no);
void OLED_ShowHzbig(uint8_t x, uint8_t y, uint8_t n);
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t accuracy, uint8_t fontsize);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t length, uint8_t fontsize);
void OLED_ShowStr(uint8_t x, uint8_t y, char *ch, uint8_t fontsize);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t fontsize);
void OLED_Allfill(void);
void OLED_Set_Position(uint8_t x, uint8_t y);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);

#endif  /*__OLED_H__*/
