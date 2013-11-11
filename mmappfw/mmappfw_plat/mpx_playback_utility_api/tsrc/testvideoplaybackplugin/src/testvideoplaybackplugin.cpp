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
* Description:  CTestVideoPlaybackPlugin implementation
*
*/

//
//  INCLUDE FILES
//
#include <apgcli.h>
#include <e32cmn.h>
#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxvideoplaybackdefs.h>
#include <mpxplaybackpluginobserver.h>
#include <mpxplaybackplugin.h>
#include <mpxlog.h>
#include <mpxmedia.h>

#include "testvideoplaybackplugin.h"
#include "mpxplaybackutilitytestdefs.h"
#include "mpxplaybackutilitytest.h"


//
//  CONSTANTS
//
const TUid KLocalPlaybackUid = { 0x10282556 };


// ============================ MEMBER FUNCTIONS ===============================

//  ----------------------------------------------------------------------------
//    Two-phased constructor.
//  ----------------------------------------------------------------------------
//
CTestVideoPlaybackPlugin* CTestVideoPlaybackPlugin::NewL( TAny* /*aInitParams*/ )
{
    
    CTestVideoPlaybackPlugin* p = new (ELeave) CTestVideoPlaybackPlugin();
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    return p;
}

//  ----------------------------------------------------------------------------
//    Symbian 2nd phase constructor can leave.
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::ConstructL()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::ConstructL()");
    User::LeaveIfError( iFs.Connect() );
    iFs.ShareProtected();
    
    iEventArray = new (ELeave) CArrayPtrFlat<TTestVideoPlaybackCallbackEvent>( 1 );
    iCallback   = CIdle::NewL( CActive::EPriorityLow );
}

//  ----------------------------------------------------------------------------
//    C++ constructor
//  ----------------------------------------------------------------------------
//
CTestVideoPlaybackPlugin::CTestVideoPlaybackPlugin() 

{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::CTestVideoPlaybackPlugin()");
}

//  ----------------------------------------------------------------------------
//    Destructor
//  ----------------------------------------------------------------------------
//
CTestVideoPlaybackPlugin::~CTestVideoPlaybackPlugin()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::~CTestVideoPlaybackPlugin()");
    iFile.Close();
    iFs.Close();
    
    if ( iCallback->IsActive() )
    {
        iCallback->Cancel();
    }

    delete iCallback;

    iEventArray->ResetAndDestroy();
}

//  ----------------------------------------------------------------------------
//    Set observer
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::SetObserver( MMPXPlaybackPluginObserver& aObs )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::SetObserver( MMPXPlaybackPluginObserver& aObs )");
    iObs = &aObs;
}

//  ----------------------------------------------------------------------------
//    Initializes a clip for playback from a file name
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::InitialiseL( const TDesC& aSong )
{
    
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitialiseL( const TDesc& aSong)");
    
	delete iClipName;
    iClipName = NULL;
    iClipName = aSong.AllocL();

    iFile.Close();

    TInt err = iFile.Open( iFs, aSong, EFileRead | EFileShareReadersOrWriters );

    //
    //  Remap KErrNotReady to KErrNotFound, because it is referencing a drive
    //  that is not existent
    //
    if ( KErrNotReady == err )
    {
        err = KErrNotFound;
    }

    // if aSong is an streaming link and contains one of the streaming schemas
    // eg. rtsp:// , http:// etc. then a file handle can not be opened
    // ignore KErrBadName
    if (err != KErrBadName)
    {
        User::LeaveIfError( err );    
    }    

    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );
    
    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}

//  ----------------------------------------------------------------------------
//    Initializes a clip for playback from a file handle
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::InitialiseL( RFile& aSong )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitialiseL( RFile& aSong )");
    
    delete iClipName;
    iClipName = NULL;
    iClipName = HBufC::NewL( KMaxFileName );
    TPtr ptr = iClipName->Des();
    aSong.FullName( ptr );

    iFile.Close();
    User::LeaveIfError( iFile.Duplicate( aSong ));
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );

    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );        
}

