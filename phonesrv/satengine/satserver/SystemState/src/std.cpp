/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  General functions
*
*/



// INCLUDE FILES
#include    <e32std.h>
#include    "Panic.h"
#include    "SatLog.h"

// CONSTANTS
_LIT( KSatSystemStatePanic, "SatSystemState" );

void Panic( TSatSystemStatePanic aPanic )
    {
    LOG2( SIMPLE, "SATSYSTEMSTATE: Panic calling: %d", aPanic )
    User::Panic( KSatSystemStatePanic, aPanic );
    LOG( SIMPLE, "SATSYSTEMSTATE: Panic exiting" )
    }

//  End of File
