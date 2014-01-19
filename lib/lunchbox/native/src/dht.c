#include <lunchbox/dht.h>

#define DHT_INPUT(bit) P1DIR &= ~(bit)
#define DHT_OUTPUT(bit) P1DIR |= bit

#define DIGITAL_READ(bit) (P1IN & bit ? HIGH : LOW)

#define DHT_PACKET_LENGTH 40

inline static int cycleRead(int pinBit, int direction) {
  int count = 0;

  do {
    count++;
  } while(direction == DIGITAL_READ(pinBit));

  return count;
}

void dhtInit(dht * ctx, int pin) {
  ctx->pin = pin;
  ctx->pinBit = pinBit(pin);
}

void dhtSetup(dht * ctx) {
  DHT_OUTPUT(ctx->pinBit);
  digitalWrite(ctx->pin, HIGH);
}

void startPulse(dht * ctx) {
  DHT_OUTPUT(ctx->pinBit);
  digitalWrite(ctx->pin, LOW); /* send start. */
  delayMicroseconds(1100);
  digitalWrite(ctx->pin, HIGH);
  delayMicroseconds(20);
}

DHT_ERROR_t findAck(dht * ctx) {
  int cycles;

  DHT_INPUT(ctx->pinBit);

  // Find the start of the ACK Pulse
  cycles = cycleRead(ctx->pinBit, LOW);
  if (cycles > 15)
    return DHT_ERROR_NO_START_ACK;

  // Find the end of the ACK Pulse
  cycles = cycleRead(ctx->pinBit, HIGH);
  if (cycles > 15)
    return DHT_ERROR_ACK_TOO_LONG;

  return DHT_OK;
}

#define copyBits(s, f, v) { \
  for(i = s; i < f; i++) { \
    v <<= 1; \
    if(deltas[i] > 3) \
      v |= 0x1; \
  } \
}

DHT_ERROR_t readBits(dht * ctx) {
  int i;
  int deltas[40];
  unsigned char rhIntegral = 0;
  unsigned char rhDecimal = 0;
  unsigned char tIntegral = 0;
  unsigned char tDecimal = 0;
  unsigned char checksum = 0;

  for(i = 0; i < 40; i++) {
    cycleRead(ctx->pinBit, LOW);

    /* Time the data bit */
    deltas[i] = cycleRead(ctx->pinBit, HIGH);
  }

  copyBits(0,  8,  rhIntegral);
  copyBits(8,  16, rhDecimal);
  copyBits(16, 24, tIntegral);
  copyBits(24, 32, tDecimal);
  copyBits(32, 40, checksum);

  if(checksum == ((tIntegral + tDecimal + rhIntegral + rhDecimal) & 0xFF)) {
    ctx->rhIntegral = rhIntegral;
    ctx->rhDecimal  = rhDecimal;
    ctx->tIntegral  = tIntegral;
    ctx->tDecimal   = tDecimal;
    return DHT_OK;
  }

  return DHT_ERROR_CHECKSUM;
}

DHT_ERROR_t dhtRead(dht * ctx) {
  startPulse(ctx);
  findAck(ctx);
  return readBits(ctx);
}
