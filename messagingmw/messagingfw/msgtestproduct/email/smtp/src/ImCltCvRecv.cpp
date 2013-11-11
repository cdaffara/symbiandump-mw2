// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @file
*/

#include <imcm.rsg>
#include <barsread.h>

#include <imcvcodc.h>
#include <imcvutil.h>
#include "cmsvbodytext.h"
#include "ImCltCvRecv.h"

#ifdef __WINS__
#include <e32wins.h> 
#include <msvapi.h>
#endif

#ifndef __WINS__
#include <msvapi.h>
#endif
#include <cmsvattachment.h> 
#include <mmsvattachmentmanager.h>
#include <mmsvattachmentmanagersync.h>

#ifndef _MSG_NO_LOGGING
#define __IMUT_LOGGING
_LIT(KLogFilePath, "c:\\logs\\mailtext\\in.txt");
_LIT8(KNewLogHeader, "\r\n------ New Receive Session ------\r\n");
_LIT8(KFound, "found: ");
_LIT8(KPartLine, "unknown or part line");
_LIT8(KEndOFHeader, "End of header");
_LIT8(KSkippingData, "Skipping data");

_LIT8(KFoundMIMEBoundary, "MIME boundary");
_LIT8(KRemoveBoundary, "Remove boundary");
_LIT8(KRemovedBoundary, "Removed boundary");
_LIT8(KSectionHeader, "Section header");
_LIT8(KWritingToFile, "writing attachment");
_LIT8(KWroteToFile, "wrote attachment");
_LIT8(KFailedToWriteToFile, "Failed to write to attachment file");
_LIT8(KFoundUUEStartBoundary, "UUE start boundary");
_LIT8(KFoundUUEEndBoundary, "UUE end boundary");
_LIT8(KReset, "Resetting CImRecvConvert");
_LIT8(KReseted, "Resetting CImRecvConvert");
_LIT8(KLastToken, "That was the last token");
_LIT8(KResetForNewEntry, "Reset for a New Entry");
_LIT8(KResetedForNewEntry, "Reseted for a New Entry");
_LIT8(KCreatingEntry, "Creating Entry");
_LIT8(KCreatedEntry, "Created Entry");
_LIT8(KUpdatingEntry, "Updating Entry");
_LIT8(KUpdatedEntry, "Updated Entry");
_LIT8(KMoveToParentEntry, "Moved to parent entry");
_LIT8(KMovedToParentEntry, "End of Moved to parent entry");

_LIT8(KIgnoringStreams, "Discarding streams");
_LIT8(KStartMessageComplete, "Start Message Complete");
_LIT8(KMessageComplete, "Message Complete");
_LIT8(KHeaderComplete, "Message Header complete");
_LIT8(KReturnReceiptTo, "return-receipt-To is set");
_LIT8(KStartStoringEntryStream, "Starting Storing Entry Stream");
_LIT8(KStoringEntryStream, "Storing Entry Stream");
_LIT8(KDoneStoringEntryStream, "Done Storing Entry Stream");

#endif // _MSG_NO_LOGGING

// uncomment the line below if logging is to be enabled

#if defined(__IMUT_LOGGING)
#define RECVLOG(text) (iImcvLog?iImcvLog->AppendComment(text):void(0));
#define RECVLOG_OUT(text) (iImcvLog?iImcvLog->AppendOut(text):void(0));
#define LOGGING(string1, string2) (Logging(string1, string2));
#else
#define RECVLOG(text) (void(0));
#define RECVLOG_OUT(text) (void(0));
#define LOGGING(string1, string2) (void(0));
#endif


const TInt KBodyTextChunkSizeBytes = 512;


/**
NewLC()
Static factory constructor. Uses two phase construction 
and pushes the newly created object into the Cleanup stack.

@param anFs
@param aClientEntry
@param aMsgType
@param aEmailServiceId
@return
*/
 CImCltRecvConvert* CImCltRecvConvert::NewLC(RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId)
	{
	CImCltRecvConvert* self = new (ELeave) CImCltRecvConvert(anFs,aClientEntry,aMsgType,aEmailServiceId);
	CleanupStack::PushL(self);
	self->ConstructL(anFs);
	return self;
	}


/**
NewL()
Static factory constructor. Uses two phase construction.

@param anFs
@param aClientEntry
@param aMsgType
@param aEmailServiceId
@return
*/
 CImCltRecvConvert* CImCltRecvConvert::NewL(RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId)
	{
	CImCltRecvConvert* self = CImCltRecvConvert::NewLC(anFs,aClientEntry,aMsgType,aEmailServiceId);
	CleanupStack::Pop();
	return self;
	}


