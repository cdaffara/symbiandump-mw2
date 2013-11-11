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
// AddFileAttachmentByPath
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
// CMsvStore::EditStoreL
// CMsvStore::AttachmentManagerL
// CMsvAttachment::NewL
// CMsvAttachment::SetMimeType
// MMsvAttachmentManager::AddAttachmentL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionAddFileAttachmentByPath.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionAddFileAttachmentByPath::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddFileAttachmentByPath* self = new(ELeave) CMtfTestActionAddFileAttachmentByPath(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAddFileAttachmentByPath::CMtfTestActionAddFileAttachmentByPath(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionAddFileAttachmentByPath::~CMtfTestActionAddFileAttachmentByPath()
	{
	Cancel();
	delete iAttachment;
	delete iStore;
	delete iEntry;
	}

void CMtfTestActionAddFileAttachmentByPath::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddFileAttachmentByPath);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 6 )
		{
		User::LeaveIfError(err);
		}
	else if( err!=KErrNone )
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));
		delete iStore;
		iStore = NULL;
		delete iEntry;
		iEntry = NULL;
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddFileAttachmentByPath);
		TestCase().ActionCompletedL(*this);
		}
	}

void CMtfTestActionAddFileAttachmentByPath::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));
	HBufC8* paramMimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(3));

	iEntry = paramSession->GetEntryL(messageEntry);
	iStore = iEntry->EditStoreL();
	iAttachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	
	iAttachment->SetMimeTypeL(*paramMimeType);
	TEntry fileEntry;
	User::LeaveIfError(paramSession->FileSession().Entry(*paramFilePath, fileEntry));
	iAttachment->SetSize(fileEntry.iSize);
	
	TParse fileNameParser;
	User::LeaveIfError(fileNameParser.Set(*paramFilePath, NULL, NULL));
	iAttachment->SetAttachmentNameL(fileNameParser.NameAndExt());
	
	MMsvAttachmentManager& manager = iStore->AttachmentManagerL();
	
	CActiveScheduler::Add(this);
	manager.AddAttachmentL(*paramFilePath, iAttachment, iStatus);
	SetActive();
	iAttachmentId = iAttachment->Id();
	iAttachment = NULL; // ownership passed to manager
	}

void CMtfTestActionAddFileAttachmentByPath::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	iStore->CommitL();
	
	delete iStore;
	iStore = NULL;
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

void CMtfTestActionAddFileAttachmentByPath::DoCancel()
	{
	iStore->AttachmentManagerL().CancelRequest();
	}

TInt CMtfTestActionAddFileAttachmentByPath::RunError(TInt aError)
	{
	if(ActionParameters().Count() == 6)
		{
		StoreParameterL<TInt>(TestCase(),aError,ActionParameters().Parameter(5));
		}
	delete iStore;
	iStore = NULL;
	delete iEntry;
	iEntry = NULL;
	TRAPD(err, TestCase().ActionCompletedL(*this));
	return err;
	}
	
