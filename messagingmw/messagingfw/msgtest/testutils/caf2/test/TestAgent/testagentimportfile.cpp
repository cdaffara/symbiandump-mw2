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
#include <s32file.h>
#include <caf/caferr.h>
#include <caf/supplieroutputfile.h>
#include <caf/metadata.h>
#include <caf/metadataarray.h>
#include "testagentimportfile.h"



using namespace ContentAccess;

_LIT(KOutDir,"C:\\MsgLogs\\CafMetaData\\");
_LIT(KDefaultName,"MetaData");
_LIT(KExt,"txt");

CTestAgentImportFile* CTestAgentImportFile::NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName)
	{
	CTestAgentImportFile* self=new(ELeave) CTestAgentImportFile(ETrue);
	CleanupStack::PushL(self);
	self->ConstructL(aMimeType, aMetaDataArray, aOutputDirectory, aSuggestedFileName);
	CleanupStack::Pop(self);
	return self;
	}

CTestAgentImportFile* CTestAgentImportFile::NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray)
	{
	CTestAgentImportFile* self=new(ELeave) CTestAgentImportFile(EFalse);
	CleanupStack::PushL(self);
	self->ConstructL(aMimeType, aMetaDataArray, KNullDesC(), KNullDesC());
	CleanupStack::Pop(self);
	return self;
	}
	
CTestAgentImportFile::CTestAgentImportFile(TBool aAgentCreatesOutputFiles) : iAgentCreatesOutputFiles(aAgentCreatesOutputFiles)
	{
	iImportStatus = EInProgress;
	}

CTestAgentImportFile::~CTestAgentImportFile()
	{
	if(iFileOpen)
		{
		iFile.Close();
		iFileOpen = EFalse;
		}
	if(iOutputFileName)
		{
		iFs.Delete(iOutputFileName->Des());
		delete iOutputFileName;
		}
	iFs.Close();
	delete iOutputDirectory;
	delete iSuggestedFileName;
	delete iLastWriteData;
	iOutputFiles.ResetAndDestroy();
	}
  
void CTestAgentImportFile::ConstructL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName)
	{
	_LIT8(KDrmMimeType, "application/testagent.drm");
	TBuf8 <KMaxDataTypeLength> mimeType;
	mimeType.Copy(aMimeType);
	mimeType.LowerCase();
	if(mimeType != KDrmMimeType())
		{
		User::Leave(KErrCANotSupported);
		}

	// create buffer to store any output path and filename supplied by the client
	iOutputDirectory = aOutputDirectory.AllocL();
	iSuggestedFileName = aSuggestedFileName.AllocL();

	User::LeaveIfError(iFs.Connect());
	
	static_cast<TAny>(iFs.MkDirAll(KOutDir));
	TInt id(0);
	TInt err(0);
	TFileName filename;
	RFile file;
	do	{
		filename.SetLength(0);
		filename.Append(KOutDir);
		filename.Append(KDefaultName);
		filename.AppendNum(++id);
		filename.Append('.');
		filename.Append(KExt);
		err = file.Create(iFs,filename,EFileWrite);
		}while(err == KErrAlreadyExists);
	if(err != KErrNone)
		return;
	for(TInt i=0;i<aMetaDataArray.Count();i++)
		{
		static_cast<TAny>(file.Write(aMetaDataArray[i].Field8()));
		static_cast<TAny>(file.Write(_L8(" ")));
		static_cast<TAny>(file.Write(aMetaDataArray[i].Data8()));
		static_cast<TAny>(file.Write(_L8("\r\n")));
		}
	file.Close();
	}

TInt CTestAgentImportFile::OpenOutputFile()
	{
	TInt err = KErrNone;
	if(iOutputFileName)
		{
		return KErrInUse;
		}

	if(iAgentCreatesOutputFiles)
		{
		// Figure out a name including full path for the output file
		TRAP(err, iOutputFileName = HBufC::NewL(iOutputDirectory->Des().Length() + iSuggestedFileName->Des().Length()));
		if(err == KErrNone)
			{
			TPtr fileNamePtr = iOutputFileName->Des();
			fileNamePtr.Copy(*iOutputDirectory);
			fileNamePtr.Append(*iSuggestedFileName);

			// create output file
			err = iFile.Create(iFs, fileNamePtr, EFileShareAny  | EFileStream | EFileWrite);
			if(err == KErrNone)
				{
				iFileOpen = ETrue;
				}
			}
		return err;
		}
	else
		{
		// The application must supply a new file handle for us to adopt
		return KErrCANewFileHandleRequired;
		}
	}

TInt CTestAgentImportFile::WriteData(const TDesC8& aData)
	{
	TInt err = KErrNone;

	if(!iFileOpen)
		{
		err = OpenOutputFile();
		}
	if(err == KErrNone)
		{
		err = iFile.Write(aData);
		}

	// check if the output file creation or file writing needs a new file handle
	if(err == KErrCANewFileHandleRequired)
		{
		// save write parameters
		delete iLastWriteData;
		iLastWriteData = NULL;
		
		TRAPD(leaveValue, iLastWriteData = aData.AllocL());
		if(leaveValue != KErrNone)
			{
			return leaveValue;
			}
		}
	return err;
	}

