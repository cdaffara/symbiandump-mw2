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
// MSVSTORE.CPP
//

#include <txtrich.h>


#include "MSVSTD.H"
#include "MSVSTORE.H"
#include "MSVPANIC.H"
#include "CMsvCachedStore.h"
#include "CMsvBodyText.h"
#include <cmsvplainbodytext.h>
#include "CMsvAttachmentManager.h"
#include <cmsvattachment.h>
#include <charconv.h>
#include <escapeutils.h>
#include <mmsvstoremanager.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif


//
// CMsvStore
//
const TUint8 KCarriageReturn = 0x0D;
const TUint8 KLineFeed       = 0x0A;

EXPORT_C CMsvStore::~CMsvStore()
//
//
//
/** Destructor.

If the message store is currently locked, the destructor releases it before 
deleting the object. */
	{
	// tell the observer that this store is being released/closed
	if (iConstructed)
		{
		// only inform server if the store was successfully opened
		if (iLockStatus==EMsvStoreLocked)
			iObserver.HandleStoreEvent(MMsvStoreObserver::EMsvEditStoreClosed, iId);
		else
			iObserver.HandleStoreEvent(MMsvStoreObserver::EMsvReadStoreClosed, iId);
		}

	delete iAttachmentManager;

	delete iStore;
	delete iBodyText;
	}

EXPORT_C void CMsvStore::StoreBodyTextL(const CRichText& aRichTextBody)
//
// stores and commits body text to the file store, overwriting any existing data
// will panic if the store is unlocked and will leave if no store is found
//
/** Stores the body text stream of the message store. Any existing 
data is overwritten. Write access is needed for this function. A commit must be done
following this function for the new data to be retained.

@param aRichTextBody The body data to be externalised 
@leave KErrAccessDenied Store is not open for writing 
@leave Other Standard streaming leave codes */
	{
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		User::Leave(KErrAccessDenied);

	iBodyText->RemoveL(*this);
	
	// Remove the body text data if stored in KMsvPlainBodyText8 or KMsvPlainBodyText16 format.
	if (IsPresentL(KMsvPlainBodyText8))
		{
		RemoveL(KMsvPlainBodyText8);
		}
	if (IsPresentL(KMsvPlainBodyText16))
		{
		RemoveL(KMsvPlainBodyText16);
		}
	
	RMsvWriteStream out;
	out.AssignL(*this,KMsvEntryRichTextBody);
	CEmbeddedStore* embeddedStore=CEmbeddedStore::NewLC(out);
	TStreamId streamId=aRichTextBody.StoreL(*embeddedStore);
	embeddedStore->SetRootL(streamId);
	embeddedStore->CommitL();
	CleanupStack::PopAndDestroy(); //embeddedstore
	}

void CMsvStore::Lock()
	{
	iLockStatus=EMsvStoreLocked;
	}

EXPORT_C TBool CMsvStore::IsNullL() const
/** Tests whether the message store contains any streams.
@return ETrue if the store has no streams, else EFalse
*/
	{
	return iStore->IsNullL();		
	}

EXPORT_C TBool CMsvStore::IsPresentL(TUid aUid) const
/** Tests whether the message store contains a stream identifier.
Do not use to check if there is a body text part stored with the email;
use CMsvStore::HasBodyTextL() instead.
@param aUid Identifier of stream 
@return ETrue if the store has a stream aUid, else EFalse 
 */
	{
	return iStore->IsPresentL(aUid);
	}
	

EXPORT_C TBool CMsvStore::HasBodyTextL() const
/** Checks if the store has a body text stream; IsPresentL(KMsvEntryRichTextBody)
will always return EFalse when the store_8bit_flag in imcm.rss is enabled.
@return ETrue if the store has a body text stream, else EFalse 
*/
	{
	return iStore->IsPresentL(KMsvEntryRichTextBody) || iBodyText->IsPresentL(*this)|| iStore->IsPresentL(KMsvPlainBodyText8)|| iStore->IsPresentL(KMsvPlainBodyText16);
	}

