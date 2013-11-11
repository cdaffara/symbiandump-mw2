/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This is the main implementation of radio server.
*
*/



// INCLUDE FILES
#include    <mmf/common/mmfstandardcustomcommands.h>
#include    <centralrepository.h>
#include    <CoreApplicationUIsSDKCRKeys.h>
#include    <AudioPreference.h>

#include    "RadioServer.h"
#include    "RadioServerSession.h"
#include    "RadioServerShutdown.h"
#include    "RadioServerSettings.h"
#include    "RadioServerFMTuner.h"
#include    "RadioDebug.h"

// CONSTANTS
const TInt KFMDefaultFreq           = 87500000; // 87.5 MHz
const TInt KJapaneseFMDefaultFreq   = 76000000; // 76.0 MHz

#ifndef RD_FM_RADIO_ENHANCEMENTS
const TInt KEuroAmericaMin  =  87500000;
const TInt KEuroAmericaMax  = 108000000;
const TInt KJapanMin        =  76000000;
const TInt KJapanMax        =  90000000;
#endif //#ifdef RD_FM_RADIO_ENHANCEMENTS


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRadioServer::CRadioServer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CRadioServer::CRadioServer()
    :   CServer2(CActive::EPriorityStandard),
        iState(EStateStarted),
        iSessionCount(0),
        iSessionCountPrimary(0),
        iShutdownTimer(NULL),
        iSettings(NULL),
        iTunerControlObserver(NULL),
        iTunerControl(NULL),
        iRdsControl(NULL),
        iDevSound(NULL),
        iRepository(NULL),
        iCenRepNotifyHandler(NULL),
        iAsyncRequestQue(_FOFF( TRadioMessageRequestData, iLink )),
        iAsyncRequest(NULL),
        iSyncRequestQue(_FOFF( TRadioMessageRequestData, iLink )),
        iSyncRequest(NULL),
        iMaxSigStrength(0),
        iEnableTunerInOffline(EFalse),
        iSquelch(EFalse),
        iPreEmpted(EFalse),
        iSchedulerWait( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CRadioServer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRadioServer::ConstructL()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] ConstructL() - Start"));

    StartL(KRadioServerName);

    iDevSound = CMMFDevSound::NewL();
    TMMFPrioritySettings prioritySettings;
    prioritySettings.iPriority = (TInt)KAudioPriorityFMRadio;
    prioritySettings.iPref = (TMdaPriorityPreference)KAudioPrefRadioAudioEvent;
    iDevSound->SetPrioritySettings(prioritySettings);

    // Sign up for offline mode status notification and get the latest status
    iRepository = CRepository::NewL(KCRUidCoreApplicationUIs);
    iCenRepNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository,
            CCenRepNotifyHandler::EIntKey, KCoreAppUIsNetworkConnectionAllowed);
    iCenRepNotifyHandler->StartListeningL();
    TInt value;
    TBool offlineMode = EFalse;
    iRepository->Get(KCoreAppUIsNetworkConnectionAllowed, value);
    if ( (TCoreAppUIsNetworkConnectionAllowed)value == ECoreAppUIsNetworkConnectionNotAllowed )
        {
        offlineMode = ETrue;
        }

    // Assume Antenna is connected. Otherwise, initialization will handle it anyways.
    // After that, RadioServer gets the antenna status notification from the tuner.
    TBool antennaStatus = ETrue;

    // Max volume won't change, so just make the server remember it locally.
    iMaxVolume = iDevSound->MaxVolume();
    TInt volume = iDevSound->Volume();
    TInt left, right;
    iDevSound->GetPlayBalanceL(left, right);

    iSettings = CRadioServerSettings::NewL();
    iSettings->SetAntennaStatus(antennaStatus, EFalse);
    iSettings->SetOfflineModeStatus(offlineMode, EFalse);
    iSettings->SetVolume(volume, EFalse);
    iSettings->SetBalance(left, right, EFalse);

    // Initiate shut down unless we get client connections
    iShutdownTimer = CRadioServerShutdown::NewL();
    iShutdownTimer->Start();
#ifdef RD_TSP_CLIENT_MAPPER
    iMapper = CTspClientMapper::NewL();
    iTspState = CTspClientMapper::ERegisteredClients;
#endif // RD_TSP_CLIENT_MAPPER
    
    iSchedulerWait  = new (ELeave) CActiveSchedulerWait;

    RADIO_RDEBUG(_L("[RADIO-SVR] ConstructL() - End"));
    }

// -----------------------------------------------------------------------------
// CRadioServer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioServer* CRadioServer::NewL()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] NewL"));
    CRadioServer* self = new( ELeave ) CRadioServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CRadioServer::~CRadioServer()
    {
    if ( iShutdownTimer )
        {
        iShutdownTimer->Cancel();
        delete iShutdownTimer;
        }
    if ( iCenRepNotifyHandler )
        {
        iCenRepNotifyHandler->StopListening();
        delete iCenRepNotifyHandler;
        }
    delete iRepository;

    delete iDevSound;
    delete iSettings;
    delete iTunerControl;
    delete iTunerControlObserver;
    delete iAsyncRequest;
    delete iSchedulerWait;

    ClearQueue();
    iRdsNotifyClientIdArray.Close();

#ifdef RD_TSP_CLIENT_MAPPER
    delete iMapper;
    iClientPids.Close();
#endif // RD_TSP_CLIENT_MAPPER
    }

