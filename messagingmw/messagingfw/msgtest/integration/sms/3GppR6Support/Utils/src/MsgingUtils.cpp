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

/**
 @file
*/
 
#include "MsgingUtils.h"

#include <msvapi.h>
#include <msvids.h>

/**
Returns the UID of the folder required.

@param aFolderName		The name of folder whose UID is needed

@internalTechnology
*/
TMsvId MsgingUtils::GetLocalFolderId(const TDesC& aFolderName)
	{
	TMsvId paramFolderId = 0;

	if(aFolderName.CompareF(KInbox) == 0)
		{
		paramFolderId = KMsvGlobalInBoxIndexEntryId;
		}
	if(aFolderName.CompareF(KOutbox) == 0)
		{
		paramFolderId = KMsvGlobalOutBoxIndexEntryId;
		}
	if(aFolderName.Compare(KDraft) == 0)
		{
		paramFolderId = KMsvDraftEntryId;
		}
	if(aFolderName.Compare(KSent) == 0)
		{
		paramFolderId = KMsvSentEntryId;
		}
	if(aFolderName.Compare(KDeletedEntryFolder) == 0)
		{
		paramFolderId = KMsvDeletedEntryFolderEntryId;
		}
	if(aFolderName.Compare(KRoot) == 0)
		{
		paramFolderId = KMsvRootIndexEntryId;
		}
	if(aFolderName.Compare(KLocal) == 0)
		{
		paramFolderId = KMsvLocalServiceIndexEntryId;
		}
	return paramFolderId;
	}


/**
Searches for the entry by the subject specified on given folder

@param aSettingsFile		Subject of the Msg
@param aFolderID			Folder Id in which to search for the entry
@param aSession 			Session object
@internalTechnology
*/
TMsvId MsgingUtils::GetEntryByNameL(const TDesC& aSubject, TMsvId aFolderId, CMsvSession& aSession)
	{
	
//	CMsvEntry* entry;
//	TRAPD(err, entry = aSession.GetEntryL(aFolderId));
	CMsvEntry* entry = aSession.GetEntryL(aFolderId);
	CleanupStack::PushL(entry);
	TMsvId paramEntryId = entry->Entry().Id();

	entry->SetEntryL(paramEntryId);

	TMsvSelectionOrdering order;
	order.SetShowInvisibleEntries(ETrue);
	entry->SetSortTypeL(order);

	CMsvEntrySelection* selection = entry->ChildrenL();
	CleanupStack::PushL(selection);
	
	TBool found = EFalse;
	TInt count = selection->Count();
	for (TInt i=0; i<count; i++) 
		{	
		entry->SetEntryL((*selection)[i]);
		if ( ( aSubject.CompareF( entry->Entry().iDescription ) == 0 ) ||
		    ( aSubject.CompareF( entry->Entry().iDetails ) == 0 ) )
			{
			found = ETrue;
			break;
			}
		}

	if (found)
		paramEntryId = entry->Entry().Id();
	else
		{
		User::Leave(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(selection);
	CleanupStack::PopAndDestroy(entry);
	
	return paramEntryId;
	}

CEntryCmdHandler* CEntryCmdHandler::NewL()
	{
	CEntryCmdHandler* entryHandle = new(ELeave) CEntryCmdHandler();
	CleanupStack::PushL(entryHandle);
	entryHandle->ConstructL();
	CleanupStack::Pop(entryHandle);
	return entryHandle;
	}

CEntryCmdHandler::CEntryCmdHandler(){}

CEntryCmdHandler::~CEntryCmdHandler(){}

/**
Handles any events occuring with Entry

@param aEvent				Event that needs to be handled
@param aAny1
@param aAny2
@param aAny3
@internalTechnology
*/
void CEntryCmdHandler::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /* aArg2*/, TAny* /*aArg3*/)
/**
 * Called by CMsvEntry when a messaging event has occurred. It is used here to find out if any new messages have been created.
*/
	{
	switch (aEvent)
		{
		case EMsvNewChildren:
			if(aArg1 == NULL)
				{
				User::Leave(KErrGeneral);
				}
			else{
				iCount++;
				}
			break;
              
		default:
			User::Leave(KErrGeneral);
		}
	}
	
void CEntryCmdHandler::ConstructL()
	{
	iCount = 0;
	}
