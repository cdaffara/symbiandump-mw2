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
#include <msvstd.h>
#include <msvfind.h>
#include <msvapi.h>
#include <msvids.h>
#include "mdesession.h"
#include "mdeconstants.h"
#include "mdenamespacedef.h"
#include "messagescannerao.h"
#include "harvesterlog.h"
#include "messagemonitorplugin.h"
#include "clientkeywatcherkeys.h"

CMessageScannerAO* CMessageScannerAO::NewL( CMsvSession* aMsvSession, 
		CMdESession* aMdeSession,
		MMonitorPluginObserver* aMonitorPluginObserver )
	{
	CMessageScannerAO* self = new (ELeave) CMessageScannerAO( 
			aMsvSession, aMdeSession, aMonitorPluginObserver );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

CMessageScannerAO::CMessageScannerAO( CMsvSession* aMsvSession, 
		CMdESession* aMdeSession, 
		MMonitorPluginObserver* aMonitorPluginObserver ) :
		CActive( CActive::EPriorityStandard ),
		iFindOperation( NULL ), iObjectQuery( NULL )
	{
	CActiveScheduler::Add( this );
	iMsvSession = aMsvSession;
	iMdeSession = aMdeSession;
	iMonitorPluginObserver = aMonitorPluginObserver;
	iState = EScannerIdle;
	}

CMessageScannerAO::~CMessageScannerAO()
	{
	Cancel();
	Cleanup();
	}

void CMessageScannerAO::StartL()
	{
	WRITELOG("CMessageScannerAO::Start()");
	_LIT( KEmpty, "" );
	iFindOperation = CMsvFindOperation::FindInChildrenL( *iMsvSession, KEmpty, 
			KMsvRootIndexEntryId, KMsvMessagePartNone, iStatus );
	SetActive();
	iState = EScannerRunning;
	WRITELOG("CMessageScannerAO::Start() - ends");
	}
		
void CMessageScannerAO::ConstructL()
	{
	WRITELOG("CMessageScannerAO::ConstructL()");
	User::LeaveIfError( iProperty.Attach( KPSRestoreWatcherCategory,
			KPSRestoreWatcherClientsKey, EOwnerThread ) );
	
	TInt registeredClients = 0;
	iProperty.Get( registeredClients );
	TInt error = iProperty.Set( registeredClients + 1 );
	
	WRITELOG1("CMessageScannerAO::ConstructL() - error: %d", error);
	}

void CMessageScannerAO::RunL()
	{
	WRITELOG("CMessageScannerAO::RunL()");
	if ( iStatus.Int() != KErrNone )
		{
#ifdef _DEBUG
		WRITELOG1("CMessageScannerAO::RunL() . error %d", iStatus.Int());
#endif
		return;
		}
	
	switch ( iState )
		{
		case EScannerRunning:
			{
			QueryAllMessagesL();
			break;
			}
		case EScannerFinished:
			{
			Cleanup();
			break;
			}
		}
	}

void CMessageScannerAO::DoCancel()
	{
	iFindOperation->Cancel();
	iProperty.Cancel();
	}

#ifdef _DEBUG
TInt CMessageScannerAO::RunError( TInt aError )
#else
TInt CMessageScannerAO::RunError( TInt /*aError*/ )
#endif
	{
	WRITELOG1( "CMessageScannerAO::RunError - error: %d", aError );
	return KErrNone;
	}

void CMessageScannerAO::QueryAllMessagesL()
	{
	WRITELOG("CMessageScannerAO::QueryAllMessagesL()");
	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	CMdEObjectDef& messageDef = namespaceDef.GetObjectDefL( MdeConstants::Message::KMessageObject );
	
	iObjectQuery = iMdeSession->NewObjectQueryL( namespaceDef, messageDef, this );
	iObjectQuery->SetResultMode( EQueryResultModeItem );
	
	iObjectQuery->FindL();
	WRITELOG("CMessageScannerAO::QueryAllMessagesL() - ends");
	}

void CMessageScannerAO::HandleQueryNewResults( CMdEQuery& /*aQuery*/, TInt /*aFirstNewItemIndex*/,
		TInt /*aNewItemCount*/ )
	{
	}

void CMessageScannerAO::HandleQueryCompleted( CMdEQuery& aQuery, TInt /*aError*/ )
	{
	WRITELOG("CMessageScannerAO::HandleQueryCompleted()");
	RArray<TMessage> messages;
	TInt error = KErrNone;
	
	if ( aQuery.Count() > 0 )
		{
#ifdef _DEBUG
		WRITELOG1("CMessageScannerAO::HandleQueryCompleted() - aQuery count %d", aQuery.Count());
#endif
		TRAP( error, ProcessMessagesL( aQuery, messages ) );
		}

	if ( error == KErrNone )
		{
		TRAP_IGNORE( ScanMessagesL( messages ) );
		}
	
	messages.Close();
	Unregister();
	iState = EScannerFinished;
	
	SetActive();
	TRequestStatus* pStatus = &iStatus;
    User::RequestComplete( pStatus, KErrNone );
	}

void CMessageScannerAO::ProcessMessagesL( CMdEQuery& aQuery, RArray<TMessage>& aMessages )
	{
	_LIT( KPeriod, "." );

	CMdENamespaceDef& namespaceDef = iMdeSession->GetDefaultNamespaceDefL();
	CMdEObjectDef& messageDef = namespaceDef.GetObjectDefL( MdeConstants::Message::KMessageObject );
	CMdEPropertyDef& creationDateDef = messageDef.GetPropertyDefL( 
			MdeConstants::Object::KCreationDateProperty );

	TInt count = aQuery.Count();
	
	for (TInt i = 0; i < count; i++ )
		{
		CMdEObject& object = static_cast<CMdEObject&>( aQuery.ResultItem( i ) );
		TInt index = object.Uri().Find( KPeriod );
		if ( index != KErrNotFound )
			{
			TLex16 lex( object.Uri().Left( index ) );
			TMessage message;
			message.iMdeId = object.Id();

			CMdEProperty* creationDateProp = NULL;
			object.Property( creationDateDef, creationDateProp, 0 );
			message.iCreationDate = creationDateProp->TimeValueL();

			TInt error = lex.Val( message.iMsvId );
			if ( error == KErrNone )
				{
				aMessages.Append( message );
				}
			}
		}
	}

void CMessageScannerAO::ScanMessagesL( RArray<TMessage>& aMessages )
	{
	WRITELOG("CMessageScannerAO::ScanMessagesL()");
	const CMsvFindResultSelection& findResults = iFindOperation->GetFindResult();
	CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
	TInt msvCount = findResults.Count();
	
	for ( TInt i = 0; i < msvCount; i++ )
		{
		WRITELOG1("CMessageScannerAO::ScanMessagesL() - MSV item %d", i);
		TBool idMatched = EFalse;
		for ( TInt j = 0; j < aMessages.Count(); j++ )
			{
			WRITELOG1("CMessageScannerAO::ScanMessagesL() - MDE item %d", j);
			if ( findResults[i].iId == aMessages[j].iMsvId )
				{
				WRITELOG("CMessageScannerAO::ScanMessagesL() - id matches");
				idMatched = ETrue;
				CMsvEntry* e = iMsvSession->GetEntryL( findResults[i].iId );
				if ( !e )
					{
					WRITELOG("CMessageScannerAO::ScanMessagesL() - message entry is NULL!?");
					continue;
					}
				
				CleanupStack::PushL( e );
				const TMsvEntry& entry = e->Entry();
				WRITELOG("CMessageScannerAO::ScanMessagesL() - comparing dates");
				if ( entry.iDate != aMessages[j].iCreationDate )
					{
					WRITELOG("CMessageScannerAO::ScanMessagesL() - creation dates don't match");
					// Harvest the message with EHarvesterEdit event type.
					HBufC* uri = CMessageMonitorPlugin::CreateUriL( entry.iMtm, entry.Id() );
					CleanupStack::PushL( uri );
					CHarvesterData* hd = CHarvesterData::NewL( uri );
					CleanupStack::Pop( uri );
					hd->SetEventType( EHarvesterEdit );
					hd->SetTakeSnapshot( EFalse );
					hd->SetBinary( EFalse );
					iMonitorPluginObserver->MonitorEvent( hd );
					}
				
				CleanupStack::PopAndDestroy( e );
				aMessages.Remove( j );
				j--;
				break;
				}
			}
		
		if ( !idMatched )
			{
			WRITELOG("CMessageScannerAO::ScanMessagesL() - harvesting new message");
			// Harvest the message as new message.
			CMsvEntry* e = iMsvSession->GetEntryL( findResults[i].iId );
			if ( !e )
				{
				WRITELOG("CMessageScannerAO::ScanMessagesL() - message entry is NULL!?");
				continue;
				}
			
			CleanupStack::PushL( e );
			const TMsvEntry& entry = e->Entry();
			HBufC* uri = CMessageMonitorPlugin::CreateUriL( entry.iMtm, entry.Id() );
			CleanupStack::PushL( uri );
			CHarvesterData* hd = CHarvesterData::NewL( uri );
			CleanupStack::Pop( uri );
			hd->SetEventType( EHarvesterAdd );
			hd->SetTakeSnapshot( EFalse );
			hd->SetBinary( EFalse );
			iMonitorPluginObserver->MonitorEvent( hd );
			
			CleanupStack::PopAndDestroy( e );
			}
		}
	
	// Remove extra messages in MdS db.
	if ( aMessages.Count() > 0 )
		{
		WRITELOG1("CMessageScannerAO::ScanMessagesL() - removing extra messages from mde (%d)", 
				aMessages.Count() );
		RArray<TItemId> extraMessages;
		CleanupClosePushL( extraMessages );
		
		for ( TInt i = 0; i < aMessages.Count(); i++ )
			{
			extraMessages.Append( aMessages[i].iMdeId );
			}
		
		RArray<TItemId> results;
		CleanupClosePushL( results );
		iMdeSession->RemoveObjectsL( extraMessages, results, &defaultNamespace );
		CleanupStack::PopAndDestroy( 2 ); // extramessages, results
		}
	}

void CMessageScannerAO::Unregister()
	{
	TInt registeredClients = 0;
	iProperty.Get( registeredClients );
	WRITELOG1("CMessageScannerAO::Unregister() - registered clients: %d", registeredClients);
	if ( registeredClients > 0 )
		{
		iProperty.Set( registeredClients - 1 );
		}
	}

void CMessageScannerAO::Cleanup()
	{
	iProperty.Close();
	delete iFindOperation;
	iFindOperation = NULL;
	delete iObjectQuery;
	iObjectQuery = NULL;
	}

