// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSICONTENTHANDLER_H__
#define __CSICONTENTHANDLER_H__

// System includes
//
#include <e32base.h>

// Local includes
//
#include <push/ccontenthandlerbase.h>
#include "CMsgParser.h"

// Constants
//
const TUid KUidPushSIContentHandler	= { 0x101F3E61 };
_LIT(KSIContentHandlerData,"text/vnd.wap.si||application/vnd.wap.sic");

// Forward class declarations
//
class CSIPushMsgEntry;
class CSISLPushMsgUtils;

/** 
 * CSIContentHandler handles Service Indication content  
 *
 * This handler takes ownership of the SI message and processes it
 * according to the Reception rules 
 * [WAP Service Indication version 08-Nov-1999 section 7.2]
 * and either stores the message or deletes it.
 *
 * OVERVIEW:
 *	The main body of this class and its functionality lies within
 *	HandleMessage (asynchonous). This :
 *		1. takes ownership of the push message
 *		2. validates the message and generates a parsed document tree
 *		3. extracts the message attribute values from document tree 
 *		4. processes the received message
 *		5. finishes
 *
 *	Step 4: is an algorithmic implementation of 7.2 Reception rules
 *		from WAP Service Indication 08-Nov-1999 with the exception
 *		that the final step (step 5: handling of information specified
 *		in the info element) has not been implemented as this is a UI
 *		feature. 
 *
 *		NB: a received message with no creation date will be stored (unless
 *		its action is set to delete or the expiry condition is met). This 
 *		means that the si-id in the message store will not be unique.
 */
//##ModelId=3B659DB10116
class CSIContentHandler : public CContentHandlerBase, public MWAPElementHandler
/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CSIContentHandler* NewL();

	virtual ~CSIContentHandler();

private:	// Methods from CPushHandlerBase
	
	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	virtual void HandleMessageL(CPushMessage* aPushMsg);

	virtual void CancelHandleMessage();

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();

private:
	// From MWAPElementHandler
	void HandleElementL ( const RString& aTag, const RString& aAttributeName, const RString& aAttributeValue );
	void HandleContentL ( const TDesC8& aBytes );

private:	// Methods from CActive

	virtual void DoCancel();

	virtual void RunL();

	virtual TInt RunError(TInt aError);

private:	// Methods

	CSIContentHandler();

	void  ConstructL();

	void  ParsePushMsgL();

	void  SetSIPushMsgEntryFieldsL(CSIPushMsgEntry& aSIPushMsgEntry);

	void  ProcessingPushMsgEntryL();

	void StoreSIMessageL();

	TBool HandleMsgOrderReceptionL();

	// conversion utilities

	TBool ConvertDateTimeL(const TDesC& aDateTime, TTime &aConvertedDate);

	TBool IsValidUTCTime(TDes& aDateTime);

	HBufC16* FormatDateStringL(const TDesC& aData);

	TUint ConvertActionString(const RString& aActionString);

	CSIPushMsgEntry* GetSiEntryL();
	
	TBool ValidateDateTime(TDes& aDateTime);

	// Attribute flags
	inline void  SetActionFlag(TBool aAction);

	inline void  SetCreatedFlag(TBool aCreated);

	inline void  SetHrefFlag(TBool aHref);

	inline void  SetExpiresFlag(TBool aExpires);

	inline void  SetSiIdFlag(TBool aSiId);

	inline void  SetDataFlag(TBool aData); 

	inline TBool ActionFlag() const;

	inline TBool CreatedFlag() const;

	inline TBool HrefFlag() const;

	inline TBool ExpiresFlag() const;
	
	inline TBool SiIdFlag() const;

	inline TBool DataFlag() const;

private:	// Attributes

	CSISLPushMsgUtils*  iWapPushUtils;
	
	TUint32				iAttributes;

	enum { EAction=0x01, ECreated=0x02, EHref=0x04, EExpires=0x08, ESiId=0x10, EData=0x20 };
	
	enum TState { EParsing, EProcessing, EDone };

	TInt				iPushMsgAction;	

	HBufC*				iHrefBuf;		

	HBufC*				iSiIdBuf;
	
	TTime				iExpiresTime;

	TTime				iCreatedTime;

	HBufC*				iData;

	TBool				iIndicationTagFound;
	
	TBool               iValidDate;	
	};

#include "CSIContentHandler.inl"

#endif	// __CSICONTENTHANDLER_H__
