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

#ifndef __ENTIREFILE_H__
#define __ENTIREFILE_H__

#include <f32file.h>
#include "virtualFile.h"

namespace ReferenceTestAgent
	{
	class CEntireFile : public CVirtualFile
		{
	public:
		IMPORT_C static CEntireFile* NewL(const TDesC& aFileName, TBool aCreateFile=EFalse);
		virtual ~CEntireFile();	

		virtual TInt Read(TDes8& aBuffer, TInt aLength);
		virtual TInt Read(TInt aPos, TDes8& aBuffer, TInt aLength);
		virtual TInt Write(const TDesC8& aBuffer, TInt aLength);
		virtual HBufC8* ReadDescriptor8L();
		virtual void WriteDescriptor8L(const TDesC8& aBuffer);
		virtual TInt Seek(TSeek aMode, TInt &aPos);
		virtual TInt Size(TInt& aSize);
		
		virtual RFile& FileHandle();
	private:
		CEntireFile();
		void ConstructL(const TDesC& aFileName, TBool aCreateFile);

		RFs iFs;
		RFile iFile;
		};
	}

#endif

