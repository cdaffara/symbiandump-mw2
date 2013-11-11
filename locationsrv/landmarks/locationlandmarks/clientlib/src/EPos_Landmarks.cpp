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
* Description: Common implementation  for Landmarks API
*
*
*/


// INCLUDE FILES
#include    <ecom/ecom.h>
#include    "EPos_Landmarks.h"

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// ReleaseLandmarkResources
// Calls FinalClose on ECom
// -----------------------------------------------------------------------------
//
EXPORT_C void ReleaseLandmarkResources(TAny*)
    {
    REComSession::FinalClose();
    }

//  End of File
