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

// Local includes
//
#include "CUnknownContentHandler.h"

// System includes
//
#include <biodb.h>		// BIO Message Database and message query methods
#include <gsmubuf.h>	// CSmsBuffer class
#include <gsmumsg.h>	// CSmsMessage class
#include <smuthdr.h>	// CSmsHeader class
#include <msvuids.h>	// KUidMsvMessageEntry const
#include <smut.h>		// TSmsUtilities class
#include <txtrich.h>	// CRichText class
#include <biouids.h>	// KUidBioMessageTypeMtm const
#include <push/pushmessage.h>
#include <wspdecoder.h>
#include <push/cunknownpushmsgentry.h>
#include <push/cwappushmsgutils.h>


#if defined(_DEBUG)
_LIT(KErrPushMsgNull,	"NULL CPushMessage");
_LIT(KAssertFailed,     "Assertion failed");
#endif

// Constants
_LIT(KReserved, "Reserved");
_LIT8(KFilename8, "filename");


void CUnknownContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}


void CUnknownContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}


/**
 * The Unknown Content handler private constructor
 * Index number : EUnknownContentHandlerIndex 
 */ 
CUnknownContentHandler::CUnknownContentHandler()
: CContentHandlerBase()
	{
	}


/**
 *  This will complete initialization of the object
 */
void CUnknownContentHandler::ConstructL()
	{
	iWapPushUtils = CWapPushMsgUtils::NewL();
	// Note that CSmsGetDetDescInterface is an ECOM interface.
	iSmsDetDesc = CSmsGetDetDescInterface::NewL();
	CActiveScheduler::Add(this);
	}


/**
 * Static Factory Construction
 *
 * Version of NewL which leaves nothing
 * on the cleanup stack
 */
CUnknownContentHandler* CUnknownContentHandler::NewL()
	{
	CUnknownContentHandler* self = new(ELeave) CUnknownContentHandler; 
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


/**
 * Default d'tor
 */
CUnknownContentHandler::~CUnknownContentHandler()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: Destructor Called"); 
	delete iWapPushUtils;
	delete iSmsDetDesc;
	}


/**
 * HandleMessage Async. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *	@param aPushMsg
 *		CPushMessage to process
 *	@param aStatus
 *		request status variable for use in asynchronous operations
 */
void CUnknownContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: HandleMessage Async Func. Called"); 
	__ASSERT_DEBUG( aPushMsg != NULL , User::Panic(KErrPushMsgNull, KErrNone));

	iMessage = aPushMsg;
	iAcknowledge = ETrue;
	SetConfirmationStatus(aStatus);

	iState = EProcessing;
	IdleComplete();
	}


/**
 * HandleMessage Sync. Version
 *	Takes ownership of Push Message and sets self active to continue
 *	processing message.
 *	@param aPushMsg
 *		CPushMessage to process
 */
void CUnknownContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: HandleMessage Sync Func. Called"); 
	__ASSERT_DEBUG( aPushMsg != NULL , User::Panic(KErrPushMsgNull, KErrNone));
	
	iAcknowledge = EFalse;
	iMessage = aPushMsg;
	
	iState = EProcessing;
	IdleComplete();
	}


/**
 *	Create/Saving Push Msg Entry in the appropriate message store.
 *	First query the type of push message to check if it is a known BIO
 *	message type: 
 *	If it is then 
 *		save Push Message as appropriate BIO message
 *	else
 *		save as UnknownPushMsgEntry.
 *
 *  The supplied PushMsg is deleted in any case in the destructor.
 */
void CUnknownContentHandler::ProcessingPushMsgEntryL()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: ProcessingPushMsgEntryL Called"); 
	
	TBool bioMsg = BioMessageTypeL();
	if (bioMsg)
		// convert to bio format & save
		SaveBioMessageEntryL();
	else
		// create unknown push message entry and save
		SaveUnknownPushMsgEntryL();
		
	iState = EDone;
	IdleComplete();
	}


/** 
 *	Check if the given Push Message, which is now owned by the handler,
 *	is a BIO Messsage (setting the member variable iBioMsgUID if it is)
 *	and returning a boolean to indicate success (ETrue) or EFalse otherwise.
 *	@return TBool
 *		ETrue - push message is a BIO message
 *		EFalse - push message is not BIO message
 */
