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

#include <e32std.h>
#include <msvapi.h>
#include <msvids.h>
#include <msvuids.h>
#include <push/unknownmimedefs.h>
#include <pushentry.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "PushEntryPanic.h"
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS

GLDEF_C TPtrC16 LimitStringSize(const TPtrC16& aString, TInt aMaxSize)
	{
	if (aString.Length() < aMaxSize)
		return aString;
	else
		return aString.Left(aMaxSize);
	}

GLDEF_C TPtrC8 LimitStringSize(const TPtrC8& aString, TInt aMaxSize)
	{
	if (aString.Length() < aMaxSize)
		return aString;
	else
		return aString.Left(aMaxSize);
	}


/** 
Destructor. 
*/
EXPORT_C CPushMsgEntryBase::~CPushMsgEntryBase()
	{
	delete iMsgDetails;
	delete iMsgDescription;
	delete iHeader;
	delete iFrom;
	delete iAppIDString;
	}


/** 
Stores the Push message in the message store.

It creates an entry in the message server as a child of the specified parent, 
sets the relevant index entry fields, and calls ExternalizeL() to save additional 
data to a message store for the entry.

If the operation fails the entry is deleted, along with the data in its message 
store.

@param aSession
Message server session.

@param aParent
ID of the parent for the new entry. It is the caller's responsibility to ensure 
that the parent ID is correct.

@return 
ID of the new message server entry.
*/
EXPORT_C TMsvId CPushMsgEntryBase::SaveL(CMsvSession& aSession, TMsvId aParent)
	{
	__ASSERT_ALWAYS(aParent != KMsvNullIndexEntryId, 
					User::Panic(KPushPanicMoniker, EPushEntryNullMsgId));
	// Ensure that the entry parameters are correctly set.
	iEntry.iServiceId = KMsvLocalServiceIndexEntryId;  //Saving under the local service
	iEntry.iMtm = KUidMtmWapPush;	
	iEntry.iType = KUidMsvMessageEntry;
	iEntry.SetInPreparation(ETrue);
	iEntry.SetReadOnly(EFalse);
	iEntry.SetUnread(ETrue);
	SetPushMsgType();	

	// Create this outside of the TRAP - otherwise the leave would get trapped and
	// We'd tried to delete an entry that doesn't exist.
	CMsvEntry* msvEntry = aSession.GetEntryL(aParent);	//Get parent entry
	CleanupStack::PushL(msvEntry);
	
	TUid mtmUid = msvEntry->Entry().iMtm;  
	__ASSERT_ALWAYS((mtmUid == KUidMtmWapPush)||(mtmUid == KUidMsvLocalServiceMtm), 
						User::Panic(KPushPanicMoniker,EPushEntryWrongMTMtype)); 
	msvEntry->CreateL(iEntry);

	// CMsvSession::CleanupEntryPushL() can fail, leaving an partially complete
	// entry in the store. Use a TRAP and RemoveEntry to do the same job.
	TRAPD(createErr, DoSaveL(*msvEntry));
	
	if (createErr != KErrNone)
		{
		aSession.RemoveEntry(iEntry.Id());
		User::Leave(createErr);
		}

	iHasServerEntry = ETrue;

	// CMsvEntry is gone, so ensure iDescription & iDetails point
	//  to persistent copies of the strings, if they exist else set to null
	if (iMsgDescription)
		iEntry.iDescription.Set(*iMsgDescription); 
	else
		iEntry.iDescription.Set(KNullDesC);
	if (iMsgDetails)
		iEntry.iDetails.Set(*iMsgDetails); 
    else
		iEntry.iDescription.Set(KNullDesC);

	iEntry.SetInPreparation(EFalse);
	UpdateL(aSession);

	CleanupStack::PopAndDestroy(); //msvEntry 
	return iEntry.Id();
	}


