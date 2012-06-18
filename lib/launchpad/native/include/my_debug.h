#ifndef _MYDEBUG
#define _MYDEBUG

void blinkRed(void) {
  digitalWrite(LED1, HIGH);
  delaySeconds(1);
  digitalWrite(LED1, LOW);
  delaySeconds(1);
}

void blinkGreen(void) {
  digitalWrite(LED2, HIGH);
  delaySeconds(1);
  digitalWrite(LED2, LOW);
  delaySeconds(1);
}

#endif
