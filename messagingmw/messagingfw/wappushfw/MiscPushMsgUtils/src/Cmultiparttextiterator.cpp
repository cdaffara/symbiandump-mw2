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
  
#include "cmultiparttextiterator.h"

EXPORT_C  CMultipartTextIterator* CMultipartTextIterator::NewL(CPushMessage& aPushMessage)
/** Allocates and creates a new MIME multipart message iterator.

@param aPushMessage MIME multipart message to process
@return New MIME multipart message iterator */
	{
	CMultipartTextIterator* self =  new (ELeave) CMultipartTextIterator(aPushMessage);
	CleanupStack::PushL(self);
	self->ConstructL(); 
	CleanupStack::Pop();
	return self;
	}


/**
 * CMultipartTextMessage
 * @param aPushMessage A message determined to be text/multipart. 
 */
CMultipartTextIterator::CMultipartTextIterator(CPushMessage& aPushMessage) : 
	CMultipartIteratorBase(aPushMessage)
	{
	}

/**
 * Fully instantiates this class.  
 * Finds the boundary and sets the internal pointer to the first part
 */
void CMultipartTextIterator::ConstructL()  
	{
	BoundaryParamL();
	FirstL();
	}

EXPORT_C CMultipartTextIterator::~CMultipartTextIterator()
/** Destructor. */
	{
	delete iBoundary;
	}



/**
 * Finds the boundary from the WAP Binary Header.  
 * The actually boundary conforms to RFC 2061.  
 * @leave KErrCorrupt if the message does not contain a boundary
 * @leave HBufC8::NewL
*/
void CMultipartTextIterator::BoundaryParamL()
	{
	TPtrC contentType;
	iPushMessage.GetContentType(contentType);
	__ASSERT_DEBUG( contentType.FindF(KMultipartText) == 0 , User::Panic(KPushInvalidContentType,0));

	const TInt KMaxBoundaryLen(74);	// Max Boundary = 70 + preceeding "--" and at the end "--"
	TBuf8<KMaxBoundaryLen> cleanCtHdrValue;
	TPtrC8 ctHdrValue;
	TPtrC8 bndLabel(KBoundaryParam);
	iPushMessage.GetBinaryHeaderField(EHttpContentType, ctHdrValue);


	TInt paramOffset = ctHdrValue.FindF(KBoundaryParam);
	if (paramOffset != KErrNotFound)
		{
		ctHdrValue.Set(ctHdrValue.Mid(paramOffset + bndLabel.Length()));

		// remove the NULL terminators
		cleanCtHdrValue.Copy(ctHdrValue);
		TInt nullTerm = cleanCtHdrValue.Locate(0);
		for ( ; nullTerm != KErrNotFound; nullTerm = cleanCtHdrValue.Locate(0))
			cleanCtHdrValue.Delete(nullTerm,1);

		// remove start & end quotes
		if (cleanCtHdrValue[0] == '\"')
			{
			cleanCtHdrValue.Delete(0,1);
			TInt endIndex = cleanCtHdrValue.Length() - 1;
			if (cleanCtHdrValue[endIndex] == '\"')
				cleanCtHdrValue.Delete(endIndex,1);
			else
				User::Leave(KErrCorrupt);
			}
		}
	else
		User::Leave(KErrCorrupt);

	iBoundary = HBufC8::NewL(sizeof(KBoundaryStartEnd) - 1 + ctHdrValue.Length());
	iBoundary->Des().Copy(KBoundaryStartEnd);
	iBoundary->Des().Append(cleanCtHdrValue);	
	}


EXPORT_C TBool CMultipartTextIterator::NextL()
/** Moves the iterator to the next message part.

@return ETrue if there is a next part, EFalse otherwise */
	{
	TPtrC8 hdrPtr;
	TInt offset = 0;
	
	hdrPtr.Set(iIndexPtr);

	// find the boudary which starts our next part
	offset = hdrPtr.Find(*iBoundary);
	
	if (offset == KErrNotFound )
			return EFalse;

	// point to the end of the boudary string
	hdrPtr.Set(hdrPtr.Mid(offset + iBoundary->Length()));

	// did we encounter the last "--"
	if (hdrPtr.Find(KBoundaryStartEnd) == 0)
			return EFalse;

	// see if this is really the boundary.  Expecting to see \r\n, no spurious text following it
	if (hdrPtr.Find(KCRLF) != 0)
			return EFalse;
	
	// point to the beginning of the new part 
	TPtrC8 ptrCRLF(KCRLF);
	hdrPtr.Set(hdrPtr.Mid(ptrCRLF.Length()));
	iIndexPtr.Set(hdrPtr);

	return ETrue;
	}


EXPORT_C void CMultipartTextIterator::FirstL()
/** Resets the iterator the beginning of the first part of the message. */
	{
	// Set iIndexPtr to the start of the body
	iPushMessage.GetMessageBody(iIndexPtr);

	// need to call this to get past the first boundary
	NextL();
	}


EXPORT_C CPushMessage*	CMultipartTextIterator::PartL()
/** Gets the message part that the iterator is currently referencing.

@return Message part */
	{
	// iIndexPtr should be pointing to line after the boudary

	// check if there are boudary headers
	TPtrC8 hdrSep(KHdrSeparator);
	TPtrC8 hdrStart(iIndexPtr);

	TInt offset = hdrStart.Find(KHdrSeparator);
	if (offset == KErrNotFound)
		User::Leave(KErrNotFound);
	
	HBufC8* headerBuf = hdrStart.Mid(0, offset).AllocLC();

	// now find the body
	TPtrC8 bodyStart;
	bodyStart.Set(hdrStart.Mid(offset + hdrSep.Length()));

	offset = bodyStart.Find(*iBoundary);
	if (offset == KErrNotFound)
		User::Leave(KErrNotFound);

	HBufC8* bodyBuf = bodyStart.Mid(0, offset).AllocLC();
	
	CPushMessage* pm = CPushMessage::NewL( headerBuf,  bodyBuf);
	CleanupStack::Pop(2, headerBuf);  // bodyBuf, headerBuf
	return pm;
	}

