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
#include "CMtfTestActionSendAsSendMessageNew.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

CMtfTestAction* CMtfTestActionSendAsSendMessageNew::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSendMessageNew* self = new (ELeave) CMtfTestActionSendAsSendMessageNew(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSendAsSendMessageNew::CMtfTestActionSendAsSendMessageNew(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionSendAsSendMessageNew::~CMtfTestActionSendAsSendMessageNew()
	{
	}


void CMtfTestActionSendAsSendMessageNew::ExecuteActionL()
	{	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSendMessageNew);
	iSendAsMessage  = ObtainValueParameterL<RSendAsMessage>(TestCase(),ActionParameters().Parameter(0));
	TInt	paramConfirmedSend = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1), EFalse);
	TInt	paramCancelSend = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2), EFalse);
	TInt	paramSetCharacterSet = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4), EFalse);
	
	CActiveScheduler::Add (this);
	
	TInt	err = KErrNone;
	
	// Sets the characterset for a message,it could be 8 bit or 16 bit (UCS2) Unicode value
	if(paramSetCharacterSet == TSmsDataCodingScheme::ESmsAlphabet8Bit || paramSetCharacterSet == TSmsDataCodingScheme::ESmsAlphabetUCS2 || paramSetCharacterSet == TSmsDataCodingScheme::ESmsAlphabet7Bit)
		{
		TRAP(err, iSendAsMessage.SetCharacterSetL(paramSetCharacterSet));
		TestCase().INFO_PRINTF2(_L("SetCharacterSetL completed with error %d"), err);
		}
		
	if(err == -47)
		{
		iStatus = KRequestPending;
		TRequestStatus *status = &iStatus;
		User::RequestComplete(status, err);
		}
	else
		{
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
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSendMessageNew);
	SetActive();
	}

void CMtfTestActionSendAsSendMessageNew::DoCancel()
	{
	iSendAsMessage.Cancel();
	}

void CMtfTestActionSendAsSendMessageNew::RunL()
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

	if(ActionParameters().Count() == 5 || ActionParameters().Count() == 4)
		{
		StoreParameterL<TInt>(TestCase(),errorCode,ActionParameters().Parameter(3));
		}
	else
		{
		User::LeaveIfError(errorCode);
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSendMessageNew);
	TestCase().ActionCompletedL(*this);
	}

