// Copyright (c) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * HEADER FILES
 */

#include "cmsvversion1version2converter.h"
#include "cmessageconvertermanager.h"
#include "CMsvCachedStore.h"
#include "MSVUTILS.H"
#include "txtrich.h"


// constants
const TInt KArrayGranularity = 16;
const TInt KMaxImHeaderStringLength = 1000;
const TInt KMaxImMimeFieldLength	= 1000;
const TInt KDesCArrayGranularity = 8;
const TInt KMaxBufferLength = 1000;
const TInt KDBCommitCount = 50;
const TInt KMaxLengthOfRecipientField = 50;
const TInt KSmcmSmsNumberMaxNumberLength = 21;
const TInt KSmcmSmsNumberMaxNameLength = 256;							
const TUid KUidMsvSmsEmailFieldsStream	= {0x10204C9D};
const TUid KUidSMSHeaderStream = {0x10001834};
const TUid KMsvEntryFileForSms={0x10003C68};
const TUid KUidMsgFileIMailHeader	= {0x100001D8}; // mail header UID
const TUid KUidMsgFileIMailEncodedHeader = {0x101FD0E3};
const TUid KUidMsgFileMimeHeader	= {0x1000160F};		// 268441103 decimal
const TInt KMaxConverterNumber = 100;
const TInt KMaxPathConverter = 1000;

//Literals
_LIT16(KSeparator,";");
_LIT16(KComma,",");
_LIT16(KDetails,"");
_LIT16(KSpace,"");
_LIT16(KDelimiter, ";");

// Sms header fields
_LIT16(KSmsHeadearversion , "SmsHeadearversion");
_LIT16(KSmsRecipientCount , "RecipientCount");
_LIT16(KRecipients , "To");
_LIT16(KSmsFlags , "SmsFlags");
_LIT16(KTextBioMsgIdType,"BioMsgIdType");


// global function declaration
extern void ExternalizeL(const CDesC8Array& anArray,RWriteStream& aStream);
extern void InternalizeL(CDesC8Array& anArray,RReadStream& aStream);
extern void ExternalizeL(const CDesC16Array& anArray,RWriteStream& aStream);
extern void InternalizeL(CDesC16Array& anArray,RReadStream& aStream);

/**
InternalizeL()

@param anArray: A reference to an array of 16 bit descriptor arrays
@param aStream: The read stream interface
@retun None.
*/
void InternalizeL(CDesC8Array& anArray,RReadStream& aStream)
	{
	TCardinality card;
	aStream >> card;
	TInt count=card;
	anArray.Reset();
	for (TInt ii=0;ii<count;++ii)
		{
		HBufC8* buf=HBufC8::NewLC(aStream,KMaxImHeaderStringLength);
		anArray.CArrayFixBase::InsertL(ii,&buf);
		CleanupStack::Pop();
		}
	}

/**
InternalizeL()

@param anArray: A reference to an array of 16 bit descriptor arrays
@param aStream: The read stream interface
@retun None.
*/
void InternalizeL(CDesC16Array& anArray,RReadStream& aStream)
	{
	TCardinality card;
	aStream >> card;
	TInt count=card;
	anArray.Reset();
	for (TInt ii=0;ii<count;++ii)
		{
		HBufC16* buf=HBufC16::NewLC(aStream,KMaxImHeaderStringLength);
		anArray.CArrayFixBase::InsertL(ii,&buf);
		CleanupStack::Pop();
		}
	}

/**
HMsvReadBuffer()

@param aBuf: 8-bit non-modifiable descriptors
@retun None.
*/
HMsvReadBuffer::HMsvReadBuffer(const TDesC8 &aBuf)
	{ 
	TUint8* ptr=(TUint8 *)aBuf.Ptr();
	Set(ptr,ptr+aBuf.Length(),ERead);
	}	
	
void HMsvReadBuffer::DoRelease()
	{
	delete this;
	}
	
	
//****************************************************************************************
//              Class TImHeaderEncodingInfo Functions
//****************************************************************************************

TImHeaderEncodingInfo::TImHeaderEncodingInfo() :  
		iType(ENoEncoding), iAddSpace(EFalse), iCharsetUid(0)
	{
	}


/** 
TImHeaderEncodingInfo()
Copy constructor.

@param aFrom: Object to copy 
*/
TImHeaderEncodingInfo::TImHeaderEncodingInfo(const TImHeaderEncodingInfo& aFrom) 
	{
	SetField( aFrom.Field() );
	SetOffset( aFrom.Offset() );
	SetLength( aFrom.Length() );
	SetEncodingType( aFrom.EncodingType() );
	SetArrayValue( aFrom.ArrayValue() );
	SetCharsetUid(aFrom.CharsetUid());
	SetAddSpace( aFrom.AddSpace() ); 
	SetEncodedLength( aFrom.EncodedLength() ); 
	}


/** 
InternalizeL()
Internalises the object from the specified stream.

@param aReadStream Stream to read from */
void TImHeaderEncodingInfo::InternalizeL( RReadStream& aReadStream )
	{
	SetField( (TFieldList) aReadStream.ReadUint16L());
	SetOffset( aReadStream.ReadUint16L());
	SetLength( aReadStream.ReadUint16L());
	SetEncodingType( (TEncodingType) aReadStream.ReadUint16L());
	SetArrayValue( aReadStream.ReadUint16L());
	SetCharsetUid( aReadStream.ReadUint32L());
	SetAddSpace( aReadStream.ReadUint16L());
	SetEncodedLength( aReadStream.ReadUint8L());
	}

void TImHeaderEncodingInfo::SetField(const TFieldList aField)
	{
	iField = aField;
	}
	
/** 
Gets the field type.

@return Field type 
*/
TImHeaderEncodingInfo::TFieldList TImHeaderEncodingInfo::Field() const
	{
	return iField;
	}

/** 
Sets the field offset.

@param aOffset Field offset 
*/
void TImHeaderEncodingInfo::SetOffset(const TInt aOffset)
	{
	iOffset = (TUint16)aOffset;
	}

/** 
Gets the field offset.

@return Field offset 
*/
TInt TImHeaderEncodingInfo::Offset() const
	{
	return iOffset;
	}

/** 
Sets the field length.

@param aLength Field length 
*/
void TImHeaderEncodingInfo::SetLength(const TInt aLength)
	{
	iLength = (TUint16)aLength;
	}

/** 
Gets the field length.

@return Field length 
*/
TInt TImHeaderEncodingInfo::Length()  const
	{
	return iLength;
	}

/** Sets the character set UID.

@param aUid Character set UID 
*/
void TImHeaderEncodingInfo::SetCharsetUid(const TUint aUid)
	{
	iCharsetUid = aUid;
	}

/** 
Gets the character set UID.
@return Character set UID 
*/
TUint TImHeaderEncodingInfo::CharsetUid() const
	{
	return iCharsetUid;
	}

/** 
Sets the encoding type (string).
@param aChar Encoding type 
*/
void TImHeaderEncodingInfo::SetEncodingType(const TDesC8& aChar)
	{
	// If invalid, defaults to QPEncoding
	if (aChar[0] == 'b' || aChar[0] == 'B')
		iType = TImHeaderEncodingInfo::EBase64;
	else if (aChar[0] == 'q' || aChar[0] == 'Q')
		iType = TImHeaderEncodingInfo::EQP;
	else
		iType = TImHeaderEncodingInfo::ENoEncoding;
	}

/**
Sets the encoding type (enumeration).

@param aType Encoding type
*/
void TImHeaderEncodingInfo::SetEncodingType(const TEncodingType aType)
	{
	iType = aType;
	}

/** 
Gets the encoding type (enumeration).

@return Encoding type 
*/
TImHeaderEncodingInfo::TEncodingType TImHeaderEncodingInfo::EncodingType()  const
	{
	return iType;
	}

/** 
Sets the array value.

@param aValue Array value 
*/
void TImHeaderEncodingInfo::SetArrayValue(const TInt aValue)
	{
	iArrayValue = (TUint16)aValue;
	}

/** 
Gets the array value.

@return Array value 
*/
TInt TImHeaderEncodingInfo::ArrayValue()  const
	{
	return iArrayValue;
	}

/** 
Sets if a space character needs adding between two adjoining encoded-words.

@param atrueFalse True to add a space character 
*/
void TImHeaderEncodingInfo::SetAddSpace(const TBool atrueFalse)
	{
	iAddSpace = atrueFalse;
	}

/** 
Tests if the add space character flag is set.

@return True for added space characters 
*/
 TBool TImHeaderEncodingInfo::AddSpace() const
	{
	return iAddSpace;
	}

/** 
Sets the encoded length.

@param aLength Encoded length 
*/
void TImHeaderEncodingInfo::SetEncodedLength(const TInt aLength)
	{
	iEncodedLength = (TInt8) aLength;
	}

/** 
Gets the encoded length.

@return Encoded length 
*/
TInt TImHeaderEncodingInfo::EncodedLength() const
	{
	return iEncodedLength + (AddSpace() ? 1 : 0);
	}



//****************************************************************************************
//              Class CMsvVersion1Version2Converter Functions
//****************************************************************************************


/*
CMsvVersion1Version2Converter()

@param aDBAdapter: 
@param aMesasgeDBAdapter: 
@retun None.
@internalComponent
*/
CMsvVersion1Version2Converter::CMsvVersion1Version2Converter(CMsvDBAdapter& aDBAdapter,CMsvMessageDBAdapter& aMessageDBAdapter,TDriveNumber aDriveNum,TBool aConversionResume)
: iDBAdapter(aDBAdapter),iMessageDBAdapter(aMessageDBAdapter)
	{
	iResume = aConversionResume;
	iDrive = aDriveNum;
	}

/**
NewL()

@param aDBAdapter: 
@param aMesasgeDBAdapter: 
@retun None.
@internalComponent
*/
CMsvVersion1Version2Converter* CMsvVersion1Version2Converter::NewL(CMsvDBAdapter& aDBAdapter,CMsvMessageDBAdapter& aMessageDBAdapter,TDriveNumber aDriveNum,TBool aConversionResume)
	{
	CMsvVersion1Version2Converter* self = new (ELeave) CMsvVersion1Version2Converter(aDBAdapter,aMessageDBAdapter,aDriveNum,aConversionResume);
	CleanupStack::PushL(self);
	
	self->ConstructL();

	CleanupStack::Pop(self);
	return self;
	}

/**
~CMsvVersion1Version2Converter()

@param None: 
@retun None.
@internalComponent
*/
CMsvVersion1Version2Converter::~CMsvVersion1Version2Converter()
	{
	iConvertedMtmArray.Close();

	delete iSmsRecipientAddressFields;
	iSmsRecipientAddressFields = NULL;
			
	iRecipientDetails.Close();
	iEmailOverSmsAddress.Close();

	iEmailfieldPairList.ResetAndDestroy();	
	iEmailfieldPairList.Close();
	iTableFieldPairList.ResetAndDestroy();
	iTableFieldPairList.Close();
		
	delete iEncodingInfo;
	iEncodingInfo = NULL;
	
	iServiceIdArray.Close();
	}	

/**
ConstructL()

@param None: 
@retun None.
@internalComponent
*/	
void CMsvVersion1Version2Converter::ConstructL()
	{
	iResumeStatus.lastId = 0;
	iResumeStatus.mtmId.iUid = 0;
	iResumeStatus.serviceId = 0;
	iResumeStatus.sourceVersion = 1;
	iResumeStatus.targetVersion = 2;
	iResumeStatus.remainingCount = 0;
	
	iEncodingInfo = new (ELeave) CArrayFixFlat<TImHeaderEncodingInfo>(KArrayGranularity);
	}

/*
GetPropertyValue()
Gets the value of RProperty object. This value is used to check if cancel request is 
issued for the store conversion in progress

@param None
@return: Value of the property
@internalComponent
*/
TInt CMsvVersion1Version2Converter::GetPropertyValue()
	{
	TInt value;
	TInt err = RProperty::Get(KMyPropertyCat,KMyPropertyName, value);
	return value;
	}


/**
ConvertMessageStoreL()
Starts conversion of version 1 message store to version 2
It is responsible for the conversion of sms, imap and pop messages.
It sequentially converts the above MTM's, reading messages and inserting the same in database table
Header tables for MTM's are created only if a service entry is found for it.
It takes care of resuming conversion (RESTART), updation/removal of conversion status table entries,
creation of header tables etc.

@param None:
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::ConvertMessageStoreL()
	{
	// this class will take care of converting all mtms for v1 to v2

	iConvertedMtmArray.AppendL(KUidMsvSmsMtm);
	iConvertedMtmArray.AppendL(KUidMsgTypeIMAP4); // imap
	iConvertedMtmArray.AppendL(KUidMsgTypePOP3); // pop/smtpKUidMsgTypeSMTP
	iConvertedMtmArray.AppendL(KUidMsgTypeSMTP); // pop/smtpKUidMsgTypeSMTP
			
	TInt pos=0;
	if(iResume)
		{
		iDBAdapter.GetConversionStatusL(iResumeStatus);
		pos = iConvertedMtmArray.Find(iResumeStatus.mtmId);
		
		if (pos == KErrNotFound)// nothing found
			{
			pos = 0;
			iResume = EFalse;
			}		
		}
	
	while(pos < iConvertedMtmArray.Count())
		{

		TInt mtmId = iConvertedMtmArray[pos].iUid;
		
		switch(mtmId)
			{
			case 0x1000102C: // sms
				{
					/*
					1. Read TMsvIds
					2. Construct path
					3. Create header table for header part
					4. Open file and Read header part and Email over sms part
					5. Insert Header and Email Over sms part into the header table
					6  Open file and Read body part 
					7. Insert body part into the body table
					8. commit the changes.	
					*/
				iResumeStatus.mtmId.iUid = iConvertedMtmArray[pos].iUid;
				iDBAdapter.BeginTransactionL();
				if(iResume)
					{
					// Resume conversion from the last point
					iDBAdapter.GetRemainingServiceIdsL(iServiceIdArray);
					iResume = EFalse;
					}
				else
					{	
					// get all serviceIds belong to sms mtm.
					iDBAdapter.GetAllServiceIDsForMtmL(iConvertedMtmArray[pos].iUid,iServiceIdArray);
					
					//Lets create resume status for all the serviceids
					iResumeStatus.mtmId.iUid = iConvertedMtmArray[pos].iUid;
			
					for(TInt j =0; j < iServiceIdArray.Count(); j++)
						{
						iResumeStatus.lastId = 0;
						
						iResumeStatus.serviceId = iServiceIdArray[j];
						iResumeStatus.sourceVersion = 1;
						iResumeStatus.targetVersion = 2;
						iResumeStatus.remainingCount = iDBAdapter.GetMessageCountL(iServiceIdArray[j]);
						iDBAdapter.AddConversionStatusEntryL(iResumeStatus);
						}
					if(iServiceIdArray.Count() !=0)
						{
						CreateSmsHeaderTableL();
						iMessageDBAdapter.CreateHeaderTableL(TUid::Uid(iResumeStatus.mtmId.iUid),iTableFieldPairList,iLastErrorMessage);
						iTableFieldPairList.ResetAndDestroy();
						}
					}	
	
				iDBAdapter.CommitTransactionL();						
											
				for(TInt serviceIdPosition = 0;serviceIdPosition<iServiceIdArray.Count();serviceIdPosition++)
					{
					/* Read and Insert Header and Body part of Sms Mesasge */
					InternalizeSmsMessageL(iServiceIdArray[serviceIdPosition],iResumeStatus.mtmId.iUid);
					}
				/* remove all conversion status entries */
				for(TInt serviceIdPosition = 0;serviceIdPosition<iServiceIdArray.Count();serviceIdPosition++)
					{
					iDBAdapter.RemoveConversionStatusEntriesL();							
					}
							
				iServiceIdArray.Reset();	
				pos++; // next one is imap
				
				// get all service ids for IMAP
				iDBAdapter.GetAllServiceIDsForMtmL(iConvertedMtmArray[pos].iUid,iServiceIdArray);
				}
			   break;
			case 0x1000102A: // imap
				{
				ConvertEMailImapMessagesL(pos);
			
				iServiceIdArray.Reset();
				pos++; // next one is pop
							
				// get all service ids for POP
				iDBAdapter.GetAllServiceIDsForMtmL(iConvertedMtmArray[pos].iUid,iServiceIdArray);
				
				break;// imap conversion
				}
			
			case 0x10001029:  //email pop/smtp
				{
				ConvertEMailPopMessagesL(pos);
				iServiceIdArray.Reset();
				pos++;
				// get all service ids for POP
				iDBAdapter.GetAllServiceIDsForMtmL(iConvertedMtmArray[pos].iUid,iServiceIdArray);
				
				break;
				}
			
			case 0x10001028:
				{
				ConvertEMailSmtpMessagesL(pos);
				iServiceIdArray.Reset();
				pos++;
				}
			}
		}
	}

/**
ConvertEMailImapMessagesL()
Converts IMAP message headers

@param None: 
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::ConvertEMailImapMessagesL(TInt aConvertedMtmArrayIndex)
	{
	if(iResume)
		{
		// Resume conversion from the last point
		iDBAdapter.GetRemainingServiceIdsL(iServiceIdArray);
							
		iResume = EFalse;
		}
	else 	// new conversion request
		{
		if(iServiceIdArray.Count())
			{
			/* New request for Imap messages.
			 iServiceIdArray array is filled with all service ids prior to this function call.
			 This ensures we periodically update the conversion resume table and commit the open 
			 transaction with the database*/
			 
			iDBAdapter.BeginTransactionL();
			
			//Lets create resume status for all the serviceids
			iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;

			for(TInt j =0; j < iServiceIdArray.Count(); j++)
				{
				iResumeStatus.lastId = 0;
				
				iResumeStatus.serviceId = iServiceIdArray[j];
				iResumeStatus.sourceVersion = 1;
				iResumeStatus.targetVersion = 2;
				iResumeStatus.remainingCount = iDBAdapter.GetMessageCountL(iServiceIdArray[j]);
				iDBAdapter.AddConversionStatusEntryL(iResumeStatus);
				}
			
			// Create header table
			CreateEMailHeaderTableL(iConvertedMtmArray[aConvertedMtmArrayIndex]);	
						
			iDBAdapter.CommitTransactionL();// done
			}
		}
	iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;
	
	StartEMailMessageConversionL(iResumeStatus.mtmId.iUid);

	// remove all conversion status entries
	iDBAdapter.RemoveConversionStatusEntriesL();	
	}

/**
ConvertEMailPopMessagesL()
Converts POP message headers.

@param None:
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::ConvertEMailPopMessagesL(TInt aConvertedMtmArrayIndex)
	{
	
	iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;
		
	if(iResume)
		{
		// Resume conversion from the last point
		iDBAdapter.GetRemainingServiceIdsL(iServiceIdArray);
		iDBAdapter.GetConversionStatusL(iResumeStatus);
		iResume = EFalse;
		}
	else
		{
		if(iServiceIdArray.Count())
			{
			/* New request for pop messages.
			 iServiceIdArray array is filled with all service ids prior to this function call.
			 This ensures we periodically update the conversion resume table and commit the open 
			 transaction with the database */
			 
			iDBAdapter.BeginTransactionL();
			 
			iTableFieldPairList.ResetAndDestroy();
	
			// Create header table
			CreateEMailHeaderTableL(iConvertedMtmArray[aConvertedMtmArrayIndex]);	
						
			//Lets create resume status for all the serviceids
			iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;
			
			for(TInt j =0; j < iServiceIdArray.Count(); j++)
				{
				iResumeStatus.lastId = 0;
				
				iResumeStatus.serviceId = iServiceIdArray[j];
				iResumeStatus.sourceVersion = 1;
				iResumeStatus.targetVersion = 2;
				iResumeStatus.remainingCount = iDBAdapter.GetMessageCountL(iServiceIdArray[j]);
				iDBAdapter.AddConversionStatusEntryL(iResumeStatus);
				}

			iDBAdapter.CommitTransactionL();// done
			}
		}
	StartEMailMessageConversionL(iResumeStatus.mtmId.iUid);
	
	// remove all conversion ststus entries
	iDBAdapter.RemoveConversionStatusEntriesL();							
	
	}

