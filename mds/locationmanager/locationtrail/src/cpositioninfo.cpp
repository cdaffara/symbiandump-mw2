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
* Description:  A class for getting network cell id.
*
*/

#include <etel3rdparty.h>

#include "rlocationtrail.h"
#include "cpositioninfo.h"
#include "locationtraildefs.h"
#include "locationmanagerdebug.h"

// --------------------------------------------------------------------------
// CPositionInfo::NewL
// --------------------------------------------------------------------------
//
EXPORT_C CPositionInfo* CPositionInfo::NewL( MPositionInfoObserver* aTrail )
    {
    LOG( "CPositionInfo::NewL(), begin" );
    CPositionInfo* self = new (ELeave) CPositionInfo( aTrail );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    LOG( "CPositionInfo::NewL(), end" );
    return self;
    }
        
// --------------------------------------------------------------------------
// CPositionInfo::CPositionInfo
// --------------------------------------------------------------------------
//  
CPositionInfo::CPositionInfo( MPositionInfoObserver* aTrail ) 
    : CActive( CActive::EPriorityStandard ),
    iState( EPositionOptStateNone ),
    iConnectedPositionServer(EFalse)
    {
    LOG( "CPositionInfo::CPositionInfo()");
    CActiveScheduler::Add( this );
    iTrail = aTrail;
    iTrailCaptureSetting = RLocationTrail::ECaptureAll;
    
    // Set update interval.
    iUpdateOptions.SetUpdateInterval( TTimeIntervalMicroSeconds(KFirstInterval) );
    // Set time out level. 
    iUpdateOptions.SetUpdateTimeOut( TTimeIntervalMicroSeconds(KFirstTimeOut) );
    // Positions which have time stamp below KMaxAge can be reused
    iUpdateOptions.SetMaxUpdateAge( TTimeIntervalMicroSeconds(KMaxAge) );
    // Disables location framework to send partial position data
    iUpdateOptions.SetAcceptPartialUpdates( EFalse );    
    }

// --------------------------------------------------------------------------
// CPositionInfo::ConstructL
// --------------------------------------------------------------------------
//    
void CPositionInfo::ConstructL()
    {
    
    }
    
// --------------------------------------------------------------------------
// CPositionInfo::~CPositionInfo
// --------------------------------------------------------------------------
//    
EXPORT_C CPositionInfo::~CPositionInfo()
    {
    Stop();
    }

// --------------------------------------------------------------------------
// CPositionInfo::RunError
// --------------------------------------------------------------------------
//
TInt CPositionInfo::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// --------------------------------------------------------------------------
// CPositionInfo::GetCellId
// --------------------------------------------------------------------------
//
void CPositionInfo::StartL( RLocationTrail::TTrailCaptureSetting aCaptureSetting, TInt aUpdateInterval )
    {
    LOG( "CPositionInfo::StartL(), begin" );
    if(IsActive()) 
        {
        Cancel();
        }
    iTrailCaptureSetting = aCaptureSetting;
    iUpdateInterval = aUpdateInterval;
    iState = EPositionOptStateNone;
    
    iPositionInfo = TPositionSatelliteInfo();
    
    if ( aCaptureSetting == RLocationTrail::ECaptureAll ) 
    	{
    	if(!iConnectedPositionServer)
            {   
             // Positions which have time stamp below KMaxAge can be reused
             iUpdateOptions.SetMaxUpdateAge( TTimeIntervalMicroSeconds(KMaxAge) );
             // Disables location framework to send partial position data
             iUpdateOptions.SetAcceptPartialUpdates( EFalse );
    	    User::LeaveIfError( iPosServer.Connect() );
            CleanupClosePushL(iPosServer);
    	    User::LeaveIfError( iPositioner.Open( iPosServer ) );
            CleanupClosePushL(iPositioner);
    	    User::LeaveIfError( iPositioner.SetRequestor( CRequestor::ERequestorService,
    	                        CRequestor::EFormatApplication, KRequestor ) );
            iUpdateOptions.SetUpdateInterval( TTimeIntervalMicroSeconds (iUpdateInterval) );  
            iUpdateOptions.SetUpdateTimeOut( TTimeIntervalMicroSeconds(KUpdateTimeOut ) );
            User::LeaveIfError( iPositioner.SetUpdateOptions( iUpdateOptions ) );
            CleanupStack::Pop(2); // iPositioner, iPosServer
            iConnectedPositionServer = ETrue;
           }
        iState = EPositionOptStateGetLastKnownPosition;
        iPositioner.GetLastKnownPosition( iPositionInfo, iStatus );
        SetActive();
    	}
    else if ( aCaptureSetting == RLocationTrail::ECaptureNetworkInfo ) 
    	{
    	TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
    	}

    LOG( "CPositionInfo::StartL(), end" );
    }

