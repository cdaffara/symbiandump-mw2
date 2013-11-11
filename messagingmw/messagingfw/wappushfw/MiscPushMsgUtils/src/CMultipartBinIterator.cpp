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
    
#include "CMultipartBinIterator.h"


EXPORT_C  CMultipartBinIterator* CMultipartBinIterator::NewL(CPushMessage& aPushMessage)
/** Allocates and creates a new binary multipart message iterator.

@param aPushMessage Binary multipart message to process
@return New binary multipart message iterator */
	{
	CMultipartBinIterator* self =  new (ELeave) CMultipartBinIterator(aPushMessage);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop();
	return self;
	}


/**
 * CMultipartPushMessage is a base utility class to crack out parts of a 
 * multipart message.
 * @param aPushMessage: A message determined to be multipart. 
 */
CMultipartBinIterator::CMultipartBinIterator(CPushMessage& aPushMessage) : 
	CMultipartIteratorBase(aPushMessage)
	{
	}


/**
 * Validates & completes the construction of this class.
 */
void CMultipartBinIterator::ConstructL() 
	{
#ifdef _DEBUG
	_LIT(KMultipartBin, "application/vnd.wap.multipart.");
	TPtrC contentType;
	iPushMessage.GetContentType(contentType);
	__ASSERT_DEBUG( contentType.Find(KMultipartBin) == 0 , User::Panic(KPushInvalidContentType,0));
#endif
	
	TPtrC8 msgBody;
	if (!iPushMessage.GetMessageBody(msgBody))
		User::Leave(KErrNotFound);
	
	// get the nEntries field from the start of the binary multipart
	TWapBinCodex::ExtractUIntvarL(msgBody, 0, iMultiNumEntries);
		
	if (iMultiNumEntries.iValue == 0) // No multiparts
		User::Leave(KErrCorrupt);

		
	// move the iterator to start off after the multipart header
	iCurrentPartStart = iMultiNumEntries.iOctetSize;		
	}


EXPORT_C TBool CMultipartBinIterator::NextL()
/** Moves the iterator to the next message part.

@return ETrue if there is a next part, EFalse otherwise */
	{
	if (++iCurrentPart >= iMultiNumEntries.iValue)
		return EFalse;

	TWapBinCodex::TUintvar hdrSize, bodySize;
		
	TPtrC8 msgBody;
	if (!iPushMessage.GetMessageBody(msgBody))
		return EFalse;

	// Get the header size
	TWapBinCodex::ExtractUIntvarL(msgBody, iCurrentPartStart,  hdrSize);

	// move index over the header size data
	iCurrentPartStart += hdrSize.iOctetSize;

	// Get the body size
	TWapBinCodex::ExtractUIntvarL(msgBody, iCurrentPartStart, bodySize);

	// move index over the body size data
	iCurrentPartStart += bodySize.iOctetSize;

	// move index over the part data
	iCurrentPartStart += (hdrSize.iValue + bodySize.iValue);

	return ETrue;
	}

EXPORT_C void CMultipartBinIterator::FirstL()
/** Resets the iterator the beginning of the first part of the message. */
	{
	iCurrentPartStart = iMultiNumEntries.iOctetSize;	
	iCurrentPart = 0;
	}

EXPORT_C CPushMessage* CMultipartBinIterator::PartL()
/** Gets the message part currently referenced by the iterator.

@return Message part 
@leave KErrCorrupt The message is too short
@leave KErrNotFound Will leave if message body is empty
@leave TDesC8::AllocLC
@leave CPushMessage::NewL
*/
	{
	__ASSERT_DEBUG( iCurrentPart < iMultiNumEntries.iValue, User::Panic(KPushPartIndexOutOfRange,0));

	TPtrC8 msgBody;
	if (!iPushMessage.GetMessageBody(msgBody))
		User::Leave(KErrNotFound);

	// iCurrentPartStart is pointing to beginning of the message 
	// index will be used to get past the message headers
	TUint index = iCurrentPartStart;

	// Move over the headers
	TWapBinCodex::TUintvar hdrSize, bodySize;
	TWapBinCodex::ExtractUIntvarL(msgBody, index,  hdrSize);
	index += hdrSize.iOctetSize;
	TWapBinCodex::ExtractUIntvarL(msgBody, index, bodySize);
	index += bodySize.iOctetSize;

	// Now get the data
	TUint len = msgBody.Length();

	// validate that we have as much data as HEADER indicates
	if ( hdrSize.iValue < 0 || // No negative indices
		(len < (index + hdrSize.iValue)))	
		User::Leave(KErrCorrupt);

	// Get the Header
	HBufC8* header = msgBody.Mid(index, hdrSize.iValue).AllocLC();
	index += hdrSize.iValue;
		
	// validate that we have as much data as BODY indicates 
	if ( bodySize.iValue < 0 || // No negative indices
		(len < (index + bodySize.iValue)))	
		User::Leave(KErrCorrupt);

	// Get the Body
	HBufC8* body = msgBody.Mid(index, bodySize.iValue).AllocLC();
		
	CPushMessage* pm = CPushMessage::NewL( header,  body);
	CleanupStack::Pop(2, header);  // body, header
	return pm;
	}

