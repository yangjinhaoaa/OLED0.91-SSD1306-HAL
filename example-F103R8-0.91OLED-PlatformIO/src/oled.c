/*
this library is a 0.91'OLED(ssd1306) driver
*/

//Header file reference
//The oledfont.h, oled.h and STM32's i2c.h files need to be referenced in the oled.c file
#include "oled.h"
#include "oledfont.h"
#include "i2c.h"

/**
 * 0.91 "OLED initialization control word
 * Each control word can change the display properties of the screen according to the manufacturer's Datasheet
 * For example, in the fifth line from the bottom, the control word 0x81,0x80.you can changes the contrast by changing 0x80
 * 
*/
uint8_t initcmd1[] = {
	0xAE,		//display off
	0xD5, 0x80, //Set Display Clock Divide Ratio/Oscillator Frequency
	0xA8, 0x1F, //set multiplex Ratio
	0xD3, 0x00, //display offset
	0x40,		//set display start line
	0x8d, 0x14, //set charge pump
	0xa1,		//set segment remap
	0xc8,		//set com output scan direction
	0xda, 0x00, //set com pins hardware configuration
	0x81, 0x80, //set contrast control
	0xd9, 0x1f, //set pre-charge period
	0xdb, 0x40, //set vcom deselect level
	0xa4,		//Set Entire Display On/Off
	0xaf,		//set display on
};
/**
 * OLED writes commands and data functions
 * OLED writes commands, data functions, and changes the contents of these two functions if you want to migrate them to another development board
 * For example: I use the i2c2 interface, then you only need to change &hi2c1 to &hi2c2.
**/
void OLED_Write_cmd(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c2, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x100);
}
void OLED_Write_data(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c2, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x100);
}






/**
 * @brief	Image display function
 * @param x0  Image display start position x-axis
 * @param y0  Image display start position y-axis
 * @param x1  Image display end position x-axis 1 - 127
 * @param y1  Image display end position x-axis 1 - 4
 * @param BMP Image display pointer address
 * @note	The image needs to be converted to an array and passed into this function
*/
void OLED_ShowPic(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint16_t i = 0;
	uint8_t x, y;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Position(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_Write_data(BMP[i++]);
		}
	}
}

/**
 * @brief	Display a 16*16 pixel Chinese character
 * @param x  position x-axis  0 - 127
 * @param y  position y-axis  0 - 3
 * @param no  The order of the Chinese characters in the hzk[] array
 * @note	The Chinese character library is in the Hzk array in the oledfont.h file, 
 * You need to convert Chinese characters into arrays
*/
void OLED_ShowHanzi(uint8_t x, uint8_t y, uint8_t no)
{
	uint8_t t, adder = 0;
	OLED_Set_Position(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_Write_data(Hzk[2 * no][t]);
		adder += 1;
	}
	OLED_Set_Position(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_Write_data(Hzk[2 * no + 1][t]);
		adder += 1;
	}
}

/**
 * @brief	Display a 32*32 pixel Chinese character .all screen display
 * @param x  position x-axis  0 - 127
 * @param y  position y-axis  0
 * @param n  The order of the Chinese characters in the Hzb[] array
 * @note	
*/
void OLED_ShowHzbig(uint8_t x, uint8_t y, uint8_t n)
{
	uint8_t t, adder = 0;
	OLED_Set_Position(x, y);
	for (t = 0; t < 32; t++)
	{
		OLED_Write_data(Hzb[4 * n][t]);
		adder += 1;
	}
	OLED_Set_Position(x, y + 1);
	for (t = 0; t < 32; t++)
	{
		OLED_Write_data(Hzb[4 * n + 1][t]);
		adder += 1;
	}

	OLED_Set_Position(x, y + 2);
	for (t = 0; t < 32; t++)
	{
		OLED_Write_data(Hzb[4 * n + 2][t]);
		adder += 1;
	}
	OLED_Set_Position(x, y + 3);
	for (t = 0; t < 32; t++)
	{
		OLED_Write_data(Hzb[4 * n + 3][t]);
		adder += 1;
	}
}

/**
 * @brief	Display a float 
 * @param x  position x-axis  0 - 127
 * @param y  position y-axis  0
 * @param num  The order of the Chinese characters in the Hzb[] array
 * @param accuracy Preserve decimal places
 * @param fontsize 8/16
 * @note	
*/
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t accuracy, uint8_t fontsize)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t t = 0;
	uint8_t temp = 0;
	uint16_t numel = 0;
	uint32_t integer = 0;
	float decimals = 0;

	//Is a negative number?
	if (num < 0)
	{
		OLED_ShowChar(x, y, '-', fontsize);
		num = 0 - num;
		i++;
	}

	integer = (uint32_t)num;
	decimals = num - integer;

	//Integer part
	if (integer)
	{
		numel = integer;

		while (numel)
		{
			numel /= 10;
			j++;
		}
		i += (j - 1);
		for (temp = 0; temp < j; temp++)
		{
			OLED_ShowChar(x + 8 * (i - temp), y, integer % 10 + '0', fontsize); // 显示整数部分
			integer /= 10;
		}
	}
	else
	{
		OLED_ShowChar(x + 8 * i, y, temp + '0', fontsize);
	}
	i++;
	//Decimal part
	if (accuracy)
	{
		OLED_ShowChar(x + 8 * i, y, '.', fontsize);

		i++;
		for (t = 0; t < accuracy; t++)
		{
			decimals *= 10;
			temp = (uint8_t)decimals;
			OLED_ShowChar(x + 8 * (i + t), y, temp + '0', fontsize);
			decimals -= temp;
		}
	}
}

