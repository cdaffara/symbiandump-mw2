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
* Description: Implementation of CPhoneSettingsTestCliObserver class.
*
*/



// INCLUDE FILES
#include <PsetContainer.h>
#include <PsetCli.h>
#include <nwdefs.h> 
#include <etelmm.h> 
#include "PhoneSettingsTestCliObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::CPhoneSettingsTestCliObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCliObserver::CPhoneSettingsTestCliObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCliObserver* CPhoneSettingsTestCliObserver::NewL()
    {
    CPhoneSettingsTestCliObserver* self = 
        new (ELeave) CPhoneSettingsTestCliObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCliObserver::~CPhoneSettingsTestCliObserver()
    {          		
    }
        
// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::SetEngineContact
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::SetEngineContact(MPsetCli* aEngine)
    {
    iEngine = aEngine; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::HandleCliRequestingL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::HandleCliRequestingL(TBool /*aOngoing*/, 
    TBool /*aInterrupted*/)
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::CliInformationL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::CliInformationL(TPsuiCli /*aType*/)
    {
    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::HandleCliStatusL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::HandleCliStatusL(
    TUint8 /*aBsc*/[KPSetNumberOfBsc], TPsuiCli /*aMode*/)
    {
    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::HandleCnapStatusL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::HandleCnapStatusL(TInt /*aStatus*/)
    {
    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestCliObserver::HandleCliErrorL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCliObserver::HandleCliErrorL(TInt /*aError*/)
    {
    
    }
	        
//  End of File
