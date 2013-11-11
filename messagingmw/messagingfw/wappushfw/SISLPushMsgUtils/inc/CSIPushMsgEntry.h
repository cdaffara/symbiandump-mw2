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

#ifndef __CSIPUSHMSGENTRY_H__
#define __CSIPUSHMSGENTRY_H__

#include <pushentry.h>


/** 
Handles storage of Service Indication (SI) WAP Push Messages in the message server.

It provides additional accessor functions to allow access to all the fields holding 
data for the SI push messages. The additional information for a SI message is:

1. Action: 5 possible action levels are stored in bits 4-7 of TMsvEntry::iMtmData1

2. SI ID (unique identifier for this SI Message).

3. URL (location of the content to present to the user).

4. Creation (date and time of creation of the message). 

5. Expires (date and time after which the message is invalid and should be discarded).

6. Text (additional information sent with the message).

@see	CPushMsgEntryBase

@publishedPartner
@released
*/
class CSIPushMsgEntry : public CPushMsgEntryBase
	{
public:
	/** 
	SI Push messages Action flags. 
	
	@publishedPartner
	@released
	*/
	enum TSIPushMsgAction 
		{			
		// Note that these values get bit-shifted so that they set Bits 4-7 of iMtmData1
		/** Delete action. */
		ESIPushMsgDelete = 0,	// effectively  Bit 4 = 0
		/** Signal none action. */
		ESIPushMsgSignalNone,	//              Bit 4 = 1, i.e. 16   
		/** Signal low action. */
		ESIPushMsgSignalLow,	//              Bit 4 = 0, 5 =1. i.e. 32
		/** Signal medium action. */
		ESIPushMsgSignalMedium,
		/** Signal high action. */
		ESIPushMsgSignalHigh
		};

	IMPORT_C static CSIPushMsgEntry* NewL();
	IMPORT_C static CSIPushMsgEntry* NewL(const TPtrC8& aAppURI);
	IMPORT_C static CSIPushMsgEntry* NewL(TInt& aAppID);
	IMPORT_C ~CSIPushMsgEntry();
	inline const TDesC& Id() const;
	IMPORT_C void SetIdL(const TDesC& aID);
	IMPORT_C TInt Action() const;
	IMPORT_C void SetAction(TInt aActionFlags);

	inline const TTime& Created() const;
	inline void	SetCreated(const TTime& aCreated);
	inline const TTime& Expires() const;
	inline void	SetExpires(const TTime& aExpires);
	inline const TDesC&  Url() const;
	inline const TDesC& ContentType() const;

	IMPORT_C void SetUrlL(const TDesC& aUrl);
	IMPORT_C const TDesC& Text() const;
	IMPORT_C void SetTextL(const TDesC& aText);
	IMPORT_C TInt32 PushMsgType() const;
	
	IMPORT_C void SetTrusted(TBool aTrusted);
	IMPORT_C TBool Trusted() const;
	IMPORT_C void SetMsgOriginUriL(const TDesC8& aOriginUri);
	IMPORT_C const TDesC8& MsgOriginUri() const;

private:  //functions
	CSIPushMsgEntry();
	void ExternalizeL(RMsvWriteStream& aStream);
	void InternalizeL(RMsvReadStream& aStream);
	void SetPushMsgType();

private: //data
	TTime	iExpiryDate;
	TTime	iCreatedDate;
	HBufC*	iText;
	HBufC*	iUrl;
	HBufC*  iMsgID;
	HBufC8*	iOriginUri;
	};

#include <push/csipushmsgentry.inl>

#endif // __CSIPUSHMSGENTRY_H__
