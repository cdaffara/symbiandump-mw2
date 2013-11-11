/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "LocationManagerTestScripter.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CLocationManagerTestScripter::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksRequests );
    
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Connect", CLocationManagerTestScripter::ConnectL ),
        ENTRY( "Close", CLocationManagerTestScripter::CloseL ),     
        ENTRY( "TrailConnect", CLocationManagerTestScripter::TrailConnectL ),
        ENTRY( "TrailClose", CLocationManagerTestScripter::TrailCloseL ),    
        ENTRY( "StartTrail", CLocationManagerTestScripter::StartTrailL ),
        ENTRY( "StopTrail", CLocationManagerTestScripter::StopTrailL ),    
        ENTRY( "StartTrailTests", CLocationManagerTestScripter::StartTrailTestsL ),    
        ENTRY( "GetTrailState", CLocationManagerTestScripter::GetTrailStateL ), 
        ENTRY( "LocationTrailNotifyTest", CLocationManagerTestScripter::LocationTrailNotifyTestL ),  
        ENTRY( "RetrieveLocation", CLocationManagerTestScripter::RetrieveLocationL ),  
        ENTRY( "CurrentLocation", CLocationManagerTestScripter::CurrentLocationL ),  
                
        ENTRY( "PrepareSession", CLocationManagerTestScripter::PrepareSessionL ),
        ENTRY( "SetupOM", CLocationManagerTestScripter::SetupOML ),
        ENTRY( "LocationSnapshot", CLocationManagerTestScripter::LocationSnapshotL ),
        ENTRY( "RemoveLocationObject", CLocationManagerTestScripter::RemoveLocationObjectL ),
        ENTRY( "TearDownOM", CLocationManagerTestScripter::TearDownOML ),
        ENTRY( "CloseOM", CLocationManagerTestScripter::CloseOML ),
        ENTRY( "CopyByID", CLocationManagerTestScripter::RLocationObjectManipulatorTest_CopyByIDL ),
        ENTRY( "CopyByURI", CLocationManagerTestScripter::RLocationObjectManipulatorTest_CopyByURIL ),
        ENTRY( "CopyByIDDisco", CLocationManagerTestScripter::RLocationObjectManipulatorTest_CopyByID_DisconnectedL),
        ENTRY( "CopyByURIDisco", CLocationManagerTestScripter::RLocationObjectManipulatorTest_CopyByURI_DisconnectedL),
        
        ENTRY( "SetupTrackLog", CLocationManagerTestScripter::SetupTrackLogL ),
        ENTRY( "TearDownTrackLog", CLocationManagerTestScripter::TearDownTrackLog ),
        ENTRY( "AddObserver", CLocationManagerTestScripter::RTrackLogTest_AddObserverL ),
        ENTRY( "RemoveObserver", CLocationManagerTestScripter::RTrackLogTest_RemoveObserverL ),
        ENTRY( "StartTrackLog", CLocationManagerTestScripter::RTrackLogTest_StartTrackLogL ),
        ENTRY( "Recording", CLocationManagerTestScripter::RTrackLogTest_RecordingL ),
        ENTRY( "StopTrackLog", CLocationManagerTestScripter::RTrackLogTest_StopTrackLogL ),
        ENTRY( "NotRecording", CLocationManagerTestScripter::RTrackLogTest_NotRecordingL ),
        ENTRY( "CrashLocationManager" , CLocationManagerTestScripter::CrashLocationManagerL ),
        ENTRY( "GetFile" , CLocationManagerTestScripter::GetFileL ),
        ENTRY( "DeleteFile" , CLocationManagerTestScripter::DeleteFileL ),
        ENTRY( "GetStatus" , CLocationManagerTestScripter::GetStatusL ),
        ENTRY( "GetStatus2" , CLocationManagerTestScripter::GetStatus2L )

#ifdef LOC_REVERSEGEOCODE
        ,ENTRY( "ConnectGeoTaggerL", CLocationManagerTestScripter::ConnectGeoTaggerL )
        ,ENTRY( "CloseGeoTaggerL", CLocationManagerTestScripter::CloseGeoTaggerL )
        ,ENTRY( "StartGeoTaggingL", CLocationManagerTestScripter::StartGeoTaggingL )
        ,ENTRY( "TestGeoTagCompleteStatusL", CLocationManagerTestScripter::TestGeoTagCompleteStatusL )
        ,ENTRY( "TestGeoTagPendingStatusL", CLocationManagerTestScripter::TestGeoTagPendingStatusL )
        ,ENTRY( "TestGeoTagGoingOnStatusL", CLocationManagerTestScripter::TestGeoTagGoingOnStatusL )
        ,ENTRY( "CancelGeoTaggingL", CLocationManagerTestScripter::CancelGeoTaggingL )
