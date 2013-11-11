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
* Description: Panic implemenataion for landmarks
*
*
*/


// INCLUDE FILES
#include "EPos_LandmarksErrors.h"

// -----------------------------------------------------------------------------
// Panic
// -----------------------------------------------------------------------------
EXPORT_C GLDEF_C void Panic(const TDesC& aCategory, TInt aReason)
    {
    User::Panic(aCategory, aReason);
    }

//  End of File
