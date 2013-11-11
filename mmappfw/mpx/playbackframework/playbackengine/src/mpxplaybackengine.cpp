/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Controls playback via plug-ins
*
*/


#include <featmgr.h>
#include <mpxprivatecrkeys.h>
#include <mpxlog.h>
#include <mpxtaskqueue.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmedia.h>
#include <mpxcommonframeworkdefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxplaybackcommanddefs.h>
#include <mpxcollectionpath.h>
#include <mpxattributespecs.h>
#include <mpxmessagepluginupdatedefs.h>
#include <mpxplaybackpluginversion2.h>
#include <mpxcmn.h>
#include <caf/caferr.h>
#include <mpxmediageneralextdefs.h>

#include "mpxplaybacksettings.h"
#include "mpxautoresumehandler.h"
#include "mpxplaybackcommandbuffer.h"
#include "mpxplaybackactiveengineobserver.h"
#include "mpxplaybackinitializer.h"
#include "mpxplaybackengine.h"
#include "mpxplaybackmediahelper.h"
#include "mpxplaybackdummymediaobserver.h"

// CONSTANTS
const TInt KMPXSmallVolumeIncrement = 5;
const TInt KMPXLargeVolumeIncrement = 10;
const TInt KPercentMultiplier = 100;
const TInt KMPXSyncMsgTimer = 3000000; // 3 seconds
_LIT(KWmaExtension, ".wma");
_LIT(KRaExtension, ".ra");


// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaybackEngine* CMPXPlaybackEngine::NewL(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackActiveEngineObserver& aObserver,
    MMPXClientlistObserver* aClientListObserver,
    const TUid& aModeId)
    {
    CMPXPlaybackEngine* p=new(ELeave)CMPXPlaybackEngine(aPluginMonitor,
                                                        aObserver, aModeId);
    CleanupStack::PushL(p);
    p->ConstructL(aClientListObserver);
    CleanupStack::Pop(p);
    return p;
    }


// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaybackEngine* CMPXPlaybackEngine::NewL(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackActiveEngineObserver& aObserver,
    MMPXClientlistObserver* aClientListObserver,
    const TUid& aModeId,
    const TInt aCategory)
    {
    CMPXPlaybackEngine* p=new(ELeave)CMPXPlaybackEngine(aPluginMonitor,
                                                        aObserver, aModeId, aCategory);
    CleanupStack::PushL(p);
    p->ConstructL(aClientListObserver);
    CleanupStack::Pop(p);
    return p;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackEngine::CMPXPlaybackEngine(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackActiveEngineObserver& aObserver,
    const TUid& aModeId)
:   iReflector(*this),
    iPluginMonitor(aPluginMonitor),
    iItemId(KMPXInvalidItemId),
    iPlaySource(EPlayNone),
    iAccessPoint(0),
    iAccessPointSet(EFalse),
    iState(EPbStateNotInitialised),
    iNextState(EPbStateNotInitialised),
    iPluginState(EPbStateNotInitialised),
    iCategory(EMPXCategoryUndefined),
    iModeId(aModeId),
    iObserver(aObserver),
    iPreservedState( EPbStateNotInitialised ),
    iPreservedPosition( KErrNotFound ),
    iSkipping(EFalse),
    iPluginUid(KNullUid),
    iLastActiveProcess(KNullProcessId),
    iLastInactiveProcess(KNullProcessId),
    iPositionFromMedia( KErrNotFound ) 
    {
    iProperties[EPbPropertyVolumeRamp]=KPbFadeInDurationMicroSeconds;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPlaybackEngine::CMPXPlaybackEngine(
    CMPXPluginMonitor& aPluginMonitor,
    MMPXPlaybackActiveEngineObserver& aObserver,
    const TUid& aModeId,
    const TInt aCategory)
:   iReflector(*this),
    iPluginMonitor(aPluginMonitor),
    iItemId(KMPXInvalidItemId),
    iPlaySource(EPlayNone),
    iAccessPoint(0),
    iAccessPointSet(EFalse),
    iState(EPbStateNotInitialised),
    iNextState(EPbStateNotInitialised),
    iPluginState(EPbStateNotInitialised),
    iModeId(aModeId),
    iCategory(aCategory),
    iObserver(aObserver),
    iPreservedState( EPbStateNotInitialised ),
    iPreservedPosition( KErrNotFound ),
    iSkipping(EFalse),
    iPluginUid(KNullUid),
    iLastActiveProcess(KNullProcessId),
    iLastInactiveProcess(KNullProcessId),
    iPositionFromMedia( KErrNotFound ) 
    {
    iProperties[EPbPropertyVolumeRamp]=KPbFadeInDurationMicroSeconds;
    }


// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::ConstructL(MMPXClientlistObserver* aClientListObserver)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::ConstructL");
    iClientList=CMPXClientList::NewL(aClientListObserver);
    iPluginHandler=CMPXPlaybackPluginHandler::NewL(iPluginMonitor, *this, *this);
    iCmdBuffer=CMPXPlaybackCmdBuffer::NewL(*this);
    iProgressTimer=CHeartbeat::NewL(CActive::EPriorityStandard);
    iSeekTimer=CPeriodic::NewL(CActive::EPriorityStandard);
    iAccessoryObs=CMPXAccessoryObserver::NewL(*this);
    iAutoResumeHandler = CMPXAutoResumeHandler::NewL(*this, EFalse);
    iPlaybackSettings = CMPXPlaybackSettings::NewL();
    iTaskQueue = CMPXActiveTaskQueue::NewL();
    FeatureManager::InitializeLibL();

#if defined(__HIGH_RESOLUTION_VOLUME)
    // Intialise volume level increment based on the accessory plugged in.
    SetVolumeIncrement( iAccessoryObs->AccessoryModeL() );
#else
    iVolumeIncrement = KMPXLargeVolumeIncrement;
#endif

    iMediaHelper = CMPXPlaybackMediaHelper::NewL( *this );
    iDummyMediaObserver = new(ELeave) CMPXPlaybackDummyMediaObserver();
    iInitVolume = EFalse;
    
    if (iCategory != EMPXCategoryVideo)
        {
        // Select local plugin by default if none selected
        iPluginHandler->SelectPlayersL( EPbLocal );
        iInitVolume = ETrue;
        iPluginHandler->Plugin()->PropertyL( EPbPropertyVolume );
        }
    
#if defined(__HIGH_RESOLUTION_VOLUME)
    iVolRoundedUp = EFalse;
#endif
    
    iSyncMsgTimer = CPeriodic::NewL( CActive::EPriorityIdle ); 
    iSyncMsgWait = new (ELeave) CActiveSchedulerWait; 
    iProperties[EPbPropertyPosition] = 0; 
    	}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPlaybackEngine::~CMPXPlaybackEngine()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::~CMPXPlaybackEngine");
    if (iPluginHandler->Plugin())
        {
        iPluginHandler->Plugin()->CancelRequest();
        }

    DoClose();

    if (iTaskQueue)
        {
        delete iTaskQueue;
        }

    if (iProgressTimer)
        {
        delete iProgressTimer;
        }

    if (iSeekTimer)
        {
        delete iSeekTimer;
        }

    FeatureManager::UnInitializeLib();
    iFile.Close();
    delete iCmdBuffer;
    delete iAccessoryObs;
    delete iAutoResumeHandler;
    delete iPluginHandler;
    delete iClientList;
    delete iPlaybackSettings;
    delete iInitializer;
    iMediaAttrs.Close();
    delete iMediaHelper;
    delete iDummyMediaObserver;
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    iFile64.Close();
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    delete iSyncMsgTimer;
    if (iSyncMsgWait && iSyncMsgWait->IsStarted() )
            {
            iSyncMsgWait->AsyncStop();
            }
    delete iSyncMsgWait; 
    }

// ----------------------------------------------------------------------------
// Initialises from collection
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitL(
    const CMPXCollectionPlaylist& aPlaylist,
    TBool aPlay )
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitL");
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext = aPlaylist.PreInitPlugin();
    if ( iInitNext )
        {
        if ( !iInitializer )
            {
            iInitializer=CMPXPlaybackInitializer::NewL(*this, *iMediaHelper);
            }
        iInitializer->Close();
        }

    HandleCloseL();
    iPlaySource=EPlayFromCollection;
    iPlaylist=CMPXCollectionPlaylist::NewL(aPlaylist, this);
    CMPXCollectionPlaylist::TRepeatMode repeatmode =
            static_cast<CMPXCollectionPlaylist::TRepeatMode>(
                    iProperties[EPbPropertyRepeatMode]);
    iPlaylist->SetRepeatMode(repeatmode);
    // if current index is not 0, play the selected item, otherwise play any one
    iPlaylist->SetShuffleL(iProperties[EPbPropertyRandomMode],
                           iPlaylist->Index()!=-1);
    iSkipping = EFalse;
    if ( aPlay )
        {
        if ( iPlaylist->Count() > 0 )
            {
            iNextState=EPbStateNotInitialised;
            MediaFromCollectionL();
            }
        }
    else
        {
        if ( iPlaylist->Count() > 0 )
            {
            iState = EPbStateStopped;
            }
        else
            {
            iState = EPbStateNotInitialised;
            }
        iAutoResumeHandler->HandlePlaybackStateChange(iState);
        iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
        iNextState = iState;
        //update now playing view
        iClientList->SendMsgL(TMPXPlaybackMessage(
                TMPXPlaybackMessage::EPlaylistUpdated));
        iClientList->SendMsgL(
            TMPXPlaybackMessage(
                TMPXPlaybackMessage::EInitializeComplete,
                0,
                EFalse));
				
        if ( iPluginHandler->PlayerFound() )
        {
        	iPluginUid = iPluginHandler->Plugin()->Uid();
        }

        RArray<TMPXAttribute> dummy;
        CleanupClosePushL( dummy );
        dummy.AppendL( KMPXMediaGeneralUri ); // dummy attribute to get
        iPlaylist->MediaL( dummy.Array(), *iDummyMediaObserver);
        CleanupStack::PopAndDestroy( &dummy );
        }
    }

