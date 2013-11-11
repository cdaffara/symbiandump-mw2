/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester client active object
*
*/


#include "harvesterclientao.h"
#include "harvestercommon.h"
#include "harvesterlog.h"
#include "mdsutils.h"
#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesterclientaoTraces.h"
#endif


// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterClientAO* CHarvesterClientAO::NewL( RHarvesterClient& aHarvesterClient )
	{
    WRITELOG( "CHarvesterClientAO::NewL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_NEWL, "CHarvesterClientAO::NewL" );
    
	CHarvesterClientAO* self = new (ELeave) CHarvesterClientAO( aHarvesterClient );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// ~CHarvesterClientAO
// ---------------------------------------------------------------------------
//
CHarvesterClientAO::~CHarvesterClientAO() // destruct
	{   
    OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_CHARVESTERCLIENTAO, "CHarvesterClientAO::~CHarvesterClientAO" );
    
    WRITELOG( "CHarvesterClientAO::~CHarvesterClientAO()" );
    CancelRequest();
 	}

// ---------------------------------------------------------------------------
// CHarvesterClientAO
// First-phase C++ constructor
// ---------------------------------------------------------------------------
//
CHarvesterClientAO::CHarvesterClientAO( RHarvesterClient& aHarvesterClient )
    : CActive( CActive::EPriorityUserInput ), 
    iObserver( NULL ),
    iHarvesterClient( aHarvesterClient ),
    iCancelled( EFalse )
  	{
    OstTrace0( TRACE_NORMAL, DUP1_CHARVESTERCLIENTAO_CHARVESTERCLIENTAO, "CHarvesterClientAO::CHarvesterClientAO" );
    
    WRITELOG( "CHarvesterClientAO::CHarvesterClientAO()" );
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::ConstructL() // second-phase constructor
	{
    WRITELOG( "CHarvesterClientAO::ConstructL()" );   
    OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_CONSTRUCTL, "CHarvesterClientAO::ConstructL" );
    
    CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// SetObserver
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::SetObserver( MHarvestObserver* aObserver )
	{
	WRITELOG( "CHarvesterClientAO::SetObserver()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_SETOBSERVER, "CHarvesterClientAO::SetObserver" );
	
	iObserver = aObserver;
	}

// ---------------------------------------------------------------------------
// RemoveObserver
// ---------------------------------------------------------------------------
//	
void CHarvesterClientAO::RemoveObserver( MHarvestObserver* /*aObserver*/ )
	{
	WRITELOG( "CHarvesterClientAO::RemoveObserver()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_REMOVEOBSERVER, "CHarvesterClientAO::RemoveObserver" );
    if ( iObserver )
        {
        WRITELOG( "CHarvesterClientAO::RemoveObserver() - deleting observer" );
        iObserver = NULL;
        }
    
    CancelRequest();
	}

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::DoCancel()
	{
	WRITELOG( "CHarvesterClientAO::DoCancel()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_DOCANCEL, "CHarvesterClientAO::DoCancel" );
	iHarvesterClient.UnregisterHarvestComplete();
	WRITELOG( "CHarvesterClientAO::DoCancel() - end" );
	}
	
// ---------------------------------------------------------------------------
// Active
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::Active()
	{	
    WRITELOG( "CHarvesterClientAO::Active" );
	if ( iObserver && !IsActive())
		{
	    WRITELOG( "CHarvesterClientAO::Active - calling RegisterHarvestComplete" );
		iHarvesterClient.RegisterHarvestComplete(iURI, iStatus);
		iCancelled = EFalse;
		SetActive();
		}
	}

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::RunL()
	{
	WRITELOG( "CHarvesterClientAO::RunL()" );
	OstTrace0( TRACE_NORMAL, CHARVESTERCLIENTAO_RUNL, "CHarvesterClientAO::RunL" );

	const TInt status = iStatus.Int();
	
    if ( status < KErrNone )
        {
        WRITELOG1( "CHarvesterClientAO::RunL() - Error occured while harvesting, error:%d", status );
        }

	// Callback to client process
	if ( iObserver )
		{
		WRITELOG( "CHarvesterClientAO::RunL() - ECompleteRequest - calling callback" );
		iObserver->HarvestingComplete( iURI, status );
		}
	
	// if the request was not canceled or server is not terminated, Activating AO again
	if ( status != KErrCancel && status != KErrServerTerminated && iObserver && !iCancelled )
		{
	    WRITELOG( "CHarvesterClientAO::RunL() - not cancelled or terminated, calling Active" );
		Active();
		}
	WRITELOG( "CHarvesterClientAO::RunL() - end" );
	}
	
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//	
#ifdef _DEBUG
TInt CHarvesterClientAO::RunError( TInt aError )
#else
TInt CHarvesterClientAO::RunError( TInt )
#endif
    {
    WRITELOG1( "CHarvesterClientAO::RunError(), errorcode: %d", aError );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Active
// ---------------------------------------------------------------------------
//
void CHarvesterClientAO::CancelRequest()
    {   
    WRITELOG( "CHarvesterClientAO::CancelRequest" );
    if( !iCancelled )
        {
        WRITELOG( "CHarvesterClientAO::CancelRequest - not cancelled, calling Cancel" );
        iCancelled = ETrue;
        Cancel();
        }
    if( !IsActive() )
        {
        WRITELOG( "CHarvesterClientAO::CancelRequest - setting priority to High" );
        SetPriority( CActive::EPriorityHigh );
        }
    }