/**
Helper function that does the actual saving of various data members to the message store
attached to the entry. It is done this way so that if any function call leaves, the 
error can be trapped in SaveL, and the new entry deleted before SaveL leaves.

@param aMsvEntry Parent of the new entry.

@internalComponent
*/
void CPushMsgEntryBase::DoSaveL(CMsvEntry& aMsvEntry)
	{
	// synchronous create in the Push Msg folder under Local Service
 	aMsvEntry.SetEntryL(iEntry.Id());
	__ASSERT_ALWAYS(iEntry.iBioType == PushMsgType(),
			User::Panic(KPushPanicMoniker,EPushEntryWrongMsgtype));

	CMsvStore* store = aMsvEntry.EditStoreL();
	CleanupStack::PushL(store);

	RMsvWriteStream out;
	TUid streamId;
	streamId.iUid = PushMsgType();
	out.AssignLC(*store, streamId);
	ExternalizeL(out);  //call this polymorphic function to save data
	out.CommitL();
	out.Close(); // make sure we close the file
	store->CommitL();
	CleanupStack::PopAndDestroy(2); //out, store
	}


/** 
Updates an existing message server entry.

The functionality is similiar to SaveL(), except no new entry is created. 
Before calling this function, the existing entry must be loaded into the object 
using RetrieveL().

@param aSession
Message server session.
*/
EXPORT_C void CPushMsgEntryBase::UpdateL(CMsvSession& aSession)
	{
	if (iHasServerEntry ==EFalse )
		User::Leave(KWPushNoMsgIndexEntry);
	CMsvEntry* msvEntry = aSession.GetEntryL(iEntry.Id());
	CleanupStack::PushL(msvEntry);

	//Can only overwrite a pushmessage of the same type 
	// Mtm & Push Type Uid   must be correct
	__ASSERT_ALWAYS( msvEntry->Entry().iMtm == KUidMtmWapPush, 
					User::Panic(KPushPanicMoniker, EPushEntryWrongMTMtype));
	__ASSERT_ALWAYS( msvEntry->Entry().iBioType == PushMsgType(), 
					User::Panic(KPushPanicMoniker, EPushEntryWrongMsgtype));

	// Remove existing contents of stream and store new data
	CMsvStore* store;
	store = msvEntry->EditStoreL();
	CleanupStack::PushL(store);

	RMsvWriteStream out;
	TUid streamId;
	streamId.iUid = PushMsgType();
	out.AssignLC(*store, streamId);
	ExternalizeL(out);

	// Ensure the defaults are set correctly. Assume others set by user
	iEntry.iMtm = KUidMtmWapPush;
	SetPushMsgType();
	msvEntry->ChangeL(iEntry);

	// Done the changes to the TMsvEntry, now commit changes to the stream & store
	out.CommitL();
	out.Close(); // make sure we close the file
	store->CommitL();
	CleanupStack::PopAndDestroy(3); //out, store, msventry
	iHasServerEntry = ETrue;
	
	// CMsvEntry is gone, so ensure iDescription & iDetails point to persistent
	// copies of the strings, if they exist else point them to Null Descriptors
	if (iMsgDescription)
		iEntry.iDescription.Set(*iMsgDescription); 
	else
		iEntry.iDescription.Set(KNullDesC);

	if (iMsgDetails)
		iEntry.iDetails.Set(*iMsgDetails); 
    else
		iEntry.iDescription.Set(KNullDesC);
	}


/** 
Retrieves Push message from the message store into the object.

The data held in TMsvEntry::iDetails and TMsvEntry::iDescription is copied 
to member descriptors to ensure that it persists after the local CMsvEntry 
variable is destroyed. 

@param aSession
Message server session.

@param aMsgId
ID of the entry to load.

@leave KErrNotFound
The push message cannot be located in the Message Store.
@leave CMsvSession::GetEntryL
@leave TDesC::AllocL
@leave CMsvEntry::ReadStoreL
@leave CPushMsgEntryBase::RestoreL
*/
EXPORT_C void CPushMsgEntryBase::RetrieveL(CMsvSession& aSession, TMsvId aMsgId)
	{
	__ASSERT_ALWAYS(aMsgId!= KMsvNullIndexEntryId, 
					User::Panic(KPushPanicMoniker, EPushEntryNullMsgId));

	// Switch to our entry & then get the associated message store.
	CMsvEntry* msvEntry = aSession.GetEntryL(aMsgId);
	CleanupStack::PushL(msvEntry);

	// Set our TMsvEntry member variable
	iEntry = msvEntry->Entry();

	// Make iDetails and iDescription persist as long as this object exists.
	// Delete any existing buffers 
	delete iMsgDetails;
	iMsgDetails = NULL;
	iMsgDetails = iEntry.iDetails.AllocL();
	iEntry.iDetails.Set(*iMsgDetails);

	delete iMsgDescription;
	iMsgDescription =NULL;
	iMsgDescription = iEntry.iDescription.AllocL();
	iEntry.iDescription.Set(*iMsgDescription);
	
	CMsvStore* store = msvEntry->ReadStoreL();
	CleanupStack::PushL(store);

	//Load in the additional data from the message store
	RestoreL(*store);

	CleanupStack::PopAndDestroy(2); //store, msvEntry
	// Obviously has a server entry
	iHasServerEntry = ETrue;
	}


