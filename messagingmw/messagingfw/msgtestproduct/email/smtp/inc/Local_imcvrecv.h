/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/


#ifndef __LOCAL_IMCVRECV_H__
#define __LOCAL_IMCVRECV_H__

// epoc include
#include <s32mem.h>
#include <badesca.h> // required for CBufSeg
#include <msvstd.h>
#include <msventry.h>
#include <msvstore.h>
#include <msvuids.h>
#include <msvids.h>
#include <miutatch.h>
#include <miutlog.h>
#include <imcvcodc.h>
#include <imcvtext.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "timrfc822datefield.h"
#include "cimconvertheader.h"
#endif

// Forward Declaration
class CImHeader;
class CRichText;
class CLocalMimeParser;
class CLocalImRecvConvert;
class RFs;
class TImConvert;
class TImAttachmentFile;
class CImcvUtils;
class CMsvBodyText;

const TInt KHeaderBufferLength = 100;

// Max filename length
const TInt MaxMimeParameterValueLength = 256;
// Likely maximum size of an encoded parameter value decoding to a length of 
// MaxMimeParameterValueLength. In quoted printable usually 3 encoded characters
// per decoded character. 60 characters is given to allow for delimiters and
// charset information.
const TInt KMimeEncodedParameterSize = MaxMimeParameterValueLength * 3 + 60;

const TInt KConversionRemainderLength=20;

//boundary stuff
const TInt KBoundaryNotFound	=0;
const TInt KBoundaryFound		=1;
const TInt KBoundaryError		=2;


/**
Stores the details about a message nneded in the Message entry.
*/
struct TParentDetails
	{
	TInt				iSize;
	TBool				iAttachment;
	TBool				iMHTML;
	TImEmailFolderType  iFolder;	// Only interested in the Related type
	};


/**
@internalComponent
@released
*/
class CLocalRfc822Token : public CBase
	{
public:
	enum THeaderPart { EUnknown, ENotFinished, EFrom, EReplyTo, ETo, ECc, EBcc, ESubject, EDate, EMessageId, EPriority, EImportance, EReturnReceiptTo, EEndOfHeader };
	THeaderPart iHeaderPart;
	TBool iLastToken;
	TBool iFirstLine;
	TBool i822FieldsExist;

	/*IMPORT_C*/ static CLocalRfc822Token* NewLC();
	/*IMPORT_C*/ void Reset();
	inline HBufC8* OutputLine();
	/*IMPORT_C*/ virtual void ParseNextLineL( const TDesC8& aSourceLine );
	/*IMPORT_C*/ ~CLocalRfc822Token();
	inline TBool LastToken();

	inline void SetImRecvConvert( CLocalImRecvConvert* );
private:
	TBool MatchAndRemoveL( const TDesC8& );
	TBool PriorityAndReceiptsMatchAndRemoveL(THeaderPart aPriority);

	CLocalRfc822Token();
	void ConstructL();

private:
	TBuf8<KMaxIMailHeaderReadLineLength+1> iInputLine;
	TBuf8<KMaxIMailHeaderReadLineLength+1> iBufferedInput;
	HBufC8* iOutputLine;
	CLocalImRecvConvert* iImRecvConvert;
	};


