// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendasserver.h"

#include <ecom/ecom.h>
#include <barsc.h> 
#include <barsread.h>

#include "csendassession.h"
#include "csendasservertimer.h"
#include "sendasserverdefs.h"
#include "sendasservername.h"
#include "csendasactivecontainer.h"
#include "csendasmtmmanager.h"

// time the server will stay alive after the last session closes.
const TInt KSendAsServerCloseTime 		= 2000000;  //  2 seconds

// time the server will stay alive between starting and the first connect
// operation arriving.
const TInt KSendAsServerInitCloseTime 	= 10000000; // 10 seconds

// location of the server resource file and resource struct version number
_LIT(KSendAsServerResourceFile,"Z:\\resource\\messaging\\sendasserver.rsc");
const TInt KResVersion = 0x00000001;

/** Factory function to create a new instance of the SendAs server.

@return
A new CSendAsServer object.  This object is left on the cleanup stack.
*/
CSendAsServer* CSendAsServer::NewLC()
	{
	CSendAsServer* self = new (ELeave) CSendAsServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}
	
/** Second-stage constructor.

Here the server allocates the 'object container index' object that is used to
keep track of the sessions.
*/
void CSendAsServer::ConstructL()
	{
	iMsvSession = CMsvSession::OpenSyncL(*this);
	iContainerIndex = CObjectConIx::NewL();
	
	iMtmManager = CSendAsMtmManager::NewL(*this);

	// read UIDs for edit tools and confirmation notifier from file
	ReadResourceFileL();	

	// configure the close timer to shut us down if there are no sessions
	// connected in the first 10 seconds.
	iCloseTimer = CSendAsServerTimer::NewL();
	iCloseTimer->After(KSendAsServerInitCloseTime);
	
	iActiveContainer = CSendAsActiveContainer::NewL(*this);
	
	StartL(KSendAsServerName);
	}

CSendAsServer::CSendAsServer()
: CServer2(CActive::EPriorityStandard)
	{
	}

CSendAsServer::~CSendAsServer()
	{
	delete iCloseTimer;
	delete iContainerIndex;
	delete iActiveContainer;
	delete iMtmManager;	
	delete iMsvSession;

	REComSession::FinalClose();
	}
	
/** Create a new session.

Called inside the NewSessionL callback.  This creates a new session
using the factory function.  The new session will call back into the server to
request an object container from the index during the operation of this factory function.
*/
CSession2* CSendAsServer::DoNewSessionL(const TVersion& aVersion, const RMessage2& /*aMsg*/)
	{
	// stop the delayed startup shutdown mechanism
	iCloseTimer->Cancel();
	
	CSendAsSession* session = CSendAsSession::NewL(aVersion, *this);
	return session;
	}
	
/** Create a new session.

The callback called by the server framework when a client calls Connect on an RSession-derived
object.
In the SendAs Server, a server-side CSendAsSession represents an RSendAs client-side object.
*/
CSession2* CSendAsServer::NewSessionL(const TVersion& aVersion, const RMessage2& aMsg) const
	{
	return ((CSendAsServer*)this)->DoNewSessionL(aVersion, aMsg);
	}

/** Generate a new object container for the session.

(Callback from session creation factory function).
Server remembers how many sessions are open.
*/
CObjectCon* CSendAsServer::NewContainerL()
	{
	CObjectCon* con = iContainerIndex->CreateL();
	++iOpenSessions;
	return con;
	}

/** Indication of session closing down.

Called from the session destructor to allow the server to dereference the
object container for that session.
*/
void CSendAsServer::SessionClosed(CObjectCon* aSessionContainer)
	{
	if( aSessionContainer != NULL)
		{
		iContainerIndex->Remove(aSessionContainer);
		--iOpenSessions;
		if( iOpenSessions == 0 )
			{
			if ( iActiveContainer->IsEmpty() )
				{
				iCloseTimer->After(KSendAsServerCloseTime);
				}
			else
				{
				// ensure no orphaned entries in background task container
				iActiveContainer->PurgeInactive();
				}
			}
		}
	}

