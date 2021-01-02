/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file uart.c
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing definitions for UART.
 */

#include "uart.h"

#define MCGFLLCLK       0x01

static volatile uint8_t received_data;

void UART0_IRQHandler(void);

void UART_Init(void)
{
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;                    /* clock for UART0 */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;                    /* clock for PORTB */
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);           /* select MCGFLLCLK clock for the UART0 transmit and receive clock */
    PORTB->PCR[1] = PORT_PCR_MUX(2);                       /* PTB1 = TX_D */
    PORTB->PCR[2] = PORT_PCR_MUX(2);                       /* PTB2 = RX_D */

    UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );  /* transmitter and receiver disabled */
    UART0->BDH = 0x01;
    UART0->BDL = 0x11;                                     /* set Baud Rate = 9600 */
    UART0->C4 &= ~UART0_C4_OSR_MASK;                       /* clear OSR */
    UART0->C4 |= UART0_C4_OSR(0x0F);                       /* set OSR at 15 */
    UART0->C5 |= UART0_C5_BOTHEDGE_MASK;                   /* enable sampling on both edges of the baud rate clock */
    UART0->C2 |= UART0_C2_RE_MASK | UART0_C2_RIE_MASK;     /* enable receiver and RX IRQ */
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void)
{
    received_data = UART0->D;                              /* read data and clear RDRF flag */
}

uint8_t UART_GetReceivedData(void)
{
    return received_data;
}