// -----------------------------------------------------------------------------
// CRadioServer::AddSession
// This method increases the client session count, and cancels the shutdown timer.
// The timer is reset once the last client has disconnected
// -----------------------------------------------------------------------------
//
void CRadioServer::AddSession()
    {
    if ( ++iSessionCount == 1 )
        {
        iShutdownTimer->Cancel();
        iState = EStateStarted;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::DropSession
// This method decrements the client session count, and if no clients are connected,
// it initiates the shutdown timer.
// -----------------------------------------------------------------------------
//
void CRadioServer::DropSession()
    {
    if ( --iSessionCount == 0 )
        {
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] DropSession() - Radio OFF. State[%d]"), iState);
        switch ( iState )
            {
            case EStateStarted:
            case EStateTunerOff:
                // Shutting down normally.
                iState = EStateStarted;
                iMaxSigStrength = 0;
                iEnableTunerInOffline = EFalse;
                delete iTunerControl;
                iTunerControl = NULL;
                ClearQueue();
                iShutdownTimer->Start();
                break;
            case EStateTunerOn:
                iState = EStateShutdown;
                iTunerControl->TunerOff();
                break;
            case EStatePlaying:
                iState = EStateShutdown;
                iDevSound->Stop();
                iTunerControl->TunerOff();
                break;
            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ServiceRequestL
// Main entry for Radio Server. If the cancel-request message is already outstanding,
// just let it complete. Only allow for cancel if the message hasn't been initiated yet.
// For all other requests, process the message if no other outstanding message exists.
// Otherwise, queue the message. Execution order is guaranteed for all asynchronous
// requests.
// -----------------------------------------------------------------------------
//
void CRadioServer::ServiceRequestL(
    const RMessage2& aMessage )
    {
    TRadioServerRequest req;
    TRadioMessageRequestData* data = NULL;
#ifdef RD_TSP_CLIENT_MAPPER
    iMessageTsp = aMessage;
#endif // RD_TSP_CLIENT_MAPPER
    switch( aMessage.Function() )
        {
        case ERadioServAddPrimaryClient:
            iSessionCountPrimary++;
            RegisterClientPidL(aMessage);
            RADIO_RDEBUG_INT(_L("[RADIO-SVR] ServiceRequestL() prim=[%d]"), iSessionCountPrimary);
            aMessage.Complete(KErrNone);
            break;
        case ERadioServRemovePrimaryClient:
            aMessage.Complete(ProcessRemovePrimaryClientL(aMessage));
            break;
        case ERadioServGetTunerCapabilities:
            aMessage.Complete(ProcessGetTunerCapabilities(aMessage));
            break;
        case ERadioServEnableTunerInOfflineMode:
            aMessage.Complete(ProcessEnableTunerInOfflineMode(aMessage.Int1()));
            break;
        case ERadioServGetFrequencyRange:
            aMessage.Complete(ProcessGetFrequencyRange(aMessage));
            break;
        case ERadioServGetFrequency:
            aMessage.Complete(ProcessGetFrequency(aMessage));
            break;
        case ERadioServGetForceMonoReception:
            aMessage.Complete(ProcessGetForceMonoReception(aMessage));
            break;
        case ERadioServSetSquelch:
            aMessage.Complete(ProcessSetSquelch(aMessage.Int1()));
            break;
        case ERadioServGetSquelch:
            aMessage.Complete(ProcessGetSquelch(aMessage));
            break;
        case ERadioServGetSignalStrength:
        case ERadioServGetMaxSignalStrength:
        case ERadioServGetStereoMode:
        case ERadioServForceMonoReception:
            // These requests are synchronous request from the client, however, the corresponding
            // request to the tuner control is asynchronous. Therefore, these messages need to be
            // serialized using a queue.
            data = new (ELeave) TRadioMessageRequestData;
            data->iType = (TRadioServerRequest) aMessage.Function();
            data->iMessage = aMessage;
            if ( iSyncRequest )
                {
                // Outstanding request exits; add it to the queue
                iSyncRequestQue.AddLast(*data);
                }
            else
                {
                // No outstanding request; process the request
                ProcessSyncRequest(data);
                }
            break;
        case ERadioServGetPlayerState:
            aMessage.Complete(ProcessGetPlayerState(aMessage));
            break;
        case ERadioServGetMaxVolume:
            aMessage.Complete(ProcessGetMaxVolume(aMessage));
            break;
        case ERadioServSetVolume:
            aMessage.Complete(ProcessSetVolume(aMessage.Int1()));
            break;
        case ERadioServGetVolume:
            aMessage.Complete(ProcessGetVolume(aMessage));
            break;
        case ERadioServSetVolumeRamp:
            aMessage.Complete(ProcessSetVolumeRamp(aMessage));
            break;
        case ERadioServSetMute:
            aMessage.Complete(ProcessSetMute(aMessage.Int1()));
            break;
        case ERadioServGetMuteStatus:
            aMessage.Complete(ProcessGetMuteStatus(aMessage));
            break;
        case ERadioServSetBalance:
            aMessage.Complete(ProcessSetBalance(aMessage.Int1(), aMessage.Int2()));
            break;
        case ERadioServGetBalance:
            aMessage.Complete(ProcessGetBalance(aMessage));
            break;
        case ERadioServRequestTunerControl:
        case ERadioServSetFrequencyRange:
        case ERadioServSetFrequency:
        case ERadioServStationSeek:
        case ERadioServPlay:
        case ERadioServStop:
        case ERadioServStationSeekByPTY:
        case ERadioServStationSeekByTA:
        case ERadioServStationSeekByTP:
            // These requests are asynchronous request from the client. In order to guarantee the
            // execution order these messages are serialized using a queue.
            data = new (ELeave) TRadioMessageRequestData;
            data->iType = (TRadioServerRequest) aMessage.Function();
            data->iMessage = aMessage;
            if ( iAsyncRequest )
                {
                // Outstanding request exits; add it to the queue
                iAsyncRequestQue.AddLast(*data);
                }
            else
                {
                // No outstanding request; process the request
                ProcessAsyncRequest(data);
                }
            break;
        case ERadioServGetRdsCapabilities:
            aMessage.Complete(ProcessGetRdsCapabilities(aMessage));
            break;
        case ERadioServGetRdsSignalStatus:
            aMessage.Complete(ProcessGetRdsSignalStatus(aMessage));
            break;
        case ERadioServNotifyRdsDataChange:
            aMessage.Complete(ProcessNotifyRdsDataChange(aMessage));
            break;
        case ERadioServCancelNotifyRdsDataChange:
            aMessage.Complete(ProcessCancelNotifyRdsDataChange(aMessage));
            break;
        case ERadioServSetAutomaticSwitching:
            aMessage.Complete(ProcessSetAutomaticSwitching(aMessage.Int1()));
            break;
        case ERadioServGetAutomaticSwitching:
            aMessage.Complete(ProcessGetAutomaticSwitching(aMessage));
            break;
        case ERadioServCancelAFSearch:
            aMessage.Complete(ProcessCancelAFSearch());
            break;
        case ERadioServGetProgrammeIdentification:
            aMessage.Complete(ProcessGetProgrammeIdentification(aMessage));
            break;
        case ERadioServGetProgrammeType:
            aMessage.Complete(ProcessGetProgrammeType(aMessage));
            break;
        case ERadioServGetProgrammeService:
            aMessage.Complete(ProcessGetProgrammeService(aMessage));
            break;
        case ERadioServGetRadioText:
            aMessage.Complete(ProcessGetRadioText(aMessage));
            break;
        case ERadioServGetClockTime:
            aMessage.Complete(ProcessGetClockTime(aMessage));
            break;
        case ERadioServGetTrafficAnnouncementStatus:
            aMessage.Complete(ProcessGetTrafficAnnouncementStatus(aMessage));
            break;
        case ERadioServGetTrafficProgrammeStatus:
            aMessage.Complete(ProcessGetTrafficProgrammeStatus(aMessage));
            break;
        case ERadioServCancel:
            req = (TRadioServerRequest) aMessage.Int1();
            RADIO_RDEBUG_INT(_L("[RADIO-SVR] ServiceRequestL(): Cancel[%d]"), req);
            switch ( req )
                {
                case ERadioServRequestTunerControl:
                case ERadioServSetFrequencyRange:
                case ERadioServSetFrequency:
                    // If the message has already executed or is outstanding, just let it complete.
                    // If not, remove from queue.
                    RemoveFromQueue(iAsyncRequestQue, req);
                    aMessage.Complete(KErrNone);
                    break;
                case ERadioServStationSeek:
                case ERadioServStationSeekByPTY:
                case ERadioServStationSeekByTA:
                case ERadioServStationSeekByTP:
                    // If the request is outstanding, attempt to cancel since it can take a while
                    // to complete. Otherwise, just remove from the queue.
                    if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                                            (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                                            (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                                            (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
                        {
                        RADIO_RDEBUG(_L("[RADIO-SVR] ServiceRequestL() - Seek Outstanding"));
                        if ( iAsyncRequest->iType == ERadioServStationSeek )
                            {
                            iTunerControl->CancelStationSeek();
                            }
                        else
                            {
                            iRdsControl->CancelRdsStationSeek();
                            }
                        // Disable squelching, if on
                        if ( iSquelch )
                            {
                            // Restore the last volume
                            iDevSound->SetVolume(iSettings->Volume());
                            iSquelch = EFalse;
                            }
                        }
                    else
                        {
                        RemoveFromQueue(iAsyncRequestQue, req);
                        }
                    aMessage.Complete(KErrNone);
                    break;
                default:
                    RADIO_RDEBUG(_L("[RADIO-SVR] ServiceRequestL(): default !!!"));
                    // Could be some custom command that we don't support.
                    aMessage.Complete(KErrNotSupported);
                }
            break;
        default:
            RADIO_RDEBUG(_L("[RADIO-SVR] ServiceRequestL(): default!!!"));
            // Could be some custom command that we don't support.
            aMessage.Complete(KErrNotSupported);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::NewSessionL
// This method decrements the client session count, and if no clients are connected,
// it initiates the shutdown timer.
// -----------------------------------------------------------------------------
//
CSession2* CRadioServer::NewSessionL(
    const TVersion& aVersion,
    const RMessage2& /*aMessage*/ ) const
    {
    // Test if the version specifies a repository session
    const TVersion version(KRadioServerVersionMajor,
                           KRadioServerVersionMinor,
                           KRadioServerVersionBuild);
    if ( !User::QueryVersionSupported(aVersion, version) )
        {
        User::Leave(KErrNotSupported);
        }

    CRadioServerSession* session = CRadioServerSession::NewL(*((CRadioServer*)this));
    return session;
    }

// -----------------------------------------------------------------------------
// CRadioServer::TunerOnComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::TunerOnComplete(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] TunerOnComplete(%d) State[%d] Trigger[%d]"), aError, iState, iTunerOnTrigger);
    TFourCC radioFourCC;
    // TBool antennaStatus = iSettings->IsAntennaAttached();
    
    switch ( iTunerOnTrigger )
        {
        case ETriggerTunerControl:
            if ( aError == KErrNone )
                {
                TRsFrequencyRange range;
                TInt minFreq, maxFreq;
                if ( GetFrequencyRange(range, minFreq, maxFreq) == KErrNone )
                    {
                    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] TunerOnComplete() - [%d, %d, %d]"), range, minFreq, maxFreq);
                    iSettings->SetFrequencyRange( range, minFreq, maxFreq, EFalse );
                    }
                radioFourCC.Set(iTunerCaps.iEncoding);
                TRAPD(err, iDevSound->InitializeL(*this, radioFourCC, EMMFStatePlaying));
                if ( err != KErrNone )
                    {
                    RADIO_RDEBUG_INT(_L("[RADIO-SVR] TunerOnComplete() DevSound err=[%d]"), err);
                    CompleteAsyncRequest(err);
                    }
                }
            else
                {
                CompleteAsyncRequest(aError);
                }
            break;
        case ETriggerSetFrequency:
            if ( aError == KErrNone )
                {
                iState = EStateTunerOn;
                }
            CompleteAsyncRequest(aError);
            break;
        case ETriggerSetFrequencyRange:
            if ( aError == KErrNone )
                {
                iState = EStateTunerOn;
                TRsFrequencyRange range;
                TInt minFreq, maxFreq;
                if ( GetFrequencyRange(range, minFreq, maxFreq) == KErrNone )
                    {
                    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] TunerOnComplete() - [%d, %d, %d]"), range, minFreq, maxFreq);
                    iSettings->SetFrequencyRange( range, minFreq, maxFreq );
                    }
                }
            CompleteAsyncRequest(aError);
            break;
        case ETriggerPlay:
            if ( aError == KErrNone )
                {
                TRAPD(err, DoPlayL());
                if ( err == KErrNone )
                    {
                    iState = EStatePlaying;
                    iPreEmpted = EFalse;
                    iSettings->SetRadioOn();
                    SetTspTargetClient( ERsPlayerPlaying );
                    }
                CompleteAsyncRequest(err);
                }
            else
                {
                CompleteAsyncRequest(aError);
                }
            break;
        case ETriggerAntenna:
            if ( aError == KErrNone )
                {
                iState = EStateTunerOn;
                if ( iSettings->IsAntennaAttached() )
                    {
                    // AK - begin: to cause publishing (CPHU-73YTQW)
                    iSettings->SetAntennaStatus(EFalse, EFalse);
                    // - end
                    iSettings->SetAntennaStatus(ETrue);
                    }
                }
            // else
                // Unable to turn the tuner back on. It's possible that after TunerOn request
                // has been sent that antenna has been disconnected or the offline mode has
                // been enabled again. Just remain here...
            break;
        case ETriggerOffline:
            if ( aError == KErrNone )
                {
                iState = EStateTunerOn;
                iSettings->SetOfflineModeStatus(EFalse);
                }
            // else
                // Unable to turn the tuner back on. It's possible that after TunerOn request
                // has been sent that antenna has been disconnected or the offline mode has
                // been enabled again. Just remain here...
            break;
        case ETriggerTransmitter:
            if ( aError == KErrNone )
                {
                iState = EStateTunerOn;
                iSettings->SetTransmitterStatus(EFalse);
                }
            // else
                // Unable to turn the tuner back on. It's possible that after TunerOn request
                // has been sent that antenna has been disconnected or the offline mode has
                // been enabled again. Just remain here...
            break;
        default:
            break;
        }
    
    if ( iSettings->IsAntennaAttached() == EFalse )
        {
        // antenna removed during TunerOn sequence, request TunerOff
        // can not shutdown totally in TunerOffComplete thus set iState to EStateTunerOff
        iState = EStateTunerOff;
        iTunerControl->TunerOff();    
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::TunerOffComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::TunerOffComplete(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] TunerOffComplete(%d) State[%d]"), aError, iState);
    switch ( iState )
        {
        case EStateShutdown:
            // We are shutting down normally. Called by destructor.
            iState = EStateStarted;
            iSettings->Reset();
            iMaxSigStrength = 0;
            iEnableTunerInOffline = EFalse;
            delete iTunerControl;
            iTunerControl = NULL;
            ClearQueue();

            if ( iSessionCount == 0 )
                {
                // Make sure a new session hasn't been started while TunerOff was being processed
                iShutdownTimer->Start();
                }
            break;
        case EStateTunerOff:
            if( iSchedulerWait->IsStarted() )
                {
                // let the PlayError() continue after this callback
                iSchedulerWait->AsyncStop();
                }
            if ( iAsyncRequest && iAsyncRequest->iType == ERadioServSetFrequencyRange )
                {
                if ( aError == KErrNone )
                    {
                    RADIO_RDEBUG(_L("[RADIO-SVR] TunerOffComplete() - Tuner ON"));
                    RecreateFmTunerControl();
                    iTunerOnTrigger = ETriggerSetFrequencyRange;
                    TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iFreqRange );
                    TInt freq = KFMDefaultFreq;
                    if ( iFreqRange == ERsRangeFmJapan )
                        {
                        freq = KJapaneseFMDefaultFreq;
                        }
                    iTunerControl->TunerOn(freqRange, freq);
                    }
                else
                    {
                    CompleteAsyncRequest(aError);
                    }
                }
            else if( aError == KRadioServErrDuplicateRequest )
                {
                // tuner off and duplicate request going on, trace it out
                RADIO_RDEBUG(_L("[RADIO-SVR] TunerOffComplete() - EStateTunerOff - KRadioServErrDuplicateRequest"));
                }
            else
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] TunerOffComplete() - iState = EStateTunerOff;"));
                }
            break;
        default:
            // should never happen
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::SetFrequencyRangeComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SetFrequencyRangeComplete(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] SetFrequencyRangeComplete(%d) State[%d]"), aError, iState);
    if ( aError == KErrNone )
        {
        TRsFrequencyRange range;
        TInt minFreq, maxFreq;
        if ( GetFrequencyRange(range, minFreq, maxFreq) == KErrNone )
            {
            iSettings->SetFrequencyRange(range, minFreq, maxFreq);
            }
        else
            {
            iSettings->SetFrequencyRange(range, 0, 0);
            }
        }
    CompleteAsyncRequest(aError);
    }

// -----------------------------------------------------------------------------
// CRadioServer::SetFrequencyComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SetFrequencyComplete(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] SetFrequencyComplete(%d) State[%d]"), aError, iState);
    
    // Disable squelching, if on
    if ( iSquelch )
        {
        // Restore the last volume
        iDevSound->SetVolume(iSettings->Volume());
        iSquelch = EFalse;
        }
    
    if ( iAsyncRequest && iAsyncRequest->iType == ERadioServSetFrequency )
        {
        if ( aError == KErrNone )
            {
            iSettings->SetFrequency(iAsyncRequest->iMessage.Int1());
            }
        CompleteAsyncRequest(aError);
        }
    else
        {
        // Synchronization initiated from StationSeekComplete(). Ignore.
        RADIO_RDEBUG(_L("[RADIO-SVR] SetFrequencyComplete()-SYNC done!"));
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::StationSeekComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::StationSeekComplete(
    TRadioServerError aError,
    TInt aFrequency )
    {
    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] StationSeekComplete(%d, %d) State[%d]"), aError, aFrequency, iState);
    if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                            (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                            (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                            (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
        {
        // Disable squelching, if on
        if ( iSquelch )
            {
            // Restore the last volume
            iDevSound->SetVolume(iSettings->Volume());
            iSquelch = EFalse;
            }

        if ( aError == KErrNone )
            {
            iSettings->SetFrequency(aFrequency);
            TPckgBuf<TInt> p(aFrequency);
            iAsyncRequest->iMessage.Write(2, p);
            }
        CompleteAsyncRequest(aError);
        }
    else
        {
        // Request was cancelled. Make sure the frequency is synchronized.
        if ( iSettings->Frequency() != aFrequency )
            {
            RADIO_RDEBUG_INT(_L("[RADIO-SVR] StationSeekComplete()-SYNC to: Freq[%d]"), iSettings->Frequency());
            iTunerControl->SetFrequency(iSettings->Frequency());
            }
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::AudioModeComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::AudioModeComplete(
    TRadioServerError aError,
    TBool aStereo )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] AudioModeComplete(%d, %d)"), aError, aStereo);
    if ( iSyncRequest && iSyncRequest->iType == ERadioServGetStereoMode )
        {
        if ( aError == KErrNone )
            {
            TPckgBuf<TBool> p(aStereo);
            iSyncRequest->iMessage.Write(1, p);
            }
        CompleteSyncRequest(aError);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::SetAudioModeComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SetAudioModeComplete(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] AudioModeComplete(%d)"), aError);
    TInt err = aError;
    if ( iSyncRequest && iSyncRequest->iType == ERadioServForceMonoReception )
        {
        if ( err == KErrNone )
            {
            iSettings->SetForcedMonoStatus(iSyncRequest->iMessage.Int1());
            if ( iState == EStatePlaying )
                {
                iDevSound->Stop();
                TRAP(err, DoPlayL());
                if ( err != KErrNone )
                    {
                    iSettings->SetRadioOff(err);
                    SetTspTargetClient( ERsPlayerIdle );
                    iState = EStateTunerOff;
                    iTunerControl->TunerOff();
                    RADIO_RDEBUG(_L("[RADIO-SVR] SetAudioModeComplete() - Tuner OFF"));
                    }
                }
            }
        CompleteSyncRequest(err);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::SignalStrengthComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SignalStrengthComplete(
    TRadioServerError aError,
    TInt aStrength )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] SignalStrengthComplete(%d, %d)"), aError, aStrength);
    if ( iSyncRequest && iSyncRequest->iType == ERadioServGetSignalStrength )
        {
        if ( aError == KErrNone )
            {
            TPckgBuf<TInt> p(aStrength);
            iSyncRequest->iMessage.Write(1, p);
            }
        CompleteSyncRequest(aError);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::MaxSignalStrengthComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::MaxSignalStrengthComplete(
    TRadioServerError aError,
    TInt aMaxStrength )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] MaxSignalStrengthComplete(%d, %d)"), aError, aMaxStrength);
    if ( iSyncRequest && iSyncRequest->iType == ERadioServGetMaxSignalStrength )
        {
        if ( aError == KErrNone )
            {
            iMaxSigStrength = aMaxStrength;
            TPckgBuf<TInt> p(aMaxStrength);
            iSyncRequest->iMessage.Write(1, p);
            }
        CompleteSyncRequest(aError);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::SquelchComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SquelchComplete(
    TRadioServerError /*aError*/,
    TBool /*aEnabled*/ )
    {
    // Not used.
    // Squelch is handled internally in RadioServer
    }

// -----------------------------------------------------------------------------
// CRadioServer::SetSquelchComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SetSquelchComplete(
    TRadioServerError /*aError*/ )
    {
    // Not used.
    // Squelch is handled internally in RadioServer
    }

// -----------------------------------------------------------------------------
// CRadioServer::BufferFilled (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::BufferFilled(
    TDes8& /*aBuffer*/ )
    {
    iDevSound->PlayData();
    }

// -----------------------------------------------------------------------------
// CRadioServer::RadioEventAntennaStatusChange (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RadioEventAntennaStatusChange(
    TBool aAttached )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] RadioEventAntennaStatusChange(%d) State[%d]"), aAttached, iState);
    if ( iSettings->IsAntennaAttached() == aAttached )
        {
        // Do nothing since the state of the antenna hasn't changed.
        RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - no change"));
        return;
        }

    switch ( iState )
        {
        case EStateStarted:
            // Just update the antenna status.
            iSettings->SetAntennaStatus(aAttached);
            RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - just antenna update"));
            break;
        case EStateTunerOn:
        case EStatePlaying:
            if ( iState == EStatePlaying )
                {
                iDevSound->Stop();
                iSettings->SetRadioOff(KRadioServErrAntennaNotConnected);
                SetTspTargetClient( ERsPlayerIdle );
                }
            // If we are receiving this at this state, it must be antenna detached
            iSettings->SetAntennaStatus(EFalse);

            // If station seek request is currently pending, complete the message since the
            // adaptation will never complete this for us.
            if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                                    (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                                    (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                                    (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - Seek Outstanding"));
                if ( iAsyncRequest->iType == ERadioServStationSeek )
                    {
                    iTunerControl->CancelStationSeek();
                    }
                else
                    {
                    iRdsControl->CancelRdsStationSeek();
                    }
                // Disable squelching, if on
                if ( iSquelch )
                    {
                    // Restore the last volume
                    iDevSound->SetVolume(iSettings->Volume());
                    iSquelch = EFalse;
                    }
                CompleteAsyncRequest(KRadioServErrAntennaNotConnected);
                }

            iState = EStateTunerOff;
            iTunerControl->TunerOff();
            RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - Tuner OFF"));
            break;
        case EStateTunerOff:
            if ( aAttached )
                {
                if ( !iSettings->IsOfflineModeEnabled() && !iSettings->IsTransmitterActive() && !iPreEmpted )
                    {
                    // Don't notify the client about the antenna status change yet.
                    // Notify once the Tuner is turned ON.
                    RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - Tuner ON"));
                    // AK - begin: need to set this status because recreating tuner will trigger another
                    // RadioEventAntennaStatusChange. (CPHU-73YTQW)
                    iSettings->SetAntennaStatus(ETrue, EFalse);
                    RecreateFmTunerControl();
                    // - end
                    iTunerOnTrigger = ETriggerAntenna;
                    TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
                    iTunerControl->TunerOn(freqRange, iSettings->Frequency());
                    }
                else
                    {
                    // Can't turn the tuner ON yet. Just update the antenna status.
                    RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventAntennaStatusChange() - Wait"));
                    iSettings->SetAntennaStatus(ETrue);
                    }
                }
            else
                {
                // Just update the antenna status.
                iSettings->SetAntennaStatus(EFalse);
                }
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::RadioEventTunerControlChange (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RadioEventTunerControlChange(
    TRadioServerError aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] RadioEventTunerControlChange(%d) State[%d]"), aError, iState);
    switch ( iState )
        {
        case EStateStarted:
            // Just update the transmitter status.
            if ( aError == KErrNone )
                {
                iSettings->SetTransmitterStatus(EFalse);
                }
            else if ( aError == KRadioServErrFmTransmitterActive )
                {
                iSettings->SetTransmitterStatus(ETrue);
                }
            break;
        case EStateTunerOn:
        case EStatePlaying:
            if ( iState == EStatePlaying )
                {
                iDevSound->Stop();
                iSettings->SetRadioOff(aError);
                SetTspTargetClient( ERsPlayerIdle );
                }
            // Tuner was forced off due to hardware conflict
            if ( aError == KRadioServErrFmTransmitterActive )
                {
                iSettings->SetTransmitterStatus(ETrue);
                }
            iState = EStateTunerOff;
            iTunerControl->TunerOff();
            RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventTunerControlChange() - Tuner OFF"));
            break;
        case EStateTunerOff:
            if ( aError == KErrNone )
                {
                if ( iSettings->IsAntennaAttached() && !iSettings->IsOfflineModeEnabled() && !iPreEmpted )
                    {
                    // Don't notify the client about the transmitter status change yet.
                    // Notify once the Tuner is turned ON.
                    RADIO_RDEBUG(_L("[RADIO-SVR] RadioEventTunerControlChange() - Tuner ON"));
                    RecreateFmTunerControl();
                    iTunerOnTrigger = ETriggerTransmitter;
                    TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
                    iTunerControl->TunerOn(freqRange, iSettings->Frequency());
                    }
                else
                    {
                    // Can't turn the tuner ON yet. Just update the transmitter status.
                    iSettings->SetTransmitterStatus(EFalse);
                    }
                }
            else if ( aError == KRadioServErrFmTransmitterActive )
                {
                // Just update the transmitter status.
                iSettings->SetTransmitterStatus(ETrue);
                }
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::StationSeekByPTYComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::StationSeekByPTYComplete(
    TRadioServerError aError,
    TInt aFrequency )
    {
    StationSeekComplete(aError,aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioServer::StationSeekByTAComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::StationSeekByTAComplete(
    TRadioServerError aError,
    TInt aFrequency )
    {
    StationSeekComplete(aError,aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioServer::StationSeekByTPComplete (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::StationSeekByTPComplete(
    TRadioServerError aError,
    TInt aFrequency )
    {
    StationSeekComplete(aError,aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataPI (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataPI(
    TInt aPi )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] RdsDataPI(%d)"), aPi);
    iSettings->SetProgrammeIdentification(aPi);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataPTY (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataPTY(
    TRdsProgrammeType aPty )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] RdsDataPTY(%d)"), aPty);
    iSettings->SetProgrammeType(aPty);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataPS (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataPS(
    TRdsPSName& aPs )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] RdsDataPTY(%S)"), &aPs);
    iSettings->SetProgrammeService(aPs);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataRT (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataRT(
    TRdsRadioText& aRt,
    RArray<TRdsRTplusTag>& aRTplusTags )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] RdsDataRT(%S)"), &aRt);
    TRdsRTplusTag tag1;
    TRdsRTplusTag tag2;
    TRdsRadioText rtPlusObject1;
    TRdsRadioText rtPlusObject2;
    TRdsRadioText aRtCopy;
    
    aRtCopy.Copy( aRt );
    iSettings->SetRadioText( aRt );

    if( aRTplusTags.Count() > 0 )
        {
        // tags present, extract objects pointed by the tags
        RADIO_RDEBUG(_L("[RADIO-SVR] RT+ tags found, extract objects and pass them through"));
        tag1 = aRTplusTags[ 0 ];
        tag2 = aRTplusTags[ 1 ];
        
        if( tag1.iContentType == ERTplusItemDummy )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] Tag1 dummy, skipped!"));
            }
        else
            {
            // extract object from RadioText payload pointed by received tag information
            TInt length = tag1.iLength + 1;
            TInt start = tag1.iStart;
            TInt rtLength = aRtCopy.Length();

            if( start > rtLength )
                {
                start = rtLength;
                RADIO_RDEBUG(_L("[RADIO-SVR] Tag1 start marker adjusted"));
                }
            
            if( length + start > rtLength )
                {
                //Make sure we don't try to copy too much, in case of erroneous length indicators
                length = rtLength - start;
                RADIO_RDEBUG(_L("[RADIO-SVR] Tag1 length marker adjusted"));
                }
            rtPlusObject1 = aRtCopy.Mid( start, length );
            }
        
        if( tag2.iContentType == ERTplusItemDummy )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] Tag2 dummy, skipped!"));
            }
        else
            {
            // extract object from RadioText payload pointed by received tag information
            TInt length = tag2.iLength + 1;
            TInt start = tag2.iStart;
            TInt rtLength = aRtCopy.Length();

            if( start > rtLength )
                {
                start = rtLength;
                RADIO_RDEBUG(_L("[RADIO-SVR] Tag2 start marker adjusted"));
                }
            
            if( length + start > rtLength )
                {
                //Make sure we don't try to copy too much, in case of erroneous length indicators
                length = rtLength - start;
                RADIO_RDEBUG(_L("[RADIO-SVR] Tag2 length marker adjusted"));
                }
            rtPlusObject2 = aRtCopy.Mid( start, length );
            }
        RADIO_RDEBUG(_L("[RADIO-SVR] Passing RT+ objects"));
        iSettings->SetRadioTextPlusObjects( rtPlusObject1, tag1.iContentType,
                rtPlusObject2, tag2.iContentType );
        }
    aRTplusTags.Reset();
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataCT (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataCT(
    TDateTime& aCt )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] RdsDataCT()"));
    iSettings->SetClockTime(aCt);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsDataTA (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsDataTA(
    TBool aTaOn )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] RdsDataTA(%d)"), aTaOn);
    iSettings->SetTrafficAnnouncementStatus(aTaOn);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsSearchBeginAF (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsSearchBeginAF()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] RdsSearchBeginAF()"));
    iSettings->SetRdsBeginAF();
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsSearchEndAF (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsSearchEndAF(
    TRadioServerError aError,
    TInt aFrequency )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] RdsSearchEndAF(%d, %d)"), aError, aFrequency);
    iSettings->SetRdsEndAF(aError, aFrequency);
    }

// -----------------------------------------------------------------------------
// CRadioServer::RdsEventSignalChange (From MRadioServerFMTunerObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RdsEventSignalChange(
    TBool aSignal )
    {
    iSettings->SetRdsSignalChange(aSignal);
    }

// -----------------------------------------------------------------------------
// CRadioServer::InitializeComplete (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::InitializeComplete(
    TInt aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] InitializeComplete(%d) State[%d]"), aError, iState);
    if ( aError == KErrNone )
        {
        TMMFCapabilities devSoundCaps = iDevSound->Capabilities();
        RADIO_RDEBUG_INT3(_L("[RADIO-SVR] InitializeComplete - DevSound(%d, %d, %d)"), devSoundCaps.iRate, devSoundCaps.iEncoding, devSoundCaps.iChannels);

        TUint tunerRate = iTunerCaps.iSampleRate;
        TUint devSoundRate = devSoundCaps.iRate;
        // Pick the highest sample rate possible
        if ( (tunerRate & EFMRadioSampleRate96000Hz) && (devSoundRate & EMMFSampleRate96000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate96000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate88200Hz) && (devSoundRate & EMMFSampleRate88200Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate88200Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate64000Hz) && (devSoundRate & EMMFSampleRate64000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate64000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate48000Hz) && (devSoundRate & EMMFSampleRate48000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate48000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate44100Hz) && (devSoundRate & EMMFSampleRate44100Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate44100Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate32000Hz) && (devSoundRate & EMMFSampleRate32000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate32000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate24000Hz) && (devSoundRate & EMMFSampleRate24000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate24000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate22050Hz) && (devSoundRate & EMMFSampleRate22050Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate22050Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate16000Hz) && (devSoundRate & EMMFSampleRate16000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate16000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate12000Hz) && (devSoundRate & EMMFSampleRate12000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate12000Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate11025Hz) && (devSoundRate & EMMFSampleRate11025Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate11025Hz;
            }
        else if ( (tunerRate == EFMRadioSampleRate8000Hz) && (devSoundRate & EMMFSampleRate8000Hz) )
            {
            iDevSoundConfig.iRate = EMMFSampleRate8000Hz;
            }
        else
            {
            // Nothing matched
            RADIO_RDEBUG(_L("[RADIO-SVR] InitializeComplete() - incompatible sample rate"));
            CompleteAsyncRequest(KRadioServErrHardwareFaulty);
            return;
            }

        TUint tunerChan = iTunerCaps.iChannels;
        TUint devSoundChan = devSoundCaps.iChannels;
        if ( ((tunerChan & EFMRadioAudioMono) && (devSoundChan & EMMFMono)) &&
             ((tunerChan & EFMRadioAudioStereo) && (devSoundChan & EMMFStereo)))
            {
            iChannels = EChannelBoth;
            }
        else if ( (tunerChan & EFMRadioAudioMono) && (devSoundChan & EMMFMono) )
            {
            iChannels = EChannelMono;
            iSettings->SetForcedMonoStatus(ETrue, EFalse);
            }
        else if ( (tunerChan & EFMRadioAudioStereo) && (devSoundChan & EMMFStereo) )
            {
            iChannels = EChannelStereo;
            }
        else
            {
            // Nothing matched
            RADIO_RDEBUG(_L("[RADIO-SVR] InitializeComplete() - incompatible channels"));
            CompleteAsyncRequest(KRadioServErrHardwareFaulty);
            return;
            }
        iState = EStateTunerOn;
        CompleteAsyncRequest(KErrNone);
        }
    else
        {
        CompleteAsyncRequest(aError);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ToneFinished (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::ToneFinished(
    TInt /*aError*/ )
    {
    // Not used
    }

// -----------------------------------------------------------------------------
// CRadioServer::BufferToBeFilled (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::BufferToBeFilled(
    CMMFBuffer* aBuffer )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] BufferToBeFilled()"));
    iTunerControl->BufferToBeFilled(STATIC_CAST(CMMFDataBuffer*, aBuffer)->Data());
    }

// -----------------------------------------------------------------------------
// CRadioServer::PlayError (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::PlayError(
    TInt aError )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] PlayError(%d) State[%d]"), aError, iState);
    if ( iState == EStatePlaying )
        {
        // If station seek request is currently pending, complete the message since the
        // adaptation will never complete this for us.
        if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] PlayError() - Seek Outstanding"));
            if ( iAsyncRequest->iType == ERadioServStationSeek )
                {
                iTunerControl->CancelStationSeek();
                }
            else
                {
                iRdsControl->CancelRdsStationSeek();
                }
            // Disable squelching, if on
            if ( iSquelch )
                {
                // Restore the last volume
                iDevSound->SetVolume(iSettings->Volume());
                iSquelch = EFalse;
                }
            // do not yet complete async request with KRadioServErrTuning
            }

        // We are being pre-empted by another application with higher priority.
        // Turn the tuner off also to prevent interference + power saving.
        iState = EStateTunerOff;
        iPreEmpted = ETrue;
        iSettings->SetRadioOff(aError);
        RADIO_RDEBUG(_L("[RADIO-SVR] PlayError() - Call TunerOff and start waiting"));
        iTunerControl->TunerOff();		//The adaptation is expected to call TunerOffComplete() upon completion
        // wait for TunerOffComplete callback
        iSchedulerWait->Start();
        SetTspTargetClient( ERsPlayerIdle );
        
        if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                                (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
            {
            CompleteAsyncRequest(KRadioServErrTuning);
            }
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::BufferToBeEmptied (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::BufferToBeEmptied(
    CMMFBuffer* /*aBuffer*/ )
    {
    // Not used
    }

// -----------------------------------------------------------------------------
// CRadioServer::RecordError (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::RecordError(
    TInt /*aError*/ )
    {
    // Not used
    }

// -----------------------------------------------------------------------------
// CRadioServer::ConvertError (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::ConvertError(
    TInt /*aError*/ )
    {
    // Not used
    }

// -----------------------------------------------------------------------------
// CRadioServer::DeviceMessage (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::DeviceMessage(
    TUid /*aMessageType*/,
    const TDesC8& /*aMsg*/ )
    {
    // Not used
    }

// -----------------------------------------------------------------------------
// CRadioServer::SendEventToClient (From MDevSoundObserver)
// -----------------------------------------------------------------------------
//
void CRadioServer::SendEventToClient(
    const TMMFEvent& /*aEvent*/ )
    {
    // Attempt to acquire sound device is rejected by audio policy server.
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] BufferToBeFilled() State[%d]"), iState);
    iState = EStateTunerOn;
    }

// -----------------------------------------------------------------------------
// CRadioServer::HandleNotifyInt (From MCenRepNotifyHandlerCallback)
// Notification from Central Repository indicating change in device's offline mode.
// -----------------------------------------------------------------------------
//
void CRadioServer::HandleNotifyInt(
    TUint32 aId,
    TInt /*aNewValue*/ )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] HandleNotifyInt()"));
    // Start listening immediately before analyzing the notification to ensure that we
    // don't miss further updates.
    TRAPD(err, iCenRepNotifyHandler->StartListeningL());
    if ( err != KErrNone )
        {
        User::Panic(_L("RadioServer-CR"), err );
        }
    if ( aId != KCoreAppUIsNetworkConnectionAllowed )
        {
        return;
        }
    TInt value;
    iRepository->Get(KCoreAppUIsNetworkConnectionAllowed, value);

    TBool offlineEnabled = EFalse;
    if ( (TCoreAppUIsNetworkConnectionAllowed)value == ECoreAppUIsNetworkConnectionNotAllowed )
        {
        offlineEnabled = ETrue;
        }

    if ( iSettings->IsOfflineModeEnabled() == offlineEnabled )
        {
        // Do nothing since the offline status hasn't changed.
        return;
        }

    switch ( iState )
        {
        case EStateStarted:
            // Just update the offline mode status.
            iSettings->SetOfflineModeStatus(offlineEnabled);
            break;
        case EStateTunerOn:
        case EStatePlaying:
            iSettings->SetOfflineModeStatus(offlineEnabled);
            if ( !AllowRadioInOfflineMode() )
                {
                if ( iState == EStatePlaying )
                    {
                    iDevSound->Stop();
                    iSettings->SetRadioOff(KRadioServErrOfflineMode);
                    SetTspTargetClient( ERsPlayerIdle );
                    }

                // If station seek request is currently pending, complete the message since the
                // adaptation will never complete this for us.
                if ( iAsyncRequest && ( (iAsyncRequest->iType == ERadioServStationSeek) ||
                                        (iAsyncRequest->iType == ERadioServStationSeekByPTY) ||
                                        (iAsyncRequest->iType == ERadioServStationSeekByTA) ||
                                        (iAsyncRequest->iType == ERadioServStationSeekByTP) ) )
                    {
                    RADIO_RDEBUG(_L("[RADIO-SVR] HandleNotifyInt() - Seek Outstanding"));
                    if ( iAsyncRequest->iType == ERadioServStationSeek )
                        {
                        iTunerControl->CancelStationSeek();
                        }
                    else
                        {
                        iRdsControl->CancelRdsStationSeek();
                        }
                    // Disable squelching, if on
                    if ( iSquelch )
                        {
                        // Restore the last volume
                        iDevSound->SetVolume(iSettings->Volume());
                        iSquelch = EFalse;
                        }
                    CompleteAsyncRequest(KRadioServErrOfflineMode);
                    }

                iState = EStateTunerOff;
                iTunerControl->TunerOff();
                RADIO_RDEBUG(_L("[RADIO-SVR] HandleNotifyInt() - Tuner OFF"));
                }
            break;
        case EStateTunerOff:
            if ( offlineEnabled )
                {
                // Just update the offline mode status.
                iSettings->SetOfflineModeStatus(ETrue);
                }
            else
                {
                if ( iSettings->IsAntennaAttached() && !iSettings->IsTransmitterActive() && !iPreEmpted )
                    {
                    // Don't notify the client about the offline mode status change yet.
                    // Notify once the Tuner is turned ON.
                    RADIO_RDEBUG(_L("[RADIO-SVR] HandleNotifyInt() - Tuner ON"));
                    RecreateFmTunerControl();
                    iTunerOnTrigger = ETriggerOffline;
                    TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
                    iTunerControl->TunerOn(freqRange, iSettings->Frequency());
                    }
                else
                    {
                    // Can't turn the tuner ON yet. Just update the offline mode status.
                    iSettings->SetOfflineModeStatus(EFalse);
                    }
                }
            break;
        default :
            break;
        }
    }

/***********************************************************
 *     Internal Functions Begin                            *
 ***********************************************************/

// -----------------------------------------------------------------------------
// CRadioServer::ProcessAsyncRequest
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessAsyncRequest(
    TRadioMessageRequestData* aRequest )
    {
    // Set the current outstanding request
    iAsyncRequest = aRequest;
    switch( aRequest->iType )
        {
        case ERadioServRequestTunerControl:
            ProcessRequestTunerControl((TRsTuner)aRequest->iMessage.Int1());
            break;
        case ERadioServSetFrequencyRange:
            ProcessSetFrequencyRange((TRsFrequencyRange)aRequest->iMessage.Int1());
            break;
        case ERadioServSetFrequency:
            ProcessSetFrequency(aRequest->iMessage.Int1());
            break;
        case ERadioServStationSeek:
            ProcessStationSeek(aRequest->iMessage.Int1());
            break;
        case ERadioServPlay:
            ProcessPlay();
            break;
        case ERadioServStop:
            ProcessStop(aRequest->iMessage.Int1());
            break;
        case ERadioServStationSeekByPTY:
            ProcessStationSeekByPTY(aRequest->iMessage.Int1(), aRequest->iMessage.Int2());
            break;
        case ERadioServStationSeekByTA:
            ProcessStationSeekByTA(aRequest->iMessage.Int1());
            break;
        case ERadioServStationSeekByTP:
            ProcessStationSeekByTP(aRequest->iMessage.Int1());
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessRequestTunerControl
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessRequestTunerControl(
    TRsTuner aTuner )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessRequestTunerControl(%d) State[%d]"), aTuner, iState);
    if ( aTuner != ERsTunerFm )
        {
        // Only FM is supported at this point
        CompleteAsyncRequest(KErrNotSupported);
        return;
        }

    TInt err = KErrNone;
    if ( !iTunerControl )
        {
        if ( !iTunerControlObserver )
            {
            TRAP(err, iTunerControlObserver = CRadioServerFMTuner::NewL(*this));
            }
        if ( err == KErrNone )
            {
            TRAP(err, iTunerControl = CFMRadioTunerControl::NewL(*iTunerControlObserver));
            if ( err == KErrNone )
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRequestTunerControl() - objects created"));
                iTunerCaps = iTunerControl->Capabilities();
                // RadioServer does not support dual tuner. Update the tuner capability to
                // disable dual tuner support even if the adaptation supports it.
                iTunerCaps.iTunerFunctions = (iTunerCaps.iTunerFunctions & 0x03);

#ifdef RD_FM_RADIO_ENHANCEMENTS
                iRdsControl = iTunerControl->RdsControl(*iTunerControlObserver);
                if ( iRdsControl )
                    {
                    RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRequestTunerControl() - RDS control obtained"));
                    iRdsControl->GetCapabilities(iRdsCaps);
                    // RadioServer does not support RadioText+. Update the tuner capability to
                    // disable RadioText+ support even if the adaptation supports it.
                    iRdsCaps.iRdsFunctions = (iRdsCaps.iRdsFunctions & 0x1EF);
                    }
#endif
                }
            }
        }
    // AK - begin: (CPHU-73YTQW)
    else
        {
        RecreateFmTunerControl();
        }
    // - end

    if ( err == KErrNone )
        {
        if ( !AllowRadioInOfflineMode() )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRequestTunerControl(): Offline"));
            err = KRadioServErrOfflineMode;
            }
        else if ( iSettings->IsTransmitterActive() )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRequestTunerControl(): Transmitter active"));
            err = KRadioServErrFmTransmitterActive;
            }
        else if ( !iSettings->IsAntennaAttached() )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRequestTunerControl(): Antenna not attached"));
            err = KRadioServErrAntennaNotConnected;
            }
        }

    if ( err == KErrNone )
        {
        iTunerOnTrigger = ETriggerTunerControl;
        TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
        iTunerControl->TunerOn(freqRange, iSettings->Frequency());
        }
    else
        {
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessRequestTunerControl() - unabled to create tuner. err=[%d]"), err);
        CompleteAsyncRequest(err);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetFrequencyRange
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessSetFrequencyRange(
    TRsFrequencyRange aRange )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetFrequencyRange(%d) State[%d]"), aRange, iState);
    if ( aRange == ERsRangeFmEuroAmerica || aRange == ERsRangeFmJapan )
        {
        if ( (iSettings->FrequencyRange()) == aRange )
            {
            // Same range; no further action needed
            CompleteAsyncRequest(KErrNone);
            return;
            }

        if ( iState == EStatePlaying )
            {
            iState = EStateTunerOff;
            iFreqRange = aRange;
            iDevSound->Stop();
            iSettings->SetRadioOff(KErrNone);
            iTunerControl->TunerOff();
            // This request is completed upon TunerOffComplete()
            SetTspTargetClient( ERsPlayerIdle );
            }
        else if ( iState == EStateTunerOn )
            {
            iState = EStateTunerOff;
            iFreqRange = aRange;
            iTunerControl->TunerOff();
            // This request is completed upon TunerOffComplete()
            }
        else
            {
            CompleteAsyncRequest(KErrNotReady);
            }
        }
    else
        {
        CompleteAsyncRequest(KErrNotSupported);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetFrequency
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessSetFrequency(
    TInt aFrequency )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetFrequency(%d) State[%d]"), aFrequency, iState);
    switch ( iState )
        {
        case EStateTunerOn:
        case EStatePlaying:
            if( iSettings->Frequency() == aFrequency )
                {
                // Same frequency
                CompleteAsyncRequest(KErrNone);
                }
            else
                {
                if ( iSettings->IsSquelchEnabled() && !iSettings->IsMute() )
                    {
                    // Simulate squelching
                    iDevSound->SetVolume(0);
                    iSquelch = ETrue;
                    }
                iTunerControl->SetFrequency(aFrequency);
                }
            break;
        case EStateTunerOff:
            if ( iSettings->IsAntennaAttached() && AllowRadioInOfflineMode()
                    && !iSettings->IsTransmitterActive() && !iPreEmpted )
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessSetFrequency() - Sent TunerOn request"));
                RecreateFmTunerControl();
                iTunerOnTrigger = ETriggerSetFrequency;
                TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
                iTunerControl->TunerOn(freqRange, aFrequency);
                }
            else
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessSetFrequency - KErrNotReady"));
                CompleteAsyncRequest(KErrNotReady);
                }
            break;
        case EStateStarted:
            CompleteAsyncRequest(KErrNotReady);
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessStationSeek
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessStationSeek(
    TBool aUpwards )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessStationSeek(%d) State[%d]"), aUpwards, iState);
    switch ( iState )
        {
        case EStateTunerOn:
        case EStatePlaying:
            if ( iState == EStatePlaying )
                {
                if ( iSettings->IsSquelchEnabled() && !iSettings->IsMute() )
                    {
                    // Simulate squelching
                    iDevSound->SetVolume(0);
                    iSquelch = ETrue;
                    }
                }
            if ( aUpwards )
                {
                iTunerControl->StationSeek(EFMRadioSeekUp);
                }
            else
                {
                iTunerControl->StationSeek(EFMRadioSeekDown);
                }
            break;
        case EStateStarted:
        case EStateTunerOff:
            CompleteAsyncRequest(KErrNotReady);
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessPlay
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessPlay()
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessPlay() State[%d]"), iState);
    switch ( iState )
        {
        case EStateTunerOn:
            TRAPD(err, DoPlayL());
            if ( err == KErrNone )
                {
                iState = EStatePlaying;
                iPreEmpted = EFalse;
                iSettings->SetRadioOn();
                // Notify secondary clients that radio is now playing. This should trigger
                // secondary clients to create a session to observe for events.
                iSettings->SetRadioMonitorStatus(ETrue);
                SetTspTargetClient( ERsPlayerPlaying );
                }
            RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessPlay() DevSound err=[%d]"), err);
            CompleteAsyncRequest(err);
            break;
        case EStatePlaying:
            // Already playing
            CompleteAsyncRequest(KErrNone);
            break;
        case EStateTunerOff:
            if ( iSettings->IsAntennaAttached() && AllowRadioInOfflineMode()
                    && !iSettings->IsTransmitterActive() )
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessPlay() - Sent TunerOn request"));
                RecreateFmTunerControl();
                iTunerOnTrigger = ETriggerPlay;
                TFMRadioFrequencyRange freqRange = TunerControlFreqRange( iSettings->FrequencyRange() );
                iTunerControl->TunerOn(freqRange, iSettings->Frequency());
                }
            else
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessPlay - KErrNotReady"));
                CompleteAsyncRequest(KErrNotReady);
                }
            break;
        default:
            CompleteAsyncRequest(KErrNotReady);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessStop
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessStop(
    TBool aIfOnlyPrimaryClient )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessStop(%d) State[%d]"), aIfOnlyPrimaryClient, iState);
    if ( iState == EStatePlaying )
        {
        if ( aIfOnlyPrimaryClient && (iSessionCountPrimary > 1) )
            {
            // Client has requested stop only if the requesting client is the only primary client.
            // There are other clients, so don't stop the audio in this case.
            CompleteAsyncRequest(KErrNone);         //the request needs to be completed though 
            return;
            }
        iState = EStateTunerOn;
        // Stop ongoing playback. This is synchronous function.
        iDevSound->Stop();
        iSettings->SetRadioOff(KErrNone);
        SetTspTargetClient( ERsPlayerIdle );
        }
    CompleteAsyncRequest(KErrNone);
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessStationSeekByPTY
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessStationSeekByPTY(
    TRsRdsProgrammeType aPty,
    TBool aSeekUp )
    {
    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] ProcessStationSeekByPTY(%d, %d) State[%d]"), aPty, aSeekUp, iState);
    switch ( iState )
        {
        case EStateTunerOn:
        case EStatePlaying:
            if ( !iRdsControl )
                {
                CompleteAsyncRequest(KErrNotSupported);
                return;
                }

            if ( iSettings->IsSquelchEnabled() && !iSettings->IsMute() )
                {
                // Simulate squelching
                iDevSound->SetVolume(0);
                iSquelch = ETrue;
                }
            iRdsControl->StationSeekByPTY(aPty, aSeekUp);
            break;
        case EStateStarted:
        case EStateTunerOff:
            CompleteAsyncRequest(KErrNotReady);
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessStationSeekByTA
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessStationSeekByTA(
    TBool aSeekUp )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessStationSeekByTA(%d) State[%d]"), aSeekUp, iState);
    switch ( iState )
        {
        case EStateTunerOn:
        case EStatePlaying:
            if ( !iRdsControl )
                {
                CompleteAsyncRequest(KErrNotSupported);
                return;
                }

            if ( iSettings->IsSquelchEnabled() && !iSettings->IsMute() )
                {
                // Simulate squelching
                iDevSound->SetVolume(0);
                iSquelch = ETrue;
                }
            iRdsControl->StationSeekByTA(aSeekUp);
            break;
        case EStateStarted:
        case EStateTunerOff:
            CompleteAsyncRequest(KErrNotReady);
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessStationSeekByTP
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessStationSeekByTP(
    TBool aSeekUp )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessStationSeekByTP(%d) State[%d]"), aSeekUp, iState);
    switch ( iState )
        {
        case EStateTunerOn:
        case EStatePlaying:
            if ( !iRdsControl )
                {
                CompleteAsyncRequest(KErrNotSupported);
                return;
                }

            if ( iSettings->IsSquelchEnabled() && !iSettings->IsMute() )
                {
                // Simulate squelching
                iDevSound->SetVolume(0);
                iSquelch = ETrue;
                }
            iRdsControl->StationSeekByTP(aSeekUp);
            break;
        case EStateStarted:
        case EStateTunerOff:
            CompleteAsyncRequest(KErrNotReady);
            break;
        default :
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessRemovePrimaryClient
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessRemovePrimaryClientL(
    const RMessage2& aMessage )
    {
#ifdef RD_TSP_CLIENT_MAPPER

    RThread client;
    aMessage.ClientL( client, EOwnerProcess );
    RProcess process;
    client.Process( process );
    TInt index = iClientPids.Find( process.Id() );
    if ( index != KErrNotFound )
        {
        iClientPids.Remove( index );
        if ( iProcessId == process.Id() )
            {
            iMapper->RemoveTspTargetClient( iTspState, iProcessId );
            if ( iClientPids.Count() && iState == EStatePlaying ) // still primary clients remaining
                {
                iProcessId = iClientPids[0];
                RADIO_RDEBUG_INT( _L("RADIO-SVR] ProcessRemovePrimaryClient() pid[%d]"), (TInt)iProcessId );
                iMapper->SetTspTargetClient( CTspClientMapper::EPlayingClients, iProcessId );
                iTspState = CTspClientMapper::EPlayingClients;
                }
            }
        }
    process.Close();
    client.Close();
#endif // RD_TSP_CLIENT_MAPPER

    if ( --iSessionCountPrimary == 0 )
        {
        RADIO_RDEBUG(_L("[RADIO-SVR] ProcessRemovePrimaryClient() - last primary"));
        iSettings->SetRadioMonitorStatus(EFalse);
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetTunerCapabilities
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetTunerCapabilities(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetTunerCapabilities() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TRsTunerCapabilities caps;
    caps.iFrequencyRange = iTunerCaps.iTunerBands;
    caps.iCapabilities = iTunerCaps.iTunerFunctions;

    TPckgBuf<TRsTunerCapabilities> p(caps);
    aMessage.Write(1, p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessEnableTunerInOfflineMode
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessEnableTunerInOfflineMode(
    TBool aEnable )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessEnableTunerInOfflineMode(%d)"), aEnable);
    TInt err = KErrNone;
    if ( iState == EStateStarted )
        {
        iEnableTunerInOffline = aEnable;
        }
    else
        {
        if ( iTunerCaps.iTunerFunctions & 0x01 ) // ETunerAvailableInOfflineMode
            {
            // Tuner supports playing radio in offline mode
            iEnableTunerInOffline = aEnable;
            }
        else
            {
            err = KErrNotSupported;
            }
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetFrequencyRange
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetFrequencyRange(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetFrequencyRange() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TRsFrequencyRange range;
    TInt minFreq;
    TInt maxFreq;
    TInt err = iSettings->GetFrequencyRange(range, minFreq, maxFreq);
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetFrequencyRange() - [%d]"), err);
    RADIO_RDEBUG_INT3(_L("[RADIO-SVR] ProcessGetFrequencyRange() - [%d, %d, %d]"), range, minFreq, maxFreq);
    if ( err == KErrNone )
        {
        TPckgBuf<TRsFrequencyRange> pkg1(range);
        aMessage.Write(1, pkg1);
        TPckgBuf<TInt> pkg2(minFreq);
        aMessage.Write(2, pkg2);
        TPckgBuf<TInt> pkg3(maxFreq);
        aMessage.Write(3, pkg3);
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetFrequency
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetFrequency(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetFrequency() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TPckgBuf<TInt> p(iSettings->Frequency());
    aMessage.Write(1, p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetForceMonoReception
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetForceMonoReception(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetForceMonoReception() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TPckgBuf<TBool> p(iSettings->IsForceMonoEnabled());
    aMessage.Write(1,p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetSquelch
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetSquelch(
    TBool aEnabled )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetSquelch(%d) State[%d]"), aEnabled, iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    // Squelch is handled internally in RadioServer instead of requesting it to the tuner control
    iSettings->SetSquelch(aEnabled);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetSquelch
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetSquelch(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetSquelch() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TPckgBuf<TBool> p(iSettings->IsSquelchEnabled());
    aMessage.Write(1,p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSyncRequest
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessSyncRequest(
    TRadioMessageRequestData* aRequest )
    {
    // Set the current outstanding request
    iSyncRequest = aRequest;
    switch( aRequest->iType )
        {
        case ERadioServGetSignalStrength:
            ProcessGetSignalStrength();
            break;
        case ERadioServGetMaxSignalStrength:
            ProcessGetMaxSignalStrength();
            break;
        case ERadioServGetStereoMode:
            ProcessGetStereoMode();
            break;
        case ERadioServForceMonoReception:
            ProcessForceMonoReception(aRequest->iMessage.Int1());
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetSignalStrength
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessGetSignalStrength()
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetSignalStrength() State[%d]"), iState);
    if ( iState == EStateTunerOn || iState == EStatePlaying )
        {
        // The request is completed once SignalStrengthComplete() is received from tuner control
        iTunerControl->SignalStrength();
        }
    else
        {
        CompleteSyncRequest(KErrNotReady);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetMaxSignalStrength
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessGetMaxSignalStrength()
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetMaxSignalStrength() State[%d]"), iState);
    switch ( iState )
        {
        case EStateStarted:
            CompleteSyncRequest(KErrNotReady);
            break;
        case EStateTunerOn:
        case EStatePlaying:
            if ( iMaxSigStrength > 0 )
                {
                // This value is constant once we get it from the tuner. If the value is
                // already available, there is no need to get it again from the tuner.
                TPckgBuf<TInt> p(iMaxSigStrength);
                iSyncRequest->iMessage.Write(1, p);
                CompleteSyncRequest(KErrNone);
                }
            else
                {
                // The request is completed once MaxSignalStrengthComplete() is received from tuner control
                iTunerControl->MaxSignalStrength();
                }
            break;
        case EStateTunerOff:
            if ( iMaxSigStrength > 0 )
                {
                // This value is constant once we get it from the tuner. If the value is
                // already available, there is no need to get it again from the tuner.
                TPckgBuf<TInt> p(iMaxSigStrength);
                iSyncRequest->iMessage.Write(1, p);
                CompleteSyncRequest(KErrNone);
                }
            else
                {
                CompleteSyncRequest(KErrNotReady);
                }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetStereoMode
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessGetStereoMode()
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetStereoMode() State[%d]"), iState);
    if ( iState == EStateTunerOn || iState == EStatePlaying )
        {
        // The request is completed once AudioModeComplete() is received from tuner control
        iTunerControl->AudioMode();
        }
    else
        {
        CompleteSyncRequest(KErrNotReady);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessForceMonoReception
// -----------------------------------------------------------------------------
//
void CRadioServer::ProcessForceMonoReception(
    TBool aForcedMono )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessForceMonoReception(%d) State[%d]"), aForcedMono, iState);
    if ( aForcedMono == iSettings->IsForceMonoEnabled() )
        {
        RADIO_RDEBUG(_L("[RADIO-SVR] ProcessForceMonoReception() - no change"));
        CompleteSyncRequest(KErrNone);
        return;
        }

    if ( iState == EStateTunerOn || iState == EStatePlaying )
        {
        TFMRadioAudioMode mode;
        if ( aForcedMono )
            {
            if ( (iChannels == EChannelBoth) || (iChannels == EChannelMono) )
                {
                mode = EFMRadioAudioMono;
                }
            else
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] ProcessForceMonoReception() - unable to set to Mono"));
                CompleteSyncRequest(KErrNotSupported);
                return;
                }
            }
        else
            {
            if ( (iChannels == EChannelBoth) || (iChannels == EChannelStereo) )
                {
                mode = EFMRadioAudioStereo;
                }
            else
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] DoPlayL() - unable to set to stereo"));
                CompleteSyncRequest(KErrNotSupported);
                return;
                }
            }
        // The request is completed once SetAudioModeComplete() is received from tuner control.
        iTunerControl->SetAudioMode(mode);
        }
    else
        {
        CompleteSyncRequest(KErrNotReady);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetPlayerState
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetPlayerState(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetPlayerState() State[%d]"), iState);
    TRsPlayerState state = ERsPlayerIdle;
    if ( iState == EStatePlaying )
        {
        state = ERsPlayerPlaying;
        }
    TPckgBuf<TRsPlayerState> p(state);
    aMessage.Write(1,p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetMaxVolume
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetMaxVolume(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] ProcessGetMaxVolume()"));
    TPckgBuf<TInt> p(iMaxVolume);
    aMessage.Write(1, p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetVolume
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetVolume(
    TInt aVolume )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessSetVolume(%d)"), aVolume);
    if ( aVolume < 0 || aVolume > iMaxVolume )
        {
        return KErrArgument;
        }

    if ( aVolume != iSettings->Volume() )
        {
        if ( !iSettings->IsMute() )
            {
            iDevSound->SetVolume(aVolume);
            }
        iSettings->SetVolume(aVolume);
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetVolume
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetVolume(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] ProcessGetMaxVolume()"));
    TPckgBuf<TInt> p(iSettings->Volume());
    aMessage.Write(1, p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetVolumeRamp
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetVolumeRamp(
    const RMessage2& aMessage )
    {
    TTimeIntervalMicroSeconds rampDuration(0);
    TPckg<TTimeIntervalMicroSeconds> rampDurationPckg(rampDuration);
    TInt err = aMessage.Read(1, rampDurationPckg);
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetVolumeRamp(%d) err[%d]"), I64INT(rampDuration.Int64()), err);
    if ( err == KErrNone )
        {
        iDevSound->SetVolumeRamp(rampDuration);
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetMute
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetMute(
    TBool aMute )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessSetMute(%d)"), aMute);
    if ( aMute && !iSettings->IsMute() )
        {
        iDevSound->SetVolume(0);
        }
    else if ( !aMute && iSettings->IsMute() )
        {
        // Restore the last volume
        iDevSound->SetVolume(iSettings->Volume());
        }
    iSettings->SetMuteStatus(aMute);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetMuteStatus
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetMuteStatus(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] ProcessGetMuteStatus()"));
    TPckgBuf<TBool> p(iSettings->IsMute());
    aMessage.Write(1,p);
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetBalance
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetBalance(
    TInt aLeft,
    TInt aRight )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetBalance(%d, %d)"), aLeft, aRight);
    if ( (aLeft < KRadioServerBalanceMin) || (aLeft > KRadioServerBalanceMax)
         || (aRight < KRadioServerBalanceMin) || (aRight > KRadioServerBalanceMax) )
        {
        return KErrArgument;
        }

    TInt err = KErrNone;
    TInt left, right;
    iSettings->GetBalance(left, right);
    if ( (left != aLeft) || (right != aRight) )
        {
        TRAP(err, iDevSound->SetPlayBalanceL(aLeft, aRight));
        if( err == KErrNone )
            {
            iSettings->SetBalance(aLeft, aRight);
            }
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetBalance
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetBalance(
    const RMessage2& aMessage )
    {
    TInt left, right;
    iSettings->GetBalance(left, right);
    TPckgBuf<TInt> p1(left);
    TPckgBuf<TInt> p2(right);
    aMessage.Write(1,p1);
    aMessage.Write(2,p2);
    return KErrNone;
    }

/***********************************************************
 *   MRdsControl & MRdsControlObserver Begins
 ***********************************************************/

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetRdsCapabilities
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetRdsCapabilities(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetRdsCapabilities() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TRsRdsCapabilities caps;
        caps.iRdsFunctions = iRdsCaps.iRdsFunctions;
        TPckgBuf<TRsRdsCapabilities> p(caps);
        aMessage.Write(1, p);
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetRdsSignalStatus
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetRdsSignalStatus(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetRdsSignalStatus() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TBool signalStatus;
        err = iRdsControl->GetRdsSignalStatus(signalStatus);
        if ( err == KErrNone )
            {
            TPckgBuf<TBool> pkg(signalStatus);
            aMessage.Write(1, pkg);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessNotifyRdsDataChange
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessNotifyRdsDataChange(
        const RMessage2& aMessage )
        {
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessNotifyRdsDataChange() State[%d]"), iState);
        if ( iState == EStateStarted )
            {
            return KErrNotReady;
            }

        TInt err = KErrNone;
        if ( iRdsControl )
            {
            if ( iRdsNotifyClientIdArray.Count() == 0 )
                {
                TRdsData data;
                data.iRdsFunctions = TRdsData::KAllRdsData;
                data.iAdditionalFunctions1 = 0;     // To suppress compiler warning
                data.iAdditionalFunctions2 = 0;     // To suppress compiler warning
                err = iRdsControl->NotifyRdsDataChange(data);
                }
            if ( err == KErrNone )
                {
                RThread clientThread;
                TInt error = aMessage.Client(clientThread);
                if ( error == KErrNone )
                    {
                    TUint clientId = clientThread.Id().operator TUint();
                    AddRdsNotifyClientId(clientId);
                    }
                }
            }
        else
            {
            err = KErrNotSupported;
            }
        return err;
        }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessCancelNotifyRdsDataChange
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessCancelNotifyRdsDataChange(
        const RMessage2& aMessage )
        {
        RADIO_RDEBUG(_L("[RADIO-SVR] ProcessCancelNotifyRdsDataChange()"));
        if ( iRdsNotifyClientIdArray.Count() > 0 )
            {
            RThread clientThread;
            TInt error = aMessage.Client(clientThread);
            if ( error == KErrNone )
                {
                TUint clientId = clientThread.Id().operator TUint();
                RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessCancelNotifyRdsDataChange() clientID[%d]"), clientId);
                RemoveRdsNotifyClientId(clientId);
                }
            if ( iRdsNotifyClientIdArray.Count() == 0 )
                {
                if ( iRdsControl )
                    {
                    iRdsControl->CancelNotifyRdsDataChange();
                    }
                }
            }
        return KErrNone;
        }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessSetAutomaticSwitching
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessSetAutomaticSwitching(
    TBool aAuto )
    {
    RADIO_RDEBUG_INT2(_L("[RADIO-SVR] ProcessSetAutomaticSwitching(%d) State[%d]"), aAuto, iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        err = iRdsControl->SetAutomaticSwitching(aAuto);
        if ( err == KErrNone )
            {
            iSettings->SetAutoSwitchStatus(aAuto);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetAutomaticSwitching
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetAutomaticSwitching(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetAutomaticSwitching() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TPckgBuf<TBool> p(iSettings->AutoSwitch());
        aMessage.Write(1, p);
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessCancelAFSearch
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessCancelAFSearch()
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessCancelAFSearch() State[%d]"), iState);
    if ( iRdsControl )
        {
        iRdsControl->CancelAFSearch();
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetProgrammeIdentification
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetProgrammeIdentification(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetProgrammeIdentification() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TInt pi;
        err = iRdsControl->GetProgrammeIdentification(pi);
        if ( err == KErrNone )
            {
            TPckgBuf<TInt> p(pi);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetProgrammeType
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetProgrammeType(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetProgrammeType() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TRdsProgrammeType rdsPty;
        err = iRdsControl->GetProgrammeType(rdsPty);
        if ( err == KErrNone )
            {
            TRsRdsProgrammeType pty = rdsPty;
            TPckgBuf<TRsRdsProgrammeType> p(pty);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetProgrammeService
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetProgrammeService(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetProgrammeService() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TRdsPSName rdsPs;
        err = iRdsControl->GetProgrammeService(rdsPs);
        if ( err == KErrNone )
            {
            TRsRdsPSName ps;
            ps.Copy(rdsPs);
            TPckgBuf<TRsRdsPSName> p(ps);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetRadioText
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetRadioText(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetRadioText() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TRdsRadioText rdsRt;
        RArray<TRdsRTplusTag> rtTags;
        err = iRdsControl->GetRadioText(rdsRt, rtTags);
        rtTags.Close();
        if ( err == KErrNone )
            {
            TRsRdsRadioText rt;
            rt.Copy(rdsRt);
            TPckgBuf<TRsRdsRadioText> p(rt);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetClockTime
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetClockTime(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetClockTime() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TDateTime ct;
        err = iRdsControl->GetClockTime(ct);
        if ( err == KErrNone )
            {
            TPckgBuf<TDateTime> p(ct);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetTrafficAnnouncementStatus
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetTrafficAnnouncementStatus(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetTrafficAnnouncementStatus() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TBool ta;
        err = iRdsControl->GetTrafficAnnouncementStatus(ta);
        if ( err == KErrNone )
            {
            TPckgBuf<TBool> p(ta);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::ProcessGetTrafficProgrammeStatus
// -----------------------------------------------------------------------------
//
TInt CRadioServer::ProcessGetTrafficProgrammeStatus(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] ProcessGetTrafficProgrammeStatus() State[%d]"), iState);
    if ( iState == EStateStarted )
        {
        return KErrNotReady;
        }

    TInt err = KErrNone;
    if ( iRdsControl )
        {
        TBool tp;
        err = iRdsControl->GetTrafficProgrammeStatus(tp);
        if ( err == KErrNone )
            {
            TPckgBuf<TBool> p(tp);
            aMessage.Write(1, p);
            }
        }
    else
        {
        err = KErrNotSupported;
        }
    return err;
    }

// ----------------------------------------------------
// CRadioServer::CompleteAsyncRequest
// Complete current outstanding request
// ----------------------------------------------------
//
void CRadioServer::CompleteAsyncRequest(
    TInt aErrorCode )
    {
    if ( !iAsyncRequest )
        {
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] CompleteAsyncRequest(%d) - iAsyncRequest NULL"), aErrorCode);
        }
    else if ( !(iAsyncRequest->iMessage.IsNull()) )
        {
        RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CompleteAsyncRequest(%d, %d) - Client alive"), iAsyncRequest->iType, aErrorCode);
        iAsyncRequest->iMessage.Complete(aErrorCode);
        }
    else
        {
        RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CompleteAsyncRequest(%d, %d) - Client gone"), iAsyncRequest->iType, aErrorCode);
        }

    delete iAsyncRequest;
    iAsyncRequest = NULL;
    ProcessNextItemInQueue(iAsyncRequestQue);
    }

// ----------------------------------------------------
// CRadioServer::CompleteSyncRequest
// Complete current outstanding request
// ----------------------------------------------------
//
void CRadioServer::CompleteSyncRequest(
    TInt aErrorCode )
    {
    if ( !iSyncRequest )
        {
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] CompleteSyncRequest(%d) - iSyncRequest NULL"), aErrorCode);
        }
    else if ( !(iSyncRequest->iMessage.IsNull()) )
        {
        RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CompleteSyncRequest(%d, %d) - Client alive"), iSyncRequest->iType, aErrorCode);
        iSyncRequest->iMessage.Complete(aErrorCode);
        }
    else
        {
        RADIO_RDEBUG_INT2(_L("[RADIO-SVR] CompleteAsyncRequest(%d, %d) - Client gone"), iSyncRequest->iType, aErrorCode);
        }

    delete iSyncRequest;
    iSyncRequest = NULL;
    ProcessNextItemInQueue(iSyncRequestQue);
    }

// ----------------------------------------------------
// CRadioServer::RemoveFromQueue
// ----------------------------------------------------
//
void CRadioServer::RemoveFromQueue(
    TSglQue<TRadioMessageRequestData>& aQue,
    TRadioServerRequest aMessageType )
    {
    TRadioMessageRequestData* request;
    TSglQueIter<TRadioMessageRequestData> iter(aQue);
    iter.SetToFirst();
    request = iter++;
    while ( request )
        {
        if( request->iType == aMessageType )
            {
            aQue.Remove(*request);
            if( !(request->iMessage.IsNull()) )
                {
                RADIO_RDEBUG(_L("[RADIO-SVR] RemoveFromQueue()"));
                request->iMessage.Complete(KErrCancel);
                }
            delete request;
            break;
            }
        request = iter++;
        }
    }

// ----------------------------------------------------
// CRadioServer::ProcessNextItemInQueue
// ----------------------------------------------------
//
void CRadioServer::ProcessNextItemInQueue(
    TSglQue<TRadioMessageRequestData>& aQue )
    {
    if ( !aQue.IsEmpty() )
        {
        // there is at least one element in the linked list
        TRadioMessageRequestData* data;

        data = aQue.First();
        aQue.Remove(*data);
        ProcessAsyncRequest(data);
        }
    }

// ----------------------------------------------------
// CRadioServer::ClearQueue
// ----------------------------------------------------
//
void CRadioServer::ClearQueue()
    {
    while ( !iAsyncRequestQue.IsEmpty() )
        {
        iAsyncRequest = iAsyncRequestQue.First();
        iAsyncRequestQue.Remove(*iAsyncRequest);
        delete iAsyncRequest;
        iAsyncRequest = NULL;
        }

    while ( !iSyncRequestQue.IsEmpty() )
        {
        iSyncRequest = iSyncRequestQue.First();
        iSyncRequestQue.Remove(*iSyncRequest);
        delete iSyncRequest;
        iSyncRequest = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::AllowRadioInOfflineMode
// -----------------------------------------------------------------------------
//
TBool CRadioServer::AllowRadioInOfflineMode()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] AllowRadioInOfflineMode()"));
    TBool res = EFalse;
    if ( iSettings->IsOfflineModeEnabled() )
        {
        if ( iEnableTunerInOffline && (iTunerCaps.iTunerFunctions & 0x01) ) // ETunerAvailableInOfflineMode
            {
            // Only allowed when client requested tuner in offline AND adaptation supports it
            res = ETrue;
            }
        }
    else
        {
        // Phone is not in Offline mode anyways; OK
        res = ETrue;
        }
    return res;
    }

// -----------------------------------------------------------------------------
// CRadioServer::GetFrequencyRange
// -----------------------------------------------------------------------------
//
TInt CRadioServer::GetFrequencyRange(
    TRsFrequencyRange& aRange,
    TInt& aMinFreq,
    TInt& aMaxFreq ) const
    {
    TInt err = KErrNone;
#ifdef RD_FM_RADIO_ENHANCEMENTS
    TFMRadioFrequencyRange range;
    err = iTunerControl->GetFrequencyRange(range, aMinFreq, aMaxFreq);

    if ( err == KErrNone )
        {
        if ( range == EFMRadioFrequencyEuroAmerica )
            {
            aRange = ERsRangeFmEuroAmerica;
            }
        else
            {
            aRange = ERsRangeFmJapan;
            }
        }
#else
    aRange = iSettings->FrequencyRange();
    if ( aRange == ERsRangeFmEuroAmerica )
        {
        aMinFreq = KEuroAmericaMin;
        aMaxFreq = KEuroAmericaMax;
        }
    else
        {
        aMinFreq = KJapanMin;
        aMaxFreq = KJapanMax;
        }
#endif //#ifdef RD_FM_RADIO_ENHANCEMENTS
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] GetFrequencyRange() err[%d]"), err);
    return err;
    }

// -----------------------------------------------------------------------------
// CRadioServer::TunerControlFreqRange
// -----------------------------------------------------------------------------
//
TFMRadioFrequencyRange CRadioServer::TunerControlFreqRange(
    TRsFrequencyRange aRange )
    {
    if ( aRange == ERsRangeFmEuroAmerica )
        {
        return EFMRadioFrequencyEuroAmerica;
        }
    else
        {
        // Safe to assume; check was already done at ProcessSetFrequencyRange
        return EFMRadioFrequencyJapan;
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::DoPlayL
// -----------------------------------------------------------------------------
//
void CRadioServer::DoPlayL()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] DoPlayL()"));
    if ( iSettings->IsForceMonoEnabled() )
        {
        iDevSoundConfig.iChannels = EMMFMono;
        }
    else
        {
        iDevSoundConfig.iChannels = EMMFStereo;
        }

    iDevSound->SetConfigL(iDevSoundConfig);
    RADIO_RDEBUG(_L("[RADIO-SVR] DoPlayL() - DevSound configured"));
    iDevSound->PlayInitL();
    RADIO_RDEBUG(_L("[RADIO-SVR] DoPlayL() - DevSound playInit OK"));
    }

// -----------------------------------------------------------------------------
// CRadioServer::RecreateFmTunerControl
// -----------------------------------------------------------------------------
//
void CRadioServer::RecreateFmTunerControl()
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] RecreateFmTunerControl()"));
    // AK - temporary until adaptation fix is available (CPHU-73YTQW)
    delete iTunerControl;
    iTunerControl = NULL;
    iRdsControl = NULL;
    TRAPD(err, iTunerControl = CFMRadioTunerControl::NewL(*iTunerControlObserver));
    if ( err != KErrNone )
        {
        RADIO_RDEBUG(_L("[RADIO-SVR] RecreateFmTunerControl() - failed to recreate!"));
        }
#ifdef RD_FM_RADIO_ENHANCEMENTS
    if ( iTunerControl )
        {
        iRdsControl = iTunerControl->RdsControl(*iTunerControlObserver);
        if ( !iRdsControl )
            {
            RADIO_RDEBUG(_L("[RADIO-SVR] RecreateFmTunerControl() - failed to get RDS Control!"));
            }
        }
#endif
    }

// -----------------------------------------------------------------------------
// CRadioServer::AddRdsNotifyClientId
// -----------------------------------------------------------------------------
//
void CRadioServer::AddRdsNotifyClientId( TUint aClientId )
    {
    TInt index = iRdsNotifyClientIdArray.Find(aClientId);
    if ( index == KErrNotFound )
        {
        // Client is not in the array, so add client.
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] AddRdsNotifyClientId() Add client[%d]"), aClientId);
        iRdsNotifyClientIdArray.Append(aClientId);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::RemoveRdsNotifyClientId
// -----------------------------------------------------------------------------
//
void CRadioServer::RemoveRdsNotifyClientId( TUint aClientId )
    {
    TInt index = iRdsNotifyClientIdArray.Find(aClientId);
    if ( index != KErrNotFound )
        {
        // Client is in the array, so remove client.
        RADIO_RDEBUG_INT(_L("[RADIO-SVR] RemoveRdsNotifyClientId() Remove client[%d]"), aClientId);
        iRdsNotifyClientIdArray.Remove(index);
        }
    }

// -----------------------------------------------------------------------------
// CRadioServer::SetTspTargetClient
// -----------------------------------------------------------------------------
//
void CRadioServer::SetTspTargetClient(
    TRsPlayerState aPlayerTargetState )
    {
    RADIO_RDEBUG_INT(_L("[RADIO-SVR] SetTspTargetClient(%d)"), aPlayerTargetState);
#ifdef RD_TSP_CLIENT_MAPPER
    if ( aPlayerTargetState == ERsPlayerPlaying )
        {
        iMapper->SetTspTargetClient( CTspClientMapper::EPlayingClients, iProcessId );
        iTspState = CTspClientMapper::EPlayingClients;
        }
    else if ( aPlayerTargetState == ERsPlayerIdle )
        {
        iMapper->SetTspTargetClientToOtherType( CTspClientMapper::EStoppedClients, iProcessId );
        iTspState = CTspClientMapper::EStoppedClients;
        }
#endif // RD_TSP_CLIENT_MAPPER
    }

// -----------------------------------------------------------------------------
// CRadioServer::RegisterClientPidL
// -----------------------------------------------------------------------------
//
void CRadioServer::RegisterClientPidL(
    const RMessage2& aMessage )
    {
    RADIO_RDEBUG(_L("[RADIO-SVR] RegisterClientPid()"));
#ifdef RD_TSP_CLIENT_MAPPER

    RThread client;
    aMessage.ClientL( client, EOwnerProcess );
    RProcess process;
    client.Process( process );
    iClientPids.Append(process.Id());
    if ( iClientPids.Count() == 1 )
        {
        iProcessId = process.Id();
        }
    process.Close();
    client.Close();
#endif // RD_TSP_CLIENT_MAPPER
    }
//  End of File
