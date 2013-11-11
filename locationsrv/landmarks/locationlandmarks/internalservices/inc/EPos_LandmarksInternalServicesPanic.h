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
*   Panic codes for internal services
*
*/


#ifndef LANDMARKINTERNALSERVICESPANICS_H
#define LANDMARKINTERNALSERVICESPANICS_H

#include <e32def.h>

// DATA TYPES
enum TPosLandmarkInternalServicesDebugPanic
    {
    EPosLandmarkInternalServicesInvalidArgument = 0
    };

// FUNCTION PROTOTYPES
GLDEF_C void DebugPanic(TPosLandmarkInternalServicesDebugPanic aPanic);

#endif      // LANDMARKINTERNALSERVICESPANICS_H

// End of File
