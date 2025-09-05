#ifndef __LCD_H
#define __LCD_H
#include "main.h"
#include "stdlib.h"
///////////////////////////////////////////////// ////////////////////////////////
//STM32F4 project template-library function version
//Taobao shop: http://mcudev.taobao.com
//************************************************ ********************************
//V1.2 modification instructions
//Support the driver of SPFD5408, and print the LCD ID directly into HEX format. It is convenient to view the LCD driver IC.
//V1.3
//Added fast IO support
//Modified the polarity of backlight control (applicable to V1.8 and later development board versions)
//For the LCD module before version 1.8 (not including 1.8), please modify the LCD_LED=1 of the LCD_Init function; to LCD_LED=1;
//V1.4
//Modified the LCD_ShowChar function to draw characters using the point drawing function.
// Added support for horizontal and vertical screen display
//V1.5 20110730
//1, The bug that the color of B505 LCD is wrong has been modified.
//2, Modified the setting method of fast IO and horizontal and vertical screen.
//V1.6 20111116
//1, add driver support for LGDP4535 LCD
//V1.7 20120713
//1, add LCD_RD_DATA function
//2, add support for ILI9341
//3, increase the independent driver code of ILI9325
//4, add LCD_Scan_Dir function (use with caution)
//6, also modified some of the original functions to adapt to the operation of 9341
//V1.8 20120905
//1, add LCD important parameter setting structure lcddev
//2, add LCD_Display_Dir function, support online horizontal and vertical screen switching
//V1.9 20120911
//1, add RM68042 driver (ID: 6804), but 6804 does not support horizontal display! ! Reason: change the scanning method,
//Causes the 6804 coordinate setting to be invalid. I tried many methods but no solution.
//V2.0 20120924
//Without hardware reset, the ID read of ILI9341 will be misread as 9300. Modifying LCD_Init will not recognize
//In the case of (read the ID is 9300/illegal ID), the driver IC is forced to be specified as ILI9341, and the initialization of 9341 is performed.
//V2.1 20120930
//Fix the bug of ILI9325 reading color.
//V2.2 20121007
//Fix the bug of LCD_Scan_Dir.
//V2.3 20130120
//Add 6804 to support horizontal display
//V2.4 20131120
//1, add support for NT35310 (ID:5310) drive
//2, the new LCD_Set_Window function is added to set the window, which is useful for quick filling, but this function does not support 6804 when the screen is horizontal.
//V2.5 20140211
//1, add support for NT35510 (ID: 5510) drive
//V2.6 20140504
//1, add ASCII 24*24 font support (more font users can add by themselves)
//2, modify some function parameters to support MDK -O2 optimization
//3, for 9341/35310/35510, set the write time to the fastest, and increase the speed as much as possible
//4, remove the support of SSD1289, because 1289 is too slow, the read cycle is 1us...it is simply weird. Not suitable for F4 use
//5, fix the bug of reading color function of IC such as 68042 and C505.
//V2.7 20140514
//1, fix a bug in the LCD_Color_Fill function.
///////////////////////////////////////////////// ////////////////////////////////
 
//LCD important parameter set
typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef struct
{
u16 width; //LCD width
u16 height; //LCD height
u16 id; //LCD ID
u8 dir; //Horizontal or vertical screen control: 0, vertical screen; 1, horizontal screen.
u16 wramcmd; //Start writing gram command
u16 setxcmd; //Set x coordinate command
u16 setycmd; //Set the y coordinate command
}_lcd_dev;

//LCD parameters
extern _lcd_dev lcddev; //Manage important LCD parameters
//LCD pen color and background color
extern u16 POINT_COLOR;//default red
extern u16 BACK_COLOR; //Background color. The default is white


///////////////////////////////////////////////// ////////////////////////////////
//-----------------LCD port definition----------------
//#define LCD_LED PBout(1) //LCD backlight PB1
//LCD address structure
typedef struct
{
u16 LCD_REG;
u16 LCD_RAM;
} LCD_TypeDef;
//Use Bank1.sector1 of NOR/SRAM, address bit HADDR[27,26]=00 A18 as the data command differentiation line
//Note that STM32 will shift one bit to the right when setting it!
#define LCD_BASE ((u32)(0x60000000 | 0x00007FFFE))
#define LCD ((LCD_TypeDef *) LCD_BASE)
///////////////////////////////////////////////// ////////////////////////////////

//Scan direction definition
#define L2R_U2D 0 //From left to right, from top to bottom
#define L2R_D2U 1 //From left to right, from bottom to top
#define R2L_U2D 2 //From right to left, from top to bottom
#define R2L_D2U 3 //From right to left, from bottom to top

