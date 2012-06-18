#include <lunchbox.h>

void setup(void);
void loop(void);

int main(void) {
  P1OUT = 0x00;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  setup();
  for (;;) {
    loop();
  }
}
