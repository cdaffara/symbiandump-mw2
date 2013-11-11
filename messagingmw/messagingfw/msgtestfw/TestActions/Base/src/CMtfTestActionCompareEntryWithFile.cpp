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
// CreateEmailOverSmsMessage ????? 
// [Action Parameters]
// Session        <input>: Reference to the session. ????
// ParentId       <input>: Value of the parent id.
// ServiceId      <input>: Value of the the service id.
// (ConfigIndexHeader)	  <input>: Value of config index for SMS header settings. Default is 0.
// MessageId     <output>: Value of the created message id.
// [Action Description]
// Creates a message on the specified parent.
// [APIs Used]	
// __ACTION_INFO_END__
// 
//

// 
#include "CMtfTestActionCompareEntryWithFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsConfigFileParser.h"
#include <msvapi.h>
#include <msvuids.h>
#include <txtrich.h>
#include <smuthdr.h>
#include <csmsemailfields.h>


_LIT(KBodyText, "BodyText");

CMtfTestAction* CMtfTestActionCompareEntryWithFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareEntryWithFile* self = new (ELeave) CMtfTestActionCompareEntryWithFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionCompareEntryWithFile::CMtfTestActionCompareEntryWithFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionCompareEntryWithFile::ExecuteActionL()
	{
	TestCase().Logger().Write(_L("CMtfTestActionCompareEntryWithFile::ExecuteActionL IN"));
	TInt paramCount = ActionParameters().Count();
	if( paramCount != 3)
		{
		User::Leave(KErrArgument);
		}
		
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId paramEntryId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	HBufC* paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(2));;
	
	CMsvEntry* entry = CMsvEntry::NewL(*paramSession, paramEntryId,TMsvSelectionOrdering());
	CleanupStack::PushL(entry);
	
	if ( EqualL( (*entry) , (*paramFilePath)) )
		{ // Test passed
		TestCase().Logger().Write(_L("CMtfTestActionCompareEntryWithFile::Comparison passes"));
		TestCase().SetTestStepResult(EPass);
		}
	else
		{ // Test failed not the same data.
		TestCase().Logger().Write(_L("CMtfTestActionCompareEntryWithFile::Comparison fails"));
		TestCase().SetTestStepResult(EFail);
		}
	
	CleanupStack::PopAndDestroy(entry);
	
	TestCase().Logger().Write(_L("CMtfTestActionCompareEntryWithFile::ExecuteActionL OUT"));
	TestCase().ActionCompletedL(*this);
	}


// Compares only those fields that you request to be compared.

TBool CMtfTestActionCompareEntryWithFile::EqualL(CMsvEntry& aEntry, const TPtrC& smsMessageFileName)	
	{
	TBool ret=ETrue;
	
	CMtfTestActionUtilsConfigFileParser* configParser = CMtfTestActionUtilsConfigFileParser::NewL(smsMessageFileName);
	CleanupStack::PushL(configParser);
	
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
	CleanupStack::PushL(paraFormatLayer);
	
	CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
	CleanupStack::PushL(charFormatLayer);		
		
	_LIT( KEmailToAddress, "EmailToAddress" );
	TPtrC emailToAddressText;
	if(configParser->GetFieldAsString(KEmailToAddress, emailToAddressText) != KErrNotFound)
		{  // Then do the compare.
		const TPtrC& tmp = aEntry.Entry().iDetails;
		if ( tmp.Compare(emailToAddressText)!=0 )
			{ // Not equal fail.
			TestCase().Logger().WriteFormat(_L("CMtfTestActionCompareEntryWithFile:: EmailToAddress: (%S) (%S)"), &tmp, &emailToAddressText);
			ret = EFalse;
			}
		}

	_LIT( KSubject, "Subject" );
	TPtrC subjectText;
	if(configParser->GetFieldAsString(KSubject, subjectText) != KErrNotFound) 
		{
		const TPtrC& tmp = aEntry.Entry().iDescription;
		if ( tmp.Compare(subjectText)!=0 )
			{ // Not equal fail.
			TestCase().Logger().WriteFormat(_L("CMtfTestActionCompareEntryWithFile:: Subject: (%S) (%S)"), &tmp, &subjectText);
			ret = EFalse;
			}
		}

	TPtrC bodyText;
	if(configParser->GetFieldAsString(KBodyText, bodyText) != KErrNotFound)
		{
		CMsvStore* store = aEntry.ReadStoreL();	
		CleanupStack::PushL(store);
		
		if ( !(store->HasBodyTextL()) )
			{	// We have been asked to compare something which does not exist.
			TestCase().Logger().WriteFormat(_L("CMtfTestActionCompareEntryWithFile:: No Message exists at all"));
			ret = EFalse;
			}
		else 
			{	
			CRichText* richText = CRichText::NewL(paraFormatLayer,charFormatLayer);
			CleanupStack::PushL(richText);
			store->RestoreBodyTextL(*richText);

			const TInt length = richText->DocumentLength();
			TPtrC textMessage = richText->Read(0, length);
			
			if ( textMessage.Compare(bodyText)!=0 )
				{ // Not equal fail.
				TestCase().Logger().WriteFormat(_L("CMtfTestActionCompareEntryWithFile:: BodyText: (%S) (%S)"), &bodyText, &textMessage);
				ret = EFalse;
				}
			
			CleanupStack::PopAndDestroy(richText);
			
			}
		CleanupStack::PopAndDestroy(store);				
		}
		
	//??? SC etc
			
	CleanupStack::PopAndDestroy(3, configParser);
	
	return ret;
	}

