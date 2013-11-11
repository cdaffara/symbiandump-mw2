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

/**
 @file
 @internalComponent
 @released
*/

#ifndef __DRMFILECONTENT_H__
#define __DRMFILECONTENT_H__

#include <e32base.h>
#include <f32file.h>
#include <caf/attributeset.h>
#include <caf/stringattributeset.h>

using namespace ContentAccess;

namespace ReferenceTestAgent
	{
	class CVirtualFile;

	class CDrmFileContent : public CBase
		{
	public:
		static CDrmFileContent* NewL(const TDesC8& aCid, const TDesC& aName, const TDesC& aUniqueId, const TDesC& aFileName, RAttributeSet& aAttributeSet, RStringAttributeSet& aStringAttributeSet);
		virtual ~CDrmFileContent();
		
		IMPORT_C TInt Read(TDes8& aBuffer, TInt aLength);
		IMPORT_C TInt Read(TInt aPos, TDes8& aBuffer, TInt aLength);
		IMPORT_C TInt Seek(TSeek aMode, TInt &aPos);
		IMPORT_C TInt Size(TInt& aSize);
		IMPORT_C TInt GetAttribute(TInt aAttribute, TInt& aValue);
		IMPORT_C TInt GetStringAttribute(TInt aAttribute, TDes& aValue);
		IMPORT_C TInt GetAttributeSet(RAttributeSet &aAttributeSet); 
		IMPORT_C TInt GetStringAttributeSet(RStringAttributeSet &aStringAttributeSet);
		IMPORT_C const TDesC8& Cid();
		
		const TDesC& UniqueId();
		const TDesC& Name();
		// Convenience function rather than querying attributes
		IMPORT_C const TDesC8& MimeType();

		// setup parameters for common source file (iVirtualFile)
		IMPORT_C void SetCommonSourceFileL(RFile& aFile, RSemaphore& aFileLock, TInt aOffset, TInt aSize);

	private:
		CDrmFileContent();
		void ConstructL(const TDesC8& aCid, const TDesC& aName, const TDesC& aUniqueId, const TDesC& aFileName, RAttributeSet& aAttributeSet, RStringAttributeSet& aStringAttributeSet);
		void VirtualFilePresentL();
		void SetDefaultAttributeL(TInt aAttribute, TInt aValue, TInt aErrorCode);		
		
	private:
		HBufC8* iCid;
		HBufC* iName;
		HBufC* iUniqueId;
		HBufC* iFileName;
		
		HBufC8* iMimeType;
		
		RAttributeSet iDefaultAttributeSet;
		RAttributeSet iAttributeSet;
		RStringAttributeSet iStringAttributeSet;

		// Used to actually read files
		CVirtualFile* iVirtualFile;
		};	
	}
#endif
