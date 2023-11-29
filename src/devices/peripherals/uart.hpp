#pragma once
#include <stdint.h>
#include <cstddef>

#include "gic.hpp"

#define UART_IRQ        (6)

#define UART_CLOCK      (0x00)  // unknown not needed because baudrate will be default (hopefully)

#define UART_B          0x09000000                             //base address

//#define UARTDR		    *((volatile uint16_t*)(UART_B + 0x000))	// Data Register			            (12 bit)
#define UARTDR_ADDRESS	(UART_B + 0x000)

#define UARTRSR		    *((volatile uint8_t*)(UART_B + 0x004))	// Receive Status Register 		        (4 bit)
#define UARTECR		    *((volatile uint8_t*)(UART_B + 0x004))	// Error Clear Register			        (4 bit)

//#define UARTFR		    *((volatile uint16_t*)(UART_B + 0x018))	// Flag Register			            (9 bit)
#define UARTFR_ADDRESS  (UART_B + 0x018)	

#define UARTILPR	    *((volatile uint8_t*)(UART_B + 0x020))	// IrDA Low-Power Counter Register	    (8 bit)
#define UARTIBRD	    *((volatile uint16_t*)(UART_B + 0x024))	// Integer Baud Rate Register		    (16 bit)
#define UARTFBRD	    *((volatile uint8_t*)(UART_B + 0x028))	// Fractional Baud Rate Register	    (6 bit)

//#define UARTLCR_H	    *((volatile uint8_t*)(UART_B + 0x02C))	// Line Control Register		        (8 bit)
#define UARTLCR_H_ADDRESS	    (volatile uint8_t*)(UART_B + 0x02C)

//#define UARTCR		    *((volatile uint16_t*)(UART_B + 0x030))	// Control Register			            (16 bit)
#define UARTCR_ADDRESS  (UART_B + 0x030)

//#define UARTIFLS	    *((volatile uint8_t*)(UART_B + 0x034))	// Interrupt FIFO Level Select Register	(6 bit)
#define UARTIFLS_ADDRESS	    (volatile uint8_t*)(UART_B + 0x034)	// Interrupt FIFO Level Select Register	(6 bit)

//#define UARTIMSC	    *((volatile uint16_t*)(UART_B + 0x038))	// Interrupt Mask Set/Clear Register	(11 bit)
#define UARTIMSC_ADDRESS	    (volatile uint16_t*)(UART_B + 0x038)

#define UARTRIS		    *((volatile uint16_t*)(UART_B + 0x03C))	// Raw Interrupt Status Register	    (11 bit)
#define UARTMIS		    *((volatile uint16_t*)(UART_B + 0x040))	// Masked Interrupt Status Register	    (11 bit)
#define UARTICR		    *((volatile uint16_t*)(UART_B + 0x044))	// Interrupt Clear Register		        (11 bit)
#define UARTDMACR	    *((volatile uint8_t*)(UART_B + 0x048))	// DMA Control Register			        (3 bit)
#define UARTPeriphID0	*((volatile uint8_t*)(UART_B + 0xFE0))	// UARTPeriphID0 Register		        (8 bit)
#define UARTPeriphID1	*((volatile uint8_t*)(UART_B + 0xFE4))	// UARTPeriphID1 Register		        (8 bit)
#define UARTPeriphID2	*((volatile uint8_t*)(UART_B + 0xFE8))	// UARTPeriphID2 Register		        (8 bit)
#define UARTPeriphID3	*((volatile uint8_t*)(UART_B + 0xFEC))	// UARTPeriphID3 Register		        (8 bit)
#define UARTPCellID0	*((volatile uint8_t*)(UART_B + 0xFF0))	// UARTPCellID0 Register		        (8 bit)
#define UARTPCellID1	*((volatile uint8_t*)(UART_B + 0xFF4))	// UARTPCellID1 Register		        (8 bit)
#define UARTPCellID2	*((volatile uint8_t*)(UART_B + 0xFF8))	// UARTPCellID3 Register		        (8 bit)
#define UARTPCellID3	*((volatile uint8_t*)(UART_B + 0xFFC))	// UARTPCellID2 Register		        (8 bit)

namespace peripherals::uart {
    typedef struct 
    {
        uint8_t CTS : 1;    // 0 bit
        uint8_t DSR : 1;    // 1 bit
        uint8_t DCD : 1;    // ...
        uint8_t BUSY : 1;
        uint8_t RXFE : 1;
        uint8_t TXFF : 1;
        uint8_t RXFF : 1;
        uint8_t TXFE : 1;
        uint8_t RI : 1;
        uint8_t : 7;        // 15:9 Reserved, do not modify, read as zero
    } UARTFR;

    typedef struct
    {
        uint8_t DATA : 8;
        uint8_t FE : 1;
        uint8_t PE : 1;
        uint8_t BE : 1;
        uint8_t OE : 1;
        uint8_t : 4;        //15:12 Reserved
    } UARTDR;
    
    typedef struct {
        uint8_t UARTEN  : 1;
        uint8_t SIREN   : 1;
        uint8_t SIRLP   : 1;
        uint8_t         : 4;
        uint8_t LBE     : 1;
        uint8_t TXE     : 1;
        uint8_t RXE     : 1;
        uint8_t DTR     : 1;
        uint8_t RTS     : 1;
        uint8_t Out1    : 1;
        uint8_t Out2    : 1;
        uint8_t RTSEn   : 1;
        uint8_t CTSEn   : 1;
    } UARTCR;

    typedef struct {
        uint8_t BRK     : 1;
        uint8_t PEN     : 1;
        uint8_t EPS     : 1;
        uint8_t STP2    : 1;
        uint8_t FEN     : 1;
        uint8_t WLEN    : 2;
        uint8_t SPS     : 1;
        uint8_t         : 8;    // 15:8 Reserved, do not modify, read as zero
    } UARTLCR_H;

    typedef struct {
        uint8_t RIMIM   : 1;
        uint8_t CTSMIM  : 1;
        uint8_t DCDMIM  : 1;
        uint8_t DSRMIM  : 1;
        uint8_t RXIM    : 1;
        uint8_t TXIM    : 1;
        uint8_t RTIM    : 1;
        uint8_t FEIM    : 1;
        uint8_t PEIM    : 1;
        uint8_t BEIM    : 1;
        uint8_t OEIM    : 1;
        uint8_t         : 5;
    } UARTIMSC;
    
    typedef struct {
        uint8_t TXIFLSEL    : 2;
        uint8_t RXIFLSEL    : 3;
        int16_t             : 10;
    } UARTIFLS;
}

namespace peripherals::uart {

    namespace {
        auto flags = (UARTFR*)UARTFR_ADDRESS; 
        auto data = (UARTDR*)UARTDR_ADDRESS;
        auto control = (UARTCR *)UARTCR_ADDRESS;
        auto lineControl = (UARTLCR_H *)UARTLCR_H_ADDRESS; 
        auto intMask = (UARTIMSC *)UARTIMSC_ADDRESS;
        auto fifoInTriggerLevel = (UARTIFLS *)UARTIFLS_ADDRESS;
        bool Initialized = false;
    }

    void setBaudrate(int baudrate);

    void initInterupt();

    void init();

    void putchar(char c);

    void print(const char *s);
    void print(const char *s, size_t size);
    
    void print(int64_t val);
    void print(unsigned long ival, int base);
    void print(void* ptr);
    
    char getChar();
}


