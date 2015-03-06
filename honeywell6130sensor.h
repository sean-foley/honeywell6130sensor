#ifndef _HONEYWELL6130SENSOR_H_
#define _HONEYWELL6130SENSOR_H_

/*======================================================================
FILE: 
    honeywell6130sensor.h

CREATOR:
    Sean Foley
    coding at sean[removethis]foleydotcom

SERVICES:
    Provides support to read temperature and humidity data
    from a Honeywell 6130 i2c sensor.

DESCRIPTION:
    This header defines a simple implementation interface
    to retrieve temperature and humidity data.

PUBLIC CLASSES AND FUNCTIONS:
    Honeywell6130Sensor

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
// INCLUDES AND PUBLIC DATA DECLARATIONS
//======================================================================

//----------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------

#include <exception>
#include <string>

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Constant Declarations
//----------------------------------------------------------------------

// None.

//----------------------------------------------------------------------
// Global Data Declarations
//----------------------------------------------------------------------

struct TempHumidityData
{
    unsigned char status;
    float tempCelcius;
    float tempFahrenheit;
    float relativeHumidity;
};

//======================================================================
// WARNINGS!!!
//======================================================================

// None.

//======================================================================
// FUNCTION DECLARATIONS
//======================================================================

// None.

//=====================================================================
// EXCEPTION CLASS DEFINITIONS
//=====================================================================

class SensorException : public std::exception
{
public:

    // C-tors
    SensorException( std::string message ) :_message( message ) { }
    SensorException( const char* message ) :_message( message ) { }    

    virtual ~SensorException() throw() { } 
 
    virtual const char* what()
    {
        return _message.c_str();
    }

private:
    std::string _message;
};

//======================================================================
// CLASS DEFINITIONS
//======================================================================

/*======================================================================
CLASS: 
    Honeywell6130Sensor

DESCRIPTION:
    This class hides the low level bit fiddling needed to grab
    temp and humidity data from a Honeywell 6130 Sensor.

HOW TO USE:
    1. Construct the object passing the i2c device bus
    2. Call Read() to get data

======================================================================*/
class Honeywell6130Sensor
{
public:
	
    //=================================================================
    // TYPE DECLARATIONS AND CONSTANTS    
    //=================================================================

    // None.

    //=================================================================
    // CLIENT INTERFACE
    //=================================================================
    
    Honeywell6130Sensor( const char* i2cDevice, int i2cAddress = 0x27 );

    virtual ~Honeywell6130Sensor();

    TempHumidityData Read() const;

protected:

    //=================================================================
    // SUBCLASS INTERFACE   
    //=================================================================
    
    // None.

private:
    
    //=================================================================
    // CUSTOMIZATION INTERFACE    
    //=================================================================

    // None.

    //=================================================================
    // IMPLEMENTATION INTERFACE    
    //=================================================================

    // No copying. Not implementing the method
    // to force a link error if someone tries to 
    // invode a copy c-tor.
    Honeywell6130Sensor( const Honeywell6130Sensor &rhs );

    void initialize( const char* i2cDevice, int i2cAddress );

    //=================================================================
    // DATA MEMBERS    
    //=================================================================

    int _fileDescriptor;

};

//======================================================================
// INLINE FUNCTION DEFINITIONS
//======================================================================

// None.


/*======================================================================
// DOCUMENTATION
========================================================================

None.

======================================================================*/

#endif	// #ifendif _HONEYWELL6130SENSOR_H_
