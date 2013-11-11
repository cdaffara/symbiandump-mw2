/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "ContextEnginePluginTest.h"

#include <harvesterdata.h>
#include "mdsutils.h"
#include "mdeconstants.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        // engine		
        ENTRY( "BeginTestSession", CContextEnginePluginTest::BeginTestSessionL ),
        ENTRY( "EndTestSession", CContextEnginePluginTest::EndTestSession ),
        ENTRY( "CreateObjects", CContextEnginePluginTest::CreateObjectsL ),
        ENTRY( "DeleteObjects", CContextEnginePluginTest::DeleteObjectsL ),
        ENTRY( "CreateContextEngineWithObserver", CContextEnginePluginTest::CreateContextEngine1L ),
        ENTRY( "CreateContextEngineWithoutObserver", CContextEnginePluginTest::CreateContextEngine2L ),
        ENTRY( "ReleaseContextEngineInstance", CContextEnginePluginTest::ReleaseInstanceL ),
        ENTRY( "SetMdESessionTest", CContextEnginePluginTest::SetMdeSessionL ),
        ENTRY( "PluginCount", CContextEnginePluginTest::PluginCount ),
        ENTRY( "Destroy", CContextEnginePluginTest::Destroy ),
        ENTRY( "ContextEngineStaticData", CContextEnginePluginTest::ContextEngineStaticData ),

        ENTRY( "ContextSnapshots", CContextEnginePluginTest::ContextSnapshotsL ),
        ENTRY( "MultipleSnapshots", CContextEnginePluginTest::MultipleSnapshotsL ),
        
        // plugin
        ENTRY( "LoadPlugin", CContextEnginePluginTest::LoadPluginL ),

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::HandleSessionOpened( CMdESession& /*aClient*/, TInt aError )
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
// CContextEnginePluginTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::HandleSessionError( CMdESession& /*aClient*/, TInt aError )
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
// CContextEnginePluginTest::ContextInitializationStatus
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::ContextInitializationStatus( TInt aError )
    {
    _LIT( KMsg, "CallBck ContextInitializationStatus - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    iInitCount++;
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Init") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::ContextSnapshotStatus
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::ContextSnapshotStatus( CHarvesterData* aHD )
    {
    _LIT( KMsg1, "Enter ContextSnapshotStatus" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    _LIT( KMsg, "CallBck ContextSnapshotStatus - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aHD->ErrorCode());
    iLog->Log( msg );
    RDebug::Print( msg );
    
    if( !aHD )
        {
        iErrorCode = KErrUnknown;
        }
    
    if( aHD && iErrorCode == KErrNone )
        {
        iErrorCode = aHD->ErrorCode();
        }
    
    iSnapshotCount++;
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Snapshot") );
    TestModuleIf().Event( event );
    
    _LIT( KMsg2, "Exit ContextSnapshotStatus" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::PluginInitializationStatus
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::PluginInitializationStatus( TInt aError )
    {
    _LIT( KMsg, "CallBck PluginInitializationStatus - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    iInitCount++;
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Init") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::PluginSnapshotStatus
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::PluginSnapshotStatus( CHarvesterData* aHD )
    {
    _LIT( KMsg1, "Enter PluginSnapshotStatus" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    _LIT( KMsg, "CallBck PluginSnapshotStatus - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aHD->ErrorCode());
    iLog->Log( msg );
    RDebug::Print( msg );
    
    if( !aHD )
        {
        iErrorCode = KErrUnknown;
        }
    
    if( aHD && iErrorCode == KErrNone )
        {
        iErrorCode = aHD->ErrorCode();
        }
    
    iSnapshotCount++;
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Snapshot") );
    TestModuleIf().Event( event );
    
    _LIT( KMsg2, "Exit PluginSnapshotStatus" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::ActiveWait
// -----------------------------------------------------------------------------
//
void CContextEnginePluginTest::ActiveWait( TInt aTimeout )
    {
    _LIT( KMsg1, "Enter ActiveWait" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );

    TTimeIntervalMicroSeconds32 timeout( aTimeout );
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After(status,timeout);

    User::WaitForAnyRequest();
    
    timer.Close();
    
    _LIT( KMsg2, "Exit ActiveWait" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::BeginTestSessionL
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::BeginTestSessionL( CStifItemParser& /* aItem */ )
    {
    iMdEClient = NULL;
    TRAPD( error, iMdEClient = CMdESession::NewL( *this ) );
    iErrorCode = KErrNone;

    _LIT( KMsg, "Exit BeginTestSession - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, error);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    iInitCount = 0;
    iSnapshotCount = 0;
    iInitTarget = 0;
    iSnapshotTarget = 0;
    
    iPluginSet = EFalse;
    
    User::LeaveIfError( error );
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::EndTestSession
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::EndTestSession( CStifItemParser& /* aItem */ )
	{
    _LIT( KMsg1, "Enter EndTestSession" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
	
	iIds.Close();
	iObjectArray.Close();
	    
    if ( iContextEngine )
        {
        iContextEngine->ReleaseInstance();
        iContextEngine = NULL;
        }
    
    if ( iPlugin )
        {
        delete iPlugin;
        iPlugin = NULL;
        }
    
    if ( iLocationPlugin && !iPluginSet )
        {
        delete iLocationPlugin;
        iLocationPlugin = NULL;
        }
    
    if ( iCalendarPlugin )
        {
        delete iCalendarPlugin;
        iCalendarPlugin = NULL;
        }
	
	if ( iMdEClient )
		{
	    delete iMdEClient;
	    iMdEClient = NULL;
	    }
	
	iHD = NULL;
    
    REComSession::FinalClose();
	
    if( iInitCount != iInitTarget )
        {
        return KErrUnknown;
        }
    
    if( iSnapshotCount != iSnapshotTarget )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg2, "Exit EndTestSession" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return iErrorCode;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::CreateObjects
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::CreateObjectsL( CStifItemParser& aItem )
    {
	CMdENamespaceDef& defaultNamespace = iMdEClient->GetDefaultNamespaceDefL();
	CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
	
	// Mandatory parameters for any object.
	CMdEPropertyDef& creationDef = imageDef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
	CMdEPropertyDef& modifiedDef = imageDef.GetPropertyDefL( MdeConstants::Object::KLastModifiedDateProperty );
	CMdEPropertyDef& sizeDef = imageDef.GetPropertyDefL( MdeConstants::Object::KSizeProperty );
	CMdEPropertyDef& itemTypeDef = imageDef.GetPropertyDefL( MdeConstants::Object::KItemTypeProperty );
	
	CMdEPropertyDef& descriptionPropertyDef = imageDef.GetPropertyDefL( MdeConstants::MediaObject::KDescriptionProperty );
	CMdEPropertyDef& widthDef = imageDef.GetPropertyDefL( MdeConstants::MediaObject::KWidthProperty ); 
	CMdEPropertyDef& heightDef = imageDef.GetPropertyDefL( MdeConstants::MediaObject::KHeightProperty ); 
	CMdEPropertyDef& makeDef = imageDef.GetPropertyDefL( MdeConstants::Image::KMakeProperty );
	CMdEPropertyDef& modelDef = imageDef.GetPropertyDefL( MdeConstants::Image::KModelProperty );
    
	CMdEObject* object = NULL;
	TItemId objectId(0);
	TTime currTime;
	TPtrC inputFile;
	TInt count(0);
	TBuf <100> msg;
	
	iIds.Reset();
	
	User::LeaveIfError( aItem.GetNextInt( count ));
	
	for ( TInt i = 0; i < count; ++i )
		{			
		User::LeaveIfError( aItem.GetNextString( inputFile ));
		
		// create object
		iMdEClient->RemoveObjectL(inputFile);
		object = iMdEClient->NewObjectLC(imageDef, inputFile);
		
		currTime.HomeTime();
		object->AddTimePropertyL(creationDef, currTime);
		object->AddTimePropertyL(modifiedDef, currTime);
		object->AddUint32PropertyL( sizeDef, 0 );
		object->AddTextPropertyL( itemTypeDef, MdeConstants::Image::KImageObject );
		
		objectId = iMdEClient->AddObjectL(*object);
		iIds.Append(objectId);
		
		CleanupStack::PopAndDestroy(object);
		object = NULL;
		
		// open object for modification
		TRAPD( openError, object = iMdEClient->OpenObjectL(objectId) );
			
		if ( !object || openError != KErrNone )
			{
			_LIT( KOpenErr, "        ModifyObjects - Open error : %d" );
		    msg.Format(KOpenErr, openError);
		    iLog->Log( msg );
		    RDebug::Print(msg);
		    
		    User::LeaveIfError(openError);
			}

		CleanupStack::PushL( object );		
		
		// add properties
   		CMdEProperty* property = NULL;
   		
   		// Description
   		_LIT( KDesc, "Description" );
		object->Property( descriptionPropertyDef, property );
		
		if ( property )
			{					
			property->SetTextValueL( KDesc );
			}
		else
			{
			object->AddTextPropertyL( descriptionPropertyDef, KDesc );
			}
		
		property = NULL;
		
		// Width
		object->Property( widthDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 100 );
			}
		else
			{
			object->AddUint16PropertyL( widthDef, 100 );
			}
		
		property = NULL;
			
		// Height
		object->Property( heightDef, property );
		
		if ( property )
			{
			property->SetUint16ValueL( 100 );
			}
		else
			{
			object->AddUint16PropertyL( heightDef, 100 );
			}
		
		property = NULL;
		
		// Maker
		object->Property( makeDef, property );
		
        _LIT( KOnkia, "Maker" );
		if ( property )
			{
			property->SetTextValueL( KOnkia );
			}
		else
			{
			object->AddTextPropertyL( makeDef, KOnkia );
			}
			
		property = NULL;
			
		// Model
		object->Property( modelDef, property );
        _LIT( KModel, "Model" );
		if ( property )
			{					
			property->SetTextValueL( KModel );
			}
		else
			{
			object->AddTextPropertyL( modelDef, KModel );
			}
		
		// Commit object
		TRAPD( commitError, iMdEClient->CommitObjectL(*object) );
		if ( commitError != KErrNone )
			{		
			_LIT( KCommErr, "        CreateObjects - Commit error : %d" );
		    msg.Format(KCommErr, commitError);
		    iLog->Log( msg );
		    RDebug::Print(msg);
		    
		    User::LeaveIfError(commitError);
			}
		
		CleanupStack::PopAndDestroy(object);
		object = NULL;
		}
	
    _LIT( KMsg, "Exit    EndCreateObjects" );
    iLog->Log( KMsg );  
    RDebug::Print( KMsg );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::DeleteObjects
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::DeleteObjectsL( CStifItemParser& /* aItem */ )
    {
    ActiveWait( 2000000 );
    
    _LIT( KMsg1, "Enter EndDeleteObjects" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
	TInt count(0);
    count = iIds.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		TInt id = iIds[i];
	    iMdEClient->RemoveObjectL( id );
		}
	
	iIds.Reset();
    
    _LIT( KMsg2, "Exit EndDeleteObjects" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::CreateContextEngine1L
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::CreateContextEngine1L( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter CreateContextEngine" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iInitTarget = 1;
    
    iContextEngine = NULL;
    iContextEngine = CContextEngine::GetInstanceL( this );
    
    if( !iContextEngine )
        {
        User::Leave( KErrUnknown );
        }
    
    _LIT( KMsg2, "Exit CreateContextEngine" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::CreateContextEngine2L
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::CreateContextEngine2L( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter CreateContextEngine" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iContextEngine = NULL;
    iContextEngine = CContextEngine::GetInstanceL();
    
    // 4 seconds
    TTimeIntervalMicroSeconds32 timeout(4000000);
    RTimer timer;
    TRequestStatus status;

    timer.CreateLocal();
    timer.After( status,timeout );

    User::WaitForAnyRequest();
    
    timer.Close();
    
    if( !iContextEngine )
        {
        User::Leave( KErrUnknown );
        }
    
    _LIT( KMsg2, "Exit CreateContextEngine" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::CreateContextEngine2L
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::ReleaseInstanceL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter ReleaseInstanceL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    if( !iContextEngine )
        {
        User::Leave( KErrUnknown );
        }
    
    iContextEngine->ReleaseInstance();
    
    iContextEngine = NULL;
    
    _LIT( KMsg2, "Exit ReleaseInstanceL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::CreateContextEngine2L
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::SetMdeSessionL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter SetMdeSessionL" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iInitTarget = 1;
    
    iContextEngine = NULL;
    iContextEngine = CContextEngine::GetInstanceL( this );
    
    if( !iContextEngine )
        {
        User::Leave( KErrUnknown );
        }
    
    iContextEngine->SetMdeSession( iMdEClient );
    
    if( !iContextEngine || !iMdEClient )
        {
        User::Leave( KErrUnknown );
        }
    
    iContextEngine->SetMdeSession( NULL );
    
    if( !iContextEngine )
        {
        User::Leave( KErrUnknown );
        }
    
    _LIT( KMsg2, "Exit SetMdeSessionL" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::ContextSnapshots
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::ContextSnapshotsL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter ContextSnapshots" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iSnapshotTarget = 9;
    
    const TInt KLoopCount = 3;
    TInt count = iIds.Count();
    
    CMdEObject* mdeObject;
    iObjectArray.Reset();
    
    // choose if plugin tested, otherwise engine
    TPtrC testPlugin;
    TBool plugin = EFalse;
    User::LeaveIfError( aItem.GetNextString( testPlugin ));
    _LIT( pluginString, "plugin" );
    if( testPlugin == pluginString )
    	{
    	plugin = ETrue;
    	}
    
    // get object
    mdeObject = iMdEClient->GetObjectL( iIds[0] );

    HBufC* uriBuf = mdeObject->Uri().AllocLC();
    iHD = CHarvesterData::NewL( uriBuf );
    iHD->SetMdeObject( mdeObject );
    CleanupStack::Pop( uriBuf );
    
    // snapshots
    for ( TInt i = 0; i < KLoopCount; i++ )
        {
        if (plugin)
        	{
        	iPlugin->ContextSnapshot( *this, *iHD );
        	}
        else
        	{
        	iContextEngine->ContextSnapshot( *this, *iHD );
        	}
        }
    mdeObject = NULL;
    
    mdeObject = iMdEClient->GetObjectL( iIds[1] );
    uriBuf = mdeObject->Uri().AllocLC();
    iHD = CHarvesterData::NewL( uriBuf );
    iHD->SetMdeObject( mdeObject );
    CleanupStack::Pop( uriBuf );
    
    // snapshots
    for ( TInt i = 0; i < KLoopCount; i++ )
        {
        if (plugin)
        	{
        	iPlugin->ContextSnapshot( *this, *iHD );
        	}
        else
        	{
        	iContextEngine->ContextSnapshot( *this, *iHD );
        	}
        }    
    mdeObject = NULL;
    
    mdeObject = iMdEClient->GetObjectL( iIds[2] );
    uriBuf = mdeObject->Uri().AllocLC();
    iHD = CHarvesterData::NewL( uriBuf );
    iHD->SetMdeObject( mdeObject );
    CleanupStack::Pop( uriBuf );
    
    // snapshots
    for ( TInt i = 0; i < KLoopCount; i++ )
        {
        if (plugin)
        	{
        	iPlugin->ContextSnapshot( *this, *iHD );
        	}
        else
        	{
        	iContextEngine->ContextSnapshot( *this, *iHD );
        	}
        }  
    
    _LIT( KMsg2, "Exit ContextSnapshots" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::MultipleSnapshots
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::MultipleSnapshotsL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter MultipleSnapshots" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iSnapshotTarget = 3;
    
    TInt count = iIds.Count();
    
    CMdEObject* mdeObject;
    iObjectArray.Reset();
    
    // choose if plugin tested, otherwise engine
    TPtrC testPlugin;
    TBool plugin = EFalse;
    User::LeaveIfError( aItem.GetNextString( testPlugin ));
    _LIT( pluginString, "plugin" );
    if( testPlugin == pluginString )
    	{
    	plugin = ETrue;
    	}
    
    // multiple objects
	for ( TInt i = 0; i < count; ++i )
		{
	    iHD = NULL;
	    mdeObject = NULL;
		
	    mdeObject = iMdEClient->GetObjectL( iIds[i] );
	    HBufC* uriBuf = mdeObject->Uri().AllocLC();
	    iHD = CHarvesterData::NewL( uriBuf );
	    iHD->SetMdeObject( mdeObject );
	    CleanupStack::Pop( uriBuf );
	    
	    iObjectArray.Append( iHD );
		}
	
	// snapshots
    if( plugin )
		{
		iPlugin->ContextSnapshot( *this, iObjectArray );
		}
	else
		{
		iContextEngine->ContextSnapshot( *this, iObjectArray );
		}
    
    _LIT( KMsg2, "Exit MultipleSnapshots" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::PluginCount
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::PluginCount( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter PluginCount" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    TInt count(0);
    count = iContextEngine->PluginCount();
    
    if( count != 2 )
        {
        return KErrNotFound;
        }
    
    _LIT( KMsg, "Exit PluginCount : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, count);
    iLog->Log( msg );
    RDebug::Print(msg);
	
    _LIT( KMsg2, "Enter PluginCount" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::Destroy
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::Destroy( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter Destroy" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    iContextEngine->Destroy();
	
    _LIT( KMsg2, "Exit Destroy" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::ContextEngineStaticData
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::ContextEngineStaticData( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter ContextEngineStaticData" );
    iLog->Log( KMsg1 );  
    RDebug::Print( KMsg1 );
    
    const TInt KContextEngineTLSKey = 0x200009f6;
    
    CContextEngineStaticData* data = static_cast<CContextEngineStaticData*>(
            UserSvr::DllTls(KContextEngineTLSKey) );
    
    if( !data )
        {
        return KErrUnknown;
        }
    
    _LIT( KMsg2, "Exit ContextEngineStaticData" );
    iLog->Log( KMsg2 );  
    RDebug::Print( KMsg2 );
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CContextEnginePluginTest::LoadPluginL
// -----------------------------------------------------------------------------
//
TInt CContextEnginePluginTest::LoadPluginL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter LoadPlugins" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    iInitTarget = 2;
    
    TUid calendarContextUID = { 0x102823CB }; // calendar context plugin uid
    TUid locationContextUID = { 0x200071D0 }; // location context plugin uid
    
    RImplInfoPtrArray infoArray;
    	
	TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
	CleanupStack::PushL( cleanupItem );
	
	CContextPlugin::ListImplementationsL( infoArray );
	TInt count( 0 );
	count = infoArray.Count();
	
	TBool locationPluginFound( EFalse );
	TBool calendarPluginFound( EFalse );
	for ( TInt i=0; i < count; i++ )
		{
		TUid uid = infoArray[i]->ImplementationUid();
		
		// context plugin found
		if( uid == locationContextUID )
			{
			iLocationPlugin = CContextPlugin::NewL( uid );
			iLocationPlugin->SetMdeSession(*iMdEClient);
			iLocationPlugin->Init(*this);
			locationPluginFound = ETrue;
			iPlugin = iLocationPlugin;
			iPluginSet = ETrue;
			}		
		else if( uid == calendarContextUID )
	        {
	        iCalendarPlugin = CContextPlugin::NewL( uid );
	        iCalendarPlugin->SetMdeSession(*iMdEClient);
	        iCalendarPlugin->Init(*this);
	        calendarPluginFound = ETrue;
	        }   
		}
	
	CleanupStack::PopAndDestroy( &infoArray ); // infoArray, results in a call to CleanupEComArray
    
	if( !locationPluginFound )
	    {
	    _LIT( KLocation, "Location context plugin not found" );
	    iLog->Log( KLocation );
	    RDebug::Print( KLocation );
	    return KErrNotFound;
	    }
	
    if( !calendarPluginFound )
        {
        _LIT( KCalendar, "Calendar context plugin not found" );
        iLog->Log( KCalendar );
        RDebug::Print( KCalendar );
        return KErrNotFound;
        }
    
    _LIT( KMsg2, "Exit LoadPlugins" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

//  [End of File] - Do not remove
