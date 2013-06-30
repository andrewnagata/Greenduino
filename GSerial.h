#pragma once
//
//  GSerial.h
//  PoolsnSuch
//
//  Created by Andrew Nagata on 6/28/13.
//
//

#ifndef __PoolsnSuch__GSerial__
#define __PoolsnSuch__GSerial__

#include <iostream>
#include <termios.h>
#include <string.h>
#include <vector.h>

#include "SerialDeviceinfo.h"

#endif /* defined(__PoolsnSuch__GSerial__) */

using namespace std;

class GSerial
{
    public:

        GSerial();
        virtual ~GSerial();
    
        void			listDevices();
        vector          <SerialDeviceInfo> getDeviceList();
    
        void 			close();
        bool			setup();	// use default port, baud (0,9600)
        bool			setup(string portName, int baudrate);
        bool			setup(int deviceNumber, int baudrate);
    
    
        int 			readBytes(unsigned char * buffer, int length);
        int 			writeBytes(unsigned char * buffer, int length);
        bool			writeByte(unsigned char singleByte);
        int             readByte();  // returns -1 on no read or error...
        void			flush(bool flushIn = true, bool flushOut = true);
        int				available();
    
        void            drain();

    
    protected:
    
        void			buildDeviceList();
    
        string          deviceType;
        vector          <SerialDeviceInfo> devices;
    
        bool            bHaveEnumeratedDevices;
    
        bool            bInited;
    
        int             fd;			// the handle to the serial port mac
        struct          termios oldoptions;
};