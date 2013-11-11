/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Emergency Number Manager.
*
*/



// INCLUDE FILES
#include "cphsrvemergencynumbermanager.h" 
#include "mphsrvphoneinterface.h" 
#include "phsrvdebuginfo.h" 
#include <cemergencynumberpolicy.h> 

// CONSTANTS
_LIT(KPhSrvDosEnPolicyLibName, "dosenpolicy.dll");  // DOS emergency number policy
_LIT(KPhSrvSosEnPolicyLibName, "sosenpolicy.dll");  // SOS emergency number policy

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvEmergencyNumberManager::CPhSrvEmergencyNumberManager()
// 
// C++ constructor
// -----------------------------------------------------------------------------
//
CPhSrvEmergencyNumberManager::CPhSrvEmergencyNumberManager(
    MPhSrvPhoneInterface& aPhone ) : iPhone( aPhone )
    {
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyNumberManager::~CPhSrvEmergencyNumberManager()
// 
// C++ destructor
// -----------------------------------------------------------------------------
//
CPhSrvEmergencyNumberManager::~CPhSrvEmergencyNumberManager()
    {
    delete iEmergencyNumberPolicy;    
    iEnPolicyLib.Close();
    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyNumberManager::ConstructL()
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CPhSrvEmergencyNumberManager::ConstructL( TBool aIsDosEnPolicy )
    {
    if ( aIsDosEnPolicy )
        {
        // Dynamically load Dos emergency DLL.
        User::LeaveIfError( iEnPolicyLib.Load( KPhSrvDosEnPolicyLibName ) );
        }
    else
        {
        // Dynamically load Sos emergency DLL.
        User::LeaveIfError( iEnPolicyLib.Load( KPhSrvSosEnPolicyLibName ) );
        }

    if ( iEnPolicyLib.Type()[1] != KEnPolicyUid )
        {
        iEnPolicyLib.Close();
        User::Leave( KErrNotFound );
        }

    TLibraryFunction entry=iEnPolicyLib.Lookup(1);
    CEmergencyNumberPolicy* enPolicy = ( CEmergencyNumberPolicy* ) entry();    
    CleanupStack::PushL( enPolicy );
    enPolicy->ConstructL( iPhone.PhSrvETelServer() );
    CleanupStack::Pop();

    iEmergencyNumberPolicy = enPolicy;

    }


// -----------------------------------------------------------------------------
// CPhSrvEmergencyNumberManager::EmergencyNumberManagerIsEmergencyNumberL()
// 
// Enquire as to whether the specified number would result
// in an emergency call being placed.
//
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
/*****************************************************
*   Series 60 Customer / DOS
*   Needs customer DOS implementation
*****************************************************/
TBool CPhSrvEmergencyNumberManager::EmergencyNumberManagerIsEmergencyNumberL(
    TDes& aNumber, 
    TPhCltEmergencyNumberQueryMode aMode 
    )
    {
    _DPRINT( 4, "PSrv.EN.IsEmergency.begin" );   // debug print

    // check length of number, it must be between 1..6
    if ( aNumber.Length() < 1 )
        {
        // Empty. No further checking needed.
        return EFalse;
        }
    
    TBool isEmergencyNumber = EFalse;
    TInt err = 
        iEmergencyNumberPolicy->IsEmergencyNumber ( 
                            static_cast< TPhCltTelephoneNumber& >(aNumber),
                            aMode, 
                            isEmergencyNumber );

    if ( err != KErrNone )
        {
        // error in checking
        isEmergencyNumber = EFalse;
        }

    return isEmergencyNumber;
    }

// End of File.
