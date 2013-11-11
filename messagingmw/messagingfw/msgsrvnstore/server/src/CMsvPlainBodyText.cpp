// Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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
// CMsvPlainBodyText.cpp
//

#include <cmsvplainbodytext.h>
#include "CMsvBodyText.h"
#include <mmsvstoremanager.h>
#include "MSVPANIC.H"
#include <txtrich.h>
#include <utf.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

/**
The number of characters that will be converted to Unicode at any one time.
*/
const TInt KMaxDecodeUnicodeLength = 20;

/**
The NewL factory function for Write operation.
@param aMsvStoreManager	 The MMsvStoreManager reference to call Store related RFile APIs
@param aStore 			 The CMsvStore object.
@param aIs8Bit			 TBool indicating whether to store bdy text as 8/16 bit.
@param aCharsetId		 The charset of the body part.
@param aDefaultCharsetId The default charset of the system.
@param aMessageId		 The Id of the message that is to be stored.
@param aFs				 The RFs for handling RFile related operations.
@return CMsvPlainBodyText.
*/
CMsvPlainBodyText* CMsvPlainBodyText::NewL(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId, TMsvId aMessageId, RFs& aFs)
	{
	CMsvPlainBodyText* self = new(ELeave)CMsvPlainBodyText(aMsvStoreManager, aStore, aIs8Bit, aCharsetId, aDefaultCharsetId, aMessageId, aFs);
	CleanupStack::PushL(self);
	self->ConstructWriteL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Overloaded NewL for Read operation.
@param aMsvStoreManager	 The MMsvStoreManager reference to call Store related RFile APIs
@param aStore			 The CMsvStore object.
@param aMessageId		 The Id of the message that is to be stored.
@param aFs				 The RFs for handling RFile related operations.
@param aChunkLength		 The length of the chunk that will be stored/restored in single operaion
@return CMsvPlainBodyText.
*/
CMsvPlainBodyText* CMsvPlainBodyText::NewL(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TMsvId aMessageId, RFs& aFs, TInt aChunkLength)
	{
	CMsvPlainBodyText* self = new(ELeave)CMsvPlainBodyText(aMsvStoreManager, aStore, aMessageId, aFs, aChunkLength);
	CleanupStack::PushL(self);
	self->ConstructReadL();
	CleanupStack::Pop(self);
	return self;
	}

/**
Default Constructor for write operation.
*/
CMsvPlainBodyText::CMsvPlainBodyText(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId, TMsvId aMessageId,RFs& aFs)
: CMsgActive(EPriorityNormal), iMsvStoreManager(aMsvStoreManager), iStore(aStore), iIs8Bit(aIs8Bit), iCharsetId(aCharsetId), iDefaultCharsetId(aDefaultCharsetId), 
  iMessageId(aMessageId), iFSession(aFs), iMsvFileStatus(EMsvFileWriting)
  	{
  	iAvailable = CCnvCharacterSetConverter::ENotAvailable;
    CActiveScheduler::Add(this);
	}

/**
Default Constructor called for Read operation..
*/
CMsvPlainBodyText::CMsvPlainBodyText(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TMsvId aMessageId, RFs& aFs, TInt aChunkLength)
: CMsgActive(EPriorityNormal), iMsvStoreManager(aMsvStoreManager), iStore(aStore), iMessageId(aMessageId), iFSession(aFs), iPos(0), iChunkMaxLength(aChunkLength), 
   iStartPosOfNextChunk(0),  iMsvFileStatus(EMsvFileReading)
	{
	iAvailable = CCnvCharacterSetConverter::ENotAvailable;
	CActiveScheduler::Add(this);
	}

/**
ConstructWriteL for creating and writing to the plain bodytext file.
*/
void CMsvPlainBodyText::ConstructWriteL()
	{
	RMsvWriteStream out;

	if(iIs8Bit)
		{
		out.AssignLC(iStore, KMsvPlainBodyText8);
		out.WriteUint32L(iCharsetId);
		out.WriteUint32L(iDefaultCharsetId);
		}
	else
		{
		out.AssignLC(iStore, KMsvPlainBodyText16);
		}
	// 	iIsCommitted will be EFalse at this point.
	out.WriteUint32L(iIsCommitted);

    TBuf<KFileNameFixedWidth> fileName;
	fileName.NumFixedWidth(iMessageId, EHex, KFileNameFixedWidth);
	
	out << (TDes&)fileName;
	out.CommitL();
	out.Close();
	iStore.CommitL();

	iMsvStoreManager.CreatePlainTextFileL(iFile, iMessageId);
	
	iRemainingConvertedData.Create(KMaxDecodeUnicodeLength);
	CleanupStack::PopAndDestroy(&out);
	}

/**
ConstructL for Read operation.
*/
void CMsvPlainBodyText::ConstructReadL()
	{
	// Check whether the data is stored in old-style non-chunk storage mechanism.
	if(iStore.IsPresentL(KMsvEntryRichTextBody) || iStore.IsPresentL(KMsv8BitEncodedBodyData))
		{
		iParaLayer = CParaFormatLayer::NewL();
		iCharLayer = CCharFormatLayer::NewL();
		iRichText = CRichText::NewL(iParaLayer, iCharLayer);
		iStore.RestoreBodyTextL(*iRichText);
		}
	else // Data is stored using the new chunk storage mechanism.
		{
		RMsvReadStream in;
		if(iStore.IsPresentL(KMsvPlainBodyText8))
			{
			in.OpenLC(iStore, KMsvPlainBodyText8);
			iCharsetId = in.ReadUint32L();
			iDefaultCharsetId = in.ReadUint32L();
			iIs8Bit = ETrue;
			}
		else if(iStore.IsPresentL(KMsvPlainBodyText16))
			{
			in.OpenLC(iStore, KMsvPlainBodyText16);
			}
		else
			{
			// The stream is not found.
			User::Leave(KErrNotFound);
			}
		iIsCommitted = in.ReadUint32L();
		HBufC* filename = HBufC::NewLC(in,KMaxFileName);
		iFilePath = HBufC::NewL(KMaxFileName);
		
		TPtr16 fileNamePtr = iFilePath->Des();
		iMsvStoreManager.BodyTextFilePathL(iMessageId, fileNamePtr);
		fileNamePtr.Append(*filename);
		iMsvStoreManager.OpenBodyTextFileForReadL(iFile, iMessageId, *iFilePath);
		
		iRemainingConvertedData.Create(KMaxDecodeUnicodeLength);
		CleanupStack::PopAndDestroy(2,&in);// filename, in
		}
	}

/**
Destructor
*/
EXPORT_C CMsvPlainBodyText::~CMsvPlainBodyText()
	{
	delete iConverter;
	delete iParaLayer;
	delete iCharLayer;
	delete iRichText;
	delete iFilePath;
	iTempChunk16.Close();
	iRemainingConvertedData.Close();

	iStartPosOfEachChunkInFile.Close();
	iRfileReadBuf8.Close();
	
	// Cannot leave in destructor, so ignore any error.
	// iFile will be closed by a call to RevertL method.
	TRAP_IGNORE(RevertL());
	}

/**
Store the plain body text part in chunks.
@param aChunk. 				The 8 bit chunk that is to be stored.
@param aStatus. 			The TRequestStatus parameter for this request.
@leave KErrAccessDenied.	If CMsvStore was opened in Read mode or
							If CommitL is already called.
@leave Other 				Standard system-wide error codes.
@return void.
*/
EXPORT_C void CMsvPlainBodyText::StoreChunkL(const TDesC8& aChunk, TRequestStatus& aStatus)
	{
	iTempChunk16.Close();
	if(iIsCommitted || iMsvFileStatus == EMsvFileReading)
		{
		// Leave if CommitL is already called or CMsvStore is opened in Read mode.
		User::Leave(KErrAccessDenied);
		}

	if(iIs8Bit)
		{
		iFile.Write(aChunk, aStatus);
		}
	// convert to 16..........
	else
		{
		ConvertChunkToUnicodeForStoreL(aChunk);
		iFile.Write(iRfileWritePtr8, aStatus);
		}
	}

/**
Store the plain body text part in chunks,synchronous version.
@param aChunk. 				The 8 bit chunk that is to be stored.
@leave KErrAccessDenied.	If CMsvStore was opened in Read mode or
							If CommitL is already called.
@leave Other 				Standard system-wide error codes.
@return void.
*/
EXPORT_C void CMsvPlainBodyText::StoreChunkL(const TDesC8& aChunk)
	{
	iTempChunk16.Close();
	if(iIsCommitted || iMsvFileStatus == EMsvFileReading)
		{
		// Leave if CommitL is already called or CMsvStore is opened in Read mode.
		User::Leave(KErrAccessDenied);
		}
	if(iIs8Bit)
		{
		User::LeaveIfError(iFile.Write(aChunk));
		}
	else
		{
		ConvertChunkToUnicodeForStoreL(aChunk);
		User::LeaveIfError(iFile.Write(iRfileWritePtr8));
		}
	}

/**
Converts the 8 bit chunk to Unicode.
@param aChunk The 8bit chunk to be converted.
@return void.
*/
void CMsvPlainBodyText::ConvertChunkToUnicodeForStoreL(const TDesC8& aChunk)
	{
	TInt chunkLength = aChunk.Length();
	TPtrC8 chunkPtr8(aChunk);

	iTempChunk16.Create(chunkLength);

	PrepareToConvertL();
	TInt charsetState = CCnvCharacterSetConverter::KStateDefault;

	if (iAvailable == CCnvCharacterSetConverter::ENotAvailable)
		{
		// Copy data if character convertor not available
		iTempChunk16.Copy(chunkPtr8);
		}
	else
		{
		TInt rem = iConverter->ConvertToUnicode(iTempChunk16, chunkPtr8, charsetState);
		if (rem < 0)
			{
			// Copy the data without conversion,if there was error while converting.
			iTempChunk16.Copy(aChunk);
			}
		else if(rem > 0)
			{
			// Some of the characters were not converted because the output buffer
			// was not long enough, so reallocate the output buffer.
			do
				{
				TBuf<KMaxDecodeUnicodeLength> buf;
				chunkPtr8.Set(chunkPtr8.Right(rem));
				rem = iConverter->ConvertToUnicode(buf, chunkPtr8, charsetState);
				if(iTempChunk16.Length() + buf.Length() > iTempChunk16.MaxLength())
					{
					iTempChunk16.ReAllocL(iTempChunk16.Length() + buf.Length());
					}
				iTempChunk16.Append(buf);
				}while(rem > 0);
			}
		}
		TPtrC8 convertedChunk8((TUint8*)(iTempChunk16.Ptr()), iTempChunk16.Length()*2);
		iRfileWritePtr8.Set(convertedChunk8);
	}

/**
Store the body part in chunks(16 bit version).
@param aChunk 			 The 16 bit chunk that is to be stored.
@param aStatus	 		 The TRequestStatus parameter for this request.
@leave KErrNotSupported  If the 8-bit storage was enabled.
@leave KErrAccessDenied  If CMsvStore was opened in Read mode or
						 IfCommitL is already called.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::StoreChunkL(const TDesC16& aChunk, TRequestStatus& aStatus)
	{
	if(iIs8Bit)	
		{
		User::Leave(KErrNotSupported);
		}
	if(iIsCommitted || iMsvFileStatus == EMsvFileReading)
		{
		// Leave if CommitL is already called or CMsvStore is opened in Read mode.
		User::Leave(KErrAccessDenied);
		}	

	TPtrC8 inFileBuf8((TUint8*)(aChunk.Ptr()), aChunk.Length()*2);
	iFile.Write(inFileBuf8, aStatus);
	}

/**
Store the body part in chunks(16 bit synchronous version).
@param aChunk 			 The 16 bit chunk that is to be stored.
@leave KErrNotSupported  If the 8-bit storage was enabled.						 
@leave KErrAccessDenied  If CMsvStore was opened in Read mode or
						 If CommitL is already called.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::StoreChunkL(const TDesC16& aChunk)
	{
	if(iIs8Bit)	
		{
		User::Leave(KErrNotSupported);
		}
	if(iIsCommitted || iMsvFileStatus == EMsvFileReading)
		{
		// Leave if CommitL is already called or CMsvStore is opened in Read mode.
		User::Leave(KErrAccessDenied);
		}

	TPtrC8 inFileBuf8((TUint8*)(aChunk.Ptr()), aChunk.Length()*2);
	User::LeaveIfError(iFile.Write(inFileBuf8));
	}

/**
Returns the size of body text in bytes.
@param None.
@return TInt  The size of the plain body text stored in bytes.
*/
EXPORT_C TInt CMsvPlainBodyText::Size()
	{
	TInt size=0;
	if(iRichText)
		{
		size = iRichText->DocumentLength() * 2;// Since Unicode multiply by 2.
		}
	else
		{
		iFile.Size(size);
		}
	return size;
	}

/**
Converts and stores the CRichText contents to a plain text.
@param aRichText 		 The CRichText object that will be stored as plain body text.
@leave KErrNotSupported  If CMsvStore was opened in Read mode or
						 If CommitL is already called.
@leave Other 			 Standard system-wide error codes.
@return void.
*/
EXPORT_C void CMsvPlainBodyText::StoreRichTextAsPlainTextL(CRichText& aRichText)
	{
	if(iIs8Bit)	
		{
		User::Leave(KErrNotSupported);
		}
	if(iIsCommitted || iMsvFileStatus == EMsvFileReading)
		{
		// Leave if CommitL is already called or CMsvStore is opened in Read mode.
		User::Leave(KErrAccessDenied);
		}
  	// create a buffer of 512 bytes to extract data from a rich text instead of extracting
  	// the entire richtext so that 2 copies of the mail is not created in RAM.
	TBuf<KMsvDecodeChunkLength> richBuf;
	TInt currentPos = 0;
	TInt richTextLen = aRichText.DocumentLength();
	TInt lengthToExtract = richTextLen;
	while(richTextLen > 0)
	 	{
	 	if(lengthToExtract > KMsvDecodeChunkLength)
	 		{
	 		lengthToExtract = KMsvDecodeChunkLength;
	 		}
	 	aRichText.Extract(richBuf,currentPos,lengthToExtract);
	 	TPtrC8 richTextPtr1((TUint8*)(richBuf.Ptr()), richBuf.Length() * 2);
	 	User::LeaveIfError(iFile.Write(richTextPtr1));
	  	currentPos += lengthToExtract;
	  	richTextLen -=lengthToExtract;
	   	if(richTextLen <= lengthToExtract)
	 		{
	 		lengthToExtract = richTextLen;
	 		}
	 	 }
	}

/**
Retrieve the next chunk of the plain body text.
@param aChunk 			 The output parameter contains the requested chunk.
@param aStatus.			 The TRequestStatus for this request.
@leave KErrAccessDenied  If CMsvStore was opened in Write mode.
@leave KErrNotSupported  If 16-bit storage is enabled.
@leave KErrUnderflow 	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 				Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::NextChunkL(TDes8& aChunk, TRequestStatus& aStatus)
	{
	if(!iIs8Bit)
		{
		// If 16-bit storage is enabled.
		User::Leave(KErrNotSupported);
		}
	if(iMsvFileStatus == EMsvFileWriting)
		{
		// Leave if CMsvStore is opened in Write mode.
		User::Leave(KErrAccessDenied);
		}	
	if(aChunk.MaxLength() < iChunkMaxLength)
		{
		User::Leave(KErrUnderflow);
		}
	
	iChunk8 = &aChunk;	
	iChunk8->SetLength(0);
	
	iRetrieving8bit = ETrue;
	
	//store the current position of the file so that if request cancels and for next 
	// call to NextChunkL() will attempt to read the same chunk that was cancelled
	iCurrentFilePos = 0;
	User::LeaveIfError(iFile.Seek(ESeekCurrent, iCurrentFilePos));
	
	Queue(aStatus);
	iFile.Read(aChunk, iChunkMaxLength, iStatus);
	SetActive();
	}

/**
Retrieve the next chunk of the plain body text.
@param aChunk 			 The output parameter contains the requested chunk.
@leave KErrNotSupported  If CMsvStore was opened in Write mode.
@leave KErrNotSupported  If 16-bit storage is enabled.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::NextChunkL(TDes8& aChunk)
	{
	if(!iIs8Bit)
		{
		// If 16-bit storage is enabled.
		User::Leave(KErrNotSupported);
		}
	if(iMsvFileStatus == EMsvFileWriting)
		{
		// Leave if CMsvStore is opened in Write mode.
		User::Leave(KErrAccessDenied);
		}
	if(aChunk.MaxLength() < iChunkMaxLength)
		{
		User::Leave(KErrUnderflow);
		}
	aChunk.SetLength(0);
	User::LeaveIfError(iFile.Read(aChunk, iChunkMaxLength));

	if(aChunk.Length()>0)
		{
		iReadChunkLength = aChunk.Length();
		}
	}

/**
Retrieve  the next chunk of the plain body text. If body is stored as 8bit, the chunk is
converted to unicode.Unconverted bytes will be converted while reading the next chunk.
@param aChunk 			 The output parameter contains the requested chunk.If no more data is in Store this
						 contains NULL descriptor.
@param aStatus  		 The TRequestStatus for this request.
@leave KErrAccessDenied  If CMsvStore was opened in Write mode.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::NextChunkL(TDes16& aChunk, TRequestStatus& aStatus)
	{
	if(iMsvFileStatus == EMsvFileWriting)
		{
		// Leave if CMsvStore is opened in Write mode.
		User::Leave(KErrAccessDenied);
		}

	if(aChunk.MaxLength() < iChunkMaxLength)
		{
		User::Leave(KErrUnderflow);
		}

	if(iRichText)
 		{
 		ExtractNextChunkFromRichText(aChunk);
	 	TRequestStatus *status=&aStatus;
		User::RequestComplete(status, KErrNone);
 		}
	else
		{
		iRfileReadBuf8.Close();
		iChunk16 = &aChunk;	
		iChunk16->SetLength(0);
		
		//store the current position of the file so that if request cancels and for next 
		// call to NextChunkL() will attempt to read the same chunk that was cancelled
		iCurrentFilePos = 0;
		User::LeaveIfError(iFile.Seek(ESeekCurrent, iCurrentFilePos));
	
		Queue(aStatus);
		if(iIs8Bit)
			{
			// if there is any converted data then append to aChunk
			if(iRemainingConvertedData.Length() != 0)
				{
				iChunk16->Append(iRemainingConvertedData);
				iRemainingConvertedData.SetLength(0);
				}
				
			iRfileReadBuf8.Create(iChunkMaxLength);
			// 8-bit data read will be converted to Unicode in DoRunL.
			iFile.Read(iRfileReadBuf8, iChunkMaxLength, iStatus);
			SetActive();
			}
		else
			{
			iRfileReadBuf8.Create(iChunkMaxLength*2);
			iFile.Read(iRfileReadBuf8, iChunkMaxLength*2, iStatus);
			SetActive();
			}
		}
	}
	
/**
Retrieve the next chunk of the plain body text. If body is stored as 8bit, convert the chunk to Unicode.
Unconverted bytes will be converted while reading the next chunk.
@param aChunk			 The output parameter contains the requested chunk.If no more data is in Store this
						 contains NULL descriptor.
@leave KErrAccessDenied  If CMsvStore was opened in Write mode.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::NextChunkL(TDes16& aChunk)
	{
	if(iMsvFileStatus == EMsvFileWriting)
		{
		// Leave if CMsvStore is opened in Write mode.
		User::Leave(KErrAccessDenied);
		}

	if(aChunk.MaxLength() < iChunkMaxLength)
		{
		User::Leave(KErrUnderflow);
		}

	if(iRichText)
 		{
 		ExtractNextChunkFromRichText(aChunk);
 		}
 	else if(iIs8Bit)
 		{
 		iRfileReadBuf8.Close();
 		iChunk16 = &aChunk;	
	
 		iChunk16->SetLength(0);
 		
		TInt numOfBytesInCurrentChunk = 0;
		// if there is any converted data then append to aChunk
		if(iRemainingConvertedData.Length() != 0)
			{
			iChunk16->Append(iRemainingConvertedData);
			iRemainingConvertedData.SetLength(0);
			}

	 	PrepareToConvertL();
	 	if(iAvailable != CCnvCharacterSetConverter::EAvailable)
	 		{
	 		User::Leave(KErrNotSupported);
	 		}

 		iIsLastChunk = EFalse;
 		while(iChunk16->Length() < iChunkMaxLength && !iIsLastChunk)
			{
			iRfileReadBuf8.Create(iChunkMaxLength);
			iFile.Read(iRfileReadBuf8, iChunkMaxLength);
			// iRfileReadBuf8 will be closed when ConvertChunkToUnicodeForRestoreL returns.
 			numOfBytesInCurrentChunk = ConvertChunkToUnicodeForRestoreL();
			}

		// Push start position of current chunk in to the stack.
		iStartPosOfEachChunkInFile.AppendL(iStartPosOfNextChunk);
		iStartPosOfNextChunk += numOfBytesInCurrentChunk;
		iReadChunkLength = numOfBytesInCurrentChunk;
		}
	else // KMsvPlainBodyText16
		{
		HBufC8* inBuffer = HBufC8::NewLC(iChunkMaxLength * 2);
		TPtr8 inPtr16(inBuffer->Des());
		iFile.Read(inPtr16, iChunkMaxLength * 2); // Multiply by 2 to get no of bytes.
		TPtrC16 ptr16((TUint16*)(inBuffer->Ptr()), inBuffer->Length() / 2);
		aChunk.Copy(ptr16);
		CleanupStack::PopAndDestroy(inBuffer);
		if(aChunk.Length())
			{
			iReadChunkLength = aChunk.Length();
			}
		
		}
	}

/**
Extracts a chunk of iChunkMaxLength or less, from the CRichText object.
*/
void CMsvPlainBodyText::ExtractNextChunkFromRichText(TDes16& aChunk)
	{
	TInt docLength = iRichText->DocumentLength();
	// If we are at the end of the body text.
	if(iPos >= docLength)
		{
		aChunk.Copy(KNullDesC16);
		}
	else
		{
		if(iPos+iChunkMaxLength <= docLength)
			{
			iRichText->Extract(aChunk, iPos, iChunkMaxLength);
			iPos += iChunkMaxLength;
			iReadChunkLength = iChunkMaxLength;
			}
		else // If it is the last chunk.
			{
			iRichText->Extract(aChunk, iPos, docLength-iPos);
			iReadChunkLength = docLength-iPos;
			iPos = docLength;
			}
		}
	}

/**
Retrieve the previous chunk of the plain body text asynchronously.
@param aChunk 			 The output parameter containing the requested chunk on completion.
@param aStatus  		 The TRequestStatus for this request.
@leave KErrNotSupported  If 16-bit storage is enabled.
@leave KErrAccessDenied  If CMsvStore was opened in Write mode.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::PreviousChunkL(TDes8& aChunk, TRequestStatus& aStatus)
	{
	TInt currentPos = 0;
	User::LeaveIfError(iFile.Seek(ESeekCurrent, currentPos));
	
	// point to the position from where previous chunk was read.
	currentPos -= iReadChunkLength;
	// point to the position from where the new chunk is to be read.
	currentPos -= iChunkMaxLength;

	if(currentPos >= 0)
		{
		iFile.Seek(ESeekStart, currentPos);
		NextChunkL(aChunk, aStatus);
		}
	else
		{
		aChunk.Copy(KNullDesC8);
		TRequestStatus *status = &aStatus;
		User::RequestComplete(status, KErrNone);
		}
	}

/**
Retrieve the previous chunk of the plain body text.
@param aChunk 			 The output parameter containing the requested chunk.
@leave KErrNotSupported  If 16-bit storage is enabled.
@leave KErrAccessDenied  If CMsvStore was opened in Write mode. 
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::PreviousChunkL(TDes8& aChunk)
	{
	TInt currentPos = 0;
	iFile.Seek(ESeekCurrent, currentPos);

	// point to the position from where previous chunk was read.
	currentPos -= iReadChunkLength;
	 // point to the position from where the new chunk is to be read.
	currentPos -= iChunkMaxLength;

	if(currentPos >= 0)
		{
		iFile.Seek(ESeekStart, currentPos );
		NextChunkL(aChunk);
		}
	else
		{
		aChunk.Copy(KNullDesC8);
		}
	}

/**
Retrieve the previous chunk of the plain body text asynchronously.DoRunl converts the chunk to unicode.
@param aChunk 			 The output parameter containing the requested chunk on completion.
@param aStatus  		 The TRequestStatus for this request.
@leave KErrNotSupported  If CMsvStore was opened in Write mode.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::PreviousChunkL(TDes16& aChunk, TRequestStatus& aStatus)
	{
	iPrevChunk = ETrue;
	if(iRichText)
 		{
 		ExtractPreviousChunkFromRichText(aChunk);
 		TRequestStatus *status = &aStatus;
		User::RequestComplete(status, KErrNone);
 		}
	else 
		{
		if(iIs8Bit && iStartPosOfEachChunkInFile.Count() >= 2)
			{
			delete iRemainingUnConvertedData;
			iRemainingUnConvertedData = NULL;
			TInt count = iStartPosOfEachChunkInFile.Count() - 1;
			TInt numOfBytes = iStartPosOfEachChunkInFile[count] - iStartPosOfEachChunkInFile[count - 1];
			iFile.Seek(ESeekStart, iStartPosOfEachChunkInFile[count - 1] );
			iReadChunkLength = numOfBytes;
			NextChunkL(aChunk,aStatus);	
			}
		else
			{
			TInt startPos = 0;
			TInt currentPos = 0;
			User::LeaveIfError(iFile.Seek(ESeekCurrent, currentPos));
			TInt prevChunkPos = 0;
			if(currentPos > 0)
				{
				startPos = currentPos - iReadChunkLength * 2;
				if(startPos > 0)
					{
					prevChunkPos = startPos - iChunkMaxLength * 2;
					if(prevChunkPos >= 0)
						{
						iFile.Seek(ESeekStart, prevChunkPos);
						NextChunkL(aChunk,aStatus);	
						}
					else
						{
						iChunk16->Copy(KNullDesC16);
						TRequestStatus *status = &aStatus;
						User::RequestComplete(status, KErrNone);
						}
					}
				else
					{
					aChunk.Copy(KNullDesC16);
					TRequestStatus *status = &aStatus;
					User::RequestComplete(status, KErrNone);
					}
				}
			else
				{
				aChunk.Copy(KNullDesC16);
				TRequestStatus *status = &aStatus;
				User::RequestComplete(status, KErrNone);
				}
			}
		}	
	}

/**
Retrieve the Previouschunk of the plain body text. If body is stored as 8bit, convert the chunk to Unicode.
@param aChunk 			 The output parameter contains the requested chunk.
@leave KErrNotSupported  If CMsvStore was opened in Write mode.
@leave KErrUnderflow	 If aChunk MaxLength is less than iChunkMaxLength.
@leave Other 			 Standard system-wide error codes.
@return void
*/
EXPORT_C void CMsvPlainBodyText::PreviousChunkL(TDes16& aChunk)
	{
	if(iRichText)
 		{
 		ExtractPreviousChunkFromRichText(aChunk);
 		}
	else 
		{
		if(iIs8Bit && iStartPosOfEachChunkInFile.Count() >= 2)
			{
			delete iRemainingUnConvertedData;
			iRemainingUnConvertedData = NULL;

			TInt count = iStartPosOfEachChunkInFile.Count() - 1;

			iFile.Seek(ESeekStart, iStartPosOfEachChunkInFile[count - 1] );
			TInt numOfBytes = iStartPosOfEachChunkInFile[count] - iStartPosOfEachChunkInFile[count - 1];
			iReadChunkLength = numOfBytes;
			// Pop one position from iStartPosOfEachChunkInFile
			iStartPosOfNextChunk = iStartPosOfEachChunkInFile[count];
			iStartPosOfEachChunkInFile.Remove(count);
			NextChunkL(aChunk);
			}
		else
			{
			TInt startPos = 0;
			TInt currentPos = 0;
			iFile.Seek(ESeekCurrent, currentPos);
			TInt prevChunkPos = 0;

			if(currentPos > 0)
				{
				startPos = currentPos - iReadChunkLength * 2;// Multiply by 2 to get no of bytes.
				if(startPos > 0)
					{
					prevChunkPos = startPos - iChunkMaxLength * 2;// Multiply by 2 to get no of bytes.
					if(prevChunkPos >= 0)
						{
						iFile.Seek(ESeekStart, prevChunkPos );
						NextChunkL(aChunk);
						}
					else
						{
						aChunk.Copy(KNullDesC16);
						}
					}
				}
			}
		}	
	}

/**
Extracts a chunk of iChunkMaxLength or less, from the CRichText object.
*/
void CMsvPlainBodyText::ExtractPreviousChunkFromRichText(TDes16& aChunk)
	{
	iPos -= iReadChunkLength;
	if(iPos > 0)
		{
		iPos -= iChunkMaxLength;
		if(iPos >= 0)
			{
			iRichText->Extract(aChunk, iPos, iChunkMaxLength);
			iPos += iChunkMaxLength;
			iReadChunkLength = iChunkMaxLength;
			}
		}
	}

/**
Asynchronously retrives the body text chunks.If body is stored as 8bit, converts the chunk to Unicode.
@param None.
@return
*/
void CMsvPlainBodyText::DoRunL()
	{
	User::LeaveIfError(iStatus.Int());
	if(iRetrieving8bit)
		{
		if(iChunk8->Length())
			{
			iReadChunkLength = iChunk8->Length();
			}
		Complete(iStatus.Int());
		iRetrieving8bit = EFalse;
		}
	else
		{
		iIsLastChunk = EFalse;
		if (iIs8Bit)
	 		{
	 		PrepareToConvertL();
		 	if(iAvailable != CCnvCharacterSetConverter::EAvailable)
		 		{
		 		User::Leave(KErrNotSupported);
		 		}
	 		TInt numOfBytesInCurrentChunk = ConvertChunkToUnicodeForRestoreL();
	 		if(iChunk16->Length() < iChunkMaxLength && !iIsLastChunk)
				{
				// iRfileReadBuf8 will be closed when ConvertChunkToUnicodeForRestoreL returns.
				iRfileReadBuf8.Create(iChunkMaxLength);
				//store the current position of the file so that if request cancels and for next 
				// call to NextChunkL() will attempt to read the same chunk that was cancelled
				iCurrentFilePos = 0;
				User::LeaveIfError(iFile.Seek(ESeekCurrent, iCurrentFilePos));
				iFile.Read(iRfileReadBuf8, iChunkMaxLength, iStatus);
				SetActive();
				}
			else
				{
				if(iPrevChunk)
					{
					//Pop one position from iStartPosOfEachChunkInFile
					TInt count = iStartPosOfEachChunkInFile.Count() - 1;
					iStartPosOfNextChunk = iStartPosOfEachChunkInFile[count];
					iStartPosOfEachChunkInFile.Remove(count);
					iPrevChunk = EFalse;
					}
				else
					{
					// Push Start position of current chunk in to the stack.
					iStartPosOfEachChunkInFile.AppendL(iStartPosOfNextChunk);
					iStartPosOfNextChunk += numOfBytesInCurrentChunk;
					}
				iReadChunkLength = numOfBytesInCurrentChunk;
				Complete(iStatus.Int());
				}
			}
		else
			{
			TPtrC16 ptr16((TUint16*)(iRfileReadBuf8.Ptr()), iRfileReadBuf8.Length()/2);
			iChunk16->Copy(ptr16);
			iRfileReadBuf8.Close();
			Complete(iStatus.Int());
			iReadChunkLength = ptr16.Length();
			}
		}
	}

/**
Converts the 8 bit chunk to unicode.When this is called iRfileReadBuf8 will have the 8-bit  
data that needs to be converted to unicode.8 bit data that is not converted is copied to 
iRemainingUnConvertedData, which is used in conversion when this is called again.
@param  None.
@return TInt Number of bytes in Current Chunk.
*/
TInt CMsvPlainBodyText::ConvertChunkToUnicodeForRestoreL()
	{
	TBufC16<KMaxDecodeUnicodeLength> outBuf;
	TPtr16 outPtr = outBuf.Des();
 	TInt charsetState = CCnvCharacterSetConverter::KStateDefault;

 	TInt numOfBytesInCurrentChunk = 0;
	TInt bytesUnconverted = 0;
	// Check if we are processing the last chunk.
	if(iRfileReadBuf8.Length() == 0)
		{
		iIsLastChunk = ETrue;
		}
	// Check if there are data that are yet to be converted.
	if(iRemainingUnConvertedData)
		{
		if(iIsLastChunk)
			{
			TPtrC8 remPtr(*iRemainingUnConvertedData);
			// If it is the lastchunk then convert the data, need not insert to input buffer.
			if(iIsLastChunk)
				{
				while(remPtr.Length() > 0)
					{
					bytesUnconverted = iConverter->ConvertToUnicode(outPtr, remPtr, charsetState);
					User::LeaveIfError(bytesUnconverted);
					iChunk16->Append(outPtr);
					remPtr.Set(remPtr.Right(bytesUnconverted));
					}
				}
			}
		else // If there are data that are yet to be converted insert it at the front of the input buffer.
			{
			iRfileReadBuf8.ReAllocL(iRfileReadBuf8.Length() + iRemainingUnConvertedData->Length());
			iRfileReadBuf8.Insert(0, *iRemainingUnConvertedData);
			}
		delete iRemainingUnConvertedData;
		iRemainingUnConvertedData = NULL;
		}
	
	TPtrC8 remainingUnconvertedData(iRfileReadBuf8);
	// This loop will convert the data to unicode till iChunk16 is filled with the converted data.
	while(remainingUnconvertedData.Length() >= KMaxDecodeUnicodeLength || (iIsLastChunk && remainingUnconvertedData.Length() > 0))
		{
		bytesUnconverted = iConverter->ConvertToUnicode(outPtr, remainingUnconvertedData, charsetState);
		User::LeaveIfError(bytesUnconverted);
		TInt bytesConverted = remainingUnconvertedData.Length()-bytesUnconverted;

		if (iChunk16->Length() + outPtr.Length() <= iChunk16->MaxLength())
			{
			iChunk16->Append(outPtr);
			numOfBytesInCurrentChunk += bytesConverted;
			}
		else
			{
			// Copy remaining unconverted data to iRemainingConvertedData,
			// if outBuf.Length() is greater than the space remains in aChunk
			TInt availableSpaceOnChunk = iChunk16->MaxLength() - iChunk16->Length();
			iChunk16->Append(outPtr.Left(availableSpaceOnChunk));
			
			numOfBytesInCurrentChunk += bytesConverted - iRemainingConvertedData.Length();
			iRemainingConvertedData.Append(outPtr.Mid(availableSpaceOnChunk));
			remainingUnconvertedData.Set(remainingUnconvertedData.Right(bytesUnconverted));
			break;
			}
		remainingUnconvertedData.Set(remainingUnconvertedData.Right(bytesUnconverted));
		}

	// Copy remaining unconverted data to iRemainingUnConvertedData
	if(remainingUnconvertedData.Length())
		{
		__ASSERT_DEBUG(iRemainingUnConvertedData == NULL, PanicServer(EMsvBufferNotEmpty));
		iRemainingUnConvertedData = remainingUnconvertedData.AllocL();
		}
	iRfileReadBuf8.Close();	
	return numOfBytesInCurrentChunk;
	}

/**
Creates the character converter and specifies the character set to convert to or from.
Leaves if the specified or default character set is not available.
@param   None.
@return  void.
*/
void CMsvPlainBodyText::PrepareToConvertL()
	{
	if(!iConverter)
		{
		iConverter = CCnvCharacterSetConverter::NewL();
	 	if (iCharsetId != 0)
		 	{
		 	iAvailable = iConverter->PrepareToConvertToOrFromL(iCharsetId, iFSession);
		 	}
		if (iAvailable == CCnvCharacterSetConverter::ENotAvailable)
			{
			iAvailable = iConverter->PrepareToConvertToOrFromL(iDefaultCharsetId, iFSession);
			}
		}
 	}

/**
Set iCharsetId to aCharset if body text of the message was downloaded as 8 bit.
This can be used to override the charset with which a message was downloaded, when
the body text is opened for reading.
@param aCharset		The new charset with which it needs to be converted.
@return void.
*/
EXPORT_C void CMsvPlainBodyText::SetCharacterSetL(const TUint aCharset)
	{
	if(iStore.IsPresentL(KMsvPlainBodyText8))
		{
		iCharsetId = aCharset;
		}
	}

/**
Returns the charset for the plain text part
@param  None.
@return TUint
*/
EXPORT_C TUint CMsvPlainBodyText::CharacterSet()
	{
	return iCharsetId ;
	}

/**
Returns the default charset for the plain text part
@param  None.
@return TUint
*/
EXPORT_C TUint CMsvPlainBodyText::DefaultCharacterSet()
	{
	return iDefaultCharsetId;
	}

/**
Commit the file and the store after it is written and and also closes the file.
@param
@return void
*/
EXPORT_C void CMsvPlainBodyText::CommitL()
	{
	iFile.Close();
	RMsvWriteStream out;
	if(iIs8Bit)
		{
		out.AssignLC(iStore, KMsvPlainBodyText8);
		out.WriteUint32L(iCharsetId);
		out.WriteUint32L(iDefaultCharsetId);
		}
	else
		{
		out.AssignLC(iStore, KMsvPlainBodyText16);
		}
	iIsCommitted = ETrue;
	out.WriteUint32L(iIsCommitted);

	TBuf<KFileNameFixedWidth> fileName;
	fileName.NumFixedWidth(iMessageId, EHex, KFileNameFixedWidth);
	out << (TDes&)fileName;

	out.CommitL();
	out.Close();
	iStore.CommitL();
	iMsvStoreManager.ReplacePlainTextFileL(iMessageId);
	CleanupStack::PopAndDestroy(&out);
	}

/**
Revert/Delete the file if it is not committed.
@param  None.
@return void.
*/
void CMsvPlainBodyText::RevertL()
	{
	iFile.Close();
	if(!iIsCommitted && iMsvFileStatus == EMsvFileWriting)
		{
		iMsvStoreManager.DeletePlainTextFileL(iMessageId);
		}
	}
	
/**
Cancel the file read operation and set the filepointer to the current position of the file 
so that next call to NextChunkL() will attempt to read the same chunk that was cancelled
@param  None.
@return void.
*/	
void CMsvPlainBodyText::DoCancel()
	{
	if(iMsvFileStatus == EMsvFileReading)
		{
		iFile.ReadCancel();
		iFile.Seek(ESeekStart, iCurrentFilePos);
		}
	CMsgActive::DoCancel();
	}
