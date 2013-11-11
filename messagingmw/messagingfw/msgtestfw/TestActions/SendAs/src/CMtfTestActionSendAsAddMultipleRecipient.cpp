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
// SendAsAddMultipleRecipient
// [Action Parameters]
// RSendAsMessage			sendAsMessage	<input>			: Reference of  the RSendAsMessage object
// TDesC					recipientAddress<input>			: Recipient address
// TDesC					alias			<input>			: Alias of recipient addresses. Default value implies no Alias information is provided
// TDesC					recipientAddress1<input> 		: Second Recipient address 
// TSendAsRecipientType		recipientType	<input>			: Type of recipient (ESendAsRecipientTo or ESendAsRecipientCc or  ESendAsRecipientBcc)
// TMsvId 					paramServiceId	<input>			: Value of the SMTP service id. Recipient address determined using this ID for SMTP message.
// [Action Description]
// SendAsAddMultipleRecipient Test Action is intended to add a new recipient to the 
// created message.  
// [APIs Used]
// 1.	RSendAsMessage::AddRecipient(const TDesC& aAddress, TSendAsRecipientType aRecipientType)
// 2.	RSendAsMessage::AddRecipient(const TDesC& aAddress, const TDesC& aAlias, 
// TSendAsRecipientType aRecipientType); 
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// User include
#include "CMtfTestActionSendAsAddMultipleRecipient.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TestFrameworkActionsUtils.h"
#include "CMtfTestActionUtilsMessage.h"
#include "CMtfTestActionUtilsConfigFileMachineName.h"

#include <btmsgtypeuid.h>
#include <cemailaccounts.h>
/**
  NewL()
  Constructs a CMtfTestActionSendAsAddMultipleRecipient object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsAddMultipleRecipient
  @pre    None
  @post   CMtfTestActionSendAsAddMultipleRecipient object is created
*/
CMtfTestAction* CMtfTestActionSendAsAddMultipleRecipient::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsAddMultipleRecipient* self = 
							new (ELeave) CMtfTestActionSendAsAddMultipleRecipient(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsAddMultipleRecipient constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsAddMultipleRecipient::CMtfTestActionSendAsAddMultipleRecipient(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsCloseAndDeleteMessage
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsAddMultipleRecipient::~CMtfTestActionSendAsAddMultipleRecipient()
	{
	}


/**
  ExecuteActionL
	Obtain the input parameters
	1.	sendAsMessage
	2.	recipientAddress
	3.	alias  (Default value implies empty string)
	4. recipientAddress1
	5.	recipientType 
	IF alias for recipient address is provided as input 
	Call RSendAsMessage::AddRecipient (const TDesC& aAddress, const TDesC& aAlias, 
	TSendAsRecipientType aRecipientType) passing recipientAddress, alias and
	recipientType as input parameters
    ELSE 
	Call RSendAsMessage::AddRecipient (const TDesC& aAddress, TSendAsRecipientType aRecipientType) passing recipientAddress and recipientType as input parameters

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsAddMultipleRecipient::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsAddMultipleRecipient);

		// Get test action input parameters
		RSendAsMessage sendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
											ActionParameters().Parameter(0));

		HBufC*  recipientAddress	 = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(1), NULL);


		HBufC* alias				 = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(2), NULL);

		
		RSendAsMessage::TSendAsRecipientType recipientType	
									 = ObtainValueParameterL<RSendAsMessage::TSendAsRecipientType>(TestCase(),
											ActionParameters().Parameter(3));

		TUid msgTypeId				 =  ObtainValueParameterL<TUid>(TestCase(),
											ActionParameters().Parameter(4));
											
		TInt expectedErr			 =  ObtainValueParameterL<TInt>(TestCase(),
											ActionParameters().Parameter(5));
		//Get the second recipient address
		HBufC*  recipientAddress1	 = ObtainParameterReferenceL<HBufC>(TestCase(),
											ActionParameters().Parameter(6), NULL);
										
		TInt err = KErrNone;
		TBuf16<3> btAddress;
						

		if(recipientAddress == NULL)
			{
			if( msgTypeId.operator == (KUidMsgTypeSMTP))
				{
				TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(), ActionParameters().Parameter(7), 0);
				CImSmtpSettings* settings = new(ELeave) CImSmtpSettings();
				CleanupStack::PushL(settings);
		 		
		 		CEmailAccounts* accounts = CEmailAccounts::NewLC();
		 		TSmtpAccount smtpAccount;
		 		accounts->GetSmtpAccountL(paramServiceId, smtpAccount);
				accounts->LoadSmtpSettingsL(smtpAccount, *settings);
				CleanupStack::PopAndDestroy(accounts);
				
				recipientAddress=settings->EmailAddress().AllocL();
				CleanupStack::PopAndDestroy(settings);
				}
			else
				{
				TestCase().ERR_PRINTF1(_L("Recipient address is not provided"));
				TestCase().SetTestStepResult(EFail);
				}
			}
		else
			{
			if( msgTypeId.operator == (KUidMsgTypeBt))
				{
				CMtfTestActionUtilsMessage::FormatBtRecipientAddress(*recipientAddress, btAddress);
				}
			}
		if((TestCase().TestStepResult()) == EPass)
			{
			if(alias == NULL )
				{
				if( msgTypeId.operator == (KUidMsgTypeBt))
					{
					TRAP(err,sendAsMessage.AddRecipientL(btAddress, recipientType));
					}
				else
					{
					TRAP(err,sendAsMessage.AddRecipientL(*recipientAddress, recipientType));
					TRAP(err, sendAsMessage.AddRecipientL(*recipientAddress1,  recipientType));
					}
				if(err == expectedErr)
					{
					TestCase().INFO_PRINTF1(_L("Recipient address added successfully"));	
					}
				else
					{
					TestCase().ERR_PRINTF2(_L("Add Recipient address failed with error %d"), err);
					}
				}
			else
				{
				if( msgTypeId.operator == (KUidMsgTypeBt))
					{
					TRAP(err,sendAsMessage.AddRecipientL(btAddress, alias->Des(), recipientType));
					}
				else
					{
					TRAP(err, sendAsMessage.AddRecipientL(recipientAddress->Des(), alias->Des(), recipientType));
					}

				if (err == expectedErr)
					{
					if(err == KErrNone)
						{
						TestCase().INFO_PRINTF1(_L("Recipient address with alias added successfully"));						
						}
					else
						{
						TestCase().INFO_PRINTF1(_L("Recipient address with alias failed with expected error"));
						}
					}
				else
					{
					TestCase().ERR_PRINTF2(_L("Add Recipient address with alias failed with error %d"), err);
					}
				}
				
			}
									
			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsAddMultipleRecipient);
		}
	TestCase().ActionCompletedL(*this);
	}

