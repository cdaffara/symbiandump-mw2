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
// CMsvBodyText.cpp
//

#include "MSVSTORE.H"     // CMsvStore
#include "CMsvBodyText.h"
#include <charconv.h>     // CCnvCharacterSetConverter
#include <txtrich.h>      // CRichText
#include <s32mem.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

/**
The unique stream identifier of the character set data stored in the message store.
*/
const TUid KMsvCharData = {0x101FD0E1};


/**
The version of KMsvEntryCharStream.  Used to maintain data compatibility with older versions.
*/
const TUint8 KMsvCharDataVersion = 1;


/**
The version of KMsvEntry8BitASCIIStream.  Used to maintain data compatibility with older versions.
*/
const TUint8 KMsv8BitEncodedBodyDataVersion = 1;




EXPORT_C CMsvBodyText* CMsvBodyText::NewL()
/**
Allocates and constructs an empty body text object.
*/
	{
	return new (ELeave) CMsvBodyText();
	}


EXPORT_C CMsvBodyText* CMsvBodyText::NewLC()
/**
Allocates and constructs an empty body text object.  The object is put on the cleanup stack.
*/
	{
	CMsvBodyText* self = new (ELeave) CMsvBodyText();
	CleanupStack::PushL(self);
	return self;
	}


EXPORT_C CMsvBodyText::~CMsvBodyText()
/**
Deallocates and destroys the body text object.
*/
	{
	}


EXPORT_C void CMsvBodyText::SetCharacterSet(const TUint aCharacterSetIdentifier)
/**
Sets the character set unique id to be used when decoding the 8 bit data stream.
*/
	{
	iCharSet = aCharacterSetIdentifier;
	}


EXPORT_C TUint CMsvBodyText::CharacterSet() const
/**
Retrieves the character set unique id that will be used when decoding the 8 bit
data stream. 
*/
	{
	return iCharSet;
	}


EXPORT_C void CMsvBodyText::SetDefaultCharacterSet(const TUint aCharacterSetIdentifier)
/**
Sets the default character set to use when decoding the 8 bit data if
the character set unique id has not been specified.
*/
	{
	iDefaultCharSet = aCharacterSetIdentifier;
	}


EXPORT_C TUint CMsvBodyText::DefaultCharacterSet() const
/**
Retrieves the default character set unique id that will be used when decoding the 8 bit
data stream if the character set unique id has not been specified. 
*/
	{
	return iDefaultCharSet;
	}


EXPORT_C void CMsvBodyText::RestoreL(CMsvStore& aStore)
/**
Retrieves the character set unique ids and 8 bit data stream from the message store.

@param aStore
A store in read-only or read-write (edit) mode.

@leave KErrNotFound
The store does not contain 8 bit encoded body text data. 
*/
	{
	RMsvReadStream in;
	in.OpenLC(aStore, KMsvCharData);
	in.ReadUint8L();  // Ignore the version number.  Future versions will have to check this value for data compatibility.
	iCharSet = in.ReadUint32L();
	iDefaultCharSet = in.ReadUint32L();
	CleanupStack::PopAndDestroy(&in);
	}


EXPORT_C void CMsvBodyText::StoreL(CMsvStore& aStore)
/**
Just adds the character set identifiers to the store cache.

@pre
The store must be in read-write (edit) mode.

@param aStore
The store must be open in read-write (edit) mode.

@leave KErrAccessedDenied
The message store is not in read-write (edit) mode.  
*/
	{
	RMsvWriteStream out;
	out.AssignLC(aStore, KMsvCharData);
	out.WriteUint8L(KMsvCharDataVersion);
	out.WriteUint32L(iCharSet);
	out.WriteUint32L(iDefaultCharSet);
	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	}


EXPORT_C void CMsvBodyText::StoreL(CMsvStore& aStore, const CBufBase& aData)
/**
Adds the character identifiers and 8 bit encoded body text data to the store cache.

@pre
The store must be in read-write (edit) mode.

@param aStore
The store must be open in read-write (edit) mode.

@param aData
8 bit encoded body text data to be added to the store.

@leave KErrAccessedDenied
The message store is not in read-write (edit) mode.  
*/
	{
	// Add the character set information.
	StoreL(aStore);
	
	// Add the 8 bit data.	
	RMsvWriteStream out;
	out.AssignLC(aStore, KMsv8BitEncodedBodyData);
	out.WriteUint8L(KMsv8BitEncodedBodyDataVersion);

	RBufReadStream reader(aData);
	out.WriteL(reader,aData.Size());

	out.CommitL();
	CleanupStack::PopAndDestroy(&out);
	}