/**
ConstructL()

@param anFs
*/
void CImCltRecvConvert::ConstructL(RFs& anFs)
	{
	iFsSession = &anFs;  

	RResourceFile resFile;
	OpenResourceFileL(resFile, anFs);	// NB leaves if file not found

	// make sure the resource file will be closed if anything goes wrong
	// CloseResourceFile is declared in IMUTDLL.H and defined in IMUTDLL.CPP
	TCleanupItem close( CloseResourceFile, &resFile );
	CleanupStack::PushL( close );
	
	// Read iStore8BitData flag.
	HBufC8* buf = resFile.AllocReadLC(STORE_8BIT_BODY_TEXT);
	TResourceReader reader;
	reader.SetBuffer(buf);
	iStore8BitData = reader.ReadInt8();
	CleanupStack::PopAndDestroy(buf);

	buf = resFile.AllocReadLC(REMOVED_ATTACHMENT_TAG);
	reader.SetBuffer(buf);
	iRemovedAttachmentTag = reader.ReadTPtrC().AllocL();
	CleanupStack::PopAndDestroy(buf);
	
	buf = resFile.AllocReadLC(DEFAULT_ATTACHMENT_NAME);
	reader.SetBuffer(buf);
	iDefaultAttachmentName = reader.ReadTPtrC().AllocL();
	CleanupStack::PopAndDestroy(buf);
	buf = resFile.AllocReadLC(PARTIAL_DOWNLOAD_FOOTER_MESSAGE);
	reader.SetBuffer(buf);
	iPartialEmailFooter = (reader.ReadTPtrC()).AllocL();
	CleanupStack::PopAndDestroy(buf);
	buf = NULL;
	CleanupStack::PopAndDestroy(&resFile);

	// create CImHeader object to store Rfc822 header info...
	iOutputHeader = CImHeader::NewLC();
	CleanupStack::Pop();			   

	if (iStore8BitData)
		{
		//Create body text storage helper.
		iBodyText = CMsvBodyText::NewL();
		}
	else
		{
		// create CRichText object to store body...
		iParaLayer = CParaFormatLayer::NewL();
		iCharLayer = CCharFormatLayer::NewL();
		iOutputBody = CRichText::NewL(iParaLayer, iCharLayer);
		}
	
	// Create Rfc822 filter object...
	iRfc822Token = CLocalRfc822Token::NewLC();
	CleanupStack::Pop();				 
	iRfc822Token->SetImRecvConvert(this);

	// Create converter objects...
	iCharacterConverter = CCnvCharacterSetConverter::NewL();
	iCharConv = CImConvertCharconv::NewL(*iCharacterConverter, anFs);
	iHeaderConverter = CImConvertHeader::NewL(*iCharConv); 

	
	// Create MIME filter object...
	iMimeParser = CLocalMimeParser::NewL(*this);
	
	
	// logfile stuff
	iImcvLog=NULL;

#ifdef __IMUT_LOGGING	
	TRAP_IGNORE(iImcvLog=CImLog::NewL(KLogFilePath, EAppend));
#endif

	iRootEntryId = EntryId();

	iNotFinishedRfc822Header = ETrue;
	
	iEmailEntry = new (ELeave) TMsvEmailEntry;
	iReceivingHeadersOnly=EFalse;	

	iParent = new (ELeave) CArrayFixFlat<TParentDetails>(3);

	iFirstBoundaryReached=EFalse;

	ResetL();
	
	RECVLOG( KNewLogHeader )
	}


/**
CImCltRecvConvert()

@param anFs
@param aClientEntry
@param aMsgType
@param aEmailServiceId
*/

CImCltRecvConvert::CImCltRecvConvert(RFs& anFs,CMsvEntry* aClientEntry,TUid aMsgType,TMsvId aEmailServiceId)
	: CLocalImRecvConvert(anFs,iNullServerEntry,aMsgType,aEmailServiceId),
	  iClientEntry(aClientEntry)
	{
	__DECLARE_NAME(_S("CImCltRecvConvert"));
	}


/**
~CImCltRecvConvert()
Destructor
*/
CImCltRecvConvert::~CImCltRecvConvert()
	{
	}


/**
ResetForHeadersL()
*/
 void CImCltRecvConvert::ResetForHeadersL()
	{
	// This function is used in preference to ResetL() when the owner
	// only wishes to create  email entries in the remote mailbox - 
	// they do not wish to create any stores associated with the message
	// entry
	ResetL();
	iReceivingHeadersOnly = ETrue;
	}


/**
ResetL()
*/
 void CImCltRecvConvert::ResetL()
	{
	RECVLOG(KReset) 

	iBodyId=KMsvNullIndexEntryId;
	iSizeOfAttachmentsRemoved=0;
	iClientEntry->SetEntryL(iRootEntryId);
	
	iReceivingHeadersOnly=EFalse;
	iMessageEntryCalled=EFalse;
	iLeaveError=KErrNone; 		
	iCurrentMultipartFolderEntryId=0;

	TParentDetails parent;
	parent.iAttachment=parent.iMHTML=EFalse;
	parent.iSize=0;
	iParent->InsertL(0,parent);

	if(iEmailEntry)
		{
		delete iEmailEntry;
		iEmailEntry=NULL;
		}
	iEmailEntry = new (ELeave) TMsvEmailEntry;

	// "iSavingAttachments" commands this code to store attachments in files.
	// Currently this is always set to ETrue as we intend to save all attachments
	// However, if the user wishes to discard (ie do not save) any 
	// attachments in incoming email messages - then this member variable
	// may be set to EFalse by the inline accessor function "SaveAllAttachments(TBool)"
	iSavingAttachments=ETrue;
	iAttachmentFileState=EFileIsClosed;

	// iLeaveError contains the error value of any functions that leaves (ie CRichText::InsertL)
	// or any returned error value that generates a leave from within CImRecvConvert (ie RFile::Create)

	iGlobalIndent=0;
	iLongestLine=50;
	
	iImPriority=EMsvMediumPriority;
	if(NULL!=iImcvUtils)
		{
		delete iImcvUtils;
		iImcvUtils=NULL;
		}
	iImcvUtils=CImcvUtils::NewL();

	// reset internal date
	iTimeDate.HomeTime();
	
	iMimeParser->Reset();
	
	iNewEntry=EFalse; //EFalse if the entry was moved to. ETrue if the entry was just created
	iTopMessagePart=KMsvRootIndexEntryId;
	ResetForNewEntryL(iDefaultEntryType);
	iMIMEPart_822Header = EFalse;
	iPartialEmail=EFalse;

	RECVLOG(KReseted) 
	}


/**
ParseNextFieldL()

@param aSourceLine
@return
*/
 TInt CImCltRecvConvert::ParseNextFieldL(const TDesC8& aSourceLine) 
	{
	__ASSERT_DEBUG(iRootEntryId!=KMsvRootIndexEntryId, gPanic(KPanicServiceIdNotValid));
	RECVLOG_OUT(aSourceLine);

	if ((iLeaveError == KErrNone) && (iClientEntry->Entry().Id() == KMsvRootIndexEntryId))
		{
		iClientEntry->SetEntryL(iSavedEntryId);
		}

	if(iLeaveError==KErrNone)
		{
		TRAP(iLeaveError, ParseNextLineL(aSourceLine));		
		}

	if (iLeaveError==KErrNone)
		iSavedEntryId = iClientEntry->Entry().Id();
	TUid type = iClientEntry->Entry().iType;
	if( type != KUidMsvMessageEntry    &&
		type != KUidMsvEmailTextEntry  &&
		type != KUidMsvEmailHtmlEntry  &&
		type != KUidMsvAttachmentEntry)
		{
		iClientEntry->SetEntryL(KMsvRootIndexEntryId);
		}


	return iLeaveError;
	}


