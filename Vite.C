
/* (c)  oblong industries */
#define WORKER
#include <Greenhouse.h>
#include "Greenduino.h"

// copied from Greenduino.h
// these better match!!!!
#define ARD_INPUT                                       0x00
#define ARD_OUTPUT                                      0x01
#define ARD_ANALOG                                      0x02 // analog pin in analogInput mode
#define ARD_PWM                                         0x03 // digital pin in PWM output mode
#define ARD_SERVO                                       0x04 // digital pin in Servo output mode
#define ARD_HIGH                                        1
#define ARD_LOW                                         0
#define ARD_ON                                          1
#define ARD_OFF                                         0


class Vite  :  public Greenduino
{ protected:

	Str prov;

  public:

  Vite()
    { ParticipateInPool ("arduino");
    }

  void Metabolize (const Protein &p)
    { if (HasDescrip (p, "to-arduino"))
        { if (HasDescrip (p, "set-pin-mode"))
           { if (HasIngest (p, "pin") && HasIngest (p, "mode"))
              { int64 pin = Ingest <int64> (p, "pin");
                Str mode = Ingest <Str> (p, "mode");
                if (mode == "input")
                  sendDigitalPinMode(pin, ARD_INPUT);
                else if (mode == "output")
                  sendDigitalPinMode(pin, ARD_OUTPUT);
                else if (mode == "pwm")
                  sendDigitalPinMode(pin, ARD_PWM);
              }
            } 
        if (HasDescrip (p, "set-pin-state"))
          { if (HasIngest (p, "pin") && HasIngest (p, "state"))
              { int64 pin = Ingest <int64> (p, "pin");
                bool pinState = Ingest <bool> (p, "state");
                sendDigital (pin, pinState);
              }
          }
    }
  }

};