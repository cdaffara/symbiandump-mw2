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
* Description:  An interface to Track Log API
*
*/

#include "rtracklog.h"
#include "locationmanagerdebug.h"
#include "CTrackLogObserverAO.h"
#include "locationmanagerdefs.h"

EXPORT_C RTrackLog::RTrackLog() : RLocationManager(),
	iWrapQuality( TPckg<TPositionSatelliteInfo>( iFixQuality ) ),
	iWrapRec( TPckg<TBool>( iRecording ) ),
	iWrapEventType( TPckg<TEventTypes>( iEventType ) )
{
}

// --------------------------------------------------------------------------
// RTrackLog::StartLocationTrail
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::StartTrackLog()
    {
    LOG( "RTrackLog::StartTrackLog()" );
    TInt ret( KErrDisconnected );
    if ( iHandle )
        {
        ret = SendReceive( ELocManStartTrackLog );
        }

    return ret;
    }

// --------------------------------------------------------------------------
// RTrackLog::StopTrackLog
// --------------------------------------------------------------------------
//
EXPORT_C void RTrackLog::StopTrackLog()
    {
    LOG( "RTrackLog::StopTrackLog()" );
    if ( iHandle )
        {
        Send( ELocManStopTrackLog );
        }   
    }

// --------------------------------------------------------------------------
// RTrackLog::GetStatus
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::GetStatus( TBool& aRecording, TPositionSatelliteInfo& aFixQuality )
    {
    LOG( "RTrackLog::GetStatus()" );
    TInt ret( KErrDisconnected );
    TPckg<TBool> wrapRec( aRecording );
    TPckg<TPositionSatelliteInfo> wrapQuality( aFixQuality );
    
    if ( iHandle )
        {
        ret = SendReceive( ELocManGetTrackLogStatus, TIpcArgs( &wrapRec, &wrapQuality ) );
        }
    
    return ret;
    }

// --------------------------------------------------------------------------
// RTrackLog::DeleteTrackLog
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::DeleteTrackLog( TDesC& aUri )
    {
    LOG( "RTrackLog::DeleteTrackLog()" );
    TInt ret( KErrDisconnected );
    if ( iHandle )
        {
        ret = SendReceive( ELocManDeleteTrackLog, TIpcArgs( &aUri ) );
        }
    return ret;
    }

// --------------------------------------------------------------------------
// RTrackLog::IsRecording
// --------------------------------------------------------------------------
//
EXPORT_C TBool RTrackLog::IsRecording()
    {
    LOG( "RTrackLog::IsRecording()" );
    TBool rec = EFalse;
    TPckg<TBool> wrapRec( rec );

    if ( iHandle )
        {
        SendReceive( ELocManIsTrackLogRecording, TIpcArgs( &wrapRec ) );
        }
    
    return rec;
    }

// --------------------------------------------------------------------------
// RTrackLog::AddObserver
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::AddObserver( MTrackLogObserver* aObserver )
    {
    LOG( "RTrackLog::AddObserver()" );
    
    TInt err( KErrNone );
    
    if ( iSubscribed )
    	{
    	return KErrAlreadyExists;
    	}
    
    if ( !iHandle )
    	{
    	return KErrDisconnected;
    	}
    
    // add message to observer pointer array    
    err = iObservers.Append( aObserver );
    
    if ( err != KErrNone  )
    	{
    	return err;
    	}
    
    if ( !iTrackLogObserverAO )
    	{
    	iTrackLogObserverAO = new CTrackLogObserverAO( this );
    	}
        
	if ( iTrackLogObserverAO )
		{
		iSubscribed = ETrue;
		iTrackLogObserverAO->StartObserving();
		}
	else 
		{
		// If creating observer AO failed, reset the observer list (don't destroy observers in it).
		iObservers.Reset();
		err = KErrNoMemory;
		}
   
    return err;
    }

// --------------------------------------------------------------------------
// RTrackLog::RemoveObserver
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::RemoveObserver( MTrackLogObserver* aObserver )
    {
    LOG( "RTrackLog::RemoveObserver()" );
    TInt err( KErrNotFound  );
    TInt index( 0 );
    
    // remove message from observer pointer array
    TRAP(err, index = iObservers.FindL(aObserver));
    if (err == KErrNone)
    	{
    	iObservers.Remove(index);
    	}
  
    if ( iObservers.Count() == 0 )
    	{
    	if ( iHandle )
        	{
            SendReceive( ELocManCancelTrackLogNotify );
        	}
    	iSubscribed = EFalse;
    	delete iTrackLogObserverAO;
    	iTrackLogObserverAO = NULL;
    	}
    
    return err;
    }

// --------------------------------------------------------------------------
// RTrackLog::TrackLogName
// --------------------------------------------------------------------------
//
EXPORT_C TInt RTrackLog::TrackLogName(TDes& aTrackLogName)
    {
    LOG( "RTrackLog::TrackLogName()" );
    TInt ret( KErrDisconnected );
    
    if ( iHandle )
        {
        ret = SendReceive( ELocManTrackLogName, TIpcArgs( &aTrackLogName ) );
        }
    
    return ret;
    }

// --------------------------------------------------------------------------
// RTrackLog::Close
// --------------------------------------------------------------------------
//
EXPORT_C void RTrackLog::Close()
    {
    LOG( "RTrackLog::Close()" );
    
    if ( iHandle )
    	{
        SendReceive( ELocManCancelTrackLogNotify );
    	}
    
    iObservers.Reset();		// Just reset the array, don't destroy observers in it.
    delete iTrackLogObserverAO;
    iTrackLogObserverAO = NULL;
    iSubscribed = EFalse;
    RLocationManager::Close();
    }

// --------------------------------------------------------------------------
// RTrackLog::SendNotifyRequest
// --------------------------------------------------------------------------
//
void RTrackLog::SendNotifyRequest( TRequestStatus& aStatus )
	{
    SendReceive( ELocManRegObserver, TIpcArgs( &iWrapRec, &iWrapQuality, &iWrapEventType ), 
    		aStatus );
	}

// --------------------------------------------------------------------------
// RTrackLog::GpsQualityChange
// --------------------------------------------------------------------------
//
void RTrackLog::TrackLogEvent( TInt aError )
	{
	TInt count = iObservers.Count();
	for ( TInt index = 0; index < count; index++ )
		{
		switch( iEventType )
			{
			case EStarted:
				{
				iObservers[index]->TrackLogStarted( aError );
				}
				break;
			case EStopped:
				{
				iObservers[index]->TrackLogStopped( aError );
				}
				break;
			case ESignalChanged:
				{
				iObservers[index]->GPSFixQualityChanged( iFixQuality, iRecording );
				}
			}
		}
	}
