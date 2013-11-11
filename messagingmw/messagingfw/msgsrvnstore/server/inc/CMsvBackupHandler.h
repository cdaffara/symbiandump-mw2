// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <babackup.h>
#include <f32file.h>

class CMsvServer;

class CMsvBackupHandler : public CBase, public MBackupObserver
/**
@internalComponent
@released
*/
	{
public:
	static CMsvBackupHandler* NewL(CMsvServer &aServer);
	~CMsvBackupHandler();
	// from MBackupObserver
	virtual void ChangeFileLockL(const TDesC& aFileAffected, TFileLockFlags aFlags);
	TBool Locked();
private:
	void ConstructL();
	CMsvBackupHandler(CMsvServer &aServer);
	TBool DetachFromFile(TInt aErrorState);
	void ReloadContextL();
	
private:
	enum TFileState {
		EHaveLock=0,
		EReleasedForBackup,
		EReleasedForRestore
	};
	CMsvServer &iServer;
	CBaBackupSessionWrapper *iBackup;
	TFileState iState;
	TFileName iFileName;	
	TEntry iEntry;
	};