EXPORT_C void CMsvBodyText::GetBodyTextL(RFs& aFs, CMsvStore& aStore, CRichText& aBodyText)
/**
Decodes the encoded 8 bit body text data into the correct character set and adds the decoded output
to the richtext object owned by the client. If the character set is not specified, then
the default character set is used.  If the character set is specified but not installed on the
system then the default character set is used instead.

@pre
The store must contain encoded 8 bit body text data.
 
@param aFs
A connected file system handle.  It is used to search for installed character set decoders.

@param aStore
The store can be opened in read-only or read-write (edit) mode.

@param aBodyText
The rich text object that will be appended with the decoded text.

@leave KErrNotSupported
The character set used to decode the 8 bit data cannot be found on the system.

@leave KErrNotFound
The store does not contain encoded 8 bit body text data.
*/
	{
	// Create and initialise decoder.  Leave if the character set cannot be located.
	CCnvCharacterSetConverter* converter = CCnvCharacterSetConverter::NewL();
	CleanupStack::PushL(converter);
	CCnvCharacterSetConverter::TAvailability available = CCnvCharacterSetConverter::ENotAvailable;
	if (iCharSet != 0)
		available = converter->PrepareToConvertToOrFromL(iCharSet, aFs);
	
	if (available == CCnvCharacterSetConverter::ENotAvailable)
		available = converter->PrepareToConvertToOrFromL(iDefaultCharSet, aFs);
	
	if (available != CCnvCharacterSetConverter::EAvailable)
		User::Leave(KErrNotSupported);
	
	// Open the 8 bit data stream and calculate the number of bytes of data to be read.
	RMsvReadStream in;
	in.OpenLC(aStore, KMsv8BitEncodedBodyData);
	in.ReadUint8L();  // Ignore the version number since this is the 1st version.
	MStreamBuf* sourceStream = in.Source();
	TInt bytesRemaining = sourceStream->SizeL() - 1; // Less one to account for the TUint8 version byte.
	
	// Create temporary buffers to hold the 8 bit data chunk and decoded chunk of unicode characters.
	// 2 kB unicode output buffer + 1 kB 8 bit input data.
	HBufC8* inBuf = HBufC8::NewLC(KMsvDecodeChunkLength);
	HBufC16* outBuf = HBufC16::NewLC(KMsvDecodeChunkLength);
	TPtr8 inPtr = inBuf->Des();
	TPtr16 outPtr = outBuf->Des();
	
	TInt state = CCnvCharacterSetConverter::KStateDefault;  // Used to preserve state across multiple calls to ConvertToUnicode.
	TBool newLine = EFalse;
	TBool newPara = EFalse;
	
	// Decode chunk by chunk to limit memory consumption.
	while(bytesRemaining > 0)
		{
		// Set the buffer length to the smaller of the number of bytes remaining,
		// or the chunk length.
		inPtr.SetLength(bytesRemaining < KMsvDecodeChunkLength ? bytesRemaining : KMsvDecodeChunkLength);
		
		// Read data from stream up to and including the 1st LF encountered.
		TRAPD(err, in.ReadL(inPtr, TChar(0x0A)));
		if (err != KErrEof)
			User::LeaveIfError(err);
		TInt len = inPtr.Length();
		__ASSERT_DEBUG(len > 0, User::Invariant());
		bytesRemaining -= len;
		
		// If the data read is terminated with an LF or CRLF, remove it and mark it for
		// appending a rich text line end or paragraph delimiter. 
		TInt bytesTruncated = 0;
		if (inPtr[len - 1] != 0x0A)
			{
			// The data read does not contain any new line or paragraph delimters.
			newLine = EFalse;
			newPara = EFalse;
			}
		else
			{
			// Data read is terminated with LF.  Truncate length of data to ignore LF or CRLF bytes.
			if (len > 1 && inPtr[len - 2] == 0x0D)
				{
				bytesTruncated = 2;
				len -= bytesTruncated;
				inPtr.SetLength(len);  // Remove CRLF.
				}
			else
				{
				bytesTruncated = 1;
				len -= bytesTruncated;
				inPtr.SetLength(len);  // Remove LF.
				}
			
			// Work out if it should be a new line or new paragraph.
			if (len == 0)
				{
				if (newLine)
					{
					// There are more than two new line delimiters in a row with no data
					// between them - new paragraph.
					newLine = EFalse;
					newPara = ETrue;
					}
				else
					{
					newLine = ETrue;
					newPara = EFalse;
					}
				}
			else
				{
				// It's a new line.
				newLine = ETrue;
				newPara = EFalse;
				}
			}
		
		// Convert the 8 bit data to unicode.
		TInt bytesUnconverted = converter->ConvertToUnicode(outPtr, inPtr, state);
		if (bytesUnconverted > 0)
			{
			// Some bytes could not be converted because the output buffer was too small.
			// Seek back to the position of the 1st unconverted byte and recalculate the bytes remaining.  
			TInt bytesSeekBack = bytesUnconverted + bytesTruncated;
			sourceStream->SeekL(MStreamBuf::ERead, EStreamMark, -bytesSeekBack);
			bytesRemaining += bytesSeekBack;
			newLine = EFalse;
			newPara = EFalse;
			}
		
		// There is no AppendL method for CRichText.  Insert decoded text at end of document instead.
		aBodyText.InsertL(aBodyText.DocumentLength(), outPtr);

		// Insert a new line or paragraph delimiter if necessary.
		if (newLine)
			aBodyText.InsertL(aBodyText.DocumentLength(), CEditableText::ELineBreak);
		if (newPara)
			aBodyText.InsertL(aBodyText.DocumentLength(), CEditableText::EParagraphDelimiter);
		}
	
	CleanupStack::PopAndDestroy(4, converter); // outBuf, inBuf, in, converter
	}
	
