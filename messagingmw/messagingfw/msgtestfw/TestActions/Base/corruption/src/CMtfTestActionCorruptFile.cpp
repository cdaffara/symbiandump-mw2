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
// CorruptFile
// [Action Parameters]
// FilePath       <input>: File path of the file to corrupt.
// CorruptionType <input>: Type of corruption.
// ErrorCode	  <output>: (Optional) Returned error code, if not requested then code will
// leave if not KErrNone
// [Action Description]
// Corrupts the specified file, backing-up the original uncorrupted file.
// [APIs Used]
// BaflUtils::FileExists
// RFile::Open
// RFile::Read
// RFile::Close
// RFile::Write
// RFile::Create
// RFs::Delete
// RFs::Rename
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionCorruptFile.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include "bautils.h"

CMtfTestAction* CMtfTestActionCorruptFile::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCorruptFile* self = new(ELeave) CMtfTestActionCorruptFile(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

CMtfTestActionCorruptFile::CMtfTestActionCorruptFile(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionCorruptFile::~CMtfTestActionCorruptFile()
	{
	}

void CMtfTestActionCorruptFile::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionCorruptFile);
	TRAPD(err, RunTestL());
	
	TestCase().INFO_PRINTF2(_L("CMtfTestActionCorruptFile ended. Error = %d"), err);
		
	if (err != KErrNone)
		{
		TestCase().SetTestStepResult(EFail);
		}
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionCorruptFile);	
	TestCase().ActionCompletedL(*this);
	}

// This is used because file paths given in the ini file cannot have spaces
#define KSpaceReplacement '%'
_LIT(KSpace, " ");

void CMtfTestActionCorruptFile::RunTestL()
	{
	TestCase().INFO_PRINTF1(_L("CMtfTestActionCorruptFile started"));

	HBufC* paramFilePath	= ObtainParameterReferenceL<HBufC>(TestCase(),ActionParameters().Parameter(0));

	TInt corruptionType		= ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1));

	TFileName aFilePath;
	aFilePath.Append(*paramFilePath);

	// Put any spaces that should be in the file path
	TInt temp = aFilePath.Locate(KSpaceReplacement);

	while (temp != KErrNotFound)
		{
		aFilePath.Replace(temp, 1, KSpace);
		temp = aFilePath.Locate(KSpaceReplacement);
		}

	TestCase().INFO_PRINTF1(aFilePath);
	TestCase().INFO_PRINTF2(_L("Corruption Type = %d"), corruptionType);

	*paramFilePath = aFilePath;

	RFs aFs;
	aFs.Connect();
	CleanupClosePushL(aFs);

	// corrupt the file
	CorruptFileLC(aFs, *paramFilePath, corruptionType);	
	
	// request file server to read the file as corrupted (for corruption type 1)
	UpdateCorruptionListLC(aFs, *paramFilePath, corruptionType);	
	
	CleanupStack::PopAndDestroy(1, &aFs); // aFs
	}