TBool CUnknownContentHandler::BioMessageTypeL()
	{
	// need to create local RFs for BIO otherwise raises exception
	RFs localFS;
	CleanupClosePushL(localFS);
	User::LeaveIfError(localFS.Connect());
	CBIODatabase* bioDB = CBIODatabase::NewLC(localFS);

	TPtrC contentTypePtr;
	iMessage->GetContentType(contentTypePtr);
	
	iBioMsgUID = KNullUid;
	TBool isBio = EFalse;
	// IsBioMessageL returns KErrNone if found or KErrNotFound if not found 
	if ( bioDB->IsBioMessageL(EBioMsgIdIana, contentTypePtr, NULL, iBioMsgUID) == KErrNone)
		isBio = ETrue;
		
	CleanupStack::PopAndDestroy(2); // bioDB, localFS

	return isBio;
	}


/**
 *	Save CPushMessage as the relevant BIO message type.
 *	Firstly create SMS message and use CPushMessage as
 *	source to populate SMS. Then save SMS message.
 */
void CUnknownContentHandler::SaveBioMessageEntryL()
	{
	// create sms message from CPushMessage and then save message
	__LOG_PTR_DEBUG("CUnknownContentHandler:: SaveBioMessageEntry Called"); 
	RFs fileSession;
	User::LeaveIfError(fileSession.Connect());
	CleanupClosePushL(fileSession);
	CSmsBuffer* smsBuffer = CSmsBuffer::NewL();

	CSmsMessage* smsMessage = CSmsMessage::NewL(fileSession,CSmsPDU::ESmsDeliver, smsBuffer);
	CleanupStack::PushL(smsMessage);

	smsMessage->SmsPDU().SetAlphabet(TSmsDataCodingScheme::ESmsAlphabet8Bit);
	
	TPtrC8 fieldValue;

	if (iMessage->GetBinaryHeaderField(EHttpFrom, fieldValue) ||
		iMessage->GetBinaryHeaderField(EHttpXWapInitiatorURI, fieldValue) ||
		iMessage->GetBinaryHeaderField(EHttpContentLocation, fieldValue) )
		{
		// Convert 8 bit to 16 bit
		HBufC* tempAddr = HBufC::NewLC(fieldValue.Length());
		tempAddr->Des().Copy(fieldValue);

		smsMessage->SmsPDU().SetToFromAddressL(*tempAddr);
		CleanupStack::PopAndDestroy(tempAddr);
		}
	
	if (iMessage->GetMessageBody(fieldValue))
		{
		HBufC* tempBody = HBufC::NewLC(fieldValue.Length());
		tempBody->Des().Copy(fieldValue);

		smsBuffer->InsertL(0, *tempBody);
		CleanupStack::PopAndDestroy(tempBody);
		}

	CleanupStack::Pop(); //smsMessage
	
	StoreMsgL(smsMessage); //destroys CSmsMessage (contains smsBuffer)
	CleanupStack::PopAndDestroy(&fileSession);
	}


/** 
 *	Create CMsvEntry for sms messsage, stores the entry and 
 *	then delete entry once done.
 *	@param aSmsMsg
 *		CSmsMessage which embodies details of CPushMessage
 */
