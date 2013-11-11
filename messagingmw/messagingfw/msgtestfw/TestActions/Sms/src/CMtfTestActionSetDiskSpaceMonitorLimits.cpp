// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// SetDiskSpaceMonitorLimits
// [Action Parameters]
// lowLimit     input 		TInt
// highLimit    input 		TInt
// [Action Description]
// Setting Disk space monitor limits
// __ACTION_INFO_END__
// 
//


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionSetDiskSpaceMonitorLimits.h"

// Location and name of the RSC file.
_LIT(KSMSUResourceDir, "C:\\private\\101f7989\\sms\\");
_LIT(KSMSUResourceFile, "C:\\private\\101f7989\\sms\\smsu.rsc");



/**
  Function		: NewL
  Description	: 
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param		: aActionParams - CMtfTestActionParameters 
  @return		: CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSetDiskSpaceMonitorLimits object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSetDiskSpaceMonitorLimits::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetDiskSpaceMonitorLimits* self = new (ELeave) CMtfTestActionSetDiskSpaceMonitorLimits(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function		: CMtfTestActionSetDiskSpaceMonitorLimits
  Description	: Constructor
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return		: N/A
  @pre none
  @post none
*/
CMtfTestActionSetDiskSpaceMonitorLimits::CMtfTestActionSetDiskSpaceMonitorLimits(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function		: ~CMtfTestActionSetDiskSpaceMonitorLimits
  Description	: Destructor
  @internalTechnology
  @param		:
  @return		: 
  @pre 
  @post 
*/
CMtfTestActionSetDiskSpaceMonitorLimits::~CMtfTestActionSetDiskSpaceMonitorLimits()
	{
	}

/**
  Function		: ExecuteActionL
  Description	: Entry point for the this test action in the test framework
  @internalTechnology
  @param		: none
  @return		: void
  @pre none 
  @post none
*/
void CMtfTestActionSetDiskSpaceMonitorLimits::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetDiskSpaceMonitorLimits);

	RFs fs;
	User::LeaveIfError(fs.Connect());	

	TInt lowLimit = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));
	TInt highLimit = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	

	TestCase().INFO_PRINTF3(_L("Setting high and low .RSC limits to %ld and %ld."), highLimit, lowLimit);

	__ASSERT_ALWAYS(lowLimit  < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(highLimit < 0x7fffffff, User::Leave(KErrArgument));
	__ASSERT_ALWAYS(lowLimit  < highLimit, User::Leave(KErrArgument));

	//
	// Data for the SMSU resource file. The low limit is written at position
	// 20 and the high limit at position 24.
	//
	const TInt  smsuRscSize = 34;
	TChar  smsuRscData[smsuRscSize] =
				{0x6b, 0x4a, 0x1f, 0x10, 0x00, 0x00, 0x00, 0x00,
   	 			 0x00, 0x00, 0x00, 0x00, 0x19, 0xfd, 0x48, 0xe8,
				 0x01, 0x04, 0x00, 0x00, 0x78, 0x56, 0x34, 0x12,
				 0x87, 0x65, 0x43, 0x21, 0x14, 0x00, 0x18, 0x00, 
   		 		 0x1c, 0x00};
    						  
	smsuRscData[20] = (lowLimit  >>  0) & 0xff;
	smsuRscData[21] = (lowLimit  >>  8) & 0xff;
	smsuRscData[22] = (lowLimit  >> 16) & 0xff;
	smsuRscData[23] = (lowLimit  >> 24) & 0xff;
	smsuRscData[24] = (highLimit >>  0) & 0xff;
	smsuRscData[25] = (highLimit >>  8) & 0xff;
	smsuRscData[26] = (highLimit >> 16) & 0xff;
	smsuRscData[27] = (highLimit >> 24) & 0xff;

	TBuf8<smsuRscSize>  smsuRscBuffer;

	for (TInt index = 0;  index < smsuRscSize;  index++)
		{
		smsuRscBuffer.Append(smsuRscData[index]);
		}

	//
	// Ensure the target directory exists...
	//
	TInt  ret;
	
	ret = fs.MkDir(KSMSUResourceDir);
	if (ret != KErrNone  &&  ret != KErrAlreadyExists)
		{
		User::Leave(ret);
		}

	//
	// Write the RSC file to the private C:\ directory...
	//
	RFile  file;

	User::LeaveIfError(file.Replace(fs, KSMSUResourceFile, EFileWrite));
	CleanupClosePushL(file);
	User::LeaveIfError(file.Write(smsuRscSize, smsuRscBuffer));
	CleanupStack::PopAndDestroy(&file);

	fs.Close();
	
	TestCase().INFO_PRINTF2( _L("Test Action %S completed."), &KTestActionSetDiskSpaceMonitorLimits );
	TestCase().ActionCompletedL( *this );	
	}


