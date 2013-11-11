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
* Description: Implementation of CPhoneSettingsTestCallBarringObserver class.
*
*/



// INCLUDE FILES
#include <PsetContainer.h>
#include <PsetCallBarring.h>
#include <nwdefs.h> 
#include <etelmm.h> 
#include "PhoneSettingsTestCallBarringObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::CPhoneSettingsTestBarringObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallBarringObserver::CPhoneSettingsTestCallBarringObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarringObserver::ConstructL()
    {
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestCallBarringObserver* CPhoneSettingsTestCallBarringObserver::NewL()
    {
    CPhoneSettingsTestCallBarringObserver* self = 
        new (ELeave) CPhoneSettingsTestCallBarringObserver();         
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestCallBarringObserver::~CPhoneSettingsTestCallBarringObserver()
    {          		
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::HandleBarringModeChangedL
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestCallBarringObserver::HandleBarringModeChangedL( 
    TBarringProgram /*aType*/, TBarringStatus /*aStatus*/, TBool /*aPlural*/)
    {    
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::HandleBarringModeStatusL
// -----------------------------------------------------------------------------
//    
void CPhoneSettingsTestCallBarringObserver::HandleBarringModeStatusL( 
    TUint8 /*aBsc*/[KPSetNumberOfBsc], TBarringStatus /*aStatus*/)
    {    
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::HandleBarringErrorL
// -----------------------------------------------------------------------------
//        
void CPhoneSettingsTestCallBarringObserver::HandleBarringErrorL(TInt /*aReason*/)
    {    
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::HandleCBRequestingL
// -----------------------------------------------------------------------------
//            
void CPhoneSettingsTestCallBarringObserver::HandleCBRequestingL( 
    TBool /*aTrue*/, TBool /*aInterrupted*/)
    {    
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::SetEngineContact
// -----------------------------------------------------------------------------
//            
void CPhoneSettingsTestCallBarringObserver::SetEngineContact( 
    MPsetCallBarring* aBarringEngine)
    {
    iBarringEngine = aBarringEngine; 
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestBarringObserver::CbPasswordChangedL
// -----------------------------------------------------------------------------
//            
void CPhoneSettingsTestCallBarringObserver::CbPasswordChangedL(TBool /*aSuccess*/)
    {    
    }
	
	
//  End of File
