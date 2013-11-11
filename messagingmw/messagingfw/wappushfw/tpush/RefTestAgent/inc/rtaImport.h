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
// reftestagentimport.h
// 
//

/**
 @file
 @internalComponent
 @released
*/

#ifndef __REFTESTAGENTIMPORT_H__
#define __REFTESTAGENTIMPORT_H__

class RFile;

namespace ReferenceTestAgent
	{
	class CDrmRights;
	class CDrmRightsParser;
	

	class CRefTestAgentImport : public CBase
		{
	public:
		IMPORT_C static CRefTestAgentImport* NewL(const TDesC& aClientSuggestedFileName);
		virtual ~CRefTestAgentImport();

		// obtain the mime type of the content in the file
		IMPORT_C void WriteL(const TDesC8& aData);
		IMPORT_C void WriteCompleteL();
		IMPORT_C TInt GetSuggestedOutputFileExtension(TDes& aFileExtension);
		IMPORT_C TInt GetSuggestedOutputFileName(TDes& aFileName);

		// process the rights received during the import and save them in the rights server
		IMPORT_C void ProcessRightsL();
		
	private:
		CRefTestAgentImport();
		void ConstructL(const TDesC& aClientSuggestedFileName);
	
	private:		
		RFile iFile;
		TBool iOutputFileOpen;
		TInt iBytesReceived;
		
		TInt iRightsOffset;
		TPckg <TInt> iRightsOffsetPckg;
		TInt iRightsSize;
		TPckg <TInt> iRightsSizePckg;

		TInt iDrmFilesOffset;
		TPckg <TInt> iDrmFilesOffsetPckg;
		TInt iDrmFilesSize;
		TPckg <TInt> iDrmFilesSizePckg;

		HBufC8 *iPreviousWriteBuffer;
	
		enum TImportStage 
			{
			EReceivingRightsOffset,
			EReceivingDrmFilesOffset,
			EReceivingContent,
			EReceivingDrmFilesSize,
			EReceivingDrmFiles,
			EReceivingRightsSize,
			EReceivingRights,
			EFinished
			} iImportStage;		

		CDrmRightsParser* iDrmRightsParser;

		/** A client (JMI) requested that the DeleteFile API doubles
		 as API to delete rights objects. Clients will pass the 
		 suggested output filename at rights import time to specify
		 the rights to delete.
		 */
		HBufC8 *iClientSuggestedFileName;
		};
	}

#endif

