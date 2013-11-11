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
* Description: Implementation of CPhoneSettingsTestNetworkInfoObserver class.
*
*/



// INCLUDE FILES
#include <mpsetnetworkinfoobs.h>
#include "PhoneSettingsTestNetworkInfoObserver.h"

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::CPhoneSettingsTestNetworkInfoObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetworkInfoObserver::CPhoneSettingsTestNetworkInfoObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestNetworkInfoObserver* CPhoneSettingsTestNetworkInfoObserver::NewL()
    {
    CPhoneSettingsTestNetworkInfoObserver* self = 
        new (ELeave) CPhoneSettingsTestNetworkInfoObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestNetworkInfoObserver::~CPhoneSettingsTestNetworkInfoObserver()
    {          		
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleNetworkInfoReceivedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleNetworkInfoReceivedL( 
            const CNetworkInfoArray* /*aInfoArray*/, const TInt /*aResult*/)
    {
    }
        
// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleCurrentNetworkInfoL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleCurrentNetworkInfoL( 
            const MPsetNetworkSelect::TCurrentNetworkInfo& /*aCurrentInfo*/, 
            const TInt /*aResult*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleNetworkChangedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleNetworkChangedL( 
            const MPsetNetworkSelect::TNetworkInfo& /*aCurrentInfo*/,
            const MPsetNetworkSelect::TCurrentNetworkStatus /*aStatus*/, 
            const TInt /*aResult*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleNetworkChangedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleNetworkChangedL( 
            const MPsetNetworkSelect::TNetworkInfo& /*aCurrentInfo*/,
            const RMobilePhone::TMobilePhoneRegistrationStatus& /*aStatus*/, 
            const TInt /*aResult*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleSearchingNetworksL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleSearchingNetworksL(
    TServiceRequest /*aRequest*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleRequestingSelectedNetworkL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleRequestingSelectedNetworkL(
    TBool /*aOngoing*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleCallActivatedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleCallActivatedL()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestNetworkInfoObserver::HandleNetworkErrorL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestNetworkInfoObserver::HandleNetworkErrorL( 
    const MPsetNetworkInfoObserver::TServiceRequest /*aRequest*/, 
    const TInt /*aError*/)
    {
    }
    
	        
//  End of File