/**
Restores body text into the given rich text object - doesnt matter if store is locked or not
will leave if the file store doesn't exist, the body text does not exist, and also propagate store leaves
Reads the body text stream from the message store into the given rich text 
object. 
@param aRichTextBody 	On return, the message store's body text is inserted 
						into this object. Any previously existing contents is lost. 
@leave KErrNotFound 	There is no body text stream in the store 
@leave Other Standard streaming leave codes 
*/
EXPORT_C void CMsvStore::RestoreBodyTextL(CRichText& aRichTextBody)
	{
	// Charset set to 0 so that RestoreBodyTextL works as per existing published behaviour.
	RestoreBodyTextL(aRichTextBody, 0);
	}

/**
Restores body text into the given rich text object - doesnt matter if store is locked or not.
Reads the body text stream from the message store into the given rich text 
object.
@param aRichTextBody 		On return, the message store's body text is inserted 
							into this object. Any previously existing contents is lost. 
@param aCharsetOverride   	The charset with which to override the existing charset 
							when data is stored a 8 bit format. 
@leave KErrNotFound 		There is no body text stream in the store 
@leave Other Standard streaming leave codes 
*/
EXPORT_C void CMsvStore::RestoreBodyTextL(CRichText& aRichTextBody, TUint aCharsetOverride)
	{
	if (iStore==NULL )
		{
		User::Leave(KErrNotFound);
		}

	if (IsPresentL(KMsvEntryRichTextBody))
		{
		RMsvReadStream inputStream;
		inputStream.OpenL(*this, KMsvEntryRichTextBody); // CEmbeddedStore::FromLC() take ownership
		CEmbeddedStore* embeddedStore = CEmbeddedStore::FromLC(inputStream);
		aRichTextBody.RestoreL(*embeddedStore, embeddedStore->Root());
		CleanupStack::PopAndDestroy();	// embeddedStore
		}
	else if(iBodyText->IsPresentL(*this))
		{
		iBodyText->RestoreL(*this);
		// If a new charset was set, override the  restored charset
		// so that data is converted using new charset.
		if(aCharsetOverride != 0)
			{
			iBodyText->SetCharacterSet(aCharsetOverride);
			}
		iBodyText->GetBodyTextL(iFs, *this, aRichTextBody);
		}
	else
		{
		if(IsPresentL(KMsvPlainBodyText8) || IsPresentL(KMsvPlainBodyText16))
			{
			RestorePlainBodyTextL (aRichTextBody, aCharsetOverride);
			}
		else		
			{
			User::Leave(KErrNotFound);
			}
		}
	}




/**
Restores a CRichText object from the plain body text file.
@param aRichText 			On completion this contains the plain body text.
@param aCharsetOverride 	The new charset with which to override the existing charset 
							when data is stored a 8 bit plain text format. 
@return void.
*/
void CMsvStore::RestorePlainBodyTextL(CRichText& aRichText, TUint aCharsetOverride)
	{
	RMsvReadStream inputStream;
	TBool is8Bit = EFalse;
	TUint charSet = 0;
	TUint defaultCharSet = 0;
	if(IsPresentL(KMsvPlainBodyText8))
		{
		inputStream.OpenLC(*this, KMsvPlainBodyText8);
		charSet = inputStream.ReadUint32L();
		defaultCharSet = inputStream.ReadUint32L();
		is8Bit = ETrue;
		}
	else
		{
		inputStream.OpenLC(*this, KMsvPlainBodyText16);
		}
	// If a new charset was set, override the  restored charset so that 
	// data is converted using new charset.
	if(aCharsetOverride!=0)
		{
		charSet = aCharsetOverride;
		}
	
	TBool committed = inputStream.ReadUint32L();
	TFileName filePath;
	HBufC* fileName = HBufC::NewLC(inputStream, KMaxFileName);	
	RFile bodyTextFile;
	CleanupClosePushL(bodyTextFile);
	
	iStoreManager.BodyTextFilePathL(iId, filePath);

	filePath.Append(*fileName);
	iStoreManager.OpenBodyTextFileForReadL(bodyTextFile, iId, filePath);
	if(is8Bit)
		{
		GetRichTextFrom8BitL(bodyTextFile, aRichText, charSet, defaultCharSet);
		}
	else
		{
		GetRichTextFrom16BitL(bodyTextFile, aRichText);
		}

	CleanupStack::PopAndDestroy(3, &inputStream);//bodyTextFile,fileName,in
	}
