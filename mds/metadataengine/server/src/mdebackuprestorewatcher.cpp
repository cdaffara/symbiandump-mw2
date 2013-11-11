/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "mdebackuprestorewatcher.h"

#ifdef _DEBUG
#include <e32debug.h>
#endif

#include <e32property.h>
#include <sbdefs.h>

CMdEBackupRestoreWatcherAO* CMdEBackupRestoreWatcherAO::NewL(MMdEBackupRestoreObserver& aObserver)
	{
	CMdEBackupRestoreWatcherAO* self = CMdEBackupRestoreWatcherAO::NewLC( aObserver );
	CleanupStack::Pop( self );
	return self;
	}

CMdEBackupRestoreWatcherAO* CMdEBackupRestoreWatcherAO::NewLC(MMdEBackupRestoreObserver& aObserver)
	{
	CMdEBackupRestoreWatcherAO* self = new(ELeave) CMdEBackupRestoreWatcherAO( aObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}


CMdEBackupRestoreWatcherAO::CMdEBackupRestoreWatcherAO(MMdEBackupRestoreObserver& aObserver) 
	: CActive(CActive::EPriorityStandard), iObserver( aObserver )
	{
	}

void CMdEBackupRestoreWatcherAO::ConstructL()
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMdEBackupRestoreWatcherAO::ConstructL()"));
	#endif

	CActiveScheduler::Add( this );

	iStatus = KRequestPending;
	iProperty.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );
	iProperty.Subscribe( iStatus );
	SetActive();
	}

CMdEBackupRestoreWatcherAO::~CMdEBackupRestoreWatcherAO()
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMdEBackupRestoreWatcherAO::~CMdEBackupRestoreWatcherAO()"));	
	#endif

	Cancel();

	iProperty.Close();	
	}

void CMdEBackupRestoreWatcherAO::RunL()
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMdEBackupRestoreWatcherAO::RunL"));
	#endif

	iProperty.Subscribe( iStatus );
	SetActive();

	TInt backupStateValue = conn::EBURUnset;
	iProperty.Get( backupStateValue );

	backupStateValue &= conn::KBURPartTypeMask;

	switch ( backupStateValue )
		{
		case conn::EBURBackupFull:
		case conn::EBURBackupPartial:
			{
			#ifdef _DEBUG
			RDebug::Print(_L("CMdEBackupRestoreWatcherAO::RunL - BackupFull/BackupPartial"));
			#endif
		
			// Notify all observers about backup.
			iObserver.HandleBackup();
			break;
			}
		case conn::EBURRestoreFull:
		case conn::EBURRestorePartial:
			{
			#ifdef _DEBUG
			RDebug::Print(_L("CMdEBackupRestoreWatcherAO::RunL - RestoreFull/RestorePartial"));
			#endif
		
			// Notify all observers about restore.
			iObserver.HandleRestore();
			break;
			}
		case conn::EBURNormal:
		case conn::EBURUnset:
		default:
			{
#ifdef _DEBUG
			RDebug::Print(_L("CMdEBackupRestoreWatcherAO::RunL - Normal"));
#endif
		
			// Backup or restore is completed. Resume normal operation.
			iObserver.ResumeOperation();
			}
		}
	}

void CMdEBackupRestoreWatcherAO::DoCancel()
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMdEBackupRestoreWatcherAO::DoCancel"));
	#endif

	iProperty.Cancel();
	}

#ifdef _DEBUG	
TInt CMdEBackupRestoreWatcherAO::RunError(TInt aError)
#else
TInt CMdEBackupRestoreWatcherAO::RunError(TInt /*aError*/)
#endif
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMdEBackupRestoreWatcherAO::RunError() - error %d"), aError);
	#endif

	// start again?
	return KErrNone;
	}
