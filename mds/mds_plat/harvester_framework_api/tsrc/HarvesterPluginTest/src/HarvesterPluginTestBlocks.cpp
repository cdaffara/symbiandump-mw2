/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
#include <StifParser.h>
#include <StifTestEventInterface.h>
#include <StifTestInterface.h>
#include "HarvesterPluginTest.h"

#include "mdsutils.h"
#include "harvesterblacklist.h"
#include "mdeobject.h"
#include "mdeobjecthandler.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CHarvesterPluginTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        ENTRY( "BeginTestSession", CHarvesterPluginTest::BeginTestSessionL ),
        ENTRY( "EndTestSession", CHarvesterPluginTest::EndTestSession ),
        ENTRY( "LoadPlugins", CHarvesterPluginTest::LoadPluginsL ),
        ENTRY( "TestFunctions", CHarvesterPluginTest::TestFunctionsL ),
        		
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::HandleSessionOpened
// -----------------------------------------------------------------------------
//
void CHarvesterPluginTest::HandleSessionOpened( CMdESession& /*aClient*/, TInt aError )
    {
    _LIT( KMsg, "CallBck HandleSessionOpened - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, aError);
    iLog->Log( msg );     
    RDebug::Print(msg);
    
    TRAP_IGNORE( iMdeObjectHandler = CMdeObjectHandler::NewL( *iMdeSession ) );
    
    // session event
    TEventIf event( TEventIf::ESetEvent, _L("Session") );
    TestModuleIf().Event( event );
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::HandleSessionError
// -----------------------------------------------------------------------------
//
void CHarvesterPluginTest::HandleSessionError( CMdESession& /*aClient*/, TInt aError )
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
// CHarvesterPluginTest::HarvestingCompleted
// -----------------------------------------------------------------------------
//
void CHarvesterPluginTest::HarvestingCompleted( CHarvesterData* /* aHarvesterData */ )
	{ 
    // harvest event
    TEventIf event( TEventIf::ESetEvent, _L("Harvest") );
    TestModuleIf().Event( event );
	}

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::BeginTestSessionL
// -----------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::BeginTestSessionL( CStifItemParser& /* aItem */ )
    {
    TRAPD( error, iMdeSession = CMdESession::NewL( *this ) );

    iSession = CMdEHarvesterSession::NewL( *iMdeSession );
    
    iCtxEngine = NULL;
    	
    iBlacklist = CHarvesterBlacklist::NewL();
    
    _LIT( KMsg, "Exit BeginTestSession - Error code : %d" );
    TBuf <100> msg;
    msg.Format(KMsg, error);
    iLog->Log( msg );
    RDebug::Print( msg );
    
    User::LeaveIfError( error );
    
    return KErrNone;
    }    

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::EndTestSession
// -----------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::EndTestSession( CStifItemParser& /* aItem */ )
	{       
	iPluginInfoArray.ResetAndDestroy();
	iPluginInfoArray.Close();
	
	REComSession::FinalClose();
    
    if ( iCtxEngine )
    	{
    	iCtxEngine->ReleaseInstance();
    	iCtxEngine = NULL;
    	}

    if ( iBlacklist )
        {
        iBlacklist->CloseDatabase();
        delete iBlacklist;
        }
    
    if ( iSession )
    	{
    	delete iSession;
    	iSession = NULL;
    	}
    
    if ( iMdeObjectHandler )
    	{
    	delete iMdeObjectHandler;
    	iMdeObjectHandler = NULL;
    	}

    if( iMdeSession )
    	{
    	delete iMdeSession;
    	iMdeSession = NULL;
    	}
    
    _LIT( KMsg, "Exit    EndTestSession" );
    iLog->Log( KMsg );  
    RDebug::Print(KMsg);
	
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::LoadPluginsL
// -----------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::LoadPluginsL( CStifItemParser& /* aItem */ )
    {
    _LIT( KMsg1, "Enter LoadPlugins" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
    RImplInfoPtrArray infoArray;
    	
	TCleanupItem cleanupItem( MdsUtils::CleanupEComArray, &infoArray );
	CleanupStack::PushL( cleanupItem );
	
	CHarvesterPlugin::ListImplementationsL( infoArray );
	TInt count( 0 );
	count = infoArray.Count();
	
	if( count == 0 )
	    {
	    return KErrNotFound;
	    }
	
	for ( TInt i=0; i < count; i++ )
		{
		// Parse the file extensions and resolve plug-in's uids from infoArray to iDataTypeArray
        TBufC8<256> type;
        TBufC8<256> opaque;
        
        type = infoArray[i]->DataType();
        opaque = infoArray[i]->OpaqueData();
        TUid implUID = infoArray[i]->ImplementationUid(); 
                
        AddNewPluginL(type, opaque, implUID);
		}
	
	CleanupStack::PopAndDestroy( &infoArray ); // infoArray, results in a call to CleanupEComArray
    
    _LIT( KMsg2, "Exit    LoadPlugins" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CHarvesterPluginTest::TestFunctionsL
// -----------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::TestFunctionsL( CStifItemParser& aItem )
    {
    _LIT( KMsg1, "Enter TestFunctions" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
	TPtrC inputFile;
	User::LeaveIfError( aItem.GetNextString( inputFile ) );
	
	iMdeSession->RemoveObjectL( inputFile );
	
	TBuf<100> aObjectDef;

    _LIT( KMsg2_1, "enter GetObjectDef" );
    iLog->Log( KMsg2_1 );
    RDebug::Print( KMsg2_1 );
	
	GetObjectDef( inputFile, aObjectDef, EFalse );
    
	_LIT( KMsg2_2, "return from GetObjectDef" );
    iLog->Log( KMsg2_2 );
    RDebug::Print( KMsg2_2 );
	
	// harvesterdata object
	HBufC* uriBuf = inputFile.AllocLC();

    _LIT( KMsg3, "Creating harvester data" );
    iLog->Log( KMsg3 );
    RDebug::Print( KMsg3 );
	
	CHarvesterData* data = CHarvesterData::NewL( uriBuf );
	data->SetPluginObserver( *this );
	CMdEObject* mdeObject = NULL;
	_LIT( string, "Image" );

    _LIT( KMsg4, "Getting metadata object" );
    iLog->Log( KMsg4 );
    RDebug::Print( KMsg4 );
	
	mdeObject = iMdeObjectHandler->GetMetadataObjectL( *data, string );
	
    _LIT( KMsg5, "Setting metadata object" );
    iLog->Log( KMsg5 );
    RDebug::Print( KMsg5 );
	
	data->SetMdeObject( mdeObject );
	
    _LIT( KMsg6, "Metadata object set" );
    iLog->Log( KMsg6 );
    RDebug::Print( KMsg6 );
	
	CleanupStack::Pop( uriBuf );

    _LIT( KMsg7, "Calling HarvestL" );
    iLog->Log( KMsg7 );
    RDebug::Print( KMsg7 );
	
	HarvestL( data );
	
    _LIT( KMsg8, "Exit TestFunctions" );
    iLog->Log( KMsg8 );
    RDebug::Print( KMsg8 );
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// GetObjectDef
// ---------------------------------------------------------------------------
//
TBool CHarvesterPluginTest::GetObjectDef(const TDesC& aUri, TDes& aObjectDef, TBool aCheck)
	{
    _LIT( KMsg1, "GetObjectDef" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
    
	CHarvesterPluginInfo* hpi = NULL; 

	TRAPD( err, hpi = GetHPIFromFileNameL(aUri) );
	if ( err != KErrNone )
		{
	    _LIT( KMsg2, "GetHPIFromFileNameL not KErrNone, objectDef is 0, return" );
	    iLog->Log( KMsg2 );
	    RDebug::Print( KMsg2 );
	    
		aObjectDef.Zero();
		return EFalse;
		}

	if ( !hpi )
		{
        _LIT( KMsg3, "no hpi, objectDef is 0, return" );
        iLog->Log( KMsg3 );
        RDebug::Print( KMsg3 );
		aObjectDef.Zero();
		}
	else
		{
        _LIT( KMsg4, "hpi found" );
        iLog->Log( KMsg4 );
        RDebug::Print( KMsg4 );
        
		if( hpi->iObjectTypes.Count() > 1)
			{
	        _LIT( KMsg5_1, "plugin supports over 1 object type" );
	        iLog->Log( KMsg5_1 );
	        RDebug::Print( KMsg5_1 );
	        
			if( aCheck )
				{
				aObjectDef.Zero();
				return ETrue;
				}
			
			if ( ! hpi->iPlugin )
	    		{
	    		TRAPD( error, hpi->iPlugin = CHarvesterPlugin::NewL( hpi->iPluginUid ) );
	             if( error )
	                    {
	                    aObjectDef.Zero();
	                    return EFalse;
	                    }
	             else
	                 {
	                 hpi->iPlugin->SetQueue( hpi->iQueue );
	                 }
	    		}
			
            _LIT( KMsg5_2, "calling iPlugin->GetObjectType" );
            iLog->Log( KMsg5_2 );
            RDebug::Print( KMsg5_2 );
            
			hpi->iPlugin->GetObjectType( aUri, aObjectDef );
			
	        _LIT( KMsg5_3, "object type selected" );
	        iLog->Log( KMsg5_3 );
	        RDebug::Print( KMsg5_3 );
			}
		else
			{
            _LIT( KMsg6_1, "plugin supports 1 object type, select it" );
            iLog->Log( KMsg6_1 );
            RDebug::Print( KMsg6_1 );
		    
			aObjectDef.Copy( *(hpi->iObjectTypes[0]) );
			_LIT( KMsg6_2, "object type selected" );
            iLog->Log( KMsg6_2 );
            RDebug::Print( KMsg6_2 );
			}
		}
	
	return EFalse;
	}

// ---------------------------------------------------------------------------
// HarvestL
// ---------------------------------------------------------------------------
//
TInt CHarvesterPluginTest::HarvestL( CHarvesterData* aHD )
	{
    _LIT( KMsg1, "Enter HarvestL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	CHarvesterPluginInfo* hpi = aHD->HarvesterPluginInfo();
	
	if ( !hpi ) 
		{
		 hpi = GetHPIFromFileNameL( aHD->Uri() );
		}
	
	if ( hpi )
		{
		if ( ! hpi->iPlugin )
    		{
    		hpi->iPlugin = CHarvesterPlugin::NewL( hpi->iPluginUid );
    		hpi->iPlugin->SetQueue( hpi->iQueue );
    		hpi->iPlugin->SetBlacklist( *iBlacklist );
    		}
		
	    if( aHD->ObjectType() == EFastHarvest || aHD->Origin() == MdeConstants::Object::ECamera )
	    	{
	    	hpi->iQueue.Insert( aHD, 0 );
	    	}
	    else
			{
			hpi->iQueue.AppendL( aHD );
			}
	    
		hpi->iPlugin->StartHarvest();
	
	    _LIT( KMsg2, "Exit HarvestL" );
	    iLog->Log( KMsg2 );
	    RDebug::Print( KMsg2 );
		
		return KErrNone;
		}
	
	return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// AddNewPluginL
// -----------------------------------------------------------------------------
//
void CHarvesterPluginTest::AddNewPluginL( const TDesC8& aType, const TDesC8& aOpaque, TUid aPluginUid )
    {      
    CHarvesterPluginInfo* pluginInfo = new (ELeave) CHarvesterPluginInfo;
    CleanupStack::PushL( pluginInfo );

    // get file extensions
    TLex8 lex( aOpaque );
    while ( !lex.Eos() )
        {
        /* Tokenizing file extensions using TLex8 */
        lex.SkipSpaceAndMark();

        TPtrC8 extToken = lex.NextToken();

        HBufC* str = HBufC::NewLC( extToken.Length() );
        str->Des().Copy( extToken );
        pluginInfo->iExtensions.AppendL( str );
        CleanupStack::Pop( str );
        }

    // get object types
    TLex8 lexObjectTypes( aType );
    while ( !lexObjectTypes.Eos() )
        {
        /* Tokenizing object types using TLex8 */
        lexObjectTypes.SkipSpaceAndMark();
        
        TPtrC8 objectTypeToken = lexObjectTypes.NextToken();

        HBufC* str = HBufC::NewLC( objectTypeToken.Length() );
        str->Des().Copy( objectTypeToken );
        pluginInfo->iObjectTypes.AppendL( str );
        CleanupStack::Pop( str );
        }

    /* We set plugin as NULL - we only load them when needed */
    pluginInfo->iPlugin = NULL;
    pluginInfo->iPluginUid = aPluginUid;
    iPluginInfoArray.AppendL( pluginInfo );
    CleanupStack::Pop( pluginInfo );
    }

// ---------------------------------------------------------------------------
// GetHPIFromFileNameL
// ---------------------------------------------------------------------------
//
CHarvesterPluginInfo* CHarvesterPluginTest::GetHPIFromFileNameL(const TDesC& aFileName)
	{
    _LIT( KMsg1, "Enter GetHPIFromFileNameL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	TParsePtrC parser( aFileName );
    
    // without trailing dot "."
    TPtrC ptr = parser.Ext();
    if ( ptr.Length() > 1 )
    	{
    	ptr.Set( ptr.Mid( 1 ) );
    	}

    _LIT( KMsg2, "Exit GetHPIFromFileNameL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
    
	return GetFromNormalFileL( ptr );
	}

// ---------------------------------------------------------------------------
// GetFromNormalFile
// ---------------------------------------------------------------------------
//
CHarvesterPluginInfo* CHarvesterPluginTest::GetFromNormalFileL( TDesC& aExt )
	{
    _LIT( KMsg1, "Enter GetFromNormalFileL" );
    iLog->Log( KMsg1 );
    RDebug::Print( KMsg1 );
	
	CHarvesterPluginInfo* retVal = NULL;

	for ( TInt i = iPluginInfoArray.Count(); --i >= 0; )
        {
        CHarvesterPluginInfo* info = iPluginInfoArray[i];
        TBool found = EFalse;
        
        for ( TInt k = info->iExtensions.Count(); --k >= 0; )
            {
            TDesC* ext = info->iExtensions[k];
            
            // checking against supported plugin file extensions
            TInt result = ext->CompareF( aExt );
            if ( result == 0 )
                {
                retVal = info;
                found = ETrue;
                break;
                }
            }
        if ( found )
        	{
        	break;
        	}
        }

    _LIT( KMsg2, "Exit GetFromNormalFileL" );
    iLog->Log( KMsg2 );
    RDebug::Print( KMsg2 );
	
	return retVal;
	}

//  [End of File] - Do not remove
