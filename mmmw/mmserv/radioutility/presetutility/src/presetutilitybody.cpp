/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#include "presetutilitybody.h"
#include "cenrephandler.h"
#include "presetcrkeys.h"
#include <presetutilityobserver.h>
#include "trace.h"


// ============= MEMBER FUNCTIONS ==============

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CPresetUtilityBody* CPresetUtilityBody::NewL()
    {
    FUNC_LOG;
    CPresetUtilityBody* self = new (ELeave) CPresetUtilityBody();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CPresetUtilityBody::CPresetUtilityBody()
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CPresetUtilityBody::~CPresetUtilityBody()
    {
    FUNC_LOG;
    delete iCenrepHandler;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CPresetUtilityBody::ConstructL()
    {
    FUNC_LOG;
    iCenrepHandler = CCenrepHandler::NewL();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CPresetUtilityBody::SetObserver( MPresetUtilityObserver* aObserver )
    {
    FUNC_LOG;
    iObserver = aObserver;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::MaxNumberOfPresets() const
    {
    FUNC_LOG;
    return iCenrepHandler->MaxPresetCount();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::PresetCount() const
    {
    FUNC_LOG;
    return iCenrepHandler->PresetCount();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::FirstPreset() const
    {
    FUNC_LOG;
    return iCenrepHandler->FirstPreset();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::NextPreset( TInt aFromIndex ) const
    {
    return iCenrepHandler->NextPreset( aFromIndex );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::DeletePreset( TInt aIndex )
    {
    FUNC_LOG;
    TInt err( iCenrepHandler->DeletePreset( aIndex ) );
    if ( iObserver )
        {
        iObserver->PresetChanged( MPresetUtilityObserver::EPresetDeleted, aIndex );
        }
    return err;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CPresetUtilityBody::SavePresetL( const TPreset& aPreset, TInt aIndex )
    {
    FUNC_LOG;
    TInt usedIndex = iCenrepHandler->SavePresetL( aPreset, aIndex );
    if ( iObserver )
        {
        iObserver->PresetChanged( MPresetUtilityObserver::EPresetAdded, aIndex );
        }
    return usedIndex;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CPresetUtilityBody::ReadPresetL( TInt aIndex, TPreset& aPreset )
    {
    FUNC_LOG;
    iCenrepHandler->ReadPresetL( aIndex, aPreset );
    }
