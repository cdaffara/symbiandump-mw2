/*
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
* Description:  Queue/cache item for file server plugin events*
*/




//-----------------------------------------------------------------------------
// CMdsFSPQueueItem::NewL()
//-----------------------------------------------------------------------------
//
CMdsFSPQueueItem* CMdsFSPQueueItem::NewL()
	{
	CMdsFSPQueueItem *self = new (ELeave) CMdsFSPQueueItem();
	return self;
	}


//-----------------------------------------------------------------------------
// CMdsFSPQueueItem::NewL()
//-----------------------------------------------------------------------------
//
CMdsFSPQueueItem* CMdsFSPQueueItem::NewL( TMdsFSPStatus& aStatus )
	{
	CMdsFSPQueueItem *self = new (ELeave) CMdsFSPQueueItem();
	CleanupStack::PushL( self );
	self->SetStatusL( aStatus );
	CleanupStack::Pop( self );
	return self;
	}

//-----------------------------------------------------------------------------
// CMdsFSPQueueItem::GetAsFspStatus()
//-----------------------------------------------------------------------------
//
void CMdsFSPQueueItem::GetAsFspStatus( TMdsFSPStatus& aStatus )
	{
	aStatus.iDriveNumber = iDriveNumber;
	aStatus.iFileName.Copy( *iFileName );
	aStatus.iNewFileName.Copy( *iNewFileName );
	aStatus.iFileEventType = iFileEventType;
	aStatus.iProcessId = iProcessId;
	aStatus.iDriveMediaId = iDriveMediaId;
	}

//-----------------------------------------------------------------------------
// CMdsFSPQueueItem::SetStatus()
//-----------------------------------------------------------------------------
//
void CMdsFSPQueueItem::SetStatusL( TMdsFSPStatus& aStatus )
	{
	iDriveNumber = aStatus.iDriveNumber;
	iFileEventType = aStatus.iFileEventType;
	iProcessId = aStatus.iProcessId;

	if( iFileName ) 
		{
		delete iFileName;
		iFileName = NULL;
		}
	
	if( iNewFileName )
		{
		delete iNewFileName;
		iNewFileName = NULL;
		}
	
	iFileName = aStatus.iFileName.AllocL();
	iNewFileName = aStatus.iNewFileName.AllocL();
	
	iDriveMediaId = aStatus.iDriveMediaId;
	}

//-----------------------------------------------------------------------------
// CMdsFSPQueueItem::~CMdsFSPQueueItem()
//-----------------------------------------------------------------------------
//
CMdsFSPQueueItem::~CMdsFSPQueueItem()
	{
	delete iFileName;
	iFileName = NULL;
	delete iNewFileName;
	iNewFileName = NULL;
	}
    