// ----------------------------------------------------------------------------
// Initialises from URI
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitL(const TDesC& aUri,const TDesC8& aType)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitL(const TDesC& aUri,const TDesC8& aType)");
    MPX_DEBUG2("CMPXPlaybackEngine::InitL(%S)", &aUri);
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromUri;
    iNextState=EPbStateNotInitialised;
    delete iUri;
    iUri = NULL;
    iUri=aUri.AllocL();
    iItemId = KMPXInvalidItemId;
    TRAPD( err, InitL(&aUri,&aType,NULL) );
    if ( KErrNotFound == err || KErrPathNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Initialises from file.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitL(const RFile& aFile)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitL(const RFile& aFile)");
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromFile;
    iNextState=EPbStateNotInitialised;
    iFile.Duplicate(aFile);
    TRAPD( err, InitL(NULL,NULL,&iFile) );
    if ( KErrNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Initialises from URI
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitStreamingL(const TDesC& aUri, const TDesC8& aType, const TInt aAccessPoint)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitStreamingL(const TDesC& aUri, const TInt aAccessPoint)");
    MPX_DEBUG4("CMPXPlaybackEngine::InitStreamingL(%S), (%s), (%d)", &aUri, &aType, aAccessPoint);
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromUri;
    iNextState=EPbStateNotInitialised;
    delete iUri;
    iUri = NULL;
    iUri=aUri.AllocL();
    iItemId = KMPXInvalidItemId;
    iAccessPoint = aAccessPoint;
    iAccessPointSet = ETrue;
    TRAPD( err, InitL(&aUri,&aType,NULL,aAccessPoint) );
    if ( KErrNotFound == err || KErrPathNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Initialises from file.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitStreamingL(const RFile& aFile, const TInt aAccessPoint)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitStreamingL(const RFile& aFile, const TInt aAccessPoint)");
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromFile;
    iNextState=EPbStateNotInitialised;
    iFile.Duplicate(aFile);
    iAccessPoint = aAccessPoint;
    iAccessPointSet = ETrue;
    TRAPD( err, InitL(NULL,NULL,&iFile,aAccessPoint));
    if ( KErrNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Cancels all outsatnding calls (tasks): plug-in should only have one
// outstanding so that is canceled; the tasks are deleted and removed from the
//  queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::CancelRequests()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::CancelRequests()");
    CMPXPlaybackPlugin* p(iPluginHandler->Plugin());
    if (iCallback && iTaskQueue->PtrData())
        { // there is outstanding request
        p = static_cast<CMPXPlaybackPlugin*>(iTaskQueue->PtrData());
        }
    if (p)
        {
        p->CancelRequest();
        if (iCallback)
            {
            iTaskQueue->CompleteTask();
            iCallback = NULL;
            }
        }
    if (iPlaylist)
        {
        iPlaylist->CancelRequest();
        }
    if (iInitializer)
        {
        iInitializer->Close();
        }
    iTaskQueue->CancelRequests();
    iMediaHelper->CancelRequests();
    }

// ----------------------------------------------------------------------------
// Handle a command
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::CommandL(TMPXPlaybackCommand aCmd,
                                           TInt aData)
    {
    MPX_DEBUG2("CMPXPlaybackEngine::CommandL(%d) entering", aCmd);
    iClientList->SendMsgL(TMPXPlaybackMessage(
                            TMPXPlaybackMessage::ECommandReceived,
                            aCmd,
                            0));
    if (iCmdBuffer->CommandForBuffering(aCmd))
        {
        // Buffered commands are not valid if there are no items in playlist
        TBool cmdOK( ETrue );
        if ( iPlaySource == EPlayFromCollection )
            {
            if ( iPlaylist )
                {
                cmdOK = ( iPlaylist->Count() > 0 );
                }
            else
                {
                cmdOK = EFalse;
                }
            }
        if ( cmdOK )
            {
            iCmdBuffer->BufferCommandL(aCmd,iNextState,
                                       iProperties[EPbPropertyPosition]);
            }
        }
    else
        {
        HandleCommandL(aCmd,aData);
        }
    MPX_DEBUG1("CMPXPlaybackEngine::CommandL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle a command
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::CommandL(
    CMPXCommand& aCmd,
    const CMPXMessageQueue& aMsgQueue )
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::CommandL(CMPXCommand& aCmd)");
    ASSERT(aCmd.IsSupported(KMPXCommandGeneralId));
    TInt id = aCmd.ValueTObjectL<TInt>(KMPXCommandGeneralId);
    switch (id)
        {
        case KMPXCommandIdPlaybackGeneral:
            {
            ASSERT(aCmd.IsSupported(KMPXCommandPlaybackGeneralType));
            TMPXPlaybackCommand cmd = static_cast<TMPXPlaybackCommand>(
                        aCmd.ValueTObjectL<TInt>(KMPXCommandPlaybackGeneralType));
            if ( EPbCmdPlay == cmd ||
                    EPbCmdPlayPause == cmd ||
                    EPbCmdStop == cmd)
                {
                ASSERT(aCmd.IsSupported(KMPXCommandPlaybackGeneralClientPid));
                iLastActiveProcess = aCmd.ValueTObjectL<TProcessId>(
                                           KMPXCommandPlaybackGeneralClientPid);
                }
            TInt data(0);
            if (aCmd.IsSupported(KMPXCommandPlaybackGeneralData))
                {
                data = aCmd.ValueTObjectL<TInt>(KMPXCommandPlaybackGeneralData);
                }
            MPX_DEBUG3("CMPXPlaybackEngine::CommandL general command (%d) data %d ",
                       cmd, data);
            // Check if command is to be buffered, if not then handle command directly
            TBool noBuffer( EFalse );
            if ( aCmd.IsSupported( KMPXCommandPlaybackGeneralNoBuffer ))
                {
                noBuffer = aCmd.ValueTObjectL<TBool>(
                                        KMPXCommandPlaybackGeneralNoBuffer );
                }

            if ( noBuffer )
                {
                HandleCommandL( cmd, data );
                }
            else
                {
                // Map to TMPXCommand command
                CommandL(cmd, data);
                }
            break;
            }
        case KMPXCommandSubscriptionAdd:
            {
            TInt index( iClientList->Find( aMsgQueue ));
            CMPXMediaArray* items(
                aCmd.Value<CMPXMediaArray>( KMPXCommandSubscriptionAddItems ));
            User::LeaveIfNull(items);
            CMPXSubscription* subscription( CMPXSubscription::NewL( *items ));
             CleanupStack::PushL(subscription);
             iClientList->AddSubscriptionL( index, subscription );   // ownership transferred
             CleanupStack::Pop(subscription);
            break;
            }
        case KMPXCommandSubscriptionRemove:
            {
            TInt index( iClientList->Find( aMsgQueue ));
            CMPXMediaArray* items(
                aCmd.Value<CMPXMediaArray>( KMPXCommandSubscriptionAddItems ));
            User::LeaveIfNull(items);
            CMPXSubscription* subscription( CMPXSubscription::NewL( *items ));
             CleanupStack::PushL(subscription);
             iClientList->RemoveSubscriptionL( index, *subscription );
             CleanupStack::PopAndDestroy(subscription);
            break;
            }
        case KMPXCommandSubscriptionRemoveAll:
            {
            TInt index( iClientList->Find( aMsgQueue ));
             iClientList->RemoveAllSubscriptionsL( index );
            break;
            }

        default:
            {
            // Custom command, so just send to plugin to handle
            if ( iPluginHandler->Plugin() )
                {
                iPluginHandler->Plugin()->CommandL( aCmd );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// Set a property
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::SetL(TMPXPlaybackProperty aProperty,
                                       TInt aValue)
    {
    MPX_DEBUG3("-->CMPXPlaybackEngine::SetL(%d, %d)", aProperty, aValue);
    if ( aProperty < 0 || aProperty > EPbPropertyNum )
        {
        MPX_DEBUG1("CMPXPlaybackEngine::SetL(): Invalid Argument");
        User::Leave( KErrArgument );
        }
    switch(aProperty)
        {
        case EPbPropertyEmbeddedMode:
        case EPbPropertyCrossFade:
            if ( aProperty < iProperties.Count() )
                {
                iProperties[aProperty]=aValue;
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                    aProperty, iProperties[aProperty]));
                }
            break;
        case EPbPropertyVolume:
            MPX_DEBUG2( "CMPXPlaybackEngine::SetL EPbPropertyVolume %d", aValue );
            if ( aValue >= KPbPlaybackVolumeLevelMin &&
                 aValue <= KPbPlaybackVolumeLevelMax)
                {
                iProperties[EPbPropertyMute] = EFalse;
                iProperties[aProperty] = aValue;
                if ( iPluginHandler->Plugin() )
                    {
                    PluginL()->SetL( aProperty, aValue );
                    }
#if defined(__HIGH_RESOLUTION_VOLUME)
                iVolRoundedUp = EFalse;
#endif                	
                }
            else
                {
                User::Leave(KErrArgument);
                }
            break;
        case EPbPropertyMute:
            MPX_DEBUG2( "CMPXPlaybackEngine::SetL EPbPropertyMute %d", aValue );
            iProperties[EPbPropertyMute] = aValue; // set now, needed when EPSetComplete is converted to EPropertyChanged 
            if ( iPluginHandler->Plugin() )
                {
                PluginL()->SetL( aProperty, aValue );
                }
            break;
        case EPbPropertyRandomMode:
        	{
            MPX_DEBUG2( "CMPXPlaybackEngine::SetL EPbPropertyRandomMode %d", aValue );
            TBool randomMode = static_cast<TBool>(aValue);
            if( iProperties[EPbPropertyRandomMode] != randomMode )
                {
                iProperties[EPbPropertyRandomMode] = randomMode;
                if (iPlaySource==EPlayFromCollection && iPlaylist)
                    {
                    TRAP_IGNORE(iPlaylist->SetShuffleL(randomMode, ETrue));
                    //  Pre-initializer needs to re-copy the new shuffle list
                    //  from the engine
                    if (iInitializer)
                        {
                        iInitializer->Close();
                        }
                    }
                TRAP_IGNORE(iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        EPbPropertyRandomMode,
                                        randomMode)));
                }
            break;
            }
        case EPbPropertyRepeatMode:
        	{
            MPX_DEBUG2( "CMPXPlaybackEngine::SetL EPbPropertyRepeatMode %d", aValue );
            TMPXPlaybackRepeatMode repeat = static_cast<TMPXPlaybackRepeatMode>(aValue);
            if( iProperties[EPbPropertyRepeatMode] != repeat )
                {
                iProperties[EPbPropertyRepeatMode] = repeat;
                if (iPlaySource==EPlayFromCollection && iPlaylist)
                    {
                    CMPXCollectionPlaylist::TRepeatMode repeatmode =
                            static_cast<CMPXCollectionPlaylist::TRepeatMode>(
                            repeat);
                    iPlaylist->SetRepeatMode(repeatmode);
                    if (iInitializer)
                        {
                        iInitializer->Close();
                        }
                    }
                TRAP_IGNORE(iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        EPbPropertyRepeatMode,
                                        repeat)));
                }
            break;
            }
        case EPbPropertyPosition:
            {
            CMPXPlaybackPlugin* pi = PluginL();
            if ( pi )
                {
                MPX_DEBUG2( "CMPXPlaybackEngine::SetL setting position %d", aValue );
                iProperties[aProperty] = aValue;
                pi->SetL(EPbPropertyPosition, aValue);
                }
            break;
            }
        default:
            iProperties[aProperty]=aValue;
            PluginL()->SetL(aProperty,aValue);
        }
    MPX_DEBUG3("<--CMPXPlaybackEngine::SetL(%d, %d)", aProperty, aValue);
    }

// ----------------------------------------------------------------------------
// Property request
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::PropertyL(TMPXPlaybackProperty aProperty,
                                         MMPXPlaybackEngineObserver& aCallback)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::PropertyL()");
    MPX_DEBUG4("CMPXPlaybackEngine::PropertyL 0x%08x cb 0x%08x, prop %d",
                this, &aCallback, aProperty);
    if (aProperty == EPbPropertyVolume)
        {
        aCallback.HandleProperty(aProperty,
                                 iProperties[EPbPropertyVolume],
                                 KErrNone);
        }
    else if (aProperty == EPbPropertyMaxVolume)
        {
        aCallback.HandleProperty(aProperty,
                                 KPbPlaybackVolumeLevelMax,
                                 KErrNone);
        }
    else if (aProperty == EPbPropertyRandomMode)
        {
        TInt random(0);
        if ( iPlaylist )
            {
            random = iPlaylist->Shuffle();
            }
        else
            {
            random = iProperties[EPbPropertyRandomMode];
            }
        aCallback.HandleProperty(aProperty,
                                 random,
                                 KErrNone);
        }
    else if (aProperty == EPbPropertyRepeatMode)
        {
        TInt repeat(0);
        if ( iPlaylist )
            {
            repeat = iPlaylist->RepeatMode();
            }
        else
            {
            repeat = iProperties[EPbPropertyRepeatMode];
            }
        aCallback.HandleProperty(aProperty,
                                 repeat,
                                 KErrNone);
        }
    else if (aProperty == EPbPropertyPosition &&
             EPbStateNotInitialised == iPluginState)
        {
        aCallback.HandleProperty(aProperty,
                                 iProperties[EPbPropertyPosition],
                                 KErrNone);
        }
    else
        {
        MPX_DEBUG1("CMPXPlaybackEngine::PropertyL add request to task queue");
        iTaskQueue->AddTaskL(EProperty, &aCallback, this, aProperty);
        }
    }

// ----------------------------------------------------------------------------
// Async call: must be added to task queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::MediaL(
    MMPXPlaybackEngineObserver& aCallback,
    CBufBase* /*aBuf*/)
    {
    MPX_DEBUG3("-->CMPXPlaybackEngine::MediaL() aBuf 0x%08x cb 0x%08x", this, &aCallback);
    // DEPRECATED
    MPX_ASSERT( 0 );
    MPX_DEBUG2("<--CMPXPlaybackEngine::MediaL() aBuf 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Async call: must be added to task queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::MediaL(
    MMPXPlaybackEngineObserver& aCallback,
    const CMPXCommand& aCmd)
    {
    MPX_DEBUG3("-->CMPXPlaybackEngine::MediaL() aCmd 0x%08x cb 0x%08x", this, &aCallback);
    CMPXCommand* cmd( CMPXCommand::NewL( aCmd ) );
    CleanupStack::PushL( cmd );
    TBool redirect = EFalse;    
    if (CMPXAttributeSpecs* specs = aCmd.Value<CMPXAttributeSpecs>( KMPXCommandMediaAttributeSpecs ))
        { 
        if (specs->IsSupported(KMPXMediaGeneralExtMediaRedirect))
             {
             redirect = ETrue;
             MPX_DEBUG1("CMPXPlaybackEngine::MediaL - Redirect to Playback Plugin");
             }        
        }
    
    // If redirect is false, pass the cmd to mediahelper class
    if ( iPlaySource == EPlayFromCollection && iPlaylist && !redirect)
        {
        // Onwership of cmd is passed to mediahelper class
        iMediaHelper->MediaL( iPlaylist->Path(), cmd, &aCallback );
        }
    else
        {
        // ownership of cmd is transfered
        MediaFromPluginL( &aCallback, cmd );
        }
    CleanupStack::Pop( cmd );
    MPX_DEBUG2("<--CMPXPlaybackEngine::MediaL() aCmd 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Async call: must be added to task queue
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::SubPlayerNamesL(TUid aPlayerUid,
                                                  MMPXPlaybackEngineObserver& aCallback)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::SubPlayerNamesL()");
    CMPXPlaybackPlugin* p = iPluginHandler->CreatePlayerPluginL(aPlayerUid);
    iTaskQueue->AddTaskL(ESubPlayerNames,&aCallback, this, aPlayerUid.iUid, NULL, p);
    }

// ----------------------------------------------------------------------------
// Async call: must be added to task queue
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::MediaFromPluginL(
    MMPXPlaybackEngineObserver* aCallback,
    CMPXCommand* aCmd )
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::MediaFromPluginL");
    CleanupStack::PushL( aCmd );
    iTaskQueue->AddTaskL( EMedia, aCallback, this, 0, NULL, NULL, aCmd );
    CleanupStack::Pop( aCmd ); // Ownership transferred to the task queue
    }

// ----------------------------------------------------------------------------
// Handles a regular heartbeat timer event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::Beat()
    {
        
    // retrieve position from plugins directly
    if (iState != EPbStatePlaying)
        {
        iProgressTimer->Cancel();
        }
    else
        {
        TRAP_IGNORE(iTaskQueue->AddTaskL(EProperty, &iReflector,
                                         this, EPbPropertyPosition));
        }

    }

// ----------------------------------------------------------------------------
// Synchronises the heartbeat timer with system clock
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::Synchronize()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::Synchronize");
    MPX_DEBUG2("CMPXPlaybackEngine::Synchronize(): iState = %d", iState);
        
    // do the same thing as Beat
    Beat();
    }

// ----------------------------------------------------------------------------
// Handle accessory event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleAccesoryEventL(TMPXPlaybackAccessoryMode aMode)
    {
    MPX_DEBUG2("CMPXPlaybackEngine::HandleAccesoryEventL(%d) entering", aMode);
    iClientList->SendMsgL(
        TMPXPlaybackMessage( TMPXPlaybackMessage::EAccessoryChanged,
                             aMode,
                             0));

#if defined(__HIGH_RESOLUTION_VOLUME)
    SetVolumeIncrement( aMode );
#endif // HIGH_RESOLUTION_VOLUME

    MPX_DEBUG1("CMPXPlaybackEngine::HandleAccesoryEventL() exiting");
    }

// ----------------------------------------------------------------------------
// Callback from plug-in, handle plugin event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePluginEvent(
    TEvent aEvent,
    TInt aData,
    TInt aError)
    {
    TRAP_IGNORE(DoHandlePluginEventL(aEvent, aData, aError));
    }

// ----------------------------------------------------------------------------
// Callback from plug-in, handle plugin message
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePlaybackMessage(
    CMPXMessage* aMsg,
    TInt aErr)
    {
    iClientList->SendMsg(aMsg, aErr);
    }

