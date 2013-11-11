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

#ifndef __MESSAGINGTESTUTILITY_H__
#define __MESSAGINGTESTUTILITY_H__

#include <e32std.h>

//DeleteMessageStore headers:
#include <msvstd.h>

//GetDir headers:
#include <f32file.h>

_LIT(KTestServerName,"!MessagingTestUtilityServer");
_LIT(KTestServerImg,"MessagingTestUtilityServer");		// EXE name


class RMessagingTestUtilityServer : public RSessionBase
	{
public:
	IMPORT_C TInt Connect();
	IMPORT_C TInt DeleteMessageStore(const TMsvId aMsvId);
	IMPORT_C TInt KillProcess(const TProcessId aProcessId);
	IMPORT_C TInt CopyFile(const TDesC& aSrc, const TDesC& aDest); 
	IMPORT_C TInt DeleteFile(const TDesC& aPath); 
	IMPORT_C TInt MkDir(const TDesC& aPath); 
	IMPORT_C TInt RmDir(const TDesC& aPath);
	IMPORT_C TInt GetDirL(const TDesC& aPath, const TUint aEntryAttMask,const TUint aEntrySortKey, CDir*& aDir);
	IMPORT_C TBool FileExists(const TDesC& aFileName);
	};


enum TMessagingTestUtilityService 
	{
	EDeleteMessageStore,
	EKillProcess,
	ECopyFile,
	EDeleteFile,
	EMkDir,
	ERmDir,
	EGetDir,
	EGetDirCount,
	EGetDirEntry,
	EFileExists,
	};

class CDirDerived : public CDir
	{
public:
		static CDirDerived* NewL();
		void AddL(const TEntry& aEntry);
	};

#endif //__MESSAGINGTESTUTILITY_H__
