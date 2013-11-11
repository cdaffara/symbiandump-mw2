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
// VerifyBodyText
// [Action Parameters]
// CMsvStore				paramMsgStore			<input>	:	Reference to Message Store object
// TMtfConfigurationType	paramConfigurationType	<input>	:	Configuration type required for identifying the corresponding list of configuration files
// TInt						paramIndex				<input>	:	Index of the file name present in the list of configuration files ( file added to the list using SetDefaultConfiguration Test Action)
// ErrorCode	  <output>  (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// VerifyBodyText Test Action is intended to read the body text contents of 
// a message and compare it with the contents of a file
// [APIs Used]
// CMsvStore::HasBodyTextL ()
// CRichText::Extract ()
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
#include "CMtfTestActionVerifyBodyText.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsMessage.h"
#include "sendas2.h"

/**
  NewL()
  Constructs a CMtfTestActionVerifyBodyText object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionVerifyBodyText
  @pre    None
  @post   CMtfTestActionVerifyBodyText object is created
*/
CMtfTestAction* CMtfTestActionVerifyBodyText::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifyBodyText* self = 
							new (ELeave) CMtfTestActionVerifyBodyText(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionVerifyBodyText constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionVerifyBodyText::CMtfTestActionVerifyBodyText(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionVerifyBodyText
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionVerifyBodyText::~CMtfTestActionVerifyBodyText()
	{
	}
	
/**
  ExecuteActionL
  Obtain the input parameters
	1.	CMsvStore
	2.	paramConfigurationType
	3.	paramIndex
	Check if the store contains any streams using CMsvStore::IsNullL () function.  If store is empty, leave with appropriate error
	Call CMtfTestCase::GetConfigurationFileL(configurationType, Index) passing the configuration type and the index to the file name
	Call User::LeaveIfError( CMtfTestActionUtilsMessage::VerifyBodyTextContentsL()), passing CMsvStore and the file name to the VerifyBodyTextContentsL() function.

  
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionVerifyBodyText::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyBodyText);
	if( ActionParameters().Count() < 4 )
		{
		TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Insufficient Action Parameters") , &KTestActionVerifyBodyText);
		}
	else
		{
		TRAPD(err, RunTestL());
	
		TInt paramExpectedResult = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(3));
		
		if(err != paramExpectedResult)
			{
			TestCase().ERR_PRINTF3(_L("%S :: FAIL :: Verify Body Text Failed with error = %d") , &KTestActionVerifyBodyText, err);
			}
		else
			{
			TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifyBodyText);
			}
		}
		
	TestCase().ActionCompletedL(*this); 
	}
	
	
void CMtfTestActionVerifyBodyText::RunTestL()
	{
	
	CMsvEntry* paramEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),
									ActionParameters().Parameter(0));
									
	CMtfConfigurationType::TMtfConfigurationType paramConfigurationType = ObtainValueParameterL<CMtfConfigurationType::TMtfConfigurationType>(TestCase(),
												ActionParameters().Parameter(1));
	
	TInt paramIndex = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(2));


	TPtrC fileName = TestCase().GetConfigurationFileL(paramConfigurationType,paramIndex);
	
	// Call the utils function to verify the body text contents
	User::LeaveIfError(CMtfTestActionUtilsMessage::	VerifyBodyTextContentsL(*paramEntry,fileName.AllocLC()));

	CleanupStack::PopAndDestroy();
	
	}