/**	
Restores 8 bit body text from the Message Store
@param aInputStream		on completion the RFileReadStream contains the message body text stream.
*/
void CMsvStore::Restore8BitBodyTextL(RFileReadStream& aInputStream)
	{
	RMsvReadStream in;
	in.OpenLC(*this, KMsvPlainBodyText8);
	in.ReadUint32L();	// reads charset
	in.ReadUint32L();	// read default charset
	in.ReadUint32L();	// returns ETrue if committed to message store else EFalse
	TFileName filePath;
	HBufC* fileName = HBufC::NewLC(in, KMaxFileName);
	RFile bodyTextFile;
	CleanupClosePushL(bodyTextFile);
	iStoreManager.BodyTextFilePathL(iId, filePath);
	filePath.Append(*fileName);
	iStoreManager.OpenBodyTextFileForReadL(bodyTextFile, iId, filePath);
	aInputStream.Attach(bodyTextFile, 0);
	CleanupStack::PopAndDestroy(3, &in);	// bodyTextFile, fileName, in
	}	
	
/**
Gets CRichText from plain body text file if data stored as 8 bit.
@param aFile 			The file containing the plain body text.
@param aRichText 		The CRichText object that will be filled on return.
@param aCharSet 		The charset of the body part.
@param aDefaultCharSet  The default charset of the system.
@return void.
*/	
void CMsvStore::GetRichTextFrom8BitL(RFile& aBodyTextFile, CRichText& aRichText, TUint aCharSet, TUint aDefaultCharSet)
	{
	Convert8BitToRichTextL(aBodyTextFile, aRichText, aCharSet, aDefaultCharSet);
	}


/**
Gets CRichText from plain body text file if data stored as 16 bit.
@param aFile 	  The file containing the plain body text.
@param aRichText  The CRichText object that will be filled on return.
@return void.
*/
void CMsvStore::GetRichTextFrom16BitL(RFile& aBodyTextFile, CRichText& aRichText)
	{
	TInt size = 0;
	aBodyTextFile.Size(size);
	TBuf8<KMsvDecodeChunkLength> buf;
	TBool carriageReturn = EFalse;
	while(size > 0)
		{
		aBodyTextFile.Read(buf);
		// Divide by 2 since the 16-bit buffer was stored using 8-bit pointer.
		TPtrC16 bodyTextPtr((TUint16*)(buf.Ptr()), buf.Length()/2); 

		// Was the last character of the last block a carriage return.
		// If so we need to check for the specific condition where the carriage
		// return / line feed has been split across blocks in which case the line
		// feed should be discarded and the special character ELineBreak inserted
		// instead. If the first character of this new block is not line feed
		// then we should insert the carriage return as it is required in the
		// output data and has not been added yet.
		if (carriageReturn)
			{
			if (bodyTextPtr[0] == KLineFeed)
				{
				aRichText.InsertL(aRichText.DocumentLength(), CEditableText::ELineBreak);
				bodyTextPtr.Set(bodyTextPtr.Mid(1));
				}
			else
				{
				aRichText.InsertL(aRichText.DocumentLength(), TChar(KCarriageReturn));
				}

			carriageReturn = EFalse;
			}

		TInt start = 0;
		TInt len = bodyTextPtr.Length();
		TInt i = 0;
			
		// Insert the data read from the RFile to CRichText.If there is CRLF insert 
		// CEditableText::ELineBreak  
		for(i=1; i<len; ++i)
			{
			// This will get data from RFile line by line and insert it to CRichText.
			if(bodyTextPtr[i-1] == KCarriageReturn && bodyTextPtr[i] == KLineFeed)
				{
				TPtrC ptr = bodyTextPtr.Mid(start, (i-1)-start);
				aRichText.InsertL(aRichText.DocumentLength(), ptr);
				aRichText.InsertL(aRichText.DocumentLength(), CEditableText::ELineBreak);
				start = i+1;
				}
			}
		// We have inserted a line to the CRichText object, there were some more data left in
		// bodyTextPtr, so insert rest of it to CRichText.
		if (start != i)
			{
			// If last character is KCarriageReturn.
			if(bodyTextPtr[len - 1] == KCarriageReturn)
				{
				carriageReturn = ETrue;
				TPtrC ptr1 = bodyTextPtr.Mid(start, (i-1)-start);
		    	aRichText.InsertL(aRichText.DocumentLength(), ptr1);
				}
			else
				{
				TPtrC ptr2 = bodyTextPtr.Mid(start, i-start);
		    	aRichText.InsertL(aRichText.DocumentLength(), ptr2);
				}
			
			}
		size -= buf.Length();
		}

	// If the last character of the text was a carriage return, then we won't
	// have added it because we were waiting to check what the start of the next
	// block was. As there are no more blocks, the carriage return needs to be 
	// added now.
	if (carriageReturn)
		{
		aRichText.InsertL(aRichText.DocumentLength(), TChar(KCarriageReturn));
		}
	}

