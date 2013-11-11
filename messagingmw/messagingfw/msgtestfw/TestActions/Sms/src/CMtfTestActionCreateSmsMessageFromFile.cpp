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
// CreateSmsMessageFromFile
// [Action Parameters]
// CMsvSession			SessionId	 <input>: Reference to the session  
// TMsvId				ServiceId	 <input>: Value of the SMS service id
// TMsvId				FolderId	 <input>: Id of Folder path where the message is to be created
// TInt				    FileIndex	 <input>: Value of index for SMS data file. Default is 0
// CMsvEntrySelection	IdSelection <output-initiation>: ID of the created Sms Message 
// TMsvID				SmsMessageId <output-initiation>: ID of the created Sms Message 
// [Action Description]
// Reads the information required for creating SMS message from  the Sms Data File,
// creates a SMS message and stores it in the folder FolderId which is provided as input 
// to the test action.
// The test action currently supports creation of one message using the data file.  Future
// enhancements will be done to support creation of multiple Sms messages from the data file.
// Currently, only one recipient address is supported while creating the SMS.  Further enhancements
// will support multiple recipient addresses.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include <msvapi.h>
#include <gsmuelem.h>
#include <smuthdr.h>
#include <smutset.h>
#include <e32def.h>
#include <e32std.h>
#include <msvuids.h>
#include <txtfmlyr.h>
#include <txtrich.h>

	#include <csmsaccount.h>

#include "CMtfTestActionCreateSmsMessageFromFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"
#include "CMtfTestActionUtilsSmsScripts.h"



CMtfTestAction* CMtfTestActionCreateSmsMessageFromFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCreateSmsMessageFromFile* self = new (ELeave) CMtfTestActionCreateSmsMessageFromFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCreateSmsMessageFromFile::CMtfTestActionCreateSmsMessageFromFile(CMtfTestCase& aTestCase)
	:CMtfSynchronousTestAction(aTestCase)
	{
	}
	

CMtfTestActionCreateSmsMessageFromFile::~CMtfTestActionCreateSmsMessageFromFile()
	{
	}
	

void CMtfTestActionCreateSmsMessageFromFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCreateSmsMessageFromFile);
	CMsvSession* paramSession   = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId       paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvId       paramFolderId  = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(2));
	TInt         paramFileIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3),0); //Default value is 0

	// Check if the input paramFolderId is a local folder, leave otherwise
	if(!(paramFolderId == KMsvLocalServiceIndexEntryId ||
	     paramFolderId == KMsvGlobalInBoxIndexEntryId  || 
	     paramFolderId == KMsvGlobalOutBoxIndexEntryId || 
	     paramFolderId == KMsvDraftEntryId))
		{
		User::Leave(KErrNotFound); // Check on changing the error retured value
		}

	const TPtrC smsDataFile = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfSmsMessage,paramFileIndex);

	TMsvEntry indexEntry;
	indexEntry.iType      = KUidMsvMessageEntry;
	indexEntry.iMtm       = KUidMsgTypeSMS;
	indexEntry.iServiceId = paramServiceId;
	
	// Get Message Body
	TPtrC messageBodyPtr;
	User::LeaveIfError(CMtfTestActionUtilsSmsScripts::GetSmsMessageBodyL(smsDataFile,messageBodyPtr));

	if(messageBodyPtr.Length() <=0) 
		{
		User::Leave(KErrGeneral);
		}	
	
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);
	CRichText* bodyText = CRichText::NewL(paraFormatLayer, charFormatLayer);
	CleanupStack::PushL(bodyText);
	bodyText->InsertL(0,messageBodyPtr);
	CleanupStack::Pop(3,paraFormatLayer); 

	// Create Sms Header
	CSmsHeader* smsHeader = CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*bodyText);
	CleanupStack::PushL(smsHeader);
	User::LeaveIfError(CMtfTestActionUtilsSmsScripts::SetSmsHeaderInfoFromConfigurationFileL(TestCase(), smsDataFile,*smsHeader));

	// Create Sms Settings
	CSmsSettings* smsSettings = CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	
	// Create a CMsvEntry pointing at the SMS service
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession, paramServiceId, TMsvSelectionOrdering());
	CleanupStack::PushL(entry);


	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	smsAccount->LoadSettingsL(*smsSettings);
	CleanupStack::PopAndDestroy(smsAccount);
	

	// Override the setting with any values that might be specified for the Sms Message
	CMtfTestActionUtilsSmsScripts::ReadSmsSettingsFromConfigurationFileL(TestCase(),smsDataFile,*smsSettings);
	smsHeader->SetSmsSettingsL(*smsSettings);

	entry->SetEntryL(paramFolderId);
	entry->CreateL(indexEntry);

	TMsvId smsMessageId = indexEntry.Id();
	entry->SetEntryL(smsMessageId);

	CMsvStore* store = entry->EditStoreL();
	CleanupStack::PushL(store);
	store->StoreBodyTextL(*bodyText);
	
	smsHeader->StoreL(*store);
	store->CommitL();
	CleanupStack::PopAndDestroy(store);

	CMsvEntrySelection* smsIds = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(smsIds);
	smsIds->AppendL(smsMessageId);
	
	StoreParameterL<CMsvEntrySelection>(TestCase(),*smsIds,ActionParameters().Parameter(4));
	StoreParameterL<TMsvId>(TestCase(),smsMessageId,ActionParameters().Parameter(5));

	CleanupStack::Pop(4);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCreateSmsMessageFromFile);
	TestCase().ActionCompletedL(*this);
	}
