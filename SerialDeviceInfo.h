#pragma once
//
//  SerialDeviceInfo.h
//

#ifndef SerialDeviceInfo_h
#define SerialDeviceInfo_h


#endif

#include <string.h>

using namespace std;

//----------------------------------------------------------
// DeviceInfo
//----------------------------------------------------------
class GSerial;
class SerialDeviceInfo{
	friend class GSerial;
    
public:
    
    SerialDeviceInfo(string devicePathIn, string deviceNameIn, int deviceIDIn)
    {
        devicePath			= devicePathIn;
        deviceName			= deviceNameIn;
        deviceID			= deviceIDIn;
    }
    
    SerialDeviceInfo(){
        deviceName = "device undefined";
        deviceID   = -1;
    }
    
    string getDevicePath(){
        return devicePath;
    }
    
    string getDeviceName(){
        return deviceName;
    }
    
    int getDeviceID(){
        return deviceID;
    }
    
protected:
    string devicePath;			//eg: /dev/tty.cu/usbdevice-a440
    string deviceName;			//eg: usbdevice-a440 / COM4
    int deviceID;				//eg: 0,1,2,3 etc
    
    //TODO: other stuff for serial ?
};
