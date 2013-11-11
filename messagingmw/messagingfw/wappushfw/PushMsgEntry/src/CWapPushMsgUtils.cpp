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

#include <smut.h>
#include <msvstd.h>
#include <msventry.h>
#include <msvids.h>
#include <msvuids.h>
#include <watcher.h>
#include <pushentry.h>

#include <push/pushlog.h>
#include <push/cwappushmsgutils.h>

EXPORT_C CWapPushMsgUtils* CWapPushMsgUtils::NewL()
/**
Allocates and constructs a new Wap Push Message Utility object.

@return 
New Wap Push Message Utility object.

@leave CWapPushMsgUtils::ConstructL

*/
	{
	CWapPushMsgUtils* self = new (ELeave) CWapPushMsgUtils();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


EXPORT_C CWapPushMsgUtils::CWapPushMsgUtils()
	:iPushServiceId(KMsvNullIndexEntryId), iPushFolderId(KMsvNullIndexEntryId)
/**
Constructor.  
*/
	{
	}


EXPORT_C void CWapPushMsgUtils::ConstructL()
/**
Second stage constructor. Creates CMsvSession and CMsvEntry.   

@leave CMsvSession::OpenSyncL
@leave CMsvSession::GetEntryL
@leave CMsvSession::SetSortTypeL

*/
	{
	iMsvSession = CMsvSession::OpenSyncL(*this);
	iMsvEntry = iMsvSession->GetEntryL(KMsvRootIndexEntryId);
	
	// Ensure that our CMsvEntry can always see invisible entries
	TMsvSelectionOrdering sort;
	sort.SetShowInvisibleEntries(ETrue);
	iMsvEntry->SetSortTypeL(sort);
	}


EXPORT_C CWapPushMsgUtils::~CWapPushMsgUtils()
/**
Destructor.
*/
	{
	delete iMsvEntry;
	delete iMsvSession;
	}


EXPORT_C void CWapPushMsgUtils::GetServiceIdL(TUid aMtm, TMsvId& aServiceId)
/**
Gets the Id of the Push Message Service Entry. 

If there is no service entry then KMsvNullIndexEntry is returned. 
 
@param aMtm 
MTM type of the required service entry

@param aServiceId 
Id of the service entry
*/
	{
	//Returns the Service IDs of MTM aMtm
	
	if (iPushServiceId != KMsvNullIndexEntryId)
		{
		aServiceId = iPushServiceId;
		return;
		}

	aServiceId = KMsvNullIndexEntryId;

	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvServiceEntry);
	CleanupStack::PushL(selection);

	TInt count = selection->Count();

	//Find an entry for MTM aMtm
	for (TInt curChild = 0; curChild < count && (aServiceId == KMsvNullIndexEntryId ); curChild++)
		{
		iMsvEntry->SetEntryL(selection->At(curChild));

		if (iMsvEntry->Entry().iMtm == aMtm)
			{
			TMsvId id = iMsvEntry->Entry().Id();

			if (aServiceId == KMsvNullIndexEntryId)
				aServiceId = id;
			}
		}
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);//Point our entry to something safe

	//If the service Id is not set and we're not doing not self assignment store the ID
	if (aServiceId != KMsvNullIndexEntryId && (&iPushServiceId != &aServiceId) ) 
		iPushServiceId = aServiceId;  
	CleanupStack::PopAndDestroy(); //selection
	}

