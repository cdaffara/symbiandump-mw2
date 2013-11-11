/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



#ifndef __TESTAGENTIMPORTFILE_H__
#define __TESTAGENTIMPORTFILE_H__

#include <caf\agentinterface.h>

using namespace ContentAccess;

class CTestAgentImportFile : public CAgentImportFile
		{
	public:
		static CTestAgentImportFile* NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName);
		static CTestAgentImportFile* NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray);
		virtual ~CTestAgentImportFile();

	public:
		// From CAgentImportFile
		virtual TInt WriteData(const TDesC8& aData);
		virtual void WriteData(const TDesC8& aData, TRequestStatus& aStatus);
		virtual TInt WriteDataComplete();
		virtual void WriteDataComplete(TRequestStatus& aStatus);
		virtual TInt OutputFileCountL() const;
		virtual CSupplierOutputFile& OutputFileL(TInt aIndex);
		virtual TImportStatus GetImportStatus() const;
		virtual TInt GetSuggestedOutputFileExtension(TDes& aFileExtension);
		virtual TInt GetSuggestedOutputFileName(TDes& aFileName);
		virtual TInt ContinueWithNewOutputFile(RFile& aFile, const TDesC& aFileName);
		virtual void ContinueWithNewOutputFile(RFile& aFile, const TDesC& aFileName, TRequestStatus& aStatus);

	private:
		CTestAgentImportFile(TBool aAgentCreatesOutputFiles);
		void ConstructL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName);

		TInt OpenOutputFile();
	private:

		HBufC *iOutputDirectory;
		HBufC *iSuggestedFileName;
		HBufC *iOutputFileName;
		HBufC8 *iLastWriteData;
		RFs iFs;
		RFile iFile;
		RPointerArray <CSupplierOutputFile> iOutputFiles;
		TImportStatus iImportStatus;
		TBool iAgentCreatesOutputFiles;
		TBool iFileOpen;
		TBool iWaitingForReceiptHandle;
		};
#endif // __TESTAGENTIMPORTFILE_H__
