#include <lunchbox.h>

#define LED1 10 // LED1 is on P1.0
#define LED2 16 // LED2 is on P1.6

// This function is called once when your program starts.
void setup(void) {
  wdt_disable(); // disable the watchdog timer

  pinMode(LED1, OUTPUT); // set LED1 to an output
  pinMode(LED2, OUTPUT); // set LED2 to an output

  digitalWrite(LED1, HIGH);
}

// This function is called over and over in an infinite loop.
void loop(void) {
  if (HIGH == digitalRead(LED1)) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  }

  delaySeconds(1);
}
