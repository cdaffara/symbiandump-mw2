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

#include <e32std.h>
#include <f32file.h>
#include <caf/caf.h>
#include "Reftestagentimportfile.h"
#include "rtaImport.h"
#include "reftestagentconstants.h"

using namespace ContentAccess;

_LIT8(KReceiptMimeType,"text/plain");
_LIT(KInputDataLogFile, "c:\\logs\\rta\\WriteDataInput.dat");


CRefTestAgentImportFile* CRefTestAgentImportFile::NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName)
	{
	CRefTestAgentImportFile* self=new(ELeave) CRefTestAgentImportFile(ETrue);
	CleanupStack::PushL(self);
	self->ConstructL(aMimeType, aMetaDataArray, aOutputDirectory, aSuggestedFileName);
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentImportFile* CRefTestAgentImportFile::NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray)
	{
	CRefTestAgentImportFile* self=new(ELeave) CRefTestAgentImportFile(EFalse);
	CleanupStack::PushL(self);
	self->ConstructL(aMimeType, aMetaDataArray, KNullDesC(), KNullDesC());
	CleanupStack::Pop(self);
	return self;
	}
	
CRefTestAgentImportFile::CRefTestAgentImportFile(TBool aAgentCreatesOutputFiles) : 
iImportStatus(EInProgress), iOutputFileOpen(EFalse), 
iAgentCreatesOutputFiles(aAgentCreatesOutputFiles), iWriteComplete(EFalse)
	{
	}

CRefTestAgentImportFile::~CRefTestAgentImportFile()
	{
	delete iOutputDirectory;
	delete iSuggestedFileName;
	delete iOutputFileName;
	delete iImporter;
	iFs.Close();
	iOutputFiles.ResetAndDestroy();
	iOutputFiles.Close();
	}
  
void CRefTestAgentImportFile::ConstructL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName)
	{
	TPtrC suggestedFileName(aSuggestedFileName);	
	TPtrC outputDirectory(aOutputDirectory);
	
	// make sure any suggested extension is removed
	// otherwise our recognizer won't work
	TInt pos = aSuggestedFileName.LocateF(TChar(KExtDelimiter));
	if(pos != KErrNotFound)
		{
		// set the length so the extension is not included
		suggestedFileName.Set(aSuggestedFileName.Left(pos));
		}

	// If no output directory is suggested default to "c:\\" 
	if(outputDirectory.Length() == 0)
		{
		outputDirectory.Set(KRtaDefaultOutputDirectory());
		}

	// If no file name is suggested default to "output" 
	if(suggestedFileName.Length() == 0)
		{
		suggestedFileName.Set(KRtaDefaultOutputFile());
		}
	
	// Only files containing rights need to be imported
	if(aMimeType.CompareF(KXmlRORights()) != 0 && aMimeType.CompareF(KXmlTriggerRights()) != 0)
		{
		User::Leave(KErrCANotSupported);
		}

	// create buffer to store the output directory supplied by the client
	// Append a trailing slash if there is no slash present
	iOutputDirectory = HBufC::NewL(outputDirectory.Length() + 1);
	iOutputDirectory->Des().Copy(outputDirectory);
	if(TChar(outputDirectory[outputDirectory.Length() - 1]) != TChar(KPathDelimiter))
		{
		iOutputDirectory->Des().Append(TChar(KPathDelimiter));
		}
	
	// Create suggested file name by combining output directory and suggested name
	iSuggestedFileName = HBufC::NewL(iOutputDirectory->Des().Length() + suggestedFileName.Length() + KRtaExtensionContent().Length());
	iSuggestedFileName->Des().Copy(*iOutputDirectory);
	iSuggestedFileName->Des().Append(suggestedFileName);
	iSuggestedFileName->Des().Append(KRtaExtensionContent());

	// Create a file handle
	User::LeaveIfError(iFs.Connect());
	
	// create importer object
	iImporter = CRefTestAgentImport::NewL(*iSuggestedFileName);

	DumpMetaData(aMetaDataArray);
	
	// create input data log
	RFile inputDataFile;
	inputDataFile.Replace(iFs, KInputDataLogFile(), EFileWrite | EFileStream);
	inputDataFile.Close();
	}

void CRefTestAgentImportFile::DumpMetaData(const CMetaDataArray& aMetaDataArray)
	{
	_LIT(KMetaDataLogFile, "c:\\logs\\rta\\metadata");
	_LIT(KExt, ".log");
	
	TInt id(0);
	TInt err(KErrNone);
	TFileName filename;
	RFile file;
	do	{
		filename.SetLength(0);
		filename.Append(KMetaDataLogFile);
		filename.AppendNum(++id);
		filename.Append(KExt);
		err = file.Create(iFs,filename,EFileWrite);
		} while(err == KErrAlreadyExists);

	if(err == KErrNone)
		{
		for(TInt i=0;i<aMetaDataArray.Count();i++)
			{
			file.Write(aMetaDataArray[i].Field8());
			file.Write(_L8(" "));
			file.Write(aMetaDataArray[i].Data8());
			file.Write(_L8("\r\n"));
			}
		file.Close();
		}
	}
	
	
