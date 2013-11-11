// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <txtrich.h>
#include "MSVFTEXT.H"
#include "MSVPANIC.H"

//**********************************
// CMsvFindText
//**********************************

EXPORT_C CMsvFindText* CMsvFindText::NewLC(TInt aPriority)
/** Creates a new find text utility object with the specified priority and puts 
a pointer to the new object onto the cleanup stack.

The function leaves if the object cannot be created.

@param aPriority The priority of this active object. 
@return Pointer to the new find text utility object */
	{
	CMsvFindText* self = new(ELeave)CMsvFindText(aPriority);
	CleanupStack::PushL(self);
	return self;
	}

EXPORT_C CMsvFindText* CMsvFindText::NewL(TInt aPriority)
/** Creates a new find text utility object with the specified priority.

The function leaves if the object cannot be created.

@param aPriority The priority of this active object. 
@return Pointer to the new find text utility object */
	{
	CMsvFindText* self = NewLC(aPriority);
	CleanupStack::Pop(); // self
	return self;
	}

CMsvFindText::CMsvFindText(TInt aPriority)
: CMsgActive(aPriority), iPreChar(' '), iPostChar(' ')
	{
	CActiveScheduler::Add(this);
	}

/** Destructor.

Any outstanding search is cancelled.
*/
EXPORT_C CMsvFindText::~CMsvFindText()
	{
	Cancel();

	delete iBuf;
	delete iFind;
	}

void CMsvFindText::Check()
	{
	TPtrC findPtr(iFind->Des());

	__ASSERT_ALWAYS(findPtr.Length() > 0, PanicServer(EMsvNoFindTextSpecified));
	__ASSERT_ALWAYS(findPtr.Length() <= KMsvMaxFindTextLength, PanicServer(EMsvTooMuchFindTextSpecified));
	__ASSERT_ALWAYS(!IsActive(), PanicServer(EMsvFindTextAlreadyActive));
	}

void CMsvFindText::DoRunL()
	{
	iFoundText = DoFindStepL();

	// Continue searching?
	if (!iFoundText && (iSourcePos < iSourceLen))
		{
		TRequestStatus* status = &iStatus;
		User::RequestComplete(status, KErrNone);
		SetActive();
		}
	}

void CMsvFindText::InitialiseL(const TDesC& aFind, const TDesC* aPlainSource, const CRichText* aRichSource, TMsvPartList aFlags)
	{
	iPlainText = aPlainSource;
	iRichText = aRichSource;

	// Get length of data
	iSourceLen = iPlainText ? iPlainText->Length() : iRichText->DocumentLength();

	iSourcePos = 0;
	iFoundText = EFalse;

	// Copy text to find
	delete iFind; iFind = NULL;
	iFind = HBufC::NewL(aFind.Length());
	*iFind = aFind;

	// Initialise members
	iMaxRead = KMsvMaxFindTextLength + aFind.Length() - 1;
	iFlags = aFlags;

	// Create buffer
	delete iBuf; iBuf = NULL;
	iBuf = HBufC::NewL(iMaxRead + 1);

	Check();

	// Convert text to find to lower case if not case sensitive
	if (!(iFlags & KMsvFindCaseSensitive))
		iFind->Des().LowerCase();
	}

EXPORT_C void CMsvFindText::FindTextL(const TDesC& aFind, const TDesC& aSource, TMsvPartList aFlags, TRequestStatus& aStatus)
/** Performs an asynchronous search for a text string within another text string.

@param aFind The text string to search for. 
@param aSource The plain text to be searched.
@param aFlags A set of flags which modify the way the search works: KMsvFindCaseSensitive 
means the search is successful only if there is an exact case match for the 
text. KMsvFindWholeWord means the search is successful only if the matching 
text in aSource is not delimited by alphanumeric characters.
@param aStatus The request status object, which is set when the search operation 
is complete. Use FoundText() to return the result of the search. */
	{
	InitialiseL(aFind, &aSource, NULL, aFlags);
	DoFindAsyncL(aStatus);
	}

EXPORT_C void CMsvFindText::FindRichTextL(const TDesC& aFind, const CRichText& aSource, TMsvPartList aFlags, TRequestStatus& aStatus)
/** Searches for a specified text string within the given rich text asynchronously.

@param aFind The text string to search for.
@param aSource The rich text to be searched.
@param aFlags A set of flags which modify the way the search works: KMsvFindCaseSensitive 
means the search is successful only if there is an exact case match for the 
text. KMsvFindWholeWord means the search is successful only if the matching 
text in aSource is not delimited by alphanumeric characters.
@param aStatus The request status object, which is set when the search operation 
is complete. Use FoundText() to return the result of the search. */
	{
	InitialiseL(aFind, NULL, &aSource, aFlags);
	DoFindAsyncL(aStatus);
	}

void CMsvFindText::DoFindAsyncL(TRequestStatus& aStatus)
	{
	Queue(aStatus);
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, KErrNone);
	SetActive();
	}

