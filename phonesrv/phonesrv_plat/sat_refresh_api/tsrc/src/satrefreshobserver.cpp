/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observers SAT refresh events 
*
*/


// INCLUDE FILES
#include <Stiftestinterface.h>
#include "satrefreshobserver.h"
#include <stifparser.h>
#include <e32math.h>


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CSatRefreshObserver::CSatRefreshObserver( CStifLogger* aLog ) :
    iLog( aLog )
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSatRefreshObserver::~CSatRefreshObserver()
    { 

    }
// ---------------------------------------------------------------------------
// From class MSatRefreshObserver.
// 
// ---------------------------------------------------------------------------
//
TInt CSatRefreshObserver::AllowRefresh( TSatRefreshType /*aType*/, 
    const TSatRefreshFiles& /*aFiles*/ )
    {
    return 0;
    }
// ---------------------------------------------------------------------------
// From class MSatRefreshObserver.
// 
// ---------------------------------------------------------------------------
//
void CSatRefreshObserver::Refresh( TSatRefreshType /*aType*/, 
    const TSatRefreshFiles& /*aFiles*/ )
    {
    
    }
