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
// LoadSmsSettings
// [Action Parameters]
// newFreeValue     input 		TInt
// [Action Description]
// Setting the Disk space
// __ACTION_INFO_END__
// 
//

#include <f32file.h>

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionSetDiskSpace.h"

// Location of directory for reserving disk space
_LIT(KTempDiskSpaceDirName, "C:\\sms\\temp\\");


/**
  Function		: NewL
  Description	: 
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param		: aActionParams - CMtfTestActionParameters 
  @return		: CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSetDiskSpace object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSetDiskSpace::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetDiskSpace* self = new (ELeave) CMtfTestActionSetDiskSpace(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function		: CMtfTestActionSetDiskSpace
  Description	: Constructor
  @internalTechnology
  @param		: aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return		: N/A
  @pre none
  @post none
*/
CMtfTestActionSetDiskSpace::CMtfTestActionSetDiskSpace(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function		: ~CMtfTestActionSetDiskSpace
  Description	: Destructor
  @internalTechnology
  @param		:
  @return		: 
  @pre 
  @post 
*/
CMtfTestActionSetDiskSpace::~CMtfTestActionSetDiskSpace()
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
void CMtfTestActionSetDiskSpace::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetDiskSpace );
	
	RFs fs;
	User::LeaveIfError(fs.Connect());

	TInt newFreeValue = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

	TestCase().INFO_PRINTF2(_L("Setting Drive C free disk space to %ld bytes."), newFreeValue);

	//
	// Check the current disk space available level...
	//
	TVolumeInfo  volumeInfo;
	
	User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
	TestCase().INFO_PRINTF2(_L("  Drive C currently has %ld bytes free."), volumeInfo.iFree);

	//
	// Check that this is possible...
	//	
	if (volumeInfo.iFree < newFreeValue)
		{
		TestCase().INFO_PRINTF1(_L("  Drive C already has too little free space!"));
		User::Leave(KErrArgument);
		}

	//
	// Ensure the temporary directory exists...
	//
	TInt ret;
	
	ret = fs.MkDir(KTempDiskSpaceDirName);
	if (ret != KErrNone  &&  ret != KErrAlreadyExists)
		{
		User::Leave(ret);
		}

	//
	// Work out how many files to create and their sizes. A full temp file is
	// 1GB in size, the last temp file will handle the remainder. Then loop
	// through and create them all...
	//
	TInt64  maxSizeOfTempFile  = (1024*1024*1024); 
	TInt64  diskSpaceToReserve = volumeInfo.iFree - newFreeValue;
	TInt64  numOfFullTempFiles = (diskSpaceToReserve / maxSizeOfTempFile);
	TInt64  lastTempFileSize   = (diskSpaceToReserve % maxSizeOfTempFile);
	TBuf<32>  tempFileName;
	RFile  file;

	for(TInt fileNum = 1;  fileNum <= numOfFullTempFiles;  fileNum++)
		{
		tempFileName.Copy(KTempDiskSpaceDirName);
		tempFileName.AppendFormat(_L("reserved.%d"), fileNum);

		TestCase().INFO_PRINTF3(_L("  Creating %S of %ld bytes."), &tempFileName, maxSizeOfTempFile);

		User::LeaveIfError(file.Replace(fs, tempFileName, EFileWrite));
		User::LeaveIfError(file.SetSize(maxSizeOfTempFile));	
		file.Close();
		}

	if (lastTempFileSize > 0)
		{
		User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
		TestCase().INFO_PRINTF2(_L("  Drive C now has %ld bytes free."), volumeInfo.iFree);
		
		if( lastTempFileSize > volumeInfo.iFree )
			{	
			lastTempFileSize = volumeInfo.iFree;
			}
		
		
		tempFileName.Copy(KTempDiskSpaceDirName);
		tempFileName.AppendFormat(_L("reserved.%d"), numOfFullTempFiles+1);

		TestCase().INFO_PRINTF3(_L("  Creating %S of %ld bytes."), &tempFileName, lastTempFileSize);

		User::LeaveIfError(file.Replace(fs, tempFileName, EFileWrite));
		User::LeaveIfError(file.SetSize(lastTempFileSize));	
		file.Close();
		}

	//
	// Recheck the free space now...
	//	
	User::LeaveIfError(fs.Volume(volumeInfo, EDriveC));
	TestCase().INFO_PRINTF2(_L("  Drive C now has %ld bytes free."), volumeInfo.iFree);


	fs.Close();

	TestCase().INFO_PRINTF2( _L("Test Action %S completed."), &KTestActionSetDiskSpace );
	TestCase().ActionCompletedL( *this );	
	}


