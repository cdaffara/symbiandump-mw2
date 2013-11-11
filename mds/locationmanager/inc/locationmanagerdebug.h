/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Common trace-information file.
*
*/

#ifndef LOCATIONMANAGERDEBUG_H
#define LOCATIONMANAGERDEBUG_H

#include "harvesterlog.h"

/**
 *  Usage:  LOG(_L("[MODULE_NAME]\t Trace text here"));
 *          TRACE(Print(_L("[MODULE_NAME]\t Trace text here with parameter %d"), iCount));
 *
 *          Trace target can be changed below (file logging needs directory c:\logs\upnp)
 *          #define __CLOGGING -row uncommented            = Console logging
 */

// undefine for sure
#undef __CLOGGING__

// Uncomment this to get memory allocation prints from module
//#define MEMORY_PRINT
#ifdef _DEBUG
// Define one of these flags:
// CLOGGING = Console logging
#define __CLOGGING__
#endif

// Added to remove compilation warnings.
#define ARG_USED(a) (void)(a)


// Then actual definitions depending on the 
// flag values.

#ifdef _DEBUG

    #include <e32std.h>

    #include <f32file.h>

    // Define the top level macros
    #ifdef MEMORY_PRINT
        #define LOG(a) {PRINTLOG(a); MemPrint();}
        #define LOG1(a,b) {PRINTLOG1(a,b); MemPrint();}
        #define TRACE(a) {a; MemPrint();}
    #else
        #define LOG(a) {PRINTLOG(a);}
        #define LOG1(a,b) {PRINTLOG1(a,b);}
        #define TRACE(a) {a;}
    #endif 

    inline void MemPrint();

    // Console Logging on
    #define PRINTLOG(a) RDebug::Print(_L(a))
    #define PRINTLOG1(a,b) RDebug::Print(_L(a),b)
    
	#include "locationmanagerdebug.inl"  // inline fuctions implementations

#else

    // DEBUG build is not on --> no logging at all
    #define LOG(a)
    #define LOG1(a,b)
    #define TRACE(a)

#endif  // _DEBUG

#endif      // LOCATIONMANAGERDEBUG_H
            
// End of File