// This function edits a file called CorruptFileNames.lst which contains a list of files.
// It is read by the file server on start-up and 'pretends' that each file listed is corrupt.
// Note that the given file remains unaltered.
// The CorruptFileNames.lst file will not be read by the file server until it is re-booted, so a re-boot must occur after calling this function.
void CMtfTestActionCorruptFile::UpdateCorruptionListLC(RFs& aFs, const TDesC& aFilePathOld, const TInt& aCorruptionType)
	{
	// This is only for corruption types 1 (or 0 to de-corrupt the file)
	if (!((aCorruptionType == 0) || (aCorruptionType == 1)))
		{
		return;
		}
		
	TBuf8<KMaxFileName> aFilePath;
	aFilePath.Append(aFilePathOld);

	_LIT(KCorruptionListFilePath, "c:\\system\\data\\CorruptFileNames.lst");

	// see if the file to corrupt exists
	TBool fileToCorruptExists = BaflUtils::FileExists(aFs, aFilePathOld);

	if (!fileToCorruptExists)
		{	
		TestCase().INFO_PRINTF1(_L("File to corrupt does not exist"));	
		}
		
	// see if the corruption list exists
	TBool corruptionListExists = BaflUtils::FileExists(aFs, KCorruptionListFilePath);

	TBuf8<512> newList;
	TInt error;
	TBool corruptionListChanged = false;
	RFile corruptionListFile;

	if (corruptionListExists)
		{	
		error = corruptionListFile.Open(aFs,KCorruptionListFilePath,EFileRead);

		if (!(error==KErrNone))
			{
			TestCase().INFO_PRINTF2(_L("Cannot open corruption list. Error = %d"), error);
			User::Leave(error);
			}

		// read the contents of corruption list
		TInt fileSize = 0;
		User::LeaveIfError(corruptionListFile.Size(fileSize));
		
		HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
		TPtr8 bufferPtr(dataBuffer->Des());
		
		User::LeaveIfError(corruptionListFile.Read(bufferPtr));

		TBool matchFound;
		
		TInt temp = bufferPtr.Locate('\n');
					
		// read each line
		while (temp != KErrNotFound)
			{
			matchFound = false;
			
			TPtrC8 thisLine(bufferPtr.Left(temp+1));
							
			// see if this line contains the given file path
			if (temp > aFilePath.Length())
				{
				if (thisLine.Left(aFilePath.Length()).Match(aFilePath) != KErrNotFound)
					{
					matchFound = true;
					}
				}
			
			// remove the given file path from the list of files (it will be added again if corruption type is 1)
			if (!matchFound)
				{
				newList.Append(thisLine);		
				}
			else if (aCorruptionType == 0)
				{
				corruptionListChanged = true;
				}

			bufferPtr.Delete(0, temp+1);
			temp = bufferPtr.Locate('\n');
			}
			
		corruptionListFile.Close();
		
		CleanupStack::Pop(1); // dataBuffer
		}
		
	// add given file to corruption list
	if (aCorruptionType == 1)
		{
		newList.Append(aFilePath);
		newList.Append(_L(", -20, every\r\n"));	// the file will appear corrupted every time it is accessed
//		newList.Append(_L(", -20, ONCE\r\n"));
		corruptionListChanged = true;
		}
	
	if (corruptionListChanged)
		{
		// delete old list
		error = aFs.Delete(KCorruptionListFilePath);
		
		if (!((error==KErrNone) || (error==KErrNotFound)))
			{
			TestCase().INFO_PRINTF1(_L("Could not delete old corruption list"));	
			User::Leave(error);
			}
			
		// create new list
		error = corruptionListFile.Create(aFs,KCorruptionListFilePath,EFileWrite);

		if (!(error==KErrNone))
			{
			TestCase().INFO_PRINTF2(_L("Cannot create new corruption list. Error = %d"), error);
			User::Leave(error);
			}

		error = corruptionListFile.Write(newList);

		corruptionListFile.Close();

		TestCase().INFO_PRINTF1(_L("Corruption list updated."));	
		}
	}
	
