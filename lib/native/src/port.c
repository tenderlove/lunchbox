#include <port.h>

//------------------------------------------------------------------------------
// Hardware-related definitions
//------------------------------------------------------------------------------
#define UART_TXD   0x02                     // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD   0x04                     // RXD on P1.2 (Timer0_A.CCI1A)

//------------------------------------------------------------------------------
// Conditions for 9600 Baud SW UART, SMCLK = 1MHz
//------------------------------------------------------------------------------
#define UART_TBIT_DIV_2     (1000000 / (9600 * 2))
#define UART_TBIT           (1000000 / 9600)

//------------------------------------------------------------------------------
// Global variables used for full-duplex UART communication
//------------------------------------------------------------------------------
unsigned int txData;                        // UART internal variable for TX
unsigned char rxBuffer;                     // Received UART character

//------------------------------------------------------------------------------
// Function configures Timer_A for full-duplex UART operation
//------------------------------------------------------------------------------
void portInit(void)
{
  P1SEL = UART_TXD + UART_RXD;            // Timer function for TXD/RXD pins
  P1DIR |= ~UART_RXD;               // Set all pins but RXD to output
  TACCTL0 = OUT;                          // Set TXD Idle as Mark = '1'
  TACCTL1 = SCS + CM1 + CAP + CCIE;       // Sync, Neg Edge, Capture, Int
  TACTL = TASSEL_2 + MC_2;                // SMCLK, start in continuous mode
}

//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void portTx(unsigned char byte)
{
    while (TACCTL0 & CCIE);                 // Ensure last char got TX'd
    TACCR0 = TAR;                           // Current state of TA counter
    TACCR0 += UART_TBIT;                    // One bit time till first bit
    TACCTL0 = OUTMOD0 + CCIE;               // Set TXD on EQU0, Int
    txData = byte;                          // Load global variable
    txData |= 0x100;                        // Add mark stop bit to TXData
    txData <<= 1;                           // Add space start bit
}

//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
interrupt(TIMERA0_VECTOR) Timer_A0_ISR(void)
{
  static unsigned char txBitCnt = 10;

  TACCR0 += UART_TBIT;                    // Add Offset to CCRx
  if (txBitCnt == 0) {                    // All bits TXed?
    TACCTL0 &= ~CCIE;                   // All bits TXed, disable interrupt
    txBitCnt = 10;                      // Re-load bit counter
  }
  else {
    if (txData & 0x01) {
      TACCTL0 &= ~OUTMOD2;              // TX Mark '1'
    }
    else {
      TACCTL0 |= OUTMOD2;               // TX Space '0'
    }
    txData >>= 1;
    txBitCnt--;
  }
}
