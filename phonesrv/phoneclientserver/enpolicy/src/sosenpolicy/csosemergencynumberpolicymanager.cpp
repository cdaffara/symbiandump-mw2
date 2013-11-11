/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides SOS emergency number policy API to verify an emergency
*                number.        
*
*/



// INCLUDE FILES
#include "csosemergencynumberpolicymanager.h" 
#include "csosemergencynumberpolicyhandler.h" 


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyManager::~CSosEmergencyNumberPolicyManager()
// -----------------------------------------------------------------------------
//
CSosEmergencyNumberPolicyManager::~CSosEmergencyNumberPolicyManager()
    { 
    delete iSosEnPolicyHandler;
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyManager::ConstructL()
// -----------------------------------------------------------------------------
//
void CSosEmergencyNumberPolicyManager::ConstructL( RTelServer& aETelServer )
    {
    iSosEnPolicyHandler = CSosEmergencyNumberPolicyHandler::NewL( aETelServer);
    }

// -----------------------------------------------------------------------------
// CSosEmergencyNumberPolicyManager::IsEmergencyNumber
// 
// -----------------------------------------------------------------------------
//
TInt CSosEmergencyNumberPolicyManager::IsEmergencyNumber ( 
                                    TPhCltTelephoneNumber& aTelNumber, 
                                    TPhCltEmergencyNumberQueryMode aMode,
                                    TBool& aIsEmergencyNumber )
    {       
    return iSosEnPolicyHandler->IsEmergencyNumber( aTelNumber, 
                                                   aMode, 
                                                   aIsEmergencyNumber );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// NewEnPolicy()
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CEmergencyNumberPolicy* NewEnPolicyL()
    {
    return new (ELeave) CSosEmergencyNumberPolicyManager;
    }

// End of file
