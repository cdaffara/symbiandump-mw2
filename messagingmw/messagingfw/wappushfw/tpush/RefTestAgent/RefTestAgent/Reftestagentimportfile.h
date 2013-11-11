// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __REFTESTAGENTIMPORTFILE_H__
#define __REFTESTAGENTIMPORTFILE_H__

#include <caf/cafplatform.h>
#include <caf/agentinterface.h>
#include "rtaImport.h"

using namespace ContentAccess;
using namespace ReferenceTestAgent;

class CRefTestAgentImportFile : public CAgentImportFile
		{
	public:
		static CRefTestAgentImportFile* NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName);
		static CRefTestAgentImportFile* NewL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray);
		virtual ~CRefTestAgentImportFile();

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
		CRefTestAgentImportFile(TBool aAgentCreatesOutputFiles);
		void ConstructL(const TDesC8& aMimeType, const CMetaDataArray& aMetaDataArray, const TDesC& aOutputDirectory, const TDesC& aSuggestedFileName);
		void AddOutputFileL(const TDesC& aFileName, TOutputType aOutputType, const TDesC8& aMimeType);
		void DumpMetaData(const CMetaDataArray& aMetaDataArray);
		void LogInputData(const TDesC8& aInput);
	private:
		TImportStatus iImportStatus;
		RFs iFs;		
		HBufC* iSuggestedFileName;
		HBufC* iOutputDirectory;
		CRefTestAgentImport* iImporter;
		TBool iOutputFileOpen;
		HBufC* iOutputFileName;
		TBool iAgentCreatesOutputFiles;
		TBool iWriteComplete;
		
		RPointerArray <CSupplierOutputFile> iOutputFiles;
		};
#endif // __REFTESTAGENTIMPORTFILE_H__
