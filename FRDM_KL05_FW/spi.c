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

static volatile bool is_busy;

void DMA0_IRQHandler(void);

void SPI_Init(void)
{
    SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;       /* clock for SPI0  */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;      /* clock for PORTB */
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;      /* clock for PORTA */
    PORTA->PCR[MOSI] |= PORT_PCR_MUX(3);     /* SPI MOSI (PTA7) */
    PORTA->PCR[MISO] |= PORT_PCR_MUX(3);     /* SPI MISO (PTA6) */
    PORTB->PCR[SCK] |= PORT_PCR_MUX(3);      /* SPI SCK (PTB0) */
    SPI0->C1 |= SPI_C1_SPE_MASK;             /* SPI system enabled */
    SPI0->C1 |= SPI_C1_MSTR_MASK;            /* SPI module configured as a master SPI device */
    SPI0->C1 &= ~(SPI_C1_CPOL_MASK);         /* Clock polarity, active-high SPI clock */
    SPI0->C2 |= SPI_C2_MODFEN_MASK;          /* Mode fault function enabled
                                                master SS pin acts as the mode fault input or the slave select output */
    SPI0->C1 |= SPI_C1_SSOE_MASK;            /* SS pin function is automatic SS output */
    SPI0->BR |= SPI_BR_SPPR(3);              /* Baud rate prescaler divisor is 3 */
    SPI0->BR |= SPI_BR_SPR(0);               /* Baud rate divisor is 2 */
    SPI0->C2 |= SPI_C2_TXDMAE_MASK;          /* transmit DMA enabled */
}

uint8_t SPI_transfer(uint8_t byte)
{
    (void)SPI0->S;                           /* read S register before writing to the SPI data register */
    SPI0->D = byte;
    while(!(SPI0->S & SPI_S_SPTEF_MASK))
    {}
    return SPI0->D;
}

bool SPI_DMA_isBusy(void)
{
    return is_busy;
}

bool SPI_DMA_send(void * addr, size_t length)
{
    if(is_busy)
    {
        return false;
    }
    is_busy = true;
    DMAMUX0->CHCFG[0] = 0;                                     /* temporarily disable DMA channel in DMAMUX */

    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;             /* clear DMA DONE event so DMA can be reconfigured */

    DMA0->DMA[0].SAR = (uint32_t)addr;                         /* set DMA source address */
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR(length);           /* set DMA length */
    DMA0->DMA[0].DCR |= DMA_DCR_START_MASK | DMA_DCR_ERQ_MASK; /* start transfer */

    DMAMUX0->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK |               /* enable DMA channel in DMAMUX */
                        DMAMUX_CHCFG_SOURCE(17);               /* set SPI0 as DMA Channel Source */
    return true;
}

void SPI_DMA_Init(void)
{
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;               /* clock for DMA */
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;            /* clock for DMAMUX */
    DMA0->DMA[0].DCR |= DMA_DCR_EINT_MASK  |        /* enable interrupt on completion of transfer */
                        DMA_DCR_SINC_MASK  |        /* increment SAR value */
                        DMA_DCR_SSIZE(1)   |        /* set source size at 8 bit */
                        DMA_DCR_DSIZE(1)   |        /* set destination size at 8 bit */
                        DMA_DCR_D_REQ_MASK |        /* automatically clear ERQ bit after transfer */
                        DMA_DCR_CS_MASK;            /* enable cycle steal */
    DMA0->DMA[0].DAR = (uint32_t)&SPI0->D;          /* set DMA destination address */

    NVIC_ClearPendingIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(DMA0_IRQn);
}

void DMA0_IRQHandler(void)
{
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK;  /* clear DMA DONE event */
    is_busy = false;
}