EXPORT_C void CMsvBodyText::GetBodyTextL(CMsvStore& aStore, TDes8& aBufer)
	{
	// Open the 8 bit data stream and calculate the number of bytes of data to be read.	
	if(aStore.IsPresentL(KMsv8BitEncodedBodyData))
		{
		RMsvReadStream in;
		in.OpenLC(aStore, KMsv8BitEncodedBodyData);
		in.ReadUint8L();  // Ignore the version number since this is the 1st version.
		MStreamBuf* sourceStream = in.Source();
		TInt bytesRemaining = sourceStream->SizeL() - 1; // Less one to account for the TUint8 version byte.	
		in.ReadL(aBufer, bytesRemaining);
		CleanupStack::PopAndDestroy(); // in		
		}
	else if(aStore.IsPresentL(KMsvPlainBodyText8))
		{
		RFileReadStream inputStream;
		aStore.Restore8BitBodyTextL(inputStream);
		inputStream.PushL();
		MStreamBuf* sourceStream = inputStream.Source();
		inputStream.ReadL(aBufer, sourceStream->SizeL());
		CleanupStack::PopAndDestroy();	// inputStream
		}	
	else
		{
		User::Leave(KErrNotSupported);
		}
	}	
	
EXPORT_C TInt CMsvBodyText::GetBodyLengthL(CMsvStore& aStore)
	{
	if(aStore.IsPresentL(KMsv8BitEncodedBodyData))
		{
		RMsvReadStream in;
		in.OpenLC(aStore, KMsv8BitEncodedBodyData);
		in.ReadUint8L();  // Ignore the version number since this is the 1st version.
		MStreamBuf* sourceStream = in.Source();
		TInt length = sourceStream->SizeL() - 1; // Less one to account for the TUint8 version byte.
		CleanupStack::PopAndDestroy(); // in
		return length;		
		}
	else if(aStore.IsPresentL(KMsvPlainBodyText8))
		{
		RFileReadStream inputStream;
		aStore.Restore8BitBodyTextL(inputStream);
		inputStream.PushL();
		MStreamBuf* sourceStream = inputStream.Source();
		TInt bodyLength = sourceStream->SizeL();
		CleanupStack::PopAndDestroy();	// inputStream
		return bodyLength;
		}
	else
		{
		return KErrNotSupported;
		}		
	}
	
EXPORT_C void CMsvBodyText::RemoveL(CMsvStore& aStore)
/**
Removes the 8 bit data body text data from the store cache.

@pre
The store must be in read-write (edit) mode.

@leave KErrAccessedDenied
The message store is not in read-write (edit) mode.  
*/
	{
	if (aStore.IsPresentL(KMsvCharData))
		aStore.RemoveL(KMsvCharData);
	if (aStore.IsPresentL(KMsv8BitEncodedBodyData))
		aStore.RemoveL(KMsv8BitEncodedBodyData);
	}


EXPORT_C TBool CMsvBodyText::IsPresentL(const CMsvStore& aStore) const
/**
Tests to see if the store contains 8 bit body text data.

@param aStore
The store can be open in read-only or read-write (edit) mode.

@return
ETrue if the store contains 8 bit body text data.
*/
	{
	return aStore.IsPresentL(KMsvCharData);
	}


CMsvBodyText::CMsvBodyText()
/**
Private constructor.
*/
	{
	}