/**
ConvertEMailPopMessagesL()
Converts POP message headers.

@param None:
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::ConvertEMailSmtpMessagesL(TInt aConvertedMtmArrayIndex)
	{
	
	iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;
		
	if(iResume)
		{
		// Resume conversion from the last point
		iDBAdapter.GetRemainingServiceIdsL(iServiceIdArray);
		iDBAdapter.GetConversionStatusL(iResumeStatus);
		iResume = EFalse;
		}
	else
		{
		if(iServiceIdArray.Count())
			{
			/* New request for smtp messages.
			 iServiceIdArray array is filled with all service ids prior to this function call.
			 This ensures we periodically update the conversion resume table and commit the open 
			 transaction with the database*/
			iDBAdapter.BeginTransactionL();	
			iTableFieldPairList.ResetAndDestroy();
									
			//Lets create resume status for all the serviceids
			iResumeStatus.mtmId.iUid = iConvertedMtmArray[aConvertedMtmArrayIndex].iUid;
			
			TBool messagesFound = EFalse;
			for(TInt j =0; j < iServiceIdArray.Count(); j++)
				{
				iResumeStatus.lastId = 0;
				
				iResumeStatus.serviceId = iServiceIdArray[j];
				iResumeStatus.sourceVersion = 1;
				iResumeStatus.targetVersion = 2;
				iResumeStatus.remainingCount = iDBAdapter.GetMessageCountL(iServiceIdArray[j]);
				if(iResumeStatus.remainingCount != 0)
					{
					messagesFound = ETrue;
					}
				iDBAdapter.AddConversionStatusEntryL(iResumeStatus);
				}
			
			// Create header table if only we have some messages anywhere in the message store
			if(messagesFound)
				{
				CreateEMailHeaderTableL(iConvertedMtmArray[aConvertedMtmArrayIndex]);	
				}

			iDBAdapter.CommitTransactionL();// done
			}
		}
	StartEMailMessageConversionL(iResumeStatus.mtmId.iUid);
	
	// remove all conversion ststus entries
	iDBAdapter.RemoveConversionStatusEntriesL();							
	
	}
/**
AddFieldL()
Appends a CFieldPair  object for common header fields to RPointerArray of CFieldPair objects

@param aField: Commonly used header fields
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::AddFieldL(TCommonHeaderField aField)
	{
	CFieldPair *fieldObject = new(ELeave) CFieldPair();
	CleanupStack::PushL(fieldObject);
	
	fieldObject->iFieldType = ETextField;
	switch (aField)
		{
		case EFrom:
			_LIT16(KFromText, "From");
			fieldObject->iFieldName = KFromText().AllocL();
			break;
		
		case ETo:
			_LIT16(KToText, "To");
			fieldObject->iFieldName = KToText().AllocL();
			break;
		
		case ECC:
			_LIT16(KCCText, "CC");
			fieldObject->iFieldName = KCCText().AllocL();
			break;
		
		case EBCC:
			_LIT16(KBCCText, "BCC");
			fieldObject->iFieldName = KBCCText().AllocL();
			break;
	
		case ESubject:
			_LIT16(KSubjectText, "Subject");
			fieldObject->iFieldName = KSubjectText().AllocL();
			break;
		
		default:
			User::Leave(KErrArgument);
		}
		
	iTableFieldPairList.AppendL(fieldObject);
	CleanupStack::Pop(fieldObject);
	}

/**
AddFieldL()
Appends a CFieldPair  object to RPointerArray of CFieldPair objects

@param aFieldName: Name of the field
@param aFieldType: Field type
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::AddFieldL(HBufC* aFieldName, EFieldType aFieldType)
	{
	if(NULL == aFieldName)
		{
		User::Leave(KErrArgument);
		}

	CFieldPair *fieldObject = new(ELeave) CFieldPair();
	CleanupStack::PushL(fieldObject);
	
	fieldObject->iFieldType = aFieldType;
	fieldObject->iFieldName = aFieldName;
	
	iTableFieldPairList.AppendL(fieldObject);
	CleanupStack::Pop(fieldObject);
	}


/**
StartEMailMessageConversionL()
Gets all message IDs for each service entry for the mtm that is being converted.
It creates the path of the message, reads the message and inserts them in database.
For Mime conversion, it checks if the message has attachments and if so than converts its mime information
It periodically updates the conversion status entries in the database and commits the same on the main database.

@param None:
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::StartEMailMessageConversionL(TInt32 aMtm)
	{
	TMsvId iD;
	for(TInt pos = 0; pos<iServiceIdArray.Count();pos++)
		{
		
		
		iDBAdapter.BeginTransactionL();
		
		RArray<TMsvId> iDArray;
		CleanupClosePushL (iDArray);
				
		// Get ids from the last point
		// Note : if we are resuming than value of iResumeStatus.lastId is set to the last
		// message id that was converted, else it is Zero.
		iDBAdapter.GetAllMessageIdsL(iServiceIdArray[pos],iDArray,aMtm, iResumeStatus.lastId);
		iResumeStatus.remainingCount = iDArray.Count();
			
		TInt i=0;
		for (;i<iDArray.Count();i++)
			{
			iResumeStatus.lastId = iDArray[i];
			iResumeStatus.remainingCount--;
						
			iDBAdapter.GetOwningServiceL(iDArray[i],iD);
						
			RBuf path;
			CleanupClosePushL(path);
			path.CreateL(KMaxPath);
						
			CreatePathL(path,iDArray[i],iD);
					
			//Read the message
			TInt headerError = ReadEMailMessageHeaderL(path);
			TInt encodeHeaderError = ReadEMailMessageEncodedHeaderL(path);
	
			if(headerError == KErrNone || encodeHeaderError == KErrNone)
				{
				iMessageDBAdapter.CreateHeaderEntryL(TUid::Uid(iResumeStatus.mtmId.iUid), iDArray[i], iEmailfieldPairList); //header
 				iEmailfieldPairList.ResetAndDestroy();
				}
			
			path.Close();
			path.CreateL(KMaxPathConverter);
			// check if this has mime
			
			RArray<TMsvId> mimeIdArray;
			CleanupClosePushL(mimeIdArray);
			
			mimeIdArray.AppendL(iDArray[i]);
			iDBAdapter.GetAllMimeIdsL(iDArray[i],mimeIdArray);
						
			if(mimeIdArray.Count())
				{
				for(TInt index = 0; index < mimeIdArray.Count();index++)
					{
										
					CreatePathL(path,mimeIdArray[index],iD);
					
					TInt error = ReadEmailMimeHeaderL(path);
					if(!error)
						{
						iMessageDBAdapter.CreateHeaderEntryL(iResumeStatus.mtmId, mimeIdArray[index], iEmailfieldPairList); //header
						iEmailfieldPairList.ResetAndDestroy();
						}
					path.Zero();
					}
				}
			CleanupStack::PopAndDestroy(&mimeIdArray);		
			CleanupStack::PopAndDestroy(); //path	

			//check if we have a request for cancellation
			TInt cancel = GetPropertyValue();
			if(cancel == KErrCancel	)
				{
				User::Leave(KErrCancel);
				}
				
			//update the conversion status
			if(i % KDBCommitCount == 0 && i != 0)
				{
				iResumeStatus.lastId = iDArray[i];
				iResumeStatus.serviceId = iServiceIdArray[pos];
				iResumeStatus.remainingCount -= KDBCommitCount; 
				
				iDBAdapter.UpdateConversionStatusEntryL(iResumeStatus);
				iDBAdapter.CommitTransactionL();
				iDBAdapter.BeginTransactionL();
				}
			}
		
		CleanupStack::PopAndDestroy(&iDArray);
	
		//update status for this service id
		iResumeStatus.lastId = 0;
		iResumeStatus.serviceId = iServiceIdArray[pos];
		iResumeStatus.remainingCount = 0; 
		iDBAdapter.UpdateConversionStatusEntryL(iResumeStatus);
		iDBAdapter.CommitTransactionL();
		}	
	}




/**
CreatePathL()
Creates path of aessage. The path is constructed based on the message id and its owning service id.

@param aMessagePath: Message path
@param aId: Message id
@param aOwningId: owning id.
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::CreatePathL(TDes& aMessagePath, TMsvId aId,TMsvId& aOwningId)
	{
	// Construct the mail path for this message entry
	_LIT16(KMessageFilePath,"\\private\\1000484b\\mail2\\");
	TBuf<KMaxPath> src;
	TPtrC drive(TDriveUnit(iDrive).Name());
				
	aMessagePath.Append(drive);
	aMessagePath.Append(KMessageFilePath);
	
	TBuf<KFileNameFixedWidth> dir;
	dir.NumFixedWidth(aOwningId, EHex, KFileNameFixedWidth);
	_LIT16(KMsvDirectoryExt, "_S\\");
	aMessagePath.Append(dir);
	aMessagePath.Append(KMsvDirectoryExt);
		
	_LIT16(KDirSep,"\\");
	TBuf<KFileNameFixedWidth> dir1;
	
	dir1.NumFixedWidth(aId&0xf, EHex,1);
	aMessagePath.Append(dir1);
	aMessagePath.Append(KDirSep);
	dir.NumFixedWidth(aId, EHex, KFileNameFixedWidth);	
	aMessagePath.Append(dir);
	}

/**
CreateEMailHeaderTableL()
Creates structure necessary to create email header table.

@param aMtmId: mtm Id of email component.
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::CreateEMailHeaderTableL(const TUid aMtmId)
	{
	CreateEMailHeaderTableStructureL();
	iMessageDBAdapter.CreateHeaderTableL(aMtmId,iTableFieldPairList,iLastErrorMessage);
	}

/**
CreateEMailHeaderTableStructureL()
Creates structure necessary to create email header table.

@param None:
@retun None.
@internalComponent
*/
void CMsvVersion1Version2Converter::CreateEMailHeaderTableStructureL()
	{
	_LIT16(KEmailHeaderVersion , "Version");
	AddFieldL(KEmailHeaderVersion().AllocL(),EIntegerField); //0
	
	_LIT16(KReceiptAddress , "ReceiptAddress");
	AddFieldL(KReceiptAddress().AllocL(),ETextField);//1
	
	_LIT16(KImMsgId , "ImMsgId");
	AddFieldL(KImMsgId().AllocL(),ETextField);//2

	AddFieldL(EFrom);//3
	
	_LIT16(KReplyTo , "ReplyTo");
	AddFieldL(KReplyTo().AllocL(),ETextField);//4
	
	AddFieldL(ESubject);//5

	_LIT16(KiRemoteSize , "iRemoteSize");
	AddFieldL(KiRemoteSize().AllocL(),EIntegerField);//6
	
	AddFieldL(ETo);//7
	
	AddFieldL(ECC);//8
				
	AddFieldL(EBCC);//9

	_LIT16(KEncodingInfo , "EncodingInfo");
	AddFieldL(KEncodingInfo().AllocL(),ETextField);//10
	
	_LIT16(KResentMsgId , "ResentMsgId");
	AddFieldL(KResentMsgId().AllocL(),ETextField);//11

	_LIT16(KResentFrom , "ResentFrom");
	AddFieldL(KResentFrom().AllocL(),ETextField);//12

	_LIT16(KResentToRecipients , "ResentToRecipients");
	AddFieldL(KResentToRecipients().AllocL(),ETextField);//13

	_LIT16(KResentCcRecipients , "ResentCcRecipients");
	AddFieldL(KResentCcRecipients().AllocL(),ETextField);//14
	
	_LIT16(KResentBccRecipients , "ResentBccRecipients");
	AddFieldL(KResentBccRecipients().AllocL(),ETextField);//15

	_LIT16(KInReplyTo , "InReplyTo");
	AddFieldL(KInReplyTo().AllocL(),ETextField);//16

	_LIT16(KBodyEncoding , "BodyEncoding");
	AddFieldL(KBodyEncoding().AllocL(),EIntegerField);//17

	_LIT16(KCharset , "Charset");
	AddFieldL(KCharset().AllocL(),EIntegerField);//18
	}