/**
ParseNextLineL()

@param aSourceLine
*/
void CImCltRecvConvert::ParseNextLineL(const TDesC8& aSourceLine)
	{
	iParsedMimeBoundaryLast=0;
	if(!iFinishedHeader)
		{
		// start by getting the next token from the header
		iRfc822Token->ParseNextLineL(aSourceLine);

		switch(iRfc822Token->iHeaderPart)
			{
			case CLocalRfc822Token::EUnknown:
			case CLocalRfc822Token::ENotFinished:
				RECVLOG(KPartLine)
				break;
			case CLocalRfc822Token::EFrom:
				iOutputHeader->SetFromL(*iRfc822Token->OutputLine());
				LOGGING(KFound,KImcvFromPrompt);
				break;
			case CLocalRfc822Token::EReplyTo:
				iOutputHeader->SetReplyToL(*iRfc822Token->OutputLine());
				LOGGING(KFound,KImcvReplyToPrompt);
				break;
			case CLocalRfc822Token::ETo:
				ParseRecipientListL(iOutputHeader->ToRecipients());
				LOGGING(KFound,KImcvToPrompt);
				break;
			case CLocalRfc822Token::ECc: 
				ParseRecipientListL(iOutputHeader->CcRecipients());
				LOGGING(KFound,KImcvCcPrompt);
				break;
			case CLocalRfc822Token::EBcc: 
				ParseRecipientListL(iOutputHeader->BccRecipients());
				LOGGING(KFound,KImcvBccPrompt);
				break;
			case CLocalRfc822Token::ESubject:
				iOutputHeader->SetSubjectL(*iRfc822Token->OutputLine());
				LOGGING(KFound,KImcvSubjectPrompt);
				break;
			case CLocalRfc822Token::EDate:
				iRfc822Date.ParseDateField(*iRfc822Token->OutputLine() , iTimeDate); 
				if (iEmailEntry->Id() != iTopMessagePart)
					{
					iEmailEntry->iDate = iTimeDate;
					}
				LOGGING(KFound,KImcvDatePrompt);
				break;
			case CLocalRfc822Token::EMessageId:
				iOutputHeader->SetImMsgIdL(*iRfc822Token->OutputLine());
				LOGGING(KFound,KImcvMessageIdPrompt);
				break;
			case CLocalRfc822Token::EPriority:
				iImPriority=iImcvUtils->EvaluatePriorityText(*iRfc822Token->OutputLine());
				LOGGING(KFound,KImcvPriorityPrompt);
				break;
			case CLocalRfc822Token::EReturnReceiptTo:
				iOutputHeader->SetReceiptAddressL(*iRfc822Token->OutputLine());
				LOGGING(KFound,KReturnReceiptTo);
				break;
			case CLocalRfc822Token::EEndOfHeader:
				// the next line goes in the body part
				iFinishedHeader = ETrue;
				RECVLOG(KEndOFHeader)
				break;
			default:
				RECVLOG(KEndOFHeader)
				break;
			}


		if(iRfc822Token->iHeaderPart != CLocalRfc822Token::ENotFinished)
			{
			// Now that we've used the data, we also need to clear the output line from the tokeniser....
			iRfc822Token->OutputLine()->Des() = KNullDesC8;

			// whatever part we just read, we may also have read the empty line that separates
			// the header from the body
			if((iFinishedHeader = iRfc822Token->LastToken()) != EFalse)	//iFinishedHeader set to 1 on CRLF-
				{
				RECVLOG(KLastToken);
				iNotFinishedRfc822Header = EFalse;

				iHeaderConverter->SetMessageType(iMimeParser->MessageIsMime());
				iHeaderConverter->DecodeAllHeaderFieldsL(*iOutputHeader);

				if (iMimeParser->MessageIsMime())
					{
					EndOfHeaderMIMEProcessingL();
					}
				else
					{
					EndOfHeaderProcessingL();
					}

				if (iStore8BitData)
					{
					iBodyText->SetDefaultCharacterSet(iCharConv->SystemDefaultCharset());
					if ( iMimeParser->MessageIsMime() && iMimeParser->ContentType()==EMimeText )
						{
						iBodyText->SetCharacterSet(iMimeParser->CurrentCharsetL());
						}
					else
						{
						iBodyText->SetCharacterSet(0);
						}
					}
				else
					{
					// Get charset for decoding.
					if ( iMimeParser->MessageIsMime() && iMimeParser->ContentType()==EMimeText )
						{
						iCharConv->PrepareToConvertToFromOurCharsetL(iMimeParser->CurrentCharsetL());
						}
					else
						{
						iCharConv->PrepareToConvertToFromOurCharsetL(iCharConv->SystemDefaultCharset());
						}
					}
				}
			}
		}
	else if (iReceivingHeadersOnly==EFalse)
		{	
		//Set to EFalse when a single line is to be skipped (ie boundary line)
		iCommitLine=ETrue; 

		// read one line of the message body if I am processing a whole email message
		if(iMimeParser->MessageIsMime())
			{
			ParseMimeLineL(aSourceLine);
			}
		else
			{
			ParseBodyLineL(aSourceLine);	
			}

		if((iCommitLine)&&(!iSkipData))
			{
			DecodeAndStoreLineL(aSourceLine);
			}	
		}
	}