// ----------------------------------------------------------------------------
// Handler plugin event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::DoHandlePluginEventL(
    TEvent aEvent,
    TInt aData,
    TInt aError)
    {
    MPX_DEBUG5("-->CMPXPlaybackEngine::DoHandlePluginEventL 0x%08x, event %d, data %d, error %d.",
               this, aEvent, aData, aError);

    TInt disconnectionError = aError;
    if ( aEvent == EPPlayerUnavailable )
        {
        aError = KErrNone;
        }

    if (aError!=KErrNone)
        {
        // If error occured during pause event, change to pause but also
        // send error code to clients to handle
        if ( aEvent == EPPaused )
            {
            MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL(): Pause Error = %d", aError);

            if ( aError != KErrNotSupported )
                {
                // If error during seeking event, keep seeking and set
                // plugin state to not intialised
                if ( iState == EPbStateSeekingForward ||
                     iState == EPbStateSeekingBackward )
                    {
                    iPluginState = EPbStateNotInitialised;
                    }
                else
                    {
                    iState = EPbStatePaused;
                    iNextState = EPbStatePaused;
                    SetPlayerActivated(EFalse);
                    iClientList->SendMsgL(
                        TMPXPlaybackMessage(
                            TMPXPlaybackMessage::EStateChanged, iState, aError ));
                    if ( KErrDied == aError ||
                         KErrAccessDenied == aError ||
                         KErrInUse == aError )
                        {
                        iPluginState = EPbStateNotInitialised;
                        // fixed 
                        iAutoResumeHandler->HandlePlaybackComplete(aError);
                        iAutoResumeHandler->HandlePlaybackStateChange(iState);

                        }
                    }
                }
            else // Pause functionality is not supported, no change of state
                 // we just send the error back to Ui to let the user know
                {
                iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged, aEvent, aError));

                TMPXPlaybackState s = EPbStatePlaying;
                iNextState = EPbStatePlaying;

                // Set state first before activation, since activation
                // will send a state changed update event as well
                SetStateL(s);
                }
            }
        else if ( aEvent != EPSetComplete ) //Not paused and not EPSetComplete
            { // sometimes upnp failed to set value.
            MPX_DEBUG4("CMPXPlaybackEngine::DoHandlePluginEventL error aError %d, iState %d, iNextState %d",
                       aError, iState, iNextState );
            SavePlaybackInfoL();
            HandlePluginEventErrorHandling(aEvent, aError);
            iState = EPbStateStopped;
            iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
            if (aError)
                {
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EError,aEvent,aError));
                }
            }
        else // SetComplete
            {
            if ( aError == KErrNotSupported &&
                 aData == EPbPropertyVolume )
                {
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        aData, aError));
                }
            }
        }
    else // No errors
        {
        MPX_DEBUG4("CMPXPlaybackEngine::DoHandlePluginEventL, iState %d, iNextState %d, iSkipping %d",
                   iState, iNextState, iSkipping );

        switch(aEvent)
            {
            case EPInitialised:
            case EPClosed:
            case EPPaused:
            case EPPlaying:
            case EPStopped:
            case EPDownloadPositionChanged:
            case EPDownloadStateChanged:
            case EPDownloadCmdPauseDownload:
            case EPDownloadCmdResumeDownload:
            case EPDownloadCmdCancelDownload:
            case EPDownloadStarted:
            case EPDownloadingUpdated:
            case EPDownloadingComplete:	
            case EPBufferingStarted:
            case EPPlayComplete:  
            case EPPluginSeeking:          	
            	{
            	UpdateStateMachineL(aEvent, aData, aError);
            	break;
            	}    
            case EPActive:
                {
                MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL active message.");
                SetPlayerActivated(aData);
                break;
                }
            case EPSubPlayersChanged:
                {
                MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL subplayer changed.");
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::ESubPlayersChanged));
                break;
                }
            case EPSupportedFeaturesChanged:
                {
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                       EPbPropertySupportedFeatures, aData));
                break;
                }
            case EPSetComplete:
                {
                if ( aData < iProperties.Count() )
                    {
                    iClientList->SendMsgL(
                        TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                            aData, iProperties[aData]));
                    }
                break;
                }
            case EPDurationChanged:
                {
                iProperties[EPbPropertyDuration]=aData;
                SetPropertiesL();
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        EPbPropertyDuration,
                                        iProperties[EPbPropertyDuration]));
                break;
                }
            case EPPlayerUnavailable:
                {
                if (iInitializer)
                    {
                    iInitializer->Close();
                    }
                SavePlaybackInfoL();
                iPreservedState = iState;
                iPreservedPosition = iProperties[EPbPropertyPosition];
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPlayerUnavailable,
                                        0, disconnectionError));
                break;
                }
            case EPVolumeChanged:
                {
                iProperties[EPbPropertyVolume] = aData;
                if (iInitVolume)
                    {
                    iInitVolume = EFalse;
                    }
                else
                    {
                    iClientList->SendMsgL(
                        TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                            EPbPropertyVolume,
                                            iProperties[EPbPropertyVolume]));
                    }
                break;
                }
            case EPMuteChanged:
                {
				// property changed broadcast message after successful return from plugins
                iProperties[EPbPropertyMute] = aData;
                iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                            EPbPropertyMute,
                                            iProperties[EPbPropertyMute]));
                break;
                }
            case EPPositionChanged:
                {
                iProperties[EPbPropertyPosition] = aData;
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        EPbPropertyPosition,
                                        iProperties[EPbPropertyPosition]));
                break;
                }
            case EPDownloadFileMoved:
                {
                CMPXMessage* msg = CMPXMessage::NewL();
                CleanupStack::PushL(msg);
                TMPXMessageId id=static_cast<TMPXMessageId>(KMPXMessageGeneral);
                HBufC* path = reinterpret_cast<HBufC*>(aData);
                msg->SetTObjectValueL<TMPXMessageId>(KMPXMessageGeneralId, id);
                msg->SetTObjectValueL<TInt>(KMPXMessageGeneralEvent, TMPXPlaybackMessage::EDownloadFileMoved);
                msg->SetTObjectValueL<TInt>(KMPXMessageGeneralType, 0);
                msg->SetTObjectValueL<TInt>(KMPXMessageGeneralData, 0);
                msg->SetTextValueL( KMPXMediaGeneralUri, *path );
                iClientList->SendMsg(msg, KErrNone);
                CleanupStack::PopAndDestroy( msg );
                break;
                }
            default:
                ASSERT(0);
            }
        }
    MPX_DEBUG2("<--CMPXPlaybackEngine::DoHandlePluginEventL() 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Update State Machine
// Handle plugin events that could cause state change in playback-engine
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::UpdateStateMachineL(
	    TEvent aEvent,
	    TInt aData,
	    TInt aError)
	{
	MPX_DEBUG4("CMPXPlaybackEngine::UpdateStateMachine, iState %d, iNextState %d, iSkipping %d",
	           iState, iNextState, iSkipping );

	TMPXPlaybackState s=iState;

	switch(aEvent)
	    {
        case EPInitialised:
            {
            HandlePluginEventInitialisedL( s, aData );
            break;
            }            
	    case EPPlaying:
	        {
	        s = EPbStatePlaying;
	        iNextState = EPbStatePlaying;
	        MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL playing.");
	
	        // Set state first before activation, since activation
	        // will send a state changed update event as well
	        SetStateL(s);
	        SetPlayerActivated(ETrue);
	        iPluginState = EPbStatePlaying;
	        break;
	        }            
	    case EPClosed:
	        {
	        MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL closed.");
	        MPX_DEBUG2("CMPXPlaybackEngine::HandlePluginEventL: iState = %d", iState);

	        iPluginState = EPbStateNotInitialised;

	        // Do not set state changed event when initialising
	        if ( iState != EPbStateInitialising && EPbCmdClose == aData)
	            {
	            s=EPbStateNotInitialised;

	            // Set state first before deactivation, since deactivation
	            // will send a state changed update event as well
	            SetStateL(s);
	            SetPlayerActivated(EFalse);
	            } // else close when stop
	        break;
	        }
	    case EPPaused:
	        {
	        if (iState != EPbStateSeekingForward &&
	            iState != EPbStateSeekingBackward &&
	            iState != EPbStatePaused &&
	            !iSkipping)
	            {
	            s=EPbStatePaused;

	            if (EPbStatePlaying == iNextState)
	                { // only change state from playing to pause
	                iNextState = EPbStatePaused;
	                }

	            MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL paused.");
	
	            // Set state first before deactivation, since deactivation
	            // will send a state changed update event as well
	            SetStateL(s);
	            SetPlayerActivated(EFalse);
	            }

	        iPluginState = EPbStatePaused;
	        break;
	        }
	    case EPActive:
	        {
	        MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL active message.");
	        SetPlayerActivated(aData);
	        break;
	        }            
	    case EPStopped:
	        {
	        MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL stopped.");

	        s = EPbStateStopped;
	        SetStateL(s);
	        TBool handleStop(EFalse);

	        if (iState != EPbStateSeekingForward &&
	            iState != EPbStateSeekingBackward &&
	            !iSkipping &&
	            iNextState != EPbStatePlaying)
	            {
	            handleStop = ETrue;
	            }
	        else if (iProgressTimer->IsActive())
	            { // stop from remote player
	            Suspend();
	            iNextState = EPbStateStopped;
	            handleStop = ETrue;
	            } // else stop for seeking
	        if (handleStop)
	            {
	            SetPlayerActivated( EFalse );
	            s=EPbStateStopped;
	            if ( iProperties[EPbPropertyPosition] != 0 )
	                {
	                iProperties[EPbPropertyPosition]=0;
	                iClientList->SendMsgL(
	                    TMPXPlaybackMessage(
	                        TMPXPlaybackMessage::EPropertyChanged,
	                        EPbPropertyPosition,
	                        0 ));
	                }
	            }
	        break;
	        }
        case EPPlayComplete:
            {
            EndSeek();
            SavePlaybackCompleteInfoL();
            iAutoResumeHandler->HandlePlaybackComplete(aError);
            iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                    EPbPropertyPosition,
                                    iProperties[EPbPropertyDuration]));
            iTaskQueue->AddTaskL(EHandleNext, NULL, this, 0);
            s=EPbStateStopped; 	//State Changed indication Stopped will be sent           
            break;
        	}
        case EPDownloadPositionChanged:
            {
            if ( EPbStateInitialising == s )
                {
                s=EPbStateBuffering;
                }
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadPositionChanged,
                                     0,
                                     aData ));
            break;
            }
        case EPDownloadStateChanged:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadStateChanged,
                                     0,
                                     aData ));
            break;
            }
        case EPDownloadCmdPauseDownload:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadCmdPauseDownload,
                                     0,
                                     aData ));
            break;
            }
        case EPDownloadCmdResumeDownload:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadCmdResumeDownload,
                                     0,
                                     aData ));
            break;
            }
        case EPDownloadCmdCancelDownload:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadCmdCancelDownload,
                                     0,
                                     aData ));
            break;
            }
        case EPDownloadStarted:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::EDownloadStarted,
                                     0,
                                     aData));
            break;
            }
        case EPDownloadingUpdated:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EDownloadUpdated,
                                    0,aData));
            s=EPbStateDownloading;
            break;
            }
            // end comment this later
        case EPDownloadingComplete:
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EDownloadComplete,
                                    0,aData));
            s=EPbStateStopped;

            if ( iProperties[EPbPropertyPosition] != 0 )
                {
                iProperties[EPbPropertyPosition]=0;
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(
                        TMPXPlaybackMessage::EPropertyChanged,
                        EPbPropertyPosition,
                        0 ));
                }
            break;
            }
        case EPBufferingStarted:
        	{
        	s = EPbStateBuffering;
            iPluginState = EPbStatePlaying;	//ToDo: Do we need pluginstate?!?!?
        	break;
        	}
        case EPPluginSeeking:
          {
           // suspend playback timer
           Suspend();
           // Change to plugin seeking state
           iNextState = iState;
           s = EPbStatePluginSeeking;
           MPX_DEBUG1("CMPXPlaybackEngine::HandlePluginEventL EPbStatePluginSeeking.");

           // Set state first before activation, since activation
           // will send a state changed update event as well
           SetStateL(s);
           iPluginState = EPbStatePluginSeeking;
           break;
           }        
	    }
	SetStateL(s);				//Check if state changed and notify clients.
    }

// ----------------------------------------------------------------------------
// Handle Plugin Event : Initialised
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePluginEventInitialisedL(TMPXPlaybackState& s, TInt aData)
	{
#if defined(__HIGH_RESOLUTION_VOLUME)
// Merlin twentysteps hack start
    if ( iUri )
        {
        TParsePtrC parser(*iUri);
        
        MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL(): iUri is %S", iUri);
        if (parser.Ext().CompareF(KWmaExtension) == 0 || parser.Ext().CompareF(KRaExtension) == 0)
            {
            // for wma/ra file, increment is always 10 with or without headset
            iVolumeIncrement = KMPXLargeVolumeIncrement;  
            MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL it is a wma/ra file, volumeIncrement: %d !!!!", iVolumeIncrement); 
            }
        }
#ifdef __ACCESSORY_FW
    else if (iAccessoryObs->AccessoryModeL() == EPbAccessoryWiredHeadset || iAccessoryObs->AccessoryModeL() == EPbAccessoryHeadphones)
#else        
    else if (iAccessoryObs->AccessoryModeL() == EPbAccessoryHeadset)
#endif // __ACCESSORY_FW
        {
        // for non wma files with headset, the volume increment is 5
        iVolumeIncrement = KMPXSmallVolumeIncrement;
        }
#endif // HIGH_RESOLUTION_VOLUME    
//  twentysteps  end                	
                
    iPluginState = EPbStateStopped;
    // Reset corrupted and invalid flags if no error
    // Only set if they were not set before, this is because
    // calls to SetL() on collection are costly
    MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL() iDbFlag %x", iDbFlag);
    if ( iDbFlag & KMPXMediaGeneralFlagsIsCorrupted ||
         iDbFlag & KMPXMediaGeneralFlagsIsInvalid ||
         iDbFlag & KMPXMediaGeneralFlagsIsDrmLicenceInvalid )
        {
        TRAP_IGNORE(
            SetFlagBitsL( EFalse,
                          KMPXMediaGeneralFlagsIsCorrupted |
                          KMPXMediaGeneralFlagsIsInvalid |
                          KMPXMediaGeneralFlagsIsDrmLicenceInvalid ));
        }

    iClientList->SendMsgL(TMPXPlaybackMessage(
                    TMPXPlaybackMessage::EMediaChanged));

    iProperties[EPbPropertyDuration]=aData;		//Note, radio has no duration!
    SetPropertiesL();
    iClientList->SendMsgL(
        TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                            EPbPropertyDuration,
                            iProperties[EPbPropertyDuration]));

    //Assumption: we take duration==0 to mean "live streaming"
        // Check if position is not past the duration
	if ( iProperties[EPbPropertyPosition] >= iProperties[EPbPropertyDuration] )
            {
            iProperties[EPbPropertyPosition] = 0;
            }
	
	//check whether plugin is KMPXPlaybackPluginVersion2, if not, set saved position
    CDesCArray* interfaces = iPluginHandler->SupportedInterfacesL( iPluginUid );
    TBool version2InterfaceSupported = EFalse;
    if ( interfaces->MdcaCount() )
        {
        TInt pos(0);            
        version2InterfaceSupported = !interfaces->FindIsq( KMPXPlaybackPluginVersion2, pos );
        }
    delete interfaces;
    
    if ( !version2InterfaceSupported )
        {
        // Set position to restore saved position.
        TRAP_IGNORE( // uPnP leaves if set position in stop state
                PluginL()->SetL( EPbPropertyPosition, iProperties[EPbPropertyPosition] ));
        }

    iAutoResumeHandler->HandleOpenFileComplete();
    
    // Check if playback should not be started automatically.
    if ( iAccessPoint || ( iPlaylist && (!iPlaylist->AutoPlay()) ) )
        {
        iNextState = EPbStateNotInitialised;
        }
    
    TMPXPlaybackState nextState(iNextState); // save next state
    switch (iNextState) //What the next state could be (command may have been sent).
    	{
    	case EPbStateNotInitialised:
            { // Notify client initialized complete         
            MPX_DEBUG1("CMPXPlaybackEngine sends msg EInitializeComplete");
            iClientList->SendMsgL(
                TMPXPlaybackMessage(
                    TMPXPlaybackMessage::EInitializeComplete,
                    0,
                    ETrue));
            s = EPbStateInitialised;	//This will cause a StateChanged message to be sent          
            break;
            }
    	case EPbStateStopped:
            { // Play to the end of playlist
            PluginL()->CommandL(EPbCmdStop);
            if (iInitializer)
                { // close file in the initializer
                iInitializer->Close();
                }
            s = EPbStateStopped;
            iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                    EPbPropertyPosition,
                                    iProperties[EPbPropertyPosition]));
    		break;            
            }    		
    	default:
            {
            iState = EPbStateStopped;
            HandleCommandL(CommandFromState( iNextState ));
            s = iState; // change to new state due to handle command
            iClientList->SendMsgL(
                TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                    EPbPropertyPosition,
                                    iProperties[EPbPropertyPosition]));            
    		break;            
            }    		
        }

    if (iInitNext && EPbStateStopped != nextState)
        {
        //
        // Try pre-initialsing the next song, just in case we get there
        //
        TBool more = iInitializer->Next();
        if (!more && iPlaylist && iPlaylist->Shuffle())
            { // Reach to the end and shuffle is on, pre-initializer
              // needs  to re-copy the new shuffle list from engine
            if (iInitializer)
                {
                iInitializer->Close();
                }
            }
        }
	}

