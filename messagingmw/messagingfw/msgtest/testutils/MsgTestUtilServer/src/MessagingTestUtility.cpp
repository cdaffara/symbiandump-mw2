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

#include <e32math.h>
#include "messagingtestutility.h"

const TUid KServerUid3={0x1020427D};



static TInt StartServer()
//
// Start the server process. Simultaneous launching
// of two such processes should be detected when the second one attempts to
// create the server object, failing with KErrAlreadyExists.
//
	{
	const TUidType serverUid(KNullUid,KNullUid,KServerUid3);
	TInt r=KErrNone;
	RProcess server;
	r=server.Create(KTestServerImg,KNullDesC,serverUid);


	if (r!=KErrNone)
		return r;
	TRequestStatus stat;
	server.Rendezvous(stat);
	if (stat!=KRequestPending)
		server.Kill(0);		// abort startup
	else
		server.Resume();	// logon OK - start the server
	User::WaitForRequest(stat);		// wait for start or death
	server.Close();
	return stat.Int();
	}


EXPORT_C TInt RMessagingTestUtilityServer::Connect()
//
// Connect to the server, attempting to start it if necessary
//
	{
	TInt retry=2;
	for (;;)
		{
		TInt r=CreateSession(KTestServerName,TVersion(0,0,0),1);
		if (r!=KErrNotFound && r!=KErrServerTerminated)
			return r;
		if (--retry==0)
			return r;
		r=StartServer();
		if (r!=KErrNone && r!=KErrAlreadyExists)
			return r;
		}
	}

	
EXPORT_C TInt RMessagingTestUtilityServer::DeleteMessageStore(const TMsvId aMsvId)
//
// Delete an entry in the Message Server index.
//
	{
	
	TPckgBuf<TMsvId> msvId(aMsvId);
	TIpcArgs args(&msvId);
	
	TInt ret = SendReceive(EDeleteMessageStore, args);
	
	return ret;
	}
	
EXPORT_C TInt RMessagingTestUtilityServer::KillProcess(const TProcessId aProcessId)
//
// Kill a (non protected) process.
//
	{
	TPckgBuf<TProcessId> procIdBuf(aProcessId);
	TIpcArgs args(&procIdBuf);
	
	TInt ret = SendReceive(EKillProcess, args);
	
	return ret;
	}
	
EXPORT_C TInt RMessagingTestUtilityServer::CopyFile(const TDesC& aSrc, const TDesC& aDest)
//
// Copy a file regardless of data caging.
//
	{
	
	TIpcArgs args(&aSrc,&aDest);

	TInt ret = SendReceive(ECopyFile, args);
	
	return ret;
	}
	
EXPORT_C TInt RMessagingTestUtilityServer::DeleteFile(const TDesC& aPath)
//
// Delete a file regardless of data caging.
//
	{
	
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(EDeleteFile, args);
	
	return ret;
	}
	
EXPORT_C TInt RMessagingTestUtilityServer::MkDir(const TDesC& aPath)
//
// Create a directory regardless of data caging.
//
	{
	
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(EMkDir, args);
	
	return ret;
	}
	
EXPORT_C TInt RMessagingTestUtilityServer::RmDir(const TDesC& aPath)
//
// Remove a directory regardless of data caging.
//
	{
	
	TIpcArgs args(&aPath);

	TInt ret = SendReceive(ERmDir, args);
	
	return ret;
	}

//

CDirDerived* CDirDerived::NewL()
{
	CDir* dir = CDir::NewL();
	CDirDerived* self = static_cast<CDirDerived*>(dir);
		
	return self;
}

void CDirDerived::AddL(const TEntry& aEntry)
{
	this->CDir::AddL(aEntry);
}

EXPORT_C TInt RMessagingTestUtilityServer::GetDirL(const TDesC& aPath, const TUint aEntryAttMask, const TUint aEntrySortKey, CDir*& aDir)
//
// Obtain directory information regardless of data caging.
// Caller is responsible for deleting CDir object as with RFs::GetDir().
//
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

EXPORT_C TBool RMessagingTestUtilityServer::FileExists(const TDesC& aFileName)
//
// Check for a file's existence regardless of data caging.
//
	{
	TIpcArgs args(&aFileName);

	TInt ret = SendReceive(EFileExists, args);
	
	return ret;
	}
	
//EOF
