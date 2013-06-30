//
//  GSerial.cpp
//  PoolsnSuch
//
//  Created by Andrew Nagata on 6/28/13.
//
//

#include "GSerial.h"
#include <sys/ioctl.h>
#include <getopt.h>
#include <dirent.h>
#include <fcntl.h>

// serial error codes
#define OF_SERIAL_NO_DATA 	-2
#define OF_SERIAL_ERROR		-1

using namespace std;

//----------------------------------------------------------------
GSerial::GSerial()
{
	bInited = false;
}

//----------------------------------------------------------------
GSerial::~GSerial()
{
	close();
    
    bInited = false;
}

//----------------------------------------------------------------
static bool isDeviceArduino( SerialDeviceInfo & A ){
	
	return ( strstr(A.getDeviceName().c_str(), "usbserial") != NULL ||
            strstr(A.getDeviceName().c_str(), "usbmodem") != NULL );
}

void GSerial::buildDeviceList()
{
    string deviceType = "serial";
    
    vector<string> prefixMatch;
    
    prefixMatch.push_back("cu.");
    prefixMatch.push_back("tty.");
    
    DIR *dir;
    struct dirent *entry;
    dir = opendir("/dev");
    
    string deviceName	= "";
    int deviceCount		= 0;
    
    if (dir == NULL){
        cout << "GSerial: error listing devices in /dev" << endl;
    } else {
        //for each device
        while((entry = readdir(dir)) != NULL){
            deviceName = (char *)entry->d_name;
            
            //we go through the prefixes
            for(int k = 0; k < (int)prefixMatch.size(); k++){
                //if the device name is longer than the prefix
                if( deviceName.size() > prefixMatch[k].size() ){
                    //do they match ?
                    if( deviceName.substr(0, prefixMatch[k].size()) == prefixMatch[k].c_str() )
                    {
                        devices.push_back(SerialDeviceInfo("/dev/"+deviceName, deviceName, deviceCount));
                        deviceCount++;
                        break;
                    }
                }
            }
        }
        closedir(dir);
    }
    
    //here we sort the device to have the aruino ones first.
    partition(devices.begin(), devices.end(), isDeviceArduino);
    //we are reordering the device ids. too!
    for(int k = 0; k < (int)devices.size(); k++)
    {
        devices[k].deviceID = k;
    }
    
    bHaveEnumeratedDevices = true;
}

//----------------------------------------------------------------
void GSerial::listDevices(){
	buildDeviceList();
	for(int k = 0; k < (int)devices.size(); k++)
    {
		cout << "[" << devices[k].getDeviceID() << "] = "<< devices[k].getDeviceName().c_str() << endl;
	}
}

//----------------------------------------------------------------
vector <SerialDeviceInfo> GSerial::getDeviceList(){
	buildDeviceList();
	return devices;
}

//----------------------------------------------------------------
void GSerial::close(){
    
    //---------------------------------------------
    if (bInited){
        tcsetattr(fd,TCSANOW,&oldoptions);
        ::close(fd);
    }
    // [CHECK] -- anything else need to be reset?
    //---------------------------------------------
}

//----------------------------------------------------------------
bool GSerial::setup(){
	return setup(0,9600);		// the first one, at 9600 is a good choice...
}

//----------------------------------------------------------------
bool GSerial::setup(int deviceNumber, int baud){
    
	buildDeviceList();
	if( deviceNumber < (int)devices.size() ){
		return setup(devices[deviceNumber].devicePath, baud);
	}else{
		cout << "GSerial: could not find device " << deviceNumber << "- only " << devices.size() << " devices found";
		return false;
	}
}

