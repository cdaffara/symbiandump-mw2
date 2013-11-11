// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cmsvdiskchangenotifier.h"

CMsvDiskChangeNotifier::CMsvDiskChangeNotifier(const TDriveUnit& aDrive, MMsvDiskChangeObserver& aObserver) 
:CActive(EPriorityHigh), iObserver(aObserver)
	{
	iDrive = TInt(aDrive);
	CActiveScheduler::Add(this);
	}

CMsvDiskChangeNotifier::~CMsvDiskChangeNotifier()
	{
	Cancel();
	iFs.Close();
	delete iDrivePath;
	iDrivePath = NULL;
	}

CMsvDiskChangeNotifier* CMsvDiskChangeNotifier::NewL(const TDriveUnit& aDrive, MMsvDiskChangeObserver& aObserver)
	{
	CMsvDiskChangeNotifier* self = new(ELeave) CMsvDiskChangeNotifier(aDrive, aObserver);
	CleanupStack::PushL(self);
	
	self->ConstructL();
	
	CleanupStack::Pop(self);
	return self;
	}

void CMsvDiskChangeNotifier::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iDrivePath = HBufC16::NewL(KMaxDriveName+2);
	iDrivePath->Des().Append(iDrive.Name());
	iDrivePath->Des().Append(_L("\\"));
	}
	
void CMsvDiskChangeNotifier::Start()
	{
	iFs.NotifyChange(ENotifyDisk, iStatus, *iDrivePath);
	SetActive();
	}

void CMsvDiskChangeNotifier::DoCancel()
	{
	iFs.NotifyChangeCancel();
	}

void CMsvDiskChangeNotifier::RunL()
	{
	TVolumeInfo volume;
	TInt error = iFs.Volume(volume, TInt(iDrive));
	if(KErrNone == error)
		{
		iObserver.DiskInserted(iDrive);
		}
	else if(KErrNotReady == error)
		{
		iObserver.DiskRemoved(iDrive);
		}
	else
		{
		iObserver.DiskChanged(iDrive);
		}
	
	if(!IsActive())
		{
		iFs.NotifyChange(ENotifyDisk, iStatus, *iDrivePath);
		SetActive();
		}
	}


