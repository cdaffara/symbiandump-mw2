// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <push/cmultipartpushmsgentry.h>

GLDEF_C TPtrC16 LimitStringSize(const TPtrC16& aString, TInt aMaxSize)
	{
	if (aString.Length() < aMaxSize)
		return aString;
	else
		return aString.Left(aMaxSize);
	}


/** 
Allocates and constructs a new multipart WAP Push message object.

@return 
New multipart WAP Push message object.
*/
EXPORT_C CMultiPartPushMsgEntry* CMultiPartPushMsgEntry::NewL()
	{
	CMultiPartPushMsgEntry* self = new (ELeave) CMultiPartPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new multipart WAP Push message object,
specifying the AppURI as a string.

@param aAppURI
AppURI value as a string.

@return 
New multipart WAP Push message object.
*/
EXPORT_C CMultiPartPushMsgEntry* CMultiPartPushMsgEntry::NewL(const TPtrC8& aAppURI)
	{
	CMultiPartPushMsgEntry* self = new (ELeave) CMultiPartPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppURI);
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new multipart WAP Push message object, 
specifying the AppId as a number.

@param aAppID
AppId value as a number.

@return
New multipart WAP Push message object.
*/
EXPORT_C CMultiPartPushMsgEntry* CMultiPartPushMsgEntry::NewL(TInt& aAppID)
	{
	CMultiPartPushMsgEntry* self = new (ELeave) CMultiPartPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppID);
	CleanupStack::Pop();
	return self;
	}


/** 
Destructor. 
*/
EXPORT_C CMultiPartPushMsgEntry::~CMultiPartPushMsgEntry()
	{
	delete iContentType;
	}


/** 
Sets the Content Type of the message.

@param aContentType 
The Content Type of the message.
*/
EXPORT_C void CMultiPartPushMsgEntry::SetContentTypeL(const TDesC& aContentType)
	{
	HBufC* tempBuf = aContentType.AllocL();

	delete iContentType;
	iContentType  = tempBuf;
	}


/**
Constructor.
*/
CMultiPartPushMsgEntry::CMultiPartPushMsgEntry()
		{
		}


/** 
Gets the push message type.

For this class, the message type UID is KUidWapPushMsgMultiPart.

@return 
Push message type.
*/
TInt32 CMultiPartPushMsgEntry::PushMsgType() const
	{
	return KUidWapPushMsgMultiPart.iUid;
	}


/**
Sets the push message type UID. 

For this class, the message type UID is KUidWapPushMsgMultiPart.
*/
void CMultiPartPushMsgEntry::SetPushMsgType()
	{
	iEntry.iBioType = KUidWapPushMsgMultiPart.iUid;	
	}


/**
Externalises Push message data to a write stream.

Apart from the data stored by the base class, other data stored are Content Type and Time Sent.

@param	aStream	
The stream to which the data should be externalised.

@see	CPushMsgEntryBase::ExternalizeL()
*/	
void CMultiPartPushMsgEntry::ExternalizeL(RMsvWriteStream& aStream)
	{	
	CPushMsgEntryBase::ExternalizeL(aStream);

	aStream<<LimitStringSize(ContentType(), KLongestStringAllowed);
	aStream << iTimeSent.Int64();
	}


/** 
Internalises Push message data from a read stream.

Apart from the data read by the base class, other data read are Content Type and Time Sent.

@param	aStream	
The stream from which the data should be internalised.

@leave HBufC::NewL
@leave CPushMsgEntryBase::InternalizeL

@see	CPushMsgEntryBase::InternalizeL()
*/	
void CMultiPartPushMsgEntry::InternalizeL(RMsvReadStream& aStream)
	{
	CPushMsgEntryBase::InternalizeL(aStream);

	delete iContentType;
	iContentType = NULL;
	iContentType = HBufC::NewL(aStream, KLongestStringAllowed);

	TInt64 timeSent;
	aStream >> timeSent;
	iTimeSent = timeSent;
	}
