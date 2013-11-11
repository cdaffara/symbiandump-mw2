// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

//#define __BACKUP_LOGGING

#ifdef __BACKUP_LOGGING
	#define _BACKUPLOG( a ) RFileLogger::WriteFormat(_L("msgs"),_L("backup.txt"),EFileLoggingModeAppend, a );	
	#define _BACKUPLOGERR( a, b ) RFileLogger::WriteFormat(_L("msgs"),_L("backup.txt"),EFileLoggingModeAppend, a , b);	
#else
	#define _BACKUPLOG( a )
	#define _BACKUPLOGERR( a, b )
#endif

#include "CMsvBackupHandler.h"
#include "MSVSERV.H"
#include "MSVSTD.H"
#include "msvdbadapter.h"
#include "msvindexadapter.h"
#include <msvapi.h>
#include <e32base.h>
#include <e32property.h>
#include <connect/sbdefs.h>

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
_LIT(KDBFileNamePath,"\\messaging.db");
#else
_LIT(KDBFileNamePath,"\\private\\10281e17\\[1000484B]messaging.db");
#endif

CMsvBackupHandler* CMsvBackupHandler::NewL(CMsvServer& aServer)
	{
	CMsvBackupHandler* self=new (ELeave) CMsvBackupHandler(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return(self);
	}

CMsvBackupHandler::CMsvBackupHandler(CMsvServer& aServer) : iServer(aServer)
	{
	}

CMsvBackupHandler::~CMsvBackupHandler()
	{
	_BACKUPLOG(_L("backup handler ending\r\n"));

	// Unregister ourselves with backup server
	if (iBackup)
		{
		iBackup->DeregisterFile(iFileName);
		delete iBackup;
		}
	}

void CMsvBackupHandler::ConstructL()
	{
	_BACKUPLOG(_L("Backup handler started\r\n"))

	TParse parse;

	RFs fs;
	CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	TPtrC driveName = TDriveUnit(MessageServer::CurrentDriveL(fs)).Name();
	iFileName = KDBFileNamePath;
	parse.Set(iFileName, &driveName, NULL);
	CleanupStack::PopAndDestroy();		// fs
	iFileName=parse.FullName();
	iState=EHaveLock;

	// Create backup session
	iBackup = CBaBackupSessionWrapper::NewL();
	
	// Register with the backup server
	iBackup->RegisterFileL(iFileName, *this);
	// All done
	_BACKUPLOGERR(_L("File Registered with Backup Server: \"%S\"\r\n"), &iFileName);
	}


TBool CMsvBackupHandler::Locked()
	{
	if(iState==EHaveLock) 
		{
		return(EFalse);
		}
	else 
		{
		return(ETrue);
		}
	}

#ifdef __BACKUP_LOGGING
void CMsvBackupHandler::ChangeFileLockL(const TDesC& aFileAffected, TFileLockFlags aFlags)
#else
void CMsvBackupHandler::ChangeFileLockL(const TDesC& /*aFileAffected*/, TFileLockFlags aFlags)
#endif
	{
	_BACKUPLOG(_L("Got backup event\r\n"))
	_BACKUPLOGERR(_L("File affected: \"%S\"\r\n"), &aFileAffected);
	
	switch(aFlags)
		{
		case ETakeLock: // this state gives me the lock back after it has been taken away from me.
			{
			_BACKUPLOG(_L("Take lock\r\n"))
			if(iState==EReleasedForBackup)
				{
				iState=EHaveLock;
				_BACKUPLOG(_L("from backup\r\n"))


				//Change error state to none
				iServer.Context().IndexAdapter()->SetErrorState(KErrNone);
				
				//Change State
				iServer.SetStartupState(EMsvNullNotification);

				_BACKUPLOG(_L("Reloading context\r\n"))
				ReloadContextL();
				}
			else if(iState==EReleasedForRestore)
				{
				// Reload context will result in this object being deleted
				// therefore this is no longer valid;
				iState=EHaveLock;							
				//reload as its a new db after restore
				ReloadContextL();
				}
			else
				{
				_BACKUPLOG(_L("bad state\r\n"))
				__ASSERT_DEBUG(EFalse,PanicServer(EMsvBackupHandlerInUnkownState));
				}
			break;
			}
		case EReleaseLockNoAccess:
			{
			// please release the lock and don't read or write to the file
			_BACKUPLOG(_L("release no access\r\n"))
		
			// Need to find whether this file lock release request is due to a restore happening.  
			// It is possible that the file system may need us to release our locks for some other 
			// purpose such as formatting an MMC card.
			TInt backupRestoreState = 0;
			
			RProperty property;
			TInt err = property.Get(KUidSystemCategory, conn::KUidBackupRestoreKey, backupRestoreState);
			property.Close();
			User::LeaveIfError(err);
			
			_BACKUPLOGERR(_L("backupRestoreStateValue: \"0x%08x\"\r\n"), backupRestoreState);

			if ((backupRestoreState & conn::EBURRestoreFull) || (backupRestoreState & conn::EBURRestorePartial))
				{
				// This means they will be restoring the file so I will
				// have to cope with a new index when I get the lock back.
				if(DetachFromFile(KMsvIndexRestore)!=EFalse)
					{
					_BACKUPLOG(_L("A genuine restore is happening\r\n"))
					iState=EReleasedForRestore;
					}
				}
			else
				{
				// Not a restore operation, so no need to reload the index when 
				// I get the lock back.
				// Use KMsvIndexRestore to tell the index not to write anything to disk 
				// while the file system is locked...
				if(DetachFromFile(KMsvIndexRestore)!=EFalse) 
					{
					// ...but use EReleasedForBackup here so that we don't reload the index from disk
					// when we get the ETakeLock notification - this would overwrite any index
					// changes that have happened while the file system was locked, which we want to 
					// keep if the file system was locked for any reason other than Restore.
					_BACKUPLOG(_L("File system detached for read and write, but not for a restore\r\n"))
					iState=EReleasedForBackup;
					}
				}

			_BACKUPLOG(_L("released for no access\r\n"))
			break;
			}
		case EReleaseLockReadOnly:
			{
			// please release the lock and don't write to the file, I won't read
			// or write but I also assume the file is being backed up and therefore
			// the same index will be there when I have finished.
			_BACKUPLOG(_L("release no read only\r\n"))
			if(DetachFromFile(KMsvIndexBackup)!=EFalse)
				{
				iState=EReleasedForBackup;
				}
			_BACKUPLOG(_L("released for read only\r\n"))
			break;
			}
		default:
			{
			_BACKUPLOG(_L("bad event\r\n"))
			__ASSERT_DEBUG(EFalse,PanicServer(EMsvBackupObserverGotUnknownEvent));
			break;
			}
		}
	};


TBool CMsvBackupHandler::DetachFromFile(TInt aErrorState)
	{
	// if the context is currently in an error state don't do anything,
	// the current implications of this are that when the disk is removed
	// or the wrong disk is in the drive and we are asked to release a lock
	// on the file we won't.
	_BACKUPLOG(_L("detach to file\r\n"))
	
	TInt error = KErrNone;
	error = iServer.Context().IndexAdapter()->ErrorState();
	
	if(error == KErrNone)
		{
		_BACKUPLOG(_L("context good for closing\r\n"))
		_BACKUPLOG(_L("closing\r\n"))
		
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		TUint currDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
		TDriveNumber currDriveNum = CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber();
			
		// Flush Cache. This will also detach the DB.
		TRAP(error, iServer.Context().IndexAdapter()->RemoveDriveL(KCurrentDriveId, currDriveIndex, EFalse));
		if(error)
			return(EFalse);
		// current drive is detached from DB, so cannot access any entries from Database, so will
		// assing iDbAdpter pointer to temporary iTempDbAdapter pointer and assin NULL to iDbAdpter.
		iServer.Context().IndexAdapter()->BackupDbAdpter();
		TRAP(error, CMsvPreferredDriveList::GetDriveList()->UpdateDriveIdL(currDriveIndex, KCurrentDriveId));
		if(error)
			return(EFalse);
#else
		iEntry.iSize = 	iServer.Context().IndexAdapter()->GetDbAdapter()->Size();
		iServer.Context().IndexAdapter()->DeleteDbAdapter();
#endif		// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		
		_BACKUPLOG(_L("closed\r\n"))

		iServer.Context().IndexAdapter()->SetErrorState(aErrorState);	
		iServer.SetStartupState(EMsvMediaUnavailable);
		return(ETrue);
		}
	else
		{
		return(EFalse);
		}
	}



// CreateIndexL will result in this object being deleted
// so this is no longer valid
void CMsvBackupHandler::ReloadContextL()
	{
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// for attaching current drive to main Database, assign iDbAdpter with iTempDbAdapter.
	iServer.Context().IndexAdapter()->RestoreDbAdpter();
	iServer.Context().IndexAdapter()->ReloadCacheL();
#else
	iServer.CreateIndexL(ETrue);
#endif
	}

