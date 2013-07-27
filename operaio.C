#define WORKER
#include "Greenhouse.h"
#include "Greenduino.h"

/**
    run this app on the computer your arduino is connected to to interface
    with stethoscope.C

    if this is a different machine than the one running Sample2,
    you must run /opt/oblong/greenhouse/bin/pool-tcp-server from the
    command line before running this app

    make sure the arduino name ("ardy" in this case) is the same
    as the one defined in stethoscope.C

    replace the second string below with the serial port for your Arduino board
    you can get this from the Arduino application or via command line
    for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
    Or, look in the output window for a list of devices
*/

void Setup ()
{ new Greenduino("ardy", "/dev/tty.usbserial-A9007MYl");
}