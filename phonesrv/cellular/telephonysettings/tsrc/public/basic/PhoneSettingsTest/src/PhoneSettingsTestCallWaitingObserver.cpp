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
* Description: Implementation of CPhoneSettingsTestCallWaitingObserver class.
*
*/



// INCLUDE FILES
#include <PsetContainer.h>
#include <PsetCallWaiting.h>
#include <nwdefs.h> // Part of: Phone Settings observer API (domain)
#include <etelmm.h> 
#include "PhoneSettingsTestCallWaitingObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::CPhoneSettingsTestCallWaitingObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallWaitingObserver::CPhoneSettingsTestCallWaitingObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallWaitingObserver* CPhoneSettingsTestCallWaitingObserver::NewL()
    {
    CPhoneSettingsTestCallWaitingObserver* self = 
        new (ELeave) CPhoneSettingsTestCallWaitingObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallWaitingObserver::~CPhoneSettingsTestCallWaitingObserver()
    {          		
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::HandleCallWaitingGetStatusL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::HandleCallWaitingGetStatusL( 
    const MPsetCallWaiting::TGetCallWaitingStatus /*aStatus*/, 
    TUint8 /*aBsc*/[KPSetNumberOfBsc])
    {    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::HandleCallWaitingChangedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::HandleCallWaitingChangedL( 
    const MPsetCallWaiting::TSetCallWaiting /*aSetting*/, const TInt /*aResult*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::HandleCWRequestingL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::HandleCWRequestingL(TBool /*aOngoing*/, 
    TBool /*aInterrupted*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::HandleCWErrorL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::HandleCWErrorL(TInt /*aReason*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallWaitingObserver::SetEngineContact
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallWaitingObserver::SetEngineContact(
    MPsetCallWaiting& aEngineContact)
    {
    *iEngineContact = aEngineContact;
    }

	        
//  End of File
