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

// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include "HarvesterClientTestScripter.h"
#include "mdcserializationbuffer.h"
#include "mdeharvestersession.h"
#include "mdsutils.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHarvesterClientTestScripter::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::Delete() 
    {
    
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTestScripter::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RunMethodL( 
    CStifItemParser& aItem ) 
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

		ENTRY( "SetUp", CHarvesterClientTestScripter::SetUp ),
		ENTRY( "TearDown", CHarvesterClientTestScripter::TearDown ),

        // Test helper methods
        ENTRY( "AddObjectObserver", CHarvesterClientTestScripter::AddObjectObserverL ),
        ENTRY( "RemoveObjectObserver", CHarvesterClientTestScripter::RemoveObjectObserverL ),
        ENTRY( "SetUpHarvesting", CHarvesterClientTestScripter::BeginHarvesterSessionL ),
        ENTRY( "EndHarvesterSession", CHarvesterClientTestScripter::EndHarvesterSessionL ),

        // Harvester tests
        ENTRY( "HarvesterClientConnect", CHarvesterClientTestScripter::HarvesterClientConnectL ),
        ENTRY( "HarvesterClientClose", CHarvesterClientTestScripter::HarvesterClientCloseL ),
        ENTRY( "HarvesterClientPause", CHarvesterClientTestScripter::HarvesterClientPauseL ),
        ENTRY( "HarvesterClientResume", CHarvesterClientTestScripter::HarvesterClientResumeL ),
        ENTRY( "SetHarvesterObserver", CHarvesterClientTestScripter::SetHarvesterObserverL ),
        ENTRY( "RemoveHarvesterObserver", CHarvesterClientTestScripter::RemoveHarvesterObserverL ),
        ENTRY( "SetLocationInfo", CHarvesterClientTestScripter::SetLocationInfoL ),
        ENTRY( "HarvestFile", CHarvesterClientTestScripter::HarvestFileL ),
        ENTRY( "HarvestFileWithUID", CHarvesterClientTestScripter::HarvestFileWithUIDL ),
        ENTRY( "QueryImageObjectByIdIndex", CHarvesterClientTestScripter::QueryImageObjectByIdIndexL ),
        ENTRY( "QueryAudioObjectByIdIndex", CHarvesterClientTestScripter::QueryAudioObjectByIdIndexL ),
        ENTRY( "RemoveObject", CHarvesterClientTestScripter::RemoveObjectL ),
        ENTRY( "Results", CHarvesterClientTestScripter::Results ),     
        ENTRY( "AddHarvesterEventObserver", CHarvesterClientTestScripter::AddHarvesterEventObserverL ),
        ENTRY( "RemoveHarvesterEventObserver", CHarvesterClientTestScripter::RemoveHarvesterEventObserverL ),
        ENTRY( "SetUpBlacklist", CHarvesterClientTestScripter::SetUpBlacklistL ),
        ENTRY( "TearDownBlacklist", CHarvesterClientTestScripter::TearDownBlacklistL ),
        ENTRY( "AddFileToBlacklist", CHarvesterClientTestScripter::AddFileToBlacklistL ),
        ENTRY( "CheckBlacklist", CHarvesterClientTestScripter::CheckBlacklistL ),
        ENTRY( "RemoveFileFromBlacklist", CHarvesterClientTestScripter::RemoveFileFromBlacklistL ),
        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::SetUp
// Set up test.
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::SetUp( CStifItemParser& /* aItem */ )
	{
    _LIT( KMsg1, "SetUp" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	iHarvestingComplete = EFalse;
	iPlaceholderComplete = EFalse;
	iNewObjectComplete = EFalse;
	iObjectNotification = EFalse;
	iSetLocation = EFalse;
	iObjectIdArray.Reset();
	TRAP_IGNORE( iMdeSession = CMdESession::NewL( *this ) );

    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::TearDown
// Tear down test.
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::TearDown( CStifItemParser& /* aItem */ )
	{
    _LIT( KMsg1, "TearDown" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	delete iHarvesterSession;
	iHarvesterSession = NULL;
	delete iMdeSession;
	iMdeSession = NULL;

    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvesterClientConnectL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvesterClientConnectL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "HarvesterClientConnectL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iHc.Connect() );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvesterClientCloseL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvesterClientCloseL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "HarvesterClientCloseL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iHc.Close();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvesterClientPauseL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvesterClientPauseL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter HarvesterClientPauseL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iHc.Pause() );

    _LIT( KMsg2, "Exit HarvesterClientPauseL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvesterClientResumeL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvesterClientResumeL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter HarvesterClientResumeL" );
     iLog->Log( KMsg1 );  
     RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iHc.Resume() );

    _LIT( KMsg2, "Exit HarvesterClientResumeL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::SetHarvesterObserverL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::SetHarvesterObserverL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "SetHarvesterObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iHc.SetObserver( this );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::RemoveHarvesterObserverL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RemoveHarvesterObserverL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "RemoveHarvesterObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iHc.RemoveObserver( this );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::AddObjectObserverL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::AddObjectObserverL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "AddObjectObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iMdeSession->AddObjectObserverL( *this );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::AddObjectObserverL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RemoveObjectObserverL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "RemoveObjectObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TRAP_IGNORE( iMdeSession->RemoveObjectObserverL( *this ) );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::AddObjectObserverL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::SetLocationInfoL( CStifItemParser& /* aItem */ )
    {
    iSetLocation = ETrue;

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::StartHarvesterSessionL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::BeginHarvesterSessionL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter BeginHarvesterSessionL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));
    
    TRAP_IGNORE( iMdeSession->RemoveObjectL(inputFile) );
    
    _LIT( KMsg2, "Exit BeginHarvesterSessionL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvestFileTestL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvestFileL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter HarvestFileL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
  
	TPtrC inputFile;
	User::LeaveIfError( aItem.GetNextString( inputFile ));
    iAlbumIds.Append( 1 );
    iQuerySuccess = EFalse;
    
    iUri = inputFile;
    iHc.HarvestFile( inputFile, iAlbumIds, iSetLocation );

    _LIT( KMsg2, "Exit HarvestFileL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvestFileTestL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::HarvestFileWithUIDL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter HarvestFileWithUIDL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TUid uid( KNullUid );
    
    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));
    iAlbumIds.Append( 1 );
    iQuerySuccess = EFalse;
    
    iUri = inputFile;
    iHc.HarvestFileWithUID( inputFile, iAlbumIds, EFalse, uid );

    _LIT( KMsg2, "Exit HarvestFileWithUIDL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvestingCompleteL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::EndHarvesterSessionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "EndHarvesterSessionL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iAlbumIds.Close();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::QueryObjectByIdIndexL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::QueryImageObjectByIdIndexL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "QueryImageObjectByIdIndexL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt index;
    User::LeaveIfError( aItem.GetNextInt( index ));

    CMdENamespaceDef& defNS = iMdeSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objDef = defNS.GetObjectDefL( MdeConstants::Image::KImageObject );
    iQuery = iMdeSession->NewObjectQueryL( defNS, objDef, this );
            
    iQuery->SetResultMode( EQueryResultModeItem );
            
    iQuery->FindL();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::QueryObjectByIdIndexL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::QueryAudioObjectByIdIndexL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "QueryAudioObjectByIdIndexL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt index;
    User::LeaveIfError( aItem.GetNextInt( index ));

    CMdENamespaceDef& defNS = iMdeSession->GetDefaultNamespaceDefL();
    CMdEObjectDef& objDef = defNS.GetObjectDefL( MdeConstants::Audio::KAudioObject );
    iQuery = iMdeSession->NewObjectQueryL( defNS, objDef, this );
            
    iQuery->SetResultMode( EQueryResultModeItem );
            
    iQuery->FindL();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::RemoveObjectL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RemoveObjectL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "RemoveObjectL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));

    TRAP_IGNORE( iMdeSession->RemoveObjectL( inputFile ) );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::Results
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::Results( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter Results" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    if( !iQuerySuccess )
        {
        return KErrNotFound;
        }
    
    if( !iHarvestingComplete )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg2, "Enter Results" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::AddHarvesterEventObserver
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::AddHarvesterEventObserverL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "AddHarvesterEventObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt testCount( 0 );
    User::LeaveIfError( aItem.GetNextInt( testCount ) );
    for( TInt i = 0; i < testCount; i++ )
    	{
    	TInt err = iHc.AddHarvesterEventObserver( *this, EHEObserverTypePlaceholder, 3 );
	    TL( err == KErrNone );
    	}
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::RemoveHarvesterEventObserver
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RemoveHarvesterEventObserverL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "RemoveHarvesterEventObserverL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TInt testCount( 0 );
    User::LeaveIfError( aItem.GetNextInt( testCount ) );
    
    for( TInt i = 0; i < testCount; i++ )
		{
		TInt err = iHc.RemoveHarvesterEventObserver( *this );
	    TL( err == KErrNone );
		}
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::SetUpBlacklistL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::SetUpBlacklistL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter SetUpBlacklistL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    User::LeaveIfError( iBlacklistClient.Connect() );
    iBlacklistClient.LoadBlacklistL();
    iMediaIdUtil = &RMediaIdUtil::GetInstanceL();
    User::LeaveIfError( iFs.Connect() );
    
    _LIT( KMsg2, "Exit SetUpBlacklistL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::TearDownBlacklistL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::TearDownBlacklistL( CStifItemParser& /*aItem*/ )
    {
    _LIT( KMsg1, "Enter TearDownBlacklistL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
   
    iBlacklistClient.CloseDBL();
    iBlacklistClient.Close();
    RMediaIdUtil::ReleaseInstance();
    iMediaIdUtil = NULL;
    iFs.Close();
    
    _LIT( KMsg2, "Exit TearDownBlacklistL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::AddFileToBlacklistL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::AddFileToBlacklistL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter AddFileToBlacklistL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));

    TUint32 mediaId( 0 );
    TTime modified ( 0 );

    TRAPD( err, iBlacklistClient.AddL( inputFile, mediaId, modified ) );
    if( err != KErrNone )
        {
        return err;
        }

    _LIT( KMsg2, "Exit AddFileToBlacklistL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::CheckBlacklistL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::CheckBlacklistL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter CheckBlacklistL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );

    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));
    
    TUint32 mediaId( 0 );
    TTime modified ( 0 );

    TBool isBlacklisted( ETrue );
    _LIT( KMsg2, "Check blacklist prepare state" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    TRAP_IGNORE( isBlacklisted = iBlacklistClient.IsBlacklistedL( inputFile, mediaId, modified ) );
    if( isBlacklisted )
        {
        return KErrBadName;
        }

    _LIT( KMsg3, "Close blacklist to commit the changes" );
    iLog->Log( KMsg3 );
    RDebug::Print( KMsg3 );
    iBlacklistClient.CloseDBL();
    iBlacklistClient.Close();
    
    _LIT( KMsg4, "Re-open blacklist to check changes" );
    iLog->Log( KMsg4 );
    RDebug::Print( KMsg4 );   
    User::LeaveIfError( iBlacklistClient.Connect() );
    iBlacklistClient.LoadBlacklistL();
  
    _LIT( KMsg5, "Check changes" );
    iLog->Log( KMsg5 );
    RDebug::Print( KMsg5 );
    
    isBlacklisted = EFalse;
    TRAP_IGNORE( isBlacklisted = iBlacklistClient.IsBlacklistedL( inputFile, mediaId, modified ) );
    if( !isBlacklisted )
        {
        return KErrNotFound;
        }
    
    _LIT( KMsg6, "Exit CheckBlacklistL" );
    iLog->Log( KMsg6 );
    RDebug::Print( KMsg6 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::RemoveFileFromBlacklistL
// -----------------------------------------------------------------------------
//
TInt CHarvesterClientTestScripter::RemoveFileFromBlacklistL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter CheckBlacklistL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    TPtrC inputFile;
    User::LeaveIfError( aItem.GetNextString( inputFile ));
    
    TUint32 mediaId( 0 );
    TTime modified ( 0 );

    TRAPD( err, iBlacklistClient.RemoveL( inputFile, mediaId ) );
    if( err != KErrNone )
        {
        return err;
        }

    _LIT( KMsg2, "Exit CheckBlacklistL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvestingUpdated
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HarvestingUpdated( 
	HarvesterEventObserverType /*aHEObserverType*/, 
	HarvesterEventState /*aHarvesterEventState*/,
	TInt /*aItemsLeft*/ )
	{
    _LIT( KMsg1, "CallBck HarvestingUpdated" );
    iLog->Log( KMsg1 );     
    RDebug::Print( KMsg1 );
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HarvestingComplete
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HarvestingComplete( TDesC& aURI, TInt aError )
	{
    _LIT( KMsg, "CallBck HarvestingComplete - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    if( MdsUtils::Compare( aURI, iUri ) == 0 )
        {
        if( iObjectNotification )
            {
            iHarvestingComplete = ETrue;
            // session event
            TEventIf event( TEventIf::ESetEvent, _L("Continue") );
            TestModuleIf().Event( event );
            }
        else
            {
            iHarvestingComplete = ETrue;
            }
        }
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HandleSessionOpened( CMdESession& /* aSession */, TInt aError )
	{
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print( msg );
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HandleSessionError( CMdESession& /* aSession */, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionError - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HandleObjectNotification
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HandleObjectNotification(
		CMdESession& /* aSession */, 
		TObserverNotificationType aType,
		const RArray<TItemId>& aObjectIdArray )
	{
    _LIT( KMsg1, "CallBck HandleObjectNotification" );
    iLog->Log( KMsg1 );     
    RDebug::Print( KMsg1 );

	switch( aType )
		{
		case ENotifyAdd:
			{
			iPlaceholderComplete = ETrue;
			break;
			}
		case ENotifyModify:
			{
			iNewObjectComplete = ETrue;
			break;
			}
		case ENotifyRemove:
			{
			break;
			}		
		}
	
	CMdEObject* temp = NULL;
	TRAP_IGNORE( temp = iMdeSession->GetObjectL( iUri ) );
	if( temp && iHarvestingComplete )
	    {
	    for ( TInt i = 0; i < aObjectIdArray.Count(); ++i )
	        {
	        iObjectIdArray.Append( aObjectIdArray[i] );
	        }
	    // session event
	    TEventIf event( TEventIf::ESetEvent, _L("Continue") );
	    TestModuleIf().Event( event );
	    iObjectNotification = ETrue;
	    }
	else if( temp )
	    {
	    for ( TInt i = 0; i < aObjectIdArray.Count(); ++i )
	        {
	        iObjectIdArray.Append( aObjectIdArray[i] );
	        }
	    iObjectNotification = ETrue;
	    }
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HandleQueryNewResults
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HandleQueryNewResults( CMdEQuery& /* aQuery */,
        TInt /* aFirstNewItemIndex */,
        TInt /* aNewItemCount */ )
	{
    _LIT( KMsg1, "CallBck HandleQueryNewResults" );
    iLog->Log( KMsg1 );     
    RDebug::Print( KMsg1 );
	}

// -----------------------------------------------------------------------------
// CHarvesterClientTest::HandleQueryCompleted
// -----------------------------------------------------------------------------
//
void CHarvesterClientTestScripter::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError)
	{
    _LIT( KMsg, "CallBck HandleQueryCompleted - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print( msg );
	
	if( iQuery && iQuery == &aQuery )
		{
		for( TInt i = 0; i < aQuery.Count(); i++ )
		    {
		    TInt id = aQuery.ResultItem( i ).Id();
		    CMdEObject* temp = NULL;
		    TRAP_IGNORE( temp = iMdeSession->GetFullObjectL( id ) );
		    if( MdsUtils::Compare( temp->Uri(), iUri ) == 0 )
		        {
		        iQuerySuccess = ETrue;
		        break;
		        }
		    }
		}
	
	if( iQuerySuccess )
	    {
        // session event
        TEventIf event( TEventIf::ESetEvent, _L("Query") );
        TestModuleIf().Event( event );
	    }
	}

//  End of File
