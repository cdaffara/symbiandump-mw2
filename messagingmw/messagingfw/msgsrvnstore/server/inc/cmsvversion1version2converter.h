// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMSVVERSION1VERSION2CONVERTER_H__
#define __CMSVVERSION1VERSION2CONVERTER_H__



#include <s32mem.h>
#include <badesca.h>
#include "msvdbadapter.h"
#include "msvmessagedbadapter.h"
#include "cmessageconvertermanager.h"
#include "cmsvconverterwaiter.h"
#include "CMsvCachedStore.h"

// fwd declaration
class MMsvMessageStoreConverter;

class TPairedTUidHBufC8;
/** TCommonHeaderField
Enum specifying the common header fields
*/
enum TCommonHeaderField
	{
	EFrom,
	ETo,
	ECC,
	EBCC,
	ESubject
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	,EInvalid
#endif	
	};

typedef TInt32	TLogId;

/**
Defines the status of an acknowledgement for a recipient.
*/	
	enum TSmsAckStatus
		{
/**
An acknowledgement for this recipient has not been requested.
*/
		ENoAckRequested		= 0,
/**
The delivery status for this recipient has been requested, but the status report
has not yet been received.
*/
		EPendingAck,
/**
A successful acknowledgement for this recipient has been received.
*/
		EAckSuccessful,
/**
A failed acknowledgement for this recipient has been received.
*/
		EAckError
		};


/** TMsgOutboxBodyEncoding
Defines how emails will be encoded when they are sent.

@internalAll 
@released
*/

enum TMsgOutboxBodyEncoding
	{
	EMsgOutboxDefault,
	EMsgOutboxNoAlgorithm,
	EMsgOutboxMIME,
	EMsgOutboxMHTMLAsMIME,
	EMsgOutboxMHTMLAlternativeAsMIME
	};



/** Message sending status for a recipient. 
*/
	enum TRecipientStatus 
							{	
	/** Message is not sent. */
							ENotYetSent=0,
	/** Message has been sent. */
							ESentSuccessfully=1,
	/** Message sending failed. */
				 			EFailedToSend=2};



/**
TImHeaderEncodingInfo
Identifies the header encoding info.

@internalAll 
@released
*/

class TImHeaderEncodingInfo
	{
public:

	enum TFieldList 
		{ 
		EFrom, 		/**< "From" field. */
		EReplyTo, 	/**< "Reply To" field. */
		ETo, 		/**< "To" field. */
		ECc, 		/**< "Cc" field. */
		EBcc, 		/**< "Bcc" field. */
		ESubject	/**< Subject field. */
		};
	/** Encoding types. */
	enum TEncodingType 
		{ 
		EBase64, 		/**< Base64. */
		EQP, 			/**< QP. */
		ENoEncoding, 	/**< No encoding */
		EUU				/**< UU. */
		};

public:
	TImHeaderEncodingInfo();
	TImHeaderEncodingInfo(const TImHeaderEncodingInfo& aFrom);
	void InternalizeL( RReadStream& aReadStream );
	void SetField(const TFieldList aField);
	TFieldList Field() const;
	void SetOffset(const TInt aOffset);
	TInt Offset() const;
	void SetLength(const TInt aLength);
	TInt Length() const;
	void SetCharsetUid(const TUint aUid);
	TUint CharsetUid() const;
	void SetEncodingType(const TDesC8& aChar);
	void SetEncodingType(const TEncodingType aType);
	TEncodingType EncodingType() const;
	void SetArrayValue(const TInt aValue);
	TInt ArrayValue() const;
	void SetAddSpace(const TBool atrueFalse);
	TBool AddSpace() const;
	void SetEncodedLength(const TInt aLength);
	TInt EncodedLength() const;

public:
	friend class CMsvVersion1Version2Converter;
	
private:
	TFieldList		iField; 
	TUint16			iOffset;
	TUint16			iLength;
	TEncodingType	iType;
	TUint16			iArrayValue;
	TBool			iAddSpace; // A space char needs adding between two adjoining 'encoded-words'
	TUint8			iEncodedLength; // This can be used to determine space needed for buffer.
	TUint			iCharsetUid; 

	};




