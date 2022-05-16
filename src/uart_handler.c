// std includes
#include <stdio.h>
#include <stdlib.h>

// kernel
// #include "AuK_v1_1_7.h"

// custom includes
#include "defs.h"
#include "instruction_handler.h"
#include "msgQ.h"

// The following procedure is used to transmit a byte of data:
// 1. Configure the clock input and baud rate as detailed in Section 3.0 “Clocking and Baud
// Rate Configuration”.
// 2. Configure the data width and parity by writing a selection to the MOD<3:0> bits.
// 3. Configure the polarity, Stop bit duration and flow control.
// 4. Configure the TX interrupt watermark using the UTXISEL<2:0> bits (UxSTAH<14:12>).
// 5. Configure the address detect if needed as detailed in Section 4.5 “Address Detect”.
// 6. Set the UARTEN bit (UxMODE<15>).
// 7. Set the UTXEN bit (UxMODE<5>).
// 8. Write the data byte value to the UxTXREG register.
// A TX interrupt will be generated according to the UTXISEL<2:0> bits’ interrupt watermark setting.
// The UTXISELx bits can be configured to generate a TX interrupt when the buffer has 1 to 8 empty
// slots.
// The UARTx Transmit Buffer (UxTXREG) has two associated flags to indicate its contents. The
// TX Buffer Empty Status bit, UTXBE (UxSTAH<5>), indicates that the buffer is empty, and the TX
// Buffer Full Status bit, UTXBF (UxSTAH<4>), indicates that there are no empty slots in the buffer
// and it should not be written. 

void uart_handler()
{
    int period = 10;
    int awake;

    FOREVER{
        awake = clock() + period;

        // do stuff

        delay_until(awake);
    }
}