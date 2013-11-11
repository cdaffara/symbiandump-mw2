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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __PUSHENTRY_H__
#define __PUSHENTRY_H__

#include <e32std.h>
#include <msvapi.h>
#include <msvstd.h>


/** UID for the WAP Push MTM. */
const TUid KUidMtmWapPush = {0x10009158};
/** UID for WAP Push SI messages. */
const TUid KUidWapPushMsgSI = {0x10009159};
/** UID for WAP Push SL messages. */
const TUid KUidWapPushMsgSL = {0x1000915A};
/** UID for WAP Push multipart messages. */
const TUid KUidWapPushMsgMultiPart = {0x1000915B};
/** UID for WAP Push unknown-type messages. */
const TUid KUidWapPushMsgUnknown = {0x1000915C};


// Defines for Masking values used to extract status information from iMtmData1
/** TMsvEntry::iMtmData1 mask to get Status value. */
const TInt32 KPushMaskOnlyStatus			= 0x0000000F;
/** TMsvEntry::iMtmData1 mask to all bits except those for the Status value. */
const TInt32 KPushMaskEverythingButStatus	= 0xFFFFFFF0;
/** TMsvEntry::iMtmData1 mask to get Action value. */
const TInt32 KPushMaskOnlyAction			= 0x000000F0;
/** TMsvEntry::iMtmData1 mask to all bits except those for the Action value. */
const TInt32 KPushMaskEverythingButAction	= 0xFFFFFF0F;


// Defines for Leave Codes
/** Base value for WAP Push-specific errors. */
const TInt KWPushBaseError		 =	-1100;
/** CPushMsgEntryBase::UpdateL() called without a message server entry being loaded 
error. */
const TInt KWPushNoMsgIndexEntry =   (KWPushBaseError-1);
/** Longest String Allowed. */
const TInt KLongestStringAllowed=KMaxTInt32;


/**
WAP Push Server MTM state information.

This enum is used by TPushMTMProgress and CWapPushServerMtm.

@publishedPartner
@released
@see TPushMTMProgress 
*/
enum TPushServerMTMState
	{
	/** Server is waiting for a command. */
	EWapPushServerMtmWaiting,
	/** Server is executing a Change command. */
	EWapPushServerMtmChange,
	/** Server is executing a Create command. */
	EWapPushServerMtmCreate,
	/** Server is executing a Delete command. */
	EWapPushServerMtmDelete,
	/** Unused. */
	EWapPushServerMtmComplete
	};


/** 
Encapsulates progress information returned for asynchronous operations with 
the WAP Push Server MTM. 

This allows error status and ID of current entry to be returned to caller.

@publishedPartner
@released
@see CMsvOperation::ProgressL() 
*/
class TPushMTMProgress
	{
public:
	/** Error code. */
	TInt				iError; // client side needs to be informed of an error
	/** The number of entries on which the operation was or will be attempted. */
	TInt				iTotalMsgs;
	/** The number of entries which have been processed. */
	TInt				iMsgsProcessed;
	/** ID of the (first) message being processed. */
	TMsvId				iMsgId;
	/** Server state information. */
	TPushServerMTMState iPushState;// Operation that is currently in progress
	};

/** 
Packages a TPushMTMProgress object into a descriptor. 

@publishedPartner
@released
*/
typedef TPckgBuf<TPushMTMProgress> TPushMTMProgressBuf;

#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
/** Panic name and codes */
_LIT(KPushPanicMoniker, "PUSHENTRY");

/**
@internalComponent
@released
*/
enum TPushEntryPanic	
	{
	/** Null Ids for entries not valid (Service, &Msg). */
	EPushEntryNullMsgId,
	/** Only save messages under the Service Entry. */
	EPushEntryServiceIdMismatch,
	/** Not a WAP Push  MTM. */
	EPushEntryWrongMTMtype,
	/** iBioType value in TMsvEntry don't match Uid for Push Entry class. */
	EPushEntryWrongMsgtype
	};
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS

/** 
Abstract base class for classes that handle storage of WAP Push Messages in 
the message server.

The class acts as a wrapper for TMsvEntry, and provides accessors and storage 
functionality. Key pieces of data are mapped onto certain TMsvEntry fields: 
this speeds up access by applications to this data. This base class has the 
generic mappings and storage behaviour. More specialised mappings and storage 
are defined in each derived class.

The mappings of data to TMsvEntry fields standard to all classes are:

Push Message Type UID: TMsvEntry::iBioType

Status: sets bits 0-3 of TMsvEntry::iMtmData1

Some derived classes need to store more data than there are available fields 
in TMsvEntry, and the Unknown type potentially requires large data capacity. 
This additional data is saved in the message store associated with the message 
entry. Derived classes provide this functionality by overriding ExternalizeL() 
and InternalizeL() implementations. The base class itself uses the message store
to store the WAP Push message's raw header and From information.

@publishedPartner
@released
*/
class CPushMsgEntryBase: public CBase
   {

public:
	
	IMPORT_C CPushMsgEntryBase();

	IMPORT_C virtual ~CPushMsgEntryBase();

	// Creates its own CMsvStore from the CMsvSession & calls StoreL()
	IMPORT_C virtual TMsvId SaveL(CMsvSession& aSession, TMsvId aParent);
	IMPORT_C virtual void UpdateL(CMsvSession& aSession);

	// Creates its own CMsvStore from the CMsvSession & calls RestoreL()
	IMPORT_C virtual void RetrieveL(CMsvSession& aSession, TMsvId aMsgId);
	
	IMPORT_C void SetEntry(const TMsvEntry& aEntry);
	IMPORT_C void SetStatus(TInt aStatusFlags);
	IMPORT_C const TDesC8&  Header() const;
	IMPORT_C void SetHeaderL(const TDesC8& aHeader);
	IMPORT_C const TDesC8& From() const; 
	IMPORT_C void SetFromL(const TDesC8& aFrom);

	/**
	Gets the push message content type.
	
	@return
	Content type 
	*/
	virtual const TDesC& ContentType() const =0;
	
	/** 
	Gets the push message type.
	
	Each derived class represents a different push message type, and implements 
	this to return the appropriate value.
	
	@return 
	Push message type
	*/
	virtual TInt32 PushMsgType() const =0;
	inline const TMsvEntry& Entry() const;
	inline TInt Status() const;

	IMPORT_C const TDesC& MsgDescription() const;
	IMPORT_C void SetMsgDescriptionL(const TDesC& aDescription);
	IMPORT_C const TDesC& MsgDetails() const;
	IMPORT_C void SetMsgDetailsL(const TDesC& aDetails);
	IMPORT_C const TTime& ReceivedDate() const;
	IMPORT_C TInt AppID(TPtrC8& aAppURI, TInt& aAppID, TBool& aIsAnInt) const;

protected:
	IMPORT_C virtual void ConstructL(const TPtrC8& aAppURI);
	IMPORT_C virtual void ConstructL(const TInt& aAppID);
	IMPORT_C virtual void ConstructL();

	// No  matching StoreL due to different needs for saving & updating msg's
	IMPORT_C void RestoreL(CMsvStore& aStore );
	IMPORT_C virtual void ExternalizeL(RMsvWriteStream& aStream);
	IMPORT_C virtual void InternalizeL(RMsvReadStream& aStream);
	
	/** 	
	Sets the push message type.
	
	Each derived class represents a different push message type, and implements 
	this to set the appropriate value. 
	*/
	virtual void SetPushMsgType()=0;// Only the object can set it's push type - class dependent 
	void DoSaveL(CMsvEntry& aMsvEntry);

protected:

	/** Message server entry. */
	TMsvEntry iEntry;

	/** Copy of the message server entry Details field. */
	HBufC* iMsgDetails;    
	/** Copy of the message server entry Description field. */
	HBufC* iMsgDescription;
							    
	/** WAP Push message raw header buffer. */
	HBufC8* iHeader;
	/** From field buffer. */
	HBufC8* iFrom;
	/** Set to true if the object has an associated entry in the message server index. */
	TBool iHasServerEntry;

	/** Message AppID as a string. Only one of iAppIDString and iAppIdInt will be set.*/
	HBufC8* iAppIDString;
	/** Message AppID as a number. Only one of iAppIDString and iAppIdInt will be set.*/
	TInt iAppIdInt;

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
private:
    /** 
	WAP Push Message status values. 
	*/
	enum TPushMsgStatus	
		{
		/** Message is valid. */
		EPushMsgStatusValid = 0,
		/** Message is expired. */
		EPushMsgStatusExpired,
		/** Message is deleted. */
		EPushMsgStatusDeleted,
		/** Message is corrupt. */
		EPushMsgStatusCorrupt
		};
private:
	friend class CWapPushMsgUtils;
    friend class CMultiPartRelAndAltContentHandler;
	friend class CSISLPushMsgUtils;
#else
public:
    /** 
	WAP Push Message status values. 
	*/
	enum TPushMsgStatus	
		{
		/** Message is valid. */
		EPushMsgStatusValid = 0,
		/** Message is expired. */
		EPushMsgStatusExpired,
		/** Message is deleted. */
		EPushMsgStatusDeleted,
		/** Message is corrupt. */
		EPushMsgStatusCorrupt
		};
#endif//SYMBIAN_ENABLE_SPLIT_HEADERS
	};

#include <pushentry.inl>

#endif
