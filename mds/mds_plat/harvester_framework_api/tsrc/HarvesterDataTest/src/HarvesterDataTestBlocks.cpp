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
#include <ecom.h>
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include "HarvesterDataTest.h"

#include <etel3rdparty.h>
#include "cnetworkinfo.h"
#include "mdsutils.h"
#include "mdeconstants.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHarvesterDataTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CHarvesterDataTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        ENTRY( "ActiveWait", CHarvesterDataTest::ActiveWait ),
        ENTRY( "BeginTestSession", CHarvesterDataTest::BeginTestSessionL ),
        ENTRY( "EndTestSession", CHarvesterDataTest::EndTestSession ),
        ENTRY( "HarvestClientDataTests", CHarvesterDataTest::HarvestClientDataTestsL ),
        ENTRY( "PlaceholderDataTests", CHarvesterDataTest::PlaceholderDataTestsL ),
        ENTRY( "HarvesterDataTests", CHarvesterDataTest::HarvesterDataTestsL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CHarvesterDataTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::HandleSessionOpened( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CHarvesterDataTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::HandleSessionError( CMdESession& /*aClient*/, TInt aError )
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
// CHarvesterDataTest::HarvestingCompleted
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::HarvestingCompleted( CHarvesterData* /* aHarvesterData */ )
	{ 
	}

// -----------------------------------------------------------------------------
// CHarvesterDataTest::MonitorEvent
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::MonitorEvent( CHarvesterData* /* aHarvesterData */ )
	{ 
	}

// -----------------------------------------------------------------------------
// CHarvesterDataTest::MonitorEvent
// -----------------------------------------------------------------------------
//
void CHarvesterDataTest::MonitorEvent( RPointerArray<CHarvesterData>& /* aHarvesterDataArray */ )
	{ 
	}

// -----------------------------------------------------------------------------
// CHarvesterDataTest::ActiveWait
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::ActiveWait( CStifItemParser& /* aItem */ )
    {
    // 2,5 seconds
    TTimeIntervalMicroSeconds32 timeout(2500000);
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After(status,timeout);

    User::WaitForAnyRequest();
    
    timer.Close();
    
    _LIT( KMsg, "Exit    ActiveWait" );
    iLog->Log( KMsg );
    RDebug::Print(KMsg);
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterDataTest::BeginTestSessionL
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::BeginTestSessionL( CStifItemParser& /* aItem */ )
    {
    TRAPD( error, iMdEClient = CMdESession::NewL( *this ) );

    _LIT( KMsg, "Exit BeginTestSession - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, error);
    iLog->Log( msg );
    RDebug::Print(msg);
    
    User::LeaveIfError(error);
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CHarvesterDataTest::EndTestSession
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::EndTestSession( CStifItemParser& /* aItem */ )
	{    
	iAlbumIds.Close();
    
	REComSession::FinalClose();
	    
	if ( iMdEClient )
		{
	    delete iMdEClient;
	    iMdEClient = NULL;
	    }
	
    _LIT( KMsg, "Exit    EndTestSession" );
    iLog->Log( KMsg );  
    RDebug::Print(KMsg);
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterDataTest::HarvestClientDataTests
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::HarvestClientDataTestsL( CStifItemParser& /* aItem */ )
	{ 
    _LIT( KMsg1, "Enter HarvestClientDataTestsL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	CHarvestClientData* data = NULL;
	data = CHarvestClientData::NewL();
	
    iAlbumIds.Reset();
    iAlbumIds.Append(55555);
    iAlbumIds.Append(1);
    iAlbumIds.Append(22);
    iAlbumIds.Append(333);
    iAlbumIds.Append(4444);
    
    data->SetAlbumIds( iAlbumIds );
	
	delete data;
	data = NULL;
	
	data = CHarvestClientData::New();
	
	if( !data )
	    {
	    User::Leave( KErrUnknown );
	    }
	
	iAlbumIds.Reset();
	iAlbumIds.Append(1);
	iAlbumIds.Append(22);
	iAlbumIds.Append(333);
	iAlbumIds.Append(4444);
	iAlbumIds.Append(55555);
	
	data->SetAlbumIds( iAlbumIds );
	
	delete data;
	data = NULL;
	
    _LIT( KMsg2, "Exit HarvestClientDataTestsL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CHarvesterDataTest::PlaceholderDataTests
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::PlaceholderDataTestsL( CStifItemParser& /* aItem */ )
	{ 
    _LIT( KMsg1, "Enter PlaceholderDataTestsL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	TTime time;
	TUint32 size;
	TUint id;
	TBuf<100> tmp;
	TInt state;
	TInt preinstalled;
	_LIT( KUri, "C:\\Data\\Images\\Jpg.jpg" );
	
	CPlaceholderData* data = CPlaceholderData::NewL();
    CleanupStack::PushL( data );
	
	// modified
    _LIT( KMsg2, "modified" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	time.HomeTime();
	data->SetModified( time );
	TTime time2 = data->Modified();
    if( time != time2 )
        {
        return KErrBadDescriptor;
        }
	
	// uri
    _LIT( KMsg3, "uri" );
    iLog->Log( KMsg3 );  
    RDebug::Print( KMsg3 );
	data->SetUri( KUri );
	tmp = data->Uri();
    if( !(MdsUtils::Compare( tmp, KUri ) == 0) )
        {
        return KErrBadDescriptor;
        }
	
	// filesize
    _LIT( KMsg4, "filesize" );
    iLog->Log( KMsg4 );  
    RDebug::Print( KMsg4 );
	size = 8192;
	data->SetFileSize( size );
	TUint32 size2 = data->FileSize();
    if( size != size2 )
        {
        return KErrBadDescriptor;
        }
	
	// mediaid
    _LIT( KMsg5, "mediaid" );
    iLog->Log( KMsg5 );  
    RDebug::Print( KMsg5 );
	id = 64;
	data->SetMediaId( id );
	TUint32 id2 = data->MediaId();
    if( id != id2 )
        {
        return KErrBadDescriptor;
        }
	
    // state
    _LIT( KMsg6, "state" );
    iLog->Log( KMsg6 );  
    RDebug::Print( KMsg6 );
    state = 0;
    data->SetPresentState( state );
    TInt state2 = data->PresentState();
    if( state != state2 )
        {
        return KErrBadDescriptor;
        }
    
    // preinstalled
    _LIT( KMsg7, "preinstalled" );
    iLog->Log( KMsg7 );  
    RDebug::Print( KMsg7 );
    preinstalled = 0;
    data->SetPreinstalled( preinstalled );
    TInt preinstalled2 = data->Preinstalled();
    if( preinstalled != preinstalled2 )
        {
        return KErrBadDescriptor;
        }
    
    CleanupStack::PopAndDestroy( data );

    _LIT( KMsg8, "Exit PlaceholderDataTestsL" );
    iLog->Log( KMsg8 );  
    RDebug::Print( KMsg8 );
	
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CHarvesterDataTest::HarvestClientDataTests
// -----------------------------------------------------------------------------
//
TInt CHarvesterDataTest::HarvesterDataTestsL( CStifItemParser& /* aItem */ )
	{ 
    _LIT( KMsg1, "Enter HarvesterDataTestsL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	TInt errorcode;
	THarvesterEventType event;
	MHarvesterPluginObserver* pluginobs;
	TOrigin origin;
	TBool snap;
	THarvesterObjectType type;
	TAny* client;
	CMdEObject* object;
	TLocationData* location;
	TUid uid;
	TBool add;
	CHarvesterPluginInfo* pluginInfo;
	TBool binary;
	
	_LIT( KUri, "C:\\Data\\Images\\Jpg.jpg" );
	
	// mdeobject
	CMdENamespaceDef& defaultNamespace = iMdEClient->GetDefaultNamespaceDefL();
	CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
	
	iMdEClient->RemoveObjectL( KUri );
	object = iMdEClient->NewObjectL( imageDef, KUri );
	
	// harvesterdata object
	HBufC* uriBuf = object->Uri().AllocLC();
	CHarvesterData* data = CHarvesterData::NewL( uriBuf );
	CleanupStack::Pop( uriBuf );
	
	// uri
    _LIT( KMsg2, "uri" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	TDesC uri2 = data->Uri();

	
	// errorcode
    _LIT( KMsg3, "errorcode" );
    iLog->Log( KMsg3 );  
    RDebug::Print( KMsg3 );
	errorcode = -1;
	data->SetErrorCode( errorcode );
	TInt ret = data->ErrorCode();
	if( ret != errorcode )
	    {
	    return KErrBadDescriptor;
	    }
	
	// eventtype
    _LIT( KMsg4, "eventtype" );
    iLog->Log( KMsg4 );  
    RDebug::Print( KMsg4 );
	event = EHarvesterAdd;
	data->SetEventType(event);
	THarvesterEventType event2 = data->EventType();
    if( event2 != event )
        {
        return KErrBadDescriptor;
        }
	
	// pluginobserver
    _LIT( KMsg5, "pluginobserver" );
    iLog->Log( KMsg5 );  
    RDebug::Print( KMsg5 );
	pluginobs = this;
	data->SetPluginObserver( *pluginobs );
	MHarvesterPluginObserver* pluginobs2 = data->PluginObserver();
    if( pluginobs != pluginobs2 )
        {
        return KErrBadDescriptor;
        }
	pluginobs = NULL;
	
	// origin
    _LIT( KMsg6, "origin" );
    iLog->Log( KMsg6 );  
    RDebug::Print( KMsg6 );
	origin = MdeConstants::Object::EDownloaded;
	data->SetOrigin( origin );
	TOrigin origin2 = data->Origin();
    if( origin != origin2 )
        {
        return KErrBadDescriptor;
        }
	
	// takesnapshot
    _LIT( KMsg7, "takesnapshot" );
    iLog->Log( KMsg7 );  
    RDebug::Print( KMsg7 );
	snap = ETrue;
	data->SetTakeSnapshot( snap );
	TBool snap2 = data->TakeSnapshot();
    if( !(snap && snap2) )
        {
        return KErrBadDescriptor;
        }
	
	// objecttype
    _LIT( KMsg8, "objecttype" );
    iLog->Log( KMsg8 );  
    RDebug::Print( KMsg8 );
	type = ENormal;
	data->SetObjectType(type);
	THarvesterObjectType type2 = data->ObjectType();
    if( type != type2 )
        {
        return KErrBadDescriptor;
        }
	
	// clientdata
    _LIT( KMsg9, "clientdata" );
    iLog->Log( KMsg9 );  
    RDebug::Print( KMsg9 );
	client = NULL;
	data->SetClientData( NULL );
	TAny* client2 = data->ClientData();
    if( client != client2 )
        {
        return KErrBadDescriptor;
        }
	client = NULL;
	
	// mdeobject
    _LIT( KMsg10, "mdeobject" );
    iLog->Log( KMsg10 );  
    RDebug::Print( KMsg10 );
	data->SetMdeObject( object );
	CMdEObject* object2 = &data->MdeObject();
    if( object != object2 )
        {
        return KErrBadDescriptor;
        }
	data->SetMdeObject( NULL );
	
	// locationdata
    _LIT( KMsg11, "locationdata" );
    iLog->Log( KMsg11 );  
    RDebug::Print( KMsg11 );
	location = new ( ELeave ) TLocationData;
	location->iQuality = 0;
	data->SetLocationData( location );
	TLocationData* location2 = data->LocationData();
    if( location != location2 )
        {
        return KErrBadDescriptor;
        }

    // clientid
    _LIT( KMsg12, "clientid" );
    iLog->Log( KMsg12 );  
    RDebug::Print( KMsg12 );
    uid = KNullUid;
    data->SetClientId( uid );
    TUid uid2 = data->ClientId();
    if( uid2 != uid )
        {
        return KErrBadDescriptor;
        }    
    
    // add location
    _LIT( KMsg13, "add location" );
    iLog->Log( KMsg13 );  
    RDebug::Print( KMsg13 );
    add = ETrue;
    data->SetAddLocation( add );
    TBool add2 = data->AddLocation();
    if( !(add && add2) )
        {
        return KErrBadDescriptor;
        }    
    
    // harvesterplugininfo
    _LIT( KMsg14, "pluginobserver" );
    iLog->Log( KMsg14 );  
    RDebug::Print( KMsg14 );
    pluginInfo = NULL;
    data->SetHarvesterPluginInfo( pluginInfo );
    CHarvesterPluginInfo* pluginInfo2 = data->HarvesterPluginInfo();
    if( pluginInfo != pluginInfo2 )
        {
        return KErrBadDescriptor;
        }
    pluginInfo = NULL;

    // binary
    _LIT( KMsg15, "binary" );
    iLog->Log( KMsg15 );  
    RDebug::Print( KMsg15 );
    binary = ETrue;
    data->SetBinary( binary );
    TBool binary2 = data->IsBinary();
    if( !(binary && binary2) )
        {
        return KErrBadDescriptor;
        }   
    
	delete data;
	data = NULL;
	
    _LIT( KMsg16, "Exit HarvesterDataTestsL" );
    iLog->Log( KMsg16 );  
    RDebug::Print( KMsg16 );
	
    return KErrNone;
	}

//  [End of File] - Do not remove
