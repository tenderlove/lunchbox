#ifndef _DHT
#define _DHT

#include <lunchbox.h>

typedef enum
{
  DHT_OK = 0,
  DHT_ERROR_NO_START_ACK,
  DHT_ERROR_ACK_TOO_LONG,
  DHT_ERROR_CHECKSUM,
} DHT_ERROR_t;

typedef struct {
  int pin;
  int pinBit;
  unsigned char rhIntegral;
  unsigned char rhDecimal;
  unsigned char tIntegral;
  unsigned char tDecimal;
  unsigned char checksum;
} dht;

void dhtInit(dht * ctx, int pin);
void dhtSetup(dht * ctx);
DHT_ERROR_t dhtRead(dht * ctx);

#endif