// --------------------------------------------------------------------------
// CPositionInfo::NextPosition
// --------------------------------------------------------------------------
//
void CPositionInfo::NextPosition()
    {
    LOG( "CPositionInfo::NextPosition(), begin" );
    if(!IsActive() && iConnectedPositionServer)
        {
        LOG("Not active");
        iPositionInfo = TPositionSatelliteInfo(); // Clear position info.
        if ( iTrailCaptureSetting == RLocationTrail::ECaptureAll )
        	{
            iState = EPositionOptStateNotifyUpdate;
        	iPositioner.NotifyPositionUpdate( iPositionInfo, iStatus );
            SetActive();
        	}
        else if ( iTrailCaptureSetting == RLocationTrail::ECaptureNetworkInfo ) 
        	{
            SetActive();
        	TRequestStatus* status = &iStatus;
            User::RequestComplete( status, KErrNone );
        	}
        }
    LOG( "CPositionInfo::NextPosition(), end" );
    }
    
// --------------------------------------------------------------------------
// CPositionInfo::Stop
// --------------------------------------------------------------------------
//
void CPositionInfo::Stop()
    {
    LOG( "CPositionInfo::Stop(), begin" );
    Cancel();    
    if(iConnectedPositionServer)
        {
        iPositioner.Close();
        iPosServer.Close();
        iConnectedPositionServer = EFalse;
        }
    // reset the state
    iState = EPositionOptStateNone;
    LOG( "CPositionInfo::Stop(), end" );
    }    
        
// --------------------------------------------------------------------------
// CPositionInfo::RunL
// --------------------------------------------------------------------------
//
void CPositionInfo::RunL()
    { 
    LOG( "CPositionInfo::RunL(), begin" );
    if(iState == EPositionOptStateGetLastKnownPosition)
        {
        // get last location.. check the time and if it's within the limit, pass to trail.
        const TTimeIntervalSeconds KMaxAllowedLastKnownPosition(60*5); // 5 mins
        TTimeIntervalSeconds interval;
        TTime now;
        TPosition lastPosition;
        iPositionInfo.GetPosition(lastPosition);
        now.UniversalTime();
        now.SecondsFrom(lastPosition.Time(), interval);
        if(iStatus.Int() == KErrNone && interval < KMaxAllowedLastKnownPosition)
            {
            LOG("Last know position is recent one");
            iTrail->Position( iPositionInfo, iStatus.Int() );
            }
        else
            {
            LOG("Old last know position. Drop it..");
            }
        }
    else
        {
        // notify response.. always pass to trail
        iTrail->Position( iPositionInfo, iStatus.Int() );
        }
     
    if ( iTrailCaptureSetting == RLocationTrail::ECaptureAll &&
        iState != EPositionOptStateNone) 
    	{
    	NextPosition();
    	}
    LOG( "CPositionInfo::RunL(), end" );
    }    

// --------------------------------------------------------------------------
// CPositionInfo::DoCancel
// --------------------------------------------------------------------------
// 
void CPositionInfo::DoCancel()
    {
    LOG( "CPositionInfo::DoCancel(), begin" );
    switch(iState)
        {
        case EPositionOptStateGetLastKnownPosition:
            {
            iPositioner.CancelRequest( EPositionerGetLastKnownPosition );
            break;
            }
        case EPositionOptStateNotifyUpdate:
            {
            iPositioner.CancelRequest( EPositionerNotifyPositionUpdate );
            break;
            }
        default:
            break;
        }
	iState = EPositionOptStateNone;
    LOG( "CPositionInfo::DoCancel(), end" );
    }


// --------------------------------------------------------------------------
// CPositionInfo::HandleRemapComplete
// --------------------------------------------------------------------------
// 
void CPositionInfo::HandleRemapComplete()
    {
    LOG( "CPositionInfo::HandleRemapComplete()" );
    // Don't call notify update from RunL
    iState = EPositionOptStateNone;
    }

// End of file

