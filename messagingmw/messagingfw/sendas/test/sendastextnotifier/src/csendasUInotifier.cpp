// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <ecom/ecom.h>
#include "csendasUInotifier.h"
#include <tmsvpackednotifierrequest.h>
#include <ecom/implementationproxy.h>

/* UID of the SendAs notifier channel to get the user's confirmation */
const TUid KSendAsNotifierPluginUid     = {0x10208C14};
const TUid KTechViewScreenOutputChannel = {0x10208C14};
const TInt KSendAsServerExeUid	= {0x10204290};


RDummyServSession::RDummyServSession()
    {
    }

static TInt StartDummyServer()
	{
	const TUidType serverUid(KNullUid, TUid::Uid(KSendAsServerExeUid));
	TRequestStatus started( KRequestPending );
	RProcess server;
	TInt err = server.Create(KDummyServerExe, KNullDesC(), serverUid);

	if( err != KErrNone )
		{
		return err;
		}
	
	TRequestStatus status;
	server.Rendezvous(status);
	if( status != KRequestPending )
		{
		server.Kill(0); // abort start-up
		}
	else
		{
		server.Resume();	// wait for server start-up.
		}
	User::WaitForRequest(status);
	err = (server.ExitType() == EExitPanic ) ? KErrGeneral : status.Int();
	server.Close();
	return err;	
	}

TInt RDummyServSession::Connect()
    {
	// Start the server.
    TInt err = StartDummyServer();
    if (err == KErrNone)
    	{
    	err = CreateSession(KDummyServerName,Version(),5);
    	}
        
    return(err); 
    }


/**
Returns the earliest version number of the server that we can talk to.
*/  
TVersion RDummyServSession::Version(void) const
    {
    return(TVersion(KCountServMajorVersionNumber,KCountServMinorVersionNumber,KCountServBuildVersionNumber));
    }
    
    
TMsvEntry RDummyServSession::GetTMsvEntry(CMsvEntrySelection* iSelection)
    {
    TMsvEntry entry;
   	TPckgBuf<TMsvEntry> pckg;
    TMsvId entryId = iSelection->At(0);
	SendReceive(EServGetTMsvEntry,TIpcArgs(entryId, &pckg));	
	entry = pckg();
	return entry;
    }

EXPORT_C CArrayPtr<MEikSrvNotifierBase2>* NotifierArray()
	{
	CArrayPtrFlat<MEikSrvNotifierBase2>* subjects=NULL;
	TRAPD( err, subjects=new (ELeave)CArrayPtrFlat<MEikSrvNotifierBase2>(1) );
	if( err == KErrNone )
		{
		TRAP( err, subjects->AppendL( CSendAsUINotifier::NewL() ) );
		return(subjects);
		}
	else
		{
		return NULL;
		}
	}

CSendAsUINotifier* CSendAsUINotifier::NewL()
	{
	CSendAsUINotifier* self=new (ELeave) CSendAsUINotifier();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CSendAsUINotifier::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection();
	}

CSendAsUINotifier::~CSendAsUINotifier()
	{
	delete iSelection;
	REComSession::FinalClose();
	}

CSendAsUINotifier::CSendAsUINotifier()
	{
	iInfo.iUid      = KSendAsNotifierPluginUid;
	iInfo.iChannel  = KTechViewScreenOutputChannel;
	iInfo.iPriority = ENotifierPriorityHigh;
	}

void CSendAsUINotifier::Release()
	{
	delete this;
	}

/**
Called when a notifier is first loaded to allow any initial construction that is required.
 */
CSendAsUINotifier::TNotifierInfo CSendAsUINotifier::RegisterL()
	{
	return iInfo;
	}

CSendAsUINotifier::TNotifierInfo CSendAsUINotifier::Info() const
	{
	return iInfo;
	}
	
/**
The notifier has been deactivated so resources can be freed and outstanding messages completed.
 */
void CSendAsUINotifier::Cancel()
	{
	}

/**
Start the Notifier with data aBuffer. 

Not used for confirm notifiers
*/
TPtrC8 CSendAsUINotifier::StartL(const TDesC8& /*aBuffer*/)
	{
	User::Leave(KErrNotSupported);
	return KNullDesC8();
	}
	
/**
Start the notifier with data aBuffer. aMessage should be completed when the notifier is deactivated.

May be called multiple times if more than one client starts the notifier. The notifier is immediately 
responsible for completing aMessage.
*/
void CSendAsUINotifier::StartL(const TDesC8& aBuffer, TInt /*aReplySlot*/, const RMessagePtr2& aMessage)
	{
	// extract the notifier request parameters
	TMsvPackedNotifierRequest::UnpackL(aBuffer, *iSelection, iSecurityInfo);
	iMessage   = aMessage;
	//Start the session.
	RDummyServSession serverSession;
	serverSession.Connect();

	// Simulate "User Response"
	TMsvEntry entry =  serverSession.GetTMsvEntry(iSelection);
	serverSession.Close();
	// complete with TMsvEntry::iError as return code
	if (entry.iError != KErrNone)
		{
		iMessage.Complete(KErrPermissionDenied);
		}
	else
		{
		iMessage.Complete(KErrNone);
		}
	// Clear the CMsvEntrySelection
	iSelection->Reset();
	}
	
/**
Update a currently active notifier with data aBuffer.

Not used for confirm notifiers
*/
TPtrC8 CSendAsUINotifier::UpdateL(const TDesC8& /*aBuffer*/)
	{
	User::Leave(KErrNotSupported);
	return KNullDesC8();
	}

//Adding ECOM SUPPORT
const TImplementationProxy ImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY(0x10275530,NotifierArray)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}


