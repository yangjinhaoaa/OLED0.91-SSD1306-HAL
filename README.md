# 128×32 0.91寸OLED驱动，基于STM32 HAL库

## 硬件参数

驱动：SSD1306
接口：I2C
分辨率：128×32
尺寸：0.91寸
  ![0.96'OLED I2C](/datasheet/hardware.png)

## 数据手册

[0.91寸OLED开发手册](/datasheet/0.91白色14Pin.pdf)

[SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)

## How to use

使用STM32CubeMX生成工程
打开i2c

将oled.c添加到Src文件夹
将oledfont.h,oledpic.h添加到Inc文件夹

将以上文件添加到工程中

### 主函数编写顺序

> main.c中要添加```#include "oledpic.h"```（如果你使用```OLED_ShowPic```函数来显示图片）oledfont.h已经包含在oled.c中所以不用再引用 

初始化I2C ```MX_I2C2_Init();```

初始化OLED屏幕 ```OLED_Init();```

接下来就可以调用OLED显示函数了。




## 函数使用说明

- OLED写命令，写数据函数，如果想移植到其他开发板上可以更改这个两个函数的内容
  例如：我使用了i2c2接口，那么你只需要将&hi2c1改成&hi2c2就可以了。
	```C
  	void OLED_Write_cmd(uint8_t cmd);
  	void OLED_Write_data(uint8_t data);
	```
-  图片显示函数 图片开始、结束显示位置 x0(0~3),y0(0~128),x1(0~4),y1(1~127)，图片显示指针地址。图片取模方式：阴码，逆向，列行式，十六进制
	```C
	void OLED_ShowPic(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[]);	
	```
- 16x16汉字显示，no是该汉字在oledfont.h文件中Hzk[]数组中的序号。OLED_ShowHzbig是显示32x32的汉字。
	```C
	void OLED_ShowHanzi(uint8_t x, uint8_t y, uint8_t no);
	void OLED_ShowHzbig(uint8_t x, uint8_t y, uint8_t n);
	```
- 显示浮点数，accuracy是保留小数点的位数
	```C
	void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t accuracy, uint8_t fontsize);
	```
- 显示整数，length是显示位数
	```C
	void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t length, uint8_t fontsize);
	```
- 显示字符，字符串，fontsize可以是8或者16，分别显示6x8,8x16大小的ASCII字符
	```C
		void OLED_ShowStr(uint8_t x, uint8_t y, char *ch, uint8_t fontsize);
		void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t fontsize);
	```
- 全部填充函数，使用该函数后屏幕全部亮起
	```C
	void OLED_Allfill(void);
	```
- 设置坐标，函数库文件内部使用
	```C
	void OLED_Set_Position(uint8_t x, uint8_t y);
	```
- 清屏，清除所有像素
	```C
	void OLED_Clear(void);
	```
- OLED初始化，在使用以上函数前调用```OLED_Init```，该函数会将一些液晶控制字发送到驱动，使能显示器，必须首先调用
	```C
	void OLED_Init(void);
	```
## 示例
### 使用STM32F334R8 HAL库 ARM-MDK
使用硬件I2C1，SDA接PB7，SCL接PA15
具体配置可以看```example-F334R8-0.91OLED.ioc```文件（使用STM32CubeMX打开）

[example-F334R8-0.91OLED](/example-F334R8-0.91OLED/)

### 使用STM32F103R8T6 HAL库 ARM-MDK

使用硬件I2C2，SCL->PB10 SDA->PB11 （我的I2C1好像有问题😭）
[example-F334R8-0.91OLED](/example-F103R8-0.91OLED/)

### 使用STM32F03R8T6 HAL库 PlatformIO
硬件按配置使用I2C2，使用PlatformIO开发
[example-F103R8-0.91OLED-PlatformIO](/example-F103R8-0.91OLED-PlatformIO/)

