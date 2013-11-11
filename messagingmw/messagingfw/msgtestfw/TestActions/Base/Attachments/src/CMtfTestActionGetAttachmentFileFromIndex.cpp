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
// GetAttachmentFileFromIndex
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// Index		  <input>: Index value of attachment to retrieve.
// DataFilePath   <input>: (optional) File path of data to compare attachment to.
// ExpectedError  <input>: (optional) Expected error code to compare against.
// [Action Description]
// Gets the attachment file from the specified 
// [APIs Used]
// CMsvSession::GetEntryL
// CMsvStore::ReadStoreL
// CMsvStore::AttachmentManagerL
// TODO
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionGetAttachmentFileFromIndex.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>

CMtfTestAction* CMtfTestActionGetAttachmentFileFromIndex::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetAttachmentFileFromIndex* self = new(ELeave) CMtfTestActionGetAttachmentFileFromIndex(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetAttachmentFileFromIndex::CMtfTestActionGetAttachmentFileFromIndex(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetAttachmentFileFromIndex::~CMtfTestActionGetAttachmentFileFromIndex()
	{
	}

void CMtfTestActionGetAttachmentFileFromIndex::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetAttachmentFileFromIndex);
	TInt expectedError = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(4), KErrNone);

	if( expectedError != KErrNone )
		{
		// error checking is requested
		TRAPD(err, RunTestActionL());
		if( expectedError != err )
			{
			// not the expected error code
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		// no error expected
		RunTestActionL();
		}

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetAttachmentFileFromIndex);
	TestCase().ActionCompletedL(*this);
	}
	
void CMtfTestActionGetAttachmentFileFromIndex::RunTestActionL()
	{
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TInt attachIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));
	HBufC* dataFilePath   = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(3), NULL);

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CMsvStore* store = entry->ReadStoreL();
	CleanupStack::PushL(store);
	
	MMsvAttachmentManager& manager = store->AttachmentManagerL();
	
	RFile fileAttachment = manager.GetAttachmentFileL(attachIndex);
	CleanupClosePushL(fileAttachment);
	
	if( dataFilePath != NULL )
		{
		// check of contents of attachment file is requested
		CompareFileL(fileAttachment, *dataFilePath);
		}
	
	CleanupStack::PopAndDestroy(3, entry); // fileAttachment, store, entry
	}

void CMtfTestActionGetAttachmentFileFromIndex::CompareFileL(RFile& aAttachment, const TDesC& aDataFilePath)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	RFile dataFile;
	User::LeaveIfError(dataFile.Open(fs, aDataFilePath, EFileShareReadersOnly));
	CleanupClosePushL(dataFile);
		
	TInt fileSize = 0;
	
	User::LeaveIfError(dataFile.Size(fileSize));
	HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
	TPtr8 bufferPtr(dataBuffer->Des());
	
	User::LeaveIfError(aAttachment.Size(fileSize));
	HBufC8* attachmentData = HBufC8::NewLC(fileSize);
	TPtr8 attachmentPtr(attachmentData->Des());
	
	TInt compare = attachmentPtr.Compare(bufferPtr);
	if( compare != 0 )
		User::Leave(KErrCorrupt);
	
	dataFile.Close();
	CleanupStack::PopAndDestroy(4, &fs); // attachmentData, dataBuffer, dataFile, fs
	}
	