/**
* Initializes a file for playback.
*
* @since S60 9.2
* @param aUri URI of the item
* @param aType the mime type of the item
* @param aAccessPoint the access point
*/
void CTestVideoPlaybackPlugin::InitStreamingL(const TDesC& /*aUri*/, 
        const TDesC8& /*aType*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitStreamingL(const TDesC& /*aUri*/, const TDesC8& /*aType*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)");
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );

    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}

/**
* Initializes a file handle for playback.
*
* @since S60 9.2
* @param aFile file handle of a file
* @param aAccessPoint the access point
*/
void CTestVideoPlaybackPlugin::InitStreamingL(RFile& /*aFile*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitStreamingL(RFile& /*aFile*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)");
        
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );    
    
    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
/**
* Initializes a file handle for playback.
*
* @since S60 9.2
* @param aFile 64 bit file handle of a file
* @param aAccessPoint the access point
*/
void CTestVideoPlaybackPlugin::InitStreaming64L(RFile64& /*aFile*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitStreaming64L(RFile64& /*aFile*/, TInt /*aAccessPoint*/, TInt /*aPosition*/)");
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );

    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}

/**
* Initializes a song for playback.
*
* @since S60 9.2
* @param aFile 64 bit file handle of a song
*/
void CTestVideoPlaybackPlugin::Initialise64L(RFile64& /*aSong*/, TInt /*aPosition*/)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::Initialise64L(RFile64& /*aSong*/, TInt /*aPosition*/)");
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );

    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

//  ----------------------------------------------------------------------------
//    Executes a command on the selected song
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::CommandL( CMPXCommand& aCmd )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::CommandL( CMPXCommand& aCmd )");
    
    if ( aCmd.IsSupported( KMPXStifPlaybackCommand ) )
    {
       TMPXStifCommand cmd = static_cast<TMPXStifCommand>(aCmd.ValueTObjectL<TInt>(KMPXStifPlaybackCommand));
       TMPXPlaybackState state = static_cast<TMPXPlaybackState>(aCmd.ValueTObjectL<TInt>(KMPXCommandPlaybackGeneralData));
       
       MPX_DEBUG3("CTestVideoPlaybackPlugin::CommandL cmd = %d, state = %d ", cmd, state);
       
       switch ( cmd )
       {
           case EPbStifPlayComplete:
           {
               MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbStifPlaybackComplete");

               TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

               event->iEvent = MMPXPlaybackPluginObserver::EPPlayComplete;
               event->iData = 0;
               event->iError = KErrNone;
                   
               AddCallbackEvent( event );

               break;
           }
           case EPbStifSeekForward:
           {
               MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbStifSeekForward");
 
               TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

               event->iEvent = MMPXPlaybackPluginObserver::EPPluginSeeking;
               event->iData = 0;
               event->iError = KErrNone;
                   
               AddCallbackEvent( event );
               
               event = new TTestVideoPlaybackCallbackEvent;
               event->iEvent = MMPXPlaybackPluginObserver::EPPositionChanged;
               event->iData = 0;
               event->iError = KErrNone;
                  
               AddCallbackEvent( event );
               
               if ( state == EPbStatePlaying )
               {

                   event = new TTestVideoPlaybackCallbackEvent;
                   event->iEvent = MMPXPlaybackPluginObserver::EPPlaying;
                   event->iData = 0;
                   event->iError = KErrNone;
                      
                   AddCallbackEvent( event );               
               }
               else if ( state == EPbStatePaused )
               {

                   event = new TTestVideoPlaybackCallbackEvent;
                   event->iEvent = MMPXPlaybackPluginObserver::EPPaused;
                   event->iData = 0;
                   event->iError = KErrNone;
                      
                   AddCallbackEvent( event );
                                          
               }
               break;
           }
                   
           case EPbStifSeekBackward:
           {
              MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbStifSeekBackward");

              TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
              
              event->iEvent = MMPXPlaybackPluginObserver::EPPluginSeeking;
              event->iData = 0;
              event->iError = KErrNone;
                  
              AddCallbackEvent( event );
              
              event = new TTestVideoPlaybackCallbackEvent;
              event->iEvent = MMPXPlaybackPluginObserver::EPPositionChanged;
              event->iData = 0;
              event->iError = KErrNone;
                 
              AddCallbackEvent( event );
              
              if ( state == EPbStatePlaying )
              {

                  event = new TTestVideoPlaybackCallbackEvent;
                  event->iEvent = MMPXPlaybackPluginObserver::EPPlaying;
                  event->iData = 0;
                  event->iError = KErrNone;
                     
                  AddCallbackEvent( event );            
              }
              else if ( state == EPbStatePaused )
              {

                  event = new TTestVideoPlaybackCallbackEvent;
                  event->iEvent = MMPXPlaybackPluginObserver::EPPaused;
                  event->iData = 0;
                  event->iError = KErrNone;
                     
                  AddCallbackEvent( event );
              }
              break;
           }
           
           default:
           {
               MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd default");
               break;
           }
       }
   }       
}


