#include "uart.hpp"

namespace peripherals::uart {

    void setBaudrate(int baudrate) {
        uint32_t div = 4 * UART_CLOCK / baudrate;

        UARTFBRD = div & 0x3f;
        UARTIBRD = (div >> 6) & 0xffff;
    }

    void initInterupt() {
        intMask->RXIM = true;

        GIC::configIrq(UART_IRQ, GIC::trigger::edge);
        GIC::setPriority(UART_IRQ, 0);
        GIC::setTarget(UART_IRQ, GIC::cpuTarget::interface0);
        GIC::clearPending(UART_IRQ);
        GIC::enableInt(UART_IRQ);        
    }

    void init() {
        if(Initialized) return;

        control->UARTEN = false;    // turn uart off
        while(flags->BUSY);         // wait until not busy anymore
        lineControl->FEN = false;   // flush FIFO by turning it off

        // contorl is now ready to be changed
        
        // setBaudrate();  not used because default hopefully works

        lineControl->SPS = false;
        lineControl->WLEN = 0b11;   // 8 bit word length
        lineControl->FEN = true;
        lineControl->STP2 = true;    // 2 stop bits;
        lineControl->EPS = false;
        lineControl->PEN = false;
        lineControl->BRK = false;

        //interrupt setting
        //initInterupt();

        UARTDMACR = 0x00;   // deactivate dma

        control->TXE = true;
        control->RXE = true;

        control->UARTEN = true;

        Initialized = true ;
    }

    void putchar(char c) {
        //if(notInitialized) init();
        while(flags->TXFF);         // wait until FIFO is not full 
        data->DATA = c;
    }

    char getChar() {
        //if(notInitialized) init();
        while(flags->RXFE);      // wait until there is something to read
        char c = data->DATA;
        putchar(c);
        return c;
    }

    void print(const char *s) {
        for(;*s != '\0'; s++)
            putchar(*s);            //while(flags->BUSY);         // wait until transmission is done
        // checking for busy could be problematic. don't know if busy is set if something is incoming
        // checking if the fifo is empty may be better
        while(!(flags->TXFE));         // wait until transmission is done
    }

    void print(const char *s, size_t size) {
        for(; size != 0; size--, s++) 
            putchar(*s);
        //while(flags->BUSY);         // wait until transmission is done
        // checking for busy could be problematic. don't know if busy is set if something is incoming
        // checking if the fifo is empty may be better
        while(!(flags->TXFE));         // wait until transmission is done
    }
    
    void print(int64_t val) {
        if(val < 0) {
            putchar('-');
            val = val * -1;
        }
        if(val ==0) {
            putchar('0');
            return;
        }

        int arr[25];
    
        int i;
        for(i = 0; val != 0;) {
    
            arr[i] = (val % 10) + 0x30;
            i++;
            val = val / 10;
        }
    
        for (int j = i - 1; j > -1; j--) {
            putchar(arr[j]);
        }
    }

    void print(unsigned long ival, int base) {
        unsigned long div;
        char digit;
    
        if (base == 8)
            putchar((char)'0');         // oktale Zahlen erhalten eine fuehrende Null
        else if (base == 16) {
            putchar((char)'0');         // hexadezimale Zahlen ein "0x"
            putchar((char)'x');
        }

        // Bestimmung der groessten Potenz der gewaehlten Zahlenbasis, die
        // noch kleiner als die darzustellende Zahl ist.
        for (div = 1; ival/div >= (unsigned long) base; div *= base);

        // ziffernweise Ausgabe der Zahl
        for (; div > 0; div /= (unsigned long) base) {
            digit = ival / div;
            if (digit < 10)
                putchar((char)'0' + digit);
            else
                putchar((char)'a' + digit - 10);
            ival %= div;
        }
    }

    void print(void* ptr) {
        print((unsigned long)ptr, 16);
    }
       
}
