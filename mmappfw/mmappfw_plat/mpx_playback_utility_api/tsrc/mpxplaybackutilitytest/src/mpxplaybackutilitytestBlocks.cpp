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
* Description:  mpxplaybackutilitytest blocks implementation for STIF Test Framework TestScripter.
*
*/



// [INCLUDE FILES] - do not remove
#include <sysutil.h>
#include <s32file.h>
#include <bautils.h> 

#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <mpxvideoplaybackdefs.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcommand.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxplaybackmessage.h>
#include <mpxmediageneraldefs.h>
#include <mpxlog.h>
#include <mpxcollectionplaylist.h>
#include "mpxplaybackutilitytest.h"
#include "mpxplaybackutilitytestdefs.h"
#include "testcommonpluginuids.h"

const TUid KPbTestVideoPlugin = {KPlaybackTestVideoPluginImpId};

// ============================ MEMBER FUNCTIONS ===============================
// ---------------------------------------------------------------------------
// From MMPXPlaybackObserver
// Handle playback message.
// ---------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::HandlePlaybackMessage( CMPXMessage* aMessage, TInt aError )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::HandlePlaybackMessage( CMPXMessage* aMessage, TInt aError )");
    iLog->Log(_L("Cmpxplaybackutilitytest::HandlePlaybackMessage: Error %d"), aError);
    if ( !aError )
    {
        TMPXMessageId id( *(aMessage->Value<TMPXMessageId>(KMPXMessageGeneralId)) );
        TInt event( *aMessage->Value<TInt>( KMPXMessageGeneralEvent ) );
        TInt type( *aMessage->Value<TInt>( KMPXMessageGeneralType ) );
        TInt data( *aMessage->Value<TInt>( KMPXMessageGeneralData ) );

        if ( KMPXMessageGeneral == id )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::HandlePlaybackMessage() General event = %d type = %d  value = %d"),
                         event, type, data );
            RemoveExpectedEventL( KGeneralPlaybackMsgOffset + event, type, data );
        }
        else if ( KMPXMediaIdVideoPlayback == id )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::HandlePlaybackMessage() Video event = %d type = %d  value = %d"),
                         event, type, data );
            RemoveExpectedEventL( KVideoPlaybackMsgOffset + event, type, data );
        }
        else if ( KMPXMessageStif )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::HandlePlaybackMessage() STIF event = %d type = %d  value = %d"),
                         event, type, data );
            RemoveExpectedEventL( KStifPlaybackMsgOffset + event, type, data );
        }
    }
    else
    {
        if ( !iCallbackError )
        {
            iCallbackError = aError;
        }
    }
}

// ---------------------------------------------------------------------------
//  Handle playback property.
// ---------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::HandlePropertyL( TMPXPlaybackProperty aProperty, TInt aValue, TInt aError )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::HandlePropertyL( TMPXPlaybackProperty aProperty, TInt aValue, TInt aError )");
    iLog->Log(_L("Cmpxplaybackutilitytest::HandlePropertyL: Property %d, Value %d, Error %d"), 
            aProperty, aValue, aError);   
}

// ---------------------------------------------------------------------------
//  Method is called continously until aComplete=ETrue, signifying that 
//  it is done and there will be no more callbacks.
//  Only new items are passed each time.
// ---------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::HandleSubPlayerNamesL( TUid aPlayer, 
                                                     const MDesCArray* aSubPlayers,
                                                     TBool aComplete,
                                                     TInt aError )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::HandleSubPlayerNamesL( TUid aPlayer, const MDesCArray* aSubPlayers, TBool aComplete, TInt aError)");
    iLog->Log(_L("Cmpxplaybackutilitytest::HandleSubPlayerNamesL: Complete %d, Error %d"), aComplete, aError);   
}

// ---------------------------------------------------------------------------
//  Call back of media request.
// ---------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::HandleMediaL( const CMPXMedia& aProperties, TInt aError )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::HandleMediaL( const CMPXMedia& aProperties, TInt aError )");
    iLog->Log(_L("Cmpxplaybackutilitytest::HandleMediaL: Error %d"), aError);   
}

