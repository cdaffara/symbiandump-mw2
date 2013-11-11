/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  playlistenginetest block implementation for STIF Test Framework TestScripter.
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "playlistenginetest.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cplaylistenginetest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cplaylistenginetest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cplaylistenginetest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cplaylistenginetest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Cplaylistenginetest::ExampleL ),
        
        //test functions for mpxplaylistengine.h
        ENTRY( "CMPXPlaylistEngineCreate", Cplaylistenginetest::CMPXPlaylistEngineCreate ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineDestructor", Cplaylistenginetest::CMPXPlaylistEngineDestructor ),//Pass Debug 
        ENTRY( "CMPXPlaylistEnginePlaylistPluginHandler", Cplaylistenginetest::CMPXPlaylistEnginePlaylistPluginHandler ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineIsPlaylistL", Cplaylistenginetest::CMPXPlaylistEngineIsPlaylistL ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineInternalizePlaylistL", Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistL ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineInternalizePlaylistLRFile", Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistLRFile ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineExternalizePlaylistL", Cplaylistenginetest::CMPXPlaylistEngineExternalizePlaylistL ),//Pass Debug 
        ENTRY( "CMPXPlaylistEngineCancelRequests", Cplaylistenginetest::CMPXPlaylistEngineCancelRequests ),//Pass Debug 
       
        //test functions for mpxplaylistpluginhandler.h
        ENTRY( "CMPXPlaylistPluginHandlerCreate", Cplaylistenginetest::CMPXPlaylistPluginHandlerCreate ),//Pass Debug
        ENTRY( "CMPXPlaylistPluginHandlerDestructor", Cplaylistenginetest::CMPXPlaylistPluginHandlerDestructor ),//Pass Debug
        /*ENTRY( "CMPXPlaylistPluginHandlerPluginUid", Cplaylistenginetest::CMPXPlaylistPluginHandlerPluginUid ),//Pass Debug
        ENTRY( "CMPXPlaylistPluginHandlerPluginType", Cplaylistenginetest::CMPXPlaylistPluginHandlerPluginUid ),//Pass Debug
        ENTRY( "CMPXPlaylistPluginHandlerPluginName", Cplaylistenginetest::CMPXPlaylistPluginHandlerPluginUid ),//Pass Debug
        ENTRY( "CMPXPlaylistPluginHandlerPluginFound", Cplaylistenginetest::CMPXPlaylistPluginHandlerPluginUid ),//Pass Debug
        ENTRY( "CMPXPlaylistPluginHandlerPlugin", Cplaylistenginetest::CMPXPlaylistPluginHandlerPlugin ),//Pass Debug
*/        ENTRY( "SelectPlaylistPluginLByUri", Cplaylistenginetest::SelectPlaylistPluginLByUri ),//Pass Debug
        ENTRY( "SelectPlaylistPluginLByFile", Cplaylistenginetest::SelectPlaylistPluginLByFile ),//Pass Debug
        ENTRY( "SelectPlaylistPluginLByType", Cplaylistenginetest::SelectPlaylistPluginLByType ),//Pass Debug
        ENTRY( "SelectPlaylistPluginLByName", Cplaylistenginetest::SelectPlaylistPluginLByName ),//Pass Debug
        ENTRY( "SelectPlaylistPluginLByUid", Cplaylistenginetest::SelectPlaylistPluginLByUid ),//Pass Debug



        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cplaylistenginetest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cplaylistenginetest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kplaylistenginetest, "playlistenginetest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kplaylistenginetest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kplaylistenginetest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineCreate
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineCreate( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    MMPXPlaylistEngineObserver* aObserver(NULL);
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineCreate"));
    
    TRAP(err, iPlaylistEngine = CMPXPlaylistEngine::NewL(*aObserver));
    
	iLog->Log(_L("Cplaylistenginetest::NewL returned: %d"), err);
    return err;
    }


// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineDestructor
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineDestructor( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    MMPXPlaylistEngineObserver* aObserver(NULL);
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineDestructor"));
    if(iPlaylistEngine==NULL)
    	{
         TRAP( err , iPlaylistEngine = CMPXPlaylistEngine::NewL(*aObserver));
    	}
    delete iPlaylistEngine;
    iPlaylistEngine=NULL;
	iLog->Log(_L("Cplaylistenginetest::Destructor end. "));
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEnginePlaylistPluginHandler
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEnginePlaylistPluginHandler( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEnginePlaylistPluginHandler"));
    
    iPlaylistEngine->PlaylistPluginHandler();
    
	iLog->Log(_L("Cplaylistenginetest::PlaylistPluginHandler end."));
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineIsPlaylistL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineIsPlaylistL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineIsPlaylistL"));
   
    while ( aItem.GetNextString( string ) == KErrNone )
       {
		TBuf<120> KUri;
		KUri.Append(Kplaylistenginetest_testPath);
		KUri.Append(string);
		
	    TInt is=iPlaylistEngine->IsPlaylistL(KUri);
	    
		iLog->Log(_L("Cplaylistenginetest::IsPlaylistL returned: %d"), is);
       }
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistL( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    TPtrC string;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineIsPlaylistL"));
   
    while ( aItem.GetNextString( string ) == KErrNone )
       {
		TBuf<120> KUri;
		KUri.Append(Kplaylistenginetest_testPath);
		KUri.Append(string);
		
	    TRAP(err,iPlaylistEngine->InternalizePlaylistL(KUri));
	    
		iLog->Log(_L("Cplaylistenginetest::IsPlaylistL returned: %d"), err);
       }
    return err;
    }




// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistLRFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistLRFile( CStifItemParser& aItem )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineInternalizePlaylistLRFile"));
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
			TBuf<120> KPlaylistDir;
			KPlaylistDir.Append(Kplaylistenginetest_testPath);
			KPlaylistDir.Append(string);
		    
		    RFs fs;
	        User::LeaveIfError(fs.Connect());
	        CDir* entryList(NULL);
	      //  User::LeaveIfError(
	      //  fs.GetDir(KPlaylistDir, KEntryAttNormal, KEntryAttNormal, entryList));
	        User::LeaveIfError(
    		fs.GetDir(KPlaylistDir, KEntryAttMaskSupported, ESortByName|EDirsFirst, entryList));
	        CleanupStack::PushL(entryList);
	        RFile playlistFile;				
	        User::LeaveIfError(playlistFile.Open(fs, KPlaylistDir, EFileRead ));	
	        iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(playlistFile);
		    TRAP(err, iPlaylistEngine->InternalizePlaylistL(playlistFile));	
		    CleanupStack::PopAndDestroy(entryList);
		    playlistFile.Close();
		    fs.Close();
		   
          }
	iLog->Log(_L("Cplaylistenginetest::InternalizePlaylistL returned: %d"), err);
       
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineExternalizePlaylistL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineExternalizePlaylistL(CStifItemParser& aItem)
    {
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineExternalizePlaylistL testing  ExternalizeL   begin"));
    TInt err=KErrNone;
    TPtrC string;
    
    while ( aItem.GetNextString( string ) == KErrNone )
          {
		   	TBuf<120> KUri;
		   	KUri.Append(Kplaylistenginetest_testPath);
		    CMPXMedia* playlist = CreatePlaylistL(ETitle|EType|ECategory|EArrayContents|EArrayCount|EUri );
		    CleanupStack::PushL(playlist);
		    iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(
		    		        EMPXPlaylistTypeM3U);
		    TRAP(err, iPlaylistEngine->ExternalizePlaylistL(*playlist, KUri); );
		    CleanupStack::PopAndDestroy(playlist);
          }
    iLog->Log(_L("Cplaylistenginetest::ExternalizePlaylistL returned: %d"), err);
    return err;
    }

// ---------------------------------------------------------------------------
//  Create playlist
// ---------------------------------------------------------------------------
//    
CMPXMedia* Cplaylistenginetest::CreatePlaylistL(const TUint& aAttributes)
    {
    ////////////////////////////////////////////////////////////////////////////
    // create playlist
    ////////////////////////////////////////////////////////////////////////////
    CMPXMedia* playlist = CMPXMedia::NewL();
    CleanupStack::PushL(playlist);

    if (aAttributes & ETitle)
        {
        // set playlist title
        playlist->SetTextValueL(KMPXMediaGeneralTitle, _L("Playlist"));        
        }

    if (aAttributes & EType)
        {
        // set type
        playlist->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);        
        }

    if (aAttributes & ECategory)        
        {
        // set category
        playlist->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXPlaylist);         
        }

    if (aAttributes & EArrayContents)
        {
        // set medias
        CMPXMediaArray* medias = CMPXMediaArray::NewL();
        CleanupStack::PushL(medias);        
        playlist->SetCObjectValueL(KMPXMediaArrayContents, medias);
        CleanupStack::PopAndDestroy(medias);
        }

    if (aAttributes & EArrayCount)
        {
        /// set media count        
        playlist->SetTObjectValueL(KMPXMediaArrayCount, 0);        
        }
    if (aAttributes & EUri)
        {
        /// set media uri        
        playlist->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath2);        
        }
    
    CleanupStack::Pop(playlist);
    
    return playlist;
    }    

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistEngineCancelRequests
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistEngineCancelRequests( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistEngineCancelRequests begin:"));
    
    iPlaylistEngine->CancelRequests();
    
	iLog->Log(_L("Cplaylistenginetest::CancelRequests end."));
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistPluginHandlerCreate
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistPluginHandlerCreate( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    iLog->Log(_L("Cplaylistenginetest::CMPXPlaylistPluginHandlerCreate begin"));
    
    RFs fs=RFs();
    User::LeaveIfError(fs.Connect());
    CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet( NULL );	
    CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iAvailableCharacterSet( NULL );
    TRAP( err , iAvailableCharacterSet =  new (ELeave)CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>(12));
    TRAP( err , iTopCharacterSet = new (ELeave)CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>(12));
    
    TRAP( err , iPlaylistPluginHandler=CMPXPlaylistPluginHandler::NewL(*this,*this,fs,*iTopCharacterSet,*iAvailableCharacterSet));
    
    fs.Close();
	iLog->Log(_L("Cplaylistenginetest::NewL returned: %d"), err);
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::CMPXPlaylistPluginHandlerDestructor
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::CMPXPlaylistPluginHandlerDestructor( CStifItemParser& /*aItem*/ )
    {
    TInt err=KErrNone;
    if ( iPlaylistPluginHandler != NULL )
    	{
    	delete iPlaylistPluginHandler;
    	iPlaylistPluginHandler=NULL;
    	}
    
	iLog->Log(_L("Cplaylistenginetest::Destructor end."));
    return err;
    }


// ---------------------------------------------------------------------------
// Cplaylistenginetest::SelectPlaylistPluginLByUri
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::SelectPlaylistPluginLByUri(CStifItemParser& aItem)
    {
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByUri  begin"));
    TInt err=KErrNone;
    TPtrC string;
    
    if( aItem.GetNextString( string ) == KErrNone )
       {
        TBuf<120> KFrom;
        KFrom.Append(Kplaylistenginetest_testPath);
        KFrom.Append(string);
       
        _LIT8(DataType,"playlist/mpegurl");
        TRAP( err , iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(KFrom,KNullDesC8));
        
        iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByUri returned: %d"), err);
       }
    
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::SelectPlaylistPluginLByFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::SelectPlaylistPluginLByFile(CStifItemParser& aItem)
    {
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByFile  begin"));
    TInt err=KErrNone;
    TPtrC string;
        if( aItem.GetNextString( string ) == KErrNone )
           {
            TBuf<120> KFrom;
            KFrom.Append(Kplaylistenginetest_testPath);
            KFrom.Append(string);
            
            RFs fs=RFs();
            RFile file;
            User::LeaveIfError(fs.Connect());
            file.Open(fs, KFrom, EFileShareAny);
            
            
            TRAP( err , iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(file));
          
            file.Close();
            fs.Close();
            iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByFile returned: %d"), err);
           }
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::SelectPlaylistPluginLByType
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::SelectPlaylistPluginLByType(CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByType  begin"));
    
    TInt err=KErrNone;
    
    TRAP( err , iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(EMPXPlaylistTypeM3U));
    
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByType returned: %d"), err);
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::SelectPlaylistPluginLByName
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::SelectPlaylistPluginLByName(CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByName  begin"));
    TInt err=KErrNone;
        
    TRAP( err , iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(_L("m3u playlist")));
    
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByName returned: %d"), err);
    return err;
    }

// ---------------------------------------------------------------------------
// Cplaylistenginetest::SelectPlaylistPluginLByUid
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cplaylistenginetest::SelectPlaylistPluginLByUid(CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByUid  begin"));
    TInt err=KErrNone;
    TRAP( err , iPlaylistEngine->PlaylistPluginHandler().SelectPlaylistPluginL(TUid::Uid(0x101FFC21)));
    
    iLog->Log(_L("Cplaylistenginetest::SelectPlaylistPluginLByUid returned: %d"), err);
    return err;
    }





//  [End of File] - Do not remove
