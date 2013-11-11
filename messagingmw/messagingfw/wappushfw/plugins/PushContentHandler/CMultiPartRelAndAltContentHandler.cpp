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
// Implements Multipart Related Content Handler class 'CMultipartRelatedContentHandler'
// 
//

// Local includes
//
#include "CMultiPartRelAndAltContentHandler.h"

// System includes
//
#include "cmultiparttextiterator.h"
#include <push/cmultipartpushmsgentry.h>
#include <push/cunknownpushmsgentry.h>
#include <push/cwappushmsgutils.h>


// System includes
//
#include <msvids.h>

// Constants
_LIT(KReserved, "Reserved");

void CMultiPartRelAndAltContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CMultiPartRelAndAltContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * The Multipart Related Content handler 
 * Index number : ESLContentHandlerIndex 
 */ 
CMultiPartRelAndAltContentHandler::CMultiPartRelAndAltContentHandler()
: CContentHandlerBase(), iMsgEntryId(KMsvNullIndexEntryId)
	{
	}

/**
 *  This will complete initialization of the object
 */
void CMultiPartRelAndAltContentHandler::ConstructL()
	{
	CActiveScheduler::Add(this);
	iPushUtilities = CWapPushMsgUtils::NewL();
	}

/**
 * Static Factory Construction
 *
 * version of NewLC which leaves nothing
 * on the cleanup stack
 */
CMultiPartRelAndAltContentHandler* CMultiPartRelAndAltContentHandler::NewL()
	{
	CMultiPartRelAndAltContentHandler* self = new (ELeave) CMultiPartRelAndAltContentHandler;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * Default d'tor
 */
CMultiPartRelAndAltContentHandler::~CMultiPartRelAndAltContentHandler()
	{
	delete iMultiMessage;
	delete iPushUtilities;
	}

/**
* Setup the multipart message.
*/
void CMultiPartRelAndAltContentHandler::LoadMultiPartMsgL()
	{
#ifdef _DEBUG
	_LIT(KNullMsg,"NULL CPushMessage");
	__ASSERT_DEBUG( iMessage != 0 , User::Panic(KNullMsg,0));
#endif
	TPtrC contentType;
	iMessage->GetContentType(contentType);
	if (!contentType.FindF(KMultipartText))
		iMultiMessage = CMultipartTextIterator::NewL(*iMessage);
	else if (!contentType.FindF(KMultipartBin))
		iMultiMessage = CMultipartBinIterator::NewL(*iMessage);
	else
		User::Leave(KErrCorrupt);
	iMultiMessage->FirstL();
	}

/**
 * 
 */
void CMultiPartRelAndAltContentHandler::SaveMsgRootL()
	{
	TPtrC8 msgHeaderPtr;
	iMessage->GetHeader(msgHeaderPtr);

	TBool isInt;
	TPtrC8 appURI;
	TInt appID=0;
	iMessage->GetAppIdL(appURI, appID,isInt);

	CMultiPartPushMsgEntry* msgEntry=NULL;
	if (isInt)
		{
		msgEntry=CMultiPartPushMsgEntry::NewL(appID);
		}
	else
		{
		msgEntry=CMultiPartPushMsgEntry::NewL(appURI);
		}

	CleanupStack::PushL(msgEntry);
	msgEntry->SetHeaderL(msgHeaderPtr); //Top level entry
	TPtrC contentType;
	iMessage->GetContentType(contentType);
	msgEntry->SetContentTypeL(contentType);

	__LOG_PTR_DEBUG("CMultiPartPushMsgEntry: SaveL called");
	iMsgEntryId = msgEntry->SaveL(iPushUtilities->Session(),
										KMsvGlobalInBoxIndexEntryId);
	CleanupStack::PopAndDestroy(msgEntry);
	IdleComplete();
	}


/**
 * Async. Version
 */
void CMultiPartRelAndAltContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	__LOG_PTR_DEBUG("CMultiPartRelAndAltContentHandler:HandleMessageL called");
	iAcknowledge=ETrue;
	SetConfirmationStatus(aStatus);
	iMessage = aPushMsg;
	iState = ELoadMsgData;
	IdleComplete();
	}

/**
 * Sync. Version
 */
void CMultiPartRelAndAltContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	__LOG_PTR_DEBUG("CMultiPartRelAndAltContentHandler:HandleMessageL called");
	iAcknowledge=EFalse;
	iMessage = aPushMsg;
	iState = ELoadMsgData;
	IdleComplete();
	}

void CMultiPartRelAndAltContentHandler::CancelHandleMessage()
	{
	}

