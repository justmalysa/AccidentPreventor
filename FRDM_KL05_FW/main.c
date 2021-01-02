/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/
 
 /**
 * @file main.c
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing the main function. 
 */

#include "lcd_nokia5110.h"
#include "uart.h"


/* Function for separating specified byte into individual decimal digits */
static void parseByte(uint8_t byte, uint8_t * hundred, uint8_t * ten, uint8_t * unit)
{
    *unit    = byte % 10;
    byte    /= 10;
    *ten     = byte % 10;
    byte    /= 10;
    *hundred = byte % 10;
}

int main()
{
    UART_Init();
    nokia5110_init();

    uint8_t hundred, ten, unit;
    while(1)
    {
        uint8_t byte = UART_GetReceivedData();
        parseByte(byte, &hundred, &ten, &unit);
        nokia5110_setHundreds(hundred);
        nokia5110_setTens(ten);
        nokia5110_setUnits(unit);
        nokia5110_displayHC_SR04_value();
    }
}
