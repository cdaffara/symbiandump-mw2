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
* Description: 
*     Main class
*
*/


#include "AlwaysOnlineManagerServerSession.h"
#include "AlwaysOnlineManagerServer.h"

_LIT( KTxtServer, "AlwaysOnlineManagerServerSession" );

//----------------------------------
// CAlwaysOnlineManagerServerSession()
//----------------------------------
CAlwaysOnlineManagerServerSession::CAlwaysOnlineManagerServerSession()
: CSession2()
    {
    }

//----------------------------------
// NewL()
//----------------------------------
CAlwaysOnlineManagerServerSession* CAlwaysOnlineManagerServerSession::NewL( 
	CAlwaysOnlineManagerServer * aServer )
	{
	CAlwaysOnlineManagerServerSession* self = 
	        new(ELeave) CAlwaysOnlineManagerServerSession();
	CleanupStack::PushL( self );
	self->ConstructL( aServer );
	CleanupStack::Pop( self );
	return self;
	}

//----------------------------------
// ConstructL()
//----------------------------------
void CAlwaysOnlineManagerServerSession::ConstructL( CAlwaysOnlineManagerServer* aServer )
    {
	iAOMServer = aServer;
    }

//----------------------------------
// ~CAlwaysOnlineManagerServerSession()
//----------------------------------
CAlwaysOnlineManagerServerSession::~CAlwaysOnlineManagerServerSession()
    {
    }

//----------------------------------
// ServiceL()
//----------------------------------
void CAlwaysOnlineManagerServerSession::ServiceL(const RMessage2& aMessage)
    {
    TRAPD(err,DispatchMessageL(aMessage));
    aMessage.Complete(err);
	
    }

//----------------------------------
// DispatchMessageL()
//----------------------------------
void CAlwaysOnlineManagerServerSession::DispatchMessageL( 
    const RMessage2& aMessage )
    {
    //read the used command
    TAlwaysOnlineServerAPICommands command = 
        static_cast<TAlwaysOnlineServerAPICommands>( aMessage.Function() );

    switch ( command )
        {
        case EServerAPIEmailEMNReceived:
            {
            TPckgBuf<TEMNElement> buf;
            aMessage.ReadL( 0, buf );
            // Forward command to client
            iAOMServer->HandleClientCommandL( command, buf );
            }
            break;
        case EServerAPIEmailQueryState:
        case EServerAPIEmailSessionClosed:
            {
            TPckgBuf<RMessage2> buf(aMessage);
            iAOMServer->HandleClientCommandL( command, buf );
            }
            break;
        case EServerAPIBaseCommandStart:
        case EServerAPIBaseCommandStop:
            {
            TPckgBuf<TUid> buf;
            aMessage.ReadL( 0, buf );
            // Forward command to client
            iAOMServer->HandleClientCommandL( command, buf );
            }
            break;
            
        default:// All other commands handles only TMsvId
            {
            TPckgBuf<TMsvId> buf;
            aMessage.ReadL( 0, buf );
            // Forward command to client
            iAOMServer->HandleClientCommandL( command, buf );
            }
            break;
        }
    }

//----------------------------------
// PanicClient()
//----------------------------------
void CAlwaysOnlineManagerServerSession::PanicClient( const RMessage2& aMessage, TInt aPanic ) const
    {
    aMessage.Panic( KTxtServer, aPanic );
    }
