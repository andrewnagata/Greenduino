#include "Greenhouse.h"
#include "Greenduino.h"

class App : public Thing
{
    public:
    
    Greenduino *_greenduino;
    
    App() : Thing()
    {
        ParticipateInPool("arduino");
        _greenduino = new Greenduino("arduino");
        
        // replace the string below with the serial port for your Arduino board
        // you can get this from the Arduino application or via command line
        // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
        // Or, look in the output window for a list of devices
        _greenduino -> connect("/dev/tty.usbmodem1d11");
    }
    
    void Blurt (BlurtEvent *e)
    {
        if (Utters (e, "victory") )
        {
            _greenduino -> sendDigital(13, ARD_HIGH);
        }
        
        if (Utters (e, "elleshaped") )
        {
            _greenduino -> sendDigital(13, ARD_LOW);
        }
    }
    
    void Metabolize (const Protein &p)
    {
        if (HasDescrip (p, "EInitialized"))
        {
            INFORM("firmata v " + ToStr(_greenduino->getMajorFirmwareVersion()) + "." + ToStr(_greenduino ->getMinorFirmwareVersion()));
            
            _greenduino -> sendDigitalPinMode(13, ARD_OUTPUT);
            
            //Analog sensor connected to pin A0 on Arduino
            _greenduino -> sendAnalogPinReporting(0, ARD_ANALOG);
        }
        
        if (HasDescrip (p, "EDigitalPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
            INFORM( "pin " + ToStr(pin) + " changed to " + ToStr(value) );
        }
        
        if (HasDescrip (p, "EAnalogPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
            INFORM( "pin " + ToStr(pin) + " changed to " + ToStr(value) );
        }
    }
    
    void Travail()
    {
        _greenduino -> update();
    }
};

void Setup ()
{
    App *app = new App();
    
    app -> SlapOnFeld();
    
    DoNotRegisterForDefaultEvents();
    
    RegisterOneHandBlurt ("victory", VictoryPose);
    RegisterOneHandBlurt ("elleshaped", LShapePose);
}