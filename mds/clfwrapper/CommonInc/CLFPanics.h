/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/



#ifndef CLFPANICS_H
#define CLFPANICS_H

//  INCLUDES
#include <e32std.h>

// CONSTANTS
_LIT( KCLFPanicText, "ContentListingFramework" );

enum TCLFPanicCodes
    {
    ECLFNullPointer         = 0x1,
    ECLFIncorrectCommand    = 0x2,
    ECLFNoSourcePlugins     = 0x3,
    ECLFDbWorkThreadRunning = 0x4,
    ECLFInvalidDbItem       = 0x5,
    ECLFDbInvalidCommand    = 0x6,
    ECLFDbDatabaseCorrupt   = 0x7,
    ECLFInvalidFieldType    = 0x8,
    ECLFNotReady            = 0x9
    };

#endif      // CLFPANICS_H

// End of File
