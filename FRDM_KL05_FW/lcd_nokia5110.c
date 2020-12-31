/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file lcd_nokia5110.c
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing definitions for LCD control functions.
 */

#include "lcd_nokia5110.h"

#define RST   7
#define DC    6

#define CMD_INIT1 0x21
#define CMD_INIT2 0xBF
#define CMD_INIT3 0x04
#define CMD_INIT4 0x14
#define CMD_INIT5 0x20
#define CMD_INIT6 0x0c

#define DISPLAY_WIDTH_PX 84
#define DISPLAY_HEIGH_PX 48
#define DISPLAY_HEIGHT_BANKS 6

static void delay_ms( int n) 
{
    volatile int i;
    volatile int j;
    for( i = 0 ; i < n; i++)
    for(j = 0; j < 3500; j++) {}
}

static void nokia5110_setCmdMode(void)
{
    PTB->PCOR = (1<<DC);                   /* set low state on DC pin */
}

static void nokia5110_setDataMode(void)
{
    PTB->PSOR = (1<<DC);                   /* set high state on DC pin */
}

static void nokia5110_reset(void)
{
    PTB->PSOR = (1<<RST);                  /* set high state on RST pin */
    delay_ms(1);
    PTB->PCOR = (1<<RST);                  /* set low state on RST pin */
    delay_ms(1);
    PTB->PSOR = (1<<RST);                  /* set high state on RST pin */
}

static void nokia5110_clearDisplay(void)
{
    for(uint8_t y=0 ; y<DISPLAY_HEIGHT_BANKS ; y++)
    {
        for(uint8_t x=0 ; x<DISPLAY_WIDTH_PX ; x++)
        {
            SPI_transfer(0x00);
        }
    }
}

void nokia5110_init(void)
{
    SPI_Init();
    PORTB->PCR[RST] |= PORT_PCR_MUX(1);    /* Set Pin RST MUX as GPIO */
    PORTB->PCR[DC] |= PORT_PCR_MUX(1);     /* Set Pin DC MUX as GPIO */
    PTB->PDDR |= (1<<RST) | (1<<DC);       /* Set Pin RST and DC as output */
    nokia5110_reset();
    nokia5110_setCmdMode();
    SPI_transfer(CMD_INIT1);               /* Initialize display */
    delay_ms(1);
    SPI_transfer(CMD_INIT2);
    delay_ms(1);
    SPI_transfer(CMD_INIT3);
    delay_ms(1);
    SPI_transfer(CMD_INIT4);
    delay_ms(1);
    SPI_transfer(CMD_INIT5);
    delay_ms(1);
    SPI_transfer(CMD_INIT6);
    delay_ms(1);
    nokia5110_setDataMode();
    nokia5110_clearDisplay();
    for(uint8_t i=0; i<10u; i++)           /* check if display works */
    {
        SPI_transfer(0xFF);
    }
}
