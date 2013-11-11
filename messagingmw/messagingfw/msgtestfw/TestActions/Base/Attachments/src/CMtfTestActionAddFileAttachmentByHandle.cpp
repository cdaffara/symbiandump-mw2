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
// AddFileAttachmentByHandle
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// FilePath       <input>: File path of the attachment file to open as a handle.
// MimeType		  <input>: Mime-type of the attachment.
// AttachmentId   <output>: Value of the created attachment id.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Opens the specified file and adds the file to the message entry by the open
// file handle.
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


#include "CMtfTestActionAddFileAttachmentByHandle.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionAddFileAttachmentByHandle::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionAddFileAttachmentByHandle* self = new(ELeave) CMtfTestActionAddFileAttachmentByHandle(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionAddFileAttachmentByHandle::CMtfTestActionAddFileAttachmentByHandle(CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase)
	{
	}


CMtfTestActionAddFileAttachmentByHandle::~CMtfTestActionAddFileAttachmentByHandle()
	{
	Cancel();
	delete iAttachment;
	delete iStore;
	delete iEntry;
	}

void CMtfTestActionAddFileAttachmentByHandle::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddFileAttachmentByHandle);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 6 )
		{
		User::LeaveIfError(err);
		}
	else if( err!=KErrNone )
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddFileAttachmentByHandle);
		TestCase().ActionCompletedL(*this);
		}
	}
	
void CMtfTestActionAddFileAttachmentByHandle::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));
	HBufC8* paramMimeType = ObtainParameterReferenceL<HBufC8>(TestCase(),ActionParameters().Parameter(3));

	iEntry = paramSession->GetEntryL(messageEntry);
	iStore = iEntry->EditStoreL();
	iAttachment = CMsvAttachment::NewL(CMsvAttachment::EMsvFile);
	iAttachment->SetMimeTypeL(*paramMimeType);
	
	RFile fileHandle = OpenFileL(paramSession->FileSession(), *paramFilePath);
	CleanupClosePushL(fileHandle);
	TFileName fileName;
	User::LeaveIfError(fileHandle.Name(fileName));
	iAttachment->SetAttachmentNameL(fileName);
	TInt fileSize = 0;
	User::LeaveIfError(fileHandle.Size(fileSize));
	iAttachment->SetSize(fileSize);
	
	MMsvAttachmentManager& manager = iStore->AttachmentManagerL();
	CActiveScheduler::Add(this);
	SetActive();
	manager.AddAttachmentL(fileHandle, iAttachment, iStatus);
	iAttachmentId = iAttachment->Id();
	iAttachment = NULL; // ownership passed to manager
	
	CleanupStack::Pop(&fileHandle);
	}

void CMtfTestActionAddFileAttachmentByHandle::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	iStore->CommitL();
	
	delete iStore;
	iStore = NULL;
	delete iEntry;
	iEntry = NULL;
	
	StoreParameterL<TMsvAttachmentId>(TestCase(),iAttachmentId,ActionParameters().Parameter(4));
	if(ActionParameters().Count() == 6)
		{
		TInt err = KErrNone;
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(5));
		}
		
	TestCase().ActionCompletedL(*this);
	}

void CMtfTestActionAddFileAttachmentByHandle::DoCancel()
	{
	iStore->AttachmentManagerL().CancelRequest();
	}

TInt CMtfTestActionAddFileAttachmentByHandle::RunError(TInt aError)
	{
	StoreParameterL<TInt>(TestCase(),aError,ActionParameters().Parameter(5));
	TRAPD(err, TestCase().ActionCompletedL(*this));
	return err;
	}

RFile CMtfTestActionAddFileAttachmentByHandle::OpenFileL(RFs& aFs, const TDesC& aFilePath)
	{
	RFile fileHandle;
	User::LeaveIfError(fileHandle.Open(aFs, aFilePath, EFileRead));
	return fileHandle;
	}

