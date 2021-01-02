/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file lcd_nokia5110.h
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing declarations for LCD control functions.
 */

#ifndef LCD_NOKIA5110_H
#define LCD_NOKIA5110_H

#include "spi.h"

/**
 * @brief Nokia5110 display initialization.
 */
void nokia5110_init(void);

/**
 * @brief Display data on Nokia5110 LCD
 */
void nokia5110_displayHC_SR04_value(void);

/**
 * @brief Set hundreds digit.
 */
void nokia5110_setHundreds(uint8_t digit);

/**
 * @brief Set tens digit.
 */
void nokia5110_setTens(uint8_t digit);

/**
 * @brief Set units digit.
 */
void nokia5110_setUnits(uint8_t digit);

#endif /* LCD_NOKIA5110_H */