/** 
Restores data from the associated message store.

The data is loaded by a call to the derived class InternalizeL().

@param aStore 
Store to load from.
*/
EXPORT_C void CPushMsgEntryBase::RestoreL(CMsvStore& aStore)
	{
	RMsvReadStream in;
	TUid streamId;
	streamId.iUid = PushMsgType();
	in.OpenLC(aStore, streamId);
	InternalizeL(in);
	CleanupStack::PopAndDestroy();  //in
	}	


/** 
Sets directly the TMsvEntry for the message.

This resets the context of the Push Entry, and the caller should ensure that all 
data member variables of the class are up to date.

@param aEntry 
Message server index entry.
*/
EXPORT_C void CPushMsgEntryBase::SetEntry(const TMsvEntry& aEntry)
	{
	iEntry = aEntry;
	iEntry.iMtm = KUidMtmWapPush;
	}


/** 
Sets the Status field for the Push Message Entry. 

The bits of the TMsvEntry::iMtmData1 member holding the Status are reset.

@param aStatusFlags
Status value for the message entry.
*/
EXPORT_C void CPushMsgEntryBase::SetStatus(TInt aStatusFlags)
	{
	// Get everything except the Status bits from iMtmData1
	TInt everythingButStatus = iEntry.MtmData1() & KPushMaskEverythingButStatus;  
	// Remove any extraneous bits from the new status & then set the status + action values
	iEntry.SetMtmData1( everythingButStatus + (aStatusFlags &  KPushMaskOnlyStatus) );
	}


/** 
Gets the raw WAP Push message header.

@return 
Message header, or KNullDesC8 if it has not been set with SetHeaderL(). 
*/
EXPORT_C const TDesC8& CPushMsgEntryBase::Header() const
	{
	if (iHeader)  // check the header exists
		return *iHeader;
	else 
		return KNullDesC8;
	}


/** 
Sets a buffer that specifies the WAP Push message header.

Any existing buffer is deleted.

@param aHeader 
Message header 
*/
EXPORT_C void CPushMsgEntryBase::SetHeaderL(const TDesC8& aHeader)
	{
	HBufC8* temp = aHeader.AllocL();
	delete iHeader;
	iHeader = temp;
	}


/**
Gets a buffer holding the From field.

@return 
From field, or KNullDesC8 if it has not been set with SetFromL(). 
*/
EXPORT_C const TDesC8& CPushMsgEntryBase::From() const
	{
	if (iFrom)
		return *iFrom;
	else
		return KNullDesC8;
	}


/** 
Sets a buffer that specifies the From field.

Any existing buffer is deleted.

@param aFrom
From field 
*/
EXPORT_C void CPushMsgEntryBase::SetFromL(const TDesC8& aFrom)
	{
	HBufC8* temp = aFrom.AllocL();
	delete iFrom;
	iFrom = temp;
	}


/** 
Constructor.

This initialises TMsvEntry::iDate. 
*/
EXPORT_C CPushMsgEntryBase::CPushMsgEntryBase()
	{
	iEntry.iDate.UniversalTime();
	}


/** 
Externalises the object to a message store stream.

Derived classes should override this function if required, and call the base 
class function.

@param aStream 
Message store stream.
*/
EXPORT_C void CPushMsgEntryBase::ExternalizeL(RMsvWriteStream& aStream)
	{	
	aStream<< LimitStringSize(Header(), KLongestStringAllowed);
	aStream<< LimitStringSize(From(), KLongestStringAllowed);

	if (iAppIDString)
		aStream<< *iAppIDString;
	else
		aStream<< KNullDesC;

	aStream.WriteInt32L(iAppIdInt);
	}