void CUnknownContentHandler::StoreMsgL(CSmsMessage* aSmsMsg)
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: StoreMsgL Called"); 
	CleanupStack::PushL(aSmsMsg);

	// Create a CSmsHeader based on this message. smsHdr takes ownership of aSmsMsg
	CSmsHeader* smsHdr = CSmsHeader::NewL(aSmsMsg);
	CleanupStack::Pop(aSmsMsg);
	CleanupStack::PushL(smsHdr);

	// get root entry
	CMsvEntry* msvEntry = iWapPushUtils->Session().GetEntryL(KMsvRootIndexEntryId);
	CleanupStack::PushL(msvEntry);

	// create an invisible blank entry 
	TMsvEntry entry;
	entry.iType = KUidMsvMessageEntry;
	entry.SetVisible(EFalse);
	entry.SetInPreparation(ETrue);
	entry.SetReadOnly(EFalse);
	entry.SetUnread(ETrue);

	TMsvId serviceId = SetBioServiceIdL();
	TBuf<KSmsDescriptionLength> description;	

	if (iBioMsgUID != KNullUid)
		{
		// sets entry with values passed in
		TSmsUtilities::PopulateMsgEntry(entry, *aSmsMsg, serviceId,  KUidBIOMessageTypeMtm);
		entry.iBioType = iBioMsgUID.iUid;

		// Look up and set the description
		TInt index;
		CBIODatabase* bioDB = CBIODatabase::NewLC(msvEntry->Session().FileSession());
		bioDB->GetBioIndexWithMsgIDL(iBioMsgUID, index);
		description.Copy(bioDB->BifReader(index).Description());
		entry.iDescription.Set(description);
		CleanupStack::PopAndDestroy();	// bioDB
		}
	else
		{
		// sets entry with values passed in
		TSmsUtilities::PopulateMsgEntry(entry, *aSmsMsg, serviceId);
	
		// Look up and set the description
		iSmsDetDesc->GetDescription(*aSmsMsg, description);
		entry.iDescription.Set(description);
		}
	
	// Set the details
	TBuf<KSmsDetailsLength> details;	
	TInt err = iSmsDetDesc->GetDetails(msvEntry->Session().FileSession(), *aSmsMsg, details);
	if (!err)
		entry.iDetails.Set(details);

	// Store entry in inbox
	msvEntry->SetEntryL(KMsvGlobalInBoxIndexEntryId);
	msvEntry->CreateL(entry);
	msvEntry->Session().CleanupEntryPushL(entry.Id());
	msvEntry->SetEntryL(entry.Id());

	// Save the message
	CMsvStore* store = msvEntry->EditStoreL();
	CleanupStack::PushL(store);

	// Save off the CSmsHdr
	smsHdr->StoreL(*store);

	// Save the body
	
	// Create and fill a CRichText object to save to store..
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
	CleanupStack::PushL(richText);
	HBufC* bufBody=HBufC::NewLC(aSmsMsg->Buffer().Length());
	TPtr bufBodyPtr = bufBody->Des();
	aSmsMsg->Buffer().Extract(bufBodyPtr, 0, aSmsMsg->Buffer().Length());
	richText->InsertL(0, bufBodyPtr); 
	store->StoreBodyTextL(*richText);
	CleanupStack::PopAndDestroy(4); //bufBody, richText, charFormatLayer, paraFormatLayer
	
	store->CommitL();
	entry.SetReadOnly(ETrue);
	entry.SetVisible(ETrue);
	entry.SetInPreparation(EFalse);
	msvEntry->ChangeL(entry);

	CleanupStack::PopAndDestroy(); //store
	msvEntry->Session().CleanupEntryPop(); //entry
	CleanupStack::PopAndDestroy(2); //msvEntry, smsHdr
	}


/** 
 *	Set the service id for Bio messaging
 *	@return TMsvId
 *		the service id for the message store
 */
TMsvId CUnknownContentHandler::SetBioServiceIdL()
    {
	__LOG_PTR_DEBUG("CUnknownContentHandler:: SetBioServiceIdL Called"); 
	CMsvEntry* msvEntry = iWapPushUtils->Session().GetEntryL(KMsvRootIndexEntryId);

	CleanupStack::PushL(msvEntry);
	
	TMsvId serviceId = KMsvNullIndexEntryId; 
	TRAPD(err, TSmsUtilities::ServiceIdL(*msvEntry, serviceId,  KUidBIOMessageTypeMtm));
	if (err == KErrNotFound)
		{
		serviceId = KMsvLocalServiceIndexEntryId;
		err = KErrNone;
		}
	
	CleanupStack::PopAndDestroy(); //msvEntry
	User::LeaveIfError(err);

	return serviceId;
    }


/** 
 * Save CPushMessage as a CUnknownPushMsgEntry
 */
void CUnknownContentHandler::SaveUnknownPushMsgEntryL()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: SaveUnknownPushMsgEntryL. called");
	
	TBool isInt;
	TPtrC8 appURI;
	TInt appID=0;
	iMessage->GetAppIdL(appURI, appID,isInt);

	CUnknownPushMsgEntry* unknownPushMsgEntry =NULL;
	if (isInt)
		{
		unknownPushMsgEntry = CUnknownPushMsgEntry::NewL(appID);
		}
	else
		{
		unknownPushMsgEntry = CUnknownPushMsgEntry::NewL(appURI);
		}

	CleanupStack::PushL(unknownPushMsgEntry);

	SetUnknownPushMsgEntryFieldsL(*unknownPushMsgEntry);

	unknownPushMsgEntry->SaveL(iWapPushUtils->Session(), KMsvGlobalInBoxIndexEntryId);
	__LOG_PTR_DEBUG("CUnknownPushEntry: SaveL called");
	
	CleanupStack::PopAndDestroy(); //unknownPushMsgEntry
	}


