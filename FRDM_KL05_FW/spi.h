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
#include "stddef.h"
#include "stdbool.h"

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

/**
 * @brief SPI DMA busy check.
 * @retval true SPI DMA is busy.
 *         false SPI DMA is not busy.
 */
bool SPI_DMA_isBusy(void);

/**
 * @brief SPI with DMA transfer operation.
 * @param addr Pointer to data.
 * @param length Length of data.
 * @retval true transfer initiated successfully.
 *         false DMA is busy.
 */
bool SPI_DMA_send(void * addr, size_t length);


/**
 * @brief DMA initialization.
 */
void SPI_DMA_Init(void);

#endif /* SPI_H */