/**
MessageEntryDetailsL()

@return
*/
 TMsvEmailEntry CImCltRecvConvert::MessageEntryDetailsL()
	{

	iMessageEntryCalled=ETrue;
	CloseAttachmentFileL();

	// A message requiring manual termination and not part way through a MIME part header
	if( iTopMessagePart==EntryId() && iFinishedHeader) 
		{
		StoreEntryStreamsL(KStore822Header|KStoreMIMEHeader);
		}
	else
		{
		iClientEntry->SetEntryL(iTopMessagePart);         
		if(iEmailEntry)
			{
			delete iEmailEntry;
			iEmailEntry=NULL;
			}
	
		iEmailEntry = new (ELeave) TMsvEmailEntry(iClientEntry->Entry());
		}
		
	return *iEmailEntry;
	}


/**
MessageCompleteL()

@param aEmailEntry
*/
 void CImCltRecvConvert::MessageCompleteL(TMsvEmailEntry aEmailEntry)
	{
	if (iClientEntry->Entry().Id() == KMsvRootIndexEntryId)
		{
		iClientEntry->SetEntryL(iSavedEntryId);
		}

	__ASSERT_DEBUG(iMessageEntryCalled, gPanic(KPanicMessageEntryNotCalled));
	__ASSERT_DEBUG(aEmailEntry.Id()==iTopMessagePart, gPanic(KPanicMessageEntryIdHasChanged));

	// Called ResetL() to reset object instead of ResetForHeadersL()
	__ASSERT_DEBUG(iReceivingHeadersOnly, gPanic(KPanicIncorrectResetState)); 

	if(iEmailEntry)
		{
		if (iEmailEntry->Id())
			{
			if(iLeaveError==KErrNone)
				{
				// a remote email header cannot have any attachments or be marked as complete...
				aEmailEntry.SetAttachment(EFalse);
				aEmailEntry.SetMHTMLEmail(EFalse);
				aEmailEntry.SetComplete(EFalse);
				iClientEntry->SetEntryL(aEmailEntry.Id());
				RECVLOG(KHeaderComplete)
				}
			else
				{
				if (!iPopulateMessage)
					{
					TMsvId currentId = EntryId();
					iClientEntry->SetEntryL(iClientEntry->Entry().Parent());			
					iClientEntry->DeleteL(currentId);
					}
				User::Leave(iLeaveError);
				}
			}
		}

	iSavedEntryId = iClientEntry->Entry().Id();
	iClientEntry->SetEntryL(KMsvRootIndexEntryId);
	}