// ----------------------------------------------------------------------------
// Error handling for Handler plugin event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePluginEventErrorHandling(TEvent aEvent, TInt aError)
	{
	switch (aError)
	    {
	    case KErrGeneral:
	        {
	        // Track initialization failed
	        if ( ( aEvent == EPInitialised ) ||
	             ( aEvent == EPPlaying ) )
	            {
	            aError = KErrNotSupported;
	            // Mark item as corrupted
	
	            // fall through on purpose
	            }
	        else
	            {
	            break;
	            }
	        }
	    case KErrArgument:
	    case KErrNotSupported:
	    case KErrCorrupt:
		case KErrDivideByZero:
	        {
	        // Release file handles
	        TRAP_IGNORE(DoStopL());
	        // Mark item as corrupted
	        MarkItemCorrupted( ETrue );
	
	        // Reset DRM and invalid flags if they are set
	        MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL() iDbFlag %x", iDbFlag);
	        if ( iDbFlag & KMPXMediaGeneralFlagsIsInvalid ||
	             iDbFlag & KMPXMediaGeneralFlagsIsDrmLicenceInvalid )
	            {
	            TRAP_IGNORE(
	                SetFlagBitsL( EFalse,
	                              KMPXMediaGeneralFlagsIsInvalid |
	                              KMPXMediaGeneralFlagsIsDrmLicenceInvalid ));
	            }
	
	        break;
	        }
	    case KErrNotFound:
	        {
	        // Mark item as invalid
	        MarkItemInvalid( ETrue );
	
	        // Reset DRM and corrupted flags if they are set
	        MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL() iDbFlag %x", iDbFlag);
	        if ( iDbFlag & KMPXMediaGeneralFlagsIsCorrupted ||
	             iDbFlag & KMPXMediaGeneralFlagsIsDrmLicenceInvalid )
	            {
	            TRAP_IGNORE(
	                SetFlagBitsL( EFalse,
	                              KMPXMediaGeneralFlagsIsCorrupted |
	                              KMPXMediaGeneralFlagsIsDrmLicenceInvalid ));
	            }
	
	        break;
	        }
	    case KErrCANotSupported:
	    case KErrCANoPermission:
	    case KErrCANoRights:
	    case KErrCANoAgent:
	    case KErrCAOutOfRange:
	    case KErrCAPendingRights:
	    case KErrCASizeNotDetermined:
	    case KErrCANewFileHandleRequired:
	        {
	        // Release file handles
	        TRAP_IGNORE(DoStopL());
	        // Mark item as DRM Invalid
	        MarkItemDrmInvalid( ETrue );
	
	        // Reset corrupted and invalid flags if they are set
	        MPX_DEBUG2("CMPXPlaybackEngine::DoHandlePluginEventL() iDbFlag %x", iDbFlag);
	        if ( iDbFlag & KMPXMediaGeneralFlagsIsCorrupted ||
	             iDbFlag & KMPXMediaGeneralFlagsIsInvalid )
	            {
	            TRAP_IGNORE(
	                SetFlagBitsL( EFalse,
	                              KMPXMediaGeneralFlagsIsCorrupted |
	                              KMPXMediaGeneralFlagsIsInvalid ));
	            }
	
	
	        break;
	        }
	    case KErrDied:
	        {
	        // Release file handles
	        TRAP_IGNORE(DoStopL());
	        iNextState = EPbStateNotInitialised;
	        break;
	        }
	    default:
	        // otherwise send error message to clients
	        break;
	    }
	}