/**
 *	Sets the Unknown Push Message Entry Fields from the data in
 *	the received CPushMessage:
 *	@param aUnknownPushMsgEntry
 *		CUnknownPushMsgEntry to populate with CPushMessage details
 */
void CUnknownContentHandler::SetUnknownPushMsgEntryFieldsL(CUnknownPushMsgEntry& aUnknownPushMsgEntry)
	{
	// Set all the relevant fields
	TPtrC contentTypePtr;
	iMessage->GetContentType(contentTypePtr);
	aUnknownPushMsgEntry.SetContentTypeL(contentTypePtr); 
	
	// Set content filename if it exists
	TFileName filename;
	if (GetContentFileNameL(filename))
		{
		// This could leave because the file name specified in the Content-disposition header
		// is not a valid Symbian OS filename.
		TRAP_IGNORE(aUnknownPushMsgEntry.SetContentFileNameL(filename));
		}

	TPtrC8 msgBodyPtr;
	iMessage->GetMessageBody(msgBodyPtr);
	aUnknownPushMsgEntry.SetMessageDataL(msgBodyPtr);

	TTime pushMsgDate;
	TPtrC8 pushMsgDateDes;
	if (iMessage->GetHeaderField(EHttpDate, pushMsgDateDes))
		{
		pushMsgDate.UniversalTime();
		//Modify pushMsgDateDes descriptor to Home Time
		aUnknownPushMsgEntry.SetTimeSent(pushMsgDate);
		}
	}


/**
 * Gets content filename if it exists in the Content-Disposition push message header.
 * @param aFilename Returns filename if it exists, otherwise sets to KNullDesC.
 *                  The aFilename may have invalid filename characters or path information.
 * @return ETrue if the filename exists, EFalse otherwise.
 */
TBool CUnknownContentHandler::GetContentFileNameL(TFileName& aFilename)
	{
	// Look for a Content-Disposition header...
	TPtrC8 fieldValue;
	if (!iMessage->GetBinaryHeaderField(EHttpContentDisposition, fieldValue))
		{
		aFilename.Copy(KNullDesC); 
		return EFalse;
		}
	
	return DecodeWAPContentDispositionHeaderL(aFilename, fieldValue);
	}


/**
 *	@internalComponent
 *	@prototype
 */
TBool CUnknownContentHandler::DecodeWAPContentDispositionHeaderL(TFileName& aFilename, const TDesC8& aFieldValue)
	{
	// Decode the WAP 1.1 encoded Content-disposition header field.
	// Refer to WAP-230-WAP-20010705-a 8.4.2.53 Content-Disposition field.
	// The BNF is quoted here for reference:
	//
	// Content-disposition-value = Value-length Disposition *(Parameter)
	// Disposition = Form-data | Attachment | Inline | Token-text
	// Form-data = <Octet 128>
	// Attachment = <Octet 129>
	// Inline = <Octet 130>
	
	TInt valueLength = aFieldValue.Length();
	
	// Decode Disposition...
	TWspPrimitiveDecoder decoder(aFieldValue);
	TInt bytesRead = 0;
	TInt result = 0;
	switch(decoder.VarType())
		{
		case TWspPrimitiveDecoder::E7BitVal:
			{
			TUint8 disposition;
			result = decoder.Val7Bit(disposition);
			}
			break;
		case TWspPrimitiveDecoder::EString:		
			{
			TPtrC8 disposition;
			result = decoder.String(disposition);
			}
			break;
		default:
			{
			result = -1;
			}
		}

	if (result <= 0)
		{
		// Not encoded to specification.
		aFilename.Copy(KNullDesC); 
		return EFalse;
		}	

	bytesRead = result;
	
	// Check for any parameters...
	if (bytesRead >= valueLength)
		{
		// There are no parameters to follow.  The filename parameter value
		// does not exist in the encoded Content-Disposition header.
		aFilename.Copy(KNullDesC); 
		return EFalse;	
		}
	
	// Decode parameters...
	TInt lastBytesRead = -1;
	while(bytesRead < valueLength)
		{
		if (DecodeWAPContentDispositionParameterL(decoder, aFilename, bytesRead))
			{
			// The filename parameter has been located and decoded. 
			return ETrue;
			}

		if (bytesRead == lastBytesRead)
			{
			break;
			}

		lastBytesRead = bytesRead;  // To prevent an infinite loop if the length is corrupt.
		}
	
	// There is no filename parameter. 
	aFilename.Copy(KNullDesC); 
	return EFalse;
	};


