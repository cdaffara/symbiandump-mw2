// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// VerifyReplyToAddress
// [Action Parameters]
// 0) CMsvSession&				session   <input>			:Reference to the session.
// 1) CMsvEntrySelection&		SentMailSelection <input>	: Reference to the sent mail selection.
// 2) CMsvEntrySelection&		RecvMailSelection <input>	: Reference to the received mail selection.
// 3) TInt						Number of mails under test.
// 4) HBufC&					Full Epoc32 file path of sample email txt file used.
// 5) TInt						Whether the sample email contains a Reply-To address field or not? (1 = ETrue, 0 = EFalse)
// [Action Description]
// Implements the action to compare the Reply-To address set in header of email from txt file to
// that of the email actual sent as stored in the Message store as a TMsvEntry object.
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file CMtfTestActionVerifyReplyToAddress.cpp
*/

#include <e32std.h>
#include <txtrich.h>
#include <mtclreg.h>
#include <imcm.rsg>

#include <msvstore.h>
#include "CMtfTestActionVerifyReplyToAddress.h"
#include <cemailaccounts.h>
#include <iapprefs.h>
#include "CMtfTestActionUtilsPopScripts.h"

#include "CMtfTestActionUtils.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"
#include "TMtfTestParameterType.h"
#include "CPop3MailInfo.h"

// Expected Reply-To field to be used in sample emails 
_LIT8(KReplyToField, "Reply-To: matthewf@msexchange2k.closedtest.intra\r\n");

_LIT(KSettingsFile, "c:\\msgtest\\TestFramework\\Configurations\\EmailSettings\\Pop3Tests-lon-cn-exchng2k.txt");

#ifdef _UNICODE
 _LIT(KSmtpEmailAddressAndAliasFmtStr,"\"%S\" <%S>");
#else
 _LIT8(KSmtpEmailAddressAndAliasFmtStr,"\"%S\" <%S>");
#endif