#endif //LOC_REVERSEGEOCODE
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::Connect
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::ConnectL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "ConnectL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iLocationManager.Connect() );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::Close
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::CloseL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "CloseL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iLocationManager.Close();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::Connect
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::TrailConnectL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "ConnectL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iLocationTrail.Connect() );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::Close
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::TrailCloseL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "CloseL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iLocationTrail.Close();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::StartTrailL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::StartTrailL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "StopTrailL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TInt error = iLocationTrail.StartLocationTrail( RLocationTrail::ECaptureAll );
    User::LeaveIfError( error );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::StopTrailL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::StopTrailL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "StopTrailL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iLocationTrail.StopLocationTrail() );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::StartTrailTestsL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::StartTrailTestsL( CStifItemParser& /* aItem */ ) 
    {
    _LIT( KMsg1, "StartTrailTestsL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TInt error( KErrNone );
    
    error = iLocationTrail.StartLocationTrail( RLocationTrail::EOff );
    if( error != KErrArgument )
        {
        User::LeaveIfError( error );
        }
    RLocationTrail::TTrailCaptureSetting setting;
    User::LeaveIfError( iLocationTrail.GetTrailCaptureSetting( setting ) );
    if( setting != RLocationTrail::EOff )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg2, "StartTrailTestsL, test 1 - Error code : %d" );
    TBuf <100> msg;
    msg.Format( KMsg2, error );
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    User::LeaveIfError( iLocationTrail.StopLocationTrail() );
    
    error = iLocationTrail.StartLocationTrail( RLocationTrail::ECaptureNetworkInfo );
    User::LeaveIfError( error );
    User::LeaveIfError( iLocationTrail.GetTrailCaptureSetting( setting ) );
    if( setting != RLocationTrail::ECaptureNetworkInfo )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg3, "StartTrailTestsL, test 2 - Error code : %d" );
    msg.Format( KMsg3, error );
    iLog->Log( msg );     
    RDebug::Print( msg ); 
    
    User::LeaveIfError( iLocationTrail.StopLocationTrail() );
    
    error = iLocationTrail.StartLocationTrail( RLocationTrail::ECaptureAll );
    User::LeaveIfError( error );
    User::LeaveIfError( iLocationTrail.GetTrailCaptureSetting( setting ) );
    if( setting != RLocationTrail::ECaptureAll )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg4, "StartTrailTestsL, test 2 - Error code : %d" );
    msg.Format( KMsg4, error );
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    User::LeaveIfError( iLocationTrail.StopLocationTrail() );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::GetTrailStateL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::GetTrailStateL( CStifItemParser& /*aItem*/ ) 
    {
    _LIT( KMsg1, "GetTrailStateL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    RLocationTrail::TTrailState state;
    iLocationTrail.GetLocationTrailState( state );
    if( state != RLocationTrail::ETrailStarting )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg, "GetTrailStateL - state : %d" );
    TBuf <100> msg;
    msg.Format( KMsg, state );
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::LocationTrailNotifyTestL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::LocationTrailNotifyTestL( CStifItemParser& /*aItem*/ ) 
    {
    _LIT( KMsg1, "LocationTrailNotifyTestL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    TRequestStatus status;
    iLocationTrail.NotifyLocationTrailStateChange( status );
    iLocationTrail.CancelNotificationRequest();
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::RetrieveLocationL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::RetrieveLocationL( CStifItemParser& /*aItem*/ ) 
    {
    _LIT( KMsg1, "RetrieveLocationL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );

    TTime time;
    time.UniversalTime();
    
    TLocationData data;
    RLocationTrail::TTrailState state;
    
    TInt error = iLocationTrail.RetrieveLocation( time, data, state );
    
    _LIT( KMsg, "RetrieveLocationL - error : %d" );
    TBuf <100> msg;
    msg.Format( KMsg, error );
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CLocationManagerTestScripter::CurrentLocationL
// -----------------------------------------------------------------------------
//
TInt CLocationManagerTestScripter::CurrentLocationL( CStifItemParser& /*aItem*/ ) 
    {
    _LIT( KMsg1, "CurrentLocationL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TRequestStatus status; 
    TLocationData data;
    iLocationTrail.CurrentLocation( status, data );
    iLocationTrail.CancelLocationRequest();
    
    return KErrNone;
    }

//  [End of File] - Do not remove