CSendAsActiveContainer& CSendAsServer::ActiveContainer()
	{
	return *iActiveContainer;
	}

void CSendAsServer::ContainerEmpty()
	{
	// if no open sessions, start server shutdown timer
	if( iOpenSessions == 0 )
		{
		iCloseTimer->After(KSendAsServerCloseTime);
		}
	}

void CSendAsServer::ReadResourceFileL()
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
		
	RResourceFile file;
    file.OpenL(fs, KSendAsServerResourceFile);
    
    CleanupClosePushL(file);

	// Read the resource
	HBufC8* resBuf = file.AllocReadLC(1);

	// interpret the resource buffer
	TResourceReader reader;
	reader.SetBuffer(resBuf);
	
	// Read Edit Tools plugin UID and SendAs Confirm Notifier UID
	TInt resVersion     = reader.ReadInt32();
	iEditUtilsPluginUid = TUid::Uid(reader.ReadInt32());
	iNotifierUid        = TUid::Uid(reader.ReadInt32());
	
	if (resVersion != KResVersion || iEditUtilsPluginUid == KNullUid || iNotifierUid == KNullUid)
		{
		User::Leave(KErrCorrupt);
		}

	CleanupStack::PopAndDestroy(3, &fs);	// resBuf, file, fs
	}

const TUid& CSendAsServer::NotifierUid() const
	{
	return iNotifierUid;
	}

const TUid& CSendAsServer::EditUtilsPluginUid() const
	{
	return iEditUtilsPluginUid;
	}

CMsvSession& CSendAsServer::GetMsvSessionL()
	{ 
	// if session has become lazy - create new session
	if (iMsvSession == NULL)
		{
		iMsvSession = CMsvSession::OpenSyncL(*this);
		}
	return *iMsvSession; 
	}

CSendAsMtmManager* CSendAsServer::GetMtmManager()
	{ 
	return iMtmManager;
	}

/** Message Server Session Observer Callback.

Used to keep track of the status of the message server.

@param	aEvent
The message server event to be handled.
*/
void CSendAsServer::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* /*aArg1*/, TAny* /*aArg2*/, TAny* /*aArg3*/)
	{
	switch(aEvent)
		{
	case EMsvServerTerminated:
	case EMsvCloseSession:
		// close session (be lazy)
		delete iMsvSession;
		iMsvSession = NULL;
		break;
	case EMsvMtmGroupDeInstalled:
	case EMsvMtmGroupInstalled:
		// handle mtm installation/uninstallation 
		HandleMtmChangeL(aEvent);
		break;
	case EMsvCorruptedIndexRebuilding:
	case EMsvCorruptedIndexRebuilt:
		// ignore
	case EMsvEntriesChanged:
	case EMsvEntriesCreated:
	case EMsvEntriesDeleted:
	case EMsvEntriesMoved:
		// don't care
	case EMsvGeneralError:
		// not used after v5
	case EMsvMediaAvailable:
	case EMsvMediaChanged:
	case EMsvMediaIncorrect:
	case EMsvMediaUnavailable:
		// not to worry - appropriate errors will filter back to client
	case EMsvServerReady:
	case EMsvServerFailedToStart:
		// shouldn't happen since session is synchronously opened
		break;	
	default:
		break;
		}
	}

void CSendAsServer::HandleMtmChangeL(TMsvSessionEvent& aEvent)
	{
	// update the MTM manager
	iMtmManager->RefreshMtmUidArrayL();
	
	// call each session's MTM Change handling method if a MTM has been de-installed
	if (aEvent == EMsvMtmGroupDeInstalled)
		{
		iSessionIter.SetToFirst();
		CSendAsSession* ses = (CSendAsSession*)&(*iSessionIter++);
		while (ses!=NULL)
			{
			ses->HandleMtmChange();
			ses = (CSendAsSession*)&(*(iSessionIter++));
			}
		}
	}