/**
 * @brief	OLED pow function
 * @param m - base
 * @param n - exponent
 * @return result
*/
static uint32_t OLED_Pow(uint8_t a, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= a;
	}
	return result;
}

/**
 * @brief	Display a uint32 Interger
 * @param x  position x-axis  0 - 127
 * @param y  position y-axis  0 - 3
 * @param num  Displayed integers
 * @param length Number of integer digits
 * @note	
*/
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t length, uint8_t fontsize)
{

	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < length; t++)
	{
		temp = (num / OLED_Pow(10, length - t - 1)) % 10;
		if (enshow == 0 && t < (length - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (fontsize / 2) * t, y, ' ', fontsize);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (fontsize / 2) * t, y, temp + '0', fontsize);
	}
}


/**
 * @brief	Display ascii string
 * @param x  String start position on the X-axis  range：0 - 127
 * @param y  String start position on the Y-axis  range：0 - 3 
 * @param ch  String pointer
 * @param fontsize You can choose from two fonts 8/16
**/
void OLED_ShowStr(uint8_t x, uint8_t y, char *ch, uint8_t fontsize)
{
	uint8_t j = 0;
	while (ch[j] != '\0')
	{
		OLED_ShowChar(x, y, ch[j], fontsize);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}

/**
 * @brief	Displays ASCII characters
 * @param x  Character position on the X-axis  range：0 - 127
 * @param y  Character position on the Y-axis  range：0 - 3 
 * @param no  character
 * @param fontsize You can choose from three fonts 8/16
**/
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t fontsize)
{
	uint8_t c = 0, i = 0;
	c = ch - ' ';

	if (x > 127) //beyond the right boundary
	{
		x = 0;
		y++;
	}

	if (fontsize == 16)
	{
		OLED_Set_Position(x, y);
		for (i = 0; i < 8; i++)
		{
			OLED_Write_data(F8X16[c * 16 + i]);
		}
		OLED_Set_Position(x, y + 1);
		for (i = 0; i < 8; i++)
		{
			OLED_Write_data(F8X16[c * 16 + i + 8]);
		}
	}
	else
	{
		OLED_Set_Position(x, y);
		for (i = 0; i < 6; i++)
		{
			OLED_Write_data(F6X8[c][i]);
		}
	}
}


/**
 * OLED fill function, after using the function 0.91 inch oled screen into full white
**/
void OLED_Allfill(void)
{
	uint8_t i, j;
	for (i = 0; i < 4; i++)
	{
		OLED_Write_cmd(0xb0 + i);
		OLED_Write_cmd(0x00);
		OLED_Write_cmd(0x10);
		for (j = 0; j < 128; j++)
		{
			OLED_Write_data(0xFF);
		}
	}
}

/**
 * @brief Set coordinates
 * @param x: X position, range 0 - 127  Because our OLED screen resolution is 128*32, so the horizontal is 128 pixels
 * @param y: Y position, range 0 - 3    Because the vertical pixels are positioned in pages, each page has 8 pixels, so there are 4 pages
**/
void OLED_Set_Position(uint8_t x, uint8_t y)
{
	OLED_Write_cmd(0xb0 + y);
	OLED_Write_cmd(((x & 0xf0) >> 4) | 0x10);
	OLED_Write_cmd((x & 0x0f) | 0x00);
}
/**
 * Clear Screen Function
 * Fill each row and column with 0
**/
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 4; i++)
	{
		OLED_Write_cmd(0xb0 + i);
		OLED_Write_cmd(0x00);
		OLED_Write_cmd(0x10);
		for (n = 0; n < 128; n++)
		{
			OLED_Write_data(0);
		}
	}
}
/**
 * Turn screen display on and off
**/
void OLED_Display_On(void)
{
	OLED_Write_cmd(0x8D);
	OLED_Write_cmd(0x14);
	OLED_Write_cmd(0xAF);
}
void OLED_Display_Off(void)
{
	OLED_Write_cmd(0x8D);
	OLED_Write_cmd(0x10);
	OLED_Write_cmd(0xAF);
}

/**
 * Initialize the screen
 * Function:send control words one by one
**/
void OLED_Init(void)
{

	HAL_Delay(100);
	uint8_t i;
	for (i = 0; i < sizeof(initcmd1); i++)
	{

		OLED_Write_cmd(initcmd1[i]); //display off
	}

	OLED_Clear();
	OLED_Set_Position(0, 0);
}
