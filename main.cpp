//======================================================================
// OBJECT FILE COPYRIGHT NOTICE
//======================================================================

static const char copyright[] =
"Copyright (C) 2014 Sean Foley, All rights reserved.";

/*======================================================================
FILE: 
    main.cpp

CREATOR:
    Sean Foley
    coding at sean[removethis]foleydotcom

SERVICES: 
    Program entry point

GENERAL DESCRIPTION:
    This is a harness to drive the honeywell 6130 sensor class
   
PUBLIC CLASSES AND FUNCTIONS:	
    main
	   
INITIALIZATION AND SEQUENCING REQUIREMENTS:
    Any special requirements.

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

#include <iostream>
#include <unistd.h>

//----------------------------------------------------------------------
// Type Declarations
//----------------------------------------------------------------------

using namespace std;

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
    main()	

DESCRIPTION:
    Program entrypoint

RETURN VALUE:
    int (not used)

SIDE EFFECTS:
    none

======================================================================*/
int main()
{
    try
    {
        Honeywell6130Sensor sensor( "/dev/i2c-1", 0x27 );

        while( true ) 
        {
            TempHumidityData data = sensor.Read();

            cout << "TempC: " << data.tempCelcius << "  ";
            cout << "TempF: " << data.tempFahrenheit << "  Humidity: " << data.relativeHumidity;
            cout << endl;

            sleep( 1 );
        }
    }
    catch( SensorException ex )
    {
        cout << ex.what() << endl << endl;
    }

    return 0;
}

/*=====================================================================
// IMPLEMENTATION NOTES
//=====================================================================

    None

=====================================================================*/

