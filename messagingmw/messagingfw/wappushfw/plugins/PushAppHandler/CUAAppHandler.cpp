// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Local includes
//
#include "CUAAppHandler.h"

// System includes
//
#include <push/pushlog.h>
#include <push/pushmessage.h>
#include <push/pluginkiller.h>
#include "PushDispatcher.h"
#include <push/ccontenthandlerbase.h>

// Constants
_LIT(KReserved, "Reserved");

void CUAAppHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CUAAppHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/** CUAAppHandler Constructor.
 * 
 */ 
CUAAppHandler::CUAAppHandler()
: CPushHandlerBase()
	{
	CActiveScheduler::Add(this);
	}

/** CUAAppHandler Static Factory Construction.
 * 
 */
CUAAppHandler* CUAAppHandler::NewL()
	{
	CUAAppHandler* self = new (ELeave) CUAAppHandler;
	return self;
	}

/**
 * Default Destructor.
 */
CUAAppHandler::~CUAAppHandler()
	{
	__LOG_PTR_DEBUG("CUAAppHandler:: Destructor Called"); 
	}

/** Asynchronous Handling of Push Message.
 *
 *	@param aPushMsg  the received push message
 *	@param aStatus Status to be signalled when handling is complete
 */
void CUAAppHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	 __LOG_PTR_DEBUG("CUAAppHandler:: HandleMessageL Async Func. Called"); 	

	SetConfirmationStatus(aStatus);

	TPtrC contentType;
	aPushMsg->GetContentType(contentType);
	CContentHandlerBase& contentHandlerPi = PushContentTypeDispatcher::GetHandlerL(contentType, *iLog, *iManager);
	iContentHandlerPi = &contentHandlerPi;
	iContentHandlerPi->HandleMessageL(aPushMsg, iStatus);
	SetActive();
	}

/**
 * Cancel any outstanding requests.
 */
void CUAAppHandler::CancelHandleMessage()
	{
	if(*iConfirmStatus==KRequestPending)
		{
		iContentHandlerPi->CancelHandleMessage();
		}
	}

/**
 *  HandleMessageL - Sync. Version
 *
 * 1. Get Content-Type from Msg
 * 2. Load the specific Content handler
 * 3. Pass the Push message to Content the handler
 * 4. Invoke the Killer object to delete the Plugin Owner
 *
 * @param aPushMsg the Push Message to be handled
 */
void CUAAppHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	 __LOG_PTR_DEBUG("CUAAppHandler:: HandleMessageL Sync Func. Called"); 	

	TPtrC contentType;
	aPushMsg->GetContentType(contentType);
	TRAPD( error, 
		CleanupStack::PushL(aPushMsg);
		CContentHandlerBase& contentHandlerPi = PushContentTypeDispatcher::GetHandlerL(contentType, *iLog, *iManager);
		CleanupStack::Pop(aPushMsg);
		iContentHandlerPi = &contentHandlerPi;
		);

	if( error == KErrNone )
		{
		TRAP( error, 
			iContentHandlerPi->HandleMessageL(aPushMsg));
		}
	iPluginKiller->KillPushPlugin();
	}

/**
 * Cancel the handling of the message.
 * No Object need to be canceled in the case of CUnknownAppHandler.
 */
void CUAAppHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CUAAppHandler:: DoCancel Called");
	}

/** CUAAppHandler State machine.
 *
 * Complete Active Object Returning the comfirmation status
 * and Suicide.
 */
void CUAAppHandler::RunL()
	{
	__LOG_PTR_DEBUG("CUAAppHandler:: RunL Called");
	SignalConfirmationStatus(KErrNone);
	iPluginKiller->KillPushPlugin();
	}