TInt CTestAgentImportFile::WriteDataComplete()
	{
	TInt err = KErrNone;
	CSupplierOutputFile *temp = NULL; 
	if(iFileOpen)
		{	
		iFile.Close();
		iFileOpen = EFalse;
		}

	// add a new entry to the array of output files
	if(iOutputFileName)
		{
		// should really trap this
		TRAP(err, temp = CSupplierOutputFile::NewL(iOutputFileName->Des(), EContent, _L8("application/testagent.drm")));
		if(err == KErrNone)
			{
			err = iOutputFiles.Append(temp);
			if(err != KErrNone)
				{
				delete temp;
				}
			delete iOutputFileName;
			iOutputFileName = NULL;
			}
		}
	if(err == KErrNone)
		{
		if(!iAgentCreatesOutputFiles)
			{
			iWaitingForReceiptHandle = ETrue;
			return KErrCANewFileHandleRequired;
			}
		else
			{
			// Figure out receipt name including full path for the output file
			TRAP(err, iOutputFileName = HBufC::NewL(KMaxFileName));
			if(err == KErrNone)
				{
				TPtr fileNamePtr = iOutputFileName->Des();
				fileNamePtr.Copy(*iOutputDirectory);
				fileNamePtr.Append(_L("receipt.txt"));
				// create output file
				err = iFile.Create(iFs, fileNamePtr, EFileShareAny  | EFileStream | EFileWrite);
				if(err == KErrNone)
					{
					RFileWriteStream stream(iFile);
					TRAP(err,
						{
						stream.WriteL(_L("This is a receipt"));
						stream.CommitL();
						});
					stream.Close();
					iFile.Close();
					
					// Add to list of output files
					TRAP(err, temp = CSupplierOutputFile::NewL(iOutputFileName->Des(), EReceipt, _L8("text/plain")));
					if(err == KErrNone)
						{
						err = iOutputFiles.Append(temp);
						if(err != KErrNone)
							{
							delete temp;
							}
						}
					}
				delete iOutputFileName;
				iOutputFileName = NULL;	
				}
			}
		}
	return err;
	}
		
void CTestAgentImportFile::WriteData(const TDesC8& aData, TRequestStatus &aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	
	// call syncronous WriteData
	TInt rVal = WriteData(aData);
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}

void CTestAgentImportFile::WriteDataComplete(TRequestStatus &aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	// call syncronous WriteDataComplete
	TInt rVal = WriteDataComplete();
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}
	
TInt CTestAgentImportFile::OutputFileCountL() const
	{
	return iOutputFiles.Count();
	}

CSupplierOutputFile& CTestAgentImportFile::OutputFileL(TInt aIndex)
	{
	return *iOutputFiles[aIndex];
	}

TImportStatus CTestAgentImportFile::GetImportStatus() const
	{
	return iImportStatus;
	}	

TInt CTestAgentImportFile::GetSuggestedOutputFileExtension(TDes& aFileExtension)
	{
	if(iWaitingForReceiptHandle)
		{
		aFileExtension.Copy(_L(".TXT"));		
		}
	else
		{
		aFileExtension.Copy(_L(".DRM"));		
		}
	return KErrNone;
	}

TInt CTestAgentImportFile::GetSuggestedOutputFileName(TDes& aFileName)
	{
	if(iWaitingForReceiptHandle)
		{
		aFileName.Copy(_L("RECEIPT.TXT"));
		}
	else
		{
		aFileName.Copy(_L("OUTPUTFILE.DRM"));
		}
	return KErrNone;
	}

TInt CTestAgentImportFile::ContinueWithNewOutputFile(RFile& aFile, const TDesC& aFileName)
	{
	TInt err = iFile.Duplicate(aFile);
	if(err != KErrNone)
		{
		return err;
		}

	if(iWaitingForReceiptHandle)
		{
		RFileWriteStream stream(iFile);
		TRAP(err, stream.WriteL(_L("This is a receipt")));
		TRAP(err, stream.CommitL());
		stream.Close();
		iFile.Close();
		
		CSupplierOutputFile *temp= NULL;
		TRAP(err, temp = CSupplierOutputFile::NewL(aFileName, EReceipt, _L8("text/plain")));
		if(err == KErrNone)
			{
			err = iOutputFiles.Append(temp);
			if(err != KErrNone)
				{
				delete temp;
				}
			}
		return err;
		}
	else
		{
		iFileOpen = ETrue;
		TRAP(err, iOutputFileName = aFileName.AllocL());
		if(err != KErrNone)
			{
			return err;
			}
		return WriteData(*iLastWriteData);
		}
	}

void CTestAgentImportFile::ContinueWithNewOutputFile(RFile& aFile, const TDesC& aFileName, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	// call syncronous WriteDataComplete
	TInt rVal = ContinueWithNewOutputFile(aFile, aFileName);
	
	// complete the request
	User::RequestComplete(ptr,rVal);
	}

