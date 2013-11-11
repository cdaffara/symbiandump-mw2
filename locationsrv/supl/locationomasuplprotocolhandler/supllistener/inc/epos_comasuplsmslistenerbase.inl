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
* Description:   Implementation of inline methods
*
*/



#include <smsuaddr.h>
#include <epos_suplnetwork.h>

#include "epos_comasuplsmslistenerbase.h"
#include "epos_comasupltrace.h"

_LIT( KTraceFileName, "epos_comasuplsmslistenerbase.inl" );
const TUint KSocketServConnections = 1;
const TInt KMaxErrSize = 8;

inline COMASuplSmsListenerBase::~COMASuplSmsListenerBase()
    {
    Deque();
    iSocket.Close();
    iSocketServer.Close();
    iFs.Close();
    }


inline COMASuplSmsListenerBase::COMASuplSmsListenerBase()
: CActive(EPriorityStandard)
    {
    CActiveScheduler::Add( this );
    }
	

inline void COMASuplSmsListenerBase::ListenForSMSL()
    {
    iBuf() = KSockSelectRead;
    iSocket.Ioctl( KIOctlSelect, iStatus, &iBuf, KSOLSocket );
	iState = EListening;
	SetActive();
    }
    

// ---------------------------------------------------------------------------
// Forwards message to SUPL gateway
// ---------------------------------------------------------------------------
//  
inline void COMASuplSmsListenerBase::SendL( const TPtr8& aMessage )
    {
    Trace( _L("COMASuplSmsListenerBase::SendL() Start"), 
                                            KTraceFileName, __LINE__ );
    TInt err = KErrNone;
    
    // Create IPC session
    RSuplNetworkServer suplServ;
    err = suplServ.Connect();
    if ( KErrNone != err )
        {
        // LOG ERROR
        Trace( _L( "Unable to create IPC session with SUPL server" ), 
                                KTraceFileName, __LINE__ );
        // END
        
        User::Leave( err );
        }

    // Forward SUPL payload to SUPL server
    err = suplServ.ForwardMessage( aMessage );

    suplServ.Close();

    if ( KErrNone != err )
        {
        // ERROR
        Trace( _L( "Error sending SUPL payload to server" ), 
                                KTraceFileName, __LINE__ );
        // END
        
        User::Leave( err );
        }	
    }


// ---------------------------------------------------------------------------
// Cancels any outstanding requests
// ---------------------------------------------------------------------------
//
inline void COMASuplSmsListenerBase::DoCancel()
    {
    iSocket.CancelIoctl();    
    }
    

// ---------------------------------------------------------------------------
// Request handling function
// ---------------------------------------------------------------------------
//
inline void COMASuplSmsListenerBase::RunL()
    {
    switch ( iState )
        {
        case EListening:
            if ( KErrNone != iStatus.Int() )
                {
                // ERROR
                Trace( _L( "Error receiving SMS message" ), 
                                                    KTraceFileName, __LINE__ );
                TBuf<KMaxErrSize> buf;
                buf.AppendNum( iStatus.Int() );
                Trace( buf, KTraceFileName, __LINE__ );
                // END
                
                ListenForSMSL();
                break;
                }
            TRAPD( err, ProcessMessageL() );
            if ( KErrNone != err )
                {
                // ERROR
                Trace( _L( "Error processing message" ), 
                                                    KTraceFileName, __LINE__ );
                TBuf<KMaxErrSize> buf;
                buf.AppendNum( err );
                Trace( buf, KTraceFileName, __LINE__ );
                // END
                }
            iSocket.Ioctl( KIoctlReadMessageSucceeded, iStatus, NULL, 
                            KSolSmsProv );
            iState = EAcknowledging;
            SetActive();
            break;
            
        case EAcknowledging:
            ListenForSMSL();
            break;
            
        default:
            User::Panic( _L( "Invalid state" ), KErrUnknown );
        }
    }
    

// ---------------------------------------------------------------------------
// Wrapper function used for tracing
// ---------------------------------------------------------------------------
//   
inline void COMASuplSmsListenerBase::Trace( const TDesC& aMessage, const TDesC& aFileName, TInt line )
    {
    COMASuplTrace* trace = NULL;
    TRAPD( err, trace = COMASuplTrace::NewL() );
    if ( KErrNone == err )
        {
        trace->Trace( aMessage, aFileName, line );
        delete trace;
        }
    }
