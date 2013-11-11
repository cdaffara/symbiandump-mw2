// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//
    
// System Include
#include <wapmessage.h>
#include <wapmsgerr.h>
#include "CLWatcher.h"
#include <push/pushmessage.h>
#include "PushDispatcher.h"
#include <push/cpushhandlerbase.h>
#include <ecom/ecom.h>

// User Include
#include "CWapPushMessageFilter.h"
#include "CWapPushFilterUtils.h"
#include "CUriListLookup.h"


const TUid KUidWapUriLookup = {0x20009D3F};

/**
 * Constructor 
 * @param aLog a interface for run-time logging
 * @param aManager a interface for the connection manager
 */
CCLWatcherBase::CCLWatcherBase(MWapPushLog& aLog, MConnManObserver& aManager) :	
CActive(CActive::EPriorityStandard), 
	iState(EWaiting),
	iLog(aLog), 
	iManager(aManager),
	iCachedFilter(EFalse)
	{
	}

/**
 * Destructor
 */
CCLWatcherBase::~CCLWatcherBase()
	{
	Cancel();
	delete iWapCLServ;
	delete iHeaders;
	delete iBody;
	delete iCurrentMessage;
	delete iPushMsgFilter;
	delete iServerAddress;
	delete static_cast<CUriListLookup*>(iUriListLookup);
	}

/**
 * Construct the connectionless watcher base class
 * Add this object to the active scheduler and make it active. 
 * A wap stack session is created, construction of the connectionless 
 * connection is performed in the virtual SetupCLWatcherL() method. 
 * Concrete unsecure/secure watchers connect to different ports and require
 * different setup.
 */
void CCLWatcherBase::ConstructL()
	{
	CActiveScheduler::Add(this);
	iWapCLServ = CWapBoundCLPushService::NewL();
	SetupCLWatcherL();
	
	IdleComplete();
	}


/**
 * Asynchronous request complete
 * Utility to make a asynchronous request has been complete synchronously. 
 */
void CCLWatcherBase::IdleComplete()
	{
	if( !IsActive() )
		{
		TRequestStatus* pS=&iStatus;
		User::RequestComplete(pS,0);
		SetActive();
		}
	}

/**
 * If RunL() leaves, this gets called
 * @param aError Error passed into this function
 *
 * EWapErrPluginNotFound (-10018) indicates that the plug-in server was unable to
 * find the requested plug-in. 
*/
TInt CCLWatcherBase::RunError(TInt aError)
	{
	__LOG_DEBUG("CCLWatcherBase::RunError called");
	__LOG_ERROR_DEBUG("Error occurred", aError);
	//  Set to waiting state & reset...
 	iState = EWaiting;

	delete iHeaders;
	iHeaders = NULL;
	delete iBody;
	iBody = NULL;
	delete iCurrentMessage;
	iCurrentMessage = NULL;

	IdleComplete();
	aError = KErrNone;
	return(aError);
	}

/**
 * Handle the completion of a request that is active
 * 
 * CL Watchers are always running.  
 * States:
 * Waiting: Will call the aysnchronous call UnitWaitPush()
 * Receiving: Collects the Push Message
 * Filtering: Filters the Push Message
 * Dispatching: Dispatches the Push Message
 *
 * The EMadeRequest state is necessary to differentiate whether an async
 * request has been made (and therefore should be cancelled).
 */
void CCLWatcherBase::RunL()
	{
	switch (iState)
		{
		case EWaiting:
 			WaitForPushL();
			iState = EReceiving;
			break;
		case EReceiving:
			ReceivePushL();	
			break;
		case EFiltering:
			FilterMessageL(); 
			iState = EDispatching;
			break;
		case EDispatching:
			if (!iPassedFilter)
				{
				delete iCurrentMessage;
				iCurrentMessage = NULL;
				IdleComplete();
				}
			else	
				DispatchMessageL();
			iState = EWaiting;
			break;
		default:
			Panic(EUnknownState); 
			User::Panic(_L("WapPush ConnectionLess Watcher"), KErrNotSupported); 
			break;

		}
	}


/**
 * Cancel any asynchronous requests made by this active object
 * This shouldn't be called under normal circumstances. If it is called,
 * this means that we are no longer running and push messages are no longer
 * being collected.
 *
 * This is called when the testcode destroys the connection manager.
 */
void CCLWatcherBase::DoCancel()
	{
	switch (iState)
		{
	case EWaiting:
	case EReceiving:
		if (iWapCLServ && iStatus.Int() == KRequestPending)
			iWapCLServ->CancelAwaitPush();
			break;
	case EFiltering:
	case EDispatching:
		if (iStatus.Int() == KRequestPending && iPushMsgFilter)
			{
			iPushMsgFilter->CancelFilter();
			}
			break;
	default:
		break;
		}
	}



