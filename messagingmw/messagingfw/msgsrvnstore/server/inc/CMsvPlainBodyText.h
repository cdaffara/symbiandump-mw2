// CMSVPLAINBODYTEXT.H

// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMSVPLAINBODYTEXT_H__
#define __CMSVPLAINBODYTEXT_H__


#include <msvstore.h>
#include <mentact.h>
#include <charconv.h>

class CMsvStore;
class CRichText;
class CParaFormatLayer;
class CCharFormatLayer;
class CCnvCharacterSetConverter;

/**
This class encapsulates plainbody text information for storage within the
Message Store.It also populates a CRichText object with the 8 bit data
decoded to the specified character set.If a character set is not specified,
then the default character set is used.

@publishedAll
@released
*/
class CMsvPlainBodyText : public CMsgActive
	{
public:
	static CMsvPlainBodyText* NewL(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId, TMsvId aMessageId, RFs& aFs);
	static CMsvPlainBodyText* NewL(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TMsvId aMessageId, RFs& aFs, TInt aChunkLength);

	IMPORT_C ~CMsvPlainBodyText();

	IMPORT_C void StoreChunkL(const TDesC8& aChunk, TRequestStatus& aStatus);
	IMPORT_C void StoreChunkL(const TDesC8& aChunk);

	IMPORT_C void StoreChunkL(const TDesC16& aChunk, TRequestStatus& aStatus);
	IMPORT_C void StoreChunkL(const TDesC16& aChunk);

	IMPORT_C void StoreRichTextAsPlainTextL(CRichText& aRichText);

	IMPORT_C void CommitL();

	IMPORT_C void NextChunkL(TDes8& aChunk, TRequestStatus& aStatus);
	IMPORT_C void NextChunkL(TDes8& aChunk);

	IMPORT_C void NextChunkL(TDes16& aChunk, TRequestStatus& aStatus);
	IMPORT_C void NextChunkL(TDes16& aChunk);

	IMPORT_C void PreviousChunkL(TDes8& aChunk, TRequestStatus& aStatus);
	IMPORT_C void PreviousChunkL(TDes8& aChunk);

	IMPORT_C void PreviousChunkL(TDes16& aChunk, TRequestStatus& aStatus);
	IMPORT_C void PreviousChunkL(TDes16& aChunk);


	IMPORT_C void SetCharacterSetL(const TUint aCharset);
	IMPORT_C TUint CharacterSet();
	IMPORT_C TUint DefaultCharacterSet();

 	IMPORT_C TInt Size();

private:
	CMsvPlainBodyText(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore, TBool aIs8Bit, TUint aCharsetId, TUint aDefaultCharsetId, TMsvId aMessageId, RFs& aFs);
	void ConstructWriteL();

	CMsvPlainBodyText(MMsvStoreManager& aMsvStoreManager, CMsvStore& aStore,TMsvId aMessageId, RFs& aFs,TInt aChunkLength);
	void ConstructReadL();

	void PrepareToConvertL();
	void ConvertChunkToUnicodeForStoreL(const TDesC8& aChunk);
	TInt ConvertChunkToUnicodeForRestoreL();
	void ExtractNextChunkFromRichText(TDes16& aChunk);
	void ExtractPreviousChunkFromRichText(TDes16& aChunk);
	void RevertL();
	void DoRunL();
	void DoComplete();
	void DoCancel();

private:
	/** To access APIs related to RFile operations.*/
	MMsvStoreManager& 			  iMsvStoreManager;
	CMsvStore& 					  iStore;
	/** Indicates whether to store text as 8 bit or 16 bit.*/
	TBool 						  iIs8Bit;
	/** The charset of the body text*/
	TUint 						  iCharsetId;
 	TUint 					 	  iDefaultCharsetId;

 	/** The RFile Path.*/
	HBufC*	 					  iFilePath;
  	TMsvId 						  iMessageId;
	RFs& 						  iFSession;

	/** The RFile to which body text is written.*/
	RFile 						  iFile;
	/** Character converter for 8 to 16 bit conversion.*/
	CCnvCharacterSetConverter* 	  iConverter;
	CRichText* 					  iRichText;
	CParaFormatLayer* 			  iParaLayer;
	CCharFormatLayer* 			  iCharLayer;
	TInt 						  iReadChunkLength;
	/** Temporary buffer created to Store chunk while it is being written aynchronously.*/
	RBuf16	 					  iTempChunk16;
	/** Buffer used to read data from RFile.*/
	RBuf8 						  iRfileReadBuf8;
	/** Pointer to 8-bit buffer that needs to be written to RFile.*/
	TPtrC8 						  iRfileWritePtr8;
	/** The position from where to extract data in CRichText*/
	TInt 						  iPos;
	/** Contains the size of the chunk that will be read or write in the RFile*/
	TInt 					 	  iChunkMaxLength;
	/** Contains the remaining unconverted Unicode Data*/
	HBufC8* 					  iRemainingUnConvertedData;
	/** Array that stores the start positions of each chunk,in Rfile*/
	RArray<TInt> 			 	  iStartPosOfEachChunkInFile;
	/** Contains the start position of the next chunk*/
	TInt 						  iStartPosOfNextChunk;
	/** Contains the remaining converted Unicode Data*/
	RBuf16						  iRemainingConvertedData;
	/** Pointer to a 16 bit chunk that is being read from the store.*/
	TDes16*	 					  iChunk16;
	/** Pointer to a 8 bit chunk that is being read from the store.*/
	TDes8* 						  iChunk8;
	/** Indicates whether the Rfile is commited or not.*/
	TBool 						  iIsCommitted;

	/** Indicates whether the chunk being read is last.*/
	TBool 						  iIsLastChunk;
	/** Indicates whether call to DoRunl was initiated by PreviousChunkL method.*/
	TBool 						  iPrevChunk;
	/** Indicates whether call to DoRunl was initiated by 8 bit PreviousChunkL and NextChunkL methods.*/
	TBool 						  iRetrieving8bit;
	/** Enum indicating if RFile is being written or read */
	enum {EMsvFileReading, EMsvFileWriting} iMsvFileStatus;
	CCnvCharacterSetConverter::TAvailability iAvailable;
	//Containes current position of the Rfile
	TInt 						 iCurrentFilePos;
	};

#endif//__CMSVPLAINBODYTEXT_H__
