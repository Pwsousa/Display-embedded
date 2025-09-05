#include "menu.h"
#include "lcd.h"

static uint8_t s_selectedIndex = 0;
static const uint16_t s_headerHeight = 60;
static uint16_t s_itemBaseHeight = 48;
static uint16_t s_itemTailExtra = 0;

static const char *s_items[] = {
	"Conectar",
	"Configurar",
	"Informacoes",
};

static void Menu_DrawHeader(void)
{
	POINT_COLOR = WHITE;
	BACK_COLOR = BLUE;
	LCD_Fill(0, 0, lcddev.width-1, s_headerHeight-1, BLUE);
	LCD_ShowString(8, (s_headerHeight-24)/2, lcddev.width-16, 24, 24, (uint8_t*)"IoT Menu");
}

static void Menu_DrawItem(uint8_t index, uint16_t y, uint16_t h, uint8_t selected)
{
	uint16_t bg = selected ? LBBLUE : LGRAY;
	uint16_t fg = selected ? WHITE : BLACK;
	LCD_Fill(0, y, lcddev.width-1, y + h - 1, bg);
	POINT_COLOR = fg;
	BACK_COLOR = bg;
	LCD_ShowString(16, y + (h-24)/2, lcddev.width-32, 24, 24, (uint8_t*)s_items[index]);
}

void Menu_Init(void)
{
	LCD_Display_Dir(1);
	LCD_Clear(BLACK);
	Menu_Draw();
}

void Menu_ShowWelcome(const char *subtitle, uint32_t ms)
{
	LCD_Display_Dir(1);
	LCD_Clear(BLACK);
	// Header bar
	POINT_COLOR = WHITE;
	BACK_COLOR = DARKBLUE;
	LCD_Fill(0, 0, lcddev.width-1, 59, DARKBLUE);
	LCD_ShowString(12, 18, lcddev.width-24, 24, 24, (uint8_t*)"Bem-vindo");

	// Body
	BACK_COLOR = BLACK;
	POINT_COLOR = GREEN;
	LCD_ShowString(12, 100, lcddev.width-24, 24, 24, (uint8_t*)"Sabores Conectados");
	POINT_COLOR = LGRAY;
	if (subtitle && *subtitle)
	{
		LCD_ShowString(12, 136, lcddev.width-24, 24, 24, (uint8_t*)subtitle);
	}

	// Simple underline accent
	LCD_DrawLine(12, 128, lcddev.width-12, 128, LGRAYBLUE);

	HAL_Delay(ms);
}

void Menu_Draw(void)
{
	Menu_DrawHeader();
	uint8_t count = (sizeof(s_items)/sizeof(s_items[0]));
	uint16_t remaining = (lcddev.height > s_headerHeight) ? (lcddev.height - s_headerHeight) : 0;
	s_itemBaseHeight = (count > 0) ? (remaining / count) : remaining;
	s_itemTailExtra = (count > 0) ? (remaining - (s_itemBaseHeight * count)) : 0;
	uint16_t y = s_headerHeight;
	for (uint8_t i = 0; i < count; i++)
	{
		uint16_t h = s_itemBaseHeight + ((i == (count-1)) ? s_itemTailExtra : 0);
		if (h == 0) h = 1;
		Menu_DrawItem(i, y, h, i == s_selectedIndex);
		y += h;
	}
}

void Menu_Highlight(uint8_t index)
{
	uint8_t max = (sizeof(s_items)/sizeof(s_items[0]));
	if (index >= max) return;
	uint16_t yBase = s_headerHeight;
	// compute start y and height for previous and new indices
	uint16_t prevY = yBase + s_itemBaseHeight * s_selectedIndex;
	uint16_t prevH = s_itemBaseHeight + ((s_selectedIndex == (max-1)) ? s_itemTailExtra : 0);
	uint16_t newY  = yBase + s_itemBaseHeight * index;
	uint16_t newH  = s_itemBaseHeight + ((index == (max-1)) ? s_itemTailExtra : 0);
	// redraw previous and new selections only
	Menu_DrawItem(s_selectedIndex, prevY, prevH, 0);
	Menu_DrawItem(index, newY, newH, 1);
	s_selectedIndex = index;
}

MenuAction Menu_HandleInput(int8_t delta)
{
	uint8_t max = (sizeof(s_items)/sizeof(s_items[0]));
	int16_t next = (int16_t)s_selectedIndex + (int16_t)delta;
	if (next < 0) next = max - 1;
	if (next >= (int16_t)max) next = 0;
	if ((uint8_t)next != s_selectedIndex)
	{
		Menu_Highlight((uint8_t)next);
		return MENU_ACTION_NONE;
	}
	// delta == 0 can be used as select/confirm
	if (delta == 0)
	{
		switch (s_selectedIndex)
		{
			case 0: return MENU_ACTION_CONNECT;
			case 1: return MENU_ACTION_SETTINGS;
			case 2: return MENU_ACTION_INFO;
			default: break;
		}
	}
	return MENU_ACTION_NONE;
}


