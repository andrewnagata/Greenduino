#define WORKER
#include "Greenhouse.h"
#include "Greenduino.h"

/**
    run this app on the computer your arduino is connected to
    make sure the arduino name ("ardy" in this case) is the same
    as the one defined in stethoscope.C

    if this is a different machine than the one running Sample2,
    you must run /opt/oblong/greenhouse/bin/pool-tcp-server from the
    command line before running this app
*/

void Setup ()
{ new Greenduino("ardy", "/dev/tty.usbserial-A9007MYl");
}