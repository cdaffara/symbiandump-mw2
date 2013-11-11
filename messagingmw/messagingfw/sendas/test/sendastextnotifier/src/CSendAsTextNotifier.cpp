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

#include "csendastextnotifier.h"
#include <tmsvpackednotifierrequest.h>

/** UID of the SendAs notifier channel to get the user's confirmation */
const TUid KSendAsNotifierPluginUid     = {0x10208C14};
const TUid KTechViewScreenOutputChannel = {0x10208C14};


EXPORT_C CArrayPtr<MNotifierBase2>* NotifierArray()
	{
	CArrayPtrFlat<MNotifierBase2>* subjects=NULL;
	TRAPD( err, subjects=new (ELeave)CArrayPtrFlat<MNotifierBase2>(1) );
	if( err == KErrNone )
		{
		TRAP( err, subjects->AppendL( CSendAsTextNotifier::NewL() ) );
		return(subjects);
		}
	else
		{
		return NULL;
		}
	}

CSendAsTextNotifier* CSendAsTextNotifier::NewL()
	{
	CSendAsTextNotifier* self=new (ELeave) CSendAsTextNotifier();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
void CSendAsTextNotifier::ConstructL()
	{
	iSelection = new (ELeave) CMsvEntrySelection();
	}

CSendAsTextNotifier::~CSendAsTextNotifier()
	{
	delete iSelection;
	}

CSendAsTextNotifier::CSendAsTextNotifier()
	{
	iInfo.iUid      = KSendAsNotifierPluginUid;
	iInfo.iChannel  = KTechViewScreenOutputChannel;
	iInfo.iPriority = ENotifierPriorityHigh;
	}

void CSendAsTextNotifier::Release()
	{
	delete this;
	}

/**
Called when a notifier is first loaded to allow any initial construction that is required.
 */
CSendAsTextNotifier::TNotifierInfo CSendAsTextNotifier::RegisterL()
	{
	return iInfo;
	}

CSendAsTextNotifier::TNotifierInfo CSendAsTextNotifier::Info() const
	{
	return iInfo;
	}
	
/**
The notifier has been deactivated so resources can be freed and outstanding messages completed.
 */
void CSendAsTextNotifier::Cancel()
	{
	}

/**
Start the Notifier with data aBuffer. 

Not used for confirm notifiers
*/
TPtrC8 CSendAsTextNotifier::StartL(const TDesC8& /*aBuffer*/)
	{
	User::Leave(KErrNotSupported);
	return KNullDesC8();
	}
	
/**
Start the notifier with data aBuffer. aMessage should be completed when the notifier is deactivated.

May be called multiple times if more than one client starts the notifier. The notifier is immediately 
responsible for completing aMessage.
*/
void CSendAsTextNotifier::StartL(const TDesC8& aBuffer, TInt /*aReplySlot*/, const RMessagePtr2& aMessage)
	{
	// extract the notifier request parameters
	TMsvPackedNotifierRequest::UnpackL(aBuffer, *iSelection, iSecurityInfo);
	iMessage   = aMessage;

	// Simulate "User Response"
	CDummyObserver* ob1 = new(ELeave) CDummyObserver;
	CleanupStack::PushL(ob1);

	CMsvSession* session = CMsvSession::OpenSyncL(*ob1);
	CleanupStack::PushL(session);

	CMsvEntry* cEntry = session->GetEntryL(iSelection->At(0));
	CleanupStack::PushL(cEntry);

	TMsvEntry entry = cEntry->Entry();

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
	
	CleanupStack::PopAndDestroy(3, ob1); // cEntry, session, ob1
	}
	
/**
Update a currently active notifier with data aBuffer.

Not used for confirm notifiers
*/
TPtrC8 CSendAsTextNotifier::UpdateL(const TDesC8& /*aBuffer*/)
	{
	User::Leave(KErrNotSupported);
	return KNullDesC8();
	}

