#ifndef _LAUNCHPAD
#define _LAUNCHPAD

#include <msp430.h>

#define pinBit(pin) (0x01 << (pin - 10))

/* Disable the watchdog timer. */
#define wdt_disable() WDTCTL = WDTPW + WDTHOLD
#define wdt_enable() WDTCTL = WDTPW + WDTCNTCL

#define INPUT  &= ~
#define OUTPUT |=
#define pinMode(pin, dir) P1DIR dir(pinBit(pin))

#define LOW  0
#define HIGH 1

/* Write bit to pin. */
#define digitalWrite(pin, mode) { \
  if (mode) \
    P1OUT |= pinBit(pin); \
  else \
    P1OUT &= ~pinBit(pin); \
}

/* Read state of pin. */
#define digitalRead(pin) (P1IN & pinBit(pin) ? HIGH : LOW)

#define MCLK_FREQUENCY      1000000

#define delay(millis) { __delay_cycles(millis * MCLK_FREQUENCY / 1000); }
#define delayMicroseconds(micros) { __delay_cycles(micros); }
#define delaySeconds(sec) { \
  int i, j; \
  for(i = 0; i < sec; i++) { \
    for(j = 0; j < 10; j++) { \
      __delay_cycles(100000); \
    } \
  } \
}

typedef void(*interfunc)(void);

void attachInterrupt(int interrupt, void (*proc)(void));

int main(void);

#endif
