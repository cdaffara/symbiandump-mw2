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

#include <push/csipushmsgentry.h>

GLDEF_C TPtrC16 LimitStringSize(const TPtrC16& aString, TInt aMaxSize)
	{
	if (aString.Length() < aMaxSize)
		return aString;
	else
		return aString.Left(aMaxSize);
	}


/** 
Allocates and constructs a new Service Indication WAP Push message entry.

@return 
New Service Indication WAP Push message entry. 
*/
EXPORT_C CSIPushMsgEntry* CSIPushMsgEntry::NewL()
	{
	CSIPushMsgEntry* self = new (ELeave) CSIPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new Service Indication WAP Push message entry, specifying
the AppId as a string.

@param aAppURI 
AppId in string form.

@return 
New Service Indication WAP Push message entry. 
*/
EXPORT_C CSIPushMsgEntry* CSIPushMsgEntry::NewL(const TPtrC8& aAppURI)
	{
	CSIPushMsgEntry* self = new (ELeave) CSIPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppURI);
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new Service Indication WAP Push message entry, specifying
the AppId as a number.

@param aAppID
AppId in integer form.

@return 
New Service Indication WAP Push message entry. 
*/
EXPORT_C CSIPushMsgEntry* CSIPushMsgEntry::NewL(TInt& aAppID)
	{
	CSIPushMsgEntry* self = new (ELeave) CSIPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppID);
	CleanupStack::Pop();
	return self;
	}


/**
Destructor.
*/
EXPORT_C CSIPushMsgEntry::~CSIPushMsgEntry()
	{
	delete iText;
	delete iUrl;
	delete iMsgID;
	delete iOriginUri;
	}


/** 
Sets the ID of SI Push Entry.

@param aID 
The SI message ID.
*/
EXPORT_C void CSIPushMsgEntry::SetIdL(const TDesC& aID)
	{
	HBufC* tempID = aID.AllocL();

	delete iMsgID;
	iMsgID = tempID;
	}


/** 
Gets the Action level for the SI Push Message. 

The Action levels are defined by TSIPushMsgAction.

@return 
Action level for the SI Push Message.

@see	CSIPushMsgEntry::TSIPushMsgAction
*/
EXPORT_C TInt CSIPushMsgEntry::Action() const
	{
	//Action is stored in Bits 4-7 of iMtmData1 of the TMsvEntry in the Message Server Index.
	//Therefore the function has to mask out all the other bits, and then bitwise shift the
	//values to get the Action level. 

	TInt action = iEntry.MtmData1() & KPushMaskOnlyAction; //Get the value of Bits 4-7 for action	
	action = action >> 4;	// bitshift 4 places right
	return action;
	}


/** 
Sets the Action level for the SI Push Message. 

The Action levels are defined by TSIPushMsgAction.

@param aActionFlags 
Action level for the SI Push Message.

@see	CSIPushMsgEntry::TSIPushMsgAction
*/
EXPORT_C void CSIPushMsgEntry::SetAction(TInt aActionFlags)
	{
	//Action value is stored in Bits 4-7 of iMtmData1 of the TMsvEntry in the Message Server
	//Index. Therefore the function has to clear the old action bits through bitwise AND with
	//a mask, and then bitwise shift the action level to the correct Bit position. 
	TInt everythingButAction = iEntry.MtmData1() & KPushMaskEverythingButAction;

	TInt action = aActionFlags << 4;  // bitshift left 4 places, i.e to Bits 4-7 
	action =  action & KPushMaskOnlyAction; //eliminate any non action bits
	iEntry.SetMtmData1( action + everythingButAction );
	}


/** 
Sets the URL of the service to be accessed.

@param aUrl
URL of the service to be accessed. 
*/
EXPORT_C void CSIPushMsgEntry::SetUrlL(const TDesC& aUrl)
	{
	HBufC* tempBuf = aUrl.AllocL();

	delete iUrl;
	iUrl = tempBuf;
	}


/** 
Gets the notification text of the SI message.

@return 
SI Text information
*/
EXPORT_C const TDesC& CSIPushMsgEntry::Text() const
	{
	if (iText)
		return *iText;
	else
		return KNullDesC;
	}


/**
Sets the notification text for the SI entry.

@param aText
Notification text to be stored.
*/
EXPORT_C void CSIPushMsgEntry::SetTextL(const TDesC& aText)
	{
	HBufC* temp = aText.AllocL();
	delete iText;
	iText = NULL;
	iText = temp;
	}


