#include "Greenhouse.h"
#include "Greenduino.h"

/**
    Run this example with operaio.C (on the same machine or a different one)
    Here you will see how basic I/O with an arduino works via heartbeat proteins.
    The led connected to pin 13 can be turned on and off
    and readings will be taken from pin 0.
*/

class ArduinoController : public Thing
{
    public:
    
    /** make sure this matches the arduino_name you set for the Greenduino
        object you want to interface with
    */
    Str arduino_name = "ardy";
    /** if your arduino is connected to the same computer as this app,
        leave this string empty
        if its on a different machine, put its tcp address here
        e.g. tcp://Client-Solutions-Test-Macbook-Pro.local/
        (make sure you have a pool server running on that machine)
    */
    Str pool_path = "";
    Str output_pool_name;
    Str input_pool_name;
    Text *sensor_value;
    bool first_heartbeat_heard = false;

    ArduinoController () : Thing()
      {  output_pool_name = pool_path + "to-arduino";
         input_pool_name = pool_path + "from-arduino";
         ParticipateInPool (input_pool_name);
         ParticipateInPool (output_pool_name);
         sensor_value = new Text ("waiting");
         AppendKid (sensor_value);
      }

    void Blurt (BlurtEvent *e)
      { if (Utters (e, "e") || Utters (e, "victory"))
          SetPinState (13, true);        
        if (Utters (e, "r") )
          SetPinState (13, false);
      }
    void BlurtVanish (BlurtEvent *e)
      { if (Utters (e, "victory"))
          SetPinState (13, false);
      }
    
    void Metabolize (const Protein &p)
      { if (HasDescrip (p, arduino_name) && HasDescrip (p, "heartbeat"))
          { if (! first_heartbeat_heard)
              { // setup pins
                SetPinMode (13, "digital-output");
                SetPinMode (0, "analog-input");
                first_heartbeat_heard = true;
              }
            if (HasIngest (p, "pin-vals"))
              { Trove <int64> s = TroveFromIngest <int64> (p, "pin-vals");
                sensor_value -> SetString (ToStr (s . Nth(0)));
              }
            // set a timer that only goes off if we stop getting heartbeats
            SetFireTimer (10, 1);
          }
      }

    // we've stopped getting heartbeats,
    // so the arduino must have been disconnected
    void Fired()
      { first_heartbeat_heard = false; }

    // constructs and deposits a protein to turn a digital pin on or off
    void SetPinState (int64 num, bool state)
      { Protein p = ProteinWithDescrip (arduino_name);
        AppendDescrip (p, "set-pin-state");
        AppendIngest (p, "pin", num);
        AppendIngest (p, "state", state);
        Deposit (p, output_pool_name);
      }
    // constructs and deposits a protein to set a pin's mode
    void SetPinMode (int64 num, Str mode)
      { Protein p = ProteinWithDescrip (arduino_name);
        AppendDescrip (p, "set-pin-mode");
        AppendIngest (p, "pin", num);
        AppendIngest (p, "mode", mode);
        Deposit (p, output_pool_name);
      }
};

void Setup ()
{
    ArduinoController *ac = new ArduinoController();
    ac -> SlapOnFeld();
    
    DoNotRegisterForDefaultEvents();
    
    RegisterOneHandBlurt ("victory", VictoryPose);
}