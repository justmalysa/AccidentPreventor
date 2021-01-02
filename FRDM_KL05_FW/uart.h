/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file uart.h
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing declarations for UART.
 */


#ifndef UART_H
#define UART_H

#include "MKL05Z4.h"

/**
 * @brief UART initialization.
 */
void UART_Init(void);

/**
 * @brief Get last received byte from UART.
 */
uint8_t UART_GetReceivedData(void);

#endif /* UART_H */