/**
MessageCompleteL()

@return
*/
 TMsvId CImCltRecvConvert::MessageCompleteL()
	{
	RECVLOG(KStartMessageComplete)
	if (iClientEntry->Entry().Id() == KMsvRootIndexEntryId)
		{
		iClientEntry->SetEntryL(iSavedEntryId);
		}
	if (iParsedMimeBoundaryLast==EFalse && iAttachmentFileState==EFileIsOpen && iPartialEmail!=EFalse)
		{
		iAttachmentFileState=EFileTopIncomplete;
		}

	CloseAttachmentFileL();

	if(!iEmailEntry->Id())
		{
		return 0;
		}

	if(iLeaveError!=KErrNone)
		{
		User::Leave(iLeaveError);
		}

	if (EntryId()!=iTopMessagePart)
		{
		iEmailEntry->SetComplete(ETrue);
		StoreEntryStreamsL(KStoreBodyText);
		MoveToParentEntryL();

		if( iTopMessagePart==EntryId() && iFinishedHeader) 
			{
			iEmailEntry->SetVisible(ETrue);
			iEmailEntry->SetInPreparation(EFalse);
			iEmailEntry->SetBodyTextComplete(ETrue);
			iEmailEntry->SetComplete(ETrue);
			iEmailEntry->SetSendingState(KMsvSendStateWaiting);
			// Main message.
			StoreMessageEntryDetailsL();
			return iEmailEntry->Id();
			}
		else if(iEmailEntry->iType == KUidMsvMessageEntry)
			{
			StoreMessageEntryDetailsL();
			MoveToParentEntryL();
			StoreMessageEntryDetailsL();
			}
		}

	iClientEntry->SetEntryL(iTopMessagePart);         
	if(NULL!=iEmailEntry)
		{
		delete iEmailEntry;
		iEmailEntry=NULL;
		}
	iEmailEntry = new (ELeave) TMsvEmailEntry(iClientEntry->Entry());
	iEmailEntry->SetVisible(ETrue);
	iEmailEntry->SetInPreparation(EFalse);
	iEmailEntry->SetComplete(ETrue);
	iEmailEntry->SetBodyTextComplete(ETrue);

	iEmailEntry->SetSendingState(KMsvSendStateWaiting);	


	iEmailEntry->SetAttachment(iParent->At(0).iAttachment);
	iEmailEntry->SetMHTMLEmail(iParent->At(0).iMHTML);

	if(iEmailEntry->MHTMLEmail() == EFalse && iEmailEntry->Attachment() == EFalse && iRelatedAttachments !=EFalse)
		{
		iEmailEntry->SetAttachment(ETrue);
		}
	iRelatedAttachments=EFalse;
	
	iEmailEntry->iSize=iParent->At(0).iSize;
	iEmailEntry->SetMessageFolderType(iParent->At(0).iFolder);

	iClientEntry->ChangeL(*iEmailEntry);

	iSavedEntryId = iClientEntry->Entry().Id();
	iClientEntry->SetEntryL(KMsvRootIndexEntryId);
	RECVLOG(KMessageComplete)
	return iEmailEntry->Id();
	}


 /**
ParseMimeLineL()

@param aSourceLine
*/ 
void CImCltRecvConvert::ParseMimeLineL(const TDesC8& aSourceLine)  
	{
	iTopPartialDownloadCounter+=aSourceLine.Length(); // increment the number of bytes of the attachment downloaded so far
	if(!iMimeParser->IsBoundary(aSourceLine))
		{
		if(!iPrepared) // first line of the body
			{		
			if (iMIMEPart_822Header)
				{
				// missing 822 part header. Revert to default.
				EndOfHeaderMIMEProcessingL();
				iMIMEPart_822Header=EFalse;
				}

			PrepareDecoder();
			}

		if (CheckUUEStartL(aSourceLine))
			{
			iAlgorithm=EUUDecode;
			iCommitLine=EFalse;
			}
		}
	else
		{
		iParsedMimeBoundaryLast=ETrue;
		// found a MIME boundary so store the current parts data and update its entry.

		RECVLOG(KFoundMIMEBoundary)
		iCommitLine=EFalse; //Dont store this line as its a boundary.

		if(!iFirstBoundaryReached && iEntryType==EFolderEntry)
			{
			iFirstBoundaryReached=ETrue;
			if (!iCurrentMultipartFolderEntryId)
				iCurrentMultipartFolderEntryId = EntryId();
			MoveToParentEntryL();
			ResetForNewEntryL(iDefaultEntryType);
			return; // First boundary encountered.
			}

		CloseAttachmentFileL();
		if(iNewEntry)
			{
			iEmailEntry->SetComplete(ETrue);
			iEmailEntry->SetBodyTextComplete(ETrue);
			StoreEntryStreamsL(KStoreMIMEHeader | KStoreBodyText);
			if (iBodyId==KMsvNullIndexEntryId)
				{
				iBodyId=iEmailEntry->Id();
				}
			}

		iSkipData = EFalse;

		// check whether we just found the terminating boundary...
		if(iMimeParser->IsTerminatingBoundary())
			{
			RECVLOG(KRemoveBoundary);
			iMimeParser->RemoveBoundary();
			RECVLOG(KRemovedBoundary);
			iMIMEPart_822Header = EFalse;

			if (EntryId()!=iTopMessagePart)
				{
				if(iEmailPart==KParentPart)
					{
					// rfc822 message which is not  not multipart.
					iEmailPart=KNoPart;
					MoveToParentEntryL();				

					// Embedded message
					iEmailEntry->SetComplete(ETrue);
					iEmailEntry->SetBodyTextComplete(ETrue);
					StoreMessageEntryDetailsL();
					}
				else if (iEmailEntry->iType==KUidMsvMessageEntry)
					{
					// Moving up from a multi embedded rfc822 message.
					iEmailEntry->SetComplete(ETrue);
					iEmailEntry->SetBodyTextComplete(ETrue);
					StoreMessageEntryDetailsL();
					}
				}

			MoveToParentEntryL();
			iEntryDataSize = iEmailEntry->iSize;

			if(iClientEntry->Entry().iType == KUidMsvFolderEntry)
				{
				iCurrentMultipartFolderEntryId = EntryId();
				}
			else if (EntryId()!=iTopMessagePart)
				{
				if(iEmailEntry->iType == KUidMsvMessageEntry)
					{
					iEmailEntry->SetComplete(ETrue);
					iEmailEntry->SetBodyTextComplete(ETrue);
					StoreMessageEntryDetailsL();
					}
				MoveToParentEntryL();
				iEntryDataSize += iEmailEntry->iSize;
				}

			if(!iNewEntry)
				ResetForNonMimeEntryL();

			RECVLOG(KSkippingData)
			iSkipData = ETrue;
			iDefaultEntryType=ETextEntry;
			}
		else // if regular boundary
			{ 
			RECVLOG(KSectionHeader)
			if(iEmailPart==KParentPart && EntryId()!=iTopMessagePart)
				{
				// rfc822 message which is not  not multipart.
				iEmailPart=KNoPart;
				MoveToParentEntryL();
				// Embedded message
				iEmailEntry->SetComplete(ETrue);
				iEmailEntry->SetBodyTextComplete(ETrue);
				StoreMessageEntryDetailsL();
				}

			if (!iCurrentMultipartFolderEntryId && iEmailPart==KNoPart)
				MoveToParentEntryL();

			ResetForNewEntryL(iDefaultEntryType);
			}
		}
	}


/**
EndOfHeaderProcessingL()
*/
void CImCltRecvConvert::EndOfHeaderProcessingL()
	{	
	CreateEntryL();
	StoreEntryStreamsL(KStore822Header);
	iEntryDataSize = 0;
	}


/**
ParseBodyLineL()

@param aSourceLine
*/
void CImCltRecvConvert::ParseBodyLineL(const TDesC8& aSourceLine)
	{
	TInt len=aSourceLine.Length();
	iTopPartialDownloadCounter+=len; // added for TOP. increment the number of bytes of the attachment downloaded so far

	if (iBodyId==KMsvNullIndexEntryId)
		{
		iBodyId=iEmailEntry->Id();
		}
	
	TMsvId id=0;
	// first check whether this line is a UUEncode start boundary
	if(CheckUUEStartL(aSourceLine)) 
		{
		RECVLOG(KFoundUUEStartBoundary)
		TFileName tempStore = iAttachmentName;
		id = EntryId();
		if (!iNewNonMIMEBodyPart || id!=iTopMessagePart) // main message entry
			{
			iEmailEntry->SetComplete(ETrue);
			iEmailEntry->SetBodyTextComplete(ETrue);
			StoreEntryStreamsL(KStoreBodyText);
			}
		MoveToParentEntryL();
		if ( !CreateNonMIMEFolderEntryL(id))
			{
			ResetForNonMimeEntryL();
			}
			
		iEntryType = EAttachmentEntry;
		CreateEntryL();
		SetAttachmentName(tempStore);
		
		iCurrentPartIsRichText = EFalse;
		iAlgorithm=EUUDecode;
		iCommitLine=EFalse;
		if(!iSavingAttachments)
			{
			RECVLOG(KSkippingData)
			iSkipData=ETrue;
			}
		iNewNonMIMEBodyPart=EFalse;
		}
	else if(aSourceLine.CompareF(KImcvUueEnd)==0) // Checks for the UUEncode end boundary
		{
		RECVLOG(KFoundUUEEndBoundary)
		CloseAttachmentFileL();
		StoreEntryDataL();
		MoveToParentEntryL();
		iSkipData = EFalse;
		iCommitLine=EFalse;
		iNewNonMIMEBodyPart=ETrue; 
		}
	else if (iNewNonMIMEBodyPart && !( len==2 && aSourceLine[0]==KImcvCR && aSourceLine[1]==KImcvLF ))
		{
		id = EntryId();
		if (!iNewNonMIMEBodyPart || id!=iTopMessagePart)
			{
			iEmailEntry->SetComplete(ETrue);
			iEmailEntry->SetBodyTextComplete(ETrue);
			StoreEntryStreamsL(KStoreBodyText);
			}
		MoveToParentEntryL();
		if ( !CreateNonMIMEFolderEntryL(id))
			ResetForNonMimeEntryL();
		iAlgorithm=ENoAlgorithm;
		iEntryType = ETextEntry;
		CreateEntryL();
		iNewNonMIMEBodyPart=EFalse;
		}	
	}