/**
@internalTechnology
@released
*/
class CLocalImRecvConvert : public CBase
	{
friend class CLocalMimeParser;
public:
	/*IMPORT_C*/ static CLocalImRecvConvert* NewLC( RFs& anFs, CMsvServerEntry* aServerEntry, 
												TUid aMsgType, TMsvId aEmailServiceId);
	/*IMPORT_C*/ static CLocalImRecvConvert* NewL( RFs& anFs, CMsvServerEntry* aServerEntry,
												TUid aMsgType, TMsvId aEmailServiceId);
	/*IMPORT_C*/ ~CLocalImRecvConvert();

	/*IMPORT_C*/ void ResetL();
	/*IMPORT_C*/ void ResetForHeadersL();

	inline void SetAttachmentPathL(const TDesC& aFullPath);	// attachment path, if not set then IMCV will reject attachments
	inline void SetMsvId(TMsvId aId);

	/*IMPORT_C*/ virtual TInt ParseNextField(const TDesC8& aSourceLine);
	/*IMPORT_C*/ void MessageCompleteL();
	/*IMPORT_C*/ void MessageCompleteL(TMsvEmailEntry aEmailEntry);
	/*IMPORT_C*/ void MessageCompleteL(TBool aPartialDownload);
	/*IMPORT_C*/ TInt DeletedAttachmentSize();
	/*IMPORT_C*/ void WritePartialFooterL(TInt aAmountLeft);
	/*IMPORT_C*/ TMsvEmailEntry MessageEntryDetailsL();

	enum TValidEntryType {EUnknownEntry, EMessageEntry, EFolderEntry, EAttachmentEntry, ETextEntry, EHtmlEntry};
	enum TImEntryStreamSaveSettings
		{
		KStoreBodyText					= 0x00000001,
		KStore822Header					= KStoreBodyText << 1,		//0x00000002
		KStoreMIMEHeader				= KStore822Header << 1,	//0x00000004
		};

	inline const TTime Date() const;	// return RFC822 date
	inline const TMsvPriority Priority() const;
	inline void SaveAllAttachments(TBool aSave);
	inline const TInt ReceiveError() const;
	inline CMsvServerEntry& ServerEntry();
	inline const CImHeader& Header() const;
	inline const TBool ValidCompleteHeader() const;
	//virtual inline const TMsvId EntryId() const;	
	virtual TMsvId EntryId();	
	inline TBool NotFinishedRfc822Header();
	inline CImConvertCharconv& CharacterConverter();

protected:
	CLocalImRecvConvert(RFs& anFs, CMsvServerEntry* aServerEntry, 
							TUid aMsgType, TMsvId aEmailServiceId);
	void ConstructL(RFs& anFs);

	void ParseNextLineL(const TDesC8& aSourceLine);
	void ParseBodyLineL(const TDesC8& aSourceLine);
	void ParseMimeLineL(const TDesC8& aSourceLine);
	void EndOfHeaderProcessingL();
	virtual void  EndOfHeaderMIMEProcessingL();
	TBool CreateNonMIMEFolderEntryL(TMsvId aCurrentId);
	void DecodeAndStoreLineL(const TDesC8& aSourceLine);

	void ParseRecipientListL(CDesCArray& aRecipientList);
	void PrepareDecoder();

	void WriteToBodyL(const TDesC8& aText, TBool aBlankLine=EFalse);
	void WriteToBodyL(const TDesC16& aText);

	void AddFileExtension();
	void ExtractFilename(TLex& aLex, TDes& rFileName);
	void ReplaceInvalidCharacters(TDes& rFileName);
	void SetAttachmentName(TDes& aFileName);
	
	TBool CheckUUEStartL(const TDesC8& aSourceLine);

	// returns numeric value of text up to next delimiter
	TInt Value(const TUint8*&);
	void Whitespace(const TUint8*&);
	TBool LineIsAllWhitespace();	

	TBool CreateAttachmentL();
	virtual void WriteToAttachmentL(const TDesC8& text);
	void CloseAttachmentFileL();

	// Message Entry specific functions
	void ResetForNewEntryL(TValidEntryType entryType);
	void ResetForNonMimeEntryL();
	void MoveUpFromFolderEntryL();
	virtual void MoveToParentEntryL();
	void UpdateMultipartDataL();

	TBool StoreEntryDataL();

	virtual void StoreEntryStreamsL();
	virtual void StoreEntryStreamsL(TInt aSettings);
	void Store822HeaderL(CMsvStore& aStore, TBool& aCommit);
	void StoreMIMEHeaderL(CMsvStore& aStore, TBool& aCommit);
	void StoreBodyTextL(CMsvStore& aStore, TBool& aCommit);

	virtual void CreateEntryL();
	void Logging(const TDesC8& aString1, const TDesC8& aString2);
	void StoreMessageEntryDetailsL();
	
protected:
	enum { KNoPart = 0, KParentPart, KMultiPart };
	TInt iEmailPart;
	TMsvId iCurrentMultipartFolderEntryId;

	TBool iMessageEntryCalled;
	TBool iReceivingHeadersOnly;
	TInt iReceiveError; // indicates if any anomalies were observed whilst the message was received
	TInt iLeaveError; // keeps any errors that have caused a leave (ie Out of memory, Corrupt file, Server error)

	TMsvId iRootEntryId;
	TMsvId iTopMessagePart;

	TInt8 iBCPadding[1348]; // Padding to maintain BC - TImAttachmentFile used to be here before it's size was expanded to buffer more data.
	CMsvServerEntry* iServerEntry;
	TUid iNewMsgType;

	CLocalRfc822Token* iRfc822Token;

	TValidEntryType iDefaultEntryType;
	TValidEntryType iEntryType;

	TMsvEmailEntry* iEmailEntry;
	CImHeader* iOutputHeader;
	
	CParaFormatLayer* iParaLayer;
	CCharFormatLayer* iCharLayer;
	CRichText* iOutputBody;
	
	CImConvertCharconv* iCharConv;
	CCnvCharacterSetConverter* iConverter;
	TBool iPreparedToConvert;
	
	TBool iMIMEPart_822Header;	
	TInt iEmptyHeaderSize;
	TImBodyConvAlgorithm iAlgorithm;

	TBool iCommitLine;	
	TBool iFinalLine;	
	TBool iPrepared;
	TBool iFinishedHeader;
	TBool iNotFinishedRfc822Header;
	TBool iNewNonMIMEBodyPart;
	TBool iFirstBoundaryReached;

	CCnvCharacterSetConverter* iCharacterConverter;
	CImConvertHeader* iHeaderConverter;

	TImRfc822DateField iRfc822Date;

	RFs* iFsSession;  

	TBool iSkipData;
	TBool iEncounteredLineEndingInCarriageReturn;

	TBool iCurrentPartIsRichText;	//indicates if current body part is RichText, set on every Close Attachment()
	TImCodecQP iQPCodec;
	TImCodecB64 iB64Codec;
	TImCodecUU iUUCodec;
	TPtrC8 iThisLine;
	
	HBufC* iRemovedAttachmentTag;		// resource text which is put into the body in place of a removed MIME attachment
	HBufC* iRetainedAttachmentTag;		// Not used at all - resource text which is put into the body next to a retained MIME attachment
	HBufC* iDefaultAttachmentName;		// used when no name found	
	HBufC* iAttachmentFullPath;			// full path to current message file

	TBool iSavingAttachments;			// Turn on/off saving of attachments

	TBuf8<KMaxBoundaryTextLength> iSeekBoundary;
	
	TInt iGlobalIndent;
	TInt iPreviousLineLength;
	TInt iPreviousTrailingWhitespace;
	TChar iLastChar;
	TInt iLongestLine;
	TBool iNewEntry;

	TImAttachmentFileState iAttachmentFileState;
	TUint32 iEntryDataSize;

	TBool	iLogFileExists;
	RFile	iLogFile;
	TLex8	iLineLex;
	TTime   iTimeDate;

	//priorities   //vk
	TInt iPriority;		//{1..5}
	TInt iXPriority;   // {1..5} 1,2-high, 3-normal, 4,5-low
	TBuf8<KMaxPriorityTextLength> iPrecedence;  //   {bulk-low, list-normal,first class,special delivery-0}
	TBuf8<KMaxPriorityTextLength> iImportance;  // {high, low, normal}
	TMsvPriority iImPriority;

	TMsvId	iEmailServiceId;
	TBool	iPopulateMessage;
	CImcvUtils* iImcvUtils;

	TBuf8<KConversionRemainderLength> iLeftOver;

	TMsvId	iSavedEntryId;
	CArrayFixFlat<TParentDetails>* iParent;
	// Logging
	CImLog*					iImcvLog;
	TMsvId					iBodyId;
	
public:
	TFileName iAttachmentName;
	CLocalMimeParser* iMimeParser;
protected:
	TBool	iRelatedAttachments;
	CBufBase* iBodyBuf;
	CMsvBodyText *iBodyText;
	TInt iStore8BitData;
	TImAttachmentFile iAttachmentFile;
	TBool iParsedMimeBoundaryLast;
	TInt iSizeOfAttachmentsRemoved;
	HBufC* iPartialEmailFooter;
	TInt iTopPartialDownloadCounter; // added to support correct attachment downloading after partial download
	TBool iPartialEmail;
	RFile*	iFile;
	};