/**
EncodingInfo()
Returns TImHeaderEncodingInfo header encoding info.

@param None:
@retun None.
@internalComponent
*/
CArrayFix<TImHeaderEncodingInfo>& CMsvVersion1Version2Converter::EncodingInfo()
	{
	return *iEncodingInfo;
	}

/**
ReadEMailMessageHeaderL()
Reads an email message header from the specified path and fills 	RPointerArray of CHeaderFields
from the header objects.

@param aMessagePath: Message path.
@retun None.
@internalComponent
*/
TInt CMsvVersion1Version2Converter::ReadEMailMessageHeaderL(TDesC& aMessagePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);	
	
	RFile file;
	TInt error = file.Open(fs,aMessagePath,EFileRead);
	
	if(error != KErrNone)
		{
		CleanupStack::PopAndDestroy(&fs); //fs
		file.Close();
		fs.Close();
		return EFalse;
		}
	
	CleanupClosePushL(file);
	
	RFileReadStream in(file);
	in.PushL();

	TCheckedUid check;
	in >> check;
	if(check.UidType().IsValid()==EFalse) 
		return EFalse;
	
	CArrayFix<TPairedTUidHBufC8>* iStreams = new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);
	in >> *iStreams;; 

	// check if this stream is present
	TPairedTUidHBufC8 entry;
	entry.iUid=KUidMsgFileIMailHeader;
	TKeyArrayFix key(_FOFF(TPairedTUidHBufC8,iUid),ECmpTInt32);
	
	TInt pos;
	TInt found = (iStreams->FindIsq(entry,key,pos));
			
	if(found == KErrNone)
		{
		
		HBufC8 *buffer=iStreams->At(pos).iBuf;	
		HMsvReadBuffer *buf = new (ELeave) HMsvReadBuffer(*buffer);
		RReadStream infoHeader(buf);
		
		/*Lets start reading the Header Info */
		CHeaderFields* emailHeaderFields = new (ELeave)CHeaderFields();
		CleanupStack::PushL(emailHeaderFields);
		emailHeaderFields->iUid = KUidMsgFileIMailHeader;
		CleanupStack::Pop(emailHeaderFields);
		
		CFieldPair* emailHeaderVersionfield = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailHeaderVersionfield);
		emailHeaderVersionfield->iFieldNumValue = infoHeader.ReadUint16L();
		emailHeaderFields->iFieldPairList.AppendL(emailHeaderVersionfield);
		CleanupStack::Pop(emailHeaderVersionfield);

		HBufC16* receipt = HBufC::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailReceiptAddress = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReceiptAddress);
		emailReceiptAddress->iFieldTextValue = receipt;
		emailHeaderFields->iFieldPairList.AppendL(emailReceiptAddress);
		CleanupStack::Pop(emailReceiptAddress);
		
		HBufC8* msgId = HBufC8::NewL( infoHeader, KMaxImHeaderStringLength );	
		CFieldPair* emailImMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailImMsgId);
		emailImMsgId->iFieldTextValue = HBufC16::NewL(KMaxImHeaderStringLength);
		emailImMsgId->iFieldTextValue->Des().Copy(msgId->Des());
		emailHeaderFields->iFieldPairList.AppendL(emailImMsgId);
		CleanupStack::Pop(emailImMsgId);
		delete msgId;

		HBufC16* from = HBufC::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailFrom);
		emailFrom->iFieldTextValue = from;
		emailHeaderFields->iFieldPairList.AppendL(emailFrom);
		CleanupStack::Pop(emailFrom);

		HBufC16* replyTo = HBufC::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReplyTo);
		emailReplyTo->iFieldTextValue = replyTo;
		emailHeaderFields->iFieldPairList.AppendL(emailReplyTo);
		CleanupStack::Pop(emailReplyTo);
		
		HBufC16* subject = HBufC::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailSubject = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailSubject);
		emailSubject->iFieldTextValue = subject;
		emailHeaderFields->iFieldPairList.AppendL(emailSubject);
		CleanupStack::Pop(emailSubject);

		CFieldPair* emailRemoteSize = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailRemoteSize);
		emailRemoteSize->iFieldNumValue = infoHeader.ReadUint32L();
		emailHeaderFields->iFieldPairList.AppendL(emailRemoteSize);
		CleanupStack::Pop(emailRemoteSize);

		TInt index = 0;
		RBuf strbuf;
		CleanupClosePushL(strbuf);
		strbuf.CreateL(500);
			
		CDesCArray* to  = new (ELeave) CDesCArrayFlat( KArrayGranularity );
		infoHeader >> *to;
		for( index = 0;index< to->MdcaCount();index++)
			{
			strbuf.Append(to->MdcaPoint(index));
			if((index + 1) != to->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}
			}
			
		delete 	to;
		to = NULL;
				
		CFieldPair* emailToRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailToRecipients);
		emailToRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailToRecipients);	
		CleanupStack::Pop(emailToRecipients);
		
		strbuf.Zero();
		
		CDesCArray* cc  = new (ELeave)CDesCArrayFlat( KArrayGranularity );
		infoHeader >> *cc;
			
		for( index = 0;index< cc->MdcaCount();index++)
			{
			strbuf.Append(cc->MdcaPoint(index));
			if((index + 1) != cc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}

			
			
			}
		delete 	cc;
		cc = NULL;
						
		CFieldPair* emailCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCcRecipients);
		emailCcRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailCcRecipients);	
		CleanupStack::Pop(emailCcRecipients);
		
		strbuf.Zero();

		CDesCArray* bcc  = new (ELeave)CDesCArrayFlat( KArrayGranularity );
		infoHeader >> *bcc;
			
		for( index = 0;index< bcc->MdcaCount();index++)
			{
			strbuf.Append(bcc->MdcaPoint(index));
			if((index + 1) != bcc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}

			}
		delete 	bcc;
		bcc = NULL;
					
		CFieldPair* emailBCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBCcRecipients);
		emailBCcRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailBCcRecipients);
		CleanupStack::Pop(emailBCcRecipients);
		
		strbuf.Zero();
		
		//Read Email encoding info
		infoHeader >> EncodingInfo();
		CFieldPair* emailEncodingInfo = new (ELeave)CFieldPair();
		
		RBuf16 encodinginfo;
		CleanupClosePushL(encodinginfo);
		encodinginfo.CreateL(iEncodingInfo->Count() * sizeof(TImHeaderEncodingInfo));
		if(iEncodingInfo->Count())
			{
			for (index =0; index<iEncodingInfo->Count(); index++)
				{
				encodinginfo.AppendNum(iEncodingInfo->At(index).Field());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).Offset());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).Length());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).EncodingType());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).ArrayValue());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).CharsetUid());
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).AddSpace()); 
				strbuf.Append(KComma);
				encodinginfo.AppendNum(iEncodingInfo->At(index).EncodedLength()); 
				strbuf.Append(KSeparator);
				}
			emailEncodingInfo->iFieldTextValue = encodinginfo.AllocL();
			}
		else
			{
			emailEncodingInfo->iFieldTextValue = KNullDesC().AllocL();
			}
		emailHeaderFields->iFieldPairList.AppendL(emailEncodingInfo);

		HBufC8* resentMsgId = HBufC8::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailResentMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentMsgId);
		emailResentMsgId->iFieldTextValue = (HBufC16*)(resentMsgId);
		emailHeaderFields->iFieldPairList.AppendL(emailResentMsgId);
		CleanupStack::Pop(emailResentMsgId);

		HBufC16* resentFrom = HBufC::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailResentFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentFrom);
		emailResentFrom->iFieldTextValue =(HBufC16*)(resentFrom);
		emailHeaderFields->iFieldPairList.AppendL(emailResentFrom);
		CleanupStack::Pop(emailResentFrom);
		
		CDesCArray* resentTo  = new (ELeave)CDesCArrayFlat( KArrayGranularity );
		CDesCArray* resentCc  = new (ELeave)CDesCArrayFlat( KArrayGranularity );	
		CDesCArray* resentBcc = new (ELeave)CDesCArrayFlat( KArrayGranularity );
		
		
		CFieldPair* emailResentToRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentToRecipients);
		infoHeader >> *resentTo;
		for(index = 0;index< resentTo->MdcaCount();index++)
			{
			strbuf.Append(resentTo->MdcaPoint(index));
			if((index + 1) != resentTo->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}
			}
				
		emailResentToRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailResentToRecipients);
		CleanupStack::Pop(emailResentToRecipients);
		
		delete resentTo;
		resentTo = NULL;
				
		strbuf.Zero();

		CFieldPair* emailResentCcRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentCcRecipients);
		infoHeader >> *resentCc;
		for(index = 0;index< resentCc->MdcaCount();index++)
			{
			strbuf.Append(resentCc->MdcaPoint(index));
			if((index + 1) != resentCc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}
			}
		emailResentCcRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailResentCcRecipients);
		CleanupStack::Pop(emailResentCcRecipients);
		
		delete resentCc;
		resentCc = NULL;
		strbuf.Zero();
			
		CFieldPair* emailResentBccRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentBccRecipients);
		infoHeader >> *resentBcc;
		for(index = 0;index< resentBcc->MdcaCount();index++)
			{
			strbuf.Append(resentBcc->MdcaPoint(index));
			if((index + 1) != resentBcc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			else
				{
				strbuf.Append(KSeparator);
				}
			}
			
		emailResentBccRecipients->iFieldTextValue = strbuf.AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emailResentBccRecipients);
		CleanupStack::Pop(emailResentBccRecipients);
		
		delete resentBcc;
		resentBcc = NULL;
		strbuf.Zero();
			
		HBufC8* inReplyTo = HBufC8::NewL( infoHeader, KMaxImHeaderStringLength );
		CFieldPair* emailInReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailInReplyTo);
		emailInReplyTo->iFieldTextValue = (HBufC16*)(inReplyTo);
		emailHeaderFields->iFieldPairList.AppendL(emailInReplyTo);
		CleanupStack::Pop(emailInReplyTo);

		TInt bodyEncoding = infoHeader.ReadUint8L();
		CFieldPair* emailBodyEncoding = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBodyEncoding);
		emailBodyEncoding->iFieldNumValue = bodyEncoding;
		emailHeaderFields->iFieldPairList.AppendL(emailBodyEncoding);
		CleanupStack::Pop(emailBodyEncoding);

		TUint headerCharset = infoHeader.ReadUint32L();
		CFieldPair* emailCharset = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCharset);
		emailCharset->iFieldNumValue = headerCharset;
		emailHeaderFields->iFieldPairList.AppendL(emailCharset);
		CleanupStack::Pop(emailCharset);

		CFieldPair* emaildetailField = new (ELeave)CFieldPair();
		CleanupStack::PushL(emaildetailField);
		emaildetailField->iFieldName =  KDetails().AllocL();
		emaildetailField->iFieldType = ETextField;
		emaildetailField->iFieldTextValue = KSpace().AllocL();
		emailHeaderFields->iFieldPairList.AppendL(emaildetailField);
		CleanupStack::Pop(emaildetailField);

		iEmailfieldPairList.AppendL(emailHeaderFields);
		
		CleanupStack::PopAndDestroy(&encodinginfo); 
		CleanupStack::PopAndDestroy(&strbuf);
		infoHeader.Close();
		}
	TInt count=iStreams->Count();
	while(count--) 
		{
		delete iStreams->At(count).iBuf;
		}
		
	delete iStreams;
	iStreams=NULL;
	CleanupStack::PopAndDestroy(3); // in , file.fs
	
	return KErrNone;
	}





