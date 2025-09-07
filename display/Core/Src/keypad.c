#include "keypad.h"

/* Keypad mapping table */
static const char keypad_map[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

/* GPIO pin arrays for rows and columns */
static const uint16_t row_pins[KEYPAD_ROWS] = {L1_Pin, L2_Pin, L3_Pin, L4_Pin};
static const GPIO_TypeDef* row_ports[KEYPAD_ROWS] = {L1_GPIO_Port, L2_GPIO_Port, L3_GPIO_Port, L4_GPIO_Port};

static const uint16_t col_pins[KEYPAD_COLS] = {C1_Pin, C2_Pin, C3_Pin, C4_Pin};
static const GPIO_TypeDef* col_ports[KEYPAD_COLS] = {C1_GPIO_Port, C2_GPIO_Port, C3_GPIO_Port, C4_GPIO_Port};

/* Keypad state */
static KeypadState_t keypad_state = {0};

/**
 * @brief Initialize the keypad GPIO pins
 */
void Keypad_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Configure row pins as output (initially high)
    GPIO_InitStruct.Pin = L1_Pin | L2_Pin | L3_Pin | L4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Set all row pins high initially
    HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(L4_GPIO_Port, L4_Pin, GPIO_PIN_SET);
    
    // Configure column pins as input with pull-up
    GPIO_InitStruct.Pin = C1_Pin | C2_Pin | C3_Pin | C4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize state
    keypad_state.last_scan_time = 0;
    keypad_state.last_key_time = 0;
    keypad_state.last_key = KEYPAD_KEY_NONE;
    keypad_state.current_key = KEYPAD_KEY_NONE;
    keypad_state.key_pressed = 0;
    keypad_state.key_released = 0;
}

/**
 * @brief Scan the keypad for pressed keys
 * @return The pressed key or KEYPAD_KEY_NONE if no key is pressed
 */
KeypadKey_t Keypad_Scan(void)
{
    uint32_t current_time = HAL_GetTick();
    KeypadKey_t pressed_key = KEYPAD_KEY_NONE;
    
    // Check if enough time has passed since last scan
    if (current_time - keypad_state.last_scan_time < KEYPAD_SCAN_DELAY_MS) {
        return keypad_state.current_key;
    }
    
    keypad_state.last_scan_time = current_time;
    
    // Scan each row
    for (uint8_t row = 0; row < KEYPAD_ROWS; row++) {
        // Set current row low, others high
        for (uint8_t r = 0; r < KEYPAD_ROWS; r++) {
            if (r == row) {
                HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_RESET);
            } else {
                HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);
            }
        }
        
        // Small delay for signal stabilization
        HAL_Delay(1);
        
        // Check each column
        for (uint8_t col = 0; col < KEYPAD_COLS; col++) {
            if (HAL_GPIO_ReadPin(col_ports[col], col_pins[col]) == GPIO_PIN_RESET) {
                // Key pressed - calculate key number
                pressed_key = (KeypadKey_t)(row * KEYPAD_COLS + col + 1);
                break;
            }
        }
        
        if (pressed_key != KEYPAD_KEY_NONE) {
            break;
        }
    }
    
    // Set all rows high again
    for (uint8_t r = 0; r < KEYPAD_ROWS; r++) {
        HAL_GPIO_WritePin(row_ports[r], row_pins[r], GPIO_PIN_SET);
    }
    
    // Update state with debouncing
    if (pressed_key != keypad_state.current_key) {
        if (current_time - keypad_state.last_key_time >= KEYPAD_DEBOUNCE_MS) {
            keypad_state.last_key = keypad_state.current_key;
            keypad_state.current_key = pressed_key;
            keypad_state.last_key_time = current_time;
            
            // Set flags for key press/release detection
            if (keypad_state.last_key != KEYPAD_KEY_NONE && keypad_state.current_key == KEYPAD_KEY_NONE) {
                keypad_state.key_released = 1;
            } else if (keypad_state.last_key == KEYPAD_KEY_NONE && keypad_state.current_key != KEYPAD_KEY_NONE) {
                keypad_state.key_pressed = 1;
            }
        }
    }
    
    return keypad_state.current_key;
}

/**
 * @brief Get the currently pressed key
 * @return The pressed key or KEYPAD_KEY_NONE if no key is pressed
 */
KeypadKey_t Keypad_GetKey(void)
{
    return keypad_state.current_key;
}

/**
 * @brief Check if a specific key is currently pressed
 * @param key The key to check
 * @return 1 if key is pressed, 0 otherwise
 */
uint8_t Keypad_IsKeyPressed(KeypadKey_t key)
{
    return (keypad_state.current_key == key) ? 1 : 0;
}

/**
 * @brief Check if a key was just released
 * @param key The key to check
 * @return 1 if key was just released, 0 otherwise
 */
uint8_t Keypad_IsKeyReleased(KeypadKey_t key)
{
    if (keypad_state.key_released && keypad_state.last_key == key) {
        keypad_state.key_released = 0;
        return 1;
    }
    return 0;
}

/**
 * @brief Clear the keypad state flags
 */
void Keypad_ClearState(void)
{
    keypad_state.key_pressed = 0;
    keypad_state.key_released = 0;
}

/**
 * @brief Get navigation delta for menu control
 * @return -1 for up, 1 for down, 0 for no navigation
 */
int8_t Keypad_GetNavigationDelta(void)
{
    KeypadKey_t key = Keypad_Scan();
    
    // Check for key release events
    if (Keypad_IsKeyReleased(KEYPAD_KEY_2)) {  // Up
        return -1;
    }
    if (Keypad_IsKeyReleased(KEYPAD_KEY_8)) {  // Down
        return 1;
    }
    
    return 0;
}

/**
 * @brief Check if select/confirm key was pressed
 * @return 1 if select key was pressed, 0 otherwise
 */
uint8_t Keypad_IsSelectPressed(void)
{
    return Keypad_IsKeyReleased(KEYPAD_KEY_5);  // Center key for select
}