/** 
Internalises the object from a message store stream.

Derived classes should override this function if required, and call the base 
class function.

@param aStream 
Message store stream. 
*/ 
EXPORT_C void CPushMsgEntryBase::InternalizeL(RMsvReadStream& aStream)
	{
	delete iHeader;
	iHeader = NULL;
	iHeader = HBufC8::NewL(aStream, KLongestStringAllowed);

	delete iFrom;
	iFrom = NULL;
	iFrom = HBufC8::NewL(aStream, KLongestStringAllowed); 

	delete iAppIDString;
	iAppIDString = NULL;
	iAppIDString = HBufC8::NewL(aStream, KLongestStringAllowed); 

	iAppIdInt = aStream.ReadUint32L();
	}


/** 
Constructor, with the message AppID in string form. 

It calls SetPushMsgType() to ensure that TMsvEntry::iBioType is set to the 
correct Push Message Type UID.

@param aAppURI 
AppID in string form. 
*/
EXPORT_C void CPushMsgEntryBase::ConstructL(const TPtrC8& aAppURI)
	{
	ConstructL();
	iAppIDString = aAppURI.AllocL();
	}


/** 
Constructor, with message AppID in numeric form. 

It calls SetPushMsgType() to ensure that TMsvEntry::iBioType is set to the 
correct Push Message Type UID.

@param aAppID 
AppID in numeric form. 
*/
EXPORT_C void CPushMsgEntryBase::ConstructL(const TInt& aAppID)
	{
	ConstructL();
	iAppIdInt=aAppID;
	}

/** 
Constructor. 

It calls SetPushMsgType() to ensure that TMsvEntry::iBioType is set to the 
correct Push Message Type Uid. 
*/
EXPORT_C void CPushMsgEntryBase::ConstructL()
	{
	SetStatus(EPushMsgStatusValid);
	SetPushMsgType();  //Make sure this is set, need it for both StoreL & RestoreL
	}


/** 
Gets the message description field.

@return 
Message description field, or KNullDesC if not set. 
*/
EXPORT_C const TDesC& CPushMsgEntryBase::MsgDescription() const
	{
	if (iMsgDescription)
		return *iMsgDescription;
	else
		return KNullDesC;
	}


/** 
Sets the message description field.

@param aDescription 
Message description field value to copy.
*/
EXPORT_C void CPushMsgEntryBase::SetMsgDescriptionL(const TDesC& aDescription)
	{
	HBufC* tempBuf = aDescription.AllocL();

	delete iMsgDescription;
	iMsgDescription  = tempBuf;
	iEntry.iDescription.Set(*iMsgDescription);
	}


/** 
Gets the message details field.

@return 
Message details field, or KNullDesC if not set. 
*/
EXPORT_C const TDesC& CPushMsgEntryBase::MsgDetails() const
	{
	if (iMsgDetails)
		return *iMsgDetails;
	else
		return KNullDesC;
	}


/** 
Sets the message details field.

@param aDetails 
Message details field value to copy.
*/
EXPORT_C void CPushMsgEntryBase::SetMsgDetailsL(const TDesC& aDetails)
	{
	HBufC* tempBuf = aDetails.AllocL();

	delete iMsgDetails;
	iMsgDetails  = tempBuf;
	iEntry.iDetails.Set(*iMsgDetails);
	}


/** 
Gets the date/time that the push message was received.

@return 
Date/time that the push message was received. 
*/
EXPORT_C const TTime& CPushMsgEntryBase::ReceivedDate() const
	{
	return iEntry.iDate;
	}


/** 
Gets the AppID of the message. 

The AppID can either be an integer or a string. It is returned in one of the two 
parameters, while the third parameter indicates what form it takes.

@param aAppURI 
On return, AppId in string form.

@param aAppID 
On return, AppId in numeric form.

@param aIsAnInt 
On return, true if the AppID is an integer value, false if a string.

@return 
KErrNone if successful.
KErrNotFound if the appId has not been set.
*/
EXPORT_C TInt CPushMsgEntryBase::AppID(TPtrC8& aAppURI, TInt& aAppID, TBool& aIsAnInt) const
	{
	if (iAppIDString && iAppIDString->Length() > 0)
		{
		aAppURI.Set(*iAppIDString);
		aIsAnInt=EFalse;
		return KErrNone;
		}
	else if (iAppIdInt > 0)
		{	
		aAppID=iAppIdInt;
		aIsAnInt=ETrue;
		return KErrNone;
		}
	return KErrNotFound;
	}