void CRefTestAgentImportFile::LogInputData(const TDesC8& aInput)
	{
	RFile inputDataFile;
	TInt pos = 0;
	TInt err = inputDataFile.Open(iFs, KInputDataLogFile(), EFileWrite | EFileStream);
	if(err == KErrNone)
		{
		inputDataFile.Seek(ESeekEnd, pos);
		inputDataFile.Write(aInput);
		inputDataFile.Close();
		}
	}

TInt CRefTestAgentImportFile::WriteData(const TDesC8& aData)
	{
	LogInputData(aData);
	
	// Process the data received from the client
	TRAPD(err, iImporter->WriteL(aData));

	if(err == KErrNone)
		{
		TRAP(err,iImporter->ProcessRightsL());	
		}
	return err;
	}

TInt CRefTestAgentImportFile::WriteDataComplete()
	{
	TInt err = KErrNone;
	
	
	if(iOutputFileOpen)
		{
		// add the output content file to the array of output files
		TRAP(err, AddOutputFileL(*iOutputFileName, EContent, KRtaMimeContent()));
		}

	if(err == KErrNone)
		{
		iWriteComplete = ETrue;
		
		// this starts the rights processing
		TRAP(err,iImporter->WriteCompleteL());
		
		// Agent will ask for rights receipt files
		if(err == KErrCANewFileHandleRequired && iAgentCreatesOutputFiles)
			{
			while(err == KErrCANewFileHandleRequired)
				{
				TFileName receiptFileName;
				iImporter->GetSuggestedOutputFileName(receiptFileName);
				receiptFileName.Insert(0, *iOutputDirectory);
								
				RFile receiptFile;
				err = receiptFile.Create(iFs, receiptFileName, EFileStream | EFileWrite | EFileShareAny);
				if(err == KErrNone)
					{
					// TRAP(err, iImporter->ContinueWithNewOutputFileL(receiptFile));
					receiptFile.Close();
					
					// Add the receipt to the list of output files
					TRAP_IGNORE(AddOutputFileL(receiptFileName, EReceipt, KReceiptMimeType()));
					}
				}
			}
		}
	return err;
	}


void CRefTestAgentImportFile::AddOutputFileL(const TDesC& aFileName, TOutputType aOutputType, const TDesC8& aMimeType)
	{
	CSupplierOutputFile *temp = NULL; 
	
	temp = CSupplierOutputFile::NewL(aFileName, aOutputType, aMimeType);
	CleanupStack::PushL(temp);
	User::LeaveIfError(iOutputFiles.Append(temp));
	CleanupStack::Pop(temp);

	iOutputFileOpen = EFalse;
	delete iOutputFileName;
	iOutputFileName = NULL;
	}

		
void CRefTestAgentImportFile::WriteData(const TDesC8& aData, TRequestStatus &aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	
	// call syncronous WriteData
	TInt rVal = WriteData(aData);
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}

void CRefTestAgentImportFile::WriteDataComplete(TRequestStatus &aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	// call syncronous WriteDataComplete
	TInt rVal = WriteDataComplete();
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}
	
TInt CRefTestAgentImportFile::OutputFileCountL() const
	{
	return iOutputFiles.Count();
	}

CSupplierOutputFile& CRefTestAgentImportFile::OutputFileL(TInt aIndex)
	{
	return *iOutputFiles[aIndex];
	}

TImportStatus CRefTestAgentImportFile::GetImportStatus() const
	{
	return iImportStatus;
	}	


TInt CRefTestAgentImportFile::GetSuggestedOutputFileExtension(TDes& aFileExtension)
	{
	return iImporter->GetSuggestedOutputFileExtension(aFileExtension);
	}

TInt CRefTestAgentImportFile::GetSuggestedOutputFileName(TDes& aFileName)
	{
	return iImporter->GetSuggestedOutputFileName(aFileName);
	}
		
TInt CRefTestAgentImportFile::ContinueWithNewOutputFile(RFile& /*aFile*/, const TDesC& /*aFileName*/)
	{
/*	TRAPD(err, iImporter->ContinueWithNewOutputFileL(aFile));
	if(err == KErrNone)
		{
		if(!iWriteComplete)
			{
			TRAP(err, iOutputFileName = aFileName.AllocL());
			iOutputFileOpen = ETrue;			
			}
		else
			{
			// Add the receipt to the list of output files
			TRAP(err, AddOutputFileL(aFileName, EReceipt, KReceiptMimeType()));
			}
		}*/
	return KErrNone;
	}

void CRefTestAgentImportFile::ContinueWithNewOutputFile(RFile& aFile, const TDesC& aFileName, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	// call syncronous WriteDataComplete
	TInt rVal = ContinueWithNewOutputFile(aFile, aFileName);
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}

