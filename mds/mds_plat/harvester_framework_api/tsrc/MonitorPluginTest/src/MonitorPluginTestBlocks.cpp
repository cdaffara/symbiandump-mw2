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
* Description: 
*
*/


// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <f32file.h>
#include <bautils.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include "MonitorPluginTest.h"

#include "mdsutils.h"


_LIT( KFile1Src, "C:\\Data\\Jpg.jpg" );
_LIT( KFile1Dst, "E:\\Images\\Jpg_copy.jpg" );
_LIT( KFile1Dst2, "C:\\Data\\Images\\Pictures\\Jpg_copy2.jpg" );
_LIT( KFile1Dst3, "C:\\Data\\Images\\Pictures\\Jpg_tmp.jpg" );

_LIT( KFile2Src, "C:\\Data\\Animated.gif" );
_LIT( KFile2Dst, "E:\\Images\\Animated_copy.gif" );
_LIT( KFile2Dst2, "C:\\Data\\Images\\Pictures\\Animated_copy2.gif" );
_LIT( KFile2Dst3, "C:\\Data\\Images\\Pictures\\Animated_tmp.gif" );

_LIT( KFile3Src, "C:\\Data\\H263.3gp" );
_LIT( KFile3Dst, "E:\\Videos\\H263_copy.3gp" );
_LIT( KFile3Dst2, "C:\\Data\\H263_copy2.3gp" );
_LIT( KFile3Dst3, "C:\\Data\\H263_tmp.3gp" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMonitorPluginTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );
    
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        ENTRY( "ActiveWait", CMonitorPluginTest::ActiveWait ),
        ENTRY( "BeginMonitorSession", CMonitorPluginTest::BeginMonitorSessionL ),
        ENTRY( "EndMonitorSession", CMonitorPluginTest::EndMonitorSession ),
        ENTRY( "LoadPlugins", CMonitorPluginTest::LoadPluginsL ),
        ENTRY( "StartMonitoring", CMonitorPluginTest::StartMonitoring ),
        ENTRY( "PauseMonitoring", CMonitorPluginTest::PauseMonitoring ),
        ENTRY( "ResumeMonitoring", CMonitorPluginTest::ResumeMonitoring ),
        ENTRY( "StopMonitoring", CMonitorPluginTest::StopMonitoring ),
        ENTRY( "CreateEvents", CMonitorPluginTest::CreateEventsL ),
        ENTRY( "CreateEventsDelete", CMonitorPluginTest::CreateEventsDeleteL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::MonitorEvent
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::MonitorEvent( CHarvesterData* aHarvesterData )
    {
    _LIT( KMsg, "CallBck MonitorEvent");
    iLog->Log( KMsg );
    RDebug::Print( KMsg );

    switch ( aHarvesterData->EventType() )
        {
        case EHarvesterAdd:
            {
            _LIT( KAdded, "File added" );
            iLog->Log( KAdded );
            RDebug::Print( KAdded );
            }
        break;
        
        case EHarvesterEdit:
            {
            _LIT( KEdited, "File edited" );
            iLog->Log( KEdited );
            RDebug::Print( KEdited );
            }
        break;
        
        case EHarvesterDelete:
            {
            _LIT( KDeleted, "File deleted" );
            iLog->Log( KDeleted );
            RDebug::Print( KDeleted );
            }
        break;
        
        case EHarvesterUnknown:
            {
            _LIT( KUnknown, "Unknown event" );
            iLog->Log( KUnknown );
            RDebug::Print( KUnknown );
            }
        break;
        }
    
    // monitor event
    TEventIf event( TEventIf::ESetEvent, _L("Monitor") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::MonitorEvent
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::MonitorEvent( RPointerArray<CHarvesterData>& aHDArray )
    {
    _LIT( KMsg, "CallBck MonitorEvent");
    iLog->Log( KMsg );
    RDebug::Print( KMsg );
    
    for( TInt i = 0; i < aHDArray.Count(); i++ )
    	{
    	CHarvesterData* hd = aHDArray[i];
	    switch ( hd->EventType() )
	        {
	        case EHarvesterAdd:
	            {
	            _LIT( KAdded, " Files added." );
	            iLog->Log( KAdded );
	            RDebug::Print( KAdded );
	            }
	        break;
	        
	        case EHarvesterEdit:
	            {
	            _LIT( KEdited, " Files edited." );
	            iLog->Log( KEdited );
	            RDebug::Print( KEdited );
	            }
	        break;
	        
	        case EHarvesterDelete:
	            {
	            _LIT( KDeleted, " Files deleted." );
	            iLog->Log( KDeleted );
	            RDebug::Print( KDeleted );
	            }
	        break;
	        
	        case EHarvesterUnknown:
	            {
	            _LIT( KUnknown, "Unknown event." );
	            iLog->Log( KUnknown );
	            RDebug::Print( KUnknown );
	            }
	        break;
	        }
        }
    
    // monitor event
    TEventIf event( TEventIf::ESetEvent, _L("Monitor") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::HandleSessionOpened( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBack HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::HandleSessionError( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionError - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::ActiveWait
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::ActiveWait( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter ActiveWait" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    // 2,5 seconds
    TTimeIntervalMicroSeconds32 timeout( 2500000 );
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After(status,timeout);

    User::WaitForAnyRequest();
    
    timer.Close();
    
    _LIT( KMsg2, "Exit ActiveWait" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::ActiveWait
// -----------------------------------------------------------------------------
//
void CMonitorPluginTest::ActiveWait2()
    {
    _LIT( KMsg1, "Enter ActiveWait2" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    // 2,5 seconds
    TTimeIntervalMicroSeconds32 timeout( 2500000 );
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After(status,timeout);

    User::WaitForRequest( status);
    
    timer.Close();
    
    _LIT( KMsg2, "Exit ActiveWait2" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::BeginMonitorSessionL
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::BeginMonitorSessionL( CStifItemParser& /* aItem */ )
    {
    iMdEClient = NULL;
    TRAPD( error, iMdEClient = CMdESession::NewL( *this ) );

    _LIT( KMsg, "Exit BeginMonitorSession - Error code : %d" );
    TBuf <100> msg;
    msg.Format( KMsg, error);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    User::LeaveIfError( error );
    
    return KErrNone;
    }  

// -----------------------------------------------------------------------------
// CMonitorPluginTest::EndMonitorSession
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::EndMonitorSession( CStifItemParser& /* aItem */ )
	{
    _LIT( KMsg1, "Enter EndMonitorSession" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
	iPluginArray.ResetAndDestroy();
	iPluginArray.Close();
	
	if ( iMdEClient )
		{
	    delete iMdEClient;
	    iMdEClient = NULL;
	    }
	
	REComSession::FinalClose();
	
    _LIT( KMsg2, "Exit EndMonitorSession" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::LoadPluginsL
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::LoadPluginsL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter LoadPlugins" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    RImplInfoPtrArray infoArray;
    
    TCleanupItem cleanupItem(MdsUtils::CleanupEComArray, &infoArray);
    CleanupStack::PushL(cleanupItem);
    
    CMonitorPlugin::ListImplementationsL(infoArray);
    TInt count( 0 );
    count = infoArray.Count();
    
    for (TInt i=0; i < count; i++)
        {
        TUid uid = infoArray[i]->ImplementationUid();    // Create the plug-ins
        iPluginArray.AppendL( CMonitorPlugin::NewL( uid ) ); // and add them to array
        }
        
    CleanupStack::PopAndDestroy(&infoArray); // infoArray, results in a call to CleanupEComArray
    
    if( iPluginArray.Count() == 0 )
        {
        return KErrNotFound;
        }
    
    _LIT( KMsg2, "Exit LoadPlugins" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::StartMonitoring
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::StartMonitoring( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter StartMonitoring" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt count( iPluginArray.Count() );
    
    for (TInt i=0; i < count; i++)
        {
        TBool ok = iPluginArray[i]->StartMonitoring( *this, iMdEClient, NULL, NULL );
        if( ok )
            {
            _LIT( KMsg2, "Starting monitoring ok" );
            iLog->Log( KMsg2 );
            RDebug::Print( KMsg2 );            
            }
        else
            {
            _LIT( KMsg3, "Starting monitoring failed" );
            iLog->Log( KMsg3 );
            RDebug::Print( KMsg3 );               
            }
        }
    
    _LIT( KMsg3, "Exit StartMonitoring" );
    iLog->Log( KMsg3 );
    RDebug::Print( KMsg3 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::PauseMonitoring
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::PauseMonitoring( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter PauseMonitoring" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt count( iPluginArray.Count() );
    
    for (TInt i = 0; i < count; i++ )
        {
        iPluginArray[i]->PauseMonitoring();
        }
    
    _LIT( KMsg2, "Exit PauseMonitoring" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::ResumeMonitoring
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::ResumeMonitoring( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter ResumeMonitoring" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt count( iPluginArray.Count() );
    
    for (TInt i=0; i < count; i++)
        {
        iPluginArray[i]->ResumeMonitoring( *this, iMdEClient, NULL, NULL );
        }
    
    _LIT( KMsg2, "Exit ResumeMonitoring" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::StopMonitoring
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::StopMonitoring( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter StopMonitoring" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt count( iPluginArray.Count() );
    
    for (TInt i=0; i < count; i++)
        {
        iPluginArray[i]->StopMonitoring();
        }
    
    ActiveWait2();
    
    _LIT( KMsg2, "Exit StopMonitoring" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::CreateEvents
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::CreateEventsL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter CreateEvents" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    RFs fs;
    User::LeaveIfError( fs.Connect() );   
    
    BaflUtils::CopyFile( fs, KFile1Src, KFile1Dst2 );
    BaflUtils::CopyFile( fs, KFile2Src, KFile2Dst2 );
    BaflUtils::CopyFile( fs, KFile3Src, KFile3Dst2 ); 
    
    fs.Rename( KFile1Dst2, KFile1Dst3 );
    fs.Rename( KFile2Dst2, KFile2Dst3 );
    fs.Rename( KFile3Dst2, KFile3Dst3 );
    
    fs.Replace( KFile1Dst3, KFile1Dst2 );
    fs.Replace( KFile2Dst3, KFile2Dst2 );
    fs.Replace( KFile3Dst3, KFile3Dst2 );
    
    // MMC stuff
    if( fs.IsValidDrive( EDriveE ) )
    	{
        fs.RemountDrive( EDriveE, NULL ,80000000 );
        
        iMdEClient->RemoveObjectL( KFile1Dst );
        iMdEClient->RemoveObjectL( KFile2Dst );
        iMdEClient->RemoveObjectL( KFile3Dst );
        BaflUtils::DeleteFile( fs, KFile1Dst );
        BaflUtils::DeleteFile( fs, KFile2Dst );
        BaflUtils::DeleteFile( fs, KFile3Dst );
        
        BaflUtils::CopyFile( fs, KFile1Src, KFile1Dst );
        BaflUtils::CopyFile( fs, KFile2Src, KFile2Dst );
        BaflUtils::CopyFile( fs, KFile3Src, KFile3Dst );
    	}
    
    fs.Close();
    
    _LIT( KMsg2, "Exit CreateEvents" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMonitorPluginTest::CreateEventsDelete
// -----------------------------------------------------------------------------
//
TInt CMonitorPluginTest::CreateEventsDeleteL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter CreateEventsDelete" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    RFs fs;
    User::LeaveIfError(fs.Connect());
    
    iMdEClient->RemoveObjectL( KFile1Src );
    iMdEClient->RemoveObjectL( KFile2Src );
    iMdEClient->RemoveObjectL( KFile3Src );
    iMdEClient->RemoveObjectL( KFile1Dst2 );
    iMdEClient->RemoveObjectL( KFile2Dst2 );
    iMdEClient->RemoveObjectL( KFile3Dst2 );
    
    BaflUtils::DeleteFile( fs, KFile1Dst2 );
    BaflUtils::DeleteFile( fs, KFile2Dst2 );
    BaflUtils::DeleteFile( fs, KFile3Dst2 );
    BaflUtils::DeleteFile( fs, KFile1Dst3 );
    BaflUtils::DeleteFile( fs, KFile2Dst3 );
    BaflUtils::DeleteFile( fs, KFile3Dst3 );
    
    fs.Close();
    
    _LIT( KMsg2, "Exit CreateEventsDelete" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

//  [End of File] - Do not remove