/**
Converts 8 bit data to Unicode and inserts to the CRichText object.
@param aFile 			The file containing the plain body text.
@param aRichText 		The CRichText object that will be filled on return.
@param aCharSet 		The charset of the body part.
@param aDefaultCharSet  The default charset of the system.
@return void.
*/
void CMsvStore::Convert8BitToRichTextL(RFile& aBodyTextFile, CRichText& aRichText, TUint aCharSet, TUint aDefaultCharSet)
	{
	RFileReadStream inputStream(aBodyTextFile);
	inputStream.PushL();
	MStreamBuf* sourceStream = inputStream.Source();
	
	CCnvCharacterSetConverter* converter = CCnvCharacterSetConverter::NewL();
	CleanupStack::PushL(converter);
	
	CCnvCharacterSetConverter::TAvailability available = CCnvCharacterSetConverter::ENotAvailable;

	if (aCharSet != 0)
		{
		available = converter->PrepareToConvertToOrFromL(aCharSet, iFs);
		} 
	if (available == CCnvCharacterSetConverter::ENotAvailable)
		{
		available = converter->PrepareToConvertToOrFromL(aDefaultCharSet, iFs);
		}
	if (available != CCnvCharacterSetConverter::EAvailable)
		{
		User::Leave(KErrNotSupported);
		}	
		
	HBufC8* inBuf = HBufC8::NewLC(KMsvDecodeChunkLength);
	HBufC16* outBuf = HBufC16::NewLC(KMsvDecodeChunkLength);
	TPtr8 inPtr = inBuf->Des();
	TPtr16 outPtr = outBuf->Des();
	
	// Used to preserve state across multiple calls to ConvertToUnicode.
	TInt state = CCnvCharacterSetConverter::KStateDefault; 
	TBool newLine = EFalse;
	TBool newPara = EFalse;
	TInt bytesRemaining = sourceStream->SizeL();

	while(bytesRemaining > 0)
		{
		// Set the buffer length to the smaller of the number of bytes remaining,
		// or the chunk length.
		inPtr.SetLength(bytesRemaining < KMsvDecodeChunkLength ? bytesRemaining : KMsvDecodeChunkLength);
		// Read data from stream up to and including the 1st LF encountered.
		TRAPD(err, inputStream.ReadL(inPtr,TChar(KLineFeed)));
		if (err != KErrEof)
			{
			User::LeaveIfError(err);
			}
		
		TInt inBufLength = inPtr.Length();
		__ASSERT_DEBUG(inBufLength > 0, PanicServer(EMsvInputBufferEmpty));
		bytesRemaining -= inBufLength;
		
		// If the data read is terminated with an LF or CRLF, remove it and mark it for
		// appending a rich text line end or paragraph delimiter. 
		TInt bytesTruncated = 0;
		if (inPtr[inBufLength - 1] != KLineFeed)
			{
			// The data read does not contain any new line or paragraph delimters.
			newLine = EFalse;
			newPara = EFalse;
			}
		else
			{
			// Data read is terminated with LF.  Truncate length of data to ignore LF or CRLF bytes.
			if (inBufLength > 1 && inPtr[inBufLength - 2] == KCarriageReturn)
				{
				bytesTruncated = 2;
				inBufLength -= bytesTruncated;
				inPtr.SetLength(inBufLength);  // Remove CRLF.
				}
			else
				{
				bytesTruncated = 1;
				inBufLength -= bytesTruncated;
				inPtr.SetLength(inBufLength);  // Remove LF.
				}
			
			// Work out if it should be a new line or new paragraph.
			if (inBufLength == 0)
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
		User::LeaveIfError(bytesUnconverted);
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
		aRichText.InsertL(aRichText.DocumentLength(), outPtr);

		// Insert a new line or paragraph delimiter if necessary.
		if (newLine)
			{
			aRichText.InsertL(aRichText.DocumentLength(), CEditableText::ELineBreak);
			}
			
		if (newPara)
			{
			aRichText.InsertL(aRichText.DocumentLength(), CEditableText::EParagraphDelimiter);
			}
		}
	CleanupStack::PopAndDestroy(4, &inputStream); //outBuf,inBuf,converter,inputStream
	}


EXPORT_C void CMsvStore::DeleteBodyTextL()
//
// Deletes the body text stream
//
/** Deletes the stream that contains the body text. Write access is needed for 
this function.

@leave KErrAccessDenied Store is not open for writing 
@leave Other Standard streaming leave codes */
	{
	if (IsPresentL(KMsvEntryRichTextBody))
		RemoveL(KMsvEntryRichTextBody);
	
	// Remove the body text data if stored in KMsvPlainBodyText8 or KMsvPlainBodyText16 format.
	if (IsPresentL(KMsvPlainBodyText8))
		{
		RemoveL(KMsvPlainBodyText8);
		}
	if (IsPresentL(KMsvPlainBodyText16))
		{
		RemoveL(KMsvPlainBodyText16);
		}
		
	iBodyText->RemoveL(*this);
	}


EXPORT_C void CMsvStore::DeleteL()
//
// Deletes the file store
//
/** Deletes the message store. After a delete, the behaviour of the object is undefined: 
it can only be deleted.

Write access is needed for this function.

@leave KErrAccessDenied The store is read only 
@leave Other Standard file store error codes */
	{
	if (iStore==NULL)
		return;

	if (iLockStatus==EMsvStoreUnlocked)
		User::Leave(KErrAccessDenied);
	
	iStore->DeleteL();
	delete iStore;
	iStore=NULL;
	}


EXPORT_C TInt CMsvStore::Commit()
//
//
//
/** Commits changes to the message store, returning any error code.

Committing changes makes these changes permanent, and means that the store 
cannot be rolled back to a state before the changes were begun. 

You should use this function before deleting a CMsvStore, if you have made 
changes to it through RMsvWriteStream . Note that StoreBodyTextL() performs 
a commit itself, so an additional commit is not required after its use.

The functions is identical to the following one, except that it returns an 
error code if an error occurs.

Write access is needed.

@return Changes committed successfully 
@return Store open for read only 
@return Unable to commit changes */
	{
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		return KErrAccessDenied;
	
	TInt error = KErrNone;

	// Save the attachment information if there is any...
	if( iAttachmentManager != NULL )
		{
		TRAP(error, iAttachmentManager->StoreL());
		if( error < KErrNone )
			{
			// error occured trying to save the attachments
			return error;
			}
		}
	// commit the store
	error = iStore->Commit();
	if( iAttachmentManager != NULL && error==KErrNone )
		{
		iAttachmentManager->HandleCommitComplete();
		}
	return error;
	}


EXPORT_C void CMsvStore::CommitL()
//
//
//
/** Commits changes to the message store, leaving if an error occurs.

Committing changes makes these changes permanent, and means that the store 
cannot be rolled back to a state before the changes were begun. 

You should use this function before deleting a CMsvStore, if you have made 
changes to it through RMsvWriteStream . Note that StoreBodyTextL() performs 
a commit itself, so an additional commit is not required after its use.

The functions is identical to the previous one, except that it leaves with 
the error code if an error occurs.

Write access is needed.

@leave KErrAccessDenied Store open for read only 
@leave Standard streaming error codes Unable to commit changes */
	{
	User::LeaveIfError(Commit());
	}


EXPORT_C void CMsvStore::Remove(TUid aUid)
//
//
//
/** Deletes the identified stream from the store. The function returns, whether 
or not it completes successfully.

Write access is needed for this function.

@param aUid The UID of the stream to delete */
	{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	TRAP_IGNORE(RemoveL(aUid));
#else
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		return;
	
	if((aUid == KMsvPlainBodyText16)|| aUid == (KMsvPlainBodyText16))
		{
		TRAP_IGNORE(iStoreManager.DeletePlainTextFileL(iId));
		}
	
	iStore->Remove(aUid);
#endif
	}




EXPORT_C void CMsvStore::RemoveL(TUid aUid)
//
//
//
/** Deletes the identified stream from the store. It leaves if it cannot complete 
successfully.

Write access is needed for this function.

@param aUid The UID of the stream to delete 
@leave KErrAccessDenied The store is read only 
@leave Other Standard streaming error codes */
	{
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		User::Leave(KErrAccessDenied);
	if((aUid == KMsvPlainBodyText16)|| aUid == (KMsvPlainBodyText16))
		{
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		TRAP_IGNORE(iStoreManager.DeletePlainTextFileL(iId));
#else
		iStoreManager.DeletePlainTextFileL(iId);
#endif
		}

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	iStore->Remove(aUid);
#else	
	iStore->RemoveL(aUid);
#endif
	}




EXPORT_C void CMsvStore::Revert()
//
//
//
/** Reverts the store to the previous commit level. This removes all changes that 
have been made to the store since the last commit. 

The function cannot leave: it returns, whether or not it completes successfully.

Write access is needed for this function. */
	{
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		return;

	iStore->Revert();		// Check this for 2073
	
	// load the attachment infomation if already loaded
	if( iAttachmentManager != NULL )
		{
		TRAP_IGNORE(iAttachmentManager->RestoreL()); // ignore any errors
		}
	}



EXPORT_C void CMsvStore::RevertL()
//
//
//
/** Reverts the store to the previous commit level. This removes all changes that 
have been made to the store since the last commit. 

The function leaves if it cannot complete successfully.

Write access is needed for this function.

@leave KErrAccessDenied The store is read only 
@leave Other Standard streaming error codes */
	{
	if (iLockStatus==EMsvStoreUnlocked || iStore==NULL)
		User::Leave(KErrAccessDenied);
	
	iStore->RevertL(); 

	// load the attachment infomation if already loaded
	if( iAttachmentManager != NULL )
		{
		iAttachmentManager->RestoreL();
		}
	}

EXPORT_C TInt CMsvStore::SizeL()
//
// Returns the size of the store, or zero if the file doesn't exist
//
/** Gets the size of the message store.

@return Size of the message store in bytes */
	{
	if (iStore==NULL)
		User::Leave(KErrNotFound);
	
	return iStore->Size();
	}



/**
@internalComponent
*/
EXPORT_C CMsvStore* CMsvStore::OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	CMsvStore* store=new(ELeave) CMsvStore(aObserver, aFs, aId, aStoreManager);
	CleanupStack::PushL(store);
	store->ConstructL(ETrue);
	CleanupStack::Pop(store);
	return store;
	}

