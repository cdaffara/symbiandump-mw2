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
#include <push/pushlog.h>
#include <pushentry.h>

#include  <push/sislpushmsgutils.h>
#include <push/csipushmsgentry.h>
#include <push/cslpushmsgentry.h>


/**
Allocates and constructs a new SISL Push Message Utility object.

@return 
New SISL Push Message Utility object.
*/
EXPORT_C CSISLPushMsgUtils* CSISLPushMsgUtils::NewL()
	{
	CSISLPushMsgUtils* self = new (ELeave) CSISLPushMsgUtils();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


/**
Constructor.  
*/
CSISLPushMsgUtils::CSISLPushMsgUtils() : CWapPushMsgUtils()
	{
	}


/**
Constructor. Creates CMsvSession and CMsvEntry.   
*/
void CSISLPushMsgUtils::ConstructL()
	{
	CWapPushMsgUtils::ConstructL();
	}


/**
Destructor.
*/
EXPORT_C CSISLPushMsgUtils::~CSISLPushMsgUtils()
	{
	}


/**
Locates a selection of SI entries with the specified ID. 

Assumes that all SI entries are saved as children of the Push Service Message Folder. 
If the id of the Push Message Folder happens to be Null, it gets the Id of the folder.

The return value CMsvEntrySelection is on the Cleanup Stack.

@param aSiId 
Id of SI message to locate

@return 
Id of the message entries if it exists, or KMsvNullIndexEntryId if it doesn't.
*/
EXPORT_C CMsvEntrySelection* CSISLPushMsgUtils::FindSiIdLC(const TDesC& aSiId)
	{
	CMsvEntrySelection* matching = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(matching);
	
	CSIPushMsgEntry* siEntry = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siEntry);

	if (iPushFolderId == KMsvNullIndexEntryId)
		GetPushMsgFolderIdL(iPushFolderId);
	iMsvEntry->SetEntryL(iPushFolderId);

	CMsvEntrySelection* children;
	children = GetChildrenWithMtmLC(KUidMtmWapPush);//leave children on CleanupStack 
	
	TInt numEntries= children->Count();
	TMsvEntry currentEntry;

	// loop until we find a match or reach the end of the SI entries
	for (TInt count=0; count<numEntries; count++)
		{
		iMsvEntry->SetEntryL(children->At(count));
		currentEntry = iMsvEntry->Entry();
		// Hard coded check for the Message Push Type
		if (currentEntry.iBioType == KUidWapPushMsgSI.iUid) //It's an SI message!
			{
			siEntry->SetEntry(currentEntry);
			siEntry->RetrieveL(*iMsvSession, currentEntry.Id());
			if (aSiId.CompareF( siEntry->Id()) ==0)// Found a match
				matching->AppendL( currentEntry.Id());       // get the Id to return
			}
		}
	CleanupStack::PopAndDestroy(2); // children, siEntry

	return matching; //Still on CleanupStack
	}