/**
 *	@internalComponent
 *	@prototype
 */
TBool CUnknownContentHandler::DecodeWAPContentDispositionParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead)
	{
	// Decode the WAP 1.1 encoded Parameter field.
	// Refer to WAP-230-WAP-20010705-a 8.4.2.4 Parameter.
	// The BNF is quoted here for reference:
	//
	// Parameter = Typed-parameter | Untyped-parameter
	
	// Determine whether the Parameter is a Typed-parameter or Untyped-parameter...
	switch(aDecoder.VarType())
		{
		case TWspPrimitiveDecoder::ELengthVal :
			return DecodeWAPContentDispositionTypedParameterL(aDecoder, aFilename, aBytesRead);
		case TWspPrimitiveDecoder::EString :
			return DecodeWAPContentDispositionUntypedParameterL(aDecoder, aFilename, aBytesRead);
		default :
			{
			// Not encoded to specification. 
			return EFalse;
			}
		}
	}


/**
 *	@internalComponent
 *	@prototype
 */
TBool CUnknownContentHandler::DecodeWAPContentDispositionTypedParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead)
	{
	// Decode the WAP 1.1 encoded Parameter field.
	// Refer to WAP-230-WAP-20010705-a 8.4.2.4 Parameter.
	// The BNF is quoted here for reference:
	//
	// Typed-parameter = Well-known-parameter-token Typed-value
	// ; the actual expected type of the value is implied by the well-known parameter

	__ASSERT_DEBUG((aDecoder.VarType() == TWspPrimitiveDecoder::ELengthVal), User::Panic(KAssertFailed, KErrArgument));
	
	// Read Well-known-parameter-token.
	//
	// Well-known-parameter-token = Integer-value
	// ; the code values used for parameters are specified in the Assigned Numbers appendix
	//
	TInt result = 0;
	TInt wellKnownParameterToken;
	result = aDecoder.LengthVal(wellKnownParameterToken);
		
	if (result <= 0 )
		{
		// Not encoded to specification. 
		return EFalse;
		}
	
	aBytesRead += result;
	
	// Read Typed-value.
	//
	// Typed-value = Compact-value | Text-value
	// ; In addition to the expected type, there may be no value.
	// ; If the value cannot be encoded using the expected type, it shall be encoded as text.
	//
	// Compact-value = Integer-value | Date-value | Delta-seconds-value | Q-value | Version-value | Uri-value
	//
	// Date-value = Long-integer
	// ; The encoding of dates shall be done in number of seconds from
	// ; 1970-01-01, 00:00:00 GMT.
	//
	// Delta-seconds-value = Integer-value
	//
	// 
	//
	// Text-value = No-value | Token-text | Quoted-string
	//
	// Text-string = [Quote] *TEXT End-of-string
	// ; If the first character in the TEXT is in the range of 128-255, a Quote character must precede it.
	// ; Otherwise the Quote character must be omitted. The Quote is not part of the contents.
	//
	// Token-text = Token End-of-string
	//
	// No-value = <Octet 0>
	// ; Used to indicate that the parameter actually has no value,
	// ; eg, as the parameter "bar" in ";foo=xxx; bar; baz=xyzzy".
	TPtrC8 typedValue;
	switch(aDecoder.VarType())
		{
		case TWspPrimitiveDecoder::ELengthVal :
			{
			// Compact-value.
			TUint32 value;
			result = aDecoder.Integer(value);
			}
			break;
		case TWspPrimitiveDecoder::E7BitVal :
			{
			// Compact-value.
			TUint8 value;
			result = aDecoder.Val7Bit(value);
			}
			break;
		case TWspPrimitiveDecoder::EString :
		case TWspPrimitiveDecoder::EQuotedString :
			{
			// Text-value.
			result = aDecoder.String(typedValue);
			}
			break;
		default:
			{
			result = -1;
			}
		}
	
	if (result <= 0 )
		{
		// Not encoded to specification. 
		return EFalse;
		}
	
	aBytesRead += result;
	
	// Check to see if we have a filename parameter...
	if (wellKnownParameterToken != 0x06)
		{
		// The filename parameter has not been found.
		return EFalse;
		}
	
	// The filename parameter has been found.  Check its length.
	if (typedValue.Length() > KMaxFileName)
	{
		return EFalse;	
	}
	
	aFilename.Copy(typedValue);
	return ETrue;
	}


/**
 *	@internalComponent
 *	@prototype
 */
