#include "Greenhouse.h"
#include "Greenduino.h"

class ArduinoController : public Thing
{
    public:
    
    /** make sure this matches the arduino_name you set for the Greenduino
        you want to control
    */
    Str arduino_name = "ardy";
    /** if your arduino is connected to the same computer as this app,
        leave this string empty
        if its on a different machine, put its tcp address here
        e.g. tcp://Client-Solutions-Test-Macbook-Pro.local/
    */
    Str pool_path = "tcp://Client-Solutions-Test-Macbook-Pro.local/";
    Str output_pool_name;
    Str input_pool_name;
    Text *sensor_value;

    ArduinoController () : Thing()
    {  output_pool_name = pool_path + "to-arduino";
       input_pool_name = pool_path + "from-arduino";
       ParticipateInPool (input_pool_name);
       ParticipateInPool (output_pool_name);
       sensor_value = new Text ("waiting");
       AppendKid (sensor_value);
    }
    
    // constructs and deposits a protein to turn a digital pin on or off
    void SetPinState (int64 num, bool state)
    { Protein p = ProteinWithDescrip (arduino_name);
      AppendDescrip (p, "set-pin-state");
      AppendIngest (p, "pin", num);
      AppendIngest (p, "state", state);
      Deposit (p, output_pool_name);
    }

    void SetPinMode (int64 num, Str mode)
    { Protein p = ProteinWithDescrip (arduino_name);
      AppendDescrip (p, "set-pin-mode");
      AppendIngest (p, "pin", num);
      AppendIngest (p, "mode", mode);
      Deposit (p, pool_path + "to-arduino");
    }

    void Blurt (BlurtEvent *e)
    {
        if (Utters (e, "victory") )
          SetPinState (13, true);
        
        
        if (Utters (e, "elleshaped") )
          SetPinState (13, false);
    }
    
    void Metabolize (const Protein &p)
    {
        //Arduino is INITIALIZED
        if (HasDescrip (p, "EInitialized"))
        {
            //Set up pin modes
            SetPinMode (13, "digital-output");
            SetPinMode (0, "analog-input");
        }
        
        //A DIGITAL pin has changed value
        if (HasDescrip (p, "EDigitalPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
        }
        
        //An ANALOG pin has changed value
        if (HasDescrip (p, "EAnalogPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
        }

        if (HasDescrip (p, arduino_name) && HasDescrip (p, "heartbeat"))
        { if (HasIngest (p, "pin-vals"))
            { Trove <int64> s = TroveFromIngest <int64> (p, "pin-vals");
              sensor_value -> SetString (ToStr (s . Nth(0)));
            }

        }
    }
};

void Setup ()
{
    ArduinoController *ac = new ArduinoController();
    ac -> SlapOnFeld();
    
    DoNotRegisterForDefaultEvents();
    
    RegisterOneHandBlurt ("victory", VictoryPose);
    RegisterOneHandBlurt ("elleshaped", LShapePose);
}