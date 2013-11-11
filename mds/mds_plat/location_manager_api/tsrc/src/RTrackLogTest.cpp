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

#include "LocationManagerTestScripter.h"
#include "mdsutils.h"
#include <etel3rdparty.h>
#include <StifTestEventInterface.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include <e32cmn.h>

TInt CLocationManagerTestScripter::SetupTrackLogL( CStifItemParser& /*aItem*/ )
	{
    _LIT( KMsg1, "SetupTrackLog" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	User::LeaveIfError( iTrackLog.Connect() );
	
	return KErrNone;
	}

TInt CLocationManagerTestScripter::TearDownTrackLog( CStifItemParser& /*aItem*/ )
	{
    _LIT( KMsg1, "TearDownTrackLog" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	if ( iTrackLog.IsRecording() )
		{
		iTrackLog.StopTrackLog();
		}
	
	iTrackLog.Close();
	
	return KErrNone;
	}

TInt CLocationManagerTestScripter::RTrackLogTest_AddObserverL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "RTrackLogTest_AddObserver" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
   
    TInt error( KErrNone );
    error = iTrackLog.AddObserver( this );
    if( error != KErrNone && error != KErrAlreadyExists )
        {
        return error;
        }
    
    return KErrNone;
    }

TInt CLocationManagerTestScripter::RTrackLogTest_RemoveObserverL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "RTrackLogTest_RemoveObserver" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TInt error( KErrNone );
    error = iTrackLog.RemoveObserver( this );
    if( error != KErrNone && error != KErrNotFound )
        {
        return error;
        }
    
    return KErrNone;
    }

void CLocationManagerTestScripter::TrackLogStarted(TInt aError)
    {
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Recording") );
    TestModuleIf().Event( event );
    }

void CLocationManagerTestScripter::TrackLogStopped(TInt aError)
    {
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("StopRecording") );
    TestModuleIf().Event( event );
    }

void CLocationManagerTestScripter::GPSFixQualityChanged(TPositionSatelliteInfo& /*aFixQuality*/, 
                                                        TBool& /*aTrackingActive*/ )
    {
    _LIT( KMsg1, "CallBck GPSFixQualityChanged" );
    iLog->Log( KMsg1 );     
    RDebug::Print( KMsg1 );
    }

TInt CLocationManagerTestScripter::RTrackLogTest_StartTrackLogL( CStifItemParser& /*aItem*/ )
	{
    _LIT( KMsg1, "RTrackLogTest_StartTrackLogL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
    User::LeaveIfError( iTrackLog.StartTrackLog() );
	
	return KErrNone;
	}

TInt CLocationManagerTestScripter::RTrackLogTest_RecordingL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "RTrackLogTest_RecordingL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if ( !iTrackLog.IsRecording() )
        {
        return KErrUnknown;
        }
    
    return KErrNone;
    }

TInt CLocationManagerTestScripter::RTrackLogTest_StopTrackLogL( CStifItemParser& /*aItem*/ )
	{
    _LIT( KMsg1, "RTrackLogTest_StopTrackLogL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	iTrackLog.StopTrackLog();
	
	return KErrNone;
	}

TInt CLocationManagerTestScripter::RTrackLogTest_NotRecordingL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "RTrackLogTest_NotRecordingL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if ( iTrackLog.IsRecording() )
        {
        return KErrUnknown;
        }
    
    return KErrNone;
    }

TInt CLocationManagerTestScripter::CrashLocationManagerL( CStifItemParser& /*aItem*/ )
	{
	_LIT( name, "LocationManagerServ*" );
	TFindProcess findProcess( name );
	
	RProcess locManProcess;
	TFullName result;
	findProcess.Next( result );
	User::LeaveIfError( locManProcess.Open( findProcess ) );
	locManProcess.Kill( KErrNone );
	locManProcess.Close();

	return KErrNone;
	}

TInt CLocationManagerTestScripter::GetFileL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "GetFileL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    TFileName name;
    User::LeaveIfError( iTrackLog.TrackLogName( name ) );
    if( name.Length() <= 0 )
        {
        return KErrNotFound;
        }
    
    return KErrNone;
    }

TInt CLocationManagerTestScripter::DeleteFileL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "DeleteFileL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TFileName name;
    TInt error( KErrNone );
    error = iTrackLog.TrackLogName( name );
    if( error != KErrNotFound )
        {
        return KErrUnknown;
        }
    error = iTrackLog.DeleteTrackLog( name );
    if( error != KErrNone )
        {
        return error;
        }

    return KErrNone;
    }

TInt CLocationManagerTestScripter::GetStatusL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "GetStatusL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    TPositionSatelliteInfo info;
    TBool recording;
    User::LeaveIfError( iTrackLog.GetStatus( recording, info ) );
    
    return KErrNone;
    }

TInt CLocationManagerTestScripter::GetStatus2L( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "GetStatusL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    TPositionSatelliteInfo info;
    TBool recording;
    User::LeaveIfError( iTrackLog.GetStatus( recording, info ) );
    if( recording )
        {
        return KErrUnknown;
        }
    
    return KErrNone;
    }

// End of file
