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
* Description: Implementation of CPhoneSettingsTestUtilities class.
*
*/



// INCLUDE FILES
#include <etelmm.h> 
#include <MmTsy_names.h> 
#include "psetconstants.h"
#include "PhoneSettingsTestUtilities.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneSettingsTestUtilities::CPhoneSettingsTestUtilities
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestUtilities::CPhoneSettingsTestUtilities()
    {	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestUtilities::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestUtilities::ConstructL()
    {    
    } 

// -----------------------------------------------------------------------------
// CPhoneSettingsTestUtilities::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneSettingsTestUtilities* CPhoneSettingsTestUtilities::NewL()

    {
    CPhoneSettingsTestUtilities* self = new (ELeave) CPhoneSettingsTestUtilities;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CPhoneSettingsTestUtilities::~CPhoneSettingsTestUtilities()
    {          	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestUtilities::OpenPhone
// Initilizes given RTelServer, RMobilePhone and RMobileLine objects
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestUtilities::OpenPhoneL(
    RTelServer& aTelServer, 
    RMobilePhone& aPhone,    
	RMobileLine& aLine)	
    {
    // ConnectServer
    TInt err = KErrNone;
    for ( TInt a = 0 ; a < KPSetRetryCount ; a++ )
        {
        err = aTelServer.Connect();
        
        if ( err == KErrNone )
            {
            break;
            }        
        User::After( KPSetRetryTimeout );
        }    
    if ( err == KErrNone )
        {
        aTelServer.LoadPhoneModule( KMmTsyModuleName );
        }
        
    // Load phone     
    User::LeaveIfError( 
        aTelServer.SetExtendedErrorGranularity( RTelServer::EErrorExtended ) );    
    TInt numPhones;
    
    User::LeaveIfError( aTelServer.EnumeratePhones( numPhones ) );
    if ( !numPhones )
        {
        User::Leave( KErrGeneral );
        }
    
    // Match phone name to correct one
    RTelServer::TPhoneInfo phoneInfo;
    TName matchTsyName;
    TInt i = 0;
    for ( ; i < numPhones; i++ )
        {
        User::LeaveIfError( aTelServer.GetTsyName( i, matchTsyName ) );
        if    ( matchTsyName.CompareF( KMmTsyModuleName ) == 0 )
            {            
            User::LeaveIfError( aTelServer.GetPhoneInfo( i, phoneInfo ) );
            break;
            }
        }
    if ( i == numPhones )
        {
        User::Leave( KErrGeneral );
        }
    
    User::LeaveIfError( aPhone.Open( aTelServer, phoneInfo.iName ) ); 
    User::LeaveIfError( aLine.Open( aPhone, KMmTsyVoice1LineName ) );	
    }

// -----------------------------------------------------------------------------
// CPhoneSettingsTestUtilities::ClosePhoneL
// Closes given RTelServer, RMobilePhone and RMobileLine objects. 
// -----------------------------------------------------------------------------
//
void CPhoneSettingsTestUtilities::ClosePhoneL(
    RTelServer& aTelServer, 
    RMobilePhone& aPhone,    
	RMobileLine& aLine)	
    {
    aLine.Close();  
    aPhone.Close(); 
	aTelServer.Close();  
    }

		
//  End of File
