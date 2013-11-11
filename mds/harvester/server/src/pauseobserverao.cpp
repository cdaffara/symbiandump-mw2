/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester server's P&S pause observer active object
*
*/

#include "pauseobserverao.h"

#include <harvesterpauseps.h>
#include "harvesterserver.h"

#include "harvesterlog.h"

CPauseObserverAO* CPauseObserverAO::NewL( CHarvesterServer& aHarvesterServer )
	{
	WRITELOG("CPauseObserverAO::NewL()");
	CPauseObserverAO* self = new (ELeave) CPauseObserverAO( aHarvesterServer );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

CPauseObserverAO::~CPauseObserverAO()
	{
	Cancel();
	iPauseProperty.Close();
	}


CPauseObserverAO::CPauseObserverAO( CHarvesterServer& aHarvesterServer ) :
	CActive( CActive::EPriorityUserInput ), 
	iHarvesterServer( aHarvesterServer )
	{
	CActiveScheduler::Add( this );
	}

void CPauseObserverAO::ConstructL()
	{
	WRITELOG("CPauseObserverAO::ConstructL()");

	TUid cat = KPsHarvesterPauseCategory;
	TInt key = KPsHarvesterPauseKey;
	TInt error = iPauseProperty.Define( cat, 
			key, RProperty::EInt );

	WRITELOG1("CPauseObserverAO::ConstructL() Define error: %d", error);

	if( KErrAlreadyExists != error && KErrNone != error )
		{
		User::LeaveIfError( error );
		}

	error = iPauseProperty.Attach( cat, 
			key, EOwnerThread );

	WRITELOG1("CPauseObserverAO::ConstructL() Attach error: %d", error);	

	User::LeaveIfError( error );

	iPauseProperty.Subscribe( iStatus );
    SetActive();
    WRITELOG("CPauseObserverAO::ConstructL() - ends");
	}

void CPauseObserverAO::RunL()
	{
	WRITELOG("CPauseObserverAO::RunL()");

	TInt status = iStatus.Int();

	if( status != KErrNone )
        {
        WRITELOG1("CPauseObserverAO::RunL() - error: %d", status);
        }

    // resubscribe before processing new value to prevent missing updates
	iPauseProperty.Subscribe( iStatus );
    SetActive();

	TInt pauseState = EPsHarvesterPauseResume;

	TUid cat = KPsHarvesterPauseCategory;
	TInt key = KPsHarvesterPauseKey;	
	iPauseProperty.Get( cat, key, pauseState );

	if ( EPsHarvesterPausePause == pauseState )
		{
		WRITELOG("CPauseObserverAO::RunL() - pause");
		iHarvesterServer.Pause();
		}
	else
		{
		WRITELOG("CPauseObserverAO::RunL() - resume");
		iHarvesterServer.Resume();
		}	
	}

void CPauseObserverAO::DoCancel()
	{
	iPauseProperty.Cancel();
	}

TInt CPauseObserverAO::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}
