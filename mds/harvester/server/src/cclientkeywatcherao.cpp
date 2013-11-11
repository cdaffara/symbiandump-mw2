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

#include "harvesterlog.h"
#include "cclientkeywatcherao.h"
#include "crestorewatcher.h"

CClientKeyWatcherAO* CClientKeyWatcherAO::NewL( CRestoreWatcher* aRestoreWatcher )
	{
	WRITELOG("CClientKeyWatcherAO::NewL()");
	CClientKeyWatcherAO* self = new (ELeave) CClientKeyWatcherAO( aRestoreWatcher );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

CClientKeyWatcherAO::~CClientKeyWatcherAO()
	{
	Cancel();
	iClientsProperty.Close();

	iRestoreWatcher = NULL;
	}


CClientKeyWatcherAO::CClientKeyWatcherAO( CRestoreWatcher* aRestoreWatcher ) :
		CActive( CActive::EPriorityUserInput )
	{
	CActiveScheduler::Add( this );
	iRestoreWatcher = aRestoreWatcher;
	}

void CClientKeyWatcherAO::ConstructL()
	{
	WRITELOG("CClientKeyWatcherAO::ConstructL()");
	
	User::LeaveIfError( iClientsProperty.Attach( KPSRestoreWatcherCategory, 
			KPSRestoreWatcherClientsKey, EOwnerThread ) );
    
	iClientsProperty.Subscribe( iStatus );
    SetActive();
    WRITELOG("CClientKeyWatcherAO::ConstructL() - ends");
	}

void CClientKeyWatcherAO::RunL()
	{
	WRITELOG("CClientKeyWatcherAO::RunL()");
	if( iStatus.Int() != KErrNone )
        {
#ifdef _DEBUG
        WRITELOG1("CClientKeyWatcherAO::RunL() - error: %d", iStatus.Int());
#endif
        return;
        }
	
	TInt clients = 0;
	iClientsProperty.Get( clients );
	
	if ( clients < iClients )
		{
		WRITELOG("CClientKeyWatcherAO::RunL() - unregistering");
		iRestoreWatcher->UnregisterL();
		}
	else if ( clients > iClients )
		{
		WRITELOG("CClientKeyWatcherAO::RunL() - registering");
		iRestoreWatcher->Register();
		}
	
	iClients = clients;
	
	iClientsProperty.Subscribe( iStatus );
	SetActive();
	}

void CClientKeyWatcherAO::DoCancel()
	{
	iClientsProperty.Cancel();
	}

TInt CClientKeyWatcherAO::RunError( TInt )
	{
	return KErrNone;
	}
