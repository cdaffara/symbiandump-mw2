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

#include "CTrackLogObserverAO.h"
#include "locationmanagerdebug.h"

CTrackLogObserverAO::CTrackLogObserverAO( RTrackLog* aClient ) : 
	CActive( CActive::EPriorityStandard )
	{
	CActiveScheduler::Add( this );
	iClient = aClient;
	}

CTrackLogObserverAO::~CTrackLogObserverAO()
	{
	Cancel();
	}

void CTrackLogObserverAO::DoCancel()
	{
	
	}

void CTrackLogObserverAO::RunL()
	{
	LOG( "CTrackLogObserverAO::RunL()" );
	iClient->TrackLogEvent( iStatus.Int() );
	
	StartObserving();
	}
	
TInt CTrackLogObserverAO::StartObserving()
	{
	iClient->SendNotifyRequest( iStatus );
	SetActive();
	
	return KErrNone;
	}

