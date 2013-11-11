/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides information about ongoing calls.
*
*/


#include <e32base.h>

#include "ccallinformation.h"
#include "ccallinformationimpl.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CCallInformation::CCallInformation()
    {
        
    }


// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CCallInformation::ConstructL()
    {
    
    iImpl = CCallInformationImpl::NewL();    
    }


// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CCallInformation* CCallInformation::NewL()
    {
        
    CCallInformation* self = new (ELeave) CCallInformation( );    
        
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }



// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallInformation::~CCallInformation()
    {
        
    delete iImpl;
    }



// ---------------------------------------------------------------------------
// Sets observer for call information changes.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallInformation::NotifyCallInformationChanges(
    MCallInformationObserver& aObserver )
    {
    iImpl->NotifyCallInformationChanges( aObserver );    
    return;
    }
    
// ---------------------------------------------------------------------------
// Cancels previously placed NotifyCallInformationChangesL call.
// ---------------------------------------------------------------------------
//
EXPORT_C void CCallInformation::CancelNotification( )
    {
    iImpl->CancelNotification( ); 
    return;
    }
    
// ---------------------------------------------------------------------------
// Gets the snapshot of ongoing calls.
// ---------------------------------------------------------------------------
//
EXPORT_C CCallInfoIter& CCallInformation::GetCallsL( ) 
    {
    
    return iImpl->GetCallsL( );
    
    }

// ---------------------------------------------------------------------------
// Returns information whether call in given state exist. 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CCallInformation::IsCallInState( CCPCall::TCallState aState ) const
    {
    return iImpl->IsCallInState( aState );
    }