/**
@internalComponent
*/
EXPORT_C CMsvStore* CMsvStore::OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId)
	{
	CMsvStore* store=new(ELeave) CMsvStore(aObserver, aFs, aId, aStoreManager);
	CleanupStack::PushL(store);
	store->Lock();
	store->ConstructL(EFalse);
	CleanupStack::Pop(store);
	return store;
	}

/**
@internalComponent
*/
EXPORT_C CMsvStore::CMsvStore(MMsvStoreObserver& aObserver, RFs& aFs, TMsvId aId, MMsvStoreManager& aStoreManager)
: iLockStatus(EMsvStoreUnlocked), iFs(aFs), iObserver(aObserver), iId(aId), iConstructed(EFalse), iStoreManager(aStoreManager)
	{}




void CMsvStore::ConstructL(TBool aReadOnly)
	{
	iStore=CMsvCachedStore::OpenL(iId, iStoreManager, aReadOnly);
	iBodyText = CMsvBodyText::NewL();
	iConstructed=ETrue;
	}


/**
Returns an interface for the Attachment Manager.

This methods returns an interface that can be used to provided attachment
management functionality for each message entry.

@return A reference to the Attachment Manager interface
@see MMsvAttachmentManager
*/
EXPORT_C MMsvAttachmentManager& CMsvStore::AttachmentManagerL()
	{
	if( iAttachmentManager==NULL )
		{
		TBool isStoreReadOnly = (iLockStatus == EMsvStoreUnlocked ? ETrue : EFalse);
		iAttachmentManager = CMsvAttachmentManager::NewL(*this, iStoreManager, iId, iFs, isStoreReadOnly);
		}
		
	MMsvAttachmentManager& attachmentAPI = *iAttachmentManager;
	return attachmentAPI;
	}
	
