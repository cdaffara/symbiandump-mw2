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
* Description: Implements panics in Landmarks Internal Services
*
*
*/


// INCLUDES
#include <e32base.h>
#include "EPos_LandmarksInternalServicesPanic.h"

//
// ---------------------------------------------------------
// Panic
// Implements panics in Landmarks Internal Services
//
void DebugPanic(TPosLandmarkInternalServicesDebugPanic aPanic)
    {
    _LIT(KPosLandmarkInternalServicesPanicCategory,
        "Landmark Internal Services");
    User::Panic(KPosLandmarkInternalServicesPanicCategory, aPanic);
    }

//  End of File