/**
CMsvVersion1Version2Converter
Converts version 1 message store to version 2 message store. This class converts message part
by storing them in version 2 message store. This class can be extended in future by adding API's
that will store additional message parts in the database.


@internalAll 
@released
*/
NONSHARABLE_CLASS (CMsvVersion1Version2Converter): public CBase , public MMsvMessageStoreConverter
{
public:	


	static CMsvVersion1Version2Converter* NewL(CMsvDBAdapter& aDBAdapter,CMsvMessageDBAdapter& aMessageDBAdapter,TDriveNumber aDriveNum,TBool aConversionResume);
	~CMsvVersion1Version2Converter();
	void ConvertMessageStoreL();
private:	
	CMsvVersion1Version2Converter(CMsvDBAdapter& aDBAdapter,CMsvMessageDBAdapter& aMessageDBAdapter,TDriveNumber aDriveNum,TBool aConversionResume);	
	void ConstructL();

	void AddFieldL(TCommonHeaderField aField);
	void AddFieldL(HBufC* aFieldName, EFieldType aFieldType);
	void StartEMailMessageConversionL(TInt32 aMtm);
	void ConvertEMailImapMessagesL(TInt aConvertedMtmArrayIndex);
	void ConvertEMailPopMessagesL(TInt aConvertedMtmArrayIndex);
	void ConvertEMailSmtpMessagesL(TInt aConvertedMtmArrayIndex);
	void CreatePathL(TDes& aMessagePath, TMsvId aId,TMsvId& aOwningId);
	TInt ReadEMailMessageHeaderL(TDesC& aMessagePath);
	TInt ReadEMailMessageEncodedHeaderL(TDesC& aMessagePath);
	TInt ReadEmailMimeHeaderL(TDesC& aMessagePath);	
	void CreateEMailHeaderTableStructureL();
	void CreateEMailHeaderTableL(const TUid aMtmId);
	CArrayFix<TImHeaderEncodingInfo>& EncodingInfo();
	void InternalizeL(RMsvReadStream& aReadStream);
	TInt GetPropertyValue();
	
/* start sms APIs */
	void InternalizeSmsMessageL(TMsvId aServiceId,TUint32 aMtm);
	TBool ReadSmsMessageHeaderL(TDes& aPath);
	void InternalizeSmsHeaderL(RReadStream& aStream,TDes16& detailsOfRecipientField);
	void InternalizeEmailOverSmsHeaderL(RReadStream& smsHeaderEmailPart);
	void CreateSmsHeaderTableL();
	void AddSmsFieldL(HBufC* aFieldName, EFieldType aFieldType);
	void InitializeL();
	void DeAllocateMemory();
	TInt FindStream(TUid aUid,TInt &pos);
	void FillEmptyEosBufferL();

private:
	TPtrC iLastErrorMessage;
	TBool iResume;
	TDriveNumber iDrive;
	CMsvDBAdapter& iDBAdapter;
	CMsvMessageDBAdapter& iMessageDBAdapter;
	RPointerArray<CFieldPair> iTableFieldPairList;
	RPointerArray<CHeaderFields> iEmailfieldPairList;
	RArray<TUid> iConvertedMtmArray;
	RArray<TMsvId> iServiceIdArray;
	RArray<TMsvId> iDeleteMessageIdArray;
	TConversionResumeStatus iResumeStatus;

	CHeaderFields* iSmsHeaderFields;
	CHeaderFields* iSmsRecipientAddressFields;
	CHeaderFields* iEmailOverSmsAddressFields;

	CArrayFix<TImHeaderEncodingInfo>* iEncodingInfo;		
	TMsgOutboxBodyEncoding iBodyEncoding;
	
	RBuf16 iRecipientDetails;
	RBuf16 iEmailOverSmsAddress;	
	
	/*Prepare the store for reading*/
	CArrayFix<TPairedTUidHBufC8> *iStreams;
	
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)	
	friend class CTestSmsMessageV1toV2Converter;
	friend class CTestVersion1Version2Converter;
#endif
	};

/**
HMsvReadBuffer
This class reads information from the stream.

@internalAll 
@released
*/
NONSHARABLE_CLASS (HMsvReadBuffer) : public TMemBuf
	{
public:
	HMsvReadBuffer(const TDesC8 &aBuf);
private:
	virtual void DoRelease();
	};

#endif // __CMSVVERSION1VERSION2CONVERTER_H__