/**
Finds any entry with the specified Url and Push Message Type Uid.

The function assumes that the entries are stored under the Push Message Folder Entry. 
 
@param aUrl 
URL to find

@param aPushType 
Push Message type to filter entries.

@return 
Message entry for the specified Url and message type, 
or KMsvNullIndexEntryId if a match is not found.

@leave KErrNotFound Could not locate a SISL Push message with the specified URL and push type.
@leave CSISLPushMsgUtils::GetPushMsgFolderIdL
@leave CMsvEntry::SetEntryL
@leave CMsvEntry::HasStoreL
@leave CMsvEntry::ReadStoreL
@leave CMsvStore::IsPresentL
@leave CMsvEntry::EntryId
@leave CMsvStore::RetrieveL
*/
EXPORT_C TMsvId CSISLPushMsgUtils::FindUrlL(const TDesC& aUrl, TUid aPushType)
	{
	//Finds all of the entries with the Push Mtm type, 
	//and then checks if each that has the correct Push Message Type, until either a match is 
	//found or there are no more entries.

	TMsvId pushID = KMsvNullIndexEntryId;

	if (iPushFolderId == KMsvNullIndexEntryId)
		GetPushMsgFolderIdL(iPushFolderId);
	iMsvEntry->SetEntryL(iPushFolderId);

	CMsvEntrySelection* children;
	children = GetChildrenWithMtmLC(KUidMtmWapPush);//leave children on CleanupStack

	TInt numEntries= children->Count();
	TMsvEntry currentEntry;

	// loop until we find a match or reach the end of the SI entries
	for (TInt count=0; count<numEntries  && pushID == KMsvNullIndexEntryId; count++)
		{
		iMsvEntry->SetEntryL(children->At(count));
		currentEntry = iMsvEntry->Entry();
		if (currentEntry.iBioType == aPushType.iUid && iMsvEntry->HasStoreL()) 
			//It's the right type of message!
			{
			CPushMsgEntryBase* pushEntry = GetPushMsgEntryL(aPushType);
			if (pushEntry)
				{
				TPtrC url;
				if (aPushType == KUidWapPushMsgSI)
					{
					url.Set(static_cast<CSIPushMsgEntry*>(pushEntry)->Url());
					}
				else
					{
					url.Set(static_cast<CSLPushMsgEntry*>(pushEntry)->Url());
					}
				CleanupStack::PushL(pushEntry);
				pushEntry->RetrieveL(*iMsvSession, currentEntry.Id());
				if (aUrl.CompareF(url) == 0) 
					pushID = currentEntry.Id();
				CleanupStack::PopAndDestroy(pushEntry);
				}
			}
		}
	CleanupStack::PopAndDestroy(); // children
	return pushID;
	}


/**
Gets the action value for the specified message. 
 
@param aId 
Message entry of interest.

@return 
Action level for the message, or KErrNotFound if action level not found.
*/
EXPORT_C TInt CSISLPushMsgUtils::GetActionL(TMsvId aId)
	{
	//Creates an instance of the correct message entry type which is used to 
	//get the action value. This ensures that the decoding routine of the 
	//PushEntry class is used, maintaining encapsulation.

	TInt action = KErrNotFound;  // -1 to indicate we haven't found the action level
	TMsvEntry myEntry;
	iMsvEntry->SetEntryL(aId);
	myEntry = iMsvEntry->Entry();

	if  (myEntry.iBioType == KUidWapPushMsgSI.iUid)
		{
		CSIPushMsgEntry* siMsg = CSIPushMsgEntry::NewL();
		siMsg->SetEntry(myEntry);
		action = siMsg->Action();
		delete siMsg;
		}
	else if (myEntry.iBioType == KUidWapPushMsgSL.iUid)
		{
		CSLPushMsgEntry* slMsg = CSLPushMsgEntry::NewL();
		slMsg->SetEntry(myEntry);
		action = slMsg->Action();
		delete slMsg;
		}
	return action;
	}


/**
Gets SI or SL Push Message (CSIPushMsgEntry or CSLPushMsgEntry) object from the message 
store for the specified message type.
  
@param aPushType 
Push Message type.

@return 
SI or SL Push Message object for the specified message type.

@leave CMsvEntry::HasStoreL
@leave CMsvEntry::ReadStoreL
@leave CMsvStore::IsPresentL
@leave CMsvEntry::EntryId
@leave CMsvStore::RetrieveL
*/
CPushMsgEntryBase* CSISLPushMsgUtils::GetPushMsgEntryL(const TUid aPushType)
	{
	TBool streamExists=EFalse;

	if (iMsvEntry->HasStoreL())
		{
		CMsvStore* store = iMsvEntry->ReadStoreL();
		CleanupStack::PushL(store);
		if (store->IsPresentL(aPushType) )
			streamExists = ETrue;
		CleanupStack::PopAndDestroy(store);
		}

	if (!streamExists)
		return NULL;

	CPushMsgEntryBase* pushMsg = NULL;
	if (aPushType == KUidWapPushMsgSI)
		{
		pushMsg = CSIPushMsgEntry::NewL();
		}
	else if (aPushType == KUidWapPushMsgSL)
		{
		pushMsg = CSLPushMsgEntry::NewL();
		}

	CleanupStack::PushL(pushMsg);
	pushMsg->RetrieveL(Session(), iMsvEntry->EntryId());
	CleanupStack::Pop(pushMsg);

	return pushMsg;
	}