EXPORT_C MMsvAttachmentManagerSync& CMsvStore::AttachmentManagerExtensionsL()
	{
	if( iAttachmentManager==NULL )
		{
		TBool isStoreReadOnly = (iLockStatus == EMsvStoreUnlocked ? ETrue : EFalse);
		iAttachmentManager = CMsvAttachmentManager::NewL(*this, iStoreManager, iId, iFs, isStoreReadOnly);
		}
		
	return (MMsvAttachmentManagerSync&) *iAttachmentManager;
	}

/**
@internalTechnology
@released
*/
EXPORT_C void CMsvStore::CreateShareProtectedAttachmentL(const TDesC& aFileName, RFile& aAttachmentFile, CMsvAttachment* aAttachmentInfo)
	{
	if (iAttachmentManager == NULL)
		{
		TBool isStoreReadOnly = (iLockStatus == EMsvStoreUnlocked ? ETrue : EFalse);
		iAttachmentManager = CMsvAttachmentManager::NewL(*this, iStoreManager, iId, iFs, isStoreReadOnly);
		}

	iAttachmentManager->CreateShareProtectedAttachmentL(aFileName, aAttachmentFile, aAttachmentInfo);
	}


/**
Intialise and return the CMsvPlainBodyText object that can be used for reading/writing plain body text.
@param aIs8Bit				TBool indicating whether to store bdy text as 8/16 bit.
@param aCharsetId			The charset of the body part.
@param aDefaultCharsetId    The default charset of the system.
@leave KErrAccessDenied		If Store is opened in Read mode.
@return CMsvPlainBodyText.
*/
EXPORT_C CMsvPlainBodyText* CMsvStore::InitialisePlainBodyTextForWriteL(TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId)
	{
	if(iLockStatus != EMsvStoreLocked)
		{
		User::Leave(KErrAccessDenied);
		}
	return CMsvPlainBodyText::NewL(iStoreManager, *this, aIs8Bit, aCharsetId, aDefaultCharsetId, iId, iFs);
	}


