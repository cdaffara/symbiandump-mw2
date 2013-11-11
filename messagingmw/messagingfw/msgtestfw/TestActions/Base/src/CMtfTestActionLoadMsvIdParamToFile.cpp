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
//

#include "CMtfTestActionLoadMsvIdParamToFile.h"

#include <e32std.h>
#include <e32base.h>
#include <s32file.h>

#include <msvipc.h>
#include "MCLIENT.H"
#include "MSERVER.H"


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionLoadMsvIdParamToFile object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionLoadMsvIdParamToFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionLoadMsvIdParamToFile* self = new (ELeave) CMtfTestActionLoadMsvIdParamToFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionLoadMsvIdParamToFile
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionLoadMsvIdParamToFile::CMtfTestActionLoadMsvIdParamToFile(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionLoadMsvIdParamToFile
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionLoadMsvIdParamToFile::~CMtfTestActionLoadMsvIdParamToFile()
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




void CMtfTestActionLoadMsvIdParamToFile::ExecuteActionL()
	{

	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionLoadMsvIdParamToFile);
	HBufC* paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));
	
	RFs fs;
 	User::LeaveIfError( fs.Connect() );
 	CleanupClosePushL(fs);
	
	RFileReadStream rf;
	TInt err = rf.Open( fs , *paramFilePath , EFileRead );
	User::LeaveIfError(err);
	rf.PushL();

	TMsvId messageId;
	rf >> messageId; 

	StoreParameterL<TMsvId>(TestCase(), messageId, ActionParameters().Parameter(0) );
	
	rf.Pop();
	rf.Release();
	
	CleanupStack::PopAndDestroy(); // fs
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionLoadMsvIdParamToFile);
	TestCase().ActionCompletedL(*this);	

	
	}
