/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides implementation class for CDialer -class.
*
*/

#include "cdialer.h"

#include <mmtsy_names.h>
#include <cphcltemergencycall.h>
#include <mphcltemergencycallobserver.h>
#include <cphcltcommandhandler.h>

//Speaking clock number when calling from NTN network.
_LIT (KNumber, "99901");
_LIT( KEmergencyNumber, "112" );

// ======== MEMBER FUNCTIONS ========

CDialer::CDialer( RTelServer& aTelServer ) 
    : CActive( EPriorityStandard ), iTelServer( aTelServer )
    { 
    CActiveScheduler::Add( this ); 
    }

void CDialer::ConstructL()
    { 
    TBuf<100> name;
    name.Append( KMmTsyPhoneName );
    name.Append( _L("::") );
    name.Append( KMmTsyVoice1LineName );
    name.Append( _L("::") );
    TBuf<100> callName;
    User::LeaveIfError( iCall.OpenNewCall( iTelServer, name, callName  ) );
    iEmergency = CPhCltEmergencyCall::NewL( this );
    iCmdHandler = CPhCltCommandHandler::NewL();
    iWait = new( ELeave )CActiveSchedulerWait(); 
    }

CDialer* CDialer::NewL( RTelServer& aTelServer )
    { 
    CDialer* self = CDialer::NewLC( aTelServer );
    CleanupStack::Pop( self );
    return self;
    }

CDialer* CDialer::NewLC( RTelServer& aTelServer )
    {
    CDialer* self = new( ELeave )CDialer( aTelServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CDialer::~CDialer()
    {
    if( IsActive() )
        {
        WaitForRequest();
        }
    
    delete iEmergency;
    delete iCmdHandler;
    delete iWait;
    iCall.Close();
    }

void CDialer::Dial( const TDesC& aNumber )
    {
    iCall.Dial( iStatus, aNumber );
    SetActive();
    }
    
void CDialer::Dial()
    {
    Dial( KNumber() );
    }

void CDialer::DialEmergency()
    {
    iEmergency->DialEmergencyCallL( KEmergencyNumber );
    }
    
void CDialer::Hangup()
    {
    iCall.HangUp(iStatus);
    User::WaitForRequest(iStatus);
    //SetActive();
    }
    
void CDialer::HangupEmergencyCall()
    {
    iCmdHandler->Chup( iStatus );    
    SetActive();
    }
    
void CDialer::WaitForRequest()
    {
    if ( IsActive() || iEmergency->IsActive() )
        {
        iWait->Start();
        }
    }

void CDialer::RunL()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }
    
void CDialer::DoCancel()
    { 
    }
    
void CDialer::HandleEmergencyDialL( const TInt aStatus )
    {
    if ( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }
    }


    



