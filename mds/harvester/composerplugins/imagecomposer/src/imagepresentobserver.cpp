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
* Description:  image composer listener for setting pending image objects present
*
*/


#include "imagepresentobserver.h"
#include "harvesterlog.h"
#include "mdeconstants.h"
#include "composerimageplugin.h"
#include "mdcserializationbuffer.h"
#include "mdsutils.h"
#include "harvestercommon.h"

using namespace MdeConstants;

const TInt KBatchSize = 100;

CImagePresentObserver::CImagePresentObserver() : 
		CActive( KHarvesterPriorityComposerPlugin ),
		iDefaultNamespace( NULL ),
		iSession( NULL ),
		iMdEHarvesterSession( NULL ),
		iComposer( NULL ),
		iNextRequest( ERequestIdle ),
		iPendingCount( 0 ),
		iPendingBuffer( NULL ),
		iStarted( EFalse )
	{
	// No implementation required
	}

CImagePresentObserver::~CImagePresentObserver()
	{
	if( iSession )
	    {
	    TRAP_IGNORE( iSession->RemoveObjectPresentObserverL( *this ) );
	    }
	
	CleanUp();
	}

CImagePresentObserver* CImagePresentObserver::NewL( CMdESession* aSession, CComposerImagePlugin* aComposer, 
		CMdEHarvesterSession* aMdEHarvesterSession )
	{
	WRITELOG( "CImagePresentObserver::NewL start" );
	CImagePresentObserver* self = new (ELeave)CImagePresentObserver();
	CleanupStack::PushL(self);
	self->ConstructL( aSession, aComposer, aMdEHarvesterSession );
	CleanupStack::Pop( self ); 
	WRITELOG( "CImagePresentObserver::NewL end" );
	return self;
	}

void CImagePresentObserver::ConstructL( CMdESession* aSession, CComposerImagePlugin* aComposer, 
		CMdEHarvesterSession* aMdEHarvesterSession )
	{
    WRITELOG( "CImagePresentObserver::ConstructL start" );
    User::LeaveIfNull( iSession = aSession );
    User::LeaveIfNull( iComposer = aComposer );
    
    iDefaultNamespace = &iSession->GetDefaultNamespaceDefL();

    iSession->AddObjectPresentObserverL( *this );
    iMdEHarvesterSession = aMdEHarvesterSession;
    
    iStarted = EFalse;
    
    CActiveScheduler::Add( this );

    WRITELOG( "CImagePresentObserver::ConstructL end" );
	}


void CImagePresentObserver::HandleObjectPresentNotification(CMdESession& /*aSession*/, 
		TBool aPresent, const RArray<TItemId>& aObjectIdArray)
	{
    WRITELOG( "CImagePresentObserver::HandleObjectPresentNotification" );
	if ( aPresent )
		{
	    WRITELOG( "CImagePresentObserver::HandleObjectPresentNotification - objects present" );
		if ( !iStarted )
			{
			CMdEObjectDef* imageObjDef = NULL;
			TRAP_IGNORE( imageObjDef = &iDefaultNamespace->GetObjectDefL( 
					MdeConstants::Image::KImageObject ));
			
			iPendingCount = iMdEHarvesterSession->GetPendingCount( imageObjDef );
			SetNextRequest( ERequestWaitPendingCount );
			iStarted = ETrue;
			}
		TInt count = aObjectIdArray.Count();
		for (TInt i = 0; i < count; i++ )
			{
			iPresentObjectIds.Append( aObjectIdArray[i] );
			}
		}
	}


void CImagePresentObserver::RunL()
	{
	switch ( iNextRequest )
		{
		case ERequestIdle:
			{
			if( iComposeObjectIds.Count() > 0 )
				{
				iComposer->HandlePendingObjects( iComposeObjectIds );			
				iComposeObjectIds.Reset();
				}
			iPendingObjectIds.Reset();
			iPresentObjectIds.Reset();
			delete iPendingBuffer;
			iPendingBuffer = NULL;
			iStarted = EFalse;
			break;
			}
		
		case ERequestWaitPendingCount:
			{
			if ( iPendingCount > 0 )
				{
				CMdEObjectDef& imageObjDef = iDefaultNamespace->GetObjectDefL( 
						MdeConstants::Image::KImageObject );

				iPendingBuffer = CMdCSerializationBuffer::NewL( iPendingCount * 
						sizeof(TItemId) );
			
				const TInt result = iMdEHarvesterSession->GetPending( &imageObjDef,
						iPendingCount, *iPendingBuffer );
				
				SetNextRequest( ERequestWaitPending );
				}
			else
				{
				SetNextRequest( ERequestIdle );
				}
			break;
			}
			
		case ERequestWaitPending:
			{
			if ( iPendingBuffer )
				{
				DeserializeArrayL( iPendingBuffer->Buffer(), iPendingObjectIds );
				}
			
			SetNextRequest( ERequestProcess );
			break;
			}
		
		case ERequestProcess:
			{
			if( iPresentObjectIds.Count() > 0 && iPendingObjectIds.Count() > 0 )
				{
				TItemId itemId = iPresentObjectIds[0];
				iPresentObjectIds.Remove( 0 );
				
				TInt found = iPendingObjectIds.Find( itemId );
				if ( found != KErrNotFound )
					{
					iPendingObjectIds.Remove( found );
					iComposeObjectIds.Append( itemId );
					}
				}
			else 
				{
				SetNextRequest( ERequestIdle );
				
				iPendingObjectIds.Compress();
				iPresentObjectIds.Compress();
				
				break;
				}

			if( iComposeObjectIds.Count() > KBatchSize )
				{
				iComposer->HandlePendingObjects( iComposeObjectIds );		
				iComposeObjectIds.Reset();
				}
			
			SetNextRequest ( ERequestProcess );	
			break;
			}
		}
	}

void CImagePresentObserver::DoCancel()
	{
	}

TInt CImagePresentObserver::RunError( TInt /*aError*/ )
	{
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// SetNextRequest
// ---------------------------------------------------------------------------
//
void CImagePresentObserver::SetNextRequest( TRequest aRequest )
    {
    iNextRequest = aRequest;
            
    if ( !IsActive() )
        {
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* ptrStatus = &iStatus;
        User::RequestComplete( ptrStatus, KErrNone );
        }
    }


void CImagePresentObserver::CleanUp()
	{
	iPendingObjectIds.Close();
	iPresentObjectIds.Close();
	iComposeObjectIds.Close();
	delete iPendingBuffer;
	iPendingBuffer = NULL;
	}

