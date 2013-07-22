
/* (c)  oblong industries */
#define WORKER
#include <Greenhouse.h>
#include "Greenduino.h"

// copied from Greenduino.h
// these better match!!!!
#define ARD_INPUT        0x00
#define ARD_OUTPUT       0x01
#define ARD_ANALOG       0x02 // analog pin in analogInput mode
#define ARD_PWM          0x03 // digital pin in PWM output mode
#define ARD_SERVO        0x04 // digital pin in Servo output mode
#define ARD_HIGH         1
#define ARD_LOW          0
#define ARD_ON           1
#define ARD_OFF          0


class Vite  :  public Greenduino
{ protected:

	Str arduinoName;
  int64 digitalPinsNum;

  public:

  Vite (Str arduino_name, Str port_path)
    :  Greenduino(),
       arduinoName (arduino_name)
    { ParticipateInPool ("from-arduino");
      ParticipateInPool ("to-arduino");
      ListenForDescrip (arduinoName);
      ListenForDescrip ("all-arduinos");
      connect (port_path . utf8());
    }

  void Metabolize (const Protein &p)
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
        if (HasDescrip (p, "set-pin-pwm"))
          { if (HasIngest (p, "pin") && HasIngest (p, "pwm"))
              { int64 pin = Ingest <int64> (p, "pin");
                int64 pwm = Ingest <bool> (p, "pwm");
                sendPwm (pin, pwm);
              }
          }
    }

  void Travail()
    { update();
      Slaw pinReadings = Slaw::List();
      for (int64 i = 0; i < digitalPinsNum; i++)
        { int64 pinMode = getDigitalPinMode (i);
          int64 pinVal = -1;
          if (pinMode == ARD_INPUT)
            pinVal = getDigital (i);
          if (pinMode == ARD_ANALOG)
            pinVal = getAnalog (i);
          pinReadings = pinReadings . ListAppend (pinVal);
        }
      Protein p = Protein (Slaw::List (arduinoName, "heartbeat"),
                           pinReadings);
      Deposit (p, "from-arduino");

    }



};