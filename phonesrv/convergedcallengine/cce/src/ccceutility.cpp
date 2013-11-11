/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility methods for CCE internal use
*
*/


//  INCLUDE FILES
#include "ccceutility.h"


// Name present in CCE panics
_LIT( KCCEName, "CCE" );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Generate CCE panic
// ---------------------------------------------------------------------------
//
void CCCEUtility::Panic( const TCCEPanic aPanic )
    {
    User::Panic( KCCEName, aPanic );
    }