/**
  Function : NewL
  Description : Standard symbian 2 phase constructor
  @internalTechnology
  @param aMtfTestCase - CMtfTestCase test case reference for base class constructor
  @param aActionParameters - pointer to the parameters for this test action
  @return A CMtfTestAction pointer to the newly created CMtfTestActionVerifyReplyToAddress object
  @precondition none
  @postcondition none
*/
CMtfTestAction* CMtfTestActionVerifyReplyToAddress::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifyReplyToAddress* self = new (ELeave) CMtfTestActionVerifyReplyToAddress(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Function : CMtfTestActionVerifyReplyToAddress constructor
  Description : Private constructor - must not leave
  @internalTechnology
  @param  aTestCase - CMtfTestCase reference for the base class constructor
  @return N/A
  @precondition none
  @postcondition none
*/
CMtfTestActionVerifyReplyToAddress::CMtfTestActionVerifyReplyToAddress(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase), iSession(NULL), iSentSelectionList(NULL), iInboxSelectionList(NULL)
	{
	}

/**
  Function : CMtfTestActionVerifyReplyToAddress destructor
  Description : Destructor
  @internalTechnology
  @param  N/A
  @return N/A
  @precondition none
  @postcondition none
*/
CMtfTestActionVerifyReplyToAddress::~CMtfTestActionVerifyReplyToAddress()
	{
	}

/**
  Function : LoadParametersL
  Description : Loads the test action parameters 
  @internalTechnology
  @param  none
  @return ETrue if parameters loaded Efalse on failure
  @precondition none
  @postcondition Upon failure TestCase status is EFail
*/
TBool CMtfTestActionVerifyReplyToAddress::LoadParametersL()
	{
	iSession = ObtainParameterReferenceL<CMsvSession> (TestCase(),ActionParameters().Parameter(0));
	if(! iSession)
		{
		TestCase().ERR_PRINTF1(_L("Invalid session object"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	iSentSelectionList = ObtainParameterReferenceL<CMsvEntrySelection> (TestCase(), ActionParameters().Parameter(1));
	if(! iSentSelectionList)
		{
		TestCase().ERR_PRINTF1(_L("Invalid sentSelectionList"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	iInboxSelectionList = ObtainParameterReferenceL<CMsvEntrySelection> (TestCase(), ActionParameters().Parameter(2));
	if(! iInboxSelectionList)
		{
		TestCase().ERR_PRINTF1(_L("Invalid inboxSelectionList"));
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	
	if(iInboxSelectionList->Count() != iSentSelectionList->Count())
		{
		TestCase().ERR_PRINTF3(_L("iInboxSelectionList->Count(%d) != iSentSelectionList->Count(%d)"),
								 iInboxSelectionList->Count(), iSentSelectionList->Count() );
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}

	TInt numTests = ObtainValueParameterL<TInt> (TestCase(),ActionParameters().Parameter(3));
	if(numTests != iInboxSelectionList->Count())
		{
		TestCase().ERR_PRINTF2(_L("NumTests != iInboxSelectionList->Count(%d)"), iInboxSelectionList->Count());
		TestCase().SetTestStepResult(EFail);
		return EFalse;
		}
	
	iEmailFilePath = ObtainParameterReferenceL<HBufC> (TestCase(),ActionParameters().Parameter(4));
	TPtr ptrEmailFilePath = iEmailFilePath->Des();
	TestCase().INFO_PRINTF2(_L("Email path: %S"), &ptrEmailFilePath);
		
	iReplyToAddress = ObtainValueParameterL<TInt> (TestCase(),ActionParameters().Parameter(5));	
	TestCase().INFO_PRINTF2(_L("ReplyToField flag: %d"), iReplyToAddress);
	
	return ETrue;
	}

/**
  Function : ExecuteActionL
  Description : Test framework entry point for CMtfTestActionVerifyReplyToAddress test action
  @internalTechnology
  @param  none
  @return void
  @precondition none  
  @postcondition TestCase status EPass if tests passed otherwise EFail
*/
void CMtfTestActionVerifyReplyToAddress::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyReplyToAddress);
	if(LoadParametersL())
		{
		User::LeaveIfError(iFs.Connect());
		CleanupClosePushL(iFs);	
			// Normally do not push an object that is a data member of another object.
			// But in this case we want to close early if we get a leave.
			// Otherwise the iFs would be open till the thread had begun to die.
		
		if(TestCase().TestStepResult() == EPass)
			{
			CMsvEntry* recvMsvEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL(recvMsvEntry);
			CMsvEntry* sentMsvEntry = iSession->GetEntryL(KMsvRootIndexEntryId);
			CleanupStack::PushL(sentMsvEntry);

			for(TInt i=0; i<iInboxSelectionList->Count(); i++)
				{
				recvMsvEntry->SetEntryL(iInboxSelectionList->At(i));
				TMsvEntry recvEntry = recvMsvEntry->Entry();
				for(TInt j=0; j<iSentSelectionList->Count(); j++)
					{
					sentMsvEntry->SetEntryL(iSentSelectionList->At(j));
					TMsvEntry sentEntry = sentMsvEntry->Entry();
					if(recvEntry.iDescription.Compare(sentEntry.iDescription) == 0)
						{
						if(! ReplyToHeaderFieldCompareL(sentEntry, iEmailFilePath->Des(), iSession))
							{
							TestCase().INFO_PRINTF2(_L("Test %d failed"), j+1);
							TestCase().SetTestStepResult(EFail);
							}
						}
					}
				}
			CleanupStack::PopAndDestroy(2, recvMsvEntry); // recvMsvEntry, sentMsvEntry
			}
		CleanupStack::PopAndDestroy(&iFs);
		}
		
	TestCase().INFO_PRINTF1( _L("Test Action CMtfTestActionVerifyEmail Completed") );
	TestCase().ActionCompletedL(*this);
	}
	
TBool CMtfTestActionVerifyReplyToAddress::ReplyToHeaderFieldCompareL(TMsvEntry& aSentEntry, const TDesC& aFileName, CMsvSession* aSession)
	{
	TBool ret = ETrue;
	
	// If Reply-To address exists in email header then... 
	if(iReplyToAddress)
		{
		// Read Reply-To field from sample email txt file
		RFileReadStream rdStream;
		User::LeaveIfError(rdStream.Open(iFs, aFileName, EFileRead|EFileStreamText));
	
		if(IsEofL(rdStream))
			{
			ret = EFalse;
			return ret;
			}
		
		TBuf8<256> line;
		TChar char_LF('\n');
		line.Zero();
		
		do
			{
			rdStream.ReadL(line, char_LF);
			if(line.Compare(KReplyToField)==0)
				{
				// Break from loop if expected Reply-To field found
				break;
				}
			}
			while(!IsEofL(rdStream));
			
		rdStream.Close(); // End of reading stream
			
		// aSentEntry - Identifier of the original sent email identified by TMsvEntry	
		CMsvEntry* msvEntry = aSession->GetEntryL(aSentEntry.Id());
		CleanupStack::PushL(msvEntry);
		
		msvEntry->SetEntryL(aSentEntry.Id());
	
		CMsvStore* store = msvEntry->ReadStoreL();
		CleanupStack::PushL(store);
	
		CImHeader* header = CImHeader::NewLC();
		
		header->RestoreL(*store);
		// Read Reply-To address from email header
		TPtrC line2 = header->ReplyTo();
		
		TestCase().INFO_PRINTF2(_L("Reply-To address from actual sent message as stored in the Message store: %S"), &line2);
	
		TBuf16<256> tmp;
		tmp.Copy(line);
	
		TBuf16<256> line_16;
		
		// Extract just the Reply-To address without the initial tag and trailing CR-LF characters 
		for(TInt n=10; n<tmp.Length()-2; n++)
			{
			line_16.Append(tmp[n]);
			}
	
		// Compare if user-set Reply-To address in sample email txt file equals the one set by system in email header
		if(line_16.Compare(line2) != 0)
			{
			ret = EFalse;
			}
		
		CleanupStack::PopAndDestroy(header); 
		CleanupStack::PopAndDestroy(store); 
		CleanupStack::PopAndDestroy(msvEntry); 
		
		}
		// else no Reply-To address exists in email header... 
		else
			{
			// Check if Reply-To address is set correctly from default settings
			CEmailAccounts* accounts = CEmailAccounts::NewL();
			CleanupStack::PushL(accounts);
			CImSmtpSettings* settings = new(ELeave) CImSmtpSettings();
			CleanupStack::PushL(settings);
			CImIAPPreferences* smtpIAP = CImIAPPreferences::NewLC();
	
			TSmtpAccount smtpAccountId = accounts->CreateSmtpAccountL(_L("SMTP"),*settings, *smtpIAP, EFalse);
			accounts->SetDefaultSmtpAccountL(smtpAccountId);
			accounts->PopulateDefaultSmtpSettingsL(*settings, *smtpIAP);
			
			CImIAPPreferences *popPrefs = CImIAPPreferences::NewLC();
			CImPop3Settings *popSettings = new (ELeave) CImPop3Settings;
			CleanupStack::PushL(popSettings);

			accounts->PopulateDefaultPopSettingsL(*popSettings, *popPrefs);
			
			// TestAction CreatePop3Service doesn't save settings to central repository,
			// so default settings are reloaded
			CMtfTestActionUtilsPopScripts::ReadPopSettingsFromConfigurationFileL(
			TestCase(), KSettingsFile, *popSettings, *popPrefs, *settings, *smtpIAP);
			
			TPtrC defaultReplyToAddress = settings->ReplyToAddress();
			
			TestCase().INFO_PRINTF2(_L("Default ReplyTo Address: %S"), &defaultReplyToAddress);
			
			// Format the Reply-To address field
			HBufC* formatedAddress = CreateFormattedAddressLC(defaultReplyToAddress, settings->EmailAlias());
			TPtr ptrFormatedAddress = formatedAddress->Des();
			TestCase().INFO_PRINTF2(_L("Formated ReplyTo Address: %S"), &ptrFormatedAddress);
			
			// aSentEntry - Identifier of the original sent email identified by TMsvEntry
			CMsvEntry* msvEntry = aSession->GetEntryL(aSentEntry.Id());
			CleanupStack::PushL(msvEntry);
		
			msvEntry->SetEntryL(aSentEntry.Id());
			CMsvStore* store = msvEntry->ReadStoreL();
			CleanupStack::PushL(store);
					
			CImHeader* header = CImHeader::NewLC();
			
			header->RestoreL(*store);
			// Read Reply-To address from email header
			TPtrC usedReplyToAddress = header->ReplyTo();
		
			TestCase().INFO_PRINTF2(_L("Reply-To address from actual sent message as stored in the Message store: %S"), &usedReplyToAddress);
			
			// Compare if default Reply-To address equals the one set by system in email header
			if(ptrFormatedAddress.Compare(usedReplyToAddress) !=0)
				{
				ret = EFalse;
				}
			
			CleanupStack::PopAndDestroy(header);
			CleanupStack::PopAndDestroy(store);
			CleanupStack::PopAndDestroy(msvEntry);
			CleanupStack::PopAndDestroy(formatedAddress);
			CleanupStack::PopAndDestroy(popSettings);
			CleanupStack::PopAndDestroy(popPrefs);
			CleanupStack::PopAndDestroy(smtpIAP);
			CleanupStack::PopAndDestroy(settings);
			CleanupStack::PopAndDestroy(accounts);
			}
		return ret;
	}

/**
 IsEofL()
 Checks if the current position of read stream reached end of file
*/ 
TBool CMtfTestActionVerifyReplyToAddress::IsEofL(RReadStream& aReadStream)
	{
	TStreamPos streamPos = aReadStream.Source()->TellL(MStreamBuf::ERead);
	TInt streamSize = aReadStream.Source()->SizeL();
	
	if(streamPos >= streamSize)
		return ETrue;
	
	return EFalse;
	}

/**
 CreateFormattedAddressL()
 Re-used code from src/common/generic/messaging/email/smtpservermtm/src/imsmsend.cpp#057
 Creates a new HBufC* containing the concateneatd alias and address strings
*/
HBufC* CMtfTestActionVerifyReplyToAddress::CreateFormattedAddressLC(const TDesC& aString, const TDesC& aAlias)
    {
    TImMessageField msgField;

    // Make a buffer...
    TInt strLen     = aString.Length();
    TInt aliasLen   = aAlias.Length();

    HBufC* outString = HBufC::NewLC(strLen+aliasLen+5);
    HBufC* buffer = HBufC::NewL(strLen);

    *buffer = msgField.GetValidInternetEmailAddressFromString(aString);

	if (aliasLen)
        {
		outString->Des().Format(KSmtpEmailAddressAndAliasFmtStr,&aAlias,buffer);
        }
    else
        {
        *outString = *buffer; // just copy email address; as there's no alias to be added
        }

	delete buffer;
    return outString;
    }
    