TBool CUnknownContentHandler::DecodeWAPContentDispositionUntypedParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead)
	{
	// Decode the WAP 1.1 encoded Untyped-parameter value.
	// Refer to WAP-230-WAP-20010705-a 8.4.2.4 Parameter.
	// The BNF is quoted here for reference:
	//
	// Untyped-parameter = Token-text Untyped-value
	// ; the type of the value is unknown, but it shall be encoded as an integer, if that is possible.
	
	__ASSERT_DEBUG(aDecoder.VarType() == TWspPrimitiveDecoder::EString, User::Panic(KAssertFailed, KErrArgument));
	
	// Read token-text.
	// Token-text = Token End-of-string
	TPtrC8 tokenText;
	TInt result = aDecoder.String(tokenText);
	if (result <= 0 )
		{
		// Not encoded to specification. 
		return EFalse;
		}
	
	aBytesRead += result;
	
	// Read Untyped-value to get to next the parameter.
	//
	// Untyped-value = Integer-value | Text-value
	//
	// Integer-Value = Short-integer | Long-integer
	//
	// Short-integer = OCTET
	// ; Integers in range 0-127 shall be encoded as a one octet value with the most significant bit set
	// ; to one (1xxx xxxx) and with the value in the remaining least significant bits.
	//
	// Long-integer = Short-length Multi-octet-integer
	// ; The Short-length indicates the length of the Multi-octet-integer
	//
	// Short-length = <Any octet 0-30>
	//
	// Text-value = No-value | Token-text | Quoted-string
	//
	// Text-string = [Quote] *TEXT End-of-string
	// ; If the first character in the TEXT is in the range of 128-255, a Quote character must precede it.
	// ; Otherwise the Quote character must be omitted. The Quote is not part of the contents.
	//
	// Token-text = Token End-of-string
	//
	// No-value = <Octet 0>
	// ; Used to indicate that the parameter actually has no value,
	// ; eg, as the parameter "bar" in ";foo=xxx; bar; baz=xyzzy".

	// Determine ifUntyped value is an Integer-value or Text-value...
	TPtrC8 untypedValue;
	switch( aDecoder.VarType() )
		{
		case TWspPrimitiveDecoder::E7BitVal :
			{
			// Short-integer.
			TUint8 value = 0;
			result = aDecoder.Val7Bit(value);
			}
			break;
		case TWspPrimitiveDecoder::ELengthVal :
			{
			// Long-integer or No-value.
			TUint32 value = 0;
			result = aDecoder.Integer(value);
			}
			break;
		case TWspPrimitiveDecoder::EString :
		case TWspPrimitiveDecoder::EQuotedString :
			{
			// Text-value.
			result = aDecoder.String(untypedValue);
			}
			break;
		default :
			{
			result = -1;
			}
		}
	
	if (result <=0 )
		{
		// Not encoded to specification. 
		return EFalse;
		}
	
	aBytesRead += result;
	
	// Check to see if we have a filename parameter...
	if (tokenText.CompareF(KFilename8) != 0)
		{
		// The filename parameter has not been found.
		return EFalse;
		}
	
	// The filename parameter has been found.  Check its length.
	if (untypedValue.Length() > KMaxFileName)
	{
		return EFalse;	
	}
	
	aFilename.Copy(untypedValue);
	return ETrue;
	}


/**
 * Same functionality as DoCancel()
 */
void CUnknownContentHandler::CancelHandleMessage()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: CancelHandleMessage Called"); 
	Complete(KErrCancel);
	}


/**
 *	Cancels the handling of the message and revokes the active status
 *	of the handler 
 */
void CUnknownContentHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: DoCancel Called"); 
	Complete(KErrCancel);
	}


/**
 *  Handler States: 
 *	 EProcessing- convert CPushMessage to a format to save
 *					and save the message 
 *					(either as Bio msg or UnknownPushMsgEntry)
 *	 EDone		- Clean up
 */
void CUnknownContentHandler::RunL()
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: RunL Called");
	// use active state machine routine to manage activites:
	switch(iState)
		{
	case EProcessing:
		ProcessingPushMsgEntryL();
		break;
	case EDone:
		Complete(KErrNone);
		break;
	default:
		break;
		}
	}


/**
 *	This is invoked when RunL Leaves with an error.
 *	Cleans up and returns.
 *	@param aError Error passed into this function
 */
TInt CUnknownContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CUnknownContentHandler:: RunError Called"); 
	iState = EDone;
	Complete(aError);
	return KErrNone;
	}
