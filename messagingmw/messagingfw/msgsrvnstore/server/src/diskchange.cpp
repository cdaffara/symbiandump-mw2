// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "diskchange.h"
#include "MSVPANIC.H"

CMsvNotifyDiskChange::CMsvNotifyDiskChange(RFs& aFs, MMsvDiskChangeObserver& aObserver)
: CMsgActive(EPriorityHigh), iFs(aFs), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CMsvNotifyDiskChange::~CMsvNotifyDiskChange()
	{
	Cancel();
	}

TInt CMsvNotifyDiskChange::Start(const TDriveUnit& aDrive, TUint aUniqueId)
//
// aDrive - The drive for which we want notifications
// aUniqueId - The id we expect the drive to have
//
	{
	__ASSERT_DEBUG(!IsActive(), PanicServer(EMsvDiskChangeNotiferActive));

	iUniqueId = aUniqueId;

	// Check if the drive is removable
	TDriveInfo drive;
	TInt error = iFs.Drive(drive, TInt(aDrive));
	if (error != KErrNone)
		return error;

	iDrive = TInt(aDrive);

	// Setup file system notification
	iFs.NotifyChange(ENotifyDisk, iStatus);
	SetActive();

	// See if the disk is currently available
	TVolumeInfo volume;
	iDiskRemoved = iFs.Volume(volume, TInt(iDrive)) != KErrNone || iDiskMissing;

	return KErrNone;
	}

void CMsvNotifyDiskChange::DoCancel()
	{
	iFs.NotifyChangeCancel();
	}

void CMsvNotifyDiskChange::DoRunL()
	{
	// Flag to indicate we should continue to notify of disk changes
	TBool keepGoing = ETrue;

	// Get disk details - If this fails assume it's missing
	TVolumeInfo volume;
	if (iFs.Volume(volume, TInt(iDrive)) != KErrNone || iDiskMissing)
		{
		if (!iDiskRemoved)
			{
			// The disk has been removed
			keepGoing = iObserver.DiskRemoved(iDrive);
			}

		iDiskRemoved = ETrue;
		}
	else if (iDiskRemoved)
		{
		if (iWrongId)
			{
			// The incorrect disk is being used
			keepGoing = iObserver.DiskChanged(iDrive, volume.iUniqueID);
			}
		else
			{
			// The correct disk has been inserted
			keepGoing = iObserver.DiskInserted(iDrive);
			}

		iDiskRemoved = EFalse;
		}
	
	// Setup file system notification
	if (keepGoing)
		{
		iFs.NotifyChange(ENotifyDisk, iStatus);
		SetActive();
		}
	}

