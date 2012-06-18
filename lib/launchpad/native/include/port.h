#ifndef _PORT
#define _PORT

#include <launchpad.h>

typedef void(*received_byte)(unsigned char byte);

void portInit(received_byte func);
void portTx(unsigned char byte);

#endif