/**
ReadEMailMessageEncodedHeaderL()
Reads an email message encoded header from the specified path and fills 	RPointerArray of CHeaderFields
from these read header objects.

@param aMessagePath: message path.
@retun TInt: Error code specifying if the message was read successfully.
@internalComponent
*/
TInt CMsvVersion1Version2Converter::ReadEMailMessageEncodedHeaderL(TDesC& aMessagePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
			
	RFile file;
	TInt error = file.Open(fs,aMessagePath,EFileRead|EFileShareAny);
	if(error != KErrNone)
		{
		CleanupStack::PopAndDestroy(&fs); // fs
		file.Close();
		fs.Close();
		return EFalse;
		}

	CleanupClosePushL(file);
	
	/*Prepare the stream*/
	CArrayFix<TPairedTUidHBufC8> *iStreams = new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);
		
	RFileReadStream in(file);
	in.PushL();

	TCheckedUid check;
	in >> check;
	if(check.UidType().IsValid()==EFalse) 
		return EFalse;
	
	in >> *iStreams;; 
		
	TPairedTUidHBufC8 entry;
	entry.iUid=KUidMsgFileIMailEncodedHeader;
	
	TKeyArrayFix key(_FOFF(TPairedTUidHBufC8,iUid),ECmpTInt32);
	
	TInt pos;
	TInt found = (iStreams->FindIsq(entry,key,pos));		

	if(found == KErrNone)
		{
		HBufC8 *buffer=iStreams->At(pos).iBuf;	
		HMsvReadBuffer *buf = new (ELeave) HMsvReadBuffer(*buffer);
		RReadStream infoencodedHeader(buf);
		
		TInt version = infoencodedHeader.ReadUint16L();
		
		HBufC8* from    = HBufC8::NewL(infoencodedHeader, KMaxImHeaderStringLength);
		HBufC8* replyTo = HBufC8::NewL(infoencodedHeader, KMaxImHeaderStringLength);
		HBufC8* subject = HBufC8::NewL(infoencodedHeader, KMaxImHeaderStringLength);

		CDesCArray* to  = new (ELeave) CDesCArrayFlat( KArrayGranularity );		
		CDesCArray* cc  = new (ELeave) CDesCArrayFlat( KArrayGranularity );		
		CDesCArray* bcc  = new (ELeave) CDesCArrayFlat( KArrayGranularity );		
		
		infoencodedHeader >> *to;
		infoencodedHeader >> *cc;
		infoencodedHeader >> *bcc;

		TInt decodeCharset = infoencodedHeader.ReadUint32L();
		
		// prepare the RBuf
		RBuf strbuf;
		CleanupClosePushL(strbuf);	
		strbuf.CreateL(KMaxBufferLength);
		strbuf.AppendNum(version);
		
		strbuf.Append(KSeparator);
				
		HBufC16* fromTextValue = HBufC16::NewL(KMaxImHeaderStringLength);
		fromTextValue->Des().Copy(from->Des());
		strbuf.Append(fromTextValue->Des());
		delete from;
		delete fromTextValue;
		
		strbuf.Append(KSeparator);
		
		HBufC16* replyTextValue = HBufC16::NewL(KMaxImHeaderStringLength);
		replyTextValue->Des().Copy(replyTo->Des());
		strbuf.Append(replyTextValue->Des());	
		delete replyTo;
		delete replyTextValue;
			
		strbuf.Append(KSeparator);
		
		HBufC16* subjectTextValue = HBufC16::NewL(KMaxImHeaderStringLength);
		subjectTextValue->Des().Copy(subject->Des());
		strbuf.Append(subjectTextValue->Des());
		delete subject;
		delete subjectTextValue;

		strbuf.Append(KSeparator);
		
		for(TInt i = 0;i< to->MdcaCount();i++)
			{
			strbuf.Append(to->MdcaPoint(i));
			if((i + 1) != (to->MdcaCount()))

				{
				strbuf.Append(KComma);
				}
			}
		strbuf.Append(KSeparator);
		delete to;
		to = NULL;
		
		for(TInt i = 0;i< cc->MdcaCount();i++)
			{
			strbuf.Append(cc->MdcaPoint(i));
			if((i+1) != cc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			}
		strbuf.Append(KSeparator);
		delete cc;
		cc = NULL;
				
		
		for(TInt i = 0;i< bcc->MdcaCount();i++)
			{
			strbuf.Append(bcc->MdcaPoint(i));
			if((i + 1) != bcc->MdcaCount())
				{
				strbuf.Append(KComma);
				}
			}
		strbuf.Append(KSeparator);
		delete bcc;
		bcc = NULL;
				
		
		strbuf.AppendNum(decodeCharset);
		strbuf.Append(KSeparator);
		
		CHeaderFields* emailEncodedHeaderFields = new (ELeave) CHeaderFields();
		CleanupStack::PushL(emailEncodedHeaderFields);
		emailEncodedHeaderFields->iUid = KUidMsgFileIMailEncodedHeader;
		CleanupStack::Pop(emailEncodedHeaderFields);
		
		CFieldPair* emailHeaderVersionfield = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailHeaderVersionfield);
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailHeaderVersionfield);
		CleanupStack::Pop(emailHeaderVersionfield);

		CFieldPair* emailReceiptAddress = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReceiptAddress);
		emailReceiptAddress->iFieldTextValue = KSpace().Alloc();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailReceiptAddress);
		CleanupStack::Pop(emailReceiptAddress);
		
		CFieldPair* emailImMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailImMsgId);
		emailImMsgId->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailImMsgId);
		CleanupStack::Pop(emailImMsgId);
				
		CFieldPair* emailFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailFrom);
		emailFrom->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailFrom);
		CleanupStack::Pop(emailFrom);

		CFieldPair* emailReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReplyTo);
		emailReplyTo->iFieldTextValue = KSpace().AllocL();;
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailReplyTo);
		CleanupStack::Pop(emailReplyTo);
		
		CFieldPair* emailSubject = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailSubject);
		emailSubject->iFieldTextValue = KSpace().AllocL();;
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailSubject);
		CleanupStack::Pop(emailSubject);

		CFieldPair* emailRemoteSize = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailRemoteSize);
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailRemoteSize);
		CleanupStack::Pop(emailRemoteSize);

		CFieldPair* emailToRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailToRecipients);
		emailToRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailToRecipients);	
		CleanupStack::Pop(emailToRecipients);

		CFieldPair* emailCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCcRecipients);
		emailCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailCcRecipients);	
		CleanupStack::Pop(emailCcRecipients);

		CFieldPair* emailBCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBCcRecipients);
		emailBCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailBCcRecipients);
		CleanupStack::Pop(emailBCcRecipients);
		
		//Read Email encoding info
		CFieldPair* emailEncodingInfo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailEncodingInfo);
		emailEncodingInfo->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailEncodingInfo);
		CleanupStack::Pop(emailEncodingInfo);
		
		CFieldPair* emailResentMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentMsgId);
		emailResentMsgId->iFieldTextValue = KSpace().AllocL();;
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailResentMsgId);
		CleanupStack::Pop(emailResentMsgId);

		CFieldPair* emailResentFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentFrom);
		emailResentFrom->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailResentFrom);
		CleanupStack::Pop(emailResentFrom);
		
		CFieldPair* emailResentToRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentToRecipients);
		emailResentToRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailResentToRecipients);
		CleanupStack::Pop(emailResentToRecipients);

		CFieldPair* emailResentCcRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentCcRecipients);
		emailResentCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailResentCcRecipients);
		CleanupStack::Pop(emailResentCcRecipients);

		CFieldPair* emailResentBccRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentBccRecipients);
		emailResentBccRecipients->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailResentBccRecipients);
		CleanupStack::Pop(emailResentBccRecipients);

		CFieldPair* emailInReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailInReplyTo);
		emailInReplyTo->iFieldTextValue = KSpace().AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailInReplyTo);
		CleanupStack::Pop(emailInReplyTo);

		CFieldPair* emailBodyEncoding = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBodyEncoding);
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailBodyEncoding);
		CleanupStack::Pop(emailBodyEncoding);

		CFieldPair* emailCharset = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCharset);
		emailEncodedHeaderFields->iFieldPairList.AppendL(emailCharset);
		CleanupStack::Pop(emailCharset);

		CFieldPair* emaildetailField = new (ELeave)CFieldPair();
		CleanupStack::PushL(emaildetailField);
		emaildetailField->iFieldType = ETextField;
		emaildetailField->iFieldTextValue = strbuf.AllocL();
		emailEncodedHeaderFields->iFieldPairList.AppendL(emaildetailField);
		CleanupStack::Pop(emaildetailField);
		
		iEmailfieldPairList.AppendL(emailEncodedHeaderFields);
		
		CleanupStack::PopAndDestroy(); //strbuf
	
		infoencodedHeader.Close();
		}
	TInt count=iStreams->Count();
	while(count--) 
		{
		delete iStreams->At(count).iBuf;
		}
				
	delete iStreams;
	iStreams=NULL;
	CleanupStack::PopAndDestroy(3); //fs, in, file
	fs.Close();
	in.Close();
	return KErrNone;
	}


