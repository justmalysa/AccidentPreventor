/**********************************************************
 * This file is a part of the Accident Preventor project  *
 **********************************************************/

/**
 * @file spi.h
 * @authors Malysa Justyna, Mazurek Szymon
 * @date Dec 2020
 * @brief File containing declarations for SPI.
 */

#ifndef SPI_H
#define SPI_H

#include "MKL05Z4.h"

/**
 * @brief SPI initialization.
 */
void SPI_Init(void);

/**
 * @brief SPI transfer operation.
 * @param byte Byte to be sent.
 * @return Received byte.
 */
uint8_t SPI_transfer(uint8_t byte);

#endif /* SPI_H */
