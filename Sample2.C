#include "Greenhouse.h"
#include "Greenduino.h"

class ArduinoInterface : public Thing
{
    public:
    
    Str pool_path = "tcp://Client-Solutions-Test-Macbook-Pro.local/";

    ArduinoInterface () : Thing()
    {  ParticipateInPool (pool_path + "from-arduino");
       ParticipateInPool (pool_path + "to-arduino");
    }
    
    void Blurt (BlurtEvent *e)
    {
        if (Utters (e, "victory") )
        { Protein p = ProteinWithDescrip ("ardy");
          AppendDescrip (p, "set-pin-state");
          AppendIngest (p, "pin", (int64)13);
          AppendIngest (p, "state", true);
          Deposit (p, pool_path + "to-arduino");
        }
        
        if (Utters (e, "elleshaped") )
        { Protein p = ProteinWithDescrip ("ardy");
          AppendDescrip (p, "set-pin-state");
          AppendIngest (p, "pin", (int64)13);
          AppendIngest (p, "state", false);
          Deposit (p, pool_path + "to-arduino");
        }
    }
    
    void Metabolize (const Protein &p)
    {
        //Arduino is INITIALIZED
        if (HasDescrip (p, "EInitialized"))
        {
            //Set up pin modes
            Protein p = ProteinWithDescrip("set-pin-mode");
            AppendDescrip (p, "ardy");
            AppendIngest (p, "pin", (int64)13);
            AppendIngest (p, "mode", "digital-output");
            Deposit (p, pool_path + "to-arduino");            

            Protein p1 = ProteinWithDescrip("set-pin-mode");
            AppendDescrip (p1, "ardy");
            AppendIngest (p1, "pin", (int64)0);
            AppendIngest (p1, "mode", "analog-input");
            Deposit (p1, pool_path + "to-arduino");
            
            Protein p2 = ProteinWithDescrip("set-pin-mode");
            AppendDescrip (p2, "ardy");
            AppendIngest (p2, "pin", (int64)2);
            AppendIngest (p2, "mode", "digital-input");
            Deposit (p2, pool_path + "to-arduino");
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
    }
};

void Setup ()
{
    ArduinoInterface *ai = new ArduinoInterface();
    
    ai -> SlapOnFeld();
    
    DoNotRegisterForDefaultEvents();
    
    RegisterOneHandBlurt ("victory", VictoryPose);
    RegisterOneHandBlurt ("elleshaped", LShapePose);
}