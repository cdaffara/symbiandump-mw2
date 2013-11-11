/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  LBT Server session class defination
*
*/


#include <lbterrors.h>

#include "lbtsession.h"
#include "lbtsubsession.h"
#include "lbtserverconsts.h"
#include "lbtsubsessionregistry.h"
#include "lbtserverlogiccontainer.h"
#include "lbterrors.h"
#include "lbtserverlogicbase.h"
#include "lbtservercore.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSession::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtSession* CLbtSession::NewL(CLbtServerLogicContainer& aContainer, const TSecureId& aSecureId)
	{
	FUNC_ENTER("CLbtSession::NewL");		
	CLbtSession* self = new ( ELeave ) CLbtSession(aContainer, aSecureId);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtSession::CLbtSession
// C++ default constructor
// ---------------------------------------------------------------------------
//
CLbtSession::CLbtSession(
    CLbtServerLogicContainer& aContainer,
    const TSecureId& aSecureId) : 
	iContainer(aContainer),
	iSecureId(aSecureId)
	{
	}

// ---------------------------------------------------------------------------
// CLbtSession::~CLbtSession
// Destructor
// ---------------------------------------------------------------------------
//
CLbtSession::~CLbtSession()
	{
	FUNC_ENTER("CLbtSession::~CLbtSession");
	
	if( iSubSessionRegistry )
	    {
	    for(TInt i=0;i<iSubSessionRegistry->Count();++i)
    	    {
    	    TInt handle = iSubSessionRegistry->GetSubSessionHandle(i);
    	    CLbtServerLogicBase* serverLogic = iContainer.ServerLogic();
    		if(serverLogic)
    			{
    			serverLogic->HandleSubSessionClosure(handle, this);
    			}
    	    }
	    }
		
	if(CheckIfOnlySessionOfClient())
		{
		CLbtServerLogicBase* serverLogic = iContainer.ServerLogic();
		if(serverLogic)
			{
			TRAP_IGNORE(serverLogic->HandleSessionClosureL(iSecureId));
			}
		}
	delete iSubSessionRegistry;
	}

// ---------------------------------------------------------------------------
// CLbtSession::ConstructL()
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void CLbtSession::ConstructL()
	{
	FUNC_ENTER("CLbtSession::ConstructL");	
	iSubSessionRegistry = CLbtSubSessionRegistry::NewL();
	}

// ---------------------------------------------------------------------------
// CLbtSession::ServiceL
// ---------------------------------------------------------------------------
//
void CLbtSession::ServiceL(const RMessage2 &aMessage)
	{
	FUNC_ENTER("CLbtSession::ServiceL");	
	switch(aMessage.Function())
		{
		case ELbtSubSessionOpen:
		case ELbtMgmtSubSessionOpen:
				{
				CreateSubSessionL(aMessage);				
				break;
				}
		case ELbtSubSessionClose:
				{
				// First free all resources like IPC messages etc from server logic
				CLbtServerLogicBase* serverLogic = iContainer.ServerLogic();
				if(serverLogic)
					{
					serverLogic->HandleSubSessionClosure(aMessage.Int3(), this);
					}			    
			    
			    // Close the sub-session and free the sub-session object
			    TInt retVal = iSubSessionRegistry->CloseSubSession(aMessage.Int3());
			    RequestComplete(aMessage, retVal);
				break;
				}
		default: // Forward other messages to the sub-session
				{
				ForwardToSubSessionL(aMessage);
				break;
				}
		}
	}


// ---------------------------------------------------------------------------
// CLbtSession::ServiceError
// ---------------------------------------------------------------------------
//
void CLbtSession::ServiceError(const RMessage2& aMessage,TInt aError)
    {
    FUNC_ENTER("CLbtSession::ServiceError");	
    if ( aError == KErrBadDescriptor )
        {
        // In case the error is bad descriptor then panic the client with this
        // error code. Otherwise invoke the base class implementation.
        aMessage.Panic(KLbtClientPanicCategory, KErrBadDescriptor);
        }
    else
        {
        CSession2::ServiceError( aMessage, aError );
        }
    }


// ---------------------------------------------------------------------------
// CLbtSession::SecureId
// ---------------------------------------------------------------------------
//
TSecureId CLbtSession::SecureId()
	{
	return iSecureId;
	}
	
// ---------------------------------------------------------------------------
// CLbtSession::CreateSubSessionL
// ---------------------------------------------------------------------------
//
void CLbtSession::CreateSubSessionL(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtSession::CreateSubSessionL");	
	CLbtSubSession::TSubSessionType type;
	if(aMessage.Function() == ELbtMgmtSubSessionOpen)
		{
		type = CLbtSubSession::TLbtManagementLibrary;
		}
	else
		{
		type = CLbtSubSession::TLbtClientLibrary;
		}
	CLbtSubSession* subSession = CLbtSubSession::NewLC(iContainer, type);
	// If the NewLC cannot allocate then it leaves with KErrMemory. In such a
	// scenario the ServiceError is invoked which responds to the IPC message
	// with the error code. Hence no special handling is required for
	// KErrNoMemory here.

    TInt subSessionHandle = iSubSessionRegistry->AddSubSessionL(subSession);
    CleanupStack::Pop(subSession); // subSession ownership taken by registry
   
    // Set the client subsession identifier.    
    TPckg<TInt> handlePackage(subSessionHandle);    
    TInt retVal = aMessage.Write(KParamSubsessionHandle, handlePackage);

    // The error code from Write has to be handled as below,
	// If the retVal is an error code then 
	//      close the open subsession first.
    //      If error is KErrBadDescriptor then panic the client
    //      else complete the message with the error code.
    // else complete the message with KErrNone.
    if ( retVal != KErrNone )
    	{
    	// close the open subsession first.
        iSubSessionRegistry->CloseSubSession(subSessionHandle);    	
    	if ( retVal == KErrBadDescriptor )
    	    {
            aMessage.Panic(KLbtClientPanicCategory, KErrBadDescriptor);
    	    }
    	else
    	    {
        	RequestComplete(aMessage, retVal);
    	    }
    	}
    else
        {
    	RequestComplete(aMessage, KErrNone);
        }
	}

// ---------------------------------------------------------------------------
// CLbtSession::ForwardToSubSessionL
// ---------------------------------------------------------------------------
//
inline void CLbtSession::ForwardToSubSessionL(const RMessage2& aMessage)
	{
	FUNC_ENTER("CLbtSession::ForwardToSubSessionL");	
	CLbtSubSession* subSession = 
			iSubSessionRegistry->SubSessionFromHandle(aMessage.Int3());
			
    if (!subSession)
		{
		// Panic the client since the sub-session handle is not 
		// present in the registry
		aMessage.Panic(KLbtClientPanicCategory, ELbtServerBadHandle);              
        }
    else
        {
        subSession->ServiceL(aMessage);
        }
	}

// ---------------------------------------------------------------------------
// CLbtSession::CheckIfOnlySessionOfClient
// ---------------------------------------------------------------------------
//
TBool CLbtSession::CheckIfOnlySessionOfClient()
	{
	FUNC_ENTER("CLbtSession::CheckIfOnlySessionOfClient");	
	CLbtServerCore* server = static_cast <CLbtServerCore*> (const_cast<CServer2*>(Server()));
	if(server->NumberOfClientSessions(iSecureId) == 1)
		{
		// If this is the only session then return true
		return ETrue;
		}
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CLbtSession::RequestComplete
// ---------------------------------------------------------------------------
//
inline void CLbtSession::RequestComplete(const RMessage2& aMessage, TInt aCompleteCode)
	{
	FUNC_ENTER("CLbtSession::RequestComplete");	
	if (!aMessage.IsNull())
        {
        aMessage.Complete(aCompleteCode);
        }
	}

void CLbtSession::Disconnect(const RMessage2& aMessage)
    {
    FUNC_ENTER("CLbtSession::Disconnect");	
    CSession2::Disconnect( aMessage );
    }

// end of file