/**
Gets the expiry date for the message with the specified Id.

The function assumes that the Id passed as a parameter is for an SI message, 
and cannot currently handle any other type of message. 
 
@param aId 
Id of message to find expiry date. 

@return 
Expiry date/time of SI message.
*/
EXPORT_C const TTime  CSISLPushMsgUtils::GetExpiryL(TMsvId aId)
	{
	TInt64 nullTime = 0;
	TTime siExpires = nullTime;
	
	iMsvEntry->SetEntryL(aId);

	// Ensures we only have an SI Msg - otherwise cannot get an expiry date
	__ASSERT_ALWAYS(iMsvEntry->Entry().iBioType == KUidWapPushMsgSI.iUid, 
				User::Panic(KSISLPushMsgUtilsPanicTitle,ESISLPushMsgUtilsNotSiMsg));

	CSIPushMsgEntry* siEntry = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siEntry);
	siEntry->RetrieveL(*iMsvSession, aId);
	siExpires = siEntry->Expires();
	CleanupStack::PopAndDestroy(siEntry);
	return siExpires;
	}


/**
Finds the message of a specified Push type with the highest action level.
 
@param	aPushType 
Uid for the type of WAP Push Message

@return 
Id of the message entry with the highest action

@leave	KErrNotSupported
If aPushType is not a SI or SL push message type. 
@leave  CMsvEntry::SetEntryL
@leave	CSIPushMsgEntry::NewL
@leave	CMsvEntry::ChildrenWithMtmL
@leave	CArrayFixFlat::AppendL
*/
EXPORT_C TMsvId CSISLPushMsgUtils::GetHighestActionL(TUid aPushType)
	{
	//Action only applies to the SI and SL messages - both stored under the Service Entry
	CMsvEntrySelection* sel;

	if (iPushFolderId ==KMsvNullIndexEntryId)
		GetPushMsgFolderIdL(iPushFolderId);
	iMsvEntry->SetEntryL(iPushFolderId);

	sel=GetChildrenWithMtmLC(KUidMtmWapPush);

	CPushMsgEntryBase* pushMsg=NULL;
	
	if (aPushType ==  KUidWapPushMsgSI)
		pushMsg = CSIPushMsgEntry::NewL();
	else if(aPushType == KUidWapPushMsgSL)
		pushMsg = CSLPushMsgEntry::NewL();
	else
		User::Leave(KErrNotSupported);
	
	CleanupStack::PushL(pushMsg);

	TMsvId maxId =KMsvNullIndexEntryId ;
	TInt maxAction=-1;
	TInt currentAction=-1;
	TInt entries = sel->Count();

	for (TInt count = 0; count < entries; count++)
		{
		iMsvEntry->SetEntryL(sel->At(count));
		pushMsg->SetEntry(iMsvEntry->Entry());
		if (iMsvEntry->Entry().iBioType == aPushType.iUid)
			{
			if (aPushType == KUidWapPushMsgSI )
				currentAction = STATIC_CAST(CSIPushMsgEntry*, pushMsg) ->Action();
			else if ((aPushType == KUidWapPushMsgSL))
				currentAction = STATIC_CAST(CSLPushMsgEntry*, pushMsg) ->Action();
			else continue;

			if (currentAction>maxAction)  // Found a new highest action - store the details
				{
				maxAction = currentAction;
				maxId = sel->At(count);
				}
			currentAction = -1;
			}
		}
	CleanupStack::PopAndDestroy(2); //sel,  pushMsg
	return maxId;
	}


