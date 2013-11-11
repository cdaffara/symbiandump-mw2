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
* Description: Implementation of CPSetVariationProxy class.
*
*
*/


#include "psetvariationproxy.h" 
#include <centralrepository.h>
#include <settingsinternalcrkeys.h> 
#include <telservicesinternalcrkeys.h>

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPSetVariationProxy::CPSetVariationProxy()
    {   
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPSetVariationProxy::~CPSetVariationProxy()
    {       
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CPSetVariationProxy* CPSetVariationProxy::NewL( 
        const TUid& aUid, 
        const TUint aId )
    {
    CPSetVariationProxy* self = new ( ELeave ) CPSetVariationProxy();  
    CleanupStack::PushL( self );
    self->ConstructL( aUid, aId );
    CleanupStack::Pop( self );         
    return self;
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPSetVariationProxy::ConstructL( 
        const TUid& aUid, 
        const TUint aId  )
    {
    TInt keySetting = 0;
    CRepository* centralRepository = CRepository::NewLC( aUid );
    User::LeaveIfError( centralRepository->Get( aId, keySetting ) );
    
    if( ( aUid == KCRUidPhoneSettings ) && 
        ( keySetting ) )
        {
        iFeature |= KCallWaitingDistiquishNotProvisioned;   
        }
    else if ( aUid == KCRUidTelSrvVariation )
        {
        iFeature |= keySetting;
        }
    
    CleanupStack::PopAndDestroy( centralRepository );
    }

// ---------------------------------------------------------------------------
// IsFeatureEnabled
// ---------------------------------------------------------------------------
//
TBool CPSetVariationProxy::IsFeatureEnabled( TUint aFeature ) const
    {
    return aFeature&iFeature;
    }
