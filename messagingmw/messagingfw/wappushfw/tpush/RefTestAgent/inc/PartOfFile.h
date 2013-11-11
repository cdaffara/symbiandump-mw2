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

#ifndef __PARTOFFILE_H__
#define __PARTOFFILE_H__

#include <f32file.h>
#include "virtualFile.h"

namespace ReferenceTestAgent
	{
	class CPartOfFile : public CVirtualFile
		{
	public:
		IMPORT_C static CPartOfFile* NewL(RFile& aSharedFile, RSemaphore& aSharedLock, TInt aOffset, TInt aSize);
		virtual ~CPartOfFile();	

		virtual TInt Read(TDes8& aBuffer, TInt aLength);
		virtual TInt Read(TInt aPos, TDes8& aBuffer, TInt aLength);		
		virtual TInt Write(const TDesC8& aBuffer, TInt aLength);
		virtual HBufC8* ReadDescriptor8L();
		virtual void WriteDescriptor8L(const TDesC8& aBuffer);
		virtual TInt Seek(TSeek aMode, TInt &aPos);
		virtual TInt Size(TInt& aSize);
		virtual RFile& FileHandle();

	private:
		CPartOfFile(RFile& aSharedFile, RSemaphore& aSharedLock, TInt aOffset, TInt aSize);
		void Construct();

		TInt DoSeek(TSeek aMode, TInt &aPos);

		RFile& iFile;
		RSemaphore& iFileLock;
		const TInt iSize;
		const TInt iOffset;
		TInt iPos;
		};
	}

#endif