/**
Gets the earliest expiry date of all the SI Messages that are not flagged for deletion. 
 
@return 
Id of the message with the earliest expiry date
*/
EXPORT_C TMsvId CSISLPushMsgUtils::GetNextExpiryL()
	{
	//Function searches through the SI Messages and keeps hold of the earliest expiry up 
	//to that point. Because the function just compares each message with the preceding 
	//ones it is possible for it to return a date before the current date and time. 
	//This just means that the message has expired but has not been either removed from 
	//the system or flagged for deletion. 

	CMsvEntrySelection* sel;
	
	if (iPushFolderId ==KMsvNullIndexEntryId)
		GetPushMsgFolderIdL(iPushFolderId);
	iMsvEntry->SetEntryL(iPushFolderId);

	sel=GetChildrenWithMtmLC(KUidMtmWapPush);

	TInt count = sel->Count();
	// Ensure that we start with Max time, everything ought to be less than this
	TTime earliestTime = Time::MaxTTime(); 
	TMsvId siExpiring = KMsvNullIndexEntryId;

	CSIPushMsgEntry* siEntry = CSIPushMsgEntry::NewL();
	CleanupStack::PushL(siEntry);
			
	for (TInt current = 0; current< count;current++)
		{
	
		iMsvEntry->SetEntryL(sel->At(current));
		if (iMsvEntry->Entry().iBioType == KUidWapPushMsgSI.iUid)
			{	

			siEntry->RetrieveL(*iMsvSession, iMsvEntry->Entry().Id());

			// Ignore null expiry times and entries flagged for deletion
			if ( (siEntry->Expires() == Time::NullTTime() )
			  || (siEntry->Status() == CPushMsgEntryBase::EPushMsgStatusDeleted) )
				continue;
			else if ( (siEntry->Expires() < earliestTime ))
				{
				earliestTime = siEntry->Expires().Int64();
				siExpiring = iMsvEntry->Entry().Id();
				}
			}
		
		}
	CleanupStack::PopAndDestroy(); //siEntry
	CleanupStack::PopAndDestroy(); // sel
	return siExpiring;
	}


/**
Gets the Id of the Push Message Folder under which all Push Messages are stored. 

If the folder does not exist, it is created.

@param aFolderId 
On return, Id of the Push Message Folder.
@leave	CMsvEntry::ChildrenWithTypeL
@leave	CMsvEntry::SetEntryL
@leave	CArrayFixFlat::AppendL
*/
EXPORT_C void CSISLPushMsgUtils::GetPushMsgFolderIdL(TMsvId& aFolderId)
	{
	//Returns the Service IDs of MTM aMtm
	if (iPushFolderId != KMsvNullIndexEntryId)
		{ 
		aFolderId = iPushFolderId;
		return;
		}

	aFolderId = KMsvNullIndexEntryId;

	iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);

	//Get the children on the Root Index Entry
	CMsvEntrySelection* selection = iMsvEntry->ChildrenWithTypeL(KUidMsvFolderEntry);
	CleanupStack::PushL(selection);

	CMsvEntrySelection* pushFolders = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(pushFolders);

	TInt count = selection->Count();

	//Find an entry for MTM aMtm
	for (TInt curChild = 0; curChild < count; curChild++)
		{
		iMsvEntry->SetEntryL(selection->At(curChild));

		if (iMsvEntry->Entry().iMtm == KUidMtmWapPush)
			{
			pushFolders->AppendL( iMsvEntry->Entry().Id());
			}
		}
	iMsvEntry->SetEntryL(KMsvRootIndexEntryId);//Point our entry to something safe

	if (pushFolders->Count() >0)
		aFolderId = pushFolders->At(0); // grab the first, should only be one!
	else if (pushFolders->Count() ==0)
		{
		aFolderId = CreatePushMsgFolderL();
		}

	if (aFolderId!= KMsvNullIndexEntryId && (&iPushFolderId != &aFolderId) ) //service Id & not self assignment
		iPushFolderId = aFolderId;  // Keep this - may save effort in forthcoming fn calls
	CleanupStack::PopAndDestroy(2); //selection
	}


