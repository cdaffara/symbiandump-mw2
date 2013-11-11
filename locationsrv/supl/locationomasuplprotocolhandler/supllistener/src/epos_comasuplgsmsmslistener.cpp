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
* Description:   Implementation of GSM SMS Listener
*
*/



#include <utf.h>
#include <smsustrm.h>
#include <gsmubuf.h>
#include <gsmumsg.h>

#include "epos_comasuplgsmsmslistener.h"
#include "epos_comasuplimsiobserver.h"
#include "epos_comasuplproductconfig.h"

_LIT( KGsmSmsListener, "epos_comasuplgsmsmslistener.cpp" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplSmsListenerBase* COMASuplGsmSmsListener::NewL()
    {
    
    COMASuplGsmSmsListener* self = new ( ELeave ) COMASuplGsmSmsListener;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplGsmSmsListener::~COMASuplGsmSmsListener() 
    {
    delete iSuplImsiObserver;
    if (iProductConfig)
	delete iProductConfig;
    iProductConfig = NULL;
    }


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplGsmSmsListener::COMASuplGsmSmsListener() : COMASuplSmsListenerBase()
    {
    }


// ---------------------------------------------------------------------------
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void COMASuplGsmSmsListener::ConstructL()
    {
    Trace( _L( "COMASuplGsmSmsListener::ConstructL()" ), KGsmSmsListener, __LINE__ );

    TRAPD(err, iSuplImsiObserver = COMASuplIMSIObserver::NewL(*this) );
    if ( KErrNone != err )
        {
        // ERROR
        Trace( _L( "Error Initializing IMSI Observer" ), 
                                              KGsmSmsListener, __LINE__ );
        // END
        }
    
    Trace( _L( "Creating Product Config Object" ), KGsmSmsListener, __LINE__ );

    TRAPD(err1, iProductConfig = COMASuplProductConfig::NewL(*this) );
    if ( KErrNone != err1 )
        {
        // ERROR
        Trace( _L( "Error Initializing Product Configuration" ), 
                KGsmSmsListener, __LINE__ );
        // END
        }

if(iProductConfig)    
{
    iProductConfig->CheckForBootCompletionL();
}
    
    User::LeaveIfError( iSocketServer.Connect( KSocketServConnections ) );
    
    User::LeaveIfError( iFs.Connect() );

    User::LeaveIfError( iSocket.Open( iSocketServer, KSMSAddrFamily, 
                            KSockDatagram, KSMSDatagramProtocol ) );
    
        

    InitializeL();	
    iState = EIdle;
    }


// ---------------------------------------------------------------------------
// Binds the socket to a specific address family
// ---------------------------------------------------------------------------
//
void COMASuplGsmSmsListener::InitializeL()
    {
    Trace( _L( "InitializeL() Start" ), KGsmSmsListener, __LINE__ );
    
    TSmsAddr smsAddr;
    smsAddr.SetSmsAddrFamily( ESmsAddrApplication16BitPort );
    smsAddr.SetPort( KWDPPort );
    TInt err = iSocket.Bind( smsAddr );
    
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// Extracts the payload from the received SMS message
// ---------------------------------------------------------------------------
//
void COMASuplGsmSmsListener::ProcessMessageL()
    {
    Trace( _L( "ProcessMessageL() Start" ), KGsmSmsListener, __LINE__ );
    
    CSmsBuffer* smsBuffer = CSmsBuffer::NewL();
    CSmsMessage* smsMessage = CSmsMessage::NewL( iFs, CSmsPDU::ESmsDeliver, 
                                                smsBuffer );
    CleanupStack::PushL( smsMessage );
    
    RSmsSocketReadStream readStream( iSocket );
    readStream >> *smsMessage;
    readStream.Close();
    
    TUint smsLen = smsBuffer->Length();
    HBufC* dgram = HBufC::NewLC( smsLen );
    TPtr ptr = dgram->Des();
    smsBuffer->Extract( ptr, 0, smsLen );
        
    HBufC8* suplInfo = HBufC8::NewLC( smsLen * 2 );
    suplInfo->Des().Copy( ptr );
    
    // Forward message to SUPL gateway
    TRAPD( err, SendL( suplInfo->Des() ) );
    if ( KErrNone != err )
        {
        Trace( _L( "Error forwarding message to SUPL gateway" ), 
                            KGsmSmsListener, __LINE__ );
        User::Leave( err );
        }
    
	if ( iSuplImsiObserver && EComplete == iSuplImsiObserver->State() )
	    {
	    delete iSuplImsiObserver;
	    iSuplImsiObserver = NULL;
        Trace( _L( "Deleted SUPL IMSI Observer" ), 
                          KGsmSmsListener, __LINE__ );
	    }
	        
    CleanupStack::PopAndDestroy();  // suplInfo;
    CleanupStack::PopAndDestroy(); 	// dgram
    CleanupStack::PopAndDestroy(); 	// smsMessage & smsBuffer
    }

// ---------------------------------------------------------------------------
// Handles the notification of completion of Product configuration
// ---------------------------------------------------------------------------
//
void COMASuplGsmSmsListener::HandleProductConfigCompleteL()
    {
    if (iProductConfig)
        {
        delete iProductConfig;
        iProductConfig = NULL;
        }
    }
// ---------------------------------------------------------------------------
// Handles the notification of completion of Imsi Observer
// ---------------------------------------------------------------------------
//
void COMASuplGsmSmsListener::HandleImsiObserverCompleteL()
    {
    if (iSuplImsiObserver)
        {
        delete iSuplImsiObserver;
        iSuplImsiObserver = NULL;
        }
    }