/**
CreateNonMIMEFolderEntryL()

@param aCurrentId
@return
*/
TBool CImCltRecvConvert::CreateNonMIMEFolderEntryL(TMsvId aCurrentId)
	{
	if ( aCurrentId==iTopMessagePart || iCurrentMultipartFolderEntryId )
		return EFalse;

	iClientEntry->SetEntryL(iTopMessagePart); 
	iEmailPart = KMultiPart;
	iEntryType = EFolderEntry;
	CreateEntryL();

	TMsvId destId = EntryId();
	iClientEntry->SetEntryL(iTopMessagePart); 
	iClientEntry->MoveL(aCurrentId, destId);
	iClientEntry->SetEntryL(aCurrentId);
	iClientEntry->SetEntryL(iClientEntry->Entry().Parent()); 

	iEmailEntry->SetMessageFolderType(iMimeParser->MessageFolderType());

	RECVLOG(KResetForNewEntry) 

	iMimeParser->ResetForNewEntry();
	
	if (iStore8BitData)
		{
		//Create a buffer to hold the body text as it is down loaded.
		delete iBodyBuf;
		iBodyBuf = NULL;
		iBodyBuf = CBufFlat::NewL(KBodyTextChunkSizeBytes);
		}
	else
		{
		iOutputBody->Reset();
		}
		
	ResetForNonMimeEntryL(); 
	RECVLOG(KResetedForNewEntry) 
	return ETrue;
	}


/**
CreateAttachmentL()

@return
*/
TBool CImCltRecvConvert::CreateAttachmentL()
	{
	// added to support TOP command. Reset the download counter each time we have a new
	// attachment
	iTopPartialDownloadCounter = 0; 

	// Need to check that the complete filename: iAttachmentFullPath & iAttachmentName	
	// does not exceed 256 characters. Greater than this and it cannot be saved as a file.

	TBool addExtension = ETrue;
	if(iAttachmentName.Length() == 0)	//i.e. problem with Attachment name
	    {
		// No filename present. Generate one.
		if(iMimeParser->ContentDescription().Length()!=0)
			{
			// Use ContentDescription() as default name 
			// - as this is more informative than the default

			TLex sourceLineLex = iMimeParser->ContentDescription();
			ExtractFilename(sourceLineLex, iAttachmentName);
			}
		else
			{
			iAttachmentName = *iDefaultAttachmentName;	
			}
			
		}
	else
		{
		// Filename present. Check it is valid.
		ReplaceInvalidCharacters(iAttachmentName);
		if (iAttachmentName.Locate(KImcvFullStop)!=KErrNotFound)
			{
			addExtension=EFalse;
			}
			
		}
	if (addExtension)
		{
		AddFileExtension();
		}

	CMsvStore* store = iClientEntry->EditStoreL();	
	CleanupStack::PushL(store);
	CMsvAttachment* attachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	CleanupStack::PushL(attachment);
	attachment->SetAttachmentNameL(iAttachmentName);
	
	RFile file; 
	store->AttachmentManagerExtensionsL().CreateAttachmentL(iAttachmentName,file,attachment);
	CleanupStack::Pop(attachment);
	
	iAttachmentFile.SetFileHandle(file,TImAttachmentFile::EImFileWrite);
	store->CommitL();
	CleanupStack::PopAndDestroy(store); // store

	if(KErrNone!=iLeaveError)
		{
		iAttachmentFileState=EFileFailedToOpen;
		CloseAttachmentFileL();
		return EFalse;
		}

	iAttachmentFileState=EFileIsOpen;
	return ETrue;
	}

	
/**
WriteToAttachmentL()

@param text
*/
void CImCltRecvConvert::WriteToAttachmentL(const TDesC8& text)  
	{
	if ( (iAttachmentFileState==EFileIsClosed || iAttachmentFileState==EFileNotOpen) 
		&& CreateAttachmentL() && iEntryType!=EHtmlEntry)
			iEmailEntry->SetAttachment(ETrue);
	
	if(iAttachmentFileState!=EFileIsOpen || !text.Length())
		{
		RECVLOG(KSkippingData)
		iSkipData = ETrue;
		}

	// write decoded data into a file if there is any data there to write.

	RECVLOG(KWritingToFile)

	// Convert text before writing to attachment.
	
	// Store as Binary files..
	iLeaveError=iAttachmentFile.WriteFile(text);

	if(KErrNone==iLeaveError)
		{
		iEntryDataSize += text.Length();	
		}
		
	else	
		{
		// the file write failed (eg.there is no space left), set new file state and skip 
		RECVLOG(KFailedToWriteToFile)
		iAttachmentFileState=EFileIsIncomplete;
		CloseAttachmentFileL();
		}
	RECVLOG(KWroteToFile)
	}


/**
StoreEntryStreamsL()
*/
void CImCltRecvConvert::StoreEntryStreamsL()
	{
	StoreEntryStreamsL(KStoreBodyText | KStore822Header | KStoreMIMEHeader);
	}