/**
Gets the date field from the message with the specified Id.

Creation Date is stored in iMsgDate for SI messages, whilst the field holds the date
and time of transmission for SL messages. This function is called by GetCreationDate 
and GetTransmissionDate

@param aId 
Id of the message entry with the required date field.

@param aPushType 
Uid for the type of WAP Push Message

@return 
Value of the date field.
@leave CMsvEntry::SetEntryL
@leave CMsvEntry::HasStoreL
@leave CMsvEntry::ReadStoreL
@leave CMsvStore::IsPresentL
@leave CMsvEntry::EntryId
@leave CMsvStore::RetrieveL
*/
TTime CSISLPushMsgUtils::GetDateL(TMsvId aId, const TUid aPushType)
	{
	TTime date = Time::NullTTime();

	iMsvEntry->SetEntryL(aId);

	CPushMsgEntryBase* pushEntry = GetPushMsgEntryL(aPushType);

	if (aPushType == KUidWapPushMsgSI)
		{
		date = STATIC_CAST(CSIPushMsgEntry*, pushEntry) ->Created();
		}
	else if (aPushType == KUidWapPushMsgSL)
		{
		date = STATIC_CAST(CSLPushMsgEntry*, pushEntry) ->TimeSent();
		}
	delete pushEntry;
	return date;
	}


/**
Creates a new Push Service folder under the local service. 

Called when GetPushMsgFolderL fails to find a folder entry. 

@return 
Id of the newly created folder entry
*/ 
TMsvId CSISLPushMsgUtils::CreatePushMsgFolderL()
	{
	TMsvId id = 0;
	TMsvEntry entry;
	entry.iServiceId = KMsvLocalServiceIndexEntryId;
	entry.iMtm = KUidMtmWapPush;
	entry.iType = KUidMsvFolderEntry;
	entry.SetReadOnly(ETrue);
	entry.SetVisible(EFalse);
	entry.iDetails.Set(KPushFolderDescription);
	
	iMsvEntry->SetEntryL(KMsvLocalServiceIndexEntryId);
	iMsvEntry->CreateL(entry);
	id = entry.Id();
	return id;
	}


/**
Sets the status of the specified Push message to be deleted. 

Any messages marked as deleted in the Push Message Folder under the Local Service will be 
deleted by the UI later. Sets the value of the first 4 bits of the TMsvEntry field 
iMtmData1 to EPushMsgStatusDeleted.
 
@param aId 
Id of the entry to be set to Deleted
*/
EXPORT_C void CSISLPushMsgUtils::DeleteEntryL(TMsvId aId)
	{
	iMsvEntry->SetEntryL(aId);

	TMsvEntry entry = iMsvEntry->Entry();
	entry.SetMtmData1( (entry.MtmData1() & (~KPushMaskOnlyStatus)) 
								+ CPushMsgEntryBase::EPushMsgStatusDeleted );
	iMsvEntry->ChangeL(entry);
	}

/**
Deletes the specified Push Message. 

This function deletes the item immediately.
 
@param aId 
Id of the entry to be Deleted
*/
EXPORT_C void CSISLPushMsgUtils::DeleteEntryNowL(TMsvId aId)
	{
	iMsvEntry->DeleteL(aId);
	}

/**
Gets all the children of the current message context which have the specified Mtm Uid.  

Assumes that the calling function has set the iMsvEntry member to the correct context, prior 
to the function call.

@param aMtm 
Mtm Uid to filter children with

@return 
Pointer to an array of TMsvId's for entries which match the Mtm Uid
@leave CMsvEntry::ChildrenWithMtmL
*/
CMsvEntrySelection* CSISLPushMsgUtils::GetChildrenWithMtmLC(TUid aMtm)
	{
	CMsvEntrySelection* children;
	children = iMsvEntry->ChildrenWithMtmL(aMtm);
	CleanupStack::PushL(children);
	return children;
	}


#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason/* aReason*/)	
	{
	return (KErrNone);
	}
#endif


