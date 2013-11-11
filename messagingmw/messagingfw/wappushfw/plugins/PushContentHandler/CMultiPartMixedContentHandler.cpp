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
// Implements Multipart Mixed Loading Content Handler 'CMultiPartMixedContentHandler'
// 
//

// Local includes
//
#include "CMultiPartMixedContentHandler.h"

// System includes
#include "cmultiparttextiterator.h"
#include "PushDispatcher.h"

// Constants
_LIT(KReserved, "Reserved");

void CMultiPartMixedContentHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CMultiPartMixedContentHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/**
 * Constructor
 *
 * @param aFs Reference to a file session
 * @param aLibrary Reference to DLL Library Object
 * @param aIndex Index number corresponding to the Unknown App Handler Class 'EMultiPartMixedContentHandler'
 */ 
CMultiPartMixedContentHandler::CMultiPartMixedContentHandler()
: CContentHandlerBase()
	{
	}

/**
 *  This will complete initialization of the object
 */
void CMultiPartMixedContentHandler::ConstructL()
	{
	CActiveScheduler::Add(this);
	}

/** 
 * Static Factory Construction
 *
 * @param aFs Reference to a file session
 * @param aLibrary Reference to DLL Library Object
 * @param aIndex Index number corresponding to the Unknown App Handler Class 'EMultiPartMixedContentHandler'
 *
 * @return fully initialized instance of this class
 */
CMultiPartMixedContentHandler* CMultiPartMixedContentHandler::NewL()
	{
	CMultiPartMixedContentHandler* self = new(ELeave) CMultiPartMixedContentHandler;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/** 
 * Destructor
 */
CMultiPartMixedContentHandler::~CMultiPartMixedContentHandler()
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: Destructor"); 
	 delete iMultiMessage;
	}


/** 
 * Creates a Multipart Binary Iterator that will be used for splitting apart the binary
 * mulitpart.
 *
 * @param aPushMsg A WAP binary multipart.mixed message that will be processed
 */
void CMultiPartMixedContentHandler::LoadMultipartMsgL()
	{
	__ASSERT_DEBUG( iMessage!= 0 , User::Panic(_L("NULL CPushMessage"),0));

	TPtrC contentType;
	iMessage->GetContentType(contentType);
	if (!contentType.FindF(KMultipartText))
		iMultiMessage = CMultipartTextIterator::NewL(*iMessage);
	else if (!contentType.FindF(KMultipartBin))
		iMultiMessage = CMultipartBinIterator::NewL(*iMessage);
	else
		User::Leave(KErrCorrupt);
	
	iMultiMessage->FirstL();
	iState = EHandlePart;
	
	IdleComplete();
	}


/** 
 * The Asynchronous entry point for this plug-in to handle the CPushMessage.
 * 
 * Since we really didn't start an async event, make it look as though our
 * asynchronous request has been completed. 
 *
 * @param aPushMsg A WAP binary multipart.mixed message that will be processed
 * @param aStatus The TRequestStatus of the caller indicating that this will be used aysnchronously
 */
void CMultiPartMixedContentHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: HandleMessage Async"); 

	iAcknowledge = ETrue;
	iMessage = aPushMsg;
	SetConfirmationStatus(aStatus);
	iState = ELoadMsgData;
	IdleComplete();
	}


/** 
 * The Synchronous entry point for this plug-in to handle the CPushMessage.
 *
 * @param aPushMsg A WAP binary multipart.mixed message that will be processed
 * @param aStatus The TRequestStatus of the caller indicating that this will be used aysnchronously
 */
void CMultiPartMixedContentHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: HandleMessage Sync");

	iAcknowledge = EFalse;
	iMessage = aPushMsg;
	iState = ELoadMsgData;
	IdleComplete();
	}


/** 
 * Cancels Asynronous requests on called handler.
 */
void CMultiPartMixedContentHandler::CancelHandleMessage()
	{
	if(iContentHandler)
		iContentHandler->CancelHandleMessage();
	}

/** 
 * Cancels Asynronous requests on caller and completes self.
 */
void CMultiPartMixedContentHandler::DoCancel()
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: DoCancel");

	 if (iAcknowledge)
		User::RequestComplete(iConfirmStatus,KErrCancel);
	
	 Complete(KErrCancel);
	}

/** 
 * State machine to step through the multipart message until it is done
 *
 * State EHandlePart: Starts the processing of each part of the multipart
 * State ENextPart: Moves the interator to point to the next part of the multipart
 * when there are no more parts, EDone will be the next state
 * State EDone: Clean up and complete
 */
void CMultiPartMixedContentHandler::RunL()
	{
	switch(iState)
		{
		case ELoadMsgData:
			LoadMultipartMsgL();
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
TInt CMultiPartMixedContentHandler::RunError(TInt aError)
	{
	__LOG_PTR_DEBUG("CMultiPartMixedContentHandler::RunError Called"); 
	iState=EDone;
	Complete(aError);
	return KErrNone;
	}


/**
 *	Handles the Message Part returned from the Multipart iterator
 *  by creating a new handler based on the Content-Type, then dispatches it
 *  synchronously.
 */
void CMultiPartMixedContentHandler::HandlePartL()
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: HandlePart"); 
	CPushMessage* msgPart = iMultiMessage->PartL();
	CleanupStack::PushL(msgPart);
	
	TPtrC contentType;
	msgPart->GetContentType(contentType);
	CContentHandlerBase& contentHandler = PushContentTypeDispatcher::GetHandlerL(contentType, *iLog, *iManager);
	iContentHandler = &contentHandler;
	CleanupStack::Pop(msgPart);

	// treat as an acknowledged message always to ensure 
	// sequential creation of handlers
	iContentHandler->HandleMessageL(msgPart, iStatus);
	iState = ENextPart;
	SetActive();
	}

/**
 *	Moves multipart iterator to the next part. If there is none,
 *  State: EDone will be set to complete this handler
 */
void CMultiPartMixedContentHandler::NextPartL()
	{
	 __LOG_PTR_DEBUG("CMultiPartMixedContentHandler:: NextPartL");
	if (iMultiMessage->NextL())
		iState = EHandlePart;
	else 
		iState = EDone;
	
	IdleComplete();
	}
