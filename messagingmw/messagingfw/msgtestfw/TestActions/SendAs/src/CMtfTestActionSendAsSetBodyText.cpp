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
// SendAsSetBodyText
// [Action Parameters]
// RSendAsMessage			sendAsMessage		<input>	: RSendAsMssage object
// TInt						bufferType			<input> : Type of the buffer storing body text contents 
// TInt 					index				<input>	: Value of Index
// [Action Description]
// SendAsSetBodyText Test Action is intended to set the body text 
// contents for a message that was created using Send-As API.  
// The body text content is read from a file and then stored in a 
// CRichText object. The 
// The buffer type is taken as the input parameter to the test action 
// Buffer type 0 => Set body text using Descriptor
// Buffer type 1 => Set body text using CRichText object
// [APIs Used]
// RSendAsMessage::SetBodyTextL(const TDesC)	
// RSendAsMessage::SetBodyTextL(const TDesC)
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

// User include
#include "CMtfTestActionSendAsSetBodyText.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "sendas2.h"


/**
  NewL()
  Constructs a CMtfTestActionSendAsSetBodyText object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSendAsSetBodyText
  @pre    None
  @post   CMtfTestActionSendAsSetBodyText object is created
*/
CMtfTestAction* CMtfTestActionSendAsSetBodyText::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendAsSetBodyText* self = 
							new (ELeave) CMtfTestActionSendAsSetBodyText(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSendAsSetBodyText constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionSendAsSetBodyText::CMtfTestActionSendAsSetBodyText(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionSendAsSetBodyText
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendAsSetBodyText::~CMtfTestActionSendAsSetBodyText()
	{
	}

/**
  ExecuteActionL
  Obtains the parameters for the test action. Get the Message Id at given index. The Message 
  Id is stored as output parameters of this Test Action
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSendAsSetBodyText::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendAsSetBodyText);
	
		// Get test action input parameters
		RSendAsMessage sendAsMessage = ObtainValueParameterL<RSendAsMessage>(TestCase(),
														ActionParameters().Parameter(0));
		
		// Buffer type to be used for adding the body text 
		// 0 => Set body text using Descriptor 
		// 1 => Set body text using CRichText object (Default)	
		TInt bufferType = ObtainValueParameterL<TInt>(TestCase(),
														ActionParameters().Parameter(1),1);
	
		TInt index = ObtainValueParameterL<TInt>(TestCase(),
														ActionParameters().Parameter(2));
	
		// Get the body text configuration file name												
		TPtrC fileName = TestCase().GetConfigurationFileL(CMtfConfigurationType::EMtfDataFile,index);

		// Create a Rich text object to store the Body text contents
		CPlainText::TTextOrganisation ttOrg = {CPlainText::EOrganiseByLine};

		CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
		CleanupStack::PushL(paraFormatLayer);
		CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
		CleanupStack::PushL(charFormatLayer);
		CRichText* bodyCRichText = CRichText::NewL(paraFormatLayer, charFormatLayer);
		CleanupStack::PushL(bodyCRichText);

		HBufC* bodyTextFileName = fileName.Alloc();
		CleanupStack::PushL(bodyTextFileName);

		// Read the body text contents into the Rich text object
		bodyCRichText->ImportTextFileL(0, bodyTextFileName->Des(), ttOrg);	
	
		TInt err = KErrNone;
		// Set body text using descriptor
		if( bufferType  == 0 )
			{
			// Create a heap buffer and copy the file contents
			TInt len1 = bodyCRichText->DocumentLength();
			HBufC* buf1 = HBufC::NewL(len1);
			CleanupStack::PushL(buf1);
			
			TPtr fileContents = buf1->Des();
			bodyCRichText->Extract(fileContents,0,(len1));
			
			// Set the body text to the message
			TRAP(err, sendAsMessage.SetBodyTextL(fileContents));
			
			if(err == KErrNone)
				{
				TestCase().INFO_PRINTF1(_L("SetBodyTextL using Descriptor was successful"));
				}
			else
				{
				TestCase().ERR_PRINTF2(_L("SetBodyTextL using Descriptor failed with error: %d"), err);
				TestCase().SetTestStepResult(EFail);
				}
				
			// destroy the temporary storage of the text
			CleanupStack::PopAndDestroy(buf1);
			}
		else if (bufferType  == 1)
			{
			// Set body text using CRichText
			TRAP(err, sendAsMessage.SetBodyTextL(*bodyCRichText));
			
			if(err == KErrNone)
				{
				TestCase().INFO_PRINTF1(_L("SetBodyTextL using Rich Text was successful"));
				}
			else
				{
				TestCase().ERR_PRINTF2(_L("SetBodyTextL using Rich Text failed with error: %d"), err);
				TestCase().SetTestStepResult(EFail);
				}
			}
	 	else
			{
			err = KErrArgument;
			TestCase().ERR_PRINTF2(_L("Incorrent input parameter: Buffer type value should be either 0 or 1. The value provided is  %d"),bufferType );
			TestCase().SetTestStepResult(EFail);
			}
		StoreParameterL<TInt>(TestCase(),err,ActionParameters().Parameter(3));
			
		CleanupStack::PopAndDestroy(4);	//bodyTextFileName, bodyCRichText, charFormatLayer, paraFormatLayer
		TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendAsSetBodyText);
		}
	TestCase().ActionCompletedL(*this);
	}

