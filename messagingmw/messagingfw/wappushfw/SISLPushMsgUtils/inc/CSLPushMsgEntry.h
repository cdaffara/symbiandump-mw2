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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __CSLPUSHMSGENTRY_H__
#define __CSLPUSHMSGENTRY_H__

#include <pushentry.h>


/** 
Handles storage of Service Loading (SL) WAP Push Messages in the message server.

There are three items of data that are stored in addition to the data held by 
the base class. These are:

1. Action: action levels are stored in bits 4-7 of TMsvEntry::iMtmData1

2. URL (location of the service to load).

3. The time that the message was sent.

@see	CPushMsgEntryBase
  
@publishedPartner
@released
*/
class CSLPushMsgEntry : public CPushMsgEntryBase
	{
public:
	/** 
	SL Push messages Action flags. 
	
	@publishedPartner
	@released
	*/
	enum TSLPushMsgAction 
		{ 
		//Note that these values get bit-shifted so that they set Bits 4-7 of iMtmData1.
		/** Execute cache action. */							
		ESLPushMsgExecuteCache	 = 0,	// effectively Bit 4 = 0
		/** Execute low action. */
		ESLPushMsgExecuteLow,	//			  Bit 4 = 1, i.e. 16   
		/** Execute high action. */
		ESLPushMsgExecuteHigh	//	  Bit 4 = 0, 5 =1. i.e. 32
		};		

	IMPORT_C static	CSLPushMsgEntry* NewL();
	IMPORT_C static CSLPushMsgEntry* NewL(const TPtrC8& aAppURI);
	IMPORT_C static CSLPushMsgEntry* NewL(TInt& aAppID);
	
	IMPORT_C ~CSLPushMsgEntry();
	IMPORT_C TInt Action() const;
	IMPORT_C void SetAction(TInt aActionFlags);
	inline const TDesC&	Url() const;
	IMPORT_C void SetUrlL(const TDesC& aUrl);
	inline void	SetTimeSent(const TTime& aTime);
	inline const TTime& TimeSent() const;
	TInt32 PushMsgType() const;
	inline const TDesC& ContentType() const;
	
	IMPORT_C void SetTrusted(TBool aTrusted);
	IMPORT_C TBool Trusted() const;
	IMPORT_C void SetMsgOriginUriL(const TDesC8& aOriginUri);
	IMPORT_C const TDesC8& MsgOriginUri() const;

private:
	CSLPushMsgEntry();
	inline void SetPushMsgType();
	void ExternalizeL(RMsvWriteStream& aStream);
	void InternalizeL(RMsvReadStream& aStream);

private:
	TTime	iTimeSent;
	HBufC*	iUrl;
	HBufC8*	iOriginUri;
	};

#include <push/cslpushmsgentry.inl>

#endif // __CSLPUSHMSGENTRY_H__
