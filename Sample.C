#include "Greenhouse.h"
#include "Greenduino.h"

class App : public Thing
{
    public:
    
    Greenduino *_greenduino;
    
    App() : Thing()
    {   
        // Pass a name for this arduino
        // replace the second string below with the serial port for your Arduino board
        // you can get this from the Arduino application or via command line
        // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
        // Or, look in the output window for a list of devices
        _greenduino = new Greenduino("ardy", "/dev/tty.usbserial-A9007MYl");
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
        //Arduino is INITIALIZED
        if (HasDescrip (p, "EInitialized"))
        {
            //Set up pin modes
            
            _greenduino -> sendDigitalPinMode(13, ARD_OUTPUT);

            _greenduino -> sendAnalogPinReporting(0, ARD_ANALOG); //Analog sensor connected to pin A0 on Arduino
            
            INFORM("firmata v " + ToStr(_greenduino->getMajorFirmwareVersion()) + "." + ToStr(_greenduino ->getMinorFirmwareVersion()));
        }
        
        //A DIGITAL pin has changed value
        if (HasDescrip (p, "EDigitalPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
            INFORM( "pin " + ToStr(pin) + " changed to " + ToStr(value) );
        }
        
        //An ANALOG pin has changed value
        if (HasDescrip (p, "EAnalogPinChanged"))
        {
            int64 pin = Ingest <int64> (p, "pin");
            int64 value = Ingest <int64> (p, "value");
            INFORM( "pin " + ToStr(pin) + " changed to " + ToStr(value) );
        }
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