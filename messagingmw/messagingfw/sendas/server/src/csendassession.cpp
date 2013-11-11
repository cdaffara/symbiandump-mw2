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

#include "csendassession.h"

#include <msvids.h>
#include <s32mem.h> 
#include <csendasaccounts.h>
#include <csendasmessagetypes.h>
#include <mtmuids.h>
#include <mtclreg.h>
#include <mtclbase.h>

#include "csendasserver.h"
#include "csendasmtmmanager.h"
#include "csendasmessage.h"
#include "sendasserverdefs.h"
#include "tsendasserverpanic.h"

CSendAsSession* CSendAsSession::NewL(const TVersion& aVersion, CSendAsServer& aServer)
	{
	TVersion version(KSendAsServerVersionMajor, KSendAsServerVersionMinor, KSendAsServerVersionBuild);
	if( !User::QueryVersionSupported(version, aVersion) )
		{
		User::Leave(KErrNotSupported);
		}
	
	CSendAsSession* self = new(ELeave) CSendAsSession(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CSendAsSession::ConstructL()
	{
	iMessages = CObjectIx::NewL();
	iContainer = iServer.NewContainerL();
	TMsvSelectionOrdering order(KMsvNoGrouping, EMsvSortByDetails, ETrue);
	iMsvEntry = CMsvEntry::NewL(GetMsvSessionL(), KMsvRootIndexEntryId, order);
	iSendAsAccounts = CSendAsAccounts::NewL();
	
	// Prepare available message type list
	iAvailableMessageTypes = CSendAsMessageTypes::NewL();
	ResetTypeFilterL();
	}
	
CSendAsSession::CSendAsSession(CSendAsServer& aServer)
: iServer(aServer)
	{
	}

CSendAsSession::~CSendAsSession()
	{
	delete iMessages;
	// iContainer will be deleted by the server
	iServer.SessionClosed(iContainer);
	delete iSendAsAccounts;
	delete iAvailableMessageTypes;
	delete iMsvEntry;
	}
	
/** Service the incoming message.

If the message is for the session object, it will be dispatched to
the correct handler.  If the message is for the subsession object,
it will be dispatched to the handler located in the subsession.

This allows all of the subsession logic to be internal to itself.

If a handler requires an asynchronous response, then this method
should return ETrue.  If this method leaves, the trap harness in
the ServiceL method will catch the error and complete the message
with the error code.

@param	aMessage
The IPC message object.
*/
TBool CSendAsSession::DoServiceL(const RMessage2& aMessage)
	{
	// session methods
	switch (aMessage.Function())
		{
		case ESASSetFilter:
			DoSetMessageTypeFilterL(aMessage);
			break;
		case ESASClearFilter:
			DoClearMessageTypeFilterL();
			break;
		case ESAMDestroySubSession:
			// this is a sub session message, which means that the handle is transmitted
			// automatically, but we will handle it at the session level so we can
			// not only close the subsession, but also delete the object properly.
			RemoveMessage(aMessage.Int3());
			break;
		case ESASGetMessageTypeListLength:
			DoGetMessageTypeListLengthL(aMessage);
			break;
		case ESASGetMessageTypes:
			DoGetMessageTypeListL(aMessage);
			break;
		case ESASGetAccountListLength:
			DoGetAccountListLengthL(aMessage);
			break;
		case ESASGetAccountList:
			DoGetAccountListL(aMessage);
			break;
		case ESASCreateSubSession:
			CreateSubsessionL(aMessage);
			break;
		default:
			{
			// must be a subsession method
			CSendAsMessage* msg = MessageFromHandle(aMessage.Int3());
			if( msg == NULL )
				{
				PanicClient(aMessage, ESendAsClientPanicBadSubSessionHandle);
				User::Leave(KErrBadHandle);
				}
			return msg->DoSubSessionServiceL(aMessage);
			}
		}
	return EFalse;	
	}

/** Create a new subsession object to represent a message.

Each message is represented by a CSendAsMessage object.  These objects
are derived from CContainer.  This allows us to use the object container
framework to keep track of allocated objects and to ensure that the
object handles are unique.

The RMessage is required so that the session can pass the new
subsession handle to the client-side RSubSession object.

@param	aMessage
The IPC message object.
*/
void CSendAsSession::CreateSubsessionL(const RMessage2& aMessage)
	{
	// new message (subsession)
	CSendAsMessage* message = CSendAsMessage::NewL(*this);
	CleanupClosePushL(*message);

	// add message class into the container and obtain a handle for it.
	iContainer->AddL(message);
	TInt id = iMessages->AddL(message);
	CleanupStack::Pop(message);
	
	// write the handle back to the client
	TPckg<TInt> pckg(id);
	TInt err = aMessage.Write(3, pckg);

	if( err != KErrNone )
		{
		iMessages->Remove(id);
		message->Close();

		User::Leave(err);
		}
	++iMessageCount;
	}

/** Find a subsession pointer from its handle.

A message sent from an RSubSession-derived object will always contain
a handle to the server-side subsession object in parameter 3.

This method takes that handle and looks it up in the object index to
find the pointer to the correct subsession instance.

@param	aHandle
The handle for the subsession object

@return
The pointer to the subsesion object referred to by aHandle if it exists,
otherwise NULL is returned.
*/
CSendAsMessage* CSendAsSession::MessageFromHandle(TUint aHandle)
	{
	return static_cast<CSendAsMessage*>(iMessages->At(aHandle));
	}
	
void CSendAsSession::RemoveMessage(TUint aHandle)
	{
	// this operation will delete the subsession.
	iMessages->Remove(aHandle);
	--iMessageCount;
	}
	
void CSendAsSession::PanicClient(const RMessage2& aMessage, TSendAsClientPanic aPanic) const
	{
	_LIT(KSendAsServerSession, "SendAsSession");
	aMessage.Panic(KSendAsServerSession, aPanic);
	}

/** Add a message type filter to the list.

This removes any non-compliant MTMs from the list of available MTMs.

This is a cumulative operation, filters are applied as they are received
and the list is shortened each time.

@param	aMessage
The IPC message object.
*/
void CSendAsSession::DoSetMessageTypeFilterL(const RMessage2& aMessage)
	{
	// extract filter
	TPckgBuf<TSendAsMessageTypeFilter> filterBuf;
	aMessage.ReadL(0, filterBuf);
	
	// apply to available mtm list
	AddTypeFilterL(filterBuf());
	}

/** Remove the message type filter.

In reality, this means that we repopulate the entire list.
*/
void CSendAsSession::DoClearMessageTypeFilterL()
	{
	ResetTypeFilterL();
	}

void CSendAsSession::DoGetMessageTypeListLengthL(const RMessage2& aMessage)
	{
	// return size of message types data
	TPckgBuf<TUint32> buf(iAvailableMessageTypes->Size());
	aMessage.WriteL(0, buf);
	}
	
void CSendAsSession::DoGetMessageTypeListL(const RMessage2& aMessage)
	{
	// buffer to hold message types data
	HBufC8* buffer = HBufC8::NewLC(iAvailableMessageTypes->Size());
	
	// stream 
	RDesWriteStream strm;
	CleanupClosePushL(strm);
	TPtr8 ptr(buffer->Des());
	strm.Open(ptr);

	// write to stream
	iAvailableMessageTypes->ExternalizeL(strm);
	strm.CommitL();
	aMessage.WriteL(0, *buffer);

	// clean up
	CleanupStack::PopAndDestroy(2, buffer); // buffer, strm
	}

void CSendAsSession::DoGetAccountListLengthL(const RMessage2& aMessage)
	{
	// message slot 1 contains the mtm uid 
	TPckgBuf<TUid> uidBuf;
	aMessage.ReadL(1, uidBuf);

	// reset the account list
	iSendAsAccounts->Reset();
	
	// get account list for this mtm
	TUid acctUid = uidBuf();
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);
	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithMtmL(acctUid);
	CleanupStack::PushL(selection);
	
	// populate account list
	TInt count = selection->Count();
	iSendAsAccounts->SetMessageType(acctUid);
	for (TInt i=0; i<count; ++i)
		{
		TSendAsAccount id = selection->At(i);
		// set context
		iMsvEntry->SetEntryL(id);
		// add account		
		iSendAsAccounts->AppendAccountL(iMsvEntry->Entry().iDetails, id);
		}
	CleanupStack::PopAndDestroy(selection);

	// get size
	TPckgBuf<TInt> buf(iSendAsAccounts->Size());
	aMessage.WriteL(0, buf);
	}
	
void CSendAsSession::DoGetAccountListL(const RMessage2& aMessage)
	{
	// buffer to hold accounts data
	HBufC8* buffer = HBufC8::NewLC(iSendAsAccounts->Size());
	
	// buffer write stream
	RDesWriteStream strm;
	CleanupClosePushL(strm);
	TPtr8 ptr(buffer->Des());
	strm.Open(ptr);

	// write to buffer
	iSendAsAccounts->ExternalizeL(strm);
	
	// commit and return buffer
	strm.CommitL();
	aMessage.WriteL(0, *buffer);

	// clean up
	CleanupStack::PopAndDestroy(2, buffer);	// buffer, strm
	}
	
CMsvSession& CSendAsSession::GetMsvSessionL()
	{ 
	return iServer.GetMsvSessionL(); 
	}

CSendAsActiveContainer& CSendAsSession::ActiveContainer()
	{
	return iServer.ActiveContainer();
	}

const TUid& CSendAsSession::NotifierUid() const
	{
	return iServer.NotifierUid();
	}

const TUid& CSendAsSession::EditUtilsPluginUid() const
	{
	return iServer.EditUtilsPluginUid();
	}

/*
 *	methods from CSession2
 */
 
/** Session ServiceL method.

This method traps the real serviceL method so that leaves can
be trapped and returned as error completion codes on the message.

This prevents the session from leaving during the server AO's
RunL and causing a panic.

@param	aMessage
The IPC message object.
*/
void CSendAsSession::ServiceL(const RMessage2& aMessage)
	{
	TBool async = EFalse;
	TRAPD(err, async = DoServiceL(aMessage));
	
	// complete the message if necessary.
	if (!async || err != KErrNone)
		{
		aMessage.Complete(err);
		}
	}


/** Handles session disconnect.

@param	aMessage
The IPC message object.
*/
void CSendAsSession::Disconnect(const RMessage2& aMessage)
	{
	// Cancel all sub-session objects.
	TInt count = iMessages->Count();
	for( TInt i=0; i<count; ++i)
		{
		CSendAsMessage* message = static_cast<CSendAsMessage*>((*iMessages)[i]);
		message->CancelMessage();
		}
	
	// Must call the base class implementation - this will delete the object.
	CSession2::Disconnect(aMessage);
	}
	

/*
 *	available Client MTM array management and access methods.
 */
	
/** Resets the available message type list

*/
void CSendAsSession::ResetTypeFilterL()
	{
	// get unfiltered MTM array
	RArray<TUid>& aMtmUidArray = iServer.GetMtmManager()->GetMtmUidArray();
	
	// get client registry
	CClientMtmRegistry* aClientRegistry = iServer.GetMtmManager()->GetClientMtmRegistry();

	// reset filtered MTM array
	iAvailableMessageTypes->Reset();
	
	// populate filtered MTM array
	TInt count = aMtmUidArray.Count();
	for( TInt i=0; i<count; ++i )
		{
		// mtm uid
		TUid mtmUid = aMtmUidArray[i];
		// mtm name
		TPtrC name = aClientRegistry->RegisteredMtmDllInfo(mtmUid).HumanReadableName();
		iAvailableMessageTypes->AppendMessageTypeL(name, mtmUid);
		}
	}

/** Reduces available message type list according to filter 

@param	aFilter
The filter to be applied
*/
void CSendAsSession::AddTypeFilterL(const TSendAsMessageTypeFilter& aFilter)
	{
	if( aFilter.iMessageCapability == KUidMtmQueryCanSendMsg )
		{
		// the list of mtms available to sendas sessions is created with this 
		// capability by default by the sendas MTM manager, so no action needed
		return;
		}

	// locals used in scan loop
	CBaseMtm* mtm = NULL;
	TInt      response;
	TInt      error;
	TBool     capable;
	
	// scan list of mtms and filter out those without the required capabilities
	TInt count = iAvailableMessageTypes->Count();
	while( count-- > 0 )
		{
		// get mtm by uid		
		mtm = iServer.GetMtmManager()->FindStoredMtmL(iAvailableMessageTypes->MessageTypeUid(count));

		// check if capability supported
		response = 0;
		error = mtm->QueryCapability(aFilter.iMessageCapability, response);
		
		// if capability supported, check conditions 
		capable = EFalse;
		if( error == KErrNone )
			{
			switch (aFilter.iCondition)
				{
			case RSendAs::ESendAsNoCondition:
				capable = ETrue;
				break;
			case RSendAs::ESendAsEquals:
				capable = (response == aFilter.iValue);
				break;
			case RSendAs::ESendAsNotEquals:
				capable = (response != aFilter.iValue);
				break;
			case RSendAs::ESendAsGreaterThan:
				capable = (response > aFilter.iValue);
				break;
			case RSendAs::ESendAsLessThan:
				capable = (response < aFilter.iValue);
				break;
			case RSendAs::ESendAsBitwiseAnd:
				capable = (response & aFilter.iValue);
				break;
			case RSendAs::ESendAsBitwiseOr:
				capable = (response | aFilter.iValue);
				break;
			case RSendAs::ESendAsBitwiseNand:
				capable = !(response & aFilter.iValue);
				break;
			case RSendAs::ESendAsBitwiseNor:
				capable = !(response | aFilter.iValue);
				break;
			default:
				break;
				}
			}
		else if( error != KErrNotSupported )
			{
			User::Leave(error); 
			}

		if( !capable )
			{
			// mtm is not capable, so remove it from the available list
			iAvailableMessageTypes->RemoveMessageType(count);
			}
		}
	}

/** Updates the array of available Client MTMs

*/
void CSendAsSession::HandleMtmChange()
	{
	// get unfiltered MTM UID array
	RArray<TUid>& aMtmUidArray = iServer.GetMtmManager()->GetMtmUidArray();
	
	// check for removed mtm from filtered lists
	TInt count = iAvailableMessageTypes->Count();
	for( TInt i=0; i<count; ++i )
		{
		if (aMtmUidArray.Find(iAvailableMessageTypes->MessageTypeUid(i)) == KErrNotFound)
			{
			iAvailableMessageTypes->RemoveMessageType(i);
			break;
			}
		}
	}

/** Returns the client MTM specified by UID 

@param	aMtmUid
The UID of the Client MTM
*/
CBaseMtm* CSendAsSession::GetClientMtmL(TUid aMtmUid)
	{
	return ( iServer.GetMtmManager()->GetClientMtmL(aMtmUid) );
	}