/**
ReadEmailMimeHeaderL()
Reads an email message mime header from the specified path and fills RPointerArray of CHeaderFields
from these read header objects.

@param aMessagePath: message path.
@retun TInt: Error code specifying if the message was read successfully.
@internalComponent
*/
TInt CMsvVersion1Version2Converter::ReadEmailMimeHeaderL(TDesC& aMessagePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFile file;
	TInt error = file.Open(fs,aMessagePath,EFileRead|EFileShareAny);
	if(error != KErrNone) 
		{
		CleanupStack::PopAndDestroy(&fs); //fs
		file.Close();
		fs.Close();
		return error;
		}
	CleanupClosePushL(file);
	
	CArrayFix<TPairedTUidHBufC8> *iStreams = new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);;
		
	RFileReadStream in(file);
	in.PushL();

	TCheckedUid check;
	in >> check;
	if(check.UidType().IsValid()==EFalse) 
		return EFalse;
	
	in >> *iStreams;; 

	TPairedTUidHBufC8 entry;
	entry.iUid=KUidMsgFileMimeHeader;
	TKeyArrayFix key(_FOFF(TPairedTUidHBufC8,iUid),ECmpTInt32);
	
	TInt pos;
	TBool found = (iStreams->FindIsq(entry,key,pos));		

	if(found == KErrNone)
		{
		HBufC8 *buffer=iStreams->At(pos).iBuf;	
		HMsvReadBuffer *buf = new (ELeave) HMsvReadBuffer(*buffer);

		RReadStream infoMimeHeader(buf);
		
		RBuf8 mimeStr;
		mimeStr.CreateL(KMaxBufferLength);
		CleanupClosePushL(mimeStr);
				
		TInt version = infoMimeHeader.ReadUint16L();
		mimeStr.AppendNum(version);
		mimeStr.Append(KSeparator);
		
		HBufC8* relativePath = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(relativePath->Des());
		mimeStr.Append(KSeparator);
		delete relativePath;
				
		HBufC8* contentType = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentType->Des());
		mimeStr.Append(KSeparator);
		delete contentType;
				
		HBufC8* contentSubType = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentSubType->Des());
		mimeStr.Append(KSeparator);
		delete contentSubType;


		TInt contentTransferEncoding =  infoMimeHeader.ReadInt8L();
		mimeStr.AppendNum(contentTransferEncoding);
		mimeStr.Append(KSeparator);
		
		HBufC8* contentDisposition = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentDisposition->Des());
		mimeStr.Append(KSeparator);
		delete contentDisposition;
		
		HBufC8* contentDescription = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentDescription->Des());
		mimeStr.Append(KSeparator);
		delete contentDescription;
		
		HBufC8* contentBase = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentBase->Des());
		mimeStr.Append(KSeparator);
		delete contentBase;
		
		HBufC16* contentLocation = HBufC16::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentLocation->Des());
		mimeStr.Append(KSeparator);
		delete contentLocation;
		
		HBufC8* contentID = HBufC8::NewL( infoMimeHeader, KMaxImMimeFieldLength );
		mimeStr.Append(contentID->Des());
		mimeStr.Append(KSeparator);
		delete contentID;
		
		CDesC8Array* contentTypeParams  = new (ELeave)CDesC8ArrayFlat( KDesCArrayGranularity );
		CDesC8Array* contentDispositionParams  = new (ELeave)CDesC8ArrayFlat( KDesCArrayGranularity );	
		CDesC8Array* xTypeParams  = new(ELeave) CDesC8ArrayFlat(KDesCArrayGranularity);
		
		infoMimeHeader >> *contentTypeParams;
		for(TInt i = 0; i<contentTypeParams->Count(); i++)
			{
			mimeStr.Append(contentTypeParams->MdcaPoint(i));
			if((i + 1) != contentTypeParams->MdcaCount())
				{
				mimeStr.Append(KComma);
				}
			}
		delete contentTypeParams;
		contentTypeParams = NULL;
		
		mimeStr.Append(KSeparator);
				
		infoMimeHeader >> *contentDispositionParams;
		for(TInt i = 0; i<contentDispositionParams->Count(); i++)
			{
			mimeStr.Append(contentDispositionParams->MdcaPoint(i));
			if((i + 1) != contentDispositionParams->MdcaCount())
				{
				mimeStr.Append(KComma);
				}
			}
			
		delete contentDispositionParams;
		contentDispositionParams = NULL;
		
		mimeStr.Append(KSeparator);
		
		infoMimeHeader >> *xTypeParams;
		for(TInt i = 0; i<xTypeParams->Count(); i++)
			{
			mimeStr.Append(xTypeParams->MdcaPoint(i));
			
			if((i + 1) != xTypeParams->MdcaCount())
				{
				mimeStr.Append(KComma);
				}
			}
		delete xTypeParams;
		xTypeParams = NULL;
		
		mimeStr.Append(KSeparator);
			
		TInt mimeCharset = infoMimeHeader.ReadUint32L();
		mimeStr.AppendNum(mimeCharset);
		mimeStr.Append(KSeparator);
			
		/*Lets start reading the Header Info */
		CHeaderFields* emailMimeHeaderFields = new (ELeave) CHeaderFields();
		emailMimeHeaderFields->iUid = KUidMsgFileMimeHeader;
		
		CFieldPair* emailHeaderVersionfield = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailHeaderVersionfield);
		emailMimeHeaderFields->iFieldPairList.AppendL(emailHeaderVersionfield);
		CleanupStack::Pop(emailHeaderVersionfield);

		CFieldPair* emailReceiptAddress = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReceiptAddress);
		emailReceiptAddress->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailReceiptAddress);
		CleanupStack::Pop(emailReceiptAddress);
		
		CFieldPair* emailImMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailImMsgId);
		emailImMsgId->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailImMsgId);
		CleanupStack::Pop(emailImMsgId);
		
		CFieldPair* emailFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailFrom);
		emailFrom->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailFrom);
		CleanupStack::Pop(emailFrom);

		CFieldPair* emailReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailReplyTo);
		emailReplyTo->iFieldTextValue = KSpace().AllocL();;
		emailMimeHeaderFields->iFieldPairList.AppendL(emailReplyTo);
		CleanupStack::Pop(emailReplyTo);
		
		CFieldPair* emailSubject = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailSubject);
		emailSubject->iFieldTextValue = KSpace().AllocL();;
		emailMimeHeaderFields->iFieldPairList.AppendL(emailSubject);
		CleanupStack::Pop(emailSubject);

		CFieldPair* emailRemoteSize = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailRemoteSize);
		emailMimeHeaderFields->iFieldPairList.AppendL(emailRemoteSize);
		CleanupStack::Pop(emailRemoteSize);

		CFieldPair* emailToRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailToRecipients);
		emailToRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailToRecipients);	
		CleanupStack::Pop(emailToRecipients);
			
		CFieldPair* emailCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCcRecipients);
		emailCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailCcRecipients);	
		CleanupStack::Pop(emailCcRecipients);

		CFieldPair* emailBCcRecipients = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBCcRecipients);
		emailBCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailBCcRecipients);
		CleanupStack::Pop(emailBCcRecipients);
		
		//Read Email encoding info
		CFieldPair* emailEncodingInfo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailEncodingInfo);
		emailEncodingInfo->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailEncodingInfo);
		CleanupStack::Pop(emailEncodingInfo);
		
		CFieldPair* emailResentMsgId = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentMsgId);
		emailResentMsgId->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailResentMsgId);
		CleanupStack::Pop(emailResentMsgId);

		CFieldPair* emailResentFrom = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentFrom);
		emailResentFrom->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailResentFrom);
		CleanupStack::Pop(emailResentFrom);
		
		CFieldPair* emailResentToRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentToRecipients);
		emailResentToRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailResentToRecipients);
		CleanupStack::Pop(emailResentToRecipients);

		CFieldPair* emailResentCcRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentCcRecipients);
		emailResentCcRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailResentCcRecipients);
		CleanupStack::Pop(emailResentCcRecipients);

		CFieldPair* emailResentBccRecipients= new (ELeave)CFieldPair();
		CleanupStack::PushL(emailResentBccRecipients);
		emailResentBccRecipients->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailResentBccRecipients);
		CleanupStack::Pop(emailResentBccRecipients);

		CFieldPair* emailInReplyTo = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailInReplyTo);
		emailInReplyTo->iFieldTextValue = KSpace().AllocL();
		emailMimeHeaderFields->iFieldPairList.AppendL(emailInReplyTo);
		CleanupStack::Pop(emailInReplyTo);

		CFieldPair* emailBodyEncoding = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailBodyEncoding);
		emailMimeHeaderFields->iFieldPairList.AppendL(emailBodyEncoding);
		CleanupStack::Pop(emailBodyEncoding);

		CFieldPair* emailCharset = new (ELeave)CFieldPair();
		CleanupStack::PushL(emailCharset);
		emailMimeHeaderFields->iFieldPairList.AppendL(emailCharset);
		CleanupStack::Pop(emailCharset);
		// 8bit to 16bit
		HBufC* newMimeStr = HBufC::NewL(mimeStr.Length());
		newMimeStr->Des().Copy(mimeStr);
		
		CFieldPair* emaildetailField = new (ELeave)CFieldPair();
		CleanupStack::PushL(emaildetailField);
		emaildetailField->iFieldName = KDetails().AllocL();;
		emaildetailField->iFieldType = ETextField;
		emaildetailField->iFieldTextValue= newMimeStr;
		emailMimeHeaderFields->iFieldPairList.AppendL(emaildetailField);
		CleanupStack::Pop(emaildetailField);
		
		iEmailfieldPairList.AppendL(emailMimeHeaderFields);
		
		CleanupStack::PopAndDestroy(); //mimestr
				
		infoMimeHeader.Close();
		}
	TInt count=iStreams->Count();
	while(count--) 
		{
		delete iStreams->At(count).iBuf;
		}
	delete iStreams;
	iStreams=NULL;
	CleanupStack::PopAndDestroy(3); //fs,file,in
	
	fs.Close();
	in.Close();
	
	return KErrNone;
	}

