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
// GetAttachmentInfoByIndex
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// Index	      <input>: Index value of the attachment.
// AttachmentInfo <output>: Reference to the created attachment info
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Creates an attachment info object from the specified index for the
// supplied message entry. Will fail if the entry or attachment does
// not exist.
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// MMsvAttachmentManager::GetAttachmentInfoL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionGetAttachmentInfoByIndex.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <cmsvattachment.h>
#include <mmsvattachmentmanager.h>

const TInt KMimeTypeLength = 50;

CMtfTestAction* CMtfTestActionGetAttachmentInfoByIndex::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetAttachmentInfoByIndex* self = new(ELeave) CMtfTestActionGetAttachmentInfoByIndex(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetAttachmentInfoByIndex::CMtfTestActionGetAttachmentInfoByIndex(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetAttachmentInfoByIndex::~CMtfTestActionGetAttachmentInfoByIndex()
	{
	}

void CMtfTestActionGetAttachmentInfoByIndex::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetAttachmentInfoByIndex);
	TRAPD(err, RunTestL());
	
	if( ActionParameters().Count() < 5 )
		{
		User::LeaveIfError(err);
		}
	else
		{
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(4));
		}
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetAttachmentInfoByIndex);
	TestCase().ActionCompletedL(*this); 
	}

void CMtfTestActionGetAttachmentInfoByIndex::RunTestL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt attachIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	CMsvAttachment* attachInfo = manager.GetAttachmentInfoL(attachIndex);
	CleanupStack::PushL(attachInfo);
	
	PrintAttachmentInfo(*attachInfo);
	
	StoreParameterL<CMsvAttachment>(TestCase(),*attachInfo,ActionParameters().Parameter(3));
	CleanupStack::Pop(attachInfo);
	CleanupStack::PopAndDestroy(2, entry); // store, entry
	}

void CMtfTestActionGetAttachmentInfoByIndex::PrintAttachmentInfo(CMsvAttachment& aInfo)
	{
	TestCase().INFO_PRINTF1(_L("CMsvAttachment Info..."));
	TestCase().INFO_PRINTF2(_L("ID - %d"), aInfo.Id() );
	switch(aInfo.Type())
		{
		case CMsvAttachment::EMsvFile:
			TestCase().INFO_PRINTF1(_L("Type - EMsvFile"));
			TestCase().INFO_PRINTF2(_L("FilePath - %S"), &aInfo.FilePath());
			break;
		case CMsvAttachment::EMsvLinkedFile:
			TestCase().INFO_PRINTF1(_L("Type - EMsvLinkedFile"));
			TestCase().INFO_PRINTF2(_L("FilePath - %S"), &aInfo.FilePath());
			break;
		case CMsvAttachment::EMsvMessageEntry:
			TestCase().INFO_PRINTF1(_L("Type - EMsvMessageEntry"));
			TestCase().INFO_PRINTF2(_L("EntryId - %d"), aInfo.EntryAttachmentId());
			break;
		default:
			User::Invariant();
			break;
		}
	TestCase().INFO_PRINTF2(_L("AttachmentName - %S"), &aInfo.AttachmentName());
	TestCase().INFO_PRINTF2(_L("Size - %d"), aInfo.Size());
	TBuf<KMimeTypeLength> mimeType;
	mimeType.Copy(aInfo.MimeType());
	TestCase().INFO_PRINTF2(_L("MimeType - %S"), &mimeType);
	}
