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
* Description:  An interface to Location Manager server
*
*/

#include <f32file.h>
#include <s32mem.h>
#include <data_caging_path_literals.hrh>

#include <locationeventdef.h>
#include "rlocationtrail.h"
#include "locationmanagerdefs.h"
#include "locationmanagerdebug.h"

EXPORT_C RLocationTrail::RLocationTrail() : iCurrentLocData( NULL )
	{
	}

EXPORT_C RLocationTrail::~RLocationTrail()
	{
	delete iCurrentLocData;
	iCurrentLocData = NULL;
	}

// --------------------------------------------------------------------------
// RLocationTrail::StartLocationTrail
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationTrail::StartLocationTrail(TTrailCaptureSetting aState)
    {
    LOG( "RLocationTrail::StartLocationTrail()" );
    TInt ret( KErrDisconnected );
    TPckg<TTrailCaptureSetting> state( aState );
    
    if ( aState == RLocationTrail::EOff )
    	{
    	return KErrArgument;
    	}
    
    if ( iHandle )
        {
        ret = SendReceive( ELocManStartTrail, TIpcArgs( &state ) );
        }
    return ret;
    }
// --------------------------------------------------------------------------
// RLocationTrail::StopLocationTrail
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationTrail::StopLocationTrail()
    {
    LOG( "RLocationTrail::StopLocationTrail()" );
    TInt ret( KErrNone );
    if ( iHandle )
        {
        Send( ELocManStopTrail );    
        }
    else
        {
        ret = KErrDisconnected;
        }
    
    return ret;
    }
// --------------------------------------------------------------------------
// RLocationTrail::GetLocationTrailState
// --------------------------------------------------------------------------
//
EXPORT_C TInt RLocationTrail::GetLocationTrailState( TTrailState& aState )
    {
    LOG( "RLocationTrail::GetLocationTrailState()" );
    TInt ret( KErrDisconnected );
    TPckg<TTrailState> state( aState );
    
    if ( iHandle )
        {
        ret = SendReceive( ELocManTrailState, TIpcArgs( &state ) );
        }
    return ret;
    }
// --------------------------------------------------------------------------
// RLocationTrail::NotifyLocationTrailStateChange
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationTrail::NotifyLocationTrailStateChange( TRequestStatus& aStatus )
    {
    LOG( "RLocationTrail::NotifyLocationTrailStateChange()" );
    if ( iHandle )
        {
        SendReceive( ELocManNotifyTrailStateChange, aStatus );
        }        
    else
        {
        CompleteRequest(aStatus, KErrDisconnected);
        }
    }

// --------------------------------------------------------------------------
// RLocationTrail::CancelNotificationRequest
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationTrail::CancelNotificationRequest()
    {
    LOG( "RLocationTrail::CancelNotificationRequest()" );
    if ( iHandle )
    	{
        SendReceive( ELocManCancelNotification );
    	}
    }    

// --------------------------------------------------------------------------
// RLocationTrail::RetrieveLocation
// --------------------------------------------------------------------------
//    
EXPORT_C TInt RLocationTrail::RetrieveLocation( const TTime& aTimeStamp, 
												TLocationData& aLocationData,
                                                TTrailState& aState )
    {
    TInt ret( KErrDisconnected );
    TPckg<TTime> timeStamp( aTimeStamp );
    TPckg<TTrailState> state( aState );
    TPckg<TLocationData> locationData( aLocationData );
    if ( iHandle )
        {
        ret = SendReceive( ELocManRetrieveLoc, TIpcArgs( &timeStamp, &locationData, &state ) );    
        }
    return ret;
    }

// --------------------------------------------------------------------------
// RLocationTrail::CurrentLocation
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationTrail::CurrentLocation( TRequestStatus& aStatus, 
											   TLocationData& aLocationData )
    {
    LOG( "RLocationTrail::CurrentLocation(), begin" );
    
    delete iCurrentLocData;
    iCurrentLocData = NULL;
    iCurrentLocData = new TPckg<TLocationData>( aLocationData );
    
    if ( iHandle && iCurrentLocData )
        {
        SendReceive( ELocManCurrentLoc, TIpcArgs( iCurrentLocData ), aStatus );
        }
    else
        {
        if ( !iCurrentLocData )
        	{
        	CompleteRequest(aStatus, KErrNoMemory);
        	}
        else
        	{
        	CompleteRequest(aStatus, KErrDisconnected);
        	}
        }
    LOG( "RLocationTrail::CurrentLocation(), end" );
    }

// --------------------------------------------------------------------------
// RLocationTrail::CancelLocationRequest
// --------------------------------------------------------------------------
//
EXPORT_C void RLocationTrail::CancelLocationRequest()
    {
    if ( iHandle )
        {
        SendReceive( ELocManCancelCurrentLoc );
        }
    }

EXPORT_C TInt RLocationTrail::GetTrailCaptureSetting( TTrailCaptureSetting& aCaptureSetting )
	{
	TPckg<TTrailCaptureSetting> captureSetting( aCaptureSetting );
	TInt err( KErrDisconnected );
	
	if ( iHandle )
		{
		err = SendReceive( ELocManGetCaptureSetting, TIpcArgs( &captureSetting ) );
		}
	
	return err;
	}

//End of File