// This function edits the given file to make it corrupted.
// The original file is backed up first, and is restored if corruption type is 0.
// corruption type 0: no corruption
// corruption type 1: file server returns error KErrCorrupt
// corruption type 2: file does not exist
// corruption type 3: file contains zero-length data
// corruption type 4: file contains random data
void CMtfTestActionCorruptFile::CorruptFileLC(RFs& aFs, const TDesC& aFilePath, const TInt& aCorruptionType)
	{
	_LIT(KUncorruptedFileFolder, "c:\\msgtest\\TestFramework\\ScriptedTestCases\\Corruption\\");

	// calculate a file path for the backed-up (original) version of the file
	TFileName uncorruptedFilePath;
	TParse parseFile;
	
	parseFile.Set(aFilePath, NULL, NULL);

	uncorruptedFilePath.Append(KUncorruptedFileFolder);
	uncorruptedFilePath.Append(parseFile.NameAndExt());
	uncorruptedFilePath.Append(_L(".uncorrupted"));
		
	TInt error;
	
	// see if there is an uncorrupted version of this file
	TBool uncorruptedFileExists = BaflUtils::FileExists(aFs, uncorruptedFilePath);

	if (aCorruptionType == 0)	// no corruption
		{
		if(uncorruptedFileExists)
			{
			// over-write corrupted file with uncorrupted version
			TestCase().INFO_PRINTF1(_L("Uncorrupted version of file exists"));	

			// delete corrupted version of file
			error = aFs.Delete(aFilePath);
			
			if (!((error==KErrNone) || (error==KErrNotFound)))
				{
				TestCase().INFO_PRINTF1(_L("Could not delete corrupted version of file"));	
				User::Leave(error);
				}
					
			// put back uncorrupted version of file
			error = aFs.Rename(uncorruptedFilePath, aFilePath);

			if (!(error==KErrNone))
				{
				TestCase().INFO_PRINTF1(_L("Could not put back uncorrupted version of file"));	
				User::Leave(error);
				}
				
			TestCase().INFO_PRINTF1(_L("File uncorrupted"));	
			}
		else
			{
			TestCase().INFO_PRINTF1(_L("Uncorrupted version of file doesn't exist"));	
			}
		}
	else if ((aCorruptionType == 2) || (aCorruptionType == 3) || (aCorruptionType == 4))
		{
		if(uncorruptedFileExists)
			{
			TestCase().INFO_PRINTF1(_L("Uncorrupted version of file exists"));	

			// delete uncorrupted version of file
			error = aFs.Delete(uncorruptedFilePath);
			
			if (!((error==KErrNone) || (error==KErrNotFound)))
				{
				TestCase().INFO_PRINTF1(_L("Could not delete uncorrupted version of file"));	
				User::Leave(error);
				}
			}
			
		// move file to be corrupted
		error = aFs.Rename(aFilePath, uncorruptedFilePath);
		
		if (!((error==KErrNone) || (error==KErrNotFound)))
			{
			TestCase().INFO_PRINTF2(_L("Cannot move file to be corrupted. Error = %d"), error);
			User::Leave(error);
			}
			
		if (error==KErrNotFound)
			{
			TestCase().INFO_PRINTF1(_L("File to be corrupted does not exist"));
			}
			
		if ((aCorruptionType == 3) || (aCorruptionType == 4))
			{
			RFile fileHandle;
			
			// need to create an empty file
			error = fileHandle.Create(aFs,aFilePath,EFileWrite);

			if (!(error==KErrNone))
				{
				TestCase().INFO_PRINTF2(_L("Cannot create empty file. Error = %d"), error);
				User::Leave(error);
				}
				
			CleanupClosePushL(fileHandle);
			
			if (aCorruptionType == 4)
				{
				// need to add random data to file
				_LIT(KRandomDataFilePath, "c:\\msgtest\\TestFramework\\ScriptedTestCases\\Corruption\\data\\randomdata.txt");
		
				RFile randomDataFile;
				
				error = randomDataFile.Open(aFs,KRandomDataFilePath,EFileRead);

				if (!(error==KErrNone))
					{
					TestCase().INFO_PRINTF2(_L("Cannot open random data file. Error = %d"), error);
					User::Leave(error);
					}

				CleanupClosePushL(randomDataFile);
				
				// read in random data
				TInt fileSize = 0;
				User::LeaveIfError(randomDataFile.Size(fileSize));
				
				HBufC8* dataBuffer = HBufC8::NewLC(fileSize);
				TPtr8 bufferPtr(dataBuffer->Des());
				
				User::LeaveIfError(randomDataFile.Read(bufferPtr));

				// write random data to file
				error = fileHandle.Write(bufferPtr);
				
				CleanupStack::PopAndDestroy(1, dataBuffer); // dataBuffer
				CleanupStack::PopAndDestroy(1, &randomDataFile); // randomDataFile
				
				if (!(error==KErrNone))
					{
					TestCase().INFO_PRINTF2(_L("Cannot write random data to file. Error = %d"), error);
					User::Leave(error);
					}
					
				TestCase().INFO_PRINTF1(_L("File corrupted with random data"));
				}
			else
				{
				TestCase().INFO_PRINTF1(_L("File corrupted with zero-length data"));	
				}
				
			CleanupStack::PopAndDestroy(1, &fileHandle); // fileHandle
			}
		else
			{
			TestCase().INFO_PRINTF1(_L("File corrupted by removing file"));	
			}
		}
	}