/**
Constructor. 

Initialises expiry date to Null.
*/
CSIPushMsgEntry::CSIPushMsgEntry()
	{
	iExpiryDate = Time::NullTTime();
	}


/** 
Externalises the object to a message store stream.

@param aStream
The stream to which the data should be externalised.
*/
void CSIPushMsgEntry::ExternalizeL(RMsvWriteStream& aStream)
	{// Call the Base Class method to store iHeader & iFrom.
	 // Custom code for iExpiryDate. Data in iText & iUrl assigned to iEntry - no problem
	CPushMsgEntryBase::ExternalizeL(aStream);
	aStream << iExpiryDate.Int64();
	aStream << LimitStringSize(Text(), KLongestStringAllowed);
	aStream<< LimitStringSize(Url(), KLongestStringAllowed);;
	aStream << iCreatedDate.Int64();

	if (iMsgID)
		aStream << *iMsgID;
	else
		aStream << KNullDesC;
	
	aStream << MsgOriginUri();
	}


/** 
Internalises the object from a message store stream.

@param aStream
The stream from which the data should be internalised.
*/
void CSIPushMsgEntry::InternalizeL(RMsvReadStream& aStream)
	{ 
	// Base class restores iHeader & iFrom. We only have to do iExpiryDate
	CPushMsgEntryBase::InternalizeL(aStream);	

	//Read in the Time & Date as a 64 bit int
	TInt64 expiryTime;
	aStream >> expiryTime;
	iExpiryDate = expiryTime;

	delete iText;
	iText = NULL;
	iText = HBufC::NewL(aStream, KLongestStringAllowed);

	delete iUrl;
	iUrl = NULL;
	iUrl = HBufC::NewL(aStream, KLongestStringAllowed);

	//Read in Created Time & Date as a 64 bit int
	TInt64 createdTime;
	aStream >> createdTime;
	iCreatedDate = createdTime;

	delete iMsgID;
	iMsgID = NULL;
	iMsgID = HBufC::NewL(aStream, KLongestStringAllowed);
	
	delete iOriginUri;
	iOriginUri = NULL;
	iOriginUri = HBufC8::NewL(aStream, KLongestStringAllowed);
	}


/** 
Gets the SI push message type UID.

For this class, this is KUidWapPushMsgSI.

@return 
Push message type UID as a long integer 
*/
EXPORT_C TInt32 CSIPushMsgEntry::PushMsgType() const
	{
	return KUidWapPushMsgSI.iUid;
	}


/** 
Sets the SI push message type UID. 

For this class, this is KUidWapPushMsgSI. 
*/
void CSIPushMsgEntry::SetPushMsgType()
	{
	iEntry.iBioType = KUidWapPushMsgSI.iUid;
	}


/**
Sets the 8th bit of TMsvEntry::iMtmData1 to indicate whether the SI message is from a
whitelisted source or from an unknown origin.

@param aTrusted ETrue - push PDU source address is in whitelist.
				EFalse - push PDU is from unknown origin.

@see TMsvEntry
*/
EXPORT_C void CSIPushMsgEntry::SetTrusted(TBool aTrusted)
	{
	(aTrusted == 1) ? (iEntry.SetMtmData1((iEntry.MtmData1()) | 0x00000100)) : (iEntry.SetMtmData1((iEntry.MtmData1()) & 0xFFFFFEFF));	
	}


/**
Reads the 8th bit of TMsvEntry::iMtmEntry1 and returns true if the SI message is from a
whitelisted origin and false if the SI message is from an unknown origin.

@return ETrue - push PDU source address is in whitelist.
		EFalse - push PDU is from unknown origin.
*/
EXPORT_C TBool CSIPushMsgEntry::Trusted() const
	{
	// Unmask and return the bit-8
	 return (((iEntry.MtmData1() & 0x00000100) == 0x00000100) ? ETrue : EFalse);
	}
	
	
/**
Sets the origin URI of the SI message.

@param aOriginUri Origin URI of the SI message.
*/
EXPORT_C void CSIPushMsgEntry::SetMsgOriginUriL(const TDesC8& aOriginUri)
	{
	delete iOriginUri;
	iOriginUri = NULL;
	iOriginUri = aOriginUri.AllocL();
	}


/**
Gets the origin URI of the SI message.

@return Origin URI of the SI message.
*/
EXPORT_C const TDesC8& CSIPushMsgEntry::MsgOriginUri() const
	{
	if (iOriginUri)
		{
		return (*(iOriginUri));
		}
	else
		{
		return KNullDesC8;
		}
	}







