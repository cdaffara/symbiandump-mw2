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
* Description:  Active Object which actually performs the re-harvesting of the files*
*/

#include "reharvesterao.h"
#include "harvesterlog.h"
#include "mdeobject.h"
#include "mdsutils.h"

const TInt KResumeTime = 3000000;  //microseconds
const TInt KTimeIncrease = 1000000; //microseconds
const TInt KTimeLimit = 30000000;    //microseconds

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CReHarvesterAO* CReHarvesterAO::NewLC()
	{
    WRITELOG( "CReHarvesterAO::NewLC() - begin" );
    
	CReHarvesterAO* self = new (ELeave) CReHarvesterAO();
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}
// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CReHarvesterAO* CReHarvesterAO::NewL()
	{  
    WRITELOG( "CReHarvesterAO::NewL() - begin" );
    
	CReHarvesterAO* self = CReHarvesterAO::NewLC();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// CReHarvesterAO
// ---------------------------------------------------------------------------
//
CReHarvesterAO::CReHarvesterAO() : CActive( CActive::EPriorityStandard ),
    iQueue( NULL ), iDelay( KResumeTime )
 	{
    WRITELOG( "CReHarvesterAO::CReHarvesterAO()" );
 	}
 	
// ---------------------------------------------------------------------------
// ~CReHarvesterAO
// ---------------------------------------------------------------------------
//
CReHarvesterAO::~CReHarvesterAO()
	{
	WRITELOG( "CReHarvesterAO::~CReHarvesterAO()" );
	
	Cancel();
	iItems.ResetAndDestroy();
	iItems.Close();
	iTimer.Close();
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::ConstructL()
	{
	WRITELOG( "CReHarvesterAO::ConstructL()" );
	
	CActiveScheduler::Add( this );
	iTimer.CreateLocal();
	}

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::RunL()
	{
	WRITELOG( "ATTENTION!!! CReHarvesterAO::RunL" );
	User::LeaveIfError( iStatus.Int() );
	if ( iItems.Count() > 0 )
	    {
	    while( iItems.Count() > 0 )
    	    {
    	    CHarvesterData* item = iItems[0];
    	    iQueue->MonitorEvent( item );
    	    iItems.Remove( 0 );
    	    }
	    
	    iItems.Compress();
	    
	    iDelay += KTimeIncrease;
	    if( iDelay > KTimeLimit )
	        {
	        iDelay = KResumeTime;
	        }
	    
	    const TTimeIntervalMicroSeconds32 delay = TTimeIntervalMicroSeconds32( iDelay );
    	iTimer.After( iStatus, delay );
	    SetActive();
	    }
	}
	
// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::DoCancel()
	{
	WRITELOG( "CReHarvesterAO::DoCancel()" );
	}
	
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CReHarvesterAO::RunError( TInt aError )
	{
	if (aError != KErrNone)
		{	
		WRITELOG1( "CReHarvesterAO::RunError - error: %d", aError );
		}
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// SetHarvesterQueue
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::SetHarvesterQueue( CHarvesterQueue* aQueue )
    {
    WRITELOG( "CReHarvesterAO::SetHarvesterQueue()" );
    iQueue = aQueue;
    }
    
// ---------------------------------------------------------------------------
// AddItem
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::AddItem( CHarvesterData* aItem )
    {
    WRITELOG( "CReHarvesterAO::AddItem()" );
    aItem->SetErrorCode( KErrNone );
    // check if already exists
    for( TInt i = iItems.Count(); --i >= 0; )
        {
        if ( MdsUtils::Compare( iItems[i]->Uri(), aItem->Uri() ) == 0 )
            {
#ifdef _DEBUG
            WRITELOG1("CReHarvesterAO::AddItem() - %S already exists in re-harvester queue", &aItem->Uri() );
#endif
            delete aItem;
            aItem = NULL;
            return;
            }
        }
    
    aItem->SetMdeObject( NULL );
    
    if(iItems.Append( aItem ) != KErrNone )
        {
        delete aItem;
        aItem = NULL;
        }

    iDelay = KResumeTime;
    const TTimeIntervalMicroSeconds32 delay = TTimeIntervalMicroSeconds32( iDelay );
    
    if ( !IsActive() )
        {
        iTimer.After( iStatus, delay );
        SetActive();
        }
    }
    
// ---------------------------------------------------------------------------
// CheckItem
// ---------------------------------------------------------------------------
//
void CReHarvesterAO::CheckItem( CHarvesterData& aItem )
    {
    WRITELOG( "CReHarvesterAO::CheckItem()" );
    
    for( TInt i = iItems.Count(); --i >= 0; )
        {
        CHarvesterData* item = iItems[i];
        if ( aItem.Uri().CompareF( item->Uri() ) != 0 )
            {
            continue;
            }
        // found matching item
	    iItems.Remove( i );
	    delete item;
	    item = NULL;
        }
    
    if ( iItems.Count() == 0 )
        {
        WRITELOG( "CReHarvesterAO::CheckItem() - item count 0" );
        
        iItems.Compress();
        
        Cancel();
        iTimer.Cancel();
        }
    }

// ---------------------------------------------------------------------------
// ItemsInQueue
// ---------------------------------------------------------------------------
//
TInt CReHarvesterAO::ItemsInQueue()
	{
	return iItems.Count();
	}
