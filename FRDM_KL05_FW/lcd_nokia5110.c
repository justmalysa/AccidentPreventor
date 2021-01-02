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

#define CMD_INIT1 0x21                        /* set extended instructions mode */
#define CMD_INIT2 0xB7                        /* set contrast to display */
#define CMD_INIT3 0x04                        /* set temperature coefficient 0 */
#define CMD_INIT4 0x14                        /* set bias system */
#define CMD_INIT5 0x20                        /* set basic instructions mode */
#define CMD_INIT6 0x0C                        /* set display normal mode */

#define DISPLAY_WIDTH_PX               84
#define DISPLAY_HEIGH_PX               48
#define DISPLAY_HEIGHT_BANKS            6
#define DISTANCE_SIZE                 168
#define CM_SIZE                       168
#define HC_SR04_SIZE                  168
#define DIGIT_SIZE                     20
#define DIGIT_WIDTH                    10
#define DIGIT_HUNDREDS_OFFSET          20
#define DIGIT_SPACING                   5


/* array with values to display the string "distance" */
static const uint8_t distance_tab[DISTANCE_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0xE0, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00,
                                                    0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00,
                                                    0xE0, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00,
                                                    0x20, 0x20, 0x20, 0xE0, 0x20, 0x20, 0x20, 0x00,
                                                    0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0xE0, 0x00,
                                                    0x00, 0x00, 0xE0, 0x40, 0x80, 0x00, 0x00, 0xE0,
                                                    0x00, 0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0x20,
                                                    0x00, 0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0x20,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x10, 0x10,
                                                    0x10, 0x10, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x1F,
                                                    0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11,
                                                    0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x01,
                                                    0x01, 0x01, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x00,
                                                    0x01, 0x06, 0x08, 0x1F, 0x00, 0x00, 0x00, 0x1F,
                                                    0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x1F,
                                                    0x11, 0x11, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* array with values to display the string "cm" */
static const uint8_t cm_tab[CM_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
                                        0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
                                        0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x1F, 0x01,
                                        0x02, 0x01, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* array representing value from HC-SR04 sensor */
static uint8_t HC_SR04_tab[HC_SR04_SIZE]= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* array representing value 0 */
static const uint8_t digit_0_tab[DIGIT_SIZE] = {0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80,
                                                0x80, 0x80, 0x80, 0xFF};

/* array representing value 1 */
static const uint8_t digit_1_tab[DIGIT_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0xFF};

/* array representing value 2 */
static const uint8_t digit_2_tab[DIGIT_SIZE] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0xFE, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0x83};

/* array representing value 3 */
static const uint8_t digit_3_tab[DIGIT_SIZE] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0xFF};

/* array representing value 4 */
static const uint8_t digit_4_tab[DIGIT_SIZE] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0xF0, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02,
                                                0x02, 0x02, 0x02, 0xFF};

/* array representing value 5 */
static const uint8_t digit_5_tab[DIGIT_SIZE] = {0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0x10, 0x83, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0xFE};

/* array representing value 6 */
static const uint8_t digit_6_tab[DIGIT_SIZE] = {0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0x10, 0xFF, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0xFE};

/* array representing value 7 */
static const uint8_t digit_7_tab[DIGIT_SIZE] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x00, 0x00, 0x00, 0xFF};

/* array representing value 8 */
static const uint8_t digit_8_tab[DIGIT_SIZE] = {0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0xFF, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0xFF};

/* array representing value 9 */
static const uint8_t digit_9_tab[DIGIT_SIZE] = {0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
                                                0x10, 0xF0, 0x83, 0x82, 0x82, 0x82, 0x82, 0x82,
                                                0x82, 0x82, 0x82, 0xFF};

/* array containing pointers do digits */
static const uint8_t * const pointers_tab[] = {digit_0_tab, digit_1_tab, digit_2_tab, digit_3_tab,
                                               digit_4_tab, digit_5_tab, digit_6_tab, digit_7_tab,
                                               digit_8_tab, digit_9_tab};


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

static void nokia5110_displayInitialValues(void)
{
    /* display */
    for(uint8_t i=0; i<DISTANCE_SIZE; i++)
    {
        SPI_transfer(distance_tab[i]);
    }
    
    /* cm */
    nokia5110_setCmdMode();
    SPI_transfer(0x44);                /* set y coursor at 4 byte*/
    delay_ms(1);
    SPI_transfer(0x80);                /* set x coursor at the beginning of the line */
    delay_ms(1);
    nokia5110_setDataMode();
    for(uint8_t i=0; i<CM_SIZE; i++)
    {
        SPI_transfer(cm_tab[i]);
    }
}

void nokia5110_displayHC_SR04_value(void)
{
    nokia5110_setCmdMode();
    SPI_transfer(0x42);                /* set y coursor at 2 byte*/
    delay_ms(1);
    SPI_transfer(0x80);                /* set x coursor at the beginning of the line */
    delay_ms(1);
    nokia5110_setDataMode();
    
    for(uint8_t i=0; i<HC_SR04_SIZE; i++)
    {
        SPI_transfer(HC_SR04_tab[i]);
    }
}

void nokia5110_setHundreds(uint8_t digit)
{
    const uint8_t * p_digit = pointers_tab[digit];
    
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+i] = p_digit[i];
    }
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+DISPLAY_WIDTH_PX+i] = p_digit[i+DIGIT_WIDTH];
    }
}

void nokia5110_setTens(uint8_t digit)
{
    const uint8_t * p_digit = pointers_tab[digit];
    
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+DIGIT_SPACING+DIGIT_WIDTH+i] = p_digit[i];
    }
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+DISPLAY_WIDTH_PX+DIGIT_SPACING+DIGIT_WIDTH+i] = p_digit[i+DIGIT_WIDTH];
    }
}

void nokia5110_setUnits(uint8_t digit)
{
    const uint8_t * p_digit = pointers_tab[digit];
    
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+2*DIGIT_SPACING+2*DIGIT_WIDTH+i] = p_digit[i];
    }
    for(uint8_t i = 0; i<DIGIT_WIDTH; i++)
    {
        HC_SR04_tab[DIGIT_HUNDREDS_OFFSET+DISPLAY_WIDTH_PX+2*DIGIT_SPACING+2*DIGIT_WIDTH+i] = p_digit[i+DIGIT_WIDTH];
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
    nokia5110_displayInitialValues();
}