/**
StoreEntryStreamsL()

@param aSettings
*/
void CImCltRecvConvert::StoreEntryStreamsL(TInt aSettings)
	{
	RECVLOG(KStartStoringEntryStream);
	if (iReceivingHeadersOnly==EFalse)
		{
		CMsvStore* entryStore = NULL;
		TBool commitStore = EFalse;
		TRAPD(error, entryStore = iClientEntry->EditStoreL());	
		if(error==KErrNone) // if store does not exist then the entry is the wrong type
			{
			CleanupStack::PushL(entryStore);

			if (aSettings & KStore822Header)
				Store822HeaderL(*entryStore, commitStore);
			
			if (aSettings & KStoreMIMEHeader)
				StoreMIMEHeaderL(*entryStore, commitStore);

			if (aSettings & KStoreBodyText)
				StoreBodyTextL(*entryStore, commitStore);

			// only commit to the store if I wrote something into it
			if (commitStore)
				{
				RECVLOG(KStoringEntryStream);
				entryStore->CommitL();
				}

			StoreEntryDataL();
			CleanupStack::PopAndDestroy(); //entryStore	
			}

		}
	RECVLOG(KDoneStoringEntryStream);
	}


/**
StoreEntryDataL()

@return
*/
TBool CImCltRecvConvert::StoreEntryDataL()
	{
	// NB function should only be called if a whole email is being processed
	TBool commit=EFalse;
	RECVLOG(KUpdatingEntry)

	if (iEmailEntry->iType==KUidMsvMessageEntry)
		{
		iParent->At(0).iSize += iEntryDataSize;
		}
	else
		{
		iEmailEntry->iSize += iEntryDataSize;
		if (iEntryType==EAttachmentEntry || iEntryType==EHtmlEntry)
			{
			iEmailEntry->iDetails.Set(iAttachmentName);
			}
		}
	iClientEntry->ChangeL(*iEmailEntry);

	RECVLOG(KUpdatedEntry)
	return commit;	// if I wrote data into the store, tell owner	
	}


/**
MoveToParentEntryL()
*/
void CImCltRecvConvert::MoveToParentEntryL()
	{
	// This function changes the context to the current entry's parent entry.
	RECVLOG(KMoveToParentEntry)

	// Change context to the parent entry	
	if (EntryId()==iTopMessagePart)
		{
		return; // Already there.
		}

	iClientEntry->SetEntryL(iClientEntry->Entry().Parent()); 

	iNewEntry = EFalse;
	
	// only read and write to store if this is a real email message; headers stored in
	// the remote mailbox do not require any store information.
	if (iReceivingHeadersOnly)
		{
		RECVLOG(KIgnoringStreams)
		return;
		}

	TBool allowAttachmentFlag=ETrue;
	if(iClientEntry->Entry().iType == KUidMsvFolderEntry)
		{
		// Current entry is a folder entry signifying a MIME multipart.
		// Change context to the parent entry
		TMsvEmailEntry entry = (TMsvEmailEntry) iClientEntry->Entry();
		iCurrentMultipartFolderEntryId = EntryId();
		allowAttachmentFlag = !(entry.MessageFolderType()==EFolderTypeRelated || 
								entry.MessageFolderType()==EFolderTypeAlternative);
	

		if (EntryId()!=iTopMessagePart)
			{	
			iClientEntry->SetEntryL(iClientEntry->Entry().Parent()); 
			}
		}

	TBool childIsAttachment = (iEmailEntry->Attachment() || iEmailEntry->iType == KUidMsvMessageEntry) ? ETrue:EFalse;
	TBool childIsMHTML=EFalse;
	// Dont want the flag propogated 'up' past a message entry.
	if(iEmailEntry->iType != KUidMsvMessageEntry)
		{
		childIsMHTML = iEmailEntry->MHTMLEmail() ? ETrue:EFalse;
		}
	//Make the parent entry the current entry 
	if(iEmailEntry)
		{
		delete iEmailEntry;
		iEmailEntry=NULL;
		}
	iEmailEntry = new (ELeave) TMsvEmailEntry(iClientEntry->Entry());

	if (!iParent->Count())
		{
		TParentDetails parentDetails;
		iParent->InsertL(0,parentDetails);
		}

	if (! iParent->At(0).iAttachment)
		{
		iParent->At(0).iAttachment=(childIsAttachment && allowAttachmentFlag)? ETrue:EFalse;

		// if we aren't allowing attachments because of the folder type
		// remember there where attachments and check at the end whether
		// it was an MHTML message or not.
		if(childIsAttachment && !allowAttachmentFlag)
			{
			iRelatedAttachments=ETrue;
			}
		}
	if (!iParent->At(0).iMHTML)
		{
		iParent->At(0).iMHTML = childIsMHTML ? ETrue:EFalse;
		}
	iParent->At(0).iSize += iEntryDataSize;

	iOutputHeader->Reset();
	iEmptyHeaderSize=iOutputHeader->DataSize();
	iMimeParser->ResetForNewEntry();

	iEntryDataSize=0;

	RECVLOG(KMovedToParentEntry)
	}