/**
Intialise and return the CMsvPlainBodyText object that can be used for reading plain body text.
@param aChunkLength		The length of the chunk that will be processed while reading.
@leave KErrAccessDenied	If Store is opened in Write mode.
@return CMsvPlainBodyText.
*/
EXPORT_C CMsvPlainBodyText* CMsvStore::InitialisePlainBodyTextForReadL(TInt aChunkLength)
	{
	if(iLockStatus == EMsvStoreLocked)
		{
		User::Leave(KErrAccessDenied);
		}
	return CMsvPlainBodyText::NewL(iStoreManager, *this, iId, iFs, aChunkLength);
	}



#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
Construct the CacheEntry.
@param aMtmId  A TUid.
@param aIsBodyEntry A TEntryStoreType.
@return None.
@internalComponent
*/

void CMsvStore::ConstructDbL(TUid aMtmId,TBool aReadOnly)
	{
	iStore=CMsvCachedStore::OpenL(iId, iStoreManager, aReadOnly,aMtmId);
	iStore->iMtmId = aMtmId;
	iBodyText = CMsvBodyText::NewL();	
	iConstructed=ETrue;
	}

/**
Open the Store for read.
@param aObserver A MMsvStoreObserver as reference.
@param aFs A RFs as reference.
@param aStoreManager A MMsvStoreManager as reference.
@param aId A TMsvId
@param aMtmId A TUid.
@param aIsBodyEntry A TEntryStoreType
@return CMsvStore*
@internalComponent
*/