// ----------------------------------------------------------------------------
// Callback from plug-in. Retrieve the task that resulted in this (which
// removes it from the queue), get the observer and call back with the results.
// Then the player is free to execute the next task, if any
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleProperty(
    TMPXPlaybackProperty aProperty,
    TInt aValue,
    TInt aError)
    {
    MPX_DEBUG5("-->CMPXPlaybackEngine::HandleProperty 0x%08x prop %d val %d err %d",
               this, aProperty, aValue, aError);
    if ( iInitVolume && EPbPropertyVolume == aProperty )
        {
        if (KErrNone == aError)
            {
            iProperties[EPbPropertyVolume] = aValue;
            iInitVolume = EFalse;
            }
        }
    else
        {
        ASSERT(iTaskQueue->Task() == EProperty && iTaskQueue->Callback() == iCallback);
        iCallback->HandleProperty(aProperty,aValue,aError);
        iCallback = NULL;
        iTaskQueue->CompleteTask();
        }
    MPX_DEBUG2("<--CMPXPlaybackEngine::HandleProperty 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Callback from plug-in. Retrieve the task that resulted in this (which
// removes it from the queue), get the observer and call back with the results.
// Then the player is free to execute the next task, if any
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleMedia(
    CMPXMedia* aMedia,
    TInt aError)
    {
    ASSERT(iTaskQueue->Task() == EMedia && iTaskQueue->Callback() == iCallback);
    MPX_FUNC_EX("CMPXPlaybackEngine::HandleMedia()");
    iCallback->HandleMedia(aMedia, aError);
    iCallback = NULL;
    iTaskQueue->CompleteTask();
    }

// ----------------------------------------------------------------------------
// Callback from plug-in. Retrieve the task that resulted in this (which
// removes it from the queue), get the observer and call back with the results.
// Then the player is free to execute the next task, if any
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleSubPlayerNames(
    TUid /*aPlayer*/,
    const MDesCArray* aSubPlayers,
    TBool aComplete,
    TInt aError)
    {
    ASSERT(iTaskQueue->Task() == ESubPlayerNames &&
            iTaskQueue->Callback() == iCallback);
    MPX_DEBUG4("-->CMPXPlaybackEngine::HandleSubPlayerNames 0x%08x, complete %d, err %d",
               this, aComplete, aError);
    iCallback->HandleSubPlayerNames(TUid::Uid(0), aSubPlayers,
                                    aComplete, aError);
    iCallback = NULL;
    iTaskQueue->CompleteTask();
    MPX_DEBUG2("<--CMPXPlaybackEngine::HandleSubPlayerNames 0x%08x", this);
    }

// ----------------------------------------------------------------------------
// Handle media key event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCmdBufferEventL(const TMPXPlaybackCmdInfo& aEvent)
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleCmdBufferEventL() entering");

    MPX_DEBUG4("CMPXPlaybackEngine::HandleCmdBufferEventL(): offset=%d, state=%d, navKeyBuffered=%d", aEvent.iTrackOffset, iNextState, aEvent.iNavKeyBuffered);

    // Fix for EJPI-7BHUGX, if the aEvent.iNavKeyBuffered == 1 and aEvent.iTrackOffset == 0
    // That means skip back key press once only, it should not request for media but set pos only
    if ( aEvent.iNavKeyBuffered > 1 || aEvent.iTrackOffset != 0 )
        {
        iNextState = aEvent.iState;
        if ( EPlayFromCollection == iPlaySource && iPlaylist )
            {
            MediaFromCollectionL();
            }
        else if ( EPlayFromUri == iPlaySource )
            {
            TRAP_IGNORE( InitL( iUri, &( KNullDesC8 ), NULL, iAccessPoint ) );
            }
        }
    else
        {
        // If position has changed, set the position in plugin.
        // Also the current position should be after the original position
        if ( Abs(aEvent.iPos - iProperties[EPbPropertyPosition]) >
             KPbPositionChangeThreshold &&
             aEvent.iPos < iProperties[EPbPropertyPosition] )
            {
            MPX_DEBUG1("CMPXPlaybackEngine::HandleCmdBufferEventL(): position changed");
            iProperties[EPbPropertyPosition]=aEvent.iPos;
            if ( PluginL() )
                {
                if (iState == EPbStatePlaying)
                    {
                    // Need to send pause command to plugin first so that it will
                    // not keep playing the rest of it's buffer
                    MPX_DEBUG1("CMPXPlaybackEngine::HandleCmdBufferEventL() send pause command");
                    PluginL()->CommandL(EPbCmdPause);
                    }
                PluginL()->SetL(EPbPropertyPosition,
                                iProperties[EPbPropertyPosition]);
                }
            }

        HandleCommandL(CommandFromState( aEvent.iState ));
        }
    iCmdBuffer->CompleteCommand();

    MPX_DEBUG1("CMPXPlaybackEngine::HandleCmdBufferEventL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle command skip event from key buffering
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCmdBufferSkipEventL(
    const TMPXPlaybackSkipEvent aSkipEvent )
    {
    MPX_DEBUG2("==>CMPXPlaybackEngine::HandleCmdBufferSkipEventL(%d) entering", aSkipEvent);

    if ( EPbsSkipEventNext == aSkipEvent )
        {
        iClientList->SendMsgL(
            TMPXPlaybackMessage( TMPXPlaybackMessage::ESkipping, 0, 1 ));

        TBool wasSkipping( iSkipping );
        iSkipping = ETrue;
        if ( !wasSkipping )
            {
            if ( iState == EPbStatePlaying )
                {
                Suspend();
                TRAP_IGNORE(PluginL()->CommandL(EPbCmdPause));
                TRAP_IGNORE(SavePlaybackInfoL());
                }
            else if ( iState == EPbStatePaused )
                {
                TRAP_IGNORE(SavePlaybackInfoL());
                }
            }
        if ( EPlayFromCollection == iPlaySource && iPlaylist )
            {
            if ( iPlaylist->Count() > 0 )
                {
                // Stop if playing or paused
                if ( !iPlaylist->Next( ETrue ) )
                    {
                    if ( EPbStatePlaying == iNextState ||
                         EPbStatePaused == iNextState )
                        {
                        iCmdBuffer->BufferCommandL(
                            EPbCmdStop,
                            iNextState,
                            iProperties[EPbPropertyPosition]);
                        }
                    
                    // End of playlist, send message to clients.
                    iClientList->SendMsgL(
                        TMPXPlaybackMessage(TMPXPlaybackMessage::
                                            EReachedEndOfPlaylist));
                    }
                RequestMediaL();
                }
            }
        }
    else if ( EPbsSkipEventPrevious == aSkipEvent )
        {
        TBool wasSkipping( iSkipping );
        iSkipping = ETrue;
        if ( !wasSkipping && iState == EPbStatePlaying)
            {
            TRAP_IGNORE(DoStopL());
            PluginL()->CommandL( EPbCmdPause );
            }

        // If first track in list and repeat is off, it will replay current track,
        // so do not skip
        TBool sendSkip( ETrue );
        if ( iPlaylist )
            {
            if ( 0 != iPlaylist->Index() ||
                 CMPXCollectionPlaylist::ERepeatOff != iPlaylist->RepeatMode() )
                {
                sendSkip = ETrue;
                }
            else
                {
                sendSkip = EFalse;
                }
            }
        if ( sendSkip && EPlayFromCollection == iPlaySource && iPlaylist )
            {
            iClientList->SendMsgL(
                TMPXPlaybackMessage( TMPXPlaybackMessage::ESkipping, 0, -1 ));

            if (iPlaylist->Count()>0)
                {
                iPlaylist->Previous( ETrue );
                RequestMediaL();
                }
            }
        }
    else
        {
        iSkipping = EFalse;
        iClientList->SendMsgL(
            TMPXPlaybackMessage( TMPXPlaybackMessage::ESkipEnd ));
        }

    MPX_DEBUG1("<==CMPXPlaybackEngine::HandleCmdBufferSkipEventL()");
    }


// ----------------------------------------------------------------------------
// Suspend playback
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::Suspend()
//
// Should stop in current position, i.e. clearing timers etc.
//
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::Suspend()");
    iProgressTimer->Cancel();
    EndSeek();
    iAutoResumeHandler->CancelResumeTimer();
    }

// ----------------------------------------------------------------------------
// Seek timer callback handler
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SeekTimerTick()
    {
    TInt& pos=iProperties[EPbPropertyPosition];
    pos+=iSeekStep;
    if (pos>iProperties[EPbPropertyDuration])
        {
        pos=iProperties[EPbPropertyDuration];
        iSeekTimer->Cancel();
        }
    if (pos<0)
        {
        pos=0;
        iSeekTimer->Cancel();
        }

    iSeekStep*=KPbSeekAccelerationFactor;
    if (iSeekStep>iMaxSeekStep)
        {
        iSeekStep=iMaxSeekStep;
        }
    else if (iSeekStep<-iMaxSeekStep)
        {
        iSeekStep=-iMaxSeekStep;
        }
    TRAP_IGNORE(iClientList->SendMsgL(
                    TMPXPlaybackMessage(TMPXPlaybackMessage::EPropertyChanged,
                                        EPbPropertyPosition,
                                        pos)));
    }

// ----------------------------------------------------------------------------
// Seek timer callback
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackEngine::SeekTimerCallback(TAny* aPtr)
    {
    static_cast<CMPXPlaybackEngine*>(aPtr)->SeekTimerTick();
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Initialise helper method
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::InitL(const TDesC* aSong,
                               const TDesC8* aType,
                               RFile* aFile,
                               TInt aAccessPoint)
    {
    MPX_DEBUG1("==>CMPXPlaybackEngine::InitL(const TDesC* aSong, const TDesC8* aType, RFile* aFile,TInt aAccessPoint)");
    CMPXPlaybackPlugin* p( NULL );
    if ( iPluginHandler->Plugin() )
        {
        TRAP_IGNORE(iPluginHandler->Plugin()->CommandL(EPbCmdClose));
        }
    
    if (aSong && aType)
        {
        MPX_DEBUG2("CMPXPlaybackEngine::InitL(%S)", aSong);
        iPluginHandler->SelectPlayerL(*aSong,*aType);
        }
    else if (aFile)
        {
        iPluginHandler->SelectPlayerL(*aFile);
        }
    else
        { // should never happen
        ASSERT(0);
        }

    p=iPluginHandler->Plugin();
    CheckPtrL(p);

    if (p->Uid()!=iPluginUid)
        { // new plugin or new playlist
        iPluginUid = p->Uid();
        iInitVolume = ETrue;
        p->PropertyL( EPbPropertyVolume );
        iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EPlayerChanged));
        }
    // Stop and close opened file
    TRAP_IGNORE(DoStopL(EFalse));
    TRAP_IGNORE(p->CommandL(EPbCmdClose));

    // If playing from a playlist, send the index as a data
    // parameter.  This helps performance as the client
    // doesn't need to request the index separately
    TInt index( KErrNotFound );
    if ( EPlayFromCollection == iPlaySource )
        {
        index = iPlaylist->Index();
        }
    
    TMPXPlaybackMessage msg( TMPXPlaybackMessage::EStateChanged,
                             EPbStateInitialising,
                             index );
    SetStateL( msg );
    iProperties[EPbPropertyPosition]=0;
    
    if ( KErrNotFound != iPreservedPosition )
          {
          iProperties[EPbPropertyPosition] = iPreservedPosition;
          iPreservedPosition = KErrNotFound;
          }
    else if ( KErrNotFound != iPositionFromMedia )
        {
        iProperties[EPbPropertyPosition] = iPositionFromMedia;
        iPositionFromMedia = KErrNotFound;
        }
    MPX_DEBUG2("CMPXPlaybackEngine::InitL iPropertyPosition %d", iProperties[EPbPropertyPosition] );
      
    // make sure our interface is supported
    CDesCArray* interfaces = iPluginHandler->SupportedInterfacesL( p->Uid() );
    TBool version2InterfaceSupported = EFalse;
    if (interfaces->MdcaCount())
        {
        TInt pos(0);            
        version2InterfaceSupported = !interfaces->FindIsq(KMPXPlaybackPluginVersion2, pos);
        }
    delete interfaces;
        
    // cast the plugin to use our interface
    if (version2InterfaceSupported)
        {
        CMPXPlaybackPluginVersion2* plugin = NULL;
        plugin = static_cast<CMPXPlaybackPluginVersion2*>(p);
        
        // if cast was successful, then init streaming with access point
        if (plugin)
            {
            if ( iAccessPointSet )
                {
                if (aSong && aType)
                    {
                    plugin->InitStreamingL( *aSong, *aType, aAccessPoint, iProperties[EPbPropertyPosition] ); 
                    }
                else if (aFile)
                    {
                    plugin->InitStreamingL( *aFile, aAccessPoint, iProperties[EPbPropertyPosition] ); 
                    }
                }
            else
                {
                if (aSong && aType)
                    {
                    plugin->InitialiseWithPositionL( *aSong, iProperties[EPbPropertyPosition] ); 
                    }
                else if (aFile)
                    {
                    plugin->InitialiseWithPositionL( *aFile, iProperties[EPbPropertyPosition] ); 
                    }        
                }
            }
        else // if (plugin)
            {
            MPX_DEBUG1("CMPXPlaybackEngine::InitL(): *** Init Streaming failed -- failure to convert to expected interface");                
            }
        }
    else // if (version2InterfaceSupported)
        {
        if (aSong && aType)
            {
            p->InitialiseL( *aSong );
            }
        else if (aFile)
            {
            p->InitialiseL( *aFile );
            }        
        }

    MPX_DEBUG1("<==CMPXPlaybackEngine::InitL(const TDesC* aSong, const TDesC8* aType, RFile* aFile, TInt aAccessPoint)");
    }

// ----------------------------------------------------------------------------
// Command handler
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCommandL(TMPXPlaybackCommand aCmd, TInt aData )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::HandleCommandL(%d) entering", aCmd);
    switch(aCmd)
        {
        case EPbCmdPlay:
            HandlePlayL();
            break;
        case EPbCmdStop:
            HandleStopL();
            break;
        case EPbCmdPause:
            HandlePauseL();
            break;
        case EPbCmdNext:
            HandleNextL();
            break;
        case EPbCmdPrevious:
            HandlePreviousL();
            break;
        case EPbCmdPlayWithFadeIn:
            HandlePlayWithFadeInL();
            break;
        case EPbCmdStartSeekForward:
            HandleStartSeekL(ETrue);
            break;
        case EPbCmdStartSeekBackward:
            HandleStartSeekL(EFalse);
            break;
        case EPbCmdStopSeeking:
            HandleStopSeekingL();
            break;
        case EPbCmdIncreaseVolume:
            HandleIncreaseVolumeL();
            break;
        case EPbCmdDecreaseVolume:
            HandleDecreaseVolumeL();
            break;
        case EPbCmdSetVolume:
            HandleSetVolumeL( aData );
            break;
        case EPbCmdMuteVolume:
            HandleMuteL(ETrue);
            break;
        case EPbCmdUnMuteVolume:
            HandleMuteL(EFalse);
            break;
        case EPbCmdClose:
            HandleCloseL( aData );
            break;
        case EPbCmdReplay:
            HandleReplayL();
            break;
        case EPbApplyEffect:
            HandleEffectL(aData);
            break;
		case EPbCmdDisableEffect:
			HandleDisableEffectL();
			break;
        case EPbCmdPreservePosition:
            iPreservedPosition = iProperties[EPbPropertyPosition];
            break;
        case EPbCmdPreserveState:
            iPreservedState = iState;
            break;
        case EPbCmdCloseItem:
            HandleCloseItemL( aData );
            break;
        case EPbCmdCancelInit:
            {
            if (EPbStateInitialising==iState)
                {
                PluginL()->CancelRequest();
                iNextState = EPbStateStopped;

                if (iPluginHandler->Plugin())
                    {
                    TRAP_IGNORE(iPluginHandler->Plugin()->CommandL(EPbCmdClose, aData));
                    }
                if (iInitializer)
                    {
                    iInitializer->Close();
                    }
                //Needs to update iState and report the change to Ui
                SetStateL(EPbStateStopped);
                }
            break;
            }
        case EPbCmdResetPreserveState:
            {
            iPreservedState = EPbStateNotInitialised;
            break;
            }
        case EPbCmdUnloadNonActivePlugin:
            {
            TUid uid = TUid::Uid(aData);
            if (iPluginHandler->Plugin())
                {
                if (iPluginHandler->Plugin()->Uid() != uid)
                    { //not current active plugin
                    iPluginHandler->UnloadPlugin(uid);
                    }
                }
            if (iInitializer)
                {
                if (iInitializer->PluginUid() == uid)
                    { //Plugin loaded in pre-initializer, not active one.
                    iInitializer->Close();
                    }
                }
            break;
            }
        case EPbCmdClearKeyBuffer:
            {
            MPX_DEBUG1("CMPXPlaybackEngine::HandleCommandL - EPbCmdClearKeyBuffer");
            iCmdBuffer->ClearCommands();
            break;
            }
        case EPbCmdSetAutoResume:
            {
            iAutoResumeHandler->SetAutoResume( aData );
            break;
            }
        case EPbCmdSyncMsgComplete:
            {
            StopWaitLoop();
            break;
            }
        default:
            ASSERT(0);
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleCommandL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle media properties from collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCollectionMediaL(
    const CMPXMedia& aMedia,
    TInt aError)
    {
    MPX_DEBUG3("-->CMPXPlaybackEngine::HandleCollectionMediaL 0x%08x err(%d)",
               this, aError);

    if (KErrNone == aError)
        {
        HBufC8* mimeType =
                   MPXUser::Alloc8L(aMedia.ValueText(KMPXMediaGeneralMimeType));
        CleanupStack::PushL(mimeType);

        if(iUri)
            {
            delete iUri;
            iUri = NULL;
            }

        iUri = aMedia.ValueText(KMPXMediaGeneralUri).AllocL();
        MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL uri %S", iUri);

        iItemId = KMPXInvalidItemId;
        if (aMedia.IsSupported(KMPXMediaGeneralId))
            {
            iItemId = aMedia.ValueTObjectL<TMPXItemId>( KMPXMediaGeneralId );
            MPX_DEBUG3("CMPXPlaybackEngine::HandleCollectionMediaL iItemId %d %d", iItemId.iId1, iItemId.iId2);
            }

        iDbFlag = 0;
        if ( aMedia.IsSupported( KMPXMediaGeneralFlags ))
            {
            iDbFlag = aMedia.ValueTObjectL<TUint>( KMPXMediaGeneralFlags );
            MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL iDbFlag %x", iDbFlag);
            }

        iMediaDuration = 0;
        if ( aMedia.IsSupported( KMPXMediaGeneralDuration ))
            {
            iMediaDuration = aMedia.ValueTObjectL<TInt>( KMPXMediaGeneralDuration );
            MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL iMediaDuration %d", iMediaDuration);
            }

        iAccessPoint = 0;
        iAccessPointSet = EFalse;
        if ( aMedia.IsSupported(KMPXMediaGeneralExtAccessPoint) )
            {
            iAccessPoint = aMedia.ValueTObjectL<TInt>( KMPXMediaGeneralExtAccessPoint );
            MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL iAccessPoint %d", iAccessPoint );
            iAccessPointSet = ETrue;
            }
        if (*iUri == KNullDesC)
            {
            aError = KErrNotFound;
            }
        else
            {
            RestorePlaybackPositionL( aMedia );  
            TRAP( aError, InitL( iUri, mimeType, NULL, iAccessPoint) );
            /*
            // 20 steps  fix
            // check whether we are playing WMA files, if so 
            // the increment has to be KMPXLargeVolumeIncrement 
		        TParsePtrC parser(*iUri);
		            
		        MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL(): iUri is %S", iUri);
		        if (parser.Ext().CompareF(KWmaExtension) == 0 || parser.Ext().CompareF(KRaExtension) == 0)
		            {
		            // for wma file, the increment is always 10 with or without headset
		            iVolumeIncrement = KMPXLargeVolumeIncrement;	
		            MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionMediaL it is a wma/ra file, volumeIncrement: %d !!!!", iVolumeIncrement);	
		            }
#if defined(__HIGH_RESOLUTION_VOLUME)
#ifdef __ACCESSORY_FW
		        else if (iAccessoryObs->AccessoryModeL() == EPbAccessoryWiredHeadset || iAccessoryObs->AccessoryModeL() == EPbAccessoryHeadphones)
#else        
            else if (iAccessoryObs->AccessoryModeL() == EPbAccessoryHeadset)
#endif // __ACCESSORY_FW
		            {
		            // for non wma files with headset, the volume increment is 5
		            iVolumeIncrement = KMPXSmallVolumeIncrement;
		            }
#endif // HIGH_RESOLUTION_VOLUME    
               */
            if ( KErrNone == aError )
                {
                RestorePlaybackStateL(); 
                }
            else
                {
                iClientList->SendMsgL(TMPXPlaybackMessage(
                        TMPXPlaybackMessage::EMediaChanged));
                iState = EPbStateStopped;
                }
            }
        CleanupStack::PopAndDestroy(mimeType);
        }
    else
        {
        // it'll be incorrect to leave iItemId unchanged, also KMPXInvalidItemId has special handling in
        // CreateMediaToSetLC so we'd rather set Id to some actual value even if it had some 'issues'
        iItemId = iPlaylist->Path().Id();
        }

    // Check for error again, just in case above code had any errors
    if ( KErrNone != aError )
        {
        if ( KErrNotFound == aError ||
             KErrPathNotFound == aError ||
             KErrPermissionDenied == aError )
            {
            // Mark item as Invalid
            MarkItemInvalid( ETrue );
            iClientList->SendMsgL(
               TMPXPlaybackMessage( TMPXPlaybackMessage::EError,
                                    EPInitialised,
                                    aError ));
            }
        else
            { // other system error
            HandleStopL();
            }
        }
    MPX_DEBUG1("<--CMPXPlaybackEngine::HandleCollectionMediaL()");
    }

// ----------------------------------------------------------------------------
// Handle playlist change event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCollectionPlaylistChange(TInt aError)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::HandleCollectionPlaylistChange");
    MPX_DEBUG2("CMPXPlaybackEngine::HandleCollectionPlaylistChange(%d)", aError);

    if (KErrNotFound == aError)
        {
        if ( iPlaylist )
            {
            TRAP_IGNORE(HandleStopL());

            // If playlist is empty now, update state
            if ( iPlaylist->Count() == 0 )
                {
                iNextState = EPbStateNotInitialised;
                iState = EPbStateNotInitialised;
                }
            }
        }
    else if(KErrEof == aError)
        { // current item removed at the end
        TRAP_IGNORE(HandleStopL());

        // If playlist is empty now, update state
        if ( iPlaylist->Count() == 0 )
            {
            iNextState = EPbStateNotInitialised;
            iState = EPbStateNotInitialised;
            }
        else
            {
            TRAP_IGNORE(HandleNextL(1, ETrue));
            }
        }
    else if (iInitializer)
        { // Reset initializer anyway. Make sure it is synced
         //iInitializer->Close();      //correct auto playlist crash
        }
    //update now playing view
    TRAP_IGNORE(iClientList->SendMsgL(TMPXPlaybackMessage(
                                TMPXPlaybackMessage::EPlaylistUpdated)));
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackEngine::HandlePluginHandlerEvent
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePluginHandlerEvent(
    TPluginHandlerEvents aEvent,
    const TUid& aPluginUid,
    TBool aLoaded,
    TInt aData)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::HandlePluginHandlerEvent");

    switch (aEvent)
        {
        case MMPXPluginHandlerObserver::EPluginAdd:
            {
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginAdd, aPluginUid,
                EFalse, aData));
            break;
            }
        case MMPXPluginHandlerObserver::EPluginUpdateStart:
            {
            // Handling the unloading of the previous plugin version and the loading
            // of the new plugin version is synchronous and therefore new requests
            // will not be processed by the server/engine in between EPluginUpdateStart
            // and EPluginUpdateEnd.
            //
            // If the plugin handler would unload/load plugins asynchronously then a
            // mechanism must be created where new requests are not accepted for
            // the plugin that is being updated for the duration of the update.

            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginUpdateStart,
                aPluginUid, aLoaded, aData));

            // If the current loaded plugin is being updated
            if (iPluginHandler->Plugin() &&
                (iPluginHandler->Plugin()->Uid() == aPluginUid))
                {
                // Complete all outstanding requests
                iTaskQueue->CompleteAllTasks(KErrNotReady);
                }
            break;
            }
        case MMPXPluginHandlerObserver::EPluginUpdateEnd:
            {
            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginUpdateEnd,
                aPluginUid, aLoaded, aData));
            break;
            }
        case MMPXPluginHandlerObserver::EPluginRemove:
            {
            // If the current loaded plugin is being removed
            if (iPluginHandler->Plugin() &&
                (iPluginHandler->Plugin()->Uid() == aPluginUid))
                {
                // Complete all outstanding requests
                iTaskQueue->CompleteAllTasks(KErrNotReady);
                }

            TRAP_IGNORE(SendPluginHandlerMessageL(KMPXMessagePluginRemove,
                aPluginUid, aLoaded));
            break;
            }

        default:
            {
            // ignore the event
            break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CMPXPlaybackEngine::SendPluginHandlerMessageL
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SendPluginHandlerMessageL(
    TInt aMessageId,
    const TUid& aPluginUid,
    TBool aLoaded,
    TInt aVersion /* = 0 */)
    {
    CMPXMessage* msg = CMPXMedia::NewL();
    CleanupStack::PushL(msg);

    msg->SetTObjectValueL<TMPXMessageId>(KMPXMessageGeneralId, aMessageId);
    msg->SetTObjectValueL<TUid>(KMPXAttrPluginId, aPluginUid);
    msg->SetTObjectValueL<TBool>(KMPXAttrPluginLoaded, aLoaded);
    if (aVersion > 0)
        {
        msg->SetTObjectValueL<TInt>(KMPXAttrPluginVersion, aVersion);
        }

    iClientList->SendMsg(msg, KErrNone);

    CleanupStack::PopAndDestroy(msg);
    }

// ----------------------------------------------------------------------------
// Handle play command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePlayL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePlayL(): entering");
    CMPXPlaybackPlugin* p(NULL);
    TMPXPlaybackState oldNextState( iNextState );
    iNextState = EPbStatePlaying;
    if (EPbStateNotInitialised == iPluginState &&
        EPlayFromCollection == iPlaySource &&
        EPbStatePaused != iState)
        {
        MediaFromCollectionL();
        }
    else
        {
        switch (iState)
            {
            case EPbStatePlaying: //already playing, send message to update clients
                if (EPbStatePaused == iPluginState)
                    { // paused due to skipping
                    PluginL()->CommandL(EPbCmdPlay);
                    }
                iClientList->SendMsgL(
                    TMPXPlaybackMessage(
                        TMPXPlaybackMessage::EStateChanged, iState));
                break;
            case EPbStateNotInitialised:    // No song initialised in engine
                iNextState = EPbStateNotInitialised;
                break;
            case EPbStateSeekingForward:
            case EPbStateSeekingBackward:
                EndSeek();
                p = PluginL();
                p->CommandL(EPbCmdPlay);
                break;
            case EPbStatePaused:
            case EPbStateStopped:
                {
                // If playing from collection playlist and currently in
                // stopped state, then re-get media attributes
                if ( EPlayFromCollection == iPlaySource &&
                     EPbStateStopped == oldNextState )
                    {
                    MediaFromCollectionL();
                    break;
                    }
                else
                    {
                    // fall through on purpose
                    }
                }
            default:
                {
                p= PluginL();
                p->CommandL(EPbCmdPlay);
                break;
                }
            }
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePlayL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle play command with fade in
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePlayWithFadeInL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePlayWithFadeInL(): entering");
    MPX_DEBUG2("CMPXPlaybackEngine::HandlePlayWithFadeInL(): Ramp = %d", iProperties[EPbPropertyVolumeRamp]);
    MPX_DEBUG2("CMPXPlaybackEngine::HandlePlayWithFadeInL(): Position = %d", iProperties[EPbPropertyPosition]);

    CMPXPlaybackPlugin* p( PluginL() );
    TRAP_IGNORE( // uPnP leaves if set in stop state
        p->SetL( EPbPropertyVolumeRamp,
                 iProperties[EPbPropertyVolumeRamp] ));
    p->CommandL(EPbCmdPlay);
    iNextState = EPbStatePlaying;
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePlayWithFadeInL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle pause command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePauseL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePauseL(): entering");
    Suspend();
    if ( EPbStatePaused == iState )
        {
        //already paused, send message to update clients
        iClientList->SendMsgL(
            TMPXPlaybackMessage(
                TMPXPlaybackMessage::EStateChanged, iState));
        }
    else
        {
        if (EPbStatePlaying == iNextState)
            { // only change state from playing to pause
            iNextState = EPbStatePaused;
            }
        PluginL()->CommandL(EPbCmdPause);
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePauseL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle stop command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleStopL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStopL(): entering");
    iNextState = EPbStateStopped;
    TRAP_IGNORE(DoStopL());
    if (iInitializer)
        {
        iInitializer->Close();
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStopL(): exiting");
    }

// ----------------------------------------------------------------------------
// Stop playback
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::DoStopL(TBool aSavePlaybackInfo)
    {
    MPX_DEBUG1("==>CMPXPlaybackEngine::DoStopL()");
    Suspend();
    if (iState == EPbStatePaused || iState == EPbStatePlaying ||
        iState == EPbStateInitialising || iState == EPbStateBuffering)
        {
        if (aSavePlaybackInfo && (iState == EPbStatePaused || iState == EPbStatePlaying ))
            {
            TRAP_IGNORE(SavePlaybackInfoL()); // Leave when MMC eject and database already closed.
            }
        PluginL()->CommandL(EPbCmdStop); // Leave if router power down
        }
    MPX_DEBUG1("<==CMPXPlaybackEngine::DoStopL()");
    }

// ----------------------------------------------------------------------------
// Handle next track command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleNextL(
    TInt aOffset /*= 1*/,
    TBool aIgnoreRepeat /*=EFalse*/ )
    {
    MPX_ASSERT(aOffset>0);
    MPX_DEBUG2("CMPXPlaybackEngine::HandleNextL(%d) entering", aOffset);
    EndSeek();
    TBool next = ETrue;
    TBool notify( ETrue );
    if (EPlayFromCollection == iPlaySource && iPlaylist)
        {
        // Ignore command if playlist empty
        if ( iPlaylist->Count() <= 0 )
            {
            next = EFalse;
            notify = EFalse;
            }
        else
            {
            while (aOffset-->0)
                {
                if (!iPlaylist->Next( aIgnoreRepeat ))
                    {
                    iClientList->SendMsgL(
                        TMPXPlaybackMessage(TMPXPlaybackMessage::
                                            EReachedEndOfPlaylist));
                    iNextState = EPbStateStopped;
                    if (iPlaylist->IsSingleItemPlaylist())
                        {
                        next = EFalse;
                        }
                    break;
                    }
                }
            }
        }

    // Check if repeat mode is on and playing from a file or URI
    else if ( iProperties[EPbPropertyRepeatMode] != EPbRepeatOff )
        {
        if ( EPlayFromFile == iPlaySource )
            {
            TRAP_IGNORE( InitL( NULL, NULL, &iFile, iAccessPoint ));            
            }
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        else if ( EPlayFromFile64 == iPlaySource )
            {
            TRAP_IGNORE( Init64L( &iFile64, iAccessPoint ));            
            }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
        else if ( EPlayFromUri == iPlaySource )
            {
            TRAP_IGNORE( InitL( iUri, &( KNullDesC8 ), NULL, iAccessPoint ));            
            }
        else
            {
            HandleStopL();
            }
        next = EFalse;
        }
    else
        {
        HandleStopL();
        next = EFalse;
        }
    
    if (next)
        {
        // query collection for next song
        MediaFromCollectionL();
        }
    else if ( notify )
        { // trigger UI to update duration
        iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleNextL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle previous command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandlePreviousL(
    TInt aOffset /*= -1*/,
    TBool aIgnoreRepeat /*=EFalse*/ )
    {
    MPX_ASSERT(aOffset<0);
    MPX_DEBUG2("CMPXPlaybackEngine::HandlePreviousL(%d) entering", aOffset);
    EndSeek();
    if (EPlayFromCollection == iPlaySource && iPlaylist)
        {
        // Ignore command if playlist empty
        if ( iPlaylist->Count() > 0 )
            {
            // If first item in list and repeat is off, then replay the song
            if ( 0 == iPlaylist->Index() &&
                 CMPXCollectionPlaylist::ERepeatOff == iPlaylist->RepeatMode() )
                {
                if ( EPbStatePlaying == iState ||
                     EPbStatePaused == iState )
                    {
                    HandleReplayL();
                    }
                }
            else
                {
                aOffset = -aOffset;
                while (aOffset-->0)
                    {
                    if (!iPlaylist->Previous( aIgnoreRepeat ))
                        {
                        HandleStopL();
                        break;
                        }
                    }
                MediaFromCollectionL();
                }
            }
        }
    else
        {
        HandleStopL();
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandlePreviousL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle replay command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleReplayL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleReplayL(): entering");
    EndSeek();
    iProperties[EPbPropertyPosition] = 0;
    if ( PluginL() )
        {
        TMPXPlaybackState prevState( iState );
        if ( iState == EPbStatePlaying )
            {
            // Need to send pause command to plugin first so that it will
            // not keep playing the rest of it's buffer
            MPX_DEBUG1("CMPXPlaybackEngine::HandleReplayL() send pause command");
            iState = EPbStatePaused;
            PluginL()->CommandL(EPbCmdPause);
            }
        PluginL()->SetL(EPbPropertyPosition,
                        iProperties[EPbPropertyPosition]);
        if ( prevState == EPbStatePlaying )
            {
            PluginL()->CommandL(EPbCmdPlay);
            }
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleReplayL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle start seeking command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleStartSeekL(TBool aForward)
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStartSeekL(): entering");
    if (iState==EPbStatePlaying ||
        iState==EPbStatePaused ||
        iState==EPbStateSeekingForward ||
        iState==EPbStateSeekingBackward)
        {
        Suspend();
        if (iState != EPbStateSeekingForward &&
            iState != EPbStateSeekingBackward)
            {
            iNextState = iState;
            }

        SetStateL(aForward?EPbStateSeekingForward:EPbStateSeekingBackward);

        PluginL()->CommandL(EPbCmdPause);

        TCallBack cb(SeekTimerCallback,this);
        iSeekStep = aForward ? KPbInitialSeekStepMilliSeconds :
                               -KPbInitialSeekStepMilliSeconds;
        if ( iSeekTimer->IsActive() )
            iSeekTimer->Cancel();
        iSeekTimer->Start(KPbSeekIntervalMicroSeconds,
                          KPbSeekIntervalMicroSeconds, cb);
        MPX_DEBUG2("CMPXPlaybackEngine sends msg EStateChanged to %d", iState);
        iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStartSeekL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle stop seeking command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleStopSeekingL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStopSeekingL(): entering");

    // Only perform actions if we are currently in seeking state
    if (iState == EPbStateSeekingForward ||
        iState == EPbStateSeekingBackward)
        {
        EndSeek();
        if (EPbStateNotInitialised == iPluginState &&
            EPlayFromCollection == iPlaySource )
            {
            iPreservedPosition = iProperties[EPbPropertyPosition];
            MediaFromCollectionL();
            }
        else
            {
            PluginL()->SetL(EPbPropertyPosition,iProperties[EPbPropertyPosition]);
            iState = iNextState;
            switch ( iState )
                {
                case EPbStatePlaying:
                    {
                    iClientList->SendMsgL( // update UI icons
                       TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
                    PluginL()->CommandL( EPbCmdPlay );
                    break;
                    }
                case EPbStatePaused:
                    {
                    iClientList->SendMsgL( // update UI icons
                        TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged,iState));
                    break;
                    }
                default:
                    break;
                }
            }
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleStopSeekingL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle increase volume command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleIncreaseVolumeL()
    {
    TInt level( iProperties[EPbPropertyVolume] );   	
#if defined(__HIGH_RESOLUTION_VOLUME)
    // if we already rounded up last time, don't have to increase,
    // reset flag
    if (!iVolRoundedUp)
        {
        level += iVolumeIncrement;
        }
#else    	
    level += iVolumeIncrement;
#endif
    // Ensure that level is within min and max values
    if (level > KPbPlaybackVolumeLevelMax)
        {
        level = KPbPlaybackVolumeLevelMax;
        }
    if (level < KPbPlaybackVolumeLevelMin)
        {
        level = KPbPlaybackVolumeLevelMin;
        }

    SetL(EPbPropertyVolume, level);
    }

// ----------------------------------------------------------------------------
// Handle decrease volume command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleDecreaseVolumeL()
    {
    TInt level( iProperties[EPbPropertyVolume] - iVolumeIncrement );

    // Ensure that level is within min and max values
    if (level > KPbPlaybackVolumeLevelMax)
        {
        level = KPbPlaybackVolumeLevelMax;
        }
    if (level < KPbPlaybackVolumeLevelMin)
        {
        level = KPbPlaybackVolumeLevelMin;
        }

    SetL(EPbPropertyVolume, level);
    }

// ----------------------------------------------------------------------------
//  Handle set volume command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleSetVolumeL(TInt aVolume)
    {
    TInt level = aVolume;

    // Ensure that level is within min and max values
    if (level > KPbPlaybackVolumeLevelMax)
        {
        level = KPbPlaybackVolumeLevelMax;
        }
    if (level < KPbPlaybackVolumeLevelMin)
        {
        level = KPbPlaybackVolumeLevelMin;
        }

    SetL(EPbPropertyVolume, level);
    }

// ----------------------------------------------------------------------------
// Handle mute command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleMuteL(TBool aMute)
    {
    SetL(EPbPropertyMute, aMute);
    }

// ----------------------------------------------------------------------------
// Handle close command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCloseL( TInt aData )
    {
    MPX_DEBUG1("CMPXPlaybackEngine::HandleCloseL(): entering");
    if (iPluginHandler->Plugin())
        {
        TRAP_IGNORE(DoStopL());
        iNextState = EPbStateNotInitialised;
        TRAP_IGNORE(iPluginHandler->Plugin()->CommandL(EPbCmdClose, aData));
        DoClose();
        }
    if (iInitializer)
        {
        iInitializer->Close();
        }
    MPX_DEBUG1("CMPXPlaybackEngine::HandleCloseL(): exiting");
    }

// ----------------------------------------------------------------------------
// Handle a custom command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleEffectL(TInt aData)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::HandleEffectL()");
    if(iPluginHandler->Plugin())
        {
        iPluginHandler->Plugin()->CommandL(EPbApplyEffect, aData);
        }
    }

// ----------------------------------------------------------------------------
// Handle a custom command
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleDisableEffectL()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::HandleDisableEffectL()");
    if(iPluginHandler->Plugin())
        {
        iPluginHandler->Plugin()->CommandL(EPbCmdDisableEffect);
        }
    }

// ----------------------------------------------------------------------------
// Handle request to close a specific item
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleCloseItemL( TInt aItemId )
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::HandleCloseItemL()");

    if( iPlaylist )
        {
        // If the item is currently in play, close playback or
        // if item is currently in the initializer
        if( iPlaylist->Path().Id().ApproxEqual( aItemId  ) )
            {
            HandleStopL();
            }
        else if(iInitializer)
            {
            TInt nextIndex;
            TBool repeatMode;
            repeatMode = iPlaylist->RepeatMode();
            TBool nextFound = iPlaylist->NextIndex( repeatMode, nextIndex );
            TMPXItemId nextItemId = iPlaylist->Path().IdOfIndex( nextIndex );
            
            if((iInitializer->CurrentInitItemId().iId2 == aItemId) || 
                ( iInitNext && nextFound && nextItemId.ApproxEqual( aItemId )))
                {
                iInitializer->Close();
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// Sets the plug-in properties
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetPropertiesL()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::SetPropertiesL()");
    iMaxSeekStep = iProperties[EPbPropertyDuration]/KPercentMultiplier*KPbMaxSeekStepPercent;
    MPX_DEBUG2("CMPXPlaybackEngine::SetPropertiesL(): iMediaDuration                   = %d", iMediaDuration);
    MPX_DEBUG2("CMPXPlaybackEngine::SetPropertiesL(): iProperties[EPbPropertyDuration] = %d", iProperties[EPbPropertyDuration]);
    if ( Abs(iMediaDuration - iProperties[EPbPropertyDuration]) >
         KPbDurationChangeThreshold )
        {
        MPX_DEBUG1("CMPXPlaybackEngine::SetPropertiesL(): Duration not equal");
        // Set duration if it's different than what collection has
        if ( iPlaylist)
            {
            if ( iPlaylist->Count() && !iPlaylist->EmbeddedPlaylist() )
                {
                MPX_DEBUG1("CMPXPlaybackEngine::SetPropertiesL(): Setting duration");
                CMPXMedia* media( CreateMediaToSetLC( iPlaylist->Path() ));

                iMediaDuration = iProperties[EPbPropertyDuration];
                media->SetTObjectValueL( KMPXMediaGeneralDuration,
                                         iProperties[EPbPropertyDuration] );
                iPlaylist->SetL( *media );
                CleanupStack::PopAndDestroy( media );
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// Stop seeking timer
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::EndSeek()
    {
    if (iSeekTimer)
        {
        iSeekTimer->Cancel();
        }
    }

// ----------------------------------------------------------------------------
// Set playback state
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetStateL(TMPXPlaybackState aState)
    {
    MPX_DEBUG2("CMPXPlaybackEngine::SetStateL(%d): entering", aState);

    TMPXPlaybackMessage msg( TMPXPlaybackMessage::EStateChanged,
                             aState );
    SetStateL( msg );
    MPX_DEBUG1("CMPXPlaybackEngine::SetStateL(): exiting");
    }

// ----------------------------------------------------------------------------
// Set playback state
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetStateL( TMPXPlaybackMessage& aMsg )
    {
    MPX_DEBUG1("CMPXPlaybackEngine::SetStateL(aMsg): entering");

    TMPXPlaybackState state( static_cast<TMPXPlaybackState>( aMsg.Type() ));
    MPX_DEBUG2("CMPXPlaybackEngine::SetStateL(): state = %d", state);

    if ( state!=iState )
        {
        iState = state;
        iAutoResumeHandler->HandlePlaybackStateChange( iState );
        MPX_DEBUG2("CMPXPlaybackEngine sends msg EStateChanged to %d", iState);
        iClientList->SendMsgL( aMsg );
        }
    // Restart progress timer
    if (EPbStatePlaying == iState && !iProgressTimer->IsActive())
        {
        iProgressTimer->Start(ETwelveOClock,this);
        }
    MPX_DEBUG1("CMPXPlaybackEngine::SetStateL(aMsg): exiting");
    }

// ----------------------------------------------------------------------------
// Set player activated
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetPlayerActivated(TBool aActive)
    {
    MPX_DEBUG3("CMPXPlaybackEngine::SetPlayerActivated old = %d, new = %d",
                iPlayerActive, aActive);
    if (iPlayerActive != aActive)
        {
        iPlayerActive = aActive;
        // triggle clients to update playback state
        TRAP_IGNORE(iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EStateChanged, iState)));
        MPX_DEBUG1("Playback Engine notify server active player chnaged.");
        TRAP_IGNORE(iObserver.HandleActiveEngineL(this, iPlayerActive));
        }
    MPX_DEBUG1("CMPXPlaybackEngine::SetPlayerActivated(): exiting");
    }

// ----------------------------------------------------------------------------
// Execute a async task
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::ExecuteTask(
    TInt aTask,
    TInt aParamData,
    TAny* aPtrData,
    const CBufBase& aBuf,
    TAny* aCallback,
    CBase* aCObject1,
    CBase* aCObject2)
    {
    TRAPD(err, ExecuteTaskL(aTask, aParamData, aPtrData,
                            aCallback, aBuf, aCObject1, aCObject2));
    if (KErrNone != err)
        {
        HandleError(aTask, err, aParamData);
        }
    }

// ----------------------------------------------------------------------------
// Indicates that a task was terminated with an error.
// This will be called in case of a plugin update/removal.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleTaskError(
    TInt aTask,
    TAny* /*aPtrData*/,
    TAny* /*aCallback*/,
    TInt aError)
    {
    MPX_FUNC("CMPXCollectionClientContext::HandleTaskError");
    HandleError(aTask, aError);
    }

// ----------------------------------------------------------------------------
// Execute a async task
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::ExecuteTaskL(
    TInt aTask,
    TInt aParamData,
    TAny* aPtrData,
    TAny* aCallback,
    const CBufBase& /*aBuf*/,
    CBase* aCObject1,
    CBase* /*aCObject2*/)
    {
    MPX_DEBUG5("-->CMPXPlaybackEngine::ExecuteTaskL 0x%08x cb 0x%08x (%d, %d)",
                this, aCallback, aTask, aParamData);
    if ( aTask != EHandleNext )
        {
        ASSERT(aCallback);
        iCallback = static_cast<MMPXPlaybackEngineObserver*>(aCallback);
        }
    CMPXPlaybackPlugin* p = aPtrData ?
                            static_cast<CMPXPlaybackPlugin*>(aPtrData) :
                            iPluginHandler->Plugin();
    switch(aTask)
        {
        case EProperty:
            {
            CheckPtrL(p);
            p->PropertyL(static_cast<TMPXPlaybackProperty>(aParamData));
            break;
            }
        case EMedia:
            {
            iMediaAttrs.Reset();
            CMPXCommand* cmd( static_cast<CMPXAttributeSpecs*>(aCObject1));
            const TDesC& attr = cmd->ValueText( KMPXCommandMediaAttribute );
            TPtrC8 ptr = MPXUser::Ptr( attr );
            RDesReadStream readStream( ptr );
            CleanupClosePushL( readStream );
            // Internalize attributes
            ::InternalizeL( iMediaAttrs, readStream );
            CleanupStack::PopAndDestroy( &readStream );
            CheckPtrL( p );
            TMPXPlaybackState state(State());
            if (( EPbStateInitialising == state ||
                  EPbStatePlaying == state ||
                  EPbStatePaused == state ||
                  EPbStateStopped == state ||
                  EPbStateBuffering == state ||
                  EPbStateDownloading == state ||
                  EPbStateInitialised == state ))
                {
                CMPXAttributeSpecs* specs( NULL );
                if ( cmd->IsSupported( KMPXCommandMediaAttributeSpecs ) )
                    {
                    specs = cmd->Value<CMPXAttributeSpecs>( KMPXCommandMediaAttributeSpecs );
                    User::LeaveIfNull(specs);
                    }
                p->MediaL( iMediaAttrs.Array(), specs );
                }
            else
                {
                CMPXMedia *media = CMPXMedia::NewL();
                CleanupStack::PushL(media);
                media->SetTObjectValueL(KMPXMediaColDetailMediaNotAvailable,
                                        ETrue);
                HandleMedia(media, KErrNone);
                CleanupStack::PopAndDestroy(media);
                }
            }
            break;
        case ESubPlayerNames:
            {
            CheckPtrL(p);
            p->SubPlayerNamesL();
            break;
            }
        case EHandleNext:
            {
            HandleNextL();
			if( iPlaylist )
			    {
                iClientList->SendMsgL(TMPXPlaybackMessage(
                            TMPXPlaybackMessage::EMediaChanged));
			    }
            SetStateL( iState );
            iTaskQueue->CompleteTask();
            break;
            }
        default:
            ASSERT(0);
        }
    MPX_DEBUG4("<--CMPXPlaybackEngine::ExecuteTaskL 0x%08x (%d, %d): Exiting",
                this, aTask, aParamData);
    }

// ----------------------------------------------------------------------------
// Error happens upon request
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::HandleError(
    TInt aTask,
    TInt aError,
    TInt aParamData /* = 0 */)
    {
    MPX_DEBUG3("-->CMPXPlaybackEngine::HandleError() 0x%08x: aError %d", this, aError);
    switch (aTask)
        {
        case EProperty:
            iCallback->HandleProperty(
                   static_cast<TMPXPlaybackProperty>(aParamData),
                   0,aError);
            iCallback = NULL;
            break;
        case EMedia:
            {
            iCallback->HandleMedia(NULL, aError);
            iCallback = NULL;
            }
            break;
        case ESubPlayerNames:
            {
            TUid piid=TUid::Uid(aParamData);
            iCallback->HandleSubPlayerNames(piid, NULL, ETrue, aError);
            iCallback = NULL;
            }
            break;
        default:
            ASSERT(0);
            break;
        }
    iTaskQueue->CompleteTask();
    MPX_DEBUG3("<--CMPXPlaybackEngine::HandleError() 0x%08x: aError %d", this, aError);
    }

// ----------------------------------------------------------------------------
// Get command from state
// ----------------------------------------------------------------------------
//
TMPXPlaybackCommand CMPXPlaybackEngine::CommandFromState(
    TMPXPlaybackState aState )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::CommandFromState(): aState %d", aState);
    TMPXPlaybackCommand cmd(EPbCmdStop);
    switch ( aState )
        {
        case EPbStatePlaying:
            cmd = EPbCmdPlay;
            break;
        case EPbStatePaused:
            cmd = EPbCmdPause;
            break;
        case EPbStateStopped:
            cmd = EPbCmdStop;
            break;
        default:
            break;
        }
    return cmd;
    }

// ----------------------------------------------------------------------------
// Do close
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::DoClose()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::DoClose()");
    if (iTaskQueue)
        {
        iTaskQueue->CancelRequests();
        }

    iAutoResumeHandler->CancelResumeTimer();

    if (iProgressTimer)
        {
        iProgressTimer->Cancel();
        }

    if (iSeekTimer)
        {
        iSeekTimer->Cancel();
        }

    iCmdBuffer->ClearCommands();

    iFile.Close();
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    iFile64.Close();
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    delete iPlaylist;
    iPlaylist = NULL;
    delete iUri;
    iUri = NULL;
    iItemId = KMPXInvalidItemId;
    iPlaySource = EPlayNone;
    iState = EPbStateNotInitialised;
    iNextState = EPbStateNotInitialised;
    }

// ----------------------------------------------------------------------------
// URI from collection
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::MediaFromCollectionL()
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::MediaFromCollectionL");
    if (iInitNext)
        {
        iInitializer->Init(*iPlaylist);
        iInitializer->SetActiveL();
        }
    else
        {
        RArray<TMPXAttribute> attrs;
        CleanupClosePushL(attrs);
        attrs.AppendL(KMPXMediaGeneralId);
        attrs.AppendL(KMPXMediaGeneralUri);
        attrs.AppendL(KMPXMediaGeneralMimeType);
        attrs.AppendL(KMPXMediaGeneralLastPlaybackPosition);
        attrs.AppendL(KMPXMediaGeneralFlags);

        if (iPlaylist)	///in some cases this could be empty
			{
        	iPlaylist->MediaL(attrs.Array(),*this);
			}

        CleanupStack::PopAndDestroy(&attrs);
        }
    }

// ----------------------------------------------------------------------------
// Save playback position
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SavePlaybackInfoL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::SavePlaybackInfoL() entering");

    if ( iPlaylist)
        {
        if (iPlaylist->Count() && !iPlaylist->EmbeddedPlaylist())
            {
            CMPXMedia* media( CreateMediaToSetLC( iPlaylist->Path() ));

            // Save position, if at the end, then save position is 0
            // Due to timer callbacks, the current position may not be exactly
            // at the same value as the duration, thus must give it a
            // threshold to be within.
            TInt savePosition( iProperties[EPbPropertyPosition] );
            if ( Abs( iProperties[EPbPropertyDuration] - savePosition ) <
                 KPbPositionChangeThreshold )
                {
                savePosition = 0;
                }
            media->SetTObjectValueL( KMPXMediaGeneralLastPlaybackPosition,
                                    savePosition );
            iPlaylist->SetL( *media );
            CleanupStack::PopAndDestroy(media);
            }
        }

    MPX_DEBUG1("CMPXPlaybackEngine::SavePlaybackInfoL() exiting");
    }

// ----------------------------------------------------------------------------
// Increments play count
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SavePlaybackCompleteInfoL()
    {
    MPX_DEBUG1("CMPXPlaybackEngine::SavePlaybackCompleteInfoL() entering");

    if ( iPlaylist )
        {
        if ( iPlaylist->Count() && !iPlaylist->EmbeddedPlaylist() )
            {
            CMPXMedia* media( CreateMediaToSetLC( iPlaylist->Path() ));

            // Increment play count
            // Collection implementation will increment count by the
            // value provided
            media->SetTObjectValueL( KMPXMediaGeneralPlayCount,
                                     1 );

            // reset the last playback position to 0
            media->SetTObjectValueL( KMPXMediaGeneralLastPlaybackPosition,
                                     0 );

            // Set last play time
            TTime now;
            now.UniversalTime();
            media->SetTObjectValueL(KMPXMediaGeneralLastPlaybackTime,
                    now.Int64() );

            iPlaylist->SetL( *media );
            CleanupStack::PopAndDestroy( media );
            }
        }

    MPX_DEBUG1("CMPXPlaybackEngine::SavePlaybackCompleteInfoL() exiting");
    }

// ----------------------------------------------------------------------------
// Restore playback position if it was saved previously
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::RestorePlaybackPositionL(
    const CMPXMedia& aMedia )
    {
    MPX_FUNC("CMPXPlaybackEngine::RestorePlaybackPositionL() ");

    // Restore Position
    iProperties[EPbPropertyPosition] = 0;

        // Check media
        if (aMedia.IsSupported(KMPXMediaGeneralLastPlaybackPosition))
            {
            iPositionFromMedia =
                aMedia.ValueTObjectL<TInt>( KMPXMediaGeneralLastPlaybackPosition ); 
            }
        MPX_DEBUG2("CMPXPlaybackEngine::RestorePlaybackPositionL iPositionFromMedia %d", iPositionFromMedia ); 
    }


// ----------------------------------------------------------------------------
// Restore playback state if it was saved previously
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::RestorePlaybackStateL()
    {
    MPX_FUNC("CMPXPlaybackEngine::RestorePlaybackStateL() ");
    // Restore State
    if ( EPbStateNotInitialised != iPreservedState )
        {
        iNextState = iPreservedState;
        iPreservedState = EPbStateNotInitialised;
        }   
    }

// ----------------------------------------------------------------------------
// Sets the volume increment depending on accessory state
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetVolumeIncrement( TMPXPlaybackAccessoryMode aMode )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::SetVolumeIncrement(%d) entering", aMode);

    switch ( aMode )
        {
#ifdef __ACCESSORY_FW
        case EPbAccessoryWiredHeadset:
        case EPbAccessoryHeadphones:
#else
        case EPbAccessoryHeadset:
#endif // __ACCESSORY_FW
            {
// twentysteps  start
            	
            // check whether we are playing WMA files, if so the increment has to be KMPXLargeVolumeIncrement 
                if(iUri)
                {
                //MPX_DEBUG1("still alive one !!!!");	
		            TParsePtrC parser(*iUri);
		            
		            MPX_DEBUG2("CMPXPlaybackEngine::SetVolumeIncrement(): iUri is %S", iUri);

		            if (parser.Ext().CompareF(KWmaExtension) == 0 || parser.Ext().CompareF(KRaExtension) == 0 )
		                {
		                iVolumeIncrement = KMPXLargeVolumeIncrement;	
		                }
		            else
		                {
		                iVolumeIncrement = KMPXSmallVolumeIncrement;	
		                }
                }
            else
                {
                MPX_DEBUG1("CMPXPlaybackEngine::SetVolumeIncrement iUri is NULL");  
                iVolumeIncrement = KMPXSmallVolumeIncrement;	
                }
                         
            MPX_DEBUG2("CMPXPlaybackEngine::SetVolumeIncrement(): Headset is inserted, increment is %d", iVolumeIncrement);
//  twentysteps  end            
// uncomment this when twentysteps is removed            iVolumeIncrement = KMPXSmallVolumeIncrement;
            break;
            }
        default:
            {
            // for everything else check what is the current volume value
            // and if an not an interval of KMPXLargeVolumeIncrement,
            //  which could be the case if the last
            // time user listened to music and wired headset was plugged in,
            // then convert the value used to an increment of KMPXLargeVolumeIncrement.
            MPX_DEBUG2("CMPXPlaybackEngine::SetVolumeIncrement(): NOT a headset, increment is %d", KMPXLargeVolumeIncrement);
            iVolumeIncrement = KMPXLargeVolumeIncrement;
            if ( iProperties[EPbPropertyVolume] > KPbPlaybackVolumeLevelMin && 
                 iProperties[EPbPropertyVolume] < KPbPlaybackVolumeLevelMax)
                {    
#if defined(__HIGH_RESOLUTION_VOLUME)
                // if we will round up the volume to next increment of 10, set the flag
                // so when the headset is unplugged, we just increment it again.        
                if (iProperties[EPbPropertyVolume] % KMPXLargeVolumeIncrement)
                    {
                    iVolRoundedUp = ETrue;
                    }
#endif
                while ( iProperties[EPbPropertyVolume] % KMPXLargeVolumeIncrement &&
                        iProperties[EPbPropertyVolume] <= KPbPlaybackVolumeLevelMax )
                    {
                    // increase volume until it's a valid increment of KMPXLargeVolumeIncrement
                    iProperties[EPbPropertyVolume]++;
                    }
                }
            break;
            }
        }

    MPX_DEBUG1("CMPXPlaybackEngine::SetVolumeIncrement() exiting");
    }

// ----------------------------------------------------------------------------
// Marks the current item as corrupted
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::MarkItemCorrupted( const TBool aCorrupted )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::MarkItemCorrupted(%d) entering", aCorrupted);

    TRAP_IGNORE( SetFlagBitsL( aCorrupted, KMPXMediaGeneralFlagsIsCorrupted ));

    MPX_DEBUG1("CMPXPlaybackEngine::MarkItemCorrupted() exiting");
    }

// ----------------------------------------------------------------------------
// Marks the current item as invalid
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::MarkItemInvalid( const TBool aInvalid )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::MarkItemInvalid(%d) entering", aInvalid);

    TRAP_IGNORE( SetFlagBitsL( aInvalid, KMPXMediaGeneralFlagsIsInvalid ));

    MPX_DEBUG1("CMPXPlaybackEngine::MarkItemInvalid() exiting");
    }

// ----------------------------------------------------------------------------
// Marks the current item as DRM invalid
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::MarkItemDrmInvalid( const TBool aDrmInvalid )
    {
    MPX_DEBUG2("CMPXPlaybackEngine::MarkItemDrmInvalid(%d) entering", aDrmInvalid);

    TRAP_IGNORE( SetFlagBitsL( aDrmInvalid, KMPXMediaGeneralFlagsIsDrmLicenceInvalid ));

    MPX_DEBUG1("CMPXPlaybackEngine::MarkItemDrmInvalid() exiting");
    }

// ----------------------------------------------------------------------------
// Sets flag bits
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::SetFlagBitsL(
    const TBool aSet,
    const TUint aFlag )
    {
    MPX_DEBUG3("CMPXPlaybackEngine::SetFlagBitsL(%d, %d) entering", aSet, aFlag);

    if ( iPlaylist)
        {
        if (iPlaylist->Count() && !iPlaylist->EmbeddedPlaylist())
            {
            CMPXMedia* media( CreateMediaToSetLC( iPlaylist->Path() ));

            // Set flag
            TUint flag( 0 );
            if ( aSet )
                {
                flag = KMPXMediaGeneralFlagsSetOrUnsetBit;
                }
            flag |= aFlag;
            media->SetTObjectValueL( KMPXMediaGeneralFlags,
                                     flag );
            iPlaylist->SetL( *media );
            CleanupStack::PopAndDestroy( media );
            }
        }

    MPX_DEBUG1("CMPXPlaybackEngine::SetFlagBitsL() exiting");
    }

// ----------------------------------------------------------------------------
// Creates a CMPXMedia object to be used to Set attributes
// ----------------------------------------------------------------------------
//
CMPXMedia* CMPXPlaybackEngine::CreateMediaToSetLC( const CMPXCollectionPath& aPath )
    {
    MPX_DEBUG1("-->CMPXPlaybackEngine::CreateMediaToSetLC()");

    // Create new Media object to set
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL(media);
    media->SetTObjectValueL( KMPXMediaGeneralType,
                             EMPXItem );
    media->SetTObjectValueL( KMPXMediaGeneralCategory,
                             EMPXSong );
    media->SetTObjectValueL( KMPXMediaGeneralCollectionId,
                             aPath.Id( CMPXCollectionPath::ECollectionUid ));

    if (iItemId != KMPXInvalidItemId)
        {
        media->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, iItemId);
        }
    else if (iUri)
        {
        media->SetTextValueL( KMPXMediaGeneralUri, *iUri);
        }
    else
        {
        media->SetTObjectValueL<TMPXItemId>( KMPXMediaGeneralId , aPath.Id() );
        }
    MPX_DEBUG1("<--CMPXPlaybackEngine::CreateMediaToSetLC()");
    return media;
    }

// ----------------------------------------------------------------------------
// Sends a request to iMediaHelper to request media
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::RequestMediaL()
    {
    MPX_FUNC( "CMPXPlaybackEngine::RequestMediaL" );
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    cmd->SetTObjectValueL<TInt>(KMPXCommandGeneralId, KMPXCommandContentIdMedia);
    CBufBase* buf = CBufFlat::NewL( KMPXBufGranularity );
    CleanupStack::PushL( buf );

    // Setup array buffer
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    attrs.AppendL( KMPXMediaGeneralBasic );
    attrs.AppendL( KMPXMediaGeneralUri );
    attrs.AppendL( KMPXMediaMusicAlbumArtFileName );
    attrs.AppendL( KMPXMediaMusicArtist );
    attrs.AppendL( KMPXMediaMusicAlbum );

    RBufWriteStream writeStream( *buf );
    CleanupClosePushL( writeStream );
    // externalize attributes array
    ::ExternalizeL(attrs.Array(), writeStream);
    // Close and compress buffer
    writeStream.CommitL();
    buf->Compress();
    CleanupStack::PopAndDestroy( &writeStream );
    CleanupStack::PopAndDestroy( &attrs );
    TPtrC ptr = MPXUser::Ptr( buf->Ptr( 0 ) );
    cmd->SetTextValueL( KMPXCommandMediaAttribute, ptr );

    CMPXAttributeSpecs* attrSpecs = CMPXAttributeSpecs::NewL();
    CleanupStack::PushL(attrSpecs);
    cmd->SetCObjectValueL<CMPXAttributeSpecs>(
        KMPXCommandMediaAttributeSpecs, attrSpecs );
    CleanupStack::PopAndDestroy(attrSpecs);

    iMediaHelper->MediaL( iPlaylist->Path(), cmd, NULL, ETrue, iClientList );
    CleanupStack::PopAndDestroy( buf );
    // Onwership of media is passed to mediahelper class
    CleanupStack::Pop( cmd );
    }

#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
// ----------------------------------------------------------------------------
// Initialises from file.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::Init64L(const RFile64& aFile)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::Init64L(const RFile64& aFile)");
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromFile64;
    iNextState=EPbStateNotInitialised;
    iFile64.Duplicate(aFile);
    TRAPD( err, Init64L(&iFile64) );
    if ( KErrNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Initialises from file.
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXPlaybackEngine::InitStreaming64L(const RFile64& aFile, const TInt aAccessPoint)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::InitStreaming64L(const RFile64& aFile, const TInt aAccessPoint)");
    iPluginUid = KNullUid; // Reset plugin uid for current item
    iInitNext=EFalse;
    HandleCloseL();
    iPlaySource=EPlayFromFile64;
    iNextState=EPbStateNotInitialised;
    iFile64.Duplicate(aFile);
    iAccessPoint = aAccessPoint;
    iAccessPointSet = ETrue;
    TRAPD( err, Init64L(&iFile64,aAccessPoint));
    if ( KErrNotFound == err )
        {
        // Mark item as Invalid
        MarkItemInvalid( ETrue );
        User::Leave( err );
        }
    }

// ----------------------------------------------------------------------------
// Initialise helper method
// ----------------------------------------------------------------------------
//
void CMPXPlaybackEngine::Init64L(RFile64* aFile, TInt aAccessPoint)
    {
    MPX_FUNC_EX("CMPXPlaybackEngine::Init64L(RFile64* aFile,TInt aAccessPoint)");
    CMPXPlaybackPlugin* p( NULL );
    if ( iPluginHandler->Plugin() )
        {
        TRAP_IGNORE(iPluginHandler->Plugin()->CommandL(EPbCmdClose));
        }
    
    if (aFile)
        {
        iPluginHandler->SelectPlayer64L(*aFile);
        }
    else
        { // should never happen
        ASSERT(0);
        }

    p=iPluginHandler->Plugin();
    CheckPtrL(p);

    if (p->Uid()!=iPluginUid)
        { // new plugin or new playlist
        iPluginUid = p->Uid();
        iInitVolume = ETrue;
        p->PropertyL( EPbPropertyVolume );
        iClientList->SendMsgL(
            TMPXPlaybackMessage(TMPXPlaybackMessage::EPlayerChanged));
        }
    // Stop and close opened file
    TRAP_IGNORE(DoStopL(EFalse));
    TRAP_IGNORE(p->CommandL(EPbCmdClose));

    // If playing from a playlist, send the index as a data
    // parameter.  This helps performance as the client
    // doesn't need to request the index separately
    TInt index( KErrNotFound );
    if ( EPlayFromCollection == iPlaySource )
        {
        index = iPlaylist->Index();
        }
    
    TMPXPlaybackMessage msg( TMPXPlaybackMessage::EStateChanged,
                             EPbStateInitialising,
                             index );
    SetStateL( msg );
    iProperties[EPbPropertyPosition]=0;
    
        if ( KErrNotFound != iPreservedPosition )
              {
              iProperties[EPbPropertyPosition] = iPreservedPosition;
              iPreservedPosition = KErrNotFound;
              }
        else if ( KErrNotFound != iPositionFromMedia )
            {
            iProperties[EPbPropertyPosition] = iPositionFromMedia;
            iPositionFromMedia = KErrNotFound;
            }
        MPX_DEBUG2("CMPXPlaybackEngine::InitL iPropertyPosition %d", iProperties[EPbPropertyPosition] );
            
    // Check if version2 interface is supported.
    CDesCArray* interfaces = iPluginHandler->SupportedInterfacesL( p->Uid() );
    TBool version2InterfaceSupported = EFalse;
    if (interfaces->MdcaCount())
        {
        TInt pos(0);            
        version2InterfaceSupported = !interfaces->FindIsq(KMPXPlaybackPluginVersion2, pos);
        }
    delete interfaces;
        
    // cast the plugin to use our interface
    if (version2InterfaceSupported)
        {
        CMPXPlaybackPluginVersion2* plugin = NULL;
        plugin = static_cast<CMPXPlaybackPluginVersion2*>(p);
        
        // if cast was successful, then init streaming with access point
        if (plugin)
            {
            if ( iAccessPointSet )
                {
                plugin->InitStreaming64L( *aFile, aAccessPoint, iProperties[EPbPropertyPosition] ); 
                }
            else
                {
                plugin->Initialise64L( *aFile, iProperties[EPbPropertyPosition] ); 
                }
            }
        else
            {
            MPX_DEBUG1("CMPXPlaybackEngine::Init64L(): -- failure to convert to expected interface");                
            }
        }
    else
        {
        MPX_DEBUG1("CMPXPlaybackEngine::Init64L(): expected interface not supported");
        User::Leave( KErrNotFound );
        }
    }
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

// ----------------------------------------------------------------------------
// Cancel timer. 
// ----------------------------------------------------------------------------
void CMPXPlaybackEngine::CancelSyncMsgTimer()
    {
    MPX_FUNC("CMPXPlaybackEngine::CancelSyncMsgTimer()");
    // Cancel timer.
    if ( iSyncMsgTimer && iSyncMsgTimer->IsActive() )
        {
        MPX_DEBUG1("CMPXPlaybackEngine::CancelSyncMsgTimer(): Timer active, cancelling");
        iSyncMsgTimer->Cancel();
        }
    }
// ----------------------------------------------------------------------------
// Callback for timer.
// ----------------------------------------------------------------------------
TInt CMPXPlaybackEngine::SyncMsgTimerCallback(TAny* aPtr)
    {
    MPX_FUNC("CMPXPlaybackEngine::SyncMsgTimerCallback()");
    CMPXPlaybackEngine* ptr =
        static_cast<CMPXPlaybackEngine*>(aPtr);
    ptr->StopWaitLoop();
    return KErrNone;
    }
// ----------------------------------------------------------------------------
// Handle a synchronous message
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackEngine::HandlePlaybackSyncMessage (const CMPXMessage& aMsg)
    {    
    MPX_FUNC("CMPXPlaybackEngine::HandlePlaybackSyncMessage()");    
    TInt err = iClientList->SendSyncMsg(&aMsg);
    if (err != KErrNone)
        {
        return err;
        }
    // Cancel timer.
    CancelSyncMsgTimer();
    // Start timer in case there is no callback from primary client. 
    iSyncMsgTimer->Start(
        KMPXSyncMsgTimer,
        KMPXSyncMsgTimer,
        TCallBack(SyncMsgTimerCallback, this ));
            
    // Start wait loop until we get a callback from primary client
    if ( !iSyncMsgWait->IsStarted() )
        {
        iSyncMsgWait->Start();
        }
    return KErrNone;
    }
// ----------------------------------------------------------------------------
// Stop the wait loop.
// ----------------------------------------------------------------------------
void CMPXPlaybackEngine::StopWaitLoop()
    {
    MPX_FUNC("CMPXPlaybackEngine::StopWaitLoop()");
    // Cancel timer
    CancelSyncMsgTimer();
    
    // Stop wait loop to unblock.
    if ( iSyncMsgWait->IsStarted() )
        {
        MPX_DEBUG1("CMPXPlaybackEngine::StopWaitLoop(): Stopping the wait loop.");
        iSyncMsgWait->AsyncStop();
        }
    }
// End of file
