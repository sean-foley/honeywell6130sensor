//======================================================================
// OBJECT FILE COPYRIGHT NOTICE
//======================================================================

static const char copyright[] =
"Copyright (C) 2014 Sean Foley, All rights reserved.";

/*======================================================================
FILE: 
    Honeywell6130Sensor.cpp

CREATOR:
    Sean Foley
    coding at sean[removethis]foleydotcom

SERVICES: 
    Methods to read data from the Honeywell 6130 Sensor

GENERAL DESCRIPTION:
    This file has methods to initialize and read data from
    the temp/humidity sensor
   
PUBLIC CLASSES AND FUNCTIONS:	
    Honeywell6130Sensor
	   
INITIALIZATION AND SEQUENCING REQUIREMENTS:
    Make sure the sensor is connected to the i2c bus

Copyright (C) 2014 Sean Foley  All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Give back and help someone else out.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

======================================================================*/

//======================================================================
// INCLUDES AND VARIABLE DEFINITIONS
//======================================================================

//----------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

#include "honeywell6130sensor.h"

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Data Definitions
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Static Variable Definitions 
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Required Libraries
//----------------------------------------------------------------------

// None. (Where supported these should be in the form
// of C++ pragmas).

//======================================================================
// FUNCTION IMPLEMENTATIONS
//======================================================================

/*======================================================================
FUNCTION: 
    Honeywell6130Sensor()	

DESCRIPTION:
    This c-tor will attempt to establish an open file handle to
    the i2c device

RETURN VALUE:
    none.

SIDE EFFECTS:
    none

======================================================================*/
Honeywell6130Sensor::Honeywell6130Sensor( const char* i2cDevice, int i2cAddress )
: _fileDescriptor( 0 )
{
    initialize( i2cDevice, i2cAddress );
}

/*======================================================================
FUNCTION: 
    ~Honeywell6130Sensor()	

DESCRIPTION:
    This destructor will attempt to close any open
    file descriptors

RETURN VALUE:
    none.

SIDE EFFECTS:
    none

======================================================================*/
Honeywell6130Sensor::~Honeywell6130Sensor()
{
    if ( 0 < _fileDescriptor ) 
    {
        close( _fileDescriptor );
    }
}

/*======================================================================
FUNCTION: 
    initialize()	

DESCRIPTION:
    This method will initialize the object by setting up a
    file descriptor and grabbing control of the device
    on the i2c bus

RETURN VALUE:
    none.

SIDE EFFECTS:
    none

======================================================================*/
void Honeywell6130Sensor::initialize( const char* i2cDevice, int i2cAddress )
{
    if ( ( _fileDescriptor = open( i2cDevice, O_RDWR ) ) < 0 ) 
    {   
        throw SensorException( "Failed to open i2cbus" );
    }

    if ( ioctl( _fileDescriptor, I2C_SLAVE, i2cAddress ) < 0 )  
    {
        throw SensorException( "Failed to get i/o control to the i2c bus or the device" );
    }
} 

/*======================================================================
FUNCTION: 
    Read()	

DESCRIPTION:
    This method will read the temp/humidity data from the
    device.  
 
RETURN VALUE:
    TempHumidityData

SIDE EFFECTS:
    none

======================================================================*/
TempHumidityData Honeywell6130Sensor::Read() const
{
     unsigned char command[ 1 ] = { 0 };

     if( write( _fileDescriptor, command, 1 ) != 1 ) 
     {
        throw SensorException ( "Sending the measurement command failed" );        
     }

     // The device needs some settle time
     // before reading data from it
     usleep( 1000 );
     
     const int BUFFER_SIZE = 4;

     char buffer[ BUFFER_SIZE ] = { 0 };
   
     if ( read( _fileDescriptor, buffer, BUFFER_SIZE ) != BUFFER_SIZE ) 
     {
         throw SensorException ( "Failed to read the expected number of bytes from the i2c device" );
     }

     // We can just operate against the buffer, but we are going to be
     // more explicit to help teach any one new to programming what is 
     // going on.  So we are going to create some variables that match
     // the data values for the bit definitions the sensor puts out,
     // then we are going to do some shifting and masking to break
     // apart and then glue the bits back together to get the final
     // values.
     TempHumidityData data;

     unsigned char humidityHi, humidityLo, tempHi, tempLo;

     humidityHi = buffer[ 0 ];
     humidityLo = buffer[ 1 ];
     tempHi     = buffer[ 2 ];
     tempLo     = buffer[ 3 ];

     data.status = ( humidityHi >> 6 ) & 0x03;
    
     humidityHi &= 0x3f;

     unsigned int humidity, temperature = 0;

     humidity    = ( ( unsigned int ) humidityHi << 8 ) | humidityLo;
     temperature = ( ( unsigned int ) tempHi     << 8 ) | tempLo;

     // The bottom two bits from the tempLo are not used
     // so we need to shift everything over or our number
     // will be 4x too big
     temperature = temperature >> 2;

     // The tempurature and humidity values are counts.  We
     // now have to apply the formula Honeywell provides to 
     // convert to actual values

     // RH% = Humidity Output Count X 100%
     //       ---------------------
     //           ( 2^14 - 1 )
     data.relativeHumidity = ( float ) ( humidity / (16384.0 - 1) * 100 );

     // Temp (C) = Temperature Output Count X 165 - 40
     //            ------------------------
     //              ( 2^14 - 1 )
     data.tempCelcius = ( float ) ( temperature * ( 165.0 / (16384.0 - 1) ) ) - 40;

     // Finally, do a helper for fahrenheit
     data.tempFahrenheit = data.tempCelcius * 1.8 + 32;      

     return data;
} 

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================
 
Many thanks to Peter Anderson for posting the data sheet on his site. 
http://www.phanderson.com/arduino/I2CCommunications.pdf 
 
 
      data byte 1           |        data byte 2      |        data byte 3          |   data byte 4
S1|S0|B13|B12|B11|B10|B9|B8 | B7|B6|B5|B4|B3|B2|B1|B0 | T13|T12|T11|T10|T9|T8|T7|T6 | T5|T4|T3|T2|T1|T0|X|X 
Status     Humidity         |        Humidity         |        Temperature          |     Temmpurature      
 
Status bits (high order bits on data byte 1)
S1 SO 
0  0  normal operation 
0  1  stale data. data has already been fetched since last measurement 
1  0  device in command mode 
1  1  diagnostic condition 
 
data byte 4 
low order bits b0..b1 (the X|X above) are not used. shift them out. 

=====================================================================*/