EXPORT_C CMsvStore* CMsvStore::OpenForReadL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId)
	{
	CMsvStore* store=new(ELeave)CMsvStore(aObserver, aFs, aId, aStoreManager);
	CleanupStack::PushL(store);
	store->ConstructDbL(aMtmId,ETrue);
	CleanupStack::Pop(store);
	return store;	
	}


/**
Open the Store for write.
@param aObserver A MMsvStoreObserver as reference.
@param aFs A RFs as reference.
@param aStoreManager A MMsvStoreManager as reference.
@param aId A TMsvId
@param aMtmId A TUid.
@param aIsBodyEntry A TEntryStoreType
@return CMsvStore*
@internalComponent
*/

EXPORT_C CMsvStore* CMsvStore::OpenForWriteL(MMsvStoreObserver& aObserver, RFs& aFs, MMsvStoreManager& aStoreManager, TMsvId aId, TUid aMtmId)
	{
	CMsvStore* store=new(ELeave)CMsvStore(aObserver, aFs, aId, aStoreManager);
	CleanupStack::PushL(store);
	store->ConstructDbL(aMtmId,EFalse);
	store->Lock();
	CleanupStack::Pop(store);
	return store;
	}

/**
Restore the Header entry.
@param None.
@return None.
@internalComponent
*/
void CMsvStore::RestoreL()
	{
	if (iStore==NULL )
		{
		User::Leave(KErrNotFound);
		}

	iStoreManager.LoadHeaderEntryL(iStore->iMtmId, iId, iStore->iHeaderFieldList);	
	}


/**
Create or Update the Header entry.
@param None.
@return None.
@internalComponent
*/
void CMsvStore::CommitHeaderL()
	{
	TInt error = KErrNone;
	if(!iStore->isNewEntry)
		{
		TRAP(error, iStoreManager.UpdateHeaderEntryL(iStore->iMtmId, iId, iStore->iHeaderFieldList));
		}
	else
		{
		TRAP(error, iStoreManager.CreateHeaderEntryL(iStore->iMtmId, iId, iStore->iHeaderFieldList));
		iStore->isNewEntry = EFalse;
		}
	User::LeaveIfError(error);	
	}

/**
Returns ETrue if entries stored in database
*/
EXPORT_C TBool CMsvStore::IsDbStore()
	{
	return 	iStore->isDbStore;
	}


/** Create TMsvWriteStore to read message store.

@param aMsvStore The aMsvStore as reference. 
@return TMsvWriteStore  write sore.
 */
EXPORT_C TMsvWriteStore::TMsvWriteStore(CMsvStore& aMsvStore):iMsvStore(aMsvStore)
	{
	}

/**
Assign the  header Fields in to header Field list.
@param aHeaderFields : CHeaderFields*
@return None.
*/

EXPORT_C void TMsvWriteStore::AssignL(CHeaderFields* aHeaderFields)
	{
	iMsvStore.iStore->AssignL(aHeaderFields);
	}
	
/**
Commit the header entry to message store 

@param None.
@return None.
*/	
	
EXPORT_C void TMsvWriteStore::CommitL()
	{
	iMsvStore.CommitHeaderL();
	}
	

/** Create TMsvReadStore to read message store.

@param aMsvStore The aMsvStore as reference. 
@param aUid The UID.
@return TMsvReadStore  read sore.
 */


EXPORT_C TMsvReadStore::TMsvReadStore(CMsvStore& aMsvStore, TUid aUid):
iMsvStore(aMsvStore), iUid(aUid)
	{	
	}
	

/** Load the Header entry from message store.

@param aHeaderFields The CHeaderFields as pointer reference. 
@return None.
 */
	
	
EXPORT_C void TMsvReadStore::LoadL(CHeaderFields*& aHeaderFields)
	{
	iMsvStore.RestoreL();
	iMsvStore.iStore->GetHeaderL(iUid, aHeaderFields);
	}
	

/** Read the Header entry from message store.

@param aHeaderFields The CHeaderFields as pointer reference. 
@return None.
 */

	
EXPORT_C void TMsvReadStore::ReadL(CHeaderFields*& aHeaderFields)
	{
	iMsvStore.iStore->GetHeaderL(iUid, aHeaderFields);
	}
		

#endif //#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
