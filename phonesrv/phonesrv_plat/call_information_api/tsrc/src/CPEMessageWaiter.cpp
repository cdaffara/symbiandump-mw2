/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/
#include "CPEMessageWaiter.h"

#include <eunitmacros.h>

#include "talogger.h"
#include "CActiveWait.h"


_LIT( KPanic, "CPEMessageWaiter" );
enum TPanic 
    {
    EArrivedMessagesOOM
    };

// Max time msg will be waited. If msg does not arrive in this time
// test will fail.
const TInt KDefaultMsgWaitingTimeout = 5000000; 

CPEMessageWaiter* CPEMessageWaiter::NewL()
    {
    CPEMessageWaiter* self = new( ELeave )CPEMessageWaiter();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
CPEMessageWaiter::~CPEMessageWaiter()
    {
    Cancel();
    delete iWait;
    iArrivedMsgs.Close();
    }
    
CPEMessageWaiter::CPEMessageWaiter() : CTimer( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }
    
void CPEMessageWaiter::ConstructL()
    {
    CTimer::ConstructL();
    iWait = new( ELeave )CActiveSchedulerWait();
    }
    
void CPEMessageWaiter::WaitForMsg( 
    MEngineMonitor::TPEMessagesFromPhoneEngine aMsg,
    TInt aTimeoutInSeconds)
    {
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::WaitForMsg, aMessage %d", aMsg );
    
    iWaitedMsgArrived = EFalse;
    iWaitedMsg = aMsg;
    
    After( aTimeoutInSeconds * 1000000 ); // Start timeout 
    iWait->Start();
    Cancel(); // Cancel the timer.
    
    if( !iWaitedMsgArrived )
        {
        TEFLOGSTRING2( KTAREQEND, 
           "CPEMessageWaiter::WaitForMsg, waited for message %d, and message did not arrive in timeout time", iWaitedMsg );
        EUNIT_FAIL_TEST( "TIMEOUT: Did not receive waited PE message");      
        }
    }
    
void CPEMessageWaiter::WaitL(
    TInt aTimeoutInSeconds )
    {
    RTimer timer;
    User::LeaveIfError( timer.CreateLocal() ); 
    CleanupClosePushL( timer );
    CActiveWait* wait = CActiveWait::NewLC();
    timer.After( wait->Status(), aTimeoutInSeconds * 1000000 );
    wait->WaitForRequest();
    CleanupStack::PopAndDestroy( 2 ); // timer, wait*/
    }
    
TBool CPEMessageWaiter::HasMsgArrived( 
    MEngineMonitor::TPEMessagesFromPhoneEngine aMsg )
    {
    return iArrivedMsgs.Find( aMsg ) != KErrNotFound;
    }

void CPEMessageWaiter::WaitForError( TInt aError )
    {
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::WaitForError, aMessage %d", aError );
    
    iWaitedErrorCodeArrived = EFalse;
    iWaitedErrorCode = aError;
    
    After( KDefaultMsgWaitingTimeout ); // Start timeout 
    iWait->Start();
    Cancel(); // Cancel the timer.
    
    if( !iWaitedErrorCodeArrived )
        {
        TEFLOGSTRING2( KTAREQEND, 
           "CPEMessageWaiter::WaitForError, waited for Error %d, and Error did not arrive in timeout time", iWaitedMsg );
        EUNIT_FAIL_TEST( "TIMEOUT: Did not receive waited PE Error");      
        }
    }

void CPEMessageWaiter::HandleError( 
    const TPEErrorInfo& aErrorInfo )
    {
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::HandleMessage, aMessage %d", aErrorInfo.iErrorCode );
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::WaitForMsg, waited msg %d", iWaitedErrorCode );
    
    iLastErrorCode = aErrorInfo.iErrorCode;
    
    if( iWait->IsStarted() )
        {
        if( aErrorInfo.iErrorCode == iWaitedErrorCode && aErrorInfo.iErrorCode  )
            {
            TEFLOGSTRING( KTAREQEND, 
                "CPEMessageWaiter::HandleError: Waited ErrorCode received );");
            iWaitedErrorCodeArrived = ETrue;
            iWait->AsyncStop();
            }
        }
    }

void CPEMessageWaiter::HandleMessage( 
    const TInt aMessage, 
    const TInt /*aCallId*/ )
    {
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::HandleMessage, aMessage %d", aMessage );
    TEFLOGSTRING2( KTAREQEND, "CPEMessageWaiter::WaitForMsg, waited msg %d", iWaitedMsg );
    __ASSERT_ALWAYS( iArrivedMsgs.Append( static_cast<MEngineMonitor::TPEMessagesFromPhoneEngine>( aMessage ) ) == KErrNone, User::Panic( KPanic, EArrivedMessagesOOM ) );
    
    if( iWait->IsStarted() )
        {
        if( aMessage == iWaitedMsg )
            {
            TEFLOGSTRING( KTAREQEND, 
                "CPEMessageWaiter::HandleMessage: Waited message received");
            iWaitedMsgArrived = ETrue;
            iWait->AsyncStop();
            }
        }
    }

void CPEMessageWaiter::RunL()
    {
    if( iWait->IsStarted() )
        {
        iWait->AsyncStop();
        }     
    }

TInt CPEMessageWaiter::LastErrorCode()
    {
    return iLastErrorCode;
    }

