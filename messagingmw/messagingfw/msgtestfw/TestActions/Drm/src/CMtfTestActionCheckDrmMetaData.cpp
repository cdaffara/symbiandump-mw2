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
// CheckDrmMetaData
// [Action Parameters]
// metaDataFile			input		HBufC, path and name of the meta data file to check
// numberOfFiles		input		TInt, number of expected meta data files
// [Action Description]
// CheckDrmReceipt Test Action is intended to check the DRM meta data file produced
// by the DRM test agent and verify its contents
// [APIs Used]	
// RFile::Open
// RFile::Read
// TDesC8::CompareF
// __ACTION_INFO_END__
//

//! @file

#include "CMtfTestActionCheckDrmMetaData.h"

#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

_LIT8(KContentTypeString, "Content-Type: application/");
_LIT(KFileFilter, "*.txt");

/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionCheckDrmMetaData object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionCheckDrmMetaData::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionCheckDrmMetaData* self = new (ELeave) CMtfTestActionCheckDrmMetaData(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
}

/**
  Function : CMtfTestActionCheckDrmMetaData
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionCheckDrmMetaData::CMtfTestActionCheckDrmMetaData(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
{
}
	
/**
  Function : ~CMtfTestActionCheckDrmMetaData
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionCheckDrmMetaData::~CMtfTestActionCheckDrmMetaData()
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
void CMtfTestActionCheckDrmMetaData::ExecuteActionL()
{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCheckDrmMetaData);
	HBufC* metaDataFile = ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));
	TInt numberOfFiles = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));
	
	RFs fs;
	RFile file;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	// Get the contents of the directory
	CDir* dir = NULL;
	
	{
		TFileName dirString;
		dirString.Append(*metaDataFile);
		dirString.Append(KFileFilter);
		
		User::LeaveIfError(fs.GetDir(dirString, KEntryAttMaskSupported, ESortByName, dir));
		CleanupStack::PushL(dir);
	}
	
	TBool success = ETrue;
	TInt count = dir->Count();
	
	if (count != numberOfFiles)
	{
		// no meta data generated. failed.
		TestCase().ERR_PRINTF3(_L("CMtfTestActionCheckDrmMetaData: FAILED! Incorrect number of files, expected [%d], found [%d]"), numberOfFiles, count);
		success = EFalse;
	}
	else
	{
		for (TInt i = 0; i < count; i++)
		{
			// open the file
			TFileName fileName;
			fileName.Append(*metaDataFile);
			fileName.Append((*dir)[i].iName);
			
			User::LeaveIfError(file.Open(fs, fileName, EFileShareAny | EFileRead));
			CleanupClosePushL(file);
			
			// Parse the file and verify its contents
			TInt fileSize = 0;
			User::LeaveIfError(file.Size(fileSize));
			
			HBufC8* fileContent = HBufC8::NewLC(fileSize);
			TPtr8 des = fileContent->Des();
			
			User::LeaveIfError(file.Read(des, fileSize));
			
			if (fileContent->FindF(KContentTypeString) == KErrNotFound)
			{
				TestCase().ERR_PRINTF1(_L("CMtfTestActionCheckDrmMetaData: FAILED! Content-Type not found. Meta-data file corrupt!"));
				success = EFalse;
			}
			
			CleanupStack::PopAndDestroy(2); // file, fileContent
		}
	}

	if (count > 0)
	{
		// delete the meta data files
		for (TInt i = 0; i < count; i++)
		{
			TFileName fileName;
			fileName.Append(*metaDataFile);
			fileName.Append((*dir)[i].iName);
			TInt err = fs.Delete(fileName);
			if (err != KErrNone)
			{
				TestCase().ERR_PRINTF2(_L("CMtfTestActionCheckDrmMetaData: FAILED! Couldn't delete Meta-data file: [%d]!"), err);
				success = EFalse;
			}
		}
	}
	
	CleanupStack::PopAndDestroy(2); // fs, dir
	
	if (success)
	{
		TestCase().INFO_PRINTF1(_L("CMtfTestActionCheckDrmMetaData: Meta-data check successful. Test case PASSED."));
		TestCase().SetTestStepResult(EPass);
	}
	else
	{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionCheckDrmMetaData: Meta-data check failed. Test case FAILED."));
		TestCase().SetTestStepResult(EFail);
	}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCheckDrmMetaData);
	TestCase().ActionCompletedL(*this);
}