EXPORT_C TBool CMsvFindText::FindTextL(const TDesC& aFind, const TDesC& aSource, TMsvPartList aFlags)
/** Performs a synchronous search for a text string within another text string.

@param aFind The text string to search for. 
@param aSource The plain text to be searched.
@param aFlags A set of flags which modify the way the search works: KMsvFindCaseSensitive 
means the search is successful only if there is an exact case match for the 
text. KMsvFindWholeWord means the search is successful only if the matching 
text in aSource is not delimited by alphanumeric characters.
@return ETrue if there is a match, otherwise EFalse. */
	{
	InitialiseL(aFind, &aSource, NULL, aFlags);
	return DoFindSyncL();
	}

EXPORT_C TBool CMsvFindText::FindRichTextL(const TDesC& aFind, const CRichText& aSource, TMsvPartList aFlags)
/** Searches for a specified text string within the given rich text synchronously.

@param aFind The text string to search for. 
@param aSource The rich text to be searched.
@param aFlags A set of flags which modify the way the search works: KMsvFindCaseSensitive 
means the search is successful only if there is an exact case match for the 
text. KMsvFindWholeWord means the search is successful only if the matching 
text in aSource is not delimited by alphanumeric characters. 
@return ETrue if there is a match, otherwise EFalse. */
	{
	InitialiseL(aFind, NULL, &aSource, aFlags);
	return DoFindSyncL();
	}

TBool CMsvFindText::DoFindSyncL()
	{
	// Spin through source text
	while(!iFoundText && iSourcePos < iSourceLen)
		iFoundText = DoFindStepL();

	return iFoundText;
	}

TBool CMsvFindText::DoFindStepL()
	{
	// Get amount of data to fetch
	TInt toRead = iMaxRead + 1;
	if (iSourcePos + toRead > iSourceLen)
		toRead = iSourceLen - iSourcePos;
	
	TPtrC findPtr(iFind->Des());
	TPtr bufPtr(iBuf->Des());

	// Is there enough data for a match?
	if (iSourcePos + findPtr.Length() > iSourceLen)
		{
		iSourcePos += findPtr.Length();
		return EFalse;
		}

	if (iFlags & KMsvFindWholeWord)
		{
		// Get character before the buffer
		if (iSourcePos > 0)
			iPreChar = TCharF(bufPtr[KMsvMaxFindTextLength - 1]);
		else
			iPreChar = TChar(' ');
		}

	if (iPlainText)
		{
		// Read data
		bufPtr.Copy(iPlainText->Ptr() + iSourcePos, toRead);
		}
	else
		{
		// Assume data is rich text
		// Text is always appended so clear the buffer
		bufPtr.Zero();		
		
		// Read data, possibly across segment boundary
		while (bufPtr.Length() < toRead)
			bufPtr.Append(iRichText->Read(iSourcePos + bufPtr.Length(), toRead - bufPtr.Length()));
		}

	// Convert buffer to lower case if not case sensitive
	if (!(iFlags & KMsvFindCaseSensitive))
		bufPtr.LowerCase();

	if (iFlags & KMsvFindWholeWord)
		{
		// Get character after the buffer
		if (iSourcePos + toRead < iSourceLen)
			{
			iPostChar = TCharF(bufPtr[bufPtr.Length() - 1]);
			bufPtr.SetLength(bufPtr.Length() - 1);
			}
		else
			iPostChar = TChar(' ');
		}

	// Try and find text in this block of text
	if (DoFindTextL())
		return ETrue;

	iSourcePos += KMsvMaxFindTextLength;
	return EFalse;
	}

TBool CMsvFindText::DoFindTextL()
	{
	TPtrC findPtr(iFind->Des());
	TPtrC bufPtr(iBuf->Des());
	TInt pos = 0;

	// Find the text
	// If Wild card specified
	if(iFlags & KMsvFindUsingWildCard)
		{
		pos = bufPtr.MatchC(findPtr);
		}
	else
		{
		pos = bufPtr.Find(findPtr);
		}

	// Did we find the text
	if (pos < 0)
		return EFalse;

	if (iFlags & KMsvFindWholeWord)
		{
		// If we get here its because we've found the text but we need to check for whole word
		// The following returns false (no match) if the matching text is preceded or followed by an alphanumeric digit
		// The pre and post characters are the characters that come directly before and after the buffer we maintain

		if ((pos == 0 && iPreChar.IsAlphaDigit()) ||
			(pos > 0 && TCharF(bufPtr[pos - 1]).IsAlphaDigit()) ||
			(pos + findPtr.Length() < bufPtr.Length() && TCharF(bufPtr[pos + findPtr.Length()]).IsAlphaDigit()) ||
			(pos + findPtr.Length() == bufPtr.Length() && iPostChar.IsAlphaDigit()))
			return EFalse;
		}

	// Found the text
	return ETrue;
	}

void CMsvFindText::DoComplete(TInt&)
	{
	delete iBuf; iBuf = NULL;
	delete iFind; iFind = NULL;
	}
