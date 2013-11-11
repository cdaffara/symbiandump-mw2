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

#include <push/cslpushmsgentry.h>


GLREF_C TPtrC16 LimitStringSize(const TPtrC16& aString, TInt aMaxSize);


/** 
Allocates and constructs a new Service Loading WAP Push message entry.

@return 
New Service Loading WAP Push message entry.
*/
EXPORT_C CSLPushMsgEntry* CSLPushMsgEntry::NewL()
	{
	CSLPushMsgEntry* self = new (ELeave) CSLPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}


/** 
Allocates and constructs a new Service Loading WAP Push message entry, specifying
the AppId as a string.

@param aAppURI
AppId in string form.

@return 
New Service Loading WAP Push message entry.
*/
EXPORT_C CSLPushMsgEntry* CSLPushMsgEntry::NewL(const TPtrC8& aAppURI)
	{
	CSLPushMsgEntry* self = new (ELeave) CSLPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppURI);
	CleanupStack::Pop();
	return self;
	}


/**
Allocates and constructs a new Service Loading WAP Push message entry, specifying
the AppId as a number.

@param aAppID 
AppId in integer form.

@return 
New Service Loading WAP Push message entry.
*/
EXPORT_C CSLPushMsgEntry* CSLPushMsgEntry::NewL(TInt& aAppID)
	{
	CSLPushMsgEntry* self = new (ELeave) CSLPushMsgEntry();
	CleanupStack::PushL(self);
	self->ConstructL(aAppID);
	CleanupStack::Pop();
	return self;
	}


/**
Destructor.
*/
EXPORT_C CSLPushMsgEntry::~CSLPushMsgEntry()
	{
	delete iUrl;
	delete iOriginUri;
	}


/**
Gets the Action level for the SL Push Message. 

The Action levels are defined by TSLPushMsgAction.

@return 
Action level for the SL Push Message.

@see	CSLPushMsgEntry::TSLPushMsgAction
*/
EXPORT_C TInt CSLPushMsgEntry::Action() const
	{
	//Action is stored in Bits 4-7 of iMtmData1 of the TMsvEntry in the 
	//Message Server Index. The function has to mask out all the other bits, 
	//and then bitwise shift the values to get the Action level

	TInt action = iEntry.MtmData1() & KPushMaskOnlyAction; //Get the value of Bits 4-7 for action	
	action = action >> 4;	// bitshift 4 places right
	return action;
	}


/**
Sets the Action level for the SL Push Message. 

The Action levels are defined by TSLPushMsgAction.

@param aActionFlags
Action level for the SL Push Message. 

@see	CSLPushMsgEntry::TSLPushMsgAction
*/
EXPORT_C void	CSLPushMsgEntry::SetAction(TInt aActionFlags)
	{
	//Action value is stored in Bits 4-7 of iMtmData1 of the TMsvEntry in the 
	//Message Server Index. The function clears the  old action bits with a 
	//bitwise AND, and then does a bitwise shifts on the action level to set
	//the iMtmData1 value. 

	TInt everythingButAction = iEntry.MtmData1() & KPushMaskEverythingButAction;

	TInt action = aActionFlags << 4;  // bitshift left 4 places, i.e to Bits 4-7 
	action =  action & KPushMaskOnlyAction; //eliminate any non action bits
	iEntry.SetMtmData1(action + everythingButAction);
	}


/** 
Sets the URL of the service to be loaded.

@param aUrl 
URL of the service to be loaded. 
*/
EXPORT_C void CSLPushMsgEntry::SetUrlL(const TDesC& aUrl)
	{
	HBufC* tempBuf = aUrl.AllocL();

	delete iUrl;
	iUrl = tempBuf;
	}


/**
Constructor.
*/
CSLPushMsgEntry::CSLPushMsgEntry()
	{
	}


/**
Gets the push message type.

For this class, this is KUidWapPushMsgSL.

@return 
Push message type. This is a UID expressed as an integer. 
*/
TInt32 CSLPushMsgEntry::PushMsgType() const
	{
	return KUidWapPushMsgSL.iUid;
	}


/** 
Sets the push message type UID. 

For this class, this is KUidWapPushMsgSL. 
*/
void CSLPushMsgEntry::SetPushMsgType()
	{
	iEntry.iBioType = KUidWapPushMsgSL.iUid;	
	}


/** 
Externalises the object to a message store stream.

@param aStream
The stream to which the data should be externalised.
*/
void CSLPushMsgEntry::ExternalizeL(RMsvWriteStream& aStream)
	{
	CPushMsgEntryBase::ExternalizeL(aStream);

	aStream<<LimitStringSize(Url(), KLongestStringAllowed);;
	aStream<< iTimeSent.Int64();
	aStream<< MsgOriginUri();
	}


/** 
Internalises the object from a message store stream.

@param aStream
The stream from which the data should be internalised.
*/
void CSLPushMsgEntry::InternalizeL(RMsvReadStream& aStream)
	{ 
	CPushMsgEntryBase::InternalizeL(aStream);	

	delete iUrl;
	iUrl = NULL;
	iUrl = HBufC::NewL(aStream, KLongestStringAllowed);

	TInt64 msgDateTime;
	aStream >> msgDateTime;
	iTimeSent = msgDateTime;
	
	delete iOriginUri;
	iOriginUri = NULL;
	iOriginUri = HBufC8::NewL(aStream, KLongestStringAllowed);
	}


/**
Sets the 8th bit of TMsvEntry::iMtmData1 to indicate whether the SL message is from a
whitelisted source or from an unknown origin.

@param aTrusted ETrue - push PDU source address is in whitelist.
				EFalse - push PDU is from unknown origin.

@see TMsvEntry
*/
EXPORT_C void CSLPushMsgEntry::SetTrusted(TBool aTrusted)
	{
	(aTrusted == 1) ? (iEntry.SetMtmData1((iEntry.MtmData1()) | 0x00000100)) : (iEntry.SetMtmData1((iEntry.MtmData1()) & 0xFFFFFEFF));	
	}


/**
Reads the 8th bit of TMsvEntry::iMtmEntry1 and returns true if the SL message is from a
whitelisted origin and false if the SL message is from an unknown origin.

@return ETrue - push PDU source address is in whitelist.
		EFalse - push PDU is from unknown origin.
*/
EXPORT_C TBool CSLPushMsgEntry::Trusted() const
	{
	// Unmask and return the bit-8
	 return (((iEntry.MtmData1() & 0x00000100) == 0x00000100) ? ETrue : EFalse);
	 }
	
	
/**
Sets the origin URI of the SL message.

@param aOriginUri Origin URI of the SL message.
*/
EXPORT_C void CSLPushMsgEntry::SetMsgOriginUriL(const TDesC8& aOriginUri)
	{
	delete iOriginUri;
	iOriginUri = NULL;
	iOriginUri = aOriginUri.AllocL();
	}


/**
Gets the origin URI of the SL message.

@return Origin URI of the SL message.
*/
EXPORT_C const TDesC8& CSLPushMsgEntry::MsgOriginUri() const
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







