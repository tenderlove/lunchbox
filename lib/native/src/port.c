#include <port.h>

#ifndef HW_UART
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
#endif

received_byte callback;

void portInit(received_byte func)
{
#ifdef HW_UART
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD

#ifdef HAVE_CRYSTAL
  UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
  UCA0BR0 = 0x03;                           // 32kHz/9600 = 3.41
  UCA0BR1 = 0x00;                           //
  UCA0MCTL = UCBRS1 + UCBRS0;               // Modulation UCBRSx = 3
#else
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
#endif

  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
#else
  P1SEL = UART_TXD + UART_RXD;            // Timer function for TXD/RXD pins
  P1DIR |= ~UART_RXD;               // Set all pins but RXD to output
  TACCTL0 = OUT;                          // Set TXD Idle as Mark = '1'
  TACCTL1 = SCS + CM1 + CAP + CCIE;       // Sync, Neg Edge, Capture, Int
  TACTL = TASSEL_2 + MC_2;                // SMCLK, start in continuous mode
#endif

  callback = func;
}

//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void portTx(unsigned char byte)
{
#ifdef HW_UART
    while (!(IFG2&UCA0TXIFG));
    UCA0TXBUF = byte;
#else
    while (TACCTL0 & CCIE);                 // Ensure last char got TX'd
    TACCR0 = TAR;                           // Current state of TA counter
    TACCR0 += UART_TBIT;                    // One bit time till first bit
    TACCTL0 = OUTMOD0 + CCIE;               // Set TXD on EQU0, Int
    txData = byte;                          // Load global variable
    txData |= 0x100;                        // Add mark stop bit to TXData
    txData <<= 1;                           // Add space start bit
#endif
}

#ifdef HW_UART
__attribute__ ((__interrupt__(USCIAB0RX_VECTOR)))
void USCI0RX_ISR(void)
{
  while (!(IFG2&UCA0TXIFG));                 // USCI_A0 TX buffer ready?
  if (callback)
    callback((unsigned char)UCA0RXBUF);
}
#else
//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
__attribute__ ((__interrupt__(TIMERA0_VECTOR)))
void Timer_A0_ISR(void)
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
#endif
