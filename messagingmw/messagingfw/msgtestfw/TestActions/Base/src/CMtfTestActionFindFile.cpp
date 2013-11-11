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
// FindFile
// [Action Parameters]
// HBufC fileName     <input>: Name of File along with the complete path.
// [Action Description]
// Checks for a specified file in a specified directory.
// [APIs Used]
// RMessagingTestUtilityServer::FileExists
// __ACTION_INFO_END__
// 
//

#include "messagingtestutility.h"
#include "CMtfTestActionFindFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created 
							  CMtfTestActionCompareImapAccountId object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionFindFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionFindFile* self = new (ELeave) CMtfTestActionFindFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

/**
  Function : CMtfTestActionCompareImapAccountId
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionFindFile::CMtfTestActionFindFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  Function : ~CMtfTestActionCompareImapAccountId
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionFindFile::~CMtfTestActionFindFile()
	{
	}


/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/
void CMtfTestActionFindFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionFindFile);
	HBufC* autoSendFileName = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));
	
	TBool paramTestExistanceFlags=EFalse; 
		// Indicates that we can test for non existance or existance of file.
	TInt paramExistanceFlag=EFalse; 
	
	if( ActionParameters().Count() == 2  )
		{		
		paramExistanceFlag = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) , ETrue);
		paramTestExistanceFlags=ETrue; 
		}

	RMessagingTestUtilityServer ts;
	ts.Connect();
	CleanupClosePushL(ts);

	TBool flagFileExists = ts.FileExists(*autoSendFileName);
	CleanupStack::PopAndDestroy(&ts);

	if (paramTestExistanceFlags==EFalse)
		{ // Maintain old code behaviour.
		
		if( ! flagFileExists )
			{
			User::Leave(KErrNotFound);
			//TestCase().ERR_PRINTF1(_L("File not Found !"));
			//TestCase().SetTestStepResult(EFail);
			}
		}
	else 
		{
			
		if( ( paramExistanceFlag==0) && ( ! (flagFileExists) ) )
			{ // Want to know if file does not exists, and it does not.
			}		
		else if( ( paramExistanceFlag==0) && (flagFileExists ) )
			{ // Want to know if file does not exists, and it does so error.
			TestCase().INFO_PRINTF2(_L("Test Action %S File exists but file not expected to exist."), &KTestActionFindFile);
			User::Leave(KErrGeneral);
			}
		else if( ( paramExistanceFlag==1) && (flagFileExists ) )
			{ // Want to know if file does exists, and it does so okay.
			}
		else if( ( paramExistanceFlag==1) && ( ! (flagFileExists) ) )
			{ // Want to know if file does exists, and it does not.
			TestCase().INFO_PRINTF2(_L("Test Action %S File does not exist but expected to exist."), &KTestActionFindFile);
			User::Leave(KErrNotFound);
			}
		else
			{ // Should never get here.				
			User::Leave(KErrGeneral);
			}
		
		}

//	StoreParameterL<TInt>(TestCase(), flagFileExists, ActionParameters().Parameter(0) );

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionFindFile);
	TestCase().ActionCompletedL(*this);
	}

