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
* Description: Landmarks Server panic codes used for panicking a client.
*
*
*/


// INCLUDE FILES
#include "EPos_LandmarksServerPanics.h"

// -----------------------------------------------------------------------------
// PanicClient
// -----------------------------------------------------------------------------
GLDEF_C void PanicClient(
    const RMessagePtr2& aMessagePtr,
    TInt aReason)
    {
    if (!aMessagePtr.IsNull())
        {
        aMessagePtr.Panic(KPosLandmarksServerPanic, aReason);
        }
    }

// -----------------------------------------------------------------------------
// PanicServer
// -----------------------------------------------------------------------------
GLDEF_C void PanicServer(
    TInt aReason)
    {
    User::Panic(KPosLandmarksServerPanic, aReason);
    }
//  End of File
