#define WORKER
#include "Greenhouse.h"
#include "Greenduino.h"

void Setup ()
{ new Greenduino("ardy", "/dev/tty.usbserial-A9007MYl");
}