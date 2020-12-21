/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file spi.c
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing definitions for SPI.
 */

#include "spi.h"

#define MOSI   7
#define MISO   6
#define SCK    0

void SPI_Init(void) 
{	
    SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;       /* clock for SPI0  */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      /* clock for PORTB */
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;      /* clock for PORTA */
    PORTA->PCR[MOSI] |= PORT_PCR_MUX(3);     /* SPI MOSI (PTA7) */
    PORTA->PCR[MISO] |= PORT_PCR_MUX(3);     /* SPI MISO (PTA6) */
    PORTB->PCR[SCK] |= PORT_PCR_MUX(3);      /* SPI SCK	(PTB0) */
    SPI0->C1 |= SPI_C1_SPE_MASK;             /* SPI system enabled */
    SPI0->C1 |= SPI_C1_MSTR_MASK;            /* SPI module configured as a master SPI device */
    SPI0->C1 &= ~(SPI_C1_CPOL_MASK);         /* Clock polarity, active-high SPI clock */
    SPI0->C2 |= SPI_C2_MODFEN_MASK;          /* Mode fault function enabled
                                                master SS pin acts as the mode fault input or the slave select output */
    SPI0->C1 |= SPI_C1_SSOE_MASK;            /* SS pin function is automatic SS output */
    SPI0->BR |= SPI_BR_SPPR(3);              /* Baud rate prescaler divisor is 3 */
    SPI0->BR |= SPI_BR_SPR(0);               /* Baud rate divisor is 2 */
}

uint8_t SPI_transfer(uint8_t byte)
{
    (void)SPI0->S;                           /* read S register before writing to the SPI data register */
    SPI0->D = byte;
    while(!(SPI0->S & SPI_S_SPTEF_MASK))
    {}
    return SPI0->D;
}
