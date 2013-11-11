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
// reftestagentarchive.h
// 
//

/**
 @file
 @internalComponent
 @released
*/

#ifndef __REFTESTAGENTARCHIVE_H__
#define __REFTESTAGENTARCHIVE_H__

#include <f32file.h>

namespace ReferenceTestAgent
	{
	class CDrmFiles;
	class CDrmFilesParser;
	class CPartOfFile;
	class CVirtualFile;
	class CRefTestAgentArchive : public CBase
		{
	public:
		IMPORT_C static CRefTestAgentArchive* NewL(RFile& aFile);
		virtual ~CRefTestAgentArchive();

		// obtain the mime type of the content in the file
		IMPORT_C const TDesC8& DefaultMimeType();
		
		// return the drm file object used for reading
		IMPORT_C CDrmFiles& DrmFilesL();

		IMPORT_C static TInt CreateArchive(const TDesC& aContentXmlFileName, const TDesC& aRightsXmlFileName, TDes& aOutputFileName, TDes8& aOutputMimeType);
		IMPORT_C static void GetDefaultMimeTypeFromHeaderL(const TDesC8& aBuffer, TDes8& aDefaultMimeType);
		
	private:
		CRefTestAgentArchive();
		void ConstructL(RFile& aFile);
		
		static void DoCreateArchiveL(const TDesC& aContentXmlFileName, const TDesC& aRightsXmlFileName, const TDesC& aOutputFileName);
		static void WriteHeaderL(CVirtualFile& aVirtualFile, TInt aRightsOffset, TInt aContentOffset);

		
	private:		
		CDrmFilesParser* iDrmFilesParser;
		CPartOfFile* iPartOfFile;
		CPartOfFile* iContentXmlFile;
		HBufC8* iMimeType;
		RSemaphore iFileLock;
		
		TInt iDrmFileOffset;
		TInt iDrmRightsOffset;
		TInt iContentOffset;
		TInt iFileSize;
		};
	}

#endif

