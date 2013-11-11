/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of view history
*
*/



// INCLUDE FILES
#include <mpxviewplugin.h>

#include "mpxviewhistory.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewHistory* CMPXViewHistory::NewL()
    {
    CMPXViewHistory* self = new ( ELeave ) CMPXViewHistory();
    // empty history, construction not needed
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewHistory* CMPXViewHistory::NewL( CMPXViewHistory* aHistory )
    {
    CMPXViewHistory* self = new ( ELeave ) CMPXViewHistory();
    CleanupStack::PushL( self );
    if ( aHistory )
        {
        self->ConstructL(
            aHistory->iUids, aHistory->iPluginType,
            aHistory->iPluginImplementationUid, aHistory->iPlugin );
        }
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMPXViewHistory* CMPXViewHistory::NewL(
    const RArray<TUid>& aUids,
    const TUid aPluginType,
    const TUid aPluginImplementationUid,
    CMPXViewPlugin* aPlugin )
    {
    CMPXViewHistory* self = new ( ELeave ) CMPXViewHistory();
    CleanupStack::PushL( self );
    self->ConstructL( aUids, aPluginType, aPluginImplementationUid, aPlugin );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// default constructor
// ---------------------------------------------------------------------------
//
CMPXViewHistory::CMPXViewHistory()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXViewHistory::~CMPXViewHistory()
    {
    iUids.Reset();
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMPXViewHistory::ConstructL(
    const RArray<TUid>& aUids,
    const TUid aPluginType,
    const TUid aPluginImpUid,
    CMPXViewPlugin* aPlugin )
    {
    iPluginType = aPluginType;
    iPluginImplementationUid = aPluginImpUid;
    iPlugin = aPlugin;

    for ( TInt i = 0; i < aUids.Count(); i++ )
        {
        iUids.Append( aUids[ i ] );
        }
    }

//  End of File
