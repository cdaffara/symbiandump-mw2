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

// Local includes
//
#include "CROAppHandler.h"


// System includes
//
#include <push/pushlog.h>
#include <push/pushmessage.h>
#include <push/pluginkiller.h>
#include "PushDispatcher.h"
#include <push/ccontenthandlerbase.h>


// Constants
_LIT(KReserved, "Reserved");

void CROAppHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CROAppHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/** CROAppHandler Constructor.
 * 
 */ 
CROAppHandler::CROAppHandler()
: CPushHandlerBase()
	{
	CActiveScheduler::Add(this);
	}

/** CROAppHandler Static Factory Construction.
 * 
 */
CROAppHandler* CROAppHandler::NewL()
	{
	CROAppHandler* self = new (ELeave) CROAppHandler;
	return self;
	}

/**
 * Default Destructor.
 */
CROAppHandler::~CROAppHandler()
	{
	__LOG_PTR_DEBUG("CROAppHandler:: Destructor Called"); 
	}

/** Asynchronous Handling of Push Message.
 *
 *	@param aPushMsg  the received push message
 *	@param aStatus Status to be signalled when handling is complete
 */
void CROAppHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	 __LOG_PTR_DEBUG("CROAppHandler:: HandleMessageL Async Func. Called"); 	

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
void CROAppHandler::CancelHandleMessage()
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
void CROAppHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	 __LOG_PTR_DEBUG("CROAppHandler:: HandleMessageL Sync Func. Called"); 	

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
void CROAppHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CROAppHandler:: DoCancel Called");
	}

/** CROAppHandler State machine.
 *
 * Complete Active Object Returning the comfirmation status
 * and Suicide.
 */
void CROAppHandler::RunL()
	{
	__LOG_PTR_DEBUG("CROAppHandler:: RunL Called");
	SignalConfirmationStatus(KErrNone);
	iPluginKiller->KillPushPlugin();
	}
