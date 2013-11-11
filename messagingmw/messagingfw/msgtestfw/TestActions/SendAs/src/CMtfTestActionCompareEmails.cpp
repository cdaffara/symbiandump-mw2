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
// CompareEmailMessages
// [Action Parameters]
// CMsvSession&			session   <input>			: Reference to the session.
// CMsvEntrySelection&	MailSelection1 <input>	: Reference to the
// first mail selection
// CMsvEntrySelection&	MailSelection2 <input>	: Reference to the
// second mail selection
// [Action Description]
// Compares the email messages for size, subject, body text , html text and attachment.
// [APIs Used]
// CMsvEntry::SetEntry()
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


// User include
#include "CMtfTestActionCompareEmails.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"

#include "CMsvAttachment.h"
#include <miutset.h>
#include <mmsvattachmentmanager.h>

//Granularity for CArrayFixFlat arrays
const TInt KArrayGranularity = 8;

/**
  NewL()
  Constructs a CMtfTestActionCompareEmails object.
  Uses two phase construction and leaves nothing on the CleanupStack.
  @internalTechnology
  @param  aTestCase          Test Case to which the Test Action belongs
  @param  aActionParameters  Action parameters, must not be NULL
  @return Created object of type CMtfTestAction
  @pre    None
  @post   None
*/
CMtfTestAction* CMtfTestActionCompareEmails::NewL(CMtfTestCase& aTestCase,
												  CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCompareEmails* self = 
					new (ELeave) CMtfTestActionCompareEmails(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  Constructor
  Constructs a CMtfTestActionCompareEmails object and initialises the member variables.
  @internalTechnology
  @param  aTestCase  Test Case to which the Test Action belongs
  @pre    None
  @post   None
*/
CMtfTestActionCompareEmails::CMtfTestActionCompareEmails(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase), iParamSession(NULL), iMessageInfo1(NULL), 
		iMessageInfo2(NULL)
	{
	}

/**
  Destructor
  Destructor of the class CMtfTestActionCompareEmails.
  Deletes the message information stored in the CMtfTestActionUtilsEmailMessage
  objects
  @internalTechnology
  @param  None
  @pre    None
  @post   None
*/
CMtfTestActionCompareEmails::~CMtfTestActionCompareEmails()
	{
	delete 	iMessageInfo1;
	delete 	iMessageInfo2;
	}


/**
  ExecuteActionL
  Obtains the parameters for the test action and then calls CompareEmailMessagesL()
  function to compare the sent and the received messages, based on the download
  limits set.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCompareEmails);

	// Get the session object 
	iParamSession = ObtainParameterReferenceL<CMsvSession>
									(TestCase(),ActionParameters().Parameter(0));
	
		
	// Get the first selection object of messages
	CMsvEntrySelection*	msgSelectionList1 = 
									ObtainParameterReferenceL<CMsvEntrySelection>
									(TestCase(), ActionParameters().Parameter(1)); 

	// Get the second selection object of messages
	CMsvEntrySelection*	msgSelectionList2 = 
									ObtainParameterReferenceL<CMsvEntrySelection>
									(TestCase(), ActionParameters().Parameter(2)); 
	

	if( (iParamSession == NULL) || (msgSelectionList1 == NULL) || (msgSelectionList2 == NULL) )
		{
		// Invalid input parameter value
		TestCase().ERR_PRINTF1(_L("Invalid session or message selection object found"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{		
		CompareEmailsL(*msgSelectionList1, *msgSelectionList2);		
		}
	// Test Action is complete
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCompareEmails);
	TestCase().ActionCompletedL(*this);
	}	

/**
  CompareEmailsL
  Compares body text, html text and attachments.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::CompareEmailsL(
										 const CMsvEntrySelection& aMsgSelection1,
										 const CMsvEntrySelection& aMsgSelection2)
	{
	// Verify the input parameters
	if( (!&aMsgSelection1) || (!&aMsgSelection2))	
		{
		TestCase().ERR_PRINTF1(_L("Invalid Message selection"));
		User::LeaveIfError(KErrArgument);
		}

	// Get the count of messages in both the selection object.
	TInt msgCount1 = aMsgSelection1.Count();
	TInt msgCount2 = aMsgSelection2.Count();

	TestCase().INFO_PRINTF2(_L("Message count 1 = %d"),msgCount1);
	TestCase().INFO_PRINTF2(_L("Message count 2 = %d"),msgCount2);
	
	if(msgCount1 != msgCount2)
		{
		// Number of messages is not equal in the selections passed
		TestCase().ERR_PRINTF1(_L("Message count in the selections not equal"));
		TestCase().SetTestStepResult(EFail);
		}

	if(IsTestCaseStatusPass())
		{
		CMsvEntry* msvEntry1 = iParamSession->GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(msvEntry1);

		CMsvEntry* msvEntry2 = iParamSession->GetEntryL(KMsvRootIndexEntryId);
		CleanupStack::PushL(msvEntry2);

		if((!msvEntry1) || (!msvEntry2))
			{
			TestCase().ERR_PRINTF1(_L("Invalid Message Entry"));
			User::LeaveIfError(KErrGeneral);
			}
	
		// Loop through the first list of messages
		for ( TInt msgIndex1 = 0 ; (msgIndex1 < msgCount1) &&
									  (IsTestCaseStatusPass());
									   msgIndex1 ++)
			{			
			TMsvId msgId1 = aMsgSelection1.At(msgIndex1);
			// Set the context to the first message entry
			msvEntry1->SetEntryL(msgId1);
			
			TBool found = EFalse;

			TestCase().INFO_PRINTF3(_L("Comparison of Message: %d,  Subject: %S"),
							(msgIndex1 + 1), &(msvEntry1->Entry().iDescription));
			
			for(TInt msgIndex2 = 0; msgIndex2 < msgCount2 && !found
														    ; msgIndex2++)	
				{
				TMsvId msgId2 = aMsgSelection2.At(msgIndex2);

				// Set the context to the second message entry
				msvEntry2->SetEntryL(msgId2);
				
				// Compare the subject field of the two messages
				if (msvEntry1->Entry().iDescription.
								Compare(msvEntry2->Entry().iDescription) == 0)
					{
					TestCase().INFO_PRINTF2(_L("Message with Subject field in the selection 1: %S found "),
												  &(msvEntry1->Entry().iDescription));

					found = ETrue;
					
					SetEmailMessageInfoL(*msvEntry1, *msvEntry2);
					}
				}	
	
			if ( found )
				{
				if(IsTestCaseStatusPass())
					{
					ComparePlainBodyTextL();
					}
				if(IsTestCaseStatusPass())
					{
					CompareHtmlL();
					}
				if(IsTestCaseStatusPass())
					{
					CompareAttachmentsL(*msvEntry1, *msvEntry2);
					}
				if(IsTestCaseStatusPass())
					{
					DeleteEmailMessageInfo();
					}				
				}
			else{
				TestCase().ERR_PRINTF2(_L("Email with subject %S is not found"), &msvEntry1->Entry().iDescription);
				TestCase().SetTestStepResult(EFail);
				}
			}
		// Cleanup the stack
		CleanupStack::PopAndDestroy(msvEntry2);
		CleanupStack::PopAndDestroy(msvEntry1);
		}
	}

/**
  ComparePlainBodyTextL
  Compares body text.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::ComparePlainBodyTextL()
	{

	TestCase().INFO_PRINTF1(_L("Comparing Plain text part of the message"));
	
	if( iMessageInfo1->GetBodyTextSize() > 0)
		{
		if( iMessageInfo2->GetBodyTextSize() > 0)
			{			
			CParaFormatLayer*	paraFormatLayer1 = CParaFormatLayer::NewL();
			CleanupStack::PushL(paraFormatLayer1);

			CCharFormatLayer*	charFormatLayer1 = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(charFormatLayer1);

			CRichText*	plainBodyText1  = CRichText::NewL( paraFormatLayer1,
															charFormatLayer1,
															CEditableText::EFlatStorage, 256);
			CleanupStack::PushL(plainBodyText1);
			
			iMessageInfo1->GetBodyTextL(*plainBodyText1, *paraFormatLayer1,
															*charFormatLayer1);	

			// Get message body text for 2nd message
			CParaFormatLayer*	paraFormatLayer2 = CParaFormatLayer::NewL();
			CleanupStack::PushL(paraFormatLayer2);

			CCharFormatLayer*	charFormatLayer2 = CCharFormatLayer::NewL(); 
			CleanupStack::PushL(charFormatLayer2);

			CRichText*	plainBodyText2 = CRichText::NewL( paraFormatLayer2, 
															charFormatLayer2, 
															CEditableText::EFlatStorage, 256);

			CleanupStack::PushL(plainBodyText2);
			
			iMessageInfo2->GetBodyTextL(*plainBodyText2, *paraFormatLayer2,
															*charFormatLayer2);
			
			// If the body text buffer is not available, fail the test case
			if(plainBodyText1 == NULL || plainBodyText2 == NULL)
				{
				// Error : Body text buffer not available
				TestCase().ERR_PRINTF1(_L("Body text buffer not available"));
				TestCase().SetTestStepResult(EFail);
				}			

			if(IsTestCaseStatusPass())
				{
				TPtrC bodyText1 = plainBodyText1->Read(0);
				TPtrC bodyText2 = plainBodyText2->Read(0);

				TestCase().INFO_PRINTF3(_L("Body text size of 1st message = %d, Body text size of 2nd messaage= %d"),
											bodyText1.Length(), bodyText2.Length());
								
				if( (bodyText1.Compare(bodyText2)) != 0)
					{
					// Error : Comparison of body text failed
					TestCase().ERR_PRINTF1(_L("Comparison of body text failed"));
					TestCase().SetTestStepResult(EFail);
					}

				else
					{
					TestCase().INFO_PRINTF1(_L("Comparison of Body text was successful"));					
					}				
				}
			CleanupStack::PopAndDestroy(6, paraFormatLayer1);
			}
		else
			{			
			TestCase().ERR_PRINTF1(_L("Message 1 conntains body text, but body text was not received for 2nd message"));
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("1st message does not contain Body text part"));		
		}
	}


/**
  DeleteEmailMessageInfo
  Deletes the email message info created earlier.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::DeleteEmailMessageInfo()
	{	
	delete 	iMessageInfo1;
	iMessageInfo1 = NULL;
		
	delete 	iMessageInfo2 ;
	iMessageInfo2 = NULL;
	}

/**
  SetEmailMessageInfoL
  Sets the email message for the entry specified.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::SetEmailMessageInfoL(CMsvEntry& aMsvEntry1, CMsvEntry& aMsvEntry2)
	{	
	TestCase().INFO_PRINTF1(_L("Setting information for 1st message"));
	iMessageInfo1 = CMtfTestActionUtilsEmailMessage::NewL(aMsvEntry1, TestCase());
		
	TestCase().INFO_PRINTF1(_L("Setting information for 2nd message"));	
	iMessageInfo2 = CMtfTestActionUtilsEmailMessage::NewL(aMsvEntry2, TestCase());
	}

/**
  CompareHtmlL
  Compares html text.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::CompareHtmlL()
	{	
	TPtrC htmlFileName1;
	TInt err1 = iMessageInfo1->HtmlFileName(htmlFileName1);
	TPtrC htmlFileName2;
	TInt err2 = iMessageInfo2->HtmlFileName(htmlFileName2);
	
	if((iMessageInfo1->GetHtmlSize() > 0) && (err1 == KErrNone))
		{
		if( err2 == KErrNone)
			{
			TInt errorVal;
			TBool diffFlag = EFalse;

			// Compare the html files
			CompareFilesL(htmlFileName1, htmlFileName2, errorVal, diffFlag);
			
			if (errorVal == KErrNone )
				{
				if ( diffFlag == EFalse )
					{
					TestCase().INFO_PRINTF1(_L("Comparison of Html files was successful"));
					TestCase().INFO_PRINTF2(_L("Html file for message 1: %S"),&htmlFileName1 );
					TestCase().INFO_PRINTF2(_L("Html file for message 2: %S"),&htmlFileName2 );					
					}
				else
					{
					// Comparison of Html files failed
					TestCase().ERR_PRINTF1(_L("Comparison of Html files failed"));
					TestCase().SetTestStepResult(EFail);
					}
				}
			else
				{
				// An error occured while comparing the html files
				TestCase().ERR_PRINTF1(_L("Error occurred while comparing Html files"));
				TestCase().SetTestStepResult(EFail);
				}
			}
		else
			{
			// Received message's html path was not found
			TestCase().ERR_PRINTF1(_L("Html file path not found in the 2nd message"));
			TestCase().SetTestStepResult(EFail);
			}
		}
	else
		{
		TestCase().INFO_PRINTF1(_L("Html file path not found for the 1st message"));
		}
	}

/**
  CompareFilesL
  Compares files.
  @internalTechnology 
  @pre    None
  @post   None
*/			
void CMtfTestActionCompareEmails::CompareFilesL(const TPtrC afilePath1,
													   const TPtrC afilePath2,
													   TInt& aError, TBool& aDiffFlag)
	{
	aDiffFlag = EFalse;
	CArrayFixFlat<TUint16> *ignoreCharList = new (ELeave)
											CArrayFixFlat<TUint16>(KArrayGranularity);
	CleanupStack::PushL(ignoreCharList);

	// Compare the content of the attachment files
	aError = TestFrameworkActionsUtils::CompareFilesL(afilePath1, afilePath2,
														 EFalse, ignoreCharList, aDiffFlag);
						
	CleanupStack::PopAndDestroy(ignoreCharList);
	}

/**
  CompareAttachmentsL
  Compares the attachments associated with specified entries.
  @internalTechnology 
  @pre    None
  @post   None
*/
void CMtfTestActionCompareEmails::CompareAttachmentsL(CMsvEntry& aMsvEntry1, CMsvEntry& aMsvEntry2)
	{
	TestCase().INFO_PRINTF1(_L("Comparing Attachments"));
		
	CImEmailMessage* emailMessage1 = CImEmailMessage::NewL(aMsvEntry1);
	CleanupStack::PushL(emailMessage1);
	
	CImEmailMessage* emailMessage2 = CImEmailMessage::NewL(aMsvEntry2);
	CleanupStack::PushL(emailMessage2);
	
	MMsvAttachmentManager& manager1 = emailMessage1->AttachmentManager();
	MMsvAttachmentManager& manager2 = emailMessage2->AttachmentManager();

	TInt msgAttachmentCount1 = manager1.AttachmentCount();
	TInt msgAttachmentCount2 = manager2.AttachmentCount();

	TestCase().INFO_PRINTF3(_L("1st message attachment count : %d, 2nd message attachment count : %d"),
								msgAttachmentCount1,msgAttachmentCount2);

	if(msgAttachmentCount1 == msgAttachmentCount2)
		{
		TInt index1 = 0;
		TInt index2 = 0;
		
		TBool	attachmentFound = EFalse;
		
		for (index1 = 0, index2 = 0 ; ( IsTestCaseStatusPass()) && 
					(index1 < msgAttachmentCount1) ; index1++)
			{
			attachmentFound = EFalse;
			CMsvAttachment* attachInfo1 = manager1.GetAttachmentInfoL(index1);
			CleanupStack::PushL(attachInfo1);
			
			for (index2 = 0; index2 < msgAttachmentCount2 && attachmentFound == EFalse; index2++ )
				{
				CMsvAttachment* attachInfo2 = manager2.GetAttachmentInfoL(index2);
				CleanupStack::PushL(attachInfo2);

				if ( attachInfo2->AttachmentName().Compare(attachInfo1->AttachmentName()) == 0 )
					{
					attachmentFound = ETrue;
					
					if (
						(attachInfo1->Type() == CMsvAttachment::EMsvFile || attachInfo1->Type() == CMsvAttachment::EMsvLinkedFile) &&
						(attachInfo2->Type() == CMsvAttachment::EMsvFile || attachInfo2->Type() == CMsvAttachment::EMsvLinkedFile) 
					   )
						{							
						TBool comaprisonFlag = CompareAttachmentFilesL(manager1.GetAttachmentFileL(index1), manager2.GetAttachmentFileL(index2));
						if (!comaprisonFlag)
							{
							 TestCase().SetTestStepResult(EFail);
							}
						
						}
					}
				CleanupStack::PopAndDestroy(attachInfo2);
				}
			CleanupStack::PopAndDestroy(attachInfo1);
			}
			if ( !attachmentFound )
				{
				TestCase().ERR_PRINTF1(_L("Attachments are not identical."));
				TestCase().SetTestStepResult(EFail);
				}
		}
		else
			{
			TestCase().ERR_PRINTF1(_L("Attachments count not identical."));
			TestCase().SetTestStepResult(EFail);	
			}
		CleanupStack::PopAndDestroy(2, emailMessage1);
	}

/**
  CompareAttachmentFiles
  Compares the file associated with the attachments.
  @internalTechnology 
  @pre    None
  @post   None
*/
TBool CMtfTestActionCompareEmails::CompareAttachmentFilesL(RFile aFile1, RFile aFile2)
	{
	//Do the byte by byte comparison
	TBool ret = ETrue;
	TInt sz1, sz2;

	User::LeaveIfError( aFile1.Size(sz1) );
	User::LeaveIfError( aFile2.Size(sz2) );

	if( sz1 != sz2 )
		{
		TestCase().INFO_PRINTF3( _L("File size mismatch 1(%d)   2(%d)"), sz1, sz2 );
		ret = EFalse;
		}
	else
		{
		TBuf8<1> char1;
		TBuf8<1> char2;
		TInt len1, len2;
		do
			{
			aFile1.Read(char1);
			aFile2.Read(char2);
			len1 = char1.Length();
			len2 = char2.Length();
			if( char1.Compare( char2 ) != 0 )
				{
				ret = EFalse;
				break;
				}
			}
		while( len1 && len2 );
		}
	return ret;
	}
