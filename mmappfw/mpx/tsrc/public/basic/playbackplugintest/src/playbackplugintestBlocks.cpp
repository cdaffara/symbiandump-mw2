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
* Description:  Scripted Tests for MPX Playback Plugin
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <mpxplaybackutility.h>
#include <mpxplaybackmessagedefs.h> 
#include <mpxmessagegeneraldefs.h> 
#include <mpxplaybackmessage.h>
#include "testcommonpluginuids.h"
#include "playbackplugintest.h"

const TUid KPbTestPlugin = {KPlaybackTestPluginImpId};
const TUid KPbFusionVideoHelixPlugin = {0x10282551};
const TUid KPbLocalAudioPlugin = {0x101FFC06};

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// From MMPXPlaybackObserver
// Handle playback message.
// ---------------------------------------------------------------------------
//
void CPlaybackPluginTest::HandlePlaybackMessage( 
        CMPXMessage* aMessage, TInt aError )
    {
    iLog->Log(_L("Cmpxharvestertest::HandlePlaybackMessage: %d"), KTextHandlePlaybackMessage2);
    TMPXMessageId id( *aMessage->Value<TMPXMessageId>( KMPXMessageGeneralId ) );
    TInt event( *aMessage->Value<TInt>( KMPXMessageGeneralEvent ));
    TInt type( *aMessage->Value<TInt>( KMPXMessageGeneralType ) );
    TInt data( *aMessage->Value<TInt>( KMPXMessageGeneralData ) );
    iLog->Log(_L("CPlaybackPluginTest::HandlePlaybackMessage: %d, %d, %d"), event, type, data);
   
    iLog->Log(_L("CPlaybackPluginTest::HandlePlaybackMessage: %d"), aError); 
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CPlaybackPluginTest::Delete() 
    {
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::RunMethodL( CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
                             
        ENTRY( "MMPXPlaybackUtilityNewL", CPlaybackPluginTest::MMPXPlaybackUtilityNewL ),
        ENTRY( "MMPXPlaybackUtilityNewLHelixVideoPlugin", CPlaybackPluginTest::MMPXPlaybackUtilityNewLHelixVideoPlugin ),           
        ENTRY( "MMPXPlaybackUtilityNewLLocalAudioPlugin", CPlaybackPluginTest::MMPXPlaybackUtilityNewLLocalAudioPlugin ),
        ENTRY( "MMPXPlaybackUtilityUtilityL", CPlaybackPluginTest::MMPXPlaybackUtilityUtilityL ),
        ENTRY( "InitStreamingL", CPlaybackPluginTest::InitStreamingL ),
        ENTRY( "Close", CPlaybackPluginTest::Close ),
        ENTRY( "InitStreaming2L", CPlaybackPluginTest::InitStreaming2L ),
        ENTRY( "InitRFileL", CPlaybackPluginTest::InitRFileL ),
        ENTRY( "InitURIL", CPlaybackPluginTest::InitURIL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::MMPXPlaybackUtilityNewL
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::MMPXPlaybackUtilityNewL(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;    
    TRAP(err, 
         iPlaybackUtility = MMPXPlaybackUtility::NewL(KPbModeNewPlayer, this));
    if (err == KErrNone)
        {
        TRAP(err, 
             iPlaybackUtility->PlayerManager().SelectPlayerL(KPbTestPlugin));
        }
    if (err!= KErrNone)
        {
        iLog->Log(_L("CPlaybackPluginTest::MMPXPlaybackUtilityNewL: %d"), err);
        }   
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::MMPXPlaybackUtilityNewLHelixVideoPlugin
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::MMPXPlaybackUtilityNewLHelixVideoPlugin(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;    
    TRAP(err, 
         iPlaybackUtility = MMPXPlaybackUtility::NewL(KPbModeNewPlayer, this));
    
    if (err == KErrNone)
        {
        TRAP(err, 
             iPlaybackUtility->PlayerManager().
                                 SelectPlayerL(KPbFusionVideoHelixPlugin));
        }
    
    if (err!= KErrNone)
        {
        iLog->Log(_L("CPlaybackPluginTest::MMPXPlaybackUtilityNewLHelixVideoPlugin: %d"), err);
        } 
    
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::MMPXPlaybackUtilityNewLLocalAudioPlugin
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::MMPXPlaybackUtilityNewLLocalAudioPlugin(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;    
    TRAP(err, 
         iPlaybackUtility = MMPXPlaybackUtility::NewL(KPbModeNewPlayer, this));
    
    if (err == KErrNone)
        {
        TRAP(err, 
             iPlaybackUtility->PlayerManager().
                                 SelectPlayerL(KPbLocalAudioPlugin));
        }
    
    if (err!= KErrNone)
        {
        iLog->Log(_L("CPlaybackPluginTest::MMPXPlaybackUtilityNewLLocalAudioPlugin: %d"), err);
        } 
    
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::Close
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::Close(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;
    iLog->Log( _L("Entering Close::Starting closing MMPXPlaybackUtility...") );
    if ( iPlaybackUtility )
        {
        iPlaybackUtility->Close();
        iPlaybackUtility = NULL;
        }
    iLog->Log( _L("Getting out of Close.") );
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::MMPXPlaybackUtilityUtilityL
// UtilityL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::MMPXPlaybackUtilityUtilityL(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;    
    TRAP(err,iPlaybackUtility = MMPXPlaybackUtility::UtilityL());
    if (err!= KErrNone)
        {
        iLog->Log(_L("CPlaybackPluginTest::MMPXPlaybackUtilityUtilityL: %d"), err);
        }   
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::InitStreamingL
// InitStreamingL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::InitStreamingL( CStifItemParser& aItem )
    {       
    _LIT8(KTextType, "audio/mpeg");
    TInt err = KErrNone;
    iLog->Log( _L("Entering InitStreamingL---->") );
    TPtrC uri;
    TPtrC FileNamePtr;
    TInt accesspoint = 2;
    while ( aItem.GetNextString( uri ) == KErrNone )
           {
            TBuf<120> KFrom;
            KFrom.Append(KPlaybackPluginTest_digitalPath);
            KFrom.Append(uri);
            TFileName FileName = KFrom;
            iLog->Log( _L("Input file [%S]"), &FileNamePtr );          
            TRAP(err, iPlaybackUtility->InitStreamingL( FileName, &KTextType, accesspoint));
            if( err == KErrNone )
                {
                iLog->Log( _L("Succeeded in initialising streaming.") );
                }
            else
                {
                iLog->Log( _L("Failed to initialise streaming.Error %d returns."),err );
                }
           }
    iLog->Log( _L("<----Ending InitStreamingL") );
    return err;

    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::InitStreaming2L
// InitStreamingL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::InitStreaming2L(CStifItemParser& aItem)
    {
    TInt err = KErrNone;
    iLog->Log( _L("Entering InitStreaming2L---->") );
    TInt accessPoint = 2;
    TPtrC fileFullName; 
    if( aItem.GetNextString( fileFullName ) == KErrNone )
        {
        TBuf<120> KFrom;
        KFrom.Append(KPlaybackPluginTest_digitalPath);
        KFrom.Append(fileFullName);
        TFileName FileName = KFrom;
        RFs   fs;
        RFile file;        
        User::LeaveIfError(fs.Connect());
        fs.ShareProtected();
        User::LeaveIfError(file.Open( fs, FileName, EFileShareReadersOrWriters));
        iLog->Log( _L("Starting initialising streaming...") );
        TRAP(err,iPlaybackUtility->InitStreamingL( file, accessPoint));
        fs.Close();
        if( err == KErrNone )
            {
            iLog->Log( _L("Succeeded in initialising streaming.") );
            }
        else
            {
            iLog->Log( _L("Failed to initialise streaming.Error %d returns."),err );
            }
        }
    else
        {
        err = KErrArgument;
        iLog->Log( _L("Pass in bad parameter.User leaves here.") );
        User::LeaveIfError( err );
        }
    
    iLog->Log( _L("<----Getting out of InitStreaming2L\r\n.") );
    return err;
    }


// -----------------------------------------------------------------------------
// CPlaybackPluginTest::InitRFileL
// InitL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::InitRFileL(CStifItemParser& aItem)
    {
    TInt err = KErrNone;
    iLog->Log( _L("Entering InitRFileL---->") );
    TPtrC fileFullName; 
    
    if( aItem.GetNextString( fileFullName ) == KErrNone )
        {
        TBuf<120> KFrom;
        KFrom.Append(KPlaybackPluginTest_digitalPath);
        KFrom.Append(fileFullName);
        TFileName FileName = KFrom;
        RFs   fs;
        RFile file;        
        User::LeaveIfError(fs.Connect());
        fs.ShareProtected();
        User::LeaveIfError(file.Open( fs, FileName, EFileShareReadersOrWriters));
        iLog->Log( _L("Starting InitRFileL...") );
        TRAP(err, iPlaybackUtility->InitL( file));
        fs.Close(); 
        if( err == KErrNone )
            {
            iLog->Log( _L("Succeeded InitRFileL.") );
            }
        else
            {
            iLog->Log( _L("Failed to InitRFileL. Error %d returns."),err );
            }
        }
    else
        {
        err = KErrArgument;
        iLog->Log( _L("Pass in bad parameter.User leaves here.") );
        User::LeaveIfError( err );
        }
    
    iLog->Log( _L("<----Getting out of InitRFileL\r\n.") );
    return err;
    }

// -----------------------------------------------------------------------------
// CPlaybackPluginTest::InitURIL
// InitL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPlaybackPluginTest::InitURIL(CStifItemParser& aItem)
    {
    TInt err = KErrNone;
    _LIT8(KTextType, "audio/mpeg");
    iLog->Log( _L("Entering InitURIL---->") );
    TPtrC uri;
    TPtrC FileNamePtr;
    while ( aItem.GetNextString( uri ) == KErrNone )
           {
            TBuf<120> KFrom;
            KFrom.Append(KPlaybackPluginTest_digitalPath);
            KFrom.Append(uri);
            TFileName FileName = KFrom;
            iLog->Log( _L("Input file [%S]"), &FileNamePtr );          
            TRAP(err, iPlaybackUtility->InitL( FileName, &KTextType));
            if( err == KErrNone )
                {
                iLog->Log( _L("Succeeded in InitURIL.") );
                }
            else
                {
                iLog->Log( _L("Failed to InitURIL. Error %d returns."),err );
                }
           }
    iLog->Log( _L("<----Getting out of InitURIL\r\n.") );
    return err;
    }

//  [End of File] - Do not remove
