#ifndef __MENU_H
#define __MENU_H

#include "main.h"
#include "lcd.h"

typedef enum {
	MENU_ACTION_NONE = 0,
	MENU_ACTION_CONNECT,
	MENU_ACTION_SETTINGS,
	MENU_ACTION_INFO
} MenuAction;

void Menu_Init(void);
void Menu_Draw(void);
void Menu_Highlight(uint8_t index);
MenuAction Menu_HandleInput(int8_t delta);
void Menu_ShowWelcome(const char *subtitle, uint32_t ms);

#endif