/* start SMS conversion APIs */

/**
Read and Insert header and body part of the SMS message. 
@param aServiceIdPosition		Service Id belong to Sms MTM.
@return void.
@internalComponent
*/

void CMsvVersion1Version2Converter::InternalizeSmsMessageL(TMsvId aServiceId,TUint32 aMtm)
	{
	RArray<TMsvId> iDArray;
	CleanupClosePushL (iDArray);
	TMsvId iD;
		
	iDBAdapter.BeginTransactionL();		
	/* Get all TMsvIds belong to one service Id */
	iDBAdapter.GetAllMessageIdsL(aServiceId,iDArray,aMtm, iResumeStatus.lastId);
	iResumeStatus.remainingCount = iDArray.Count();
	
	for(TInt i=0;i<iDArray.Count();i++)	
		{
		iResumeStatus.lastId = iDArray[i];
		iResumeStatus.remainingCount--;
		
		RBuf path;
		CleanupClosePushL(path);
		path.CreateL(KMaxPath);
		iDBAdapter.GetOwningServiceL(iDArray[i],iD);
		CreatePathL(path,iDArray[i],iD);
		
		InitializeL();
		/* Read Sms Header part of the message */
		TBool foundSmsHeader = ReadSmsMessageHeaderL(path);
	
		iEmailfieldPairList.AppendL(iSmsHeaderFields);
		iEmailfieldPairList.AppendL(iEmailOverSmsAddressFields);
		 
		// Insert Sms header into DB
		if(foundSmsHeader)
			{
			iMessageDBAdapter.CreateHeaderEntryL(TUid::Uid(aMtm), iDArray[i], iEmailfieldPairList);
			}
		
		//update the conversion status
		if(i % KDBCommitCount == 0 && i != 0)
			{
			iResumeStatus.lastId = iDArray[i];
			iResumeStatus.serviceId = aServiceId;
			iResumeStatus.remainingCount -= KDBCommitCount; 
				
			iDBAdapter.UpdateConversionStatusEntryL(iResumeStatus);
			iDBAdapter.CommitTransactionL();
			iDBAdapter.BeginTransactionL();
			}
		DeAllocateMemory(); // destroy the allocated resources  	
		path.Close();
		CleanupStack::PopAndDestroy(); //path
		}
	CleanupStack::PopAndDestroy(&iDArray);					   	
	
	// check if we have a request for Cancellation.
	// if one is found than we immediately cancel without comminting to the database.
	TInt cancel = GetPropertyValue();
	if(cancel == KErrCancel	)
		{
		User::Leave(KErrCancel);
		}
						
	
	//update status for this service id
	iResumeStatus.lastId = 0;
	iResumeStatus.serviceId = aServiceId;
	iResumeStatus.remainingCount = 0; 
	iDBAdapter.UpdateConversionStatusEntryL(iResumeStatus);
	iDBAdapter.CommitTransactionL();
	
	}

/**
Create SMS header table in DB. 
@param None.
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::CreateSmsHeaderTableL()
	{
	AddFieldL(KSmsHeadearversion().AllocL(),EIntegerField);
	AddFieldL(KSmsRecipientCount().AllocL(),EIntegerField);
	AddFieldL(KRecipients().AllocL(),ETextField);
	AddFieldL(KSmsFlags().AllocL(),EIntegerField);
	AddFieldL(KTextBioMsgIdType().AllocL(),EIntegerField);
	}

/**
Internalizing CMsvRecipient fields and Sms Number fields from file store. 
@param aStream					A ref for reading the content from file store.
@param aDetailsOfRecipients		Recipient detailfields buffer.
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::InternalizeSmsHeaderL(RReadStream& aStream,TDes16& aDetailsOfRecipients)
	{
	TInt16 version = aStream.ReadInt16L();
	
	// Its internalize as per CMsvRecipient::InternalizeL(aStream) 
	TRecipientStatus aStatus = (TRecipientStatus)aStream.ReadInt32L();
	aDetailsOfRecipients.AppendNum(aStatus);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsRecipientStatus = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsRecipientStatus);
	smsRecipientStatus->iFieldNumValue = (TInt64)aStatus;
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsRecipientStatus);
	CleanupStack::Pop(smsRecipientStatus);
	
	TInt32 aError = aStream.ReadInt32L();
	aDetailsOfRecipients.AppendNum(aError);
	aDetailsOfRecipients.Append(KComma);
	CFieldPair* smsError = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsError);
	smsError->iFieldNumValue = (TInt64)aError;
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsError);
	CleanupStack::Pop(smsError);
	
	TInt aRetries = aStream.ReadInt32L();
	aDetailsOfRecipients.AppendNum(aRetries);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsRetries = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsRetries);
	smsRetries->iFieldNumValue = (TInt64)aRetries;
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsRetries);
	CleanupStack::Pop(smsRetries);
	
	TInt64 time; 
	aStream >> time;
	aDetailsOfRecipients.AppendNum(time);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsTime = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsTime);
	smsTime->iFieldNumValue = time;
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsTime);
	CleanupStack::Pop(smsTime);
	// end of CMsvRecipient::Internalize(aStream)
	
	HBufC16* number = HBufC16::NewL(aStream, KSmcmSmsNumberMaxNumberLength);
	CleanupStack::PushL(number);
	aDetailsOfRecipients.Append(*number);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsNumber = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsNumber);
	smsNumber->iFieldTextValue = number;
	CleanupStack::Pop(smsNumber);
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsNumber);
	
	CleanupStack::Pop(number);
	
	HBufC* name = HBufC::NewL(aStream, KSmcmSmsNumberMaxNameLength);
	CleanupStack::PushL(name);
	aDetailsOfRecipients.Append(*name);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsName = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsName);
	smsName->iFieldName = name;
	CleanupStack::Pop(smsName);
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsName);
	
	CleanupStack::Pop(name);

	TLogId logId;
	logId = aStream.ReadInt32L();
	aDetailsOfRecipients.AppendNum(logId);
	aDetailsOfRecipients.Append(KComma);
	
	CFieldPair* smsLogId = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsLogId);
	smsLogId->iFieldNumValue = (TInt64)logId;
	CleanupStack::Pop(smsLogId);
	iSmsRecipientAddressFields->iFieldPairList.AppendL(smsLogId);
	
	CFieldPair* smsDeliverySatus = new (ELeave)CFieldPair();

	TSmsAckStatus deliveryStatus;
	if( version >= 2 )
		{
		deliveryStatus = (TSmsAckStatus) aStream.ReadInt8L();
		aDetailsOfRecipients.AppendNum(deliveryStatus);
		aDetailsOfRecipients.Append(KDelimiter);
		
		smsDeliverySatus->iFieldNumValue = (TInt64)deliveryStatus;
		iSmsRecipientAddressFields->iFieldPairList.AppendL(smsDeliverySatus);
		}
	else
		{
		deliveryStatus = ENoAckRequested;
		aDetailsOfRecipients.AppendNum(deliveryStatus);
		aDetailsOfRecipients.Append(KDelimiter);
		
		smsDeliverySatus->iFieldNumValue = (TInt64)deliveryStatus;
		iSmsRecipientAddressFields->iFieldPairList.AppendL(smsDeliverySatus);	
		}
	}

/**
Internalizing the Sms header part of the message from file store. 
@param aPath:	Path which identify the location of the file.
@return TInt 		Return error if path or sms header is not found.
@internalComponent
*/
	
