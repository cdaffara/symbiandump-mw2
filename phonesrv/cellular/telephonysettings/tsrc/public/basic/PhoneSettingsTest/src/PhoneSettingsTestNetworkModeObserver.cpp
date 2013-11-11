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
* Description: Implementation of CPhoneSettingsTestNetworkModeObserver class.
*
*/



// INCLUDE FILES
#include <e32base.h>
#include <mpsetnetworkmodeobs.h>
#include "PhoneSettingsTestNetworkModeObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkModeObserver::CPhoneSettingsTestNetworkModeObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetworkModeObserver::CPhoneSettingsTestNetworkModeObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkModeObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkModeObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkModeObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetworkModeObserver* CPhoneSettingsTestNetworkModeObserver::NewL()
    {
    CPhoneSettingsTestNetworkModeObserver* self = 
        new (ELeave) CPhoneSettingsTestNetworkModeObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestNetworkModeObserver::~CPhoneSettingsTestNetworkModeObserver()
    {          		
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkModeObserver::HandleNetworkSystemModeEventsL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkModeObserver::HandleNetworkSystemModeEventsL( 
    const MPsetNetworkModeObserver::TServiceRequest /*aRequest*/,
    const TUint32 /*aNetworkModeCaps*/)
    {
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkModeObserver::HandleNetworkErrorL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkModeObserver::HandleNetworkErrorL(
    const MPsetNetworkModeObserver::TServiceRequest /*aRequest*/,
    const TInt /*aError*/)
    {
    }
	        
//  End of File
