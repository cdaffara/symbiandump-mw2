// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*/

#include <e32math.h>
#include <messagingtestutility2.h>

const TUid KServerUid3={0x102857AB};


/**
Start the server process. Simultaneous launching
of two such processes should be detected when the second one attempts to
create the server object, failing with KErrAlreadyExists.
*/
static TInt StartServer()
	{
	const TUidType serverUid(KNullUid,KNullUid,KServerUid3);
	TInt r = KErrNone;
	RProcess server;
	r = server.Create(KTestServerImg,KNullDesC,serverUid);

	if (r != KErrNone)
		{
		return r;
		}
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat != KRequestPending)
		{
		server.Kill(0);		// abort startup
		}
	else
		{
		server.Resume();	// logon OK - start the server
		}
	User::WaitForRequest(stat);		// wait for start or death
	server.Close();
	return stat.Int();
	}


/**
Connect to the server, attempting to start it if necessary
*/
EXPORT_C TInt RMessagingTestUtilityServer2::Connect()
	{
	TInt retry = 2;
	for (;;)
		{
		TInt r=CreateSession(KTestServerName, TVersion(0, 0, 0), 1);
		if (r != KErrNotFound && r != KErrServerTerminated)
			{
			return r;
			}
		if (--retry == 0)
			{
			return r;
			}
		r = StartServer();
		if (r != KErrNone && r != KErrAlreadyExists)
			{
			return r;
			}
		}
	}


/**
Delete an entry in the Message Server index.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::DeleteMessageStore(const TMsvId aMsvId)
	{
	TPckgBuf<TMsvId> msvId(aMsvId);
	TIpcArgs args(&msvId);

	TInt ret = SendReceive(EDeleteMessageStore, args);

	return ret;
	}


/**
Kill a (non protected) process.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::KillProcess(const TProcessId aProcessId)
	{
	TPckgBuf<TProcessId> procIdBuf(aProcessId);
	TIpcArgs args(&procIdBuf);

	TInt ret = SendReceive(EKillProcess, args);

	return ret;
	}


/**
Copy a file regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::CopyFile(const TDesC& aSrc, const TDesC& aDest)
	{
	TIpcArgs args(&aSrc,&aDest);

	TInt ret = SendReceive(ECopyFile, args);

	return ret;
	}


/**
Delete a file regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::DeleteFile(const TDesC& aPath)
	{
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(EDeleteFile, args);

	return ret;
	}


/**
Create a directory regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::MkDir(const TDesC& aPath)
	{
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(EMkDir, args);

	return ret;
	}


/**
Remove a directory regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::RmDir(const TDesC& aPath)
	{
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(ERmDir, args);

	return ret;
	}


/**
NewL()
Allocates and constructs a directory object.

@leave KErrNoMemory
@return
Returns pointer to an object of CDirDerived
*/
CDirDerived* CDirDerived::NewL()
	{
	CDir* dir = CDir::NewL();
	CDirDerived* self = static_cast<CDirDerived*>(dir);

	return self;
	}

/**
AddL()
Adds the specified entry to the directory.

@param aEntry
*/
void CDirDerived::AddL(const TEntry& aEntry)
	{
	this->CDir::AddL(aEntry);
	}


/**
Obtain directory information regardless of data caging.
Caller is responsible for deleting CDir object as with RFs::GetDir().
*/
EXPORT_C TInt RMessagingTestUtilityServer2::GetDirL(const TDesC& aPath, const TUint aEntryAttMask, const TUint aEntrySortKey, CDir*& aDir)
	{
	TPckgBuf<TUint> entryAttMask(aEntryAttMask);
	TPckgBuf<TUint> entrySortKey(aEntrySortKey);

	TIpcArgs args1(&aPath, &entryAttMask, &entrySortKey);

	TInt ret = SendReceive(EGetDir, args1);		//Create CDir object on the server.

	TPckgBuf<TUint> countBuf;
	TIpcArgs args2(&countBuf);

	ret = SendReceive(EGetDirCount, args2);		//Get the number of dir entries.

	TUint count = countBuf();


	CDirDerived* dirDerived = CDirDerived::NewL();	//Construct a new CDir from CDirDerived.

	TPckgBuf<TEntry> entryBuf;

	for (TUint i=0; i<count; i++)
		{
		TPckgBuf<TInt> indexBuf(i);
		TIpcArgs args3(&indexBuf, &entryBuf);

		ret = SendReceive(EGetDirEntry, args3);	//Get each dir entry from the server.

		TEntry entry = entryBuf();

		dirDerived->AddL(entry);				//Add each entry to the client's CDir object.
		}

	aDir = dirDerived;							//Return pointer to client's CDir object.

	TInt c2 = aDir->Count();

	return KErrNone;
	}


/**
Check for a file's existence regardless of data caging.
*/
EXPORT_C TBool RMessagingTestUtilityServer2::FileExists(const TDesC& aFileName)
	{
	TIpcArgs args(&aFileName);

	TInt ret = SendReceive(EFileExists, args);

	return ret;
	}


/**
Initialise event handler.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::EventHandlerInit()
	{

	TInt ret = SendReceive(EEventHandlerInit);

	return ret;
	}


/**
Add event handler.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::EventHandlerAdd(const TDesC& aFlag, const TDesC& aCall)
	{
	TIpcArgs args(&aFlag, &aCall);

	TInt ret = SendReceive(EEventHandlerAdd, args);

	return ret;
	}


/**
Check event handler.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::EventHandlerCheck()
	{
	TInt ret = SendReceive(EEventHandlerCheck);

	return ret;
	}


/**
Mount a drive regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::MountDrive(TInt aDrive)
	{
	TIpcArgs args(&aDrive);
	TInt ret = SendReceive(EMount, args);
	return ret;
	}

/**
Dismount a drive regardless of data caging.
*/
EXPORT_C TInt RMessagingTestUtilityServer2::UnMountDrive(TInt aDrive)
	{
	TIpcArgs args(&aDrive);
	TInt ret = SendReceive(EUnMount, args);
	return ret;
	}
