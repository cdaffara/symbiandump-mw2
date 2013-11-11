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

#ifndef __VIRTUALFILE_H__
#define __VIRTUALFILE_H__

#include <f32file.h>

namespace ReferenceTestAgent
	{
	class CVirtualFile : public CBase
		{
	public:
		virtual TInt Read(TDes8& aBuffer, TInt aLength) = 0;
		virtual TInt Read(TInt aPos, TDes8& aBuffer, TInt aLength) = 0;
		virtual TInt Write(const TDesC8& aBuffer, TInt aLength) = 0;
		virtual HBufC8* ReadDescriptor8L() = 0;
		virtual void WriteDescriptor8L(const TDesC8& aBuffer) = 0;
		virtual TInt Seek(TSeek aMode, TInt &aPos) = 0;
		virtual TInt Size(TInt& aSize) = 0;
		virtual RFile& FileHandle() = 0;
		
		TInt Read(TDes8& aBuffer);
		TInt Write(const TDesC8& aBuffer);
		IMPORT_C TInt ReadIntL();
		IMPORT_C void WriteIntL(TInt aValue);
	
	protected:
		// only allow derived classes to create an instance of CVirtualFile
		CVirtualFile();

		// used by derived classes for uniform descriptor operations
		HBufC8* ReadDescriptor8L(RFile& aFile);
		void WriteDescriptor8L(RFile& aFile, const TDesC8& aBuffer);
		};
	}

#endif

