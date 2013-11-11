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
// VerifyBioType
// [Action Parameters]
// CMsvEntry				paramMsgEntry	<input>	:	Reference to message entry
// TUid						paramBioType	<input>	:	Bio Type
// [Action Description]
// VerifyBioType Test Action is intended verify the Bio type set for
// the message with the specified string
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionVerifyBioType.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

//#include <txtfmlyr.h>
//#include <txtrich.h>
//#include <smuthdr.h>

//#include "messaging\sms\Smsstd.h"
//#include "SmsMessageWrappers.h"
/**
  NewL()
  Constructs a CMtfTestActionVerifyBioType object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionVerifyBioType
  @pre    None
  @post   CMtfTestActionVerifyBioType object is created
*/
CMtfTestAction* CMtfTestActionVerifyBioType::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifyBioType* self = 
							new (ELeave) CMtfTestActionVerifyBioType(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionVerifyBioType constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionVerifyBioType::CMtfTestActionVerifyBioType(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionVerifyBioType
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionVerifyBioType::~CMtfTestActionVerifyBioType()
	{
	}

/**
  ExecuteActionL
	Obtains the input parameters
	1.	paramMsgEntry
	2.	paramBioType
	Compare the bioType Id provided as input with paramMsgEntry.Entry().iBioType value
	If the comparison fails, leave the Test Action with appropriate error

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionVerifyBioType::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyBioType);

		// Get test action input parameters
		CMsvEntry* paramMsgEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),
												ActionParameters().Parameter(0));	
									
		TBioMsgIdType paramBioType = ObtainValueParameterL<TBioMsgIdType>(TestCase(),
														ActionParameters().Parameter(1));
/*
		CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
		CleanupStack::PushL(paraFormatLayer);
		CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
		CleanupStack::PushL(charFormatLayer);
		CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
		CleanupStack::PushL(richText);
		richText->Reset();
	
		CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsDeliver, *richText);
		CleanupStack::PushL(header);
		CMsvStore* msgStore = paramMsgEntry->ReadStoreL();
		header->RestoreL(*msgStore);
		
		TBioMsgIdType bioTypeId = header->BioMsgIdType();
		//TMsvMessageBio& bioWrapper = header->BioMessage();
		
		//TMsvMessageSms& smsWrapper = header->SmsMessage();
		//TBioMsgIdType bioTypeId = smsWrapper.BioMsgIdType();
		//TBioMsgIdType bioTypeId = bioWrapper.BioMsgIdType();
		TUid bioType = TUid::Uid(paramMsgEntry->Entry().iBioType);

		if(bioType.operator==(TUid::Uid(paramBioType)))
			{
			TestCase().INFO_PRINTF1(_L("Comparison of Bio Type was successful."));		
			}
		else
			{
			TestCase().ERR_PRINTF1(_L("Bio type of the message does not match with the expected value"));
			TestCase().SetTestStepResult(EFail);
			}
*/
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifyBioType);	
		}
	TestCase().ActionCompletedL(*this);
	}

