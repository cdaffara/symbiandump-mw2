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
// SetBodyText
// [Action Parameters]
// CMsvStore   msgStore   <input> : Message Store of the message 
// for which the body text is to be set
// HBufC	   fileName	  <input> : Name of the file from which the body text
// contents are read
// [Action Description]
// Sets the body text for a message
// [APIs Used]
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

//system include
#include <msvapi.h>
#include <txtrich.h>

#include <txtetext.h>
// User include
#include "CMtfTestActionSetBodyText.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



/**
  NewL()
  Constructs a CMtfTestActionSetBodyText object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSetBodyText
  @pre    None
  @post   CMtfTestActionSetBodyText object is created
*/
CMtfTestAction* CMtfTestActionSetBodyText::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetBodyText* self = 
							new (ELeave) CMtfTestActionSetBodyText(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSetBodyText constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSetBodyText::CMtfTestActionSetBodyText(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSetBodyText
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSetBodyText::~CMtfTestActionSetBodyText()
	{
	}

/**
  ExecuteActionL
  Sets the Body text for the specified message
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSetBodyText::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetBodyText);
	
		// Get test action parameters
		CMsvStore* paramStore = ObtainParameterReferenceL<CMsvStore>(TestCase(),
												ActionParameters().Parameter(0));
												
		// File from which the body contents is read
		HBufC* paramFileName = ObtainParameterReferenceL<HBufC>(TestCase(),
												ActionParameters().Parameter(1));
	
		// Create a rich text object
		CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

		CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
		CleanupStack::PushL(paraFormatLayer);
		CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
		CleanupStack::PushL(charFormatLayer);
		CRichText* bodyCRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
		CleanupStack::PushL(bodyCRichText);

		// Copy the contents of the file into Rich text object
		bodyCRichText->ImportTextFileL(0,paramFileName->Des(),ttOrg);

		// Set the body text for the message
		TRAPD(err, paramStore->StoreBodyTextL(*bodyCRichText));
		if(err == KErrNone)
			{
			TestCase().INFO_PRINTF2(_L("Setting Body Text was successful, Error code = %d"), err);
			}
		else
			{
			TestCase().ERR_PRINTF2(_L("Setting Body Text falied with error = %d"), err);
			TestCase().SetTestStepResult(EFail);
			}
		
		CleanupStack::PopAndDestroy(3); // paraFormatLayer,  charFormatLayer, bodyCRichText
		
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetBodyText);
		}
	TestCase().ActionCompletedL(*this);
	}


