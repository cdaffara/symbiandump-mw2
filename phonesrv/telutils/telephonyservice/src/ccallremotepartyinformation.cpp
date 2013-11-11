/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#include <ccallremotepartyinformation.h>
#include <mcallremotepartyinfo.h>

#include "ccallremotepartyinformationimpl.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformation::CCallRemotePartyInformation()
    {
    }


// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInformation::ConstructL()
    {
    iImpl = CCallRemotePartyInformationImpl::NewL(); 
    }


// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CCallRemotePartyInformation* CCallRemotePartyInformation::NewL()
    {
    CCallRemotePartyInformation* self = 
        new( ELeave ) CCallRemotePartyInformation;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInformation::~CCallRemotePartyInformation()
    {
    delete iImpl;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallRemotePartyInformation::NotifyChanges(
        MCallRemotePartyInformationObserver& aObserver )
    {
    iImpl->NotifyChanges( aObserver );    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallRemotePartyInformation::CancelNotification()
    {
    iImpl->CancelNotification( ); 
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CCallRemotePartyInfoIter& CCallRemotePartyInformation::
    GetRemotePartyInformationL( )
    {
    return iImpl->GetRemotePartyInformationL( );  
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
EXPORT_C const MCallRemotePartyInfo* 
    CCallRemotePartyInformation::GetRemotePartyInformationL( 
    TInt aCallIndex )
    {
    return iImpl->GetRemotePartyInformationL( aCallIndex );    
    }
    