void CMultiPartRelAndAltContentHandler::DoCancel()
	{
	if (iAcknowledge)
		User::RequestComplete(iConfirmStatus,KErrCancel);
	Complete (KErrCancel);
	}

/**
*RunL handles each part separately
*/
void CMultiPartRelAndAltContentHandler::RunL()
	{
	switch(iState)
		{
		case ELoadMsgData:
			LoadMsgDataL();
			break;
		case EHandlePart:
			HandlePartL();
			break;
		case ENextPart:
			NextPartL();
			break;
		case EDone:
			Complete(KErrNone);
			break;
		default:
			break;
		}
	}

/** 
 * Clean up
 */
TInt CMultiPartRelAndAltContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CMultiPartRelAndAltContentHandler:: RunError Called"); 
	iState=EDone;
	if (iMsgEntryId != KMsvNullIndexEntryId)
		{
		TRAP_IGNORE(SetMsgCorruptFlagL());
		}
	Complete(aError);
	return KErrNone;
	}

/**
 * Loads multipart data and creates multipart head entry in the message 
 * server index.
 * @leave KErrCorrupt - message data is corrupt 
 */

void CMultiPartRelAndAltContentHandler::LoadMsgDataL()
	{
	LoadMultiPartMsgL();
	SaveMsgRootL();
	iState = EHandlePart;
	}


/**
 *	Move multipart iterator to the next part
 */
void CMultiPartRelAndAltContentHandler::NextPartL()
	{
	__LOG_PTR_DEBUG("CMultiPartRelAndAltContentHandler:NextPart called");
	if (iMultiMessage->NextL())
		{
		iState = EHandlePart;
		}
	else 
		{
		iState = EDone;
		}
	IdleComplete();
	}


/**
 *	Saves message part to messaging server
 */
void CMultiPartRelAndAltContentHandler::HandlePartL()
	{
	__LOG_PTR_DEBUG("CMultiPartRelAndAltContentHandler:HandlePart called");
	CPushMessage* msgPart = iMultiMessage->PartL();
	CleanupStack::PushL(msgPart);

	// Create a new Unknown Push Entry to hold the header and body data
	TBool isInt;
	TPtrC8 appURI;
	TInt appID=0;
	iMessage->GetAppIdL(appURI, appID,isInt);

	CUnknownPushMsgEntry* msgEntry =NULL;
	if (isInt)
		{
		msgEntry = CUnknownPushMsgEntry::NewL(appID);
		}
	else
		{
		msgEntry = CUnknownPushMsgEntry::NewL(appURI);
		}
	CleanupStack::PushL(msgEntry);
	// Get the header
	TPtrC8 header;
	msgPart->GetHeader(header);
	//Get the From field
	TPtrC8 from;
	if (!msgPart->GetBinaryHeaderField(EHttpFrom,from) &&
		!msgPart->GetBinaryHeaderField(EHttpXWapInitiatorURI,from) &&
		!msgPart->GetBinaryHeaderField(EHttpContentLocation,from) )
		{
		from.Set(KNullDesC8);
		}
	// Get the body
	TPtrC8 body;
	msgPart->GetMessageBody(body);
	TPtrC content;
	msgPart->GetContentType(content);

	//Set fields of the Unknown Entry
	msgEntry->SetHeaderL(header);
	msgEntry->SetMessageDataL(body);	
	msgEntry->SetFromL(from);
	//Need this next  bit so UI knows what to do with the data in the Push Entry
	msgEntry->SetContentTypeL(content);


	__LOG_PTR_DEBUG("CUnknownPushMsgEntry: SaveL called");
	msgEntry->SaveL(iPushUtilities->Session(),iMsgEntryId);
	CleanupStack::PopAndDestroy(2);//msgEntry, msgPart
	
	iState = ENextPart;
	IdleComplete();

	}

void CMultiPartRelAndAltContentHandler::SetMsgCorruptFlagL()
	{
	CMsvEntry* msvEntry = iPushUtilities->Session().GetEntryL(iMsgEntryId);
	CleanupStack::PushL(msvEntry);
	TMsvEntry entry = msvEntry->Entry();
	// get the iMtmData1 value & zero bits 0-3 then add new status
	TInt32 mtmdata1 = (entry.MtmData1()&0xFFFFFFF0);
	mtmdata1 += CPushMsgEntryBase::EPushMsgStatusCorrupt;
	// Now set values for TMsvEntry and update the server entry
	entry.SetMtmData1(mtmdata1);
	msvEntry->ChangeL(entry);

	CleanupStack::PopAndDestroy(msvEntry);
	}
