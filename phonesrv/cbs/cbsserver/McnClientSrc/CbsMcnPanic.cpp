/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is called by this library's functions on critical error situations.
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include <cbsmcnpanic.h>        // common header

// CONSTANTS

_LIT( KMcnPanic, "CbsMcn" );

// ==================== LOCAL FUNCTIONS ====================

// -----------------------------------------------------------------------------
// CbsMcnPanic
// Panics the McnClient
// Returns: None
// -----------------------------------------------------------------------------
//
GLDEF_C void CbsMcnPanic( 
    TInt aPanic )
    {
    User::Panic( KMcnPanic, aPanic );   
    }

//  End of File  
