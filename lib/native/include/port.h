#ifndef _PORT
#define _PORT

#include <launchpad.h>

void portInit(void (*proc)(unsigned char byte));
void portTx(unsigned char byte);

#endif
