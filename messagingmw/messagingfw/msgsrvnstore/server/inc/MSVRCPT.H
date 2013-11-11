// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__MSVRCPT_H__)
#define __MSVRCPT_H__

class RWriteStream;

//**********************************
// CMsvRecipient
//**********************************
//
// Base class for  multiple recipients
//

class CMsvRecipient : public CBase
/** Base class for information relating to a message recipient. 
@publishedAll
@released
*/
	{
public:
	/** Message sending status for a recipient. 
@publishedAll
@released
*/
	enum TRecipientStatus 
							{	
	/** Message is not sent. */
							ENotYetSent=0,
	/** Message has been sent. */
							ESentSuccessfully=1,
	/** Message sending failed. */
				 			EFailedToSend=2};
	IMPORT_C virtual void InternalizeL(RReadStream& aStream);
	IMPORT_C virtual void ExternalizeL(RWriteStream& aStream) const;
	inline TRecipientStatus Status() const;
	inline void SetStatus(TRecipientStatus aStatus);
	inline TInt Error() const;
	inline void SetError(TInt aError);
	inline const TTime& Time() const;
	inline TTime& Time();
	inline TInt Retries() const;
	inline void IncreaseRetries();
	inline void ResetRetries();
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)	
	inline void SetRetries(TInt aRetries);
	inline void SetTimeValue(TInt64 aTime);
#endif
protected:
	IMPORT_C CMsvRecipient();
private:
	TRecipientStatus iStatus;
	TInt   iError;
	TInt   iRetries;
	TTime	 iTime;
	};

#include <msvrcpt.inl>

#endif
