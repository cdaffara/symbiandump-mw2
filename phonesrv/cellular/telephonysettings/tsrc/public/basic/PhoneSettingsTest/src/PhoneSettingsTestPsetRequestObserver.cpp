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
* Description: Implementation of CPhoneSettingsTestPsetRequestObserver class.
*
*/



// INCLUDE FILES
#include "PhoneSettingsTestPsetRequestObserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestPsetRequestObserver::CPhoneSettingsTestPsetRequestObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestPsetRequestObserver::CPhoneSettingsTestPsetRequestObserver()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestPsetRequestObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestPsetRequestObserver::ConstructL()
    {    
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestPsetRequestObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestPsetRequestObserver* CPhoneSettingsTestPsetRequestObserver::NewL()
    {
    CPhoneSettingsTestPsetRequestObserver* self = 
        new (ELeave) CPhoneSettingsTestPsetRequestObserver();     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestPsetRequestObserver::~CPhoneSettingsTestPsetRequestObserver()
    {          	
    }
    
// -----------------------------------------------------------------------------
// CPhoneSettingsTestPsetRequestObserver::RequestComplete
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestPsetRequestObserver::RequestComplete()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestPsetRequestObserver::RequestComplete
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestPsetRequestObserver::RequestStatusChanged(
    TInt /*aNewStatus*/)
    {
    }
	
	
//  End of File
