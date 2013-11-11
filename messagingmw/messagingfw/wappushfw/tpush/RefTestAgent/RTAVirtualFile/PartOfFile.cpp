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

#include "PartOfFile.h"

using namespace ReferenceTestAgent;

EXPORT_C CPartOfFile* CPartOfFile::NewL(RFile& aSharedFile, RSemaphore& aSharedLock, TInt aOffset, TInt aSize)
	{
	CPartOfFile* self = new (ELeave) CPartOfFile(aSharedFile, aSharedLock, aOffset, aSize);
	self->Construct();
	return self;
	}

CPartOfFile::CPartOfFile(RFile& aSharedFile, RSemaphore& aSharedLock, TInt aOffset, TInt aSize) 
	: iFile(aSharedFile), iFileLock(aSharedLock), iSize(aSize), iOffset(aOffset)
	{
	}

void CPartOfFile::Construct()
	{
	iPos = 0;
	Seek(ESeekStart, iPos);	
	}

CPartOfFile::~CPartOfFile()
	{
	}


TInt CPartOfFile::Read(TDes8& aBuffer, TInt aLength) 
	{
	// lock file
	iFileLock.Wait();
	
	// Check that we are not reading past the end of the file
	if(iPos + aLength > iSize)
		{
		aLength = iSize - iPos;
		}
	
	// Seek back to our last position in case somebody else moved the
	// read from the file since the last time we did anything
	TInt err = DoSeek(ESeekStart, iPos);
	err = iFile.Read(aBuffer, aLength);

	// Get current file position
	iPos = 0;
	DoSeek(ESeekCurrent, iPos);
	
	// release file
	iFileLock.Signal();
	return err;
	}
	
	
TInt CPartOfFile::Read(TInt aPos, TDes8& aBuffer, TInt aLength) 
	{
	// lock file
	iFileLock.Wait();
	// Check that we are not reading past the end of the file
	if(aPos>iSize)
		{
		aPos=iSize;
		aLength=0;
		}
		
	else if(aPos + aLength > iSize)
		{
		aLength = iSize - aPos;
		}
	
	TInt fileOffset=aPos+iOffset;
	TInt err = iFile.Read(fileOffset, aBuffer, aLength);

	// Get current file position
	iPos = 0;
	DoSeek(ESeekCurrent, iPos);
	
	// release file
	iFileLock.Signal();
	return err;
	}


TInt CPartOfFile::Write(const TDesC8& aBuffer, TInt aLength)
	{
	TInt err = KErrNone;

	// lock file
	iFileLock.Wait();

	err = DoSeek(ESeekStart, iPos);
	if(err == KErrNone)
		{
		err = iFile.Write(aBuffer, aLength);	

		// Get current file position
		iPos = 0;
		DoSeek(ESeekCurrent, iPos);
		}

	// unlock file
	iFileLock.Signal();
	return err;
	}
	
HBufC8* CPartOfFile::ReadDescriptor8L()
	{
	HBufC8* buffer = NULL;
	TInt err = KErrNone;
	
	// lock file
	iFileLock.Wait();

	err = DoSeek(ESeekStart, iPos);
	if(err == KErrNone)
		{
		TRAP(err, buffer = CVirtualFile::ReadDescriptor8L(iFile));
		
		// Get current file position
		iPos = 0;
		DoSeek(ESeekCurrent, iPos);
		}
	// unlock file
	iFileLock.Signal();

	User::LeaveIfError(err);
	return buffer;
	}
	
void CPartOfFile::WriteDescriptor8L(const TDesC8& aBuffer)
	{
	TInt err = KErrNone;
	
	// lock file
	iFileLock.Wait();

	err = DoSeek(ESeekStart, iPos);
	if(err == KErrNone)
		{
		TRAP(err, CVirtualFile::WriteDescriptor8L(iFile, aBuffer));
		
		// Get current file position
		iPos = 0;
		DoSeek(ESeekCurrent, iPos);
		}
	// unlock file
	iFileLock.Signal();

	User::LeaveIfError(err);
	}


		
TInt CPartOfFile::Seek(TSeek aMode, TInt &aPos) 
	{
	TInt err = KErrNone;

	// lock file
	iFileLock.Wait();

	if(aMode == ESeekCurrent)
		{
		// Seek back to our last position in case somebody else moved the
		// read from the file since the last time we did anything
		err = DoSeek(ESeekStart, iPos);
		}

	err = DoSeek(aMode, aPos);

	// release file
	iFileLock.Signal();
	return err;
	}

TInt CPartOfFile::DoSeek(TSeek aMode, TInt &aPos) 
	{
	TInt err = KErrNone;
	TInt fileOffset = 0;
	TInt pos = aPos;

	switch(aMode)
		{
	case ESeekAddress:
		// Unlikely for this case to be used
		if(pos > iSize || pos < 0)
			{
			err = KErrArgument;
			}
		else
			{
			fileOffset = iOffset + pos;
			err = iFile.Seek(aMode, fileOffset);
		
			//  Offset is populated with the ROM address, fill in aPos for the caller
			aPos = fileOffset;
			}
		break;
	
	case ESeekStart:
		if(pos < 0)
			{
			err = KErrArgument;
			}
		else
			{
			if(pos > iSize)
				{
				pos = iSize;
				}
			fileOffset = iOffset + pos;
			err = iFile.Seek(aMode, fileOffset);
			iPos = fileOffset - iOffset;
			aPos = iPos;
			}
		break;
	case ESeekCurrent:
		if(pos + iPos > iSize)
			{
			pos = iSize - iPos;
			}
		else if(iPos + pos < 0)
			{
			pos = -iPos;
			}
		fileOffset = pos;
		err = iFile.Seek(aMode, fileOffset);
		iPos = fileOffset - iOffset;
		aPos = iPos;
		break;
	case ESeekEnd:
		// offsets are negative for ESeekEnd
		if(pos > 0)
			{
			err = KErrArgument;
			}
		else 
			{
			if(pos < -iSize)
				{
				pos = -iSize;
				}
			fileOffset = iOffset + iSize + pos;
			err = iFile.Seek(ESeekStart, fileOffset);
			iPos = fileOffset - iOffset;
			aPos = iPos;
			}
		break;
	default:
		return KErrNotSupported;
		}
	return err;
	}
	
TInt CPartOfFile::Size(TInt& aSize) 
	{
	aSize = iSize;
	return KErrNone;		
	}

RFile& CPartOfFile::FileHandle()
	{
	return iFile;
	}