/**
@internalComponent
@released
*/
class CLocalMimeParser : public CBase
	{
public:
	/*IMPORT_C*/ static CLocalMimeParser* NewLC(CLocalImRecvConvert& aImRecvConvert);
	/*IMPORT_C*/ static CLocalMimeParser* NewL(CLocalImRecvConvert& aImRecvConvert);
	/*IMPORT_C*/ ~CLocalMimeParser();
	void Reset();
	void ResetForNewEntry();
	void ParseLineL(const TDesC8& aSourceLine);

	void SetBoundaryL(const TDesC8& aBoundaryText);
	TBool IsBoundary(const TDesC8& aSourceLine);
	void RemoveBoundary();
	
	inline void StoreMimeHeaderL(CMsvStore& entryStore);
	inline void StoreMimeHeaderWithoutCommitL(CMsvStore& entryStore);
	inline void RestoreMimeHeaderL(CMsvStore& entryStore);

	inline TImEncodingType ContentEncoding();
	inline TMimeContentType ContentType();
	TPtrC8 ContentSubType() const;
	TUint CurrentCharsetL() const;

	inline const TBool MessageIsMime() const;
	inline const TBool IsTerminatingBoundary() const;
	inline const TBool BoundaryExists() const;
	inline const TBool BoundaryFound() const;
	inline const TInt MimeHeaderSize() const;
	inline const TInt ReceiveError() const;
	inline const TDesC& ContentDescription() const;
	inline const TPtrC ContentLocation() const;
	inline const TPtrC8 ContentId() const;
	inline const TPtrC8 ContentDisposition() const;
	inline TBool IsMessageDigest();

	TBool VCard() const;
	TBool VCalendar() const;
	TImEmailFolderType MessageFolderType() const;
	void SetMessageFolderType(TImEmailFolderType aFolderType);
	TBool StartPart() const;
	void RestoreMimeParserL(CMsvStore& entryStore);
	void ExtractParameterInfoL(const TDesC8& aTag, TDes16& rBuffer, TDes8& rBuffer8);	
	void ExtractParameterInfoL(const TDesC8& aTag, HBufC8*& rBuffer);	
	TBool MimeFieldsExist() const;
	void ResetMimeFieldsExist();
	void DoMultipartTypeForNonMIMEL();

#if defined (_DEBUG)
	inline TLex8& Lex() {return iLex;};
#endif

private:
	CLocalMimeParser(CLocalImRecvConvert& aImRecvConvert);
	void ConstructL();

	// String scanning functions
	TBool IsSpecialChar(const TUint8 aChar);
	TBool MatchAndRemoveToken(const TDesC8& aToken);

	// MIME token recognition functions
	void DoMimeVersion();
	void DoContentTypeL();
	void DoContentLocationL();
	void DoContentBaseL();
	void DoContentIdL();
	void DoEncodingL();
	void DoDescriptionL();
	void DoDispositionL();

	// MIME Content-Type type recognition functions
	void DoTextTypeL();
	void DoMultipartTypeL();
	void DoMessageTypeL();
	void DoAttachmentTypeL();

	// Will be moved to MIUT, but for now ..
	const TPtrC8 GetContentTypeValue(const TDesC8& aContentTypeParameter) const;
	      
	TBool ParseRfc2047ParameterInfoL( const TDesC8& aParam, TDes& rBuffer);
	TBool ParseRfc2231ParameterInfoL( const TDesC8& aTag, TDes& rBuffer, TInt aOffset );
	TBool DecodeRfc2231ParameterInfoL( TDes8& aInput, TDes& rBufOut, 
											TPtrC8 aCharset/*, TPtrC8 aLanguage*/);
	TPtrC8 ExtractParameterString(TLexMark8& rMark);

private:
	CLocalImRecvConvert& iImRecvConvert;
	
	CImMimeHeader* iMimeHeader;
	TInt iEmptyMimeHeaderSize;
	
	TBool isMime;
	TBool iCorrectMimeVersion;
	TImEncodingType iContentEncoding;
	TMimeContentType iContentType;
	TFileName iContentDescription;
	TBool iVCard;
	TBool iVCalendar;
	TBool iStartPart;
	TImEmailFolderType iMessageFolderType;
	TUint iCharset;
	TUint iDefaultCharset;
	HBufC8* iStartId;

	// Boundary stuff
	CDesC8ArrayFlat* iBoundaryText;
	TInt iBoundaryIndex;
	TInt iBoundaryLength;
	TBool iTerminatingBoundary;
	TBool iBoundaryFound;
	TInt iReceiveError; // Indicates if any anomalies were observed whilst the message was received
						// Can be accessed via the inline ReceiveError()
	TPtrC8 iTestLine;

	HBufC8* iMimeHeaderLine;
	TLexMark8 iMark;
	TLex8 iLex;
	TLex8 iTestLex;	
	TBool isMessageDigest;
	TBool iMimeFieldsExist;
	};

#include "local_imcvrecv.inl"

#endif 
