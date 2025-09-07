#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "main.h"

/* Keypad configuration */
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

/* Keypad debounce time in milliseconds */
#define KEYPAD_DEBOUNCE_MS 50

/* Keypad scan delay in milliseconds */
#define KEYPAD_SCAN_DELAY_MS 10

/* Keypad key definitions */
typedef enum {
    KEYPAD_KEY_NONE = 0,
    KEYPAD_KEY_1 = 1,
    KEYPAD_KEY_2 = 2,
    KEYPAD_KEY_3 = 3,
    KEYPAD_KEY_A = 4,
    KEYPAD_KEY_4 = 5,
    KEYPAD_KEY_5 = 6,
    KEYPAD_KEY_6 = 7,
    KEYPAD_KEY_B = 8,
    KEYPAD_KEY_7 = 9,
    KEYPAD_KEY_8 = 10,
    KEYPAD_KEY_9 = 11,
    KEYPAD_KEY_C = 12,
    KEYPAD_KEY_STAR = 13,
    KEYPAD_KEY_0 = 14,
    KEYPAD_KEY_HASH = 15,
    KEYPAD_KEY_D = 16
} KeypadKey_t;

/* Keypad state structure */
typedef struct {
    uint32_t last_scan_time;
    uint32_t last_key_time;
    KeypadKey_t last_key;
    KeypadKey_t current_key;
    uint8_t key_pressed;
    uint8_t key_released;
} KeypadState_t;

/* Function prototypes */
void Keypad_Init(void);
KeypadKey_t Keypad_Scan(void);
KeypadKey_t Keypad_GetKey(void);
uint8_t Keypad_IsKeyPressed(KeypadKey_t key);
uint8_t Keypad_IsKeyReleased(KeypadKey_t key);
void Keypad_ClearState(void);

/* Helper functions for menu navigation */
int8_t Keypad_GetNavigationDelta(void);
uint8_t Keypad_IsSelectPressed(void);

#endif /* __KEYPAD_H */

