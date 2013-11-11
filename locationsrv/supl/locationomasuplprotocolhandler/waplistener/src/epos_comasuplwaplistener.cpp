/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of OMA SUPL WAP Listener
*
*/



#include <utf.h>
#include <push/ccontenthandlerbase.h>
//#include <push/pushdispatcher.h> for header relocation error

#include "epos_comasuplwaplistener.h"

_LIT( KReserved, "Reserved" );
_LIT( KPushMsgNull, "NULL PUSH message" );
_LIT( KTraceFileName, "epos_suplwaplistener.cpp" );
_LIT( KSupportedContentType, "application/vnd.omaloc-supl-init");
_LIT( KSupportedBinaryContentType, "0x312");
_LIT( KSupportedBinaryContentType2, "0X312");

//const TUint8 KSupportedHEXContentType = 0x03020312;


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// COMASuplWapListener::COMASuplWapListener()
// C++ default constructor.
// -----------------------------------------------------------------------------
//
COMASuplWapListener::COMASuplWapListener()
    : CPushHandlerBase()
    {
    iErrCode = KErrNone;
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::NewL
// COMASuplWapListener instantiation method.
// -----------------------------------------------------------------------------
//
COMASuplWapListener* COMASuplWapListener::NewL()
    {
    COMASuplWapListener* self = new (ELeave) COMASuplWapListener();
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::ConstructL()
    {
    iTrace = COMASuplTrace::NewL();
    }	

// -----------------------------------------------------------------------------
// COMASuplWapListener::~COMASuplWapListener
// Default destructor.
// -----------------------------------------------------------------------------
//
COMASuplWapListener::~COMASuplWapListener()
    {
    	if(iTrace)
    		iTrace->Trace( _L( "COMASuplWapListener::~COMASuplWapListener()" ), KTraceFileName, __LINE__ );    
    delete iMessage;
    delete iTrace;
    Deque();
    }	

// -----------------------------------------------------------------------------
// COMASuplWapListener::CPushHandlerBase_Reserved1
// Reserved for ECOM for future expansion.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::CPushHandlerBase_Reserved1()
    {
    User::Panic( KReserved, KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::CPushHandlerBase_Reserved2
// Reserved for ECOM for future expansion.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::CPushHandlerBase_Reserved2()
    {
    User::Panic( KReserved, KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::HandleMessageL
// Handles a push message asynchronously.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::HandleMessageL( CPushMessage* aPushMsg, TRequestStatus& aStatus )
    {
    __ASSERT_ALWAYS( aPushMsg, User::Panic( KPushMsgNull, KErrUnknown ) );    
    iTrace->Trace( _L("COMASuplWapListener::HandleMessageL() Async Start"), KTraceFileName, __LINE__ );
    iAsyncHandling = ETrue;
    SetConfirmationStatus(aStatus);
    HandleMessageL(aPushMsg);
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::HandleMessageL
// Handles a push message synchronously.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::HandleMessageL( CPushMessage* aPushMsg )
    {
    //TBuf<64> contentType;
    __ASSERT_ALWAYS( aPushMsg, User::Panic( KPushMsgNull, KErrUnknown ) );
    iTrace->Trace( _L( "COMASuplWapListener::HandleMessageL() Sync Start" ), KTraceFileName, __LINE__ );
    iMessage = aPushMsg;
    TPtrC receivedContentType;
    iTrace->Trace( _L("Retriving Content Type:" ), KTraceFileName, __LINE__ );
    iMessage->GetContentType(receivedContentType);
    iTrace->Trace( _L("Received Content Type for WAP ..." ), KTraceFileName, __LINE__ );
    iTrace->Trace(receivedContentType, KTraceFileName, __LINE__ );
    
    if(receivedContentType.Compare(KSupportedContentType) == 0 || 
       receivedContentType.Compare(KSupportedBinaryContentType) == 0 || 
       receivedContentType.Compare(KSupportedBinaryContentType2) == 0 ) //can be combined...
    	{
    	    iState = EProcess;
    	}
	else if (CheckBinaryContentType(aPushMsg))
    	{
    	    iState = EProcess;
    	}
    else
    	{
    	iTrace->Trace( _L("ERROR:Received Content Type is not supported..." ), KTraceFileName, __LINE__ );
			iState = EComplete;
			iErrCode = KErrNotSupported;
    	}	
    IdleComplete();
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::CancelHandleMessage
// Cancels an outstanding HandleMessageL() request.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::CancelHandleMessage()
    {
    Term( KErrCancel );
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::DoCancel
// Inherited from CActive - called to cancel outanding events.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::DoCancel()
    {
    Term( KErrCancel );
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::RunL
// Inherited from CActive - called when object is active.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::RunL()
    {
    iTrace->Trace( _L( "COMASuplWapListener::RunL() Start" ), KTraceFileName, __LINE__ );

    switch ( iState )
        {
        case EProcess:
            ProcessPushMsgL();
            break;
    
        case EComplete:
            Term( iErrCode );
            break;
    
        default:
            break;
        }
}

// -----------------------------------------------------------------------------
// COMASuplWapListener::RunError
// Handles the event of leave from the request completion event handler RunL()
// -----------------------------------------------------------------------------
//
TInt COMASuplWapListener::RunError( TInt aError ) 
    {
    iState = EComplete;
    Term( aError );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::ProcessPushMsgL
// Extracts the SUPL INIT request from the WAP Push message. 
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::ProcessPushMsgL()
    {
    iTrace->Trace( _L( "COMASuplWapListener::ProcessPushMsgL() Start" ), KTraceFileName, __LINE__ );

    // Extract SUPL INIT request from the push message
    TPtrC8 msgPtr;
    iState = EComplete;
    TBool exist = iMessage->GetMessageBody( msgPtr );

    if (!exist)
        {
        iErrCode = KErrNotFound;
        IdleComplete();
        iTrace->Trace( _L( "Unable to extract SUPL payload" ), KTraceFileName, __LINE__ );
        return;
        }

    TRAP( iErrCode, SendL( msgPtr ) );
    IdleComplete();
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::SendL
// Creates an IPC connection with the SUPL gateway, forwards 
// the request and terminates the connection.
// -----------------------------------------------------------------------------
//	
void COMASuplWapListener::SendL( TPtrC8& aMessage )
    {
    iTrace->Trace( _L( "COMASuplWapListener::SendL() Start" ), KTraceFileName, __LINE__ );

    TInt err = KErrNone;
    
    // Create IPC session
    RSuplNetworkServer suplServ;
    err = suplServ.Connect();
    if ( KErrNone != err )
        {
        iTrace->Trace( _L( "Unable to create IPC session with SUPL server" ), KTraceFileName, __LINE__ );
        User::Leave(err);
        }

    // Forward SUPL payload to SUPL server
    err = suplServ.ForwardMessage( aMessage );
    suplServ.Close();
    if ( KErrNone != err )
        {
        iTrace->Trace( _L( "Error sending SUPL payload to server" ), KTraceFileName, __LINE__ );
        User::Leave( err );
        }	
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::Term
// Destroy self.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::Term( TInt aError )
    {
    iTrace->Trace( _L( "COMASuplWapListener::Term()" ), KTraceFileName, __LINE__ );
    if ( iAsyncHandling )
        {
        SignalConfirmationStatus( aError );
        }
    // Destroy self
    iPluginKiller->KillPushPlugin();
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::IdleComplete
// Completes the iRequestStatus with KErrNone.
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::IdleComplete()
    {
    TRequestStatus* pS = &iStatus;
    User::RequestComplete( pS, KErrNone );
    if ( !IsActive() )
        {
        SetActive();
        }
    }

// -----------------------------------------------------------------------------
// COMASuplWapListener::CheckBinaryContentType
// 
// -----------------------------------------------------------------------------
//
TBool COMASuplWapListener::CheckBinaryContentType(CPushMessage* aPushMsg)
{
	TPtrC8 field;
	TBool isHeaderPresent = aPushMsg->GetBinaryHeaderField(EHttpContentType, field);

#ifdef _DEBUG
	//Log the received message
    iTrace->Trace( _L( "Received Binary Content Type is:" ), KTraceFileName, __LINE__ );
    PrintHex(field, __LINE__);
#endif

	if( isHeaderPresent )
	{
		    iTrace->Trace( _L( "Binary Content type present..." ), KTraceFileName, __LINE__ );
			TUint8 code = field[0];
			
			//if(code>0x7F && ((code&0x7F) == KSupportedHEXContentType) )                 //      Content-type-value =  Short-integer
            if (field[1] == 0x03 && field[2] == 0x12)
			{
				return ETrue;
			}	
	}
	else
	{
		iTrace->Trace( _L( "No Binary Content type present..." ), KTraceFileName, __LINE__ );
	}
	return EFalse;
}

// -----------------------------------------------------------------------------
// COMASuplWapListener::PrintHex
// 
// -----------------------------------------------------------------------------
//
void COMASuplWapListener::PrintHex(const TDesC8& aBuffer,TInt aLine)
	{
			TBuf<256> buffer;
			TBuf<2> buff;
			_LIT16(KFormat1,"%02x");
			TInt len = aBuffer.Length();
			
			//The buffer is usually larger than what can be logged in a single line in the log file.  As such this should only attempt
			// to log 27 hex blocks to each log line.
			const TInt KNumberOfBlocks = 27;
			TInt blockNumber = KNumberOfBlocks;
			
			for(TInt i = 0 ; i <len; i++)
				{
					//Check to see if the buffer should be logged and then emptied
                    if(i == blockNumber)
                        {
                        //Trace the buffer as it currently is
                        iTrace->Trace(buffer, KTraceFileName, aLine);
                        //Reset the buffer for the next log line
                        buffer.Zero();
                        blockNumber += KNumberOfBlocks;
                        }
					
					buff.Zero();
					buff.Format(KFormat1,aBuffer[i]);
					buffer.Append(buff);	
					buffer.Append(_L(" "));	
				}

				iTrace->Trace(buffer, KTraceFileName, aLine); 											
	}
