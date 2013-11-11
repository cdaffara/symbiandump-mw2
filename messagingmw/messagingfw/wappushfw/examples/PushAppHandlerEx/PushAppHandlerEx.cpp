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
// CMMSAppHandler.cpp
// 
//

#include <push/ccontenthandlerbase.h>
#include "pushdispatcher.h"
#include "PushAppHandlerEx.h"


_LIT(KReserved, "Reserved");

void CPushAppHandlerEx::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CPushAppHandlerEx::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

CPushAppHandlerEx::CPushAppHandlerEx()
: CPushHandlerBase()
	{
	CActiveScheduler::Add(this);
	}

void CPushAppHandlerEx::ConstructL()
	{
	// TODO Add any more required construction to class
	}

CPushAppHandlerEx* CPushAppHandlerEx::NewL()
	{
	CPushAppHandlerEx* self = new (ELeave) CPushAppHandlerEx();
	self->ConstructL();
	return self;
	}

CPushAppHandlerEx::~CPushAppHandlerEx()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: Destructor Called"); 
	// TODO Add any more required destruction to class
	delete iMessage;
	}

void CPushAppHandlerEx::IdleComplete()
	{
	TRequestStatus* pS = &iStatus;
	User::RequestComplete(pS,KErrNone);
	SetActive();
	}

void CPushAppHandlerEx::Done(TInt aError)
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: EDone");
	if (iAsyncHandling)
		SignalConfirmationStatus(aError);
	// Time to commit suicide
	iPluginKiller->KillPushPlugin();
	}


void CPushAppHandlerEx::HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus)
	{
	 __LOG_PTR_DEBUG("CPushAppHandlerEx:: HandleMessageL Async Func. Called"); 
	 
	iAsyncHandling = ETrue;
	SetConfirmationStatus(aStatus);
	HandleMessageL(aPushMsg);
	}


void CPushAppHandlerEx::HandleMessageL(CPushMessage* aPushMsg)
	{
	 __LOG_PTR_DEBUG("CPushAppHandlerEx:: HandleMessageL Sync Func. Called"); 	
	
	iMessage = aPushMsg;
	iState = EParsing;
	IdleComplete();
	}


void CPushAppHandlerEx::CancelHandleMessage()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: CancelHandleMessage Called");
	// TODO Add Cancel code for any outstanding requests
	Done(KErrCancel);
	}


void CPushAppHandlerEx::DoCancel()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: DoCancel Called");
	// TODO Add Cancel code for any outstanding requests
	Done(KErrCancel);
	}


void CPushAppHandlerEx::RunL()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: RunL Called");

	// TODO Create a State Machine for specific actions
	switch (iState)
		{
	case EParsing :
		iState = EProcessing;
		ParsePushMsgL();
		break;
	case EProcessing:
		iState = EDone;
		ProcessPushMsgL();
		break;
	case EDone:
		Done(KErrNone);
		break;
	default:
		break;
		}
	}

void CPushAppHandlerEx::ParsePushMsgL()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: ParsePushMsgL Called");
	IdleComplete();
	}


void CPushAppHandlerEx::ProcessPushMsgL()
	{
	__LOG_PTR_DEBUG("CPushAppHandlerEx:: ProcessPushMsgL Called");
	IdleComplete();
	}