#define U2D_L2R 4 //From top to bottom, from left to right
#define U2D_R2L 5 //From top to bottom, from right to left
#define D2U_L2R 6 //From bottom to top, from left to right
#define D2U_R2L 7 //From bottom to top, from right to left

#define DFT_SCAN_DIR L2R_U2D //The default scan direction

//Paint color
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40 //Brown
#define BRRED 0XFC07 //Brown red
#define GRAY 0X8430 //Gray
//GUI color

#define DARKBLUE 0X01CF //Dark blue
#define LIGHTBLUE 0X7D7C //Light blue
#define GRAYBLUE 0X5458 //Gray blue
//The above three colors are the colors of PANEL
 
#define LIGHTGREEN 0X841F //Light green
//#define LIGHTGRAY 0XEF5B //Light gray (PANNEL)
#define LGRAY 0XC618 //PANNEL, background color of window

#define LGRAYBLUE 0XA651 //Light gray blue (middle layer color)
#define LBBLUE 0X2B12 //Light brown blue (select the inverse color of the item)

void LCD_Init(void); //Initialize
void LCD_DisplayOn(void); //Open display
void LCD_DisplayOff(void); //Turn off the display
void LCD_Clear(u16 Color); //Clear the screen
void LCD_SetCursor(u16 Xpos, u16 Ypos); //Set the cursor
void LCD_DrawPoint(u16 x,u16 y); //Draw point
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color); //Draw points quickly
u16 LCD_ReadPoint(u16 x,u16 y); //Read point
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r); //Draw a circle
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //Draw a line
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color); //Draw a rectangle
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color); //fill with a single color
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color); //Fill the specified color
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode); //Display a character
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size); //Display a number
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode); //Display number
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p); //Display a string, 12/16 font
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lcddelay(int ms);
void rotating_mysquare(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4);
void rotating_mytriangle(int x1, int x2, int x3, int y1, int y2, int y3);
void grow_mytree(int x0, int y0, float angle, int length, int level, int color);

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_Scan_Dir(u8 dir); //Set the screen scanning direction
void LCD_Display_Dir(u8 dir); //Set the screen display direction
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);//Set window
//9320/9325 LCD register
#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R4 0x04
#define R5 0x05
#define R6 0x06
#define R7 0x07
#define R8 0x08
#define R9 0x09
#define R10 0x0A
#define R12 0x0C
#define R13 0x0D
#define R14 0x0E
#define R15 0x0F
#define R16 0x10
#define R17 0x11
#define R18 0x12
#define R19 0x13
#define R20 0x14
#define R21 0x15
#define R22 0x16
#define R23 0x17
#define R24 0x18
#define R25 0x19
#define R26 0x1A
#define R27 0x1B
#define R28 0x1C
#define R29 0x1D
#define R30 0x1E
#define R31 0x1F
#define R32 0x20
#define R33 0x21
#define R34 0x22
#define R36 0x24
#define R37 0x25
#define R40 0x28
#define R41 0x29
#define R43 0x2B
#define R45 0x2D
#define R48 0x30
#define R49 0x31
#define R50 0x32
#define R51 0x33
#define R52 0x34
#define R53 0x35
#define R54 0x36
#define R55 0x37
#define R56 0x38
#define R57 0x39
#define R59 0x3B
#define R60 0x3C
#define R61 0x3D
#define R62 0x3E
#define R63 0x3F
#define R64 0x40
#define R65 0x41
#define R66 0x42
#define R67 0x43
#define R68 0x44
#define R69 0x45
#define R70 0x46
#define R71 0x47
#define R72 0x48
#define R73 0x49
#define R74 0x4A
#define R75 0x4B
#define R76 0x4C
#define R77 0x4D
#define R78 0x4E
#define R79 0x4F
#define R80 0x50
#define R81 0x51
#define R82 0x52
#define R83 0x53
#define R96 0x60
#define R97 0x61
#define R106 0x6A
#define R118 0x76
#define R128 0x80
#define R129 0x81
#define R130 0x82
#define R131 0x83
#define R132 0x84
#define R133 0x85
#define R134 0x86
#define R135 0x87
#define R136 0x88
#define R137 0x89
#define R139 0x8B
#define R140 0x8C
#define R141 0x8D
#define R143 0x8F
#define R144 0x90
#define R145 0x91
#define R146 0x92
#define R147 0x93
#define R148 0x94
#define R149 0x95
#define R150 0x96
#define R151 0x97
#define R152 0x98
#define R153 0x99
#define R154 0x9A
#define R157 0x9D
#define R192 0xC0
#define R193 0xC1
#define R229 0xE5
#endif




