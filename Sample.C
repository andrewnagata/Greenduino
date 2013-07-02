#include "Greenhouse.h"
#include "Greenduino.h"

class App : public Thing
{
    public:
    
    Greenduino *_greenduino;
    
    App() : Thing()
    {
        _greenduino = new Greenduino();
        
        // replace the string below with the serial port for your Arduino board
        // you can get this from the Arduino application or via command line
        // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
        // Or, look in the output window for a list of devices
        _greenduino -> connect("/dev/tty.usbmodem1d11");
        
        _greenduino -> sendDigitalPinMode(13, ARD_INPUT);
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