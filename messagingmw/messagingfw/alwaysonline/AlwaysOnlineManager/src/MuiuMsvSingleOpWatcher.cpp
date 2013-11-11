/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
*     Messaging progress watching classes
*
*/




// INCLUDE FILES
#include <MSVAPI.H>
//#include "muiudomainpan.h"
#include "MuiuMsvSingleOpWatcher.H"

// CONSTANTS
const TInt KSingleOpWatcherPriority = KMaxTInt;


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------
// CMsvSingleOpWatcher::NewLC
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher* CMsvSingleOpWatcher::NewLC( MMsvSingleOpWatcher& aObserver )
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::NewL
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher* CMsvSingleOpWatcher::NewL( MMsvSingleOpWatcher& aObserver )
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::CMsvSingleOpWatcher
//
// ---------------------------------------------------------
//
CMsvSingleOpWatcher::CMsvSingleOpWatcher( MMsvSingleOpWatcher& aObserver )
    : CActive( KSingleOpWatcherPriority ), iObserver( aObserver )
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::ConstructL
//
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::ConstructL()
    {
    // Nothing to do, reserved for future use...
    }


// ---------------------------------------------------------
// CMsvSingleOpWatcher::~CMsvSingleOpWatcher
//
// ---------------------------------------------------------
//
EXPORT_C CMsvSingleOpWatcher::~CMsvSingleOpWatcher()
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::SetOperation
// --- Setter ---
// Must only be called once during the lifetime of a CMsvSingleOpWatcher object.
// ---------------------------------------------------------
//
EXPORT_C void CMsvSingleOpWatcher::SetOperation(CMsvOperation* aOperation)
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::Operation
// --- Accessor ---
// ---------------------------------------------------------
//
EXPORT_C CMsvOperation& CMsvSingleOpWatcher::Operation() const
    {}


// ---------------------------------------------------------
// CMsvSingleOpWatcher::DoCancel
// --- From CActive ---
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::DoCancel()        
    {}


//------------------------------------
// CMsvSingleOpWatcher::RunL
// --- From CActive ---
// ---------------------------------------------------------
//
void CMsvSingleOpWatcher::RunL()
    {}

// End of file