/**
 * Make an asynchronous request for a push message from the WAP Stack
 */
void CCLWatcherBase::WaitForPushL()
	{
	__LOG_DEBUG("CLWatcher: UnitWaitPush");
	User::LeaveIfError(iWapCLServ->AwaitPush(iHeadersBuf, iBodyBuf, iPushID, iStatus));
	SetActive();
	}

/**
 * Wrap the message up in a push message 
 * If there is more data to come, issue another request to run again 
 * to wait for the rest.
 * If it's all here, wrap the message up in a CPushMessage Object and set the state to EFiltering
 *
 */
void CCLWatcherBase::ReceivePushL()
	{
	__LOG_DEBUG("CLWatcher: ReceivePush");
	// If we're returned something unexpected, leave
	if (iStatus.Int() != Wap::EMoreData && iStatus.Int() != KErrNone)
		User::Leave(iStatus.Int());

	if ( (!iBody) && (!iHeaders) )
		{
		iBody = HBufC8::NewL(iBodyBuf.Length());
		iBody->Des().Copy(iBodyBuf);
		iBodyBuf.Zero();

		iHeaders = HBufC8::NewL(iHeadersBuf.Length());
		iHeaders->Des().Copy(iHeadersBuf);
		iHeadersBuf.Zero();
		}
	else 
		{
		if (iBody && iBodyBuf.Length())
			{
			iBody=iBody->ReAllocL(iBody->Length() + iBodyBuf.Length());
			iBody->Des().Append(iBodyBuf);
			iBodyBuf.Zero();
			}

		if (iHeaders && iHeadersBuf.Length())
			{
			iHeaders=iHeaders->ReAllocL(iHeaders->Length() + iHeadersBuf.Length());
			iHeaders->Des().Append(iHeadersBuf);
			iHeadersBuf.Zero();
			}
		}
	
	// Get the remote server address.
	if (!iServerAddress)
		{
		iWapCLServ->GetServerAddress(iServerAddress);
		}
	
	// we've got a complete message
	if (iStatus.Int() == KErrNone) 
		{
		TInt err = KErrNone; 
		if(!iUriListLookup)
			{
			//The Uri lookup plugin is not loaded. Load the plugin.
			TRAP(err, iUriListLookup = REINTERPRET_CAST(MUriListLookup*, REComSession::CreateImplementationL(KUidWapUriLookup, _FOFF(MUriListLookup,iEcomDtorID))));
			}
			if(err == KErrNone && iUriListLookup) // Plugin loading was successful
				{
				TPtrC8 ptr(iServerAddress->Des());
				InetUriList::TListType listType;
		
				TInt error = iUriListLookup->GetListType(ptr, listType);
			
				// Source Uri is in blacklist
				if(error != InetUriList::KErrUriNotFound && listType == InetUriList::EBlackList)
					{
					__LOG_DEBUG("Received a message from blacklisted source");
	
					delete iBody;
					iBody = NULL;
				
					delete iHeaders;
					iHeaders = NULL;
				
					delete iServerAddress;
					iServerAddress = NULL;
				
					iState = EWaiting;
								
					IdleComplete();
					return;
					}
						
				// Source URI is either in whitelist or unknown
				if(error == InetUriList::KErrUriNotFound || listType == InetUriList::EWhiteList)
					{
					iCurrentMessage = CPushMessage::NewL(iHeaders, iBody, iPushID(), iServerAddress);
				
					if(error != InetUriList::KErrUriNotFound && listType == InetUriList::EWhiteList)
						{
						__LOG_DEBUG("Received a message from a whitelisted source");
						iCurrentMessage->SetMessageAllowed(ETrue);
						}
					else if(error == InetUriList::KErrUriNotFound)
						{
						__LOG_DEBUG("Received a message from unknown source");
						iCurrentMessage->SetMessageAllowed(EFalse);	
						}
					}
				
				}
		
			else // Plugin loading was unsuccessful
				{
				// The URI should be sent as if it were a trusted one.
				iCurrentMessage = CPushMessage::NewL(iHeaders, iBody, iPushID(), iServerAddress);
				iCurrentMessage->SetMessageAllowed(ETrue);
				}
			
		// Push Message took ownership, so just NULL
		iHeaders = NULL;
		iBody = NULL;
		iServerAddress=NULL;
		__LOG_DEBUG("Received complete push message\n:");
		__LOG_MSG_DEBUG(*iCurrentMessage);
		}
		
	// Set next state
	if (iStatus.Int() == Wap::EMoreData)
		iState = EWaiting;
	else
		iState = EFiltering;
	
	IdleComplete();
	}


