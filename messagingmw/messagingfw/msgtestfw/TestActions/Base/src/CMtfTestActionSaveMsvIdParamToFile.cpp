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

#include "CMtfTestActionSaveMsvIdParamToFile.h"

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSaveMsvIdParamToFile object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSaveMsvIdParamToFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSaveMsvIdParamToFile* self = new (ELeave) CMtfTestActionSaveMsvIdParamToFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSaveMsvIdParamToFile
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSaveMsvIdParamToFile::CMtfTestActionSaveMsvIdParamToFile(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSaveMsvIdParamToFile
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSaveMsvIdParamToFile::~CMtfTestActionSaveMsvIdParamToFile()
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




void CMtfTestActionSaveMsvIdParamToFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSaveMsvIdParamToFile);
		
	TMsvId messageId = ObtainValueParameterL<TMsvId>( TestCase(), ActionParameters().Parameter(0) );										
	HBufC* paramFilePath = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(1));
 
 	
 	RFs fs;
 	User::LeaveIfError( fs.Connect() );
 	CleanupClosePushL( fs );
 	
	TInt err = fs.MkDir( *paramFilePath ); 
	
	if ( ! ( (err == KErrNone ) || ( err == KErrAlreadyExists ) ) )
	{	
		User::LeaveIfError( err );
	}
	// else dir created successfully or already created.
	
	RFileWriteStream rf;
	err = rf.Open( fs , *paramFilePath , EFileWrite );
	if ( err == KErrNotFound )
	{
		err = rf.Create( fs, *paramFilePath , EFileWrite );
	}	
	User::LeaveIfError(err);
	
	rf.PushL();
		// Writes may leave.
		
	rf << messageId; 

	rf.CommitL();
	rf.Pop();
	rf.Release();
			
 	CleanupStack::PopAndDestroy(); // fs

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSaveMsvIdParamToFile);
	TestCase().ActionCompletedL(*this);	
	
	
	}
