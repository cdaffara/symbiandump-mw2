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
* Description:  
*/
#include "ondemandao.h"
#include "mdeharvestersession.h"
#include "harvesterdata.h"
#include "harvesterlog.h"
#include "harvesterplugin.h"
#include "harvesterplugininfo.h"
#include "harvesterpluginfactory.h"

COnDemandAO::COnDemandAO() :
	CActive( CActive::EPriorityUserInput )
	{
	WRITELOG("COnDemandAO::COnDemandAO");
	}

COnDemandAO* COnDemandAO::NewLC( CMdESession& aSession, MMonitorPluginObserver& aObserver,
		CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray )
	{
	WRITELOG("COnDemandAO::NewLC");
	COnDemandAO* self = new ( ELeave ) COnDemandAO();
	CleanupStack::PushL ( self);
	self->ConstructL ( aSession, aObserver, aPluginFactory, aPhArray );
	return self;
	}

COnDemandAO* COnDemandAO::NewL( CMdESession& aSession, MMonitorPluginObserver& aObserver,
		CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray )
	{
	WRITELOG("COnDemandAO::NewL");
	COnDemandAO* self = COnDemandAO::NewLC ( aSession, aObserver, aPluginFactory, aPhArray );
	CleanupStack::Pop (); // self;
	return self;
	}

void COnDemandAO::ConstructL( CMdESession& aSession, MMonitorPluginObserver& aObserver,
		CHarvesterPluginFactory& aPluginFactory, RPointerArray<CHarvesterData>* aPhArray )
	{
	WRITELOG("COnDemandAO::ConstructL");
	iMdEHarvesterSession = CMdEHarvesterSession::NewL ( aSession);
	iObserver = &aObserver;
	iPluginFactory = &aPluginFactory;
	iReadyPhArray = aPhArray;
	CActiveScheduler::Add ( this ); // Add to scheduler
	}

COnDemandAO::~COnDemandAO()
	{
	WRITELOG("COnDemandAO::~COnDemandAO");
	Cancel (); // Cancel any request, if outstanding
	if( iMdEHarvesterSession )
		{
		delete iMdEHarvesterSession;
		iMdEHarvesterSession = NULL;
		}
	// Delete instance variables if any
	}

void COnDemandAO::DoCancel()
	{
	WRITELOG("COnDemandAO::DoCancel");
	iMdEHarvesterSession->CancelHarvestingPrioritizationObserver();
	}

void COnDemandAO::StartL()
	{
	WRITELOG("COnDemandAO::StartL");
	// Cancel any request, just to be sure
	Cancel ();
	iMdEHarvesterSession->SetHarvestingPrioritizationChunkL( 16384 );
	WaitHarvestingRequest ();
	}

void COnDemandAO::RunL()
	{
	WRITELOG("COnDemandAO::RunL");
	if( iStatus.Int() != KErrNotFound )
		{
		User::LeaveIfError ( iStatus.Int ());
		
		TInt count = iMdEHarvesterSession->HarvestingPrioritizationUriCountL();
		if( count > 0 )
			{
			RPointerArray<CHarvesterPluginInfo>& pluginInfos = iPluginFactory->GetPluginInfos();
			TInt plugins = pluginInfos.Count();

			for (TInt i = count; --i >= 0;)
				{
				HBufC* uri = iMdEHarvesterSession->HarvestingPrioritizationUriL( i );
				CleanupStack::PushL( uri );
				TBool found = EFalse;

				for ( TInt j = 0; j < plugins && !found; j++ )
					{
					RPointerArray<CHarvesterData>& queue = pluginInfos[j]->iQueue;
					TInt queueSize = queue.Count();
					for ( TInt k = 0; k < queueSize && !found; k++ )
						{				
						if ( queue[k]->Uri().CompareF( *uri ) == 0 )
							{
							WRITELOG2("COnDemandAO::RunL URI %S found in plugin %d queue", uri, j);
							CHarvesterData* hd = queue[k];
							hd->SetEventType( EHarvesterEdit );
							hd->SetObjectType( EFastHarvest );
							queue.Remove( k );

							if(queue.Insert( hd, 0 ) != KErrNone)
							    {
                                delete hd;
                                hd = NULL;
							    }
							
                            found = ETrue;
							}							
						}
					}

				const TInt readyPhArraySize = iReadyPhArray->Count();
				for ( TInt j = 0; j < readyPhArraySize && !found; j++ )
					{
					if ( (*iReadyPhArray)[j]->Uri().CompareF( *uri ) == 0 )
						{
						WRITELOG1("COnDemandAO::RunL URI %S found in ph array", uri);
						CHarvesterData* hd = (*iReadyPhArray)[j];
						hd->SetEventType( EHarvesterEdit );
						hd->SetObjectType( EFastHarvest );
						iReadyPhArray->Remove( j );
						iReadyPhArray->Insert( hd, 0 );
						found = ETrue;
						}
					}

				if ( !found )
					{
					WRITELOG1("COnDemandAO::RunL URI %S wasn't found in placeholder queue.", uri);
					
					CMdESession& session = iMdEHarvesterSession->SessionRef();
					TMdEObject object;
					TRAPD( err, session.CheckObjectL( object, *uri ));
					if ( err == KErrNone && object.Placeholder() )
						{
						CHarvesterData* hd = CHarvesterData::NewL( uri );
						hd->SetEventType( EHarvesterEdit );
						hd->SetObjectType( EFastHarvest );
						iObserver->MonitorEvent( hd );
						CleanupStack::Pop( uri );
						}
					else
						{
						CleanupStack::PopAndDestroy( uri );
						}
					}
				else
					{
					CleanupStack::PopAndDestroy( uri );
					}
				}
			}
		}

	WaitHarvestingRequest();
	}

TInt COnDemandAO::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}

void COnDemandAO::WaitHarvestingRequest()
	{
	WRITELOG("COnDemandAO::WaitHarvestingRequest");
	iStatus = KRequestPending;
	iMdEHarvesterSession->AddHarvestingPrioritizationObserver( iStatus );
	// Tell scheduler a request is active
	SetActive();
	}
