// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SmtpAddFileAttachmentByPath
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// FilePath       <input>: File path of the attachment file.
// MimeType		  <input>: Mime-type of the attachment.
// AttachmentId   <output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Creates a file attachment on the specified message entry located using a specified file.
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::AddAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpAddFileAttachmentByPath.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <miutmsg.h>

CMtfTestAction* CMtfTestActionSmtpAddFileAttachmentByPath::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpAddFileAttachmentByPath* self = new(ELeave) CMtfTestActionSmtpAddFileAttachmentByPath(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpAddFileAttachmentByPath::CMtfTestActionSmtpAddFileAttachmentByPath(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpAddFileAttachmentByPath::~CMtfTestActionSmtpAddFileAttachmentByPath()
	{
	Cancel();	
	delete iAttachment;
	delete imEmailMsg;
	delete iEntry;
	}

void CMtfTestActionSmtpAddFileAttachmentByPath::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpAddFileAttachmentByPath);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 6 )
		{
		User::LeaveIfError(err);
		}
	else if( err!=KErrNone )
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));		
		delete imEmailMsg;
		imEmailMsg = NULL;
		delete iEntry;
		iEntry = NULL;		
		TestCase().ActionCompletedL(*this);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpAddFileAttachmentByPath);
	}

void CMtfTestActionSmtpAddFileAttachmentByPath::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));
	HBufC8* paramMimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(3));

	iEntry = paramSession->GetEntryL(messageEntry);
	imEmailMsg = CImEmailMessage::NewL(*iEntry);	
	
	iAttachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);

	iAttachment->SetMimeTypeL(*paramMimeType);
	
	TEntry fileEntry;
	User::LeaveIfError(paramSession->FileSession().Entry(*paramFilePath, fileEntry));
	iAttachment->SetSize(fileEntry.iSize);

	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	iAttachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	
	MMsvAttachmentManager& manager = imEmailMsg->AttachmentManager();
	
	CActiveScheduler::Add(this);	
	manager.AddAttachmentL(*paramFilePath, iAttachment, iStatus);
	SetActive();
	
	iAttachment = NULL; // ownership passed to manager
	
	}

void CMtfTestActionSmtpAddFileAttachmentByPath::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	MMsvAttachmentManager& manager = imEmailMsg->AttachmentManager();
	
	TInt attachmentCount = manager.AttachmentCount();
	CMsvAttachment* attachmentInfo = manager.GetAttachmentInfoL(attachmentCount - 1);
	CleanupStack::PushL(attachmentInfo);
	
	iAttachmentId = attachmentInfo->Id();
	CleanupStack::PopAndDestroy(attachmentInfo);
	
	delete imEmailMsg;
	imEmailMsg = NULL;
	delete iEntry;
	iEntry = NULL;
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),iAttachmentId,ActionParameters().Parameter(4));
	if( ActionParameters().Count() == 6 )
		{
		TInt err = KErrNone;
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));
		}

	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionSmtpAddFileAttachmentByPath::DoCancel()
	{
	imEmailMsg->AttachmentManager().CancelRequest();
	}

TInt CMtfTestActionSmtpAddFileAttachmentByPath::RunError(TInt aError)
	{
	if(ActionParameters().Count() == 6)
		{
		StoreParameterL<TInt>(TestCase(),aError,ActionParameters().Parameter(5));
		}
	
	delete imEmailMsg;
	imEmailMsg = NULL;
	delete iEntry;
	iEntry = NULL;
	
	TRAPD(r,TestCase().ActionCompletedL(*this));
		
	return r;
	}
	
