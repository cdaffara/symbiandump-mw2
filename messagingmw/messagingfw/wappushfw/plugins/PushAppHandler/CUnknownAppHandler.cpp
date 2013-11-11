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
#include "CUnknownAppHandler.h"

// System includes
//
#include <push/pushlog.h>
#include <push/pushmessage.h>
#include <push/pluginkiller.h>

// Constants
_LIT(KReserved, "Reserved");

void CUnknownAppHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CUnknownAppHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

/** CUnknownAppHandler Constructor.
 * 
 */ 
CUnknownAppHandler::CUnknownAppHandler()
: CPushHandlerBase()
	{
	}

/** CUnknownAppHandler Static Factory Construction.
 * 
 */
CUnknownAppHandler* CUnknownAppHandler::NewL()
	{
	CUnknownAppHandler* self = new (ELeave) CUnknownAppHandler;
	return self;
	}

/**
 * Destructor.
 */
CUnknownAppHandler::~CUnknownAppHandler()
	{
	__LOG_PTR_DEBUG("CUnknownAppHandler:: Destructor Called"); 
	}

/** Asynchronous Handle Message 
 *
 * Push Messages with an unknown AppId are deleted. No further
 * action is required.
 *
 * @param aPushMsg The Push Message with Unknown AppId
 * @param aStatus Status to be signalled when handling is complete
 */
void CUnknownAppHandler::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	 __LOG_PTR_DEBUG("CUnknownAppHandler:: HandleMessageL Async. Func. Called"); 
	delete aPushMsg;

	SetConfirmationStatus(aStatus);
	SignalConfirmationStatus(KErrNone);
	iPluginKiller->KillPushPlugin();
	}

/** Cancelling any Outstanding Request.
 *
 * No Requests need to be canceled in the case of CUnknownAppHandler
 */
void CUnknownAppHandler::CancelHandleMessage()
	{
	}

/** Synchronous Handle Message.
 * 
 * Push Messages with an unknown AppId are deleted in a similar
 * way as for the asynchronous version.
 *
 * @param aPushMsg The Push Message with Unknown AppId
 */
void CUnknownAppHandler::HandleMessageL(CPushMessage* aPushMsg)
	{
	 __LOG_PTR_DEBUG("CUnknownAppHandler:: HandleMessageL Sync. Func. Called"); 
	delete aPushMsg;
	iPluginKiller->KillPushPlugin();
	}

/**
 * Cancel any Outstanding Requests.
 */
void CUnknownAppHandler::DoCancel()
	{
	__LOG_PTR_DEBUG("CUnknownAppHandler:: DoCancel Called"); 
	}

/**
 * Handle the completion of an aysynchronous request
 * This should never be called, since no requests are made.
 */
void CUnknownAppHandler::RunL()
	{
	__LOG_PTR_DEBUG("CUnknownAppHandler:: RunL Called"); 
	}
