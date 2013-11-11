/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPSetSubscriberIdCheck class.
*
*/


// INCLUDE FILES
#include    "psetsubscriberidcheck.h" 

_LIT( KRogersImsi, "30272" );
const TInt KMinImsiLength = 5;

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// First phase constructor
// ---------------------------------------------------------------------------
//
CPSetSubscriberIdCheck* CPSetSubscriberIdCheck::NewL( )
    {
    CPSetSubscriberIdCheck* self = new ( ELeave ) CPSetSubscriberIdCheck( );
    return self;
    }

// -----------------------------------------------------------------------------
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPSetSubscriberIdCheck::CPSetSubscriberIdCheck()
    {
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPSetSubscriberIdCheck::~CPSetSubscriberIdCheck()
    {
    } 

// ---------------------------------------------------------------------------
// Check if used SIM support dual activation.
// ---------------------------------------------------------------------------
//
TBool CPSetSubscriberIdCheck::DoesSIMSupportDualActivation()
    {
    TBool result = EFalse;
    TInt error = KErrNone;

    if( iImsi.Length() == 0 )
        {
        RTelServer rTelServer;
        RTelServer::TPhoneInfo info;
        RMobilePhone rMobilePhone;
        TRequestStatus tRequestStatus;

        error = rTelServer.Connect();
        if( error == KErrNone ) 
            {
            error = rTelServer.GetPhoneInfo( 0, info );
            if( error == KErrNone )
                {
                error = rMobilePhone.Open( rTelServer, info.iName );
                if( error == KErrNone )
                    {
                    rMobilePhone.GetSubscriberId( tRequestStatus, iImsi );
                    User::WaitForRequest( tRequestStatus );
                    error = tRequestStatus.Int();
                    rMobilePhone.Close();
                    }
                }
            rTelServer.Close();
            }
        }

    if(( iImsi.Length() >= KMinImsiLength ) && ( error == KErrNone ))
        {
        if( iImsi.Left( KMinImsiLength ).Compare( KRogersImsi ) == 0 )
            {
            result = ETrue;
            }
        }

    return result;
    }

// End of File