//----------------------------------------------------------------
bool GSerial::setup(string portName, int baud){
    
	bInited = false;
	
    //lets account for the name being passed in instead of the device path
    if( portName.size() > 5 && portName.substr(0, 5) != "/dev/" ){
        portName = "/dev/" + portName;
    }
    
    cout << "GSerialInit: opening port " << portName.c_str() << " " << baud << " bps";
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd == -1){
        //ofLog(OF_LOG_ERROR,"ofSerial: unable to open port %s", portName.c_str());
        return false;
    }
    
    struct termios options;
    tcgetattr(fd,&oldoptions);
    options = oldoptions;
    switch(baud){
        case 300: 	cfsetispeed(&options,B300);
            cfsetospeed(&options,B300);
            break;
        case 1200: 	cfsetispeed(&options,B1200);
            cfsetospeed(&options,B1200);
            break;
        case 2400: 	cfsetispeed(&options,B2400);
            cfsetospeed(&options,B2400);
            break;
        case 4800: 	cfsetispeed(&options,B4800);
            cfsetospeed(&options,B4800);
            break;
        case 9600: 	cfsetispeed(&options,B9600);
            cfsetospeed(&options,B9600);
            break;
        case 14400: 	cfsetispeed(&options,B14400);
            cfsetospeed(&options,B14400);
            break;
        case 19200: 	cfsetispeed(&options,B19200);
            cfsetospeed(&options,B19200);
            break;
        case 28800: 	cfsetispeed(&options,B28800);
            cfsetospeed(&options,B28800);
            break;
        case 38400: 	cfsetispeed(&options,B38400);
            cfsetospeed(&options,B38400);
            break;
        case 57600:  cfsetispeed(&options,B57600);
            cfsetospeed(&options,B57600);
            break;
        case 115200: cfsetispeed(&options,B115200);
            cfsetospeed(&options,B115200);
            break;
        case 230400: cfsetispeed(&options,B230400);
            cfsetospeed(&options,B230400);
            break;
            
        default:	cfsetispeed(&options,B9600);
            cfsetospeed(&options,B9600);
            //ofLog(OF_LOG_ERROR,"ofSerialInit: cannot set %i baud setting baud to 9600", baud);
            break;
    }
    
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(fd,TCSANOW,&options);
    
    bInited = true;
    //ofLog(OF_LOG_NOTICE,"success in opening serial connection");

    return true;
}


//----------------------------------------------------------------
int GSerial::writeBytes(unsigned char * buffer, int length){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

    int numWritten = write(fd, buffer, length);
    if(numWritten <= 0){
        if ( errno == EAGAIN )
            return 0;
        //ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port, errno %i (%s)", errno, strerror(errno));
        return OF_SERIAL_ERROR;
    }
    
    //ofLog(OF_LOG_VERBOSE,"ofSerial: numWritten %i", numWritten);
    
    return numWritten;
}

//----------------------------------------------------------------
int GSerial::readBytes(unsigned char * buffer, int length){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}

    int nRead = read(fd, buffer, length);
    if(nRead < 0){
        if ( errno == EAGAIN )
            return OF_SERIAL_NO_DATA;
        //ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port, errno %i (%s)", errno, strerror(errno));
        return OF_SERIAL_ERROR;
    }
    return nRead;
}

//----------------------------------------------------------------
bool GSerial::writeByte(unsigned char singleByte){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		//return OF_SERIAL_ERROR; // this looks wrong.
		return false;
	}
    
	unsigned char tmpByte[1];
	tmpByte[0] = singleByte;
    
    int numWritten = 0;
    numWritten = write(fd, tmpByte, 1);
    if(numWritten <= 0 ){
        if ( errno == EAGAIN )
            return 0;
        //ofLog(OF_LOG_ERROR,"ofSerial: Can't write to com port, errno %i (%s)", errno, strerror(errno));
        return OF_SERIAL_ERROR;
    }
    //ofLog(OF_LOG_VERBOSE,"ofSerial: written byte");
    
    
    return (numWritten > 0 ? true : false);
}

//----------------------------------------------------------------
int GSerial::readByte(){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}
    
	unsigned char tmpByte[1];
	memset(tmpByte, 0, 1);
    
    int nRead = read(fd, tmpByte, 1);
    if(nRead < 0){
        if ( errno == EAGAIN )
            return OF_SERIAL_NO_DATA;
        //ofLog(OF_LOG_ERROR,"ofSerial: trouble reading from port, errno %i (%s)", errno, strerror(errno));
        return OF_SERIAL_ERROR;
    }
    if(nRead == 0)
        return OF_SERIAL_NO_DATA;
    
	return (int)(tmpByte[0]);
}


//----------------------------------------------------------------
void GSerial::flush(bool flushIn, bool flushOut){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return;
	}
    
	int flushType = 0;

    if( flushIn && flushOut) flushType = TCIOFLUSH;
    else if(flushIn) flushType = TCIFLUSH;
    else if(flushOut) flushType = TCOFLUSH;
    else return;
    
    tcflush(fd, flushType);
}

void GSerial::drain(){
    if (!bInited){
        //ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
        return;
    }

    tcdrain( fd );
}

//-------------------------------------------------------------
int GSerial::available(){
    
	if (!bInited){
		//ofLog(OF_LOG_ERROR,"ofSerial: serial not inited");
		return OF_SERIAL_ERROR;
	}
    
	int numBytes = 0;
    
    ioctl(fd,FIONREAD,&numBytes);
    
	return numBytes;
}