/**
CreateEntryL()
Creates a new entry.
*/
void CImCltRecvConvert::CreateEntryL()
	{
	RECVLOG(KCreatingEntry);
	if(NULL!=iEmailEntry)
		{
		delete iEmailEntry;
		iEmailEntry=NULL;
		}

	if (iCurrentMultipartFolderEntryId)
		{
		iClientEntry->SetEntryL(iCurrentMultipartFolderEntryId);
		iCurrentMultipartFolderEntryId=0;
		}
		
	iEmailEntry = new (ELeave) TMsvEmailEntry;

	TValidEntryType previousEntryType = iEntryType;
	if ((iTopMessagePart==KMsvRootIndexEntryId) || iMIMEPart_822Header)
		{
		// At the main header, want to create a message entry.
		// The stored iEntryType will indicate ( from header info) the next entry to be created.
		// Save temporarily.
		// Also applies to the special case where a message contains ony 1 embedded message.

		previousEntryType=iEntryType;
		iEntryType=EMessageEntry;
		}

	if ((iPopulateMessage) && (iTopMessagePart==KMsvRootIndexEntryId))
	// If this is the root of a message that is being populated then do not create it.
		{
		iClientEntry->SetEntryL(iRootEntryId);
		*iEmailEntry = iClientEntry->Entry();
		iTopMessagePart=iRootEntryId;

		// Delete all the children of the message entry.  This is needed because if the
		// message has been purged, the entries will still exist.  When the message is populated,
		// new entries are created.  If the original entries are not removed, then duplicate 
		// entries will exist.
		CMsvEntrySelection*	children = iClientEntry->ChildrenL();
		CleanupStack::PushL(children);
		if (children->Count())
		{
		TMsvLocalOperationProgress prog;
		iClientEntry->DeleteL(*children,prog);
		}
		CleanupStack::PopAndDestroy(children);
		}
	else
		{
		iEmailEntry->iMtm=iNewMsgType;
		iEmailEntry->iServiceId = iEmailServiceId;
		iEmailEntry->SetComplete(EFalse);
		iEmailEntry->iSize = 0;
		iEmailEntry->SetVisible(ETrue);
		iEmailEntry->SetInPreparation(EFalse);
		iEmailEntry->SetReceipt(EFalse);

		iEmailEntry->SetVCard(iMimeParser->VCard());
		iEmailEntry->SetVCalendar(iMimeParser->VCalendar());
		iEmailEntry->SetMessageFolderType(iMimeParser->MessageFolderType());
		iEmailEntry->SetPriority(iImPriority);
		iEmailEntry->SetNew(EFalse);

		if(iOutputHeader->ReceiptAddress().Length()>0)
			{
			iEmailEntry->SetReceipt(ETrue);
			}

		iEmailEntry->iDate=iTimeDate;
		switch(iEntryType)
			{
			case EMessageEntry:
				if(iTopMessagePart==KMsvRootIndexEntryId)
					{
					iEmailEntry->SetUnread(ETrue);
					iEmailEntry->SetNew(ETrue);
					iEmailEntry->SetVisible(EFalse);
					iEmailEntry->SetInPreparation(ETrue);
					iEmailEntry->SetSendingState(KMsvSendStateNotApplicable);
					}
				else
					{
					TParentDetails parentDetails;
					parentDetails.iMHTML=EFalse;
			        parentDetails.iAttachment=EFalse;
					parentDetails.iSize=0;
					iParent->InsertL(0,parentDetails);
					}
				iEmailEntry->iType=KUidMsvMessageEntry;
				iEmailEntry->iDetails.Set(iOutputHeader->From());
				iEmailEntry->iDescription.Set(iOutputHeader->Subject());
				break;
			case EFolderEntry:
				iEmailEntry->iType=KUidMsvFolderEntry;
				if (iMimeParser->MessageFolderType()==EFolderTypeUnknown)
					{
					// Get folder type of parent (the message)
					TMsvEmailEntry entry=iClientEntry->Entry();
					iEmailEntry->SetMessageFolderType(entry.MessageFolderType());
					}
				break;
			case EAttachmentEntry:
				iEmailEntry->iType=KUidMsvAttachmentEntry;
				iEmailEntry->iDetails.Set(iAttachmentName);
				iEmailEntry->iDescription.Set(iMimeParser->ContentDescription());
				break; 
			case ETextEntry:
				if ( iMimeParser->ContentDisposition()!=KImcvAttachment)
					iEmailEntry->iType=KUidMsvEmailTextEntry;
				else 
					{
					iEmailEntry->iType=KUidMsvAttachmentEntry;
					iEmailEntry->iDetails.Set(iAttachmentName);
					iEmailEntry->iDescription.Set(iMimeParser->ContentDescription());
					}
				break;
			case EHtmlEntry:
				iEmailEntry->iType=KUidMsvEmailHtmlEntry;
				// If disposition not set or is inline..
				if ( iMimeParser->ContentDisposition()==KImcvAttachment)
					iEmailEntry->iType=KUidMsvAttachmentEntry;
				else
					iEmailEntry->SetMHTMLEmail(ETrue);
				iEmailEntry->iDetails.Set(iAttachmentName);
				iEmailEntry->iDescription.Set(iMimeParser->ContentDescription());
				break;
			default:
				iEmailEntry->iType=KUidMsvAttachmentEntry;
				iEmailEntry->iDetails.Set(iAttachmentName);
				iEmailEntry->iDescription.Set(iMimeParser->ContentDescription());
			}

		iClientEntry->CreateL(*iEmailEntry);
		iEmailEntry->iServiceId = iEmailServiceId;
		iClientEntry->SetEntryL(iEmailEntry->Id());
		if(iTopMessagePart==KMsvRootIndexEntryId)
			{
			iTopMessagePart=iEmailEntry->Id();
			}
		}

	iEntryType=previousEntryType;
	iNewEntry = ETrue;
	RECVLOG(KCreatedEntry);
	}


/**
StoreMessageEntryDetailsL()
Stores the details of a message entry.
*/
void CImCltRecvConvert::StoreMessageEntryDetailsL()
	{
	iEmailEntry->SetAttachment(iParent->At(0).iAttachment);
	iEmailEntry->SetMHTMLEmail(iParent->At(0).iMHTML);

	if(iEmailEntry->MHTMLEmail() == EFalse && iEmailEntry->Attachment() == EFalse && iRelatedAttachments !=EFalse)
		{
		iEmailEntry->SetAttachment(ETrue);
		}
	iRelatedAttachments=EFalse;
	
	iEmailEntry->iSize=iParent->At(0).iSize;
	iEmailEntry->SetMessageFolderType(iParent->At(0).iFolder);
	StoreEntryDataL();

	if (iParent->Count()>1)
		{
		iParent->At(1).iSize+=iEmailEntry->iSize;
		iParent->Delete(0);
		}
	else
		{
		iParent->At(0).iAttachment=iParent->At(0).iMHTML=EFalse;
		iParent->At(0).iSize=0;
		}
	}