TBool CMsvVersion1Version2Converter::ReadSmsMessageHeaderL(TDes& aPath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFile file;
	TInt error = file.Open(fs,aPath,EFileRead|EFileShareAny);
	CleanupClosePushL(file);
	
	if(error !=KErrNone)
		{
		CleanupStack::PopAndDestroy(); //fs
		CleanupStack::PopAndDestroy(); // file
		return EFalse;
		}

	RFileReadStream in(file);
	in.PushL();
	TCheckedUid check;
	in >> check;
	
	if(check.UidType().IsValid()==EFalse) 
		{
		return EFalse;
		}	
	if(check.UidType()[0]!=KMsvEntryFileForSms) 
		{
		return EFalse;	
		}

	iStreams = new(ELeave) CArrayFixFlat<TPairedTUidHBufC8>(5);
	in >> *iStreams;
	
	TInt position = 0;
	error = FindStream(KUidSMSHeaderStream,position);	
	if(error == KErrNone)
		{
		
		HBufC8 *buffer = iStreams->At(position).iBuf;
		HMsvReadBuffer *buf = new (ELeave) HMsvReadBuffer(*buffer);
		CleanupStack::PushL(buf);
		RReadStream smsHeader(buf);
		CleanupStack::Pop(buf);
		
		iSmsHeaderFields->iUid = KUidSMSHeaderStream;
		
		//Version
		CFieldPair* smsHeaderVersionField = new (ELeave)CFieldPair();
		CleanupStack::PushL(smsHeaderVersionField);
		smsHeaderVersionField->iFieldNumValue = (TInt64)smsHeader.ReadInt16L();
		iSmsHeaderFields->iFieldPairList.AppendL(smsHeaderVersionField);
		CleanupStack::Pop(smsHeaderVersionField);
		
		//internalizing recipient count from sms header
		TInt32 recipientCount = smsHeader.ReadInt32L();
		CFieldPair* smsRecipientCountField = new (ELeave)CFieldPair();
		CleanupStack::PushL(smsRecipientCountField);
		smsRecipientCountField->iFieldNumValue = (TInt64)recipientCount;
		iSmsHeaderFields->iFieldPairList.AppendL(smsRecipientCountField);
		CleanupStack::Pop(smsRecipientCountField);
		
		iRecipientDetails.Create(KMaxLengthOfRecipientField*recipientCount);
		
		while(recipientCount--)
			{
			InternalizeSmsHeaderL(smsHeader,iRecipientDetails);
			}
		
		CFieldPair* detailsOfRecipientField = new (ELeave)CFieldPair();
		CleanupStack::PushL(detailsOfRecipientField);
		detailsOfRecipientField->iFieldTextValue = iRecipientDetails.AllocL();
		iSmsHeaderFields->iFieldPairList.AppendL(detailsOfRecipientField);
		CleanupStack::Pop(detailsOfRecipientField);
		
		// reading sms flag
		TUint32 iFlags = smsHeader.ReadUint32L();
		CFieldPair* smsHeaderFlagField = new (ELeave)CFieldPair();
		CleanupStack::PushL(smsHeaderFlagField);
		smsHeaderFlagField->iFieldNumValue = (TInt64)iFlags;
		iSmsHeaderFields->iFieldPairList.AppendL(smsHeaderFlagField);
		CleanupStack::Pop(smsHeaderFlagField);
		
		// reading sms biomsgtype
		TInt64 iBioMsgIdType = smsHeader.ReadInt8L();
		CFieldPair* smsHeaderBioMsgFlagField = new (ELeave)CFieldPair();
		CleanupStack::PushL(smsHeaderBioMsgFlagField);
		smsHeaderBioMsgFlagField->iFieldNumValue = iBioMsgIdType;
		iSmsHeaderFields->iFieldPairList.AppendL(smsHeaderBioMsgFlagField);
		CleanupStack::Pop(smsHeaderBioMsgFlagField);
		
		CFieldPair* smsDetailField = new (ELeave)CFieldPair();
		CleanupStack::PushL(smsDetailField);
		smsDetailField->iFieldTextValue = KSpace().AllocL();
		iSmsHeaderFields->iFieldPairList.AppendL(smsDetailField);
		CleanupStack::Pop(smsDetailField);
		
		/* Started reading EmailOverSms part of the Message */	
		TInt eosPosition = 0;
		error = FindStream(KUidMsvSmsEmailFieldsStream,eosPosition);

		if(error != KErrNone)
			{	
			FillEmptyEosBufferL();  // It meant Sms message does not have EOS fields
			}
		else
			{
			HBufC8 *eosBuffer = iStreams->At(position).iBuf;
			CleanupStack::PushL(eosBuffer);
			HMsvReadBuffer *eosBuf = new (ELeave) HMsvReadBuffer(*eosBuffer);
			CleanupStack::PushL(eosBuf);
			RReadStream smsEmailHeaderField(eosBuf);
			
			InternalizeEmailOverSmsHeaderL(smsEmailHeaderField);
			
			CleanupStack::Pop(eosBuf);
			CleanupStack::Pop(eosBuffer);	
			
			smsHeader.Close();
			smsEmailHeaderField.Close();
			}
		}
		
	TInt count=iStreams->Count();
	while(count--) 
		{
		delete iStreams->At(count).iBuf;
		}
	delete iStreams;
	iStreams=NULL;
	in.Pop();
	in.Close();
	
	CleanupStack::PopAndDestroy(); //file
	file.Close();
	CleanupStack::PopAndDestroy(); // fs
	return ETrue;	
	}

/**
Internalizing the EOS fields from file store. 
@param aSmsHeaderEmailOverSmsPart		A ref for reading the content from file store.
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::InternalizeEmailOverSmsHeaderL(RReadStream& aSmsHeaderEmailOverSmsPart)
	{	
	iEmailOverSmsAddress.Create(KSmcmSmsNumberMaxNumberLength*KMaxConverterNumber);
	iEmailOverSmsAddressFields->iUid = 	KUidMsvSmsEmailFieldsStream;
	
	// reading EmailOverSms version
	iEmailOverSmsAddress.AppendNum(aSmsHeaderEmailOverSmsPart.ReadInt16L());
	iEmailOverSmsAddress.Append(KDelimiter);

	TCardinality cardinality;
	aSmsHeaderEmailOverSmsPart >> cardinality;
	TInt count = cardinality;
	  
	iEmailOverSmsAddress.AppendNum(count);
	iEmailOverSmsAddress.Append(KDelimiter);	

	for( TInt i = 0; i < count; ++i )
		{
		HBufC* buf = HBufC::NewLC(aSmsHeaderEmailOverSmsPart, KMaxTInt);
		iEmailOverSmsAddress.Append(*buf);
		if(i<count-1)
		iEmailOverSmsAddress.Append(KComma);		
		CleanupStack::Pop(buf);
		}
	iEmailOverSmsAddress.Append(KDelimiter);
		
	// read the subject 	
	HBufC* emailOverSmsSubject = HBufC::NewLC(aSmsHeaderEmailOverSmsPart, KMaxTInt);
	iEmailOverSmsAddress.Append(*emailOverSmsSubject);
	iEmailOverSmsAddress.Append(KDelimiter);
	CleanupStack::Pop(emailOverSmsSubject);
	
	CFieldPair* eosAddressField = new (ELeave)CFieldPair();
	CleanupStack::PushL(eosAddressField);
	eosAddressField->iFieldTextValue = iEmailOverSmsAddress.AllocL();  
	iEmailOverSmsAddressFields->iFieldPairList.AppendL(eosAddressField);
	CleanupStack::Pop(eosAddressField);
	
	}

/**
Initialising the objects. 
@param None.
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::InitializeL()
	{
	iSmsHeaderFields = new (ELeave) CHeaderFields();
	iSmsRecipientAddressFields = new (ELeave) CHeaderFields();
	iEmailOverSmsAddressFields = new (ELeave) CHeaderFields();
	}
	
/**
Destroying the memory allocated. 
@param None.
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::DeAllocateMemory()
	{
	iEmailfieldPairList.ResetAndDestroy();	
	if(iSmsRecipientAddressFields != NULL)
	{
	delete iSmsRecipientAddressFields;
	iSmsRecipientAddressFields = NULL;	
	}
		
	iRecipientDetails.Close();
	iEmailOverSmsAddress.Close();
	}
	
	
/**
Find the stream by passing the Uid of the stream 
@param aUid. 				The const which identify to start position for reading.
@param aPos. 				Starting position of the stream.
@return TInt				returning the error if stream not found.
@internalComponent
*/
TInt CMsvVersion1Version2Converter::FindStream(TUid aUid,TInt &aPos)
	{
	TPairedTUidHBufC8 entry;
	entry.iUid=aUid;
	TKeyArrayFix key(_FOFF(TPairedTUidHBufC8,iUid),ECmpTInt32);
	return(iStreams->FindIsq(entry,key,aPos));
	}

/**
Store the empty buffer part in emailoversms header fields
@param None
@return void.
@internalComponent
*/
void CMsvVersion1Version2Converter::FillEmptyEosBufferL()
	{
	iEmailOverSmsAddressFields->iUid = 	KUidMsvSmsEmailFieldsStream;
	
	CFieldPair* smsdetailField = new (ELeave)CFieldPair();
	CleanupStack::PushL(smsdetailField);
	smsdetailField->iFieldTextValue = KSpace().AllocL();
	iEmailOverSmsAddressFields->iFieldPairList.AppendL(smsdetailField);
	CleanupStack::Pop(smsdetailField);	
	}
	
