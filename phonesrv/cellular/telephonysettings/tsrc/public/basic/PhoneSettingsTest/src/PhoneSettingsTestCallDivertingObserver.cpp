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
* Description: Implementation of CPhoneSettingsTestCallDivertingObserver class.
*
*/



// INCLUDE FILES
#include <PsetContainer.h>
#include <PsetCallDiverting.h>
#include <nwdefs.h>
#include <etelmm.h> 
#include "PhoneSettingsTestCallDivertingObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::CPhoneSettingsTestCallDivertingObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallDivertingObserver::CPhoneSettingsTestCallDivertingObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallDivertingObserver* CPhoneSettingsTestCallDivertingObserver::NewL()
    {
    CPhoneSettingsTestCallDivertingObserver* self = 
        new (ELeave) CPhoneSettingsTestCallDivertingObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallDivertingObserver::~CPhoneSettingsTestCallDivertingObserver()
    {          		
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::HandleDivertingChangedL
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::HandleDivertingChangedL( 
    const TCallDivertSetting& /*aSetting*/, TBool /*aPlural*/)
    {    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::HandleDivertingStatusL
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::HandleDivertingStatusL( 
    CMobilePhoneCFList& /*aSetting*/, TBool /*aPlural*/)
    {    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::HandleDivertingErrorL
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::HandleDivertingErrorL(
    TInt /*aReason*/)
    {    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::HandleCFRequestingL
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::HandleCFRequestingL(
    TBool /*aOngoing*/, TBool /*aInterrupted*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCallDivertingObserver::SetEngineContact
// 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallDivertingObserver::SetEngineContact( 
    MPsetCallDiverting* aDivertEngine)
    {
    iDivertEngine = aDivertEngine; 
    }

	        
//  End of File