// ---------------------------------------------------------------------------
//  Handle completion of a asynchronous command.
//  Note: All clients should implement this callback.
// ---------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::HandlePlaybackCommandComplete( CMPXCommand* aCommandResult, TInt aError )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::HandlePlaybackCommandComplete( CMPXCommand* aCommandResult, TInt aError )");
    iLog->Log(_L("Cmpxplaybackutilitytest::HandlePlaybackCommandComplete: Error %d"), aError);   
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::Delete()
{
    TInt delerr = KErrNone;
    
    iMPXPlaybackUtility->Close();    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    iFile64.Close();
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    iFile.Close();
    
    if ( BaflUtils::FileExists( iFs, iFileName ) )
    {
        User::After( 1000000 * 10 );
        delerr = BaflUtils::DeleteFile(iFs,iFileName);
        iLog->Log(_L("deleting tempfile delerr=%d"),delerr);       
    }    
    iFs.Close();
    iExpectedEventArray->ResetAndDestroy();
      
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::CreateL
// Create here all resources needed for test methods. 
// Called from ConstructL(). 
// -----------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::CreateL()
{
    iFs.Connect();
    iFs.ShareProtected();

    iExpectedEventArray = new (ELeave) CArrayPtrFlat<TMpxPlaybackTestEvent>( 1 );
    iCallbackError = KErrNone;
    iMPXPlaybackUtility = NULL;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::AddExpectedEventL
// -----------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::AddExpectedEventL( TInt aEvent, TInt aType, TInt aData )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::AddExpectedEventL( TInt aEvent, TInt aType, TInt aData )");
    iLog->Log(_L("Cmpxplaybackutilitytest::AddExpectedEventL() event = %d type = %d  value = %d"),
                 aEvent, aType, aData );
    
    TMpxPlaybackTestEvent* event = new (ELeave) TMpxPlaybackTestEvent;
    event->iEvent = aEvent;
    event->iType = aType;
    event->iData = aData;
    iExpectedEventArray->AppendL( event );
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::RemoveExpectedEventL
// -----------------------------------------------------------------------------
//
void Cmpxplaybackutilitytest::RemoveExpectedEventL( TInt aEvent, TInt aType, TInt aData )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::RemoveExpectedEventL( TInt aEvent, TInt aType, TInt aData )");
    iLog->Log(_L("Cmpxplaybackutilitytest::RemoveExpectedEventL() event = %d type = %d  value = %d"),
                 aEvent, aType, aData );
    
    if ( (iExpectedEventArray->Count() > 0) &&
         ((*iExpectedEventArray)[0]->iEvent == aEvent) && ((*iExpectedEventArray)[0]->iType == aType))
    {
        iLog->Log(_L("Cmpxplaybackutilitytest::RemoveExpectedEventL() event = %d removed."), aEvent);
        iExpectedEventArray->Delete( 0 );
    }
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::RunMethodL( CStifItemParser& aItem ) 
{
    static TStifFunctionInfo const KFunctions[] =
    {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 

        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        ENTRY( "MMPXPlaybackUtilityNewL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL ),
        ENTRY( "MMPXPlaybackUtilityNewWithCatL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL ),
        ENTRY( "MMPXPlaybackUtilityUtilityL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL ),
        ENTRY( "MMPXPlaybackUtilityUtilityWithCatL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL ),
        ENTRY( "MMPXPlaybackUtilityInitWithPlaylistL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL ),
        ENTRY( "MMPXPlaybackUtilityInitWithUriL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL ),
        ENTRY( "MMPXPlaybackUtilityInitWithRFileL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL ),
        ENTRY( "MMPXPlaybackUtilityInitStreamingWithUriL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL ),
        ENTRY( "MMPXPlaybackUtilityInitStreamingWithRFileL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL ),
        ENTRY( "MMPXPlaybackUtilityInit64L", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L ),
        ENTRY( "MMPXPlaybackUtilityInitStreaming64L", Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L ),
        ENTRY( "MMPXPlaybackUtilityFile64L", Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L ),
        ENTRY( "MMPXPlaybackUtilityAddObserverL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL ),
        ENTRY( "MMPXPlaybackUtilityRemoveObserverL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL ),
        ENTRY( "MMPXPlaybackUtilityPlayerManagerSelectPlayerL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityPlayerManagerSelectPlayerL ),
        
        ENTRY( "MMPXPlaybackUtilityCommandPlayL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayL ),
        ENTRY( "MMPXPlaybackUtilityCommandStopL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandStopL ),
        ENTRY( "MMPXPlaybackUtilityCommandPauseL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPauseL ),
        ENTRY( "MMPXPlaybackUtilityCommandPlayPauseL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayPauseL ),
        ENTRY( "MMPXPlaybackUtilityCommandPlayCompleteL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayCompleteL ),
        ENTRY( "MMPXPlaybackUtilityCommandSeekForwardL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekForwardL ),
        ENTRY( "MMPXPlaybackUtilityCommandSeekBackwardL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekBackwardL ),
        ENTRY( "MMPXPlaybackUtilityCommandCloseL", Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandCloseL ),
        ENTRY( "MMPXPlaybackUtilitySetL", Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL),
        
        ENTRY( "EndTest", Cmpxplaybackutilitytest::EndTest ),
    };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
   
    return RunInternalL( KFunctions, count, aItem );
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL"));
	TInt err = KErrNone;
    TInt mode;
   
    if ( aItem.GetNextInt(mode) != KErrNone )
    {
        iLog->Log(_L("MMPXPlaybackUtilityNewL - Missing playback mode."));
        err = KErrArgument;
        return err;
    }

    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL: mode %d"), mode);
	TRAP( err,iMPXPlaybackUtility = MMPXPlaybackUtility::NewL(TUid::Uid(mode), this ));
    
	if (err!= KErrNone)
    {
		iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewL: error %d"), err);
    }
	
	return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL"));
    TInt err = KErrNone;
    TInt mode;
    TInt category;
    if ( aItem.GetNextInt(category) != KErrNone )
    {
        iLog->Log(_L("MMPXPlaybackUtilityNewWithCatL - Missing category."));
        err = KErrArgument;
        return err;
    }

    if ( aItem.GetNextInt(mode) != KErrNone )
    {
        iLog->Log(_L("MMPXPlaybackUtilityNewWithCatL - Missing playback mode."));
        err = KErrArgument;
        return err;
    }

    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL: category %d"), category);
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL: mode %d"), mode);
    TRAP( err,iMPXPlaybackUtility = MMPXPlaybackUtility::NewL((TMPXCategory)category, TUid::Uid(mode), this ));
    
    if (err!= KErrNone)
    {
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityNewWithCatL: error %d"), err);
    }
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL
// UtilityL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL"));
	TInt err = KErrNone;
	TInt mode = 0;
	
	aItem.GetNextInt(mode);
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL: mode %d"), mode);
	if ( mode )
    {
	    TRAP(err,iMPXPlaybackUtility = MMPXPlaybackUtility::UtilityL(TUid::Uid(mode)));
    }
	else
    {
        TRAP(err,iMPXPlaybackUtility = MMPXPlaybackUtility::UtilityL());
    }
    
	iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityL: error %d"), err);
	
	return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL
// UtilityL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL"));
    TInt err = KErrNone;
    TInt category;
    TInt mode = 0;
    
    if ( aItem.GetNextInt(category) != KErrNone )
    {
        iLog->Log(_L("MMPXPlaybackUtilityNewWithCatL - Missing category."));
        err = KErrArgument;
        return err;
    }

    aItem.GetNextInt(mode);
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL: category %d"), category);
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL: mode %d"), mode);
    if ( mode )
    {
        TRAP(err,iMPXPlaybackUtility = MMPXPlaybackUtility::UtilityL((TMPXCategory)category, TUid::Uid(mode)));
    }
    else
    {
        TRAP(err,iMPXPlaybackUtility = MMPXPlaybackUtility::UtilityL((TMPXCategory)category));
    }
    
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityUtilityWithCatL: error %d"), err);
    
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL
// InitL(CMPXCollectionPlaylist) test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL( CStifItemParser& )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL"));
    TInt err = KErrNone;
    CMPXCollectionPlaylist* playlist = CMPXCollectionPlaylist::NewL();
    CleanupStack::PushL(playlist);
    TRAP(err,iMPXPlaybackUtility->InitL(*playlist));
    CleanupStack::PopAndDestroy(playlist);
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithPlaylistL - error=%d"),err);
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL
// InitL(Uri) test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL"));
    TInt err = KErrNone; 
    TPtrC string;
    
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();        
    
    if( aItem.GetNextString( string ) == KErrNone )
    {
        //TBuf<120> uri;
        TFileName uri;
        uri.Append(KmpxplaybackutilityTestFilePath);
        uri.Append(string);
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL - uri = %S."), &uri);
        iFileName = uri;

        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile.Create( iFs, uri, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile.Close();
        }
                                     
        TRAP(err,iMPXPlaybackUtility->InitL(uri));
        
        if ( !err )
        {
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
            AddExpectedEventL(event, 0, 0);
            
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
            AddExpectedEventL(event, 0, 0);

            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStateInitialised, 0);
            
            if ( player->UidL() == KPbTestVideoPlugin )
            {
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateBuffering, 0);
            }
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithUriL - error=%d"),err);                  
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInitWithUriL - Missing file name."));
        err = KErrArgument;
    }
    
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL
// Init64L test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL"));
    TInt err = KErrNone;
    TPtrC string;
    
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();
   
    if( aItem.GetNextString( string ) == KErrNone )
    {
        TBuf<120> KFrom;
        KFrom.Append(KmpxplaybackutilityTestFilePath);
        KFrom.Append(string);
        iFileName = KFrom;
        
        if ( iFile.SubSessionHandle() )
        {
            iFile.Close();
        }
        
        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile.Create( iFs, KFrom, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile.Close();
        }
        
        err = iFile.Open(iFs, KFrom, EFileRead | EFileShareReadersOrWriters);
        
        if ( err == KErrNone )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL - Open passed."));
            TRAP(err,iMPXPlaybackUtility->InitL(iFile));
            
            if ( !err )
            {
                TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateInitialised, 0);
                
                if ( player->UidL() == KPbTestVideoPlugin )
                {
                    event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                    AddExpectedEventL(event, EPbStateBuffering, 0);
                }
            }
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitWithRFileL - error=%d"),err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInitWithRFileL - Missing file name."));
        err = KErrArgument;
    }
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL
// InitStreamingL(Uri) test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL"));
    TInt err = KErrNone;
    TPtrC string;
    TInt accessPoint = 1;
   
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();
    
    if( aItem.GetNextString( string ) == KErrNone )
    {
        TBuf<120> uri;
        uri.Append(KmpxplaybackutilityTestFilePath);
        uri.Append(string);
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL - uri = %S."), &uri);
        iFileName = uri;

        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile.Create( iFs, uri, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile.Close();
        }
        
        TRAP(err,iMPXPlaybackUtility->InitStreamingL(uri, NULL, accessPoint));
        
        if ( !err )
        {
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
            AddExpectedEventL(event, 0, 0);
            
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
            AddExpectedEventL(event, 0, 0);
            
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStateInitialised, 0);
            
            if ( player->UidL() == KPbTestVideoPlugin )
            {
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateBuffering, 0);
            }
                        
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithUriL - error=%d"),err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInitStreamingWithUriL - Missing file name."));
        err = KErrArgument;
    }
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL
// InitStreamingL(RFile) test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL"));
    TInt err = KErrNone;
    TPtrC string;
    TInt accessPoint = 1;
    
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();
    
    if( aItem.GetNextString( string ) == KErrNone )
    {
        TBuf<120> KFrom;
        KFrom.Append(KmpxplaybackutilityTestFilePath);
        KFrom.Append(string);
        iFileName = KFrom;
        
        if ( iFile.SubSessionHandle() )
        {
            iFile.Close();
        }
        
        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile.Create( iFs, KFrom, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile.Close();
        }
        
        err = iFile.Open(iFs, KFrom, EFileRead | EFileShareReadersOrWriters);
        if ( err == KErrNone )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL - Open passed."));
            TRAP(err,iMPXPlaybackUtility->InitStreamingL(iFile, accessPoint));

            if ( !err )
            {
                TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
                AddExpectedEventL(event, 0, 0);

                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateInitialised, 0);
            
                if ( player->UidL() == KPbTestVideoPlugin )
                {
                    event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                    AddExpectedEventL(event, EPbStateBuffering, 0);
                }
                                
            }
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreamingWithRFileL - error=%d"),err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInitStreamingWithRFileL - Missing file name."));
        err = KErrArgument;
    }
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L
// Init64L test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L"));
    TInt err = KErrNone;
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    err = KErrNotSupported;
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L testing Init64L() error=%d"),err);
#else // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    TPtrC string;
   
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();
    
    if( aItem.GetNextString( string ) == KErrNone )
    {
        TBuf<120> KFrom;
        KFrom.Append(KmpxplaybackutilityTestFilePath);
        KFrom.Append(string);
        iFileName = KFrom;
        
        if ( iFile64.SubSessionHandle() )
        {
            iFile64.Close();
        }
        
        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile64.Create( iFs, KFrom, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile64.Close();
        }
        
        err = iFile64.Open(iFs, KFrom, EFileRead | EFileShareReadersOrWriters);
        
        if ( err == KErrNone )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L - Open passed."));
            TRAP(err,iMPXPlaybackUtility->Init64L(iFile64));
            
            if ( !err )
            {
                TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
                AddExpectedEventL(event, 0, 0);

                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateInitialised, 0);

                if ( player->UidL() == KPbTestVideoPlugin )
                {
                    event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                    AddExpectedEventL(event, EPbStateBuffering, 0);
                }
                                
            }
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInit64L - error=%d"),err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInit64L - Missing file name."));
        err = KErrArgument;
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L
// InitStreaming64L test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L( CStifItemParser& aItem )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L"));
    TInt err = KErrNone;
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    err = KErrNotSupported;
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L - error=%d"),err);
#else // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    TPtrC string;
    TInt accessPoint = 1;
    
    MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
    MMPXPlayer* player = manager.CurrentPlayer();
    
    if( aItem.GetNextString( string ) == KErrNone )
    {
        TBuf<120> KFrom;
        KFrom.Append(KmpxplaybackutilityTestFilePath);
        KFrom.Append(string);
        iFileName = KFrom;
        
        if ( iFile64.SubSessionHandle() )
        {
            iFile64.Close();
        }
        
        if ( ! BaflUtils::FileExists( iFs, iFileName ) )
        {             
            err = iFile64.Create( iFs, KFrom, EFileShareAny );
            iLog->Log(_L("creating tempfile error=%d"),err);
            iFile64.Close();
        }
        
        err = iFile64.Open(iFs, KFrom, EFileRead | EFileShareReadersOrWriters);
        
        if ( err == KErrNone )
        {
            iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L - Open passed."));
            TRAP(err,iMPXPlaybackUtility->InitStreaming64L(iFile64, accessPoint));

            if ( !err )
            {
                TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPlayerChanged;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EInitializeComplete;
                AddExpectedEventL(event, 0, 0);
                
                event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                AddExpectedEventL(event, EPbStateInitialised, 0);
                
                if ( player->UidL() == KPbTestVideoPlugin )
                {
                    event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
                    AddExpectedEventL(event, EPbStateBuffering, 0);                                
                }
            }
        }
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityInitStreaming64L - error=%d"),err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityInitStreaming64L - Missing file name."));
        err = KErrArgument;
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L
// File64L test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L( CStifItemParser& )");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L"));
    TInt err = KErrNone;
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    err = KErrNotSupported;
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L - error=%d"),err);
#else // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    RFile64* file64Ptr=NULL;
    TRAP(err, file64Ptr=iMPXPlaybackUtility->Source()->File64L());
    if ( !file64Ptr->SubSessionHandle() )
    {
        iLog->Log(_L("MMPXPlaybackUtilityFile64L - file64 = NULL."));
    }
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityFile64L - error=%d"),err);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL
// AddObserverL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL( CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL"));
    TInt err = KErrNone;
    TRAP(err, iMPXPlaybackUtility->AddObserverL(*this));
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityAddObserverL - error=%d"),err);
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL
// RemoveObserverL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL( CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL"));
    TInt err = KErrNone;
    TRAP(err, iMPXPlaybackUtility->RemoveObserverL(*this));
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityRemoveObserverL - error=%d"),err);
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityPlayerManagerSelectPlayerL
// RemoveObserverL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityPlayerManagerSelectPlayerL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityPlayerManagerSelectPlayerL(CStifItemParser& aItem)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityPlayerManagerSelectPlayerL"));
    TInt uidInt;
    TInt err = KErrNone;
    
    // read in UID
    if ( aItem.GetNextInt(uidInt) != KErrNone )
    {
        iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL - Missing UID."));
        err = KErrArgument;
        return err;
    }

    iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL - UID = 0x%x."), uidInt);
    if ( iMPXPlaybackUtility )
    {
        MMPXPlayerManager& manager = iMPXPlaybackUtility->PlayerManager();
        
        if (uidInt == 0x10282551)
        {
            iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL video uid"));    
            MPX_DEBUG2(("MMPXPlaybackUtilityPlayerManagerSelectPlayerL - uidInt = %d."), uidInt);
            TRAP( err, manager.SelectPlayerL( KPbTestVideoPlugin ) );
        }
        else 
        {
            iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL NOT video uid"));    
            MPX_DEBUG1(("MMPXPlaybackUtilityPlayerManagerSelectPlayerL NOT video uid"));
            TRAP( err, manager.SelectPlayerL( TUid::Uid(uidInt) ) );
        }
        iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL - SelectPlayer err = %d."), err);
    }
    else
    {
        iLog->Log(_L("MMPXPlaybackUtilityPlayerManagerSelectPlayerL - MPX Playback Utility not created."));
        err = KErrGeneral;
    }
    return err;
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayL")); 
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, EPbCmdPlay);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
    
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandPlayL err from commandl: , err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandPlayL err from commandl: , err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandPlayL no error from commandl:"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandPlayL no err from commandl:"));   
     
        TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
        
        if ( s != EPbStatePlaying )
        {
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;        
            AddExpectedEventL(event, EPbStatePlaying, 0);
        }
    }
        
    CleanupStack::PopAndDestroy (cmd);
    
    return err;
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandStopL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandStopL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandStopL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandStopL")); 
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, EPbCmdStop);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
    
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandStopL err from commandl: , err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandStopL err from commandl: , err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandStopL no error from commandl:"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandStopL no err from commandl:"));   
        
        TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
        
        if ( s != EPbStateStopped )
        {
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStateStopped, 0);
        }
                
    }
        
    CleanupStack::PopAndDestroy (cmd);
    
    return err;          
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPauseL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPauseL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPauseL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPauseL")); 
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, EPbCmdPause);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
    
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandPauseL err from commandl: , err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandPauseL err from commandl: , err = %d."), err);
    
    if ( !err )
    {    
        iLog->Log(_L("MMPXPlaybackUtilityCommandPauseL no error from commandl:"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandPauseL no err from commandl:"));   
        
        TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
        
        if ( s == EPbStatePlaying )
        {
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePaused, 0);
        }
                
    }
        
    CleanupStack::PopAndDestroy (cmd);    
    
    return err;               
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayPauseL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayPauseL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayPauseL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayPauseL")); 
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, EPbCmdPlayPause);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
    
    TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandPlayPauseL err from commandl: err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandPlayPauseL err from commandl: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandPlayPauseL no error from commandl"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandPlayPauseL no error from commandl"));
        
        
        
        if (s == EPbStatePlaying)
        {
            iLog->Log(_L("MMPXPlaybackUtilityCommandPlayPauseL EPbStatePlaying:"));
            MPX_DEBUG1(("MMPXPlaybackUtilityCommandPlayPauseL EPbStatePlaying:")); 
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePaused, 0);
        }
        else
        {
            iLog->Log(_L("MMPXPlaybackUtilityCommandPlayPauseL State is not playing:"));
            MPX_DEBUG1(("MMPXPlaybackUtilityCommandPlayPauseL State is not playing:")); 
            TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePlaying, 0);
        }
    }
        
    CleanupStack::PopAndDestroy (cmd);
    
    return err;               
}

// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayCompleteL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayCompleteL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayCompleteL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandPlayCompleteL")); 
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXMessageStif);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
    
    cmd->SetTObjectValueL<TMPXStifCommand>( KMPXStifPlaybackCommand,
                                             EPbStifPlayComplete );
        
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));    
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandPlayCompleteL error from commandl: err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandPlayCompleteL error from commandl: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandPlayCompleteL no error from commandl"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandPlayCompleteL no error from commandl"));
        
        TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPropertyChanged;
        AddExpectedEventL(event, EPbPropertyPosition, 0);
        
        event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
        AddExpectedEventL(event, EPbStateStopped, 0);            
    }
    
    CleanupStack::PopAndDestroy (cmd);
    
    return err;
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekForwardL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekForwardL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekForwardL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekForwardL")); 
    
    TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXMessageStif);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TMPXStifCommand>( KMPXStifPlaybackCommand,
                                             EPbStifSeekForward );
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, s);
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));

    iLog->Log(_L("MMPXPlaybackUtilityCommandSeekForwardL error from commandl: err = %d"), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandSeekForwardL error from commandl: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandSeekForwardL no error from commandl"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandSeekForwardL no error from commandl"));
        
        TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
        AddExpectedEventL(event, EPbStatePluginSeeking, 0);
        
        event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPropertyChanged;
        AddExpectedEventL(event, EPbPropertyPosition, 0);
        
        if ( s == EPbStatePlaying )
        {
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePlaying, 0);
        }
        else if ( s == EPbStatePaused )
        {
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePaused, 0);    
        }
    }
    CleanupStack::PopAndDestroy (cmd);
    
    return err;
}



// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekBackwardL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekBackwardL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekBackwardL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandSeekBackwardL")); 
    
    TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXMessageStif);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TMPXStifCommand>( KMPXStifPlaybackCommand,
                                             EPbStifSeekBackward );
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, s);
    
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));

    iLog->Log(_L("MMPXPlaybackUtilityCommandSeekBackwardL error from commandl: err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandSeekBackwardL error from commandl: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandSeekForwardL no error from commandl"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandSeekForwardL no error from commandl"));
        
        TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
        AddExpectedEventL(event, EPbStatePluginSeeking, 0);
        
        event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPropertyChanged;
        AddExpectedEventL(event, EPbPropertyPosition, 0);
        
        if ( s == EPbStatePlaying )
        {
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePlaying, 0);
        }
        else if ( s == EPbStatePaused )
        {
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStatePaused, 0);    
        }        
    }
    
    CleanupStack::PopAndDestroy (cmd);
       
    return err;
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandCloseL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandCloseL( CStifItemParser& /*aItem*/ )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandCloseL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilityCommandCloseL")); 
    
    TMPXPlaybackState s = iMPXPlaybackUtility->StateL();
    
    //create command
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandIdPlaybackGeneral);
    cmd->SetTObjectValueL<TBool>(KMPXCommandGeneralDoSync, ETrue);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralType, EPbCmdClose);
    cmd->SetTObjectValueL<TInt>(KMPXCommandPlaybackGeneralData, 0);
        
    TRAPD(err,iMPXPlaybackUtility->CommandL( *cmd ));
    
    iLog->Log(_L("MMPXPlaybackUtilityCommandCloseL err from commandl: err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilityCommandCloseL err from commandl: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilityCommandCloseL no error from commandl:"));
        MPX_DEBUG1(("MMPXPlaybackUtilityCommandCloseL no err from commandl:")); 
        
        TInt event;
        
        if ( s != EPbStateStopped )
        {
            event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EStateChanged;
            AddExpectedEventL(event, EPbStateStopped, 0);
        }                    
    }
        
    CleanupStack::PopAndDestroy (cmd);
    
    return err;                
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL( CStifItemParser& aItem )
{
    MPX_FUNC_EX("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL(CStifItemParser&)");
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL")); 
    
    TInt err = KErrNone;
    TInt property;
    TInt value;
    

    if( aItem.GetNextInt( property ) != KErrNone )
    {
        MPX_DEBUG1(("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL missing property " ));
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL missing property " ));
        err = KErrArgument;
        return err;
    }

    if ( aItem.GetNextInt( value ) != KErrNone)
    {
        MPX_DEBUG1(("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL missing value " ));
        iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL missing value " ));
        err = KErrArgument;
        return err;
    }

    MPX_DEBUG3(("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL property = %d, value = %d"), property, value );
    iLog->Log(_L("Cmpxplaybackutilitytest::MMPXPlaybackUtilitySetL property = %d, value %d") , property, value );

    TRAP(err, iMPXPlaybackUtility->SetL((TMPXPlaybackProperty)property, value ));    
    
    iLog->Log(_L("MMPXPlaybackUtilitySetL err from SetL: err = %d."), err);
    MPX_DEBUG2(("MMPXPlaybackUtilitySetL err from SetL: err = %d."), err);
    
    if ( !err )
    {
        iLog->Log(_L("MMPXPlaybackUtilitySetL no error from SetL"));
        MPX_DEBUG1(("MMPXPlaybackUtilitySetL no err from SetL")); 
        
        TInt event = KGeneralPlaybackMsgOffset + TMPXPlaybackMessage::EPropertyChanged;
        AddExpectedEventL(event, property, value);
    }
    
    return err;                
}


// -----------------------------------------------------------------------------
// Cmpxplaybackutilitytest::EndTest
// -----------------------------------------------------------------------------
//
TInt Cmpxplaybackutilitytest::EndTest( CStifItemParser& /*aItem*/ )
{
    iLog->Log(_L("Cmpxplaybackutilitytest::EndTest"));
    TInt err = iCallbackError;
    
    // check if event queue is empty
    if ( !err && (iExpectedEventArray->Count() > 0 ) ) 
    {
        iLog->Log(_L("Cmpxplaybackutilitytest::EndTest error = KErrTimedOut"));
        err = KErrTimedOut;
    }
    
    return err;
}

// end of file
