/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*    Location Server panic codes.
*    These codes are used for catching errors in Location Server. Usually a
*    designer will add an __ASSERT_DEBUG macro in the code to detect bad
*    parameters etc.
*
*/



#ifndef __CSuplServerDebugPanic_H__
#define __CSuplServerDebugPanic_H__

#include <e32def.h>

enum TSuplServerDebugPanic
    {
    EPosSuplServerPanicRequestInconsistency,
    EPosSuplPanicSessionObjectNotInitialized,
    EPosSuplServerPanicSessionsCountInconsistency,
    EPosSuplServerPanicUnexpectedEvent,
    EPosSuplServerPanicGeneralInconsistency
    };

// FUNCTION PROTOTYPES
GLDEF_C void DebugPanic(TSuplServerDebugPanic aPanic);

#endif      // __CSuplServerDebugPanic_H__

// End of File
