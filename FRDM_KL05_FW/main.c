/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/
 
 /**
 * @file main.c
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing the main function. 
 */

#include "spi.h"

static uint8_t byte_s = 10u;
static uint8_t byte_r;

int main()
{
    SPI_Init();
    byte_r = SPI_transfer(byte_s);
    while(1)
    {
    }
}
