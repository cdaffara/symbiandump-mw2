// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SendAsSendMessage
// [Action Parameters]
// RSendAsMessage	paramSendAsMessage			<input>:	Reference to  the RSendAsMessage object
// TInt				paramSendMessageConfirmed	<input>:	0 if the send operation is not a confirmed operation, 1 if the send operation has to be a confirmed operation.  Default value is 0.
// TInt				paramCancelSendMessage		<input>:	0 if the send operation need not be canceled, 1 if the send operation has to be canceled.  Default value is 0.
// [Action Description]
// SendAsSendMessage Test Action is intended to request the Send-As server to send the message.
// [APIs Used]
// RSendAsMessage::SendMessage (TRequestStatus& aStatus)
// RSendAsMessage::SendMessageConfirmed (TRequestStatus& aStatus)
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "sendas2.h"
#include "CMtfTestActionSendAsSendMessage.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

CMtfTestAction* CMtfTestActionSendAsSendMessage::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSendMessage* self = new (ELeave) CMtfTestActionSendAsSendMessage(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSendAsSendMessage::CMtfTestActionSendAsSendMessage(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionSendAsSendMessage::~CMtfTestActionSendAsSendMessage()
	{
	}


void CMtfTestActionSendAsSendMessage::ExecuteActionL()
	{	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSendMessage);
	iSendAsMessage  = ObtainValueParameterL<RSendAsMessage>(TestCase(),ActionParameters().Parameter(0));
	TInt	paramConfirmedSend = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1), EFalse);
	TInt	paramCancelSend = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), EFalse);
	
	CActiveScheduler::Add (this);
	
	TInt	err = KErrNone;
	
	if(paramConfirmedSend)
		{
		TRAP(err, iSendAsMessage.SendMessageConfirmed(iStatus));
		}
	else
		{
		TRAP(err, iSendAsMessage.SendMessage(iStatus));
		}

	if(paramCancelSend)
		{
		TRAP(err, iSendAsMessage.Cancel());
		}
	
	SetActive();
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSendMessage);
	}

void CMtfTestActionSendAsSendMessage::DoCancel()
	{
	iSendAsMessage.Cancel();
	}

void CMtfTestActionSendAsSendMessage::RunL()
	{
	TSendAsProgress		sendAsProgress;
	TInt	errorCode = KErrNone;
		
	if (iStatus == KErrNone)
		{
		iSendAsMessage.ProgressL(sendAsProgress);
		errorCode = sendAsProgress.iError;
		}
	else
		{
		errorCode = iStatus.Int();
		}

	if(ActionParameters().Count() == 4)
		{
		StoreParameterL<TInt>(TestCase(),errorCode,ActionParameters().Parameter(3));
		}
	else
		{
		User::LeaveIfError(errorCode);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSendMessage);
	TestCase().ActionCompletedL(*this);
	}

