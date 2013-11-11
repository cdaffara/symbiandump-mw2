/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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



/**
 @file
*/

#ifndef __MESSAGINGTESTUTILITY2_H__
#define __MESSAGINGTESTUTILITY2_H__

#include <e32std.h>

//DeleteMessageStore headers:
#include <msvstd.h>

//GetDir headers:
#include <f32file.h>

_LIT(KTestServerName,"!MessagingTestUtilityServer2");
_LIT(KTestServerImg,"MessagingTestUtilityServer2");		// EXE name


/**
Performes the operation of deleting the messagestore, making, deleting a directory
*/
class RMessagingTestUtilityServer2 : public RSessionBase
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
	IMPORT_C TInt EventHandlerInit();
	IMPORT_C TInt EventHandlerAdd(const TDesC& aFlag, const TDesC& aCall);
	IMPORT_C TInt EventHandlerCheck();

	IMPORT_C TInt MountDrive(TInt aDrive);
	IMPORT_C TInt UnMountDrive(TInt aDrive);

	};


enum TMessagingTestUtilityService2
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
	EEventHandlerInit,
	EEventHandlerAdd,
	EEventHandlerCheck,
	EMount,
	EUnMount
	};


/**
Array of directory entries that has been read into memory from the file system.
It can be read and sorted by user programs, but cannot be created by them.

@see CDir
*/
class CDirDerived : public CDir
	{
public:
	static CDirDerived* NewL();
	void AddL(const TEntry& aEntry);
	};

#endif //__MESSAGINGTESTUTILITY2_H__