/** 
	Handles result from the filter plugin.
	Load a push message filter if one is available and hold
	If the filter plugin accepts the message then it is Dispatched. Otherwise the
	message is deleted.
	In either case the watcher will then start waiting for the next message
*/
void CCLWatcherBase::FilterMessageL()
	{
	// Cache Filter if it exists
	if (!iCachedFilter)
		{		
		TRAPD(err, iPushMsgFilter = CWapPushFilterUtils::GetFilterPluginL()); 
		if (err && err != EWapErrPluginNotFound && err != KEComErrNoInterfaceIdentified)
			User::Leave(err);
		iCachedFilter = ETrue;
		}

	if (iPushMsgFilter)
	// only try and filter a message if a filter plugin is available
		{
		iPassedFilter = EFalse;
		iPushMsgFilter->FilterMessage(*iCurrentMessage,iStatus,iPassedFilter);
		SetActive();
		}
	else 
		{
		iPassedFilter = ETrue;
		IdleComplete();
		}
	}

/** 
	Dispatches the current message being processed and then sets the state to
	start waiting for the next message.
*/
void CCLWatcherBase::DispatchMessageL()
	{
	__ASSERT_ALWAYS(iCurrentMessage, Panic(ENoMessageExists));
	TPtrC8 rAppURI;
	TInt rAppID;
	TBool rIsAnInt;
	CPushHandlerBase* appHandlerPtr=NULL;

	if(iCurrentMessage->GetAppIdL(rAppURI, rAppID, rIsAnInt))
		{
		if(rIsAnInt)
			{
			CPushHandlerBase& appHandler = PushAppDispatcher::GetHandlerL(rAppID, iLog, iManager);
			appHandlerPtr= &appHandler;
			}
		else
			{
			CPushHandlerBase& appHandler = PushAppDispatcher::GetHandlerL(rAppURI, iLog, iManager); 
			appHandlerPtr= &appHandler;
			}
		}
	else
		{	// If no AppID defined, use the default User Agent
			CPushHandlerBase& appHandler = PushAppDispatcher::GetHandlerL(KUserAgentAppHandler, iLog, iManager); 
			appHandlerPtr= &appHandler;
		}

	__ASSERT_DEBUG(appHandlerPtr!=NULL, User::Invariant());
	if (appHandlerPtr)
		{
		// Plugin (HandleMessageL call) will take complete ownership of the iCurrentMessage.
		// The iCurrentMessage needs to be set to NULL before calling HandleMessageL
		// otherwise a leave from HandleMessageL will cause it to delete twice, 
		// one in HandleMessageL and second time in RunError		
		CPushMessage* tmpCurrentMessage = iCurrentMessage;
		iCurrentMessage = NULL;
		appHandlerPtr->HandleMessageL(tmpCurrentMessage);
		}
			
	IdleComplete();
	}


void CCLWatcherBase::Panic(TCLWatcherBasePanic aPanicCode)
	{
	_LIT(KCLWatcherBase,"CLWatcherBase");
	User::Panic(KCLWatcherBase,aPanicCode);
	}

/**
 * Factory construction method
 * Use this method to allocate and construct a new CCLUnsecureWatcher object
 * @param aLog a interface for run-time logging
 * @param aManager a interface for the connection manager
 */
CCLUnsecureWatcher* CCLUnsecureWatcher::NewL(MWapPushLog& aLog, MConnManObserver& aManager)
	{
	CCLUnsecureWatcher* self = new (ELeave) CCLUnsecureWatcher(aLog, aManager);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); //self
	return self;
	}

/**
 * Connect to the Unsecure WAP Push port
 */
void CCLUnsecureWatcher::SetupCLWatcherL()
	{
	User::LeaveIfError(iWapCLServ->Connect(Wap::EAll, KPushPortUnsecure, EFalse));
	}


/**
 * Desctructor
 */
CCLUnsecureWatcher::~CCLUnsecureWatcher()
	{
	}



/**
 * Factory construction method
 * Use this method to allocate and construct a new CCLSecureWatcher object
 * @param aLog a interface for run-time logging
 * @param aManager a interface for the connection manager
 */
CCLSecureWatcher* CCLSecureWatcher::NewL(MWapPushLog& aLog, MConnManObserver& aManager)
	{
	CCLSecureWatcher* self = new (ELeave) CCLSecureWatcher(aLog, aManager);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CCLSecureWatcher::~CCLSecureWatcher()
	{

	}

/**
 * Opens the Connectionless session on the stack.  For security, a certificate is not
 * negotiated with the Gateway.  A message will come in, it will be encrypted, the stack
 * decrypts based on the headers in the message.
 */
void CCLSecureWatcher::SetupCLWatcherL()
	{
	User::LeaveIfError(iWapCLServ->Connect(Wap::EAll,KPushPortSecure, ETrue));		
	}	




