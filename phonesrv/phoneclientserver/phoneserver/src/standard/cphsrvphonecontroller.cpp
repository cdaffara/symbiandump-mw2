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
* Description:  Phone Controller.
*
*/



// INCLUDE FILES
#include "cphsrvphonecontroller.h" 
#include <mmtsy_names.h>


// CONSTANTS
#define KPhSrvTsyName KMmTsyModuleName

const TInt KPhSrvTSYNameMaxLength( 80 );
const TInt KPhSrvETelRetryCount = 7;
const TInt KPhSrvETelRetryTimeout = 100000; // us



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvPhoneController::~CPhSrvPhoneController
// 
// C++ destructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
CPhSrvPhoneController::~CPhSrvPhoneController()
    {
    iCustomPhone.Close();
    iMobilePhone.Close();
    iETelServer.Close();
    }


// -----------------------------------------------------------------------------
// CPhSrvPhoneController::ConstructL
// 
// Symbian OS 2nd phase constructor
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
void CPhSrvPhoneController::ConstructL()
    {
    TInt err = KErrNone; 
    TInt phones = 0; // no phones.


    // Retry ETel connection if needed:
    for ( TInt a = 0 ; a < KPhSrvETelRetryCount ; a++ )
        {
        err = iETelServer.Connect();
        if ( err == KErrNone )
            {
            break;
            }
        User::After( KPhSrvETelRetryTimeout );
        }
    User::LeaveIfError( err );

    err = iETelServer.LoadPhoneModule(KPhSrvTsyName);
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }

    RTelServer::TPhoneInfo info;
    TBuf< KPhSrvTSYNameMaxLength > tsyName;
    iETelServer.EnumeratePhones( phones );

    TInt i = 0; // index starts from zero.
    for (; i < phones ; i++ )
        {
        err = iETelServer.GetPhoneInfo( i, info );
        User::LeaveIfError( err );

        err = iETelServer.GetTsyName( i, tsyName );
        User::LeaveIfError( err );

        if ( !tsyName.CompareF( KPhSrvTsyName ) )
            {
            break;
            }
        }

    if ( i == phones )
        {
        User::Leave( KErrNotSupported );
        }

    User::LeaveIfError( 
        iETelServer.SetExtendedErrorGranularity( RTelServer::EErrorExtended ) );

    User::LeaveIfError( iMobilePhone.Open( iETelServer , info.iName ) );

    User::LeaveIfError( iCustomPhone.Open( iMobilePhone ) );

    }


// -----------------------------------------------------------------------------
// CPhSrvPhoneController::PhSrvETelServer
// 
// Access the ETel session.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
RTelServer& CPhSrvPhoneController::PhSrvETelServer()
    {
    return iETelServer;
    }


// -----------------------------------------------------------------------------
// CPhSrvPhoneController::PhSrvMobilePhone
// 
// Access the GSM phone subsession.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
RMobilePhone& CPhSrvPhoneController::PhSrvMobilePhone()
    {
    return iMobilePhone;
    }


// -----------------------------------------------------------------------------
// CPhSrvPhoneController::PhSrvCustomPhone
// 
// Access the custom phone subsession.
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / ETel
*   Series 60  ETel API
*****************************************************/
RMmCustomAPI& CPhSrvPhoneController::PhSrvCustomPhone()
    {
    return iCustomPhone;
    }

// End of File
