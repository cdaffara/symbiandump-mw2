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
// FreeDiskSpace
// [Action Parameters]
// [Action Description]
// Free the disk space
// __ACTION_INFO_END__
// 
//

#include <f32file.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionFreeDiskSpace.h"

_LIT(KSMSUResourceFile, "C:\\private\\101f7989\\sms\\smsu.rsc");
// Location of directory for reserving disk space
_LIT(KTempDiskSpaceDirName, "C:\\sms\\temp\\");


/**
  Function		: NewL
  Description	: 
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param		: aActionParams - CMtfTestActionParameters 
  @return		: CMtfTestAction* - a base class pointer to the newly created CMtfTestActionFreeDiskSpace object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionFreeDiskSpace::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionFreeDiskSpace* self = new (ELeave) CMtfTestActionFreeDiskSpace(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function		: CMtfTestActionFreeDiskSpace
  Description	: Constructor
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return		: N/A
  @pre none
  @post none
*/
CMtfTestActionFreeDiskSpace::CMtfTestActionFreeDiskSpace(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{}
	
/**
  Function		: ~CMtfTestActionFreeDiskSpace
  Description	: Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionFreeDiskSpace::~CMtfTestActionFreeDiskSpace()
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
void CMtfTestActionFreeDiskSpace::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionFreeDiskSpace);

	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	
	//
	// Check if the directories actually exists, and contains something.
	// There is no point writing logging when nothing happened.
	//
   	TInt  ret;

	ret = fs.RmDir(KTempDiskSpaceDirName);
	if (ret == KErrNone  ||  ret == KErrNotFound  ||  ret == KErrPathNotFound)
		{
		return;
		}
	
	//
	// We need to delete the folder and it's contents...
	//
	TestCase().INFO_PRINTF1(_L("Releasing Drive C reserved disk space."));

	//
	// Check the current disk space available level...
	//
	TVolumeInfo  volumeInfo;
	
	User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
	TestCase().INFO_PRINTF2(_L("  Drive C currently has %ld bytes free."), volumeInfo.iFree);
	
	//
	// Delete the whole temporary directory. Using the File Manager class
	// is the easiest way...
	//
 	CFileMan*  fileManager = CFileMan::NewL(fs);
 	CleanupStack::PushL(fileManager);
   	
   	ret = fileManager->RmDir(KTempDiskSpaceDirName);
   	if (ret != KErrPathNotFound  &&   ret != KErrNone)
		{
	TestCase().INFO_PRINTF2(_L("  Delete of temporary files returned %d"), ret);
		User::Leave(ret);
		}

	CleanupStack::PopAndDestroy(fileManager);

	//
	// Displace the new free disk space value...
	//	
	User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
	TestCase().INFO_PRINTF2(_L("  Drive C now has %ld bytes free."), volumeInfo.iFree);	
	
	
	//
	// Remove the RSC file...
	// Removes the low and high limits and returns the SMSU.RSC to default.
	
	
	ret = fs.Delete(KSMSUResourceFile);
	if (ret != KErrNone  &&  ret != KErrNotFound)
		{
		User::Leave(ret);
		}

	fs.Close();

	TestCase().INFO_PRINTF2( _L("Test Action %S completed."), &KTestActionFreeDiskSpace );
	TestCase().ActionCompletedL( *this );	
	}


