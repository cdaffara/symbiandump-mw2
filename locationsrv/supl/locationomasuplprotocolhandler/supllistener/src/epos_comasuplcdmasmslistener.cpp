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
* Description:   Implementation of CDMA SMS Listener
*
*/



#include <utf.h>
#include <smsustrm.h>
#include <cdmaucommon.h>
#include <cdmasmsmessage.h>

#include "epos_comasuplcdmasmslistener.h"
#include "epos_comasuplimsiobserver.h"

_LIT( KCdmaSmsListener, "epos_comasuplcdmasmslistener.cpp" );

using namespace tia637;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplSmsListenerBase* COMASuplCdmaSmsListener::NewL()
    {
    COMASuplCdmaSmsListener* self = new ( ELeave ) COMASuplCdmaSmsListener;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplCdmaSmsListener::~COMASuplCdmaSmsListener() 
    {
    delete iSuplImsiObserver;
    }


// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
COMASuplCdmaSmsListener::COMASuplCdmaSmsListener() : COMASuplSmsListenerBase()
    {
    }


// ---------------------------------------------------------------------------
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void COMASuplCdmaSmsListener::ConstructL()
    {
    TRAPD(err, iSuplImsiObserver = COMASuplIMSIObserver::NewL() );
    if ( KErrNone != err )
        {
        Trace( _L( "Error Initializing IMSI Observer" ), 
                                              KCdmaSmsListener, __LINE__ );
        }
    
    // Connect to socket server
    User::LeaveIfError( iSocketServer.Connect( KSocketServConnections ) );

    // Connect to file system
    User::LeaveIfError( iFs.Connect() );

    // Create a socket
    User::LeaveIfError( iSocket.Open( iSocketServer, KCdmaSMSAddrFamily, 
                        KSockDatagram, KCdmaSMSDatagramProtocol ) );
    
    // Set socket address family
    InitializeL();	
    iState = EIdle;
    }


// ---------------------------------------------------------------------------
// Binds the socket to a specific address family
// ---------------------------------------------------------------------------
//
void COMASuplCdmaSmsListener::InitializeL()
    {
    Trace( _L( "InitializeL() Start" ), KCdmaSmsListener, __LINE__ );
    
    TCdmaSmsAddr address;
    address.SetCdmaSmsAddrFamily( ECdmaSmsAddrWdp );
    address.SetPort( KWDPPort );
    TInt err = iSocket.Bind( address );
    
    User::LeaveIfError( err );
    }


// ---------------------------------------------------------------------------
// Extracts the payload from the SMS message
// ---------------------------------------------------------------------------
//
void COMASuplCdmaSmsListener::ProcessMessageL()
    {
    Trace( _L( "ProcessMessageL() Start" ), KCdmaSmsListener, __LINE__ );
    
    CCdmaSmsBuffer* smsBuffer = CCdmaSmsBuffer::NewL();
    CCdmaSmsMessage* smsMessage = CCdmaSmsMessage::NewL( iFs, smsBuffer );
    CleanupStack::PushL( smsMessage );
    
    RSmsSocketReadStream readStream( iSocket );
    readStream >> *smsMessage;
    readStream.Close();    
    
    TUint smsLen = smsBuffer->Length();
    HBufC* dgram = HBufC::NewLC( smsLen );
    TPtr ptr = dgram->Des();
    smsBuffer->ExtractL( ptr, 0, smsLen );

    HBufC8* suplInfo = HBufC8::NewLC( smsLen * 2 );
    suplInfo->Des().Copy( ptr );
    
    // Forward message to SUPL gateway
    TRAPD( err, SendL( suplInfo->Des() ) );
    if ( KErrNone != err )
        {
        Trace( _L( "Error forwarding message to SUPL gateway" ), 
                            KCdmaSmsListener, __LINE__ );
        User::Leave( err );
        }    

	if ( iSuplImsiObserver && EComplete == iSuplImsiObserver->State() )
	    {
	    delete iSuplImsiObserver;
	    iSuplImsiObserver = NULL;
        Trace( _L( "Deleted SUPL IMSI Observer" ), 
                          KCdmaSmsListener, __LINE__ );
	    }
	    
    CleanupStack::PopAndDestroy();  // suplInfo;
    CleanupStack::PopAndDestroy(); 	// dgram
    CleanupStack::PopAndDestroy(); 	// smsMessage & smsBuffer
    }