//  ----------------------------------------------------------------------------
//    Executes a command on the selected song
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::CommandL(TMPXPlaybackCommand aCmd, TInt /*aData*/)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::CommandL(TMPXPlaybackCommand aCmd, TInt /*aData*/)");
    
    MPX_DEBUG2("CTestVideoPlaybackPlugin::CommandL aCmd = %d", aCmd);

    switch (aCmd)
    {            
        case EPbCmdPlay:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbCmdPlay");

            TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

            event->iEvent = MMPXPlaybackPluginObserver::EPPlaying;
            event->iData = 0;
            event->iError = KErrNone;
                
            AddCallbackEvent( event );
            
            break;
        }
            
        case EPbCmdClose:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbCmdClose");

            TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
            
            event->iEvent = MMPXPlaybackPluginObserver::EPClosed;
            event->iData = 0;
            event->iError = KErrNone;
                
            AddCallbackEvent( event );

            break;
        }
           
        case EPbCmdStop:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbCmdStop");

            TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

            event->iEvent = MMPXPlaybackPluginObserver::EPStopped;
            event->iData = 0;
            event->iError = KErrNone;
                
            AddCallbackEvent( event );

            break;
        }
            
        case EPbCmdPause:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbCmdPause");

            TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

            event->iEvent = MMPXPlaybackPluginObserver::EPPaused;
            event->iData = 0;
            event->iError = KErrNone;
                
            AddCallbackEvent( event );
            
            break;
        }
            
        case EPbCmdStartSeekForward:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd is EPbCmdStartSeekForward");

            TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
            
            event->iEvent = MMPXPlaybackPluginObserver::EPPluginSeeking;
            event->iData = 0;
            event->iError = KErrNone;
                
            AddCallbackEvent( event );

            event = new TTestVideoPlaybackCallbackEvent;
            event->iEvent = MMPXPlaybackPluginObserver::EPPositionChanged;
            event->iData = 0;
            event->iError = KErrNone;

            AddCallbackEvent( event );
            
            event = new TTestVideoPlaybackCallbackEvent;
            event->iEvent = MMPXPlaybackPluginObserver::EPPlaying;
            event->iData = 0;
            event->iError = KErrNone;

            AddCallbackEvent( event );
            
            break;
        }
                
        default:
        {
            MPX_DEBUG1("CTestVideoPlaybackPlugin::CommandL aCmd default");
            break;
        }
    }
}


//  ----------------------------------------------------------------------------
//    Sets a property of the plugin
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::SetL( TMPXPlaybackProperty aProperty, TInt aValue )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::SetL( TMPXPlaybackProperty /*aProperty*/, TInt /*aValue*/ )");
    MPX_DEBUG3("CTestVideoPlaybackPlugin::SetL aProperty = %d, aValue = %d", aProperty, aValue);
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;

    event->iEvent = MMPXPlaybackPluginObserver::EPSetComplete;
    event->iData = aProperty;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );    
}

//  ----------------------------------------------------------------------------
//    Gets a property of the plugin (async)
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::PropertyL( TMPXPlaybackProperty /*aProperty*/ ) const
{

}

//  ----------------------------------------------------------------------------
//    Gets a list of sub players, UPnP only
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::SubPlayerNamesL()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::SubPlayerNamesL()");
    
    iObs->HandleSubPlayerNames( KLocalPlaybackUid, NULL, ETrue, KErrNone );
}

//  ----------------------------------------------------------------------------
//    Select a sub player
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::SelectSubPlayerL( TInt /*aIndex*/ )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::SelectSubPlayerL( TInt aIndex )");
    
    User::Leave( KErrNotSupported );
}

//  ----------------------------------------------------------------------------
//    Returns current sub player name
//  ----------------------------------------------------------------------------
//
const TDesC& CTestVideoPlaybackPlugin::SubPlayerName()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::SubPlayerName()");
    
    return KNullDesC;
}

//  ----------------------------------------------------------------------------
//    Current sub player index
//  ----------------------------------------------------------------------------
//
TInt CTestVideoPlaybackPlugin::SubPlayerIndex() const
{
     
    return KErrNotFound;
}

//  ----------------------------------------------------------------------------
//    Gets media properties
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::MediaL( const TArray<TMPXAttribute>& /*aAttrs*/ )
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::MediaL( const TArray TMPXAttribute )");
}

//  ----------------------------------------------------------------------------
//    Cancel request
//  ----------------------------------------------------------------------------
//
void CTestVideoPlaybackPlugin::CancelRequest()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::CancelRequest()");
}

//  ----------------------------------------------------------------------------
//  CTestVideoPlaybackPlugin::GetFileHandle()
//  ----------------------------------------------------------------------------
//
RFile CTestVideoPlaybackPlugin::GetFileHandle()
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::GetFileHandle()");
    
    return iFile;
}

/**
* Initializes a song for playback.
*
* @since S60 9.2
* @param aSong the song path
* @param aPosition the starting position
*/
void CTestVideoPlaybackPlugin::InitialiseWithPositionL(const TDesC& aSong, TInt aPosition)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitialiseWithPositionL(const TDesC& /*aSong*/, TInt /*aPosition*/)");
    
    delete iClipName;
    iClipName = NULL;
    iClipName = aSong.AllocL();

    iFile.Close();

    TInt err = iFile.Open( iFs, aSong, EFileRead | EFileShareReadersOrWriters );

    //
    //  Remap KErrNotReady to KErrNotFound, because it is referencing a drive
    //  that is not existent
    //
    if ( KErrNotReady == err )
    {
        err = KErrNotFound;
    }

    // if aSong is an streaming link and contains one of the streaming schemas
    // eg. rtsp:// , http:// etc. then a file handle can not be opened
    // ignore KErrBadName
    if (err != KErrBadName)
    {
        User::LeaveIfError( err );    
    }            
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );    
    
    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
        
    AddCallbackEvent( event );
}

/**
* Initializes a song for playback.
*
* @since S60 9.2
* @param aFile file handle of a song
* @param aPosition the starting position
*/
void CTestVideoPlaybackPlugin::InitialiseWithPositionL(RFile& aSong, TInt aPosition)
{
    MPX_FUNC_EX("CTestVideoPlaybackPlugin::InitialiseWithPositionL(RFile& aSong, TInt aPosition)");

    delete iClipName;
    iClipName = NULL;
    iClipName = HBufC::NewL( KMaxFileName );
    TPtr ptr = iClipName->Des();
    aSong.FullName( ptr );

    iFile.Close();
    User::LeaveIfError( iFile.Duplicate( aSong ));
    
    
    TTestVideoPlaybackCallbackEvent* event = new (ELeave) TTestVideoPlaybackCallbackEvent;
    
    event->iEvent = MMPXPlaybackPluginObserver::EPInitialised;
    event->iData = 0;
    event->iError = KErrNone;
    
    AddCallbackEvent( event );

    event = new TTestVideoPlaybackCallbackEvent;
    event->iEvent = MMPXPlaybackPluginObserver::EPBufferingStarted;
    event->iData = 0;
    event->iError = KErrNone;
            
    AddCallbackEvent( event );
}


void CTestVideoPlaybackPlugin::RunL()
{
    MPX_FUNC_EX("CTestVideoPaybackPlugin::RunL");   
}

void CTestVideoPlaybackPlugin::DoCancel()
{
    MPX_FUNC_EX("CTestVideoPaybackPlugin::DoCancel");
}

void CTestVideoPlaybackPlugin::AddCallbackEvent( TTestVideoPlaybackCallbackEvent* event )
{      
    MPX_DEBUG1("CTestVideoPlaybackPlugin::AddCallbackEvent");
    
    iEventArray->AppendL( event );

    if ( ! iCallback->IsActive() )
    {
        iCallback->Start( TCallBack( CTestVideoPlaybackPlugin::SendEvent, this ) );
    }
}

TInt CTestVideoPlaybackPlugin::SendEvent (TAny* aPtr )
{
    MPX_DEBUG1("CTestVideoPlaybackPlugin::SendEvent");
    
    static_cast<CTestVideoPlaybackPlugin*>(aPtr)->DoSendEvent();

    return KErrNone;
}

void CTestVideoPlaybackPlugin::DoSendEvent()
{
    MPX_DEBUG1("-->CTestVideoPlaybackPlugin::DoSendEvent");

    TInt count = iEventArray->Count();

    if ( count > 0 )
    {
    TTestVideoPlaybackCallbackEvent* event = (*iEventArray)[0];
        
    MMPXPlaybackPluginObserver::TEvent myevent = static_cast<MMPXPlaybackPluginObserver::TEvent>(event->iEvent);
    
        iObs->HandlePluginEvent( myevent, event->iData, event->iError);

        if ( count > 1 )
        {
            //
            //  More events exist, start another callback
            //
            MPX_DEBUG1("CTestVideoPlaybackPlugin::DoSendEvent - there are more events, start another callback");
            iCallback->Start( TCallBack( CTestVideoPlaybackPlugin::SendEvent, this ) );
        }

        iEventArray->Delete( 0 );
    }
    MPX_DEBUG1("<--CTestVideoPlaybackPlugin::DoSendEvent");
}

// End of file
