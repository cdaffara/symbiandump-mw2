/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <string.h>
#include <ctype.h>
#include <uri8.h>
#include <uri16.h>
#include "cmmfbackendengine.h"
#include "markerpositiontimer.h"
#include "positionupdatetimer.h"
#include "prefetchlevelupdatetimer.h"
#include "profileutilmacro.h"
#include <mmf/common/mmfvideoenums.h>

extern "C"
    {
#include "xaadaptationmmf.h"
    }

#define RET_IF_ERR(res, val) if (res != KErrNone) return val

const XAuint32 XA_PLAYSTATE_PLAYERUNINITIALIZED = 0;

CMMFBackendEngine* CMMFBackendEngine::NewL()
    {
    CMMFBackendEngine* self = new (ELeave) CMMFBackendEngine();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CMMFBackendEngine::~CMMFBackendEngine()
    {
    Close();
    Destroy();
    }

CMMFBackendEngine::CMMFBackendEngine() :
    iPositionUpdatePeriod(1000), /* default is 1000 millisec */
	bPrefetchCallbackRegistered(EFalse),	
	bStatusChangeMask(EFalse),
	iPrefetchLevelUpdatePeriod(1000),
	iPrefetchStatus(XA_PREFETCHSTATUS_SUFFICIENTDATA), //assume sufficient data
	iNumStreams(0),
	iAudioOnly(false),
	iStreamInfoEventSubscribed(EFalse),
    iUriPtr(NULL, 0)
    {
    iRecordState = ERecorderNotReady;
    iPositionUpdateTimer = NULL;
    iMediaPlayerState = XA_PLAYSTATE_PLAYERUNINITIALIZED;
    iMMFPlayerState = EPlayerClosed;
    }

void CMMFBackendEngine::ConstructL()
    {
    iAPIBeingUsed = ENoUtility;
    iActiveSchedulerWait = new (ELeave) CActiveSchedulerWait;
    }

void CMMFBackendEngine::InitAudioRecorderUtilityL()
    {
    if (!iAudioRecorder)
        {
        iBaseAudioRecorder = CMdaAudioRecorderUtility::NewL(*this);
        iAudioRecorder = (CMdaAudioRecorderUtility*) iBaseAudioRecorder;
        }
    }

void CMMFBackendEngine::InitAudioPlayerUtilityL()
    {
    if (!iAudioPlayer)
        {
        iBaseAudioPlayer = CMdaAudioPlayerUtility::NewL(*this);
        iAudioPlayer = (CMdaAudioPlayerUtility*) iBaseAudioPlayer;
        }
    InitPlayerTimersL();
    }

void CMMFBackendEngine::InitVideoPlayerUtilityL()
    {
    if (!iVideoPlayer)
        {
        iBaseVideoPlayer = CVideoPlayerUtility2::NewL(*this,
                EMdaPriorityNormal, EMdaPriorityPreferenceTimeAndQuality);
        iVideoPlayer = (CVideoPlayerUtility2*) iBaseVideoPlayer;
        iVideoPlayer->RegisterForVideoLoadingNotification(*this);
        }
    InitPlayerTimersL();
    }

void CMMFBackendEngine::InitPlayerTimersL()
    {
    if (!iMarkerPositionTimer)
        {
        iMarkerPositionTimer = CMarkerPositionTimer::NewL(iAudioPlayer,
                iVideoPlayer);
        iMarkerPositionTimer->SetContext(iAdaptContext);
        }
    if (!iPlayItfPositionUpdateTimer)
        {
        iPlayItfPositionUpdateTimer = CPositionUpdateTimer::NewL(
                iAudioPlayer, iVideoPlayer);
        iPlayItfPositionUpdateTimer->SetContext(iAdaptContext);
        }
	if(!iPrefetchLevelUpdateTimer)
	{
		iPrefetchLevelUpdateTimer = CPrefetchLevelUpdateTimer::NewL(iAudioPlayer, iVideoPlayer);
		iPrefetchLevelUpdateTimer->SetPrefetchLevelUpdatePeriod(iPrefetchLevelUpdatePeriod); //set default update period at beginning
		iPrefetchLevelUpdateTimer->SetContext(iAdaptContext);
	}
    iMarkerPositionTimer->Stop();
    iPlayItfPositionUpdateTimer->Stop();
	iPrefetchLevelUpdateTimer->Stop();
    }

TInt CMMFBackendEngine::SetFileName(char* uri, XAuint32 format,
        TFuncInUse func)
    {
    TInt err(KErrNone);
    _LIT8(KFileSlash,"file:///");
    TInt fileslashlen = KFileSlash().Length();
    if (func == ERecord)
        {
        if (iRecordState == ERecorderNotReady)
            {
            iFileFormat = format;
            iAPIBeingUsed = DetermineAPIToUse(uri, ERecord);
            err = XA_RESULT_INTERNAL_ERROR;
            if (iAPIBeingUsed == EAudioRecorderUtility)
                {
                /* Initialize URI as needed by MMF utilities */
                err = InitializeURIForMMFUtil(uri);
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);
                /* Initalize Recorder related objects */
                TRAP(err, InitAudioRecorderUtilityL());
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);

                TRAP(err, iAudioRecorder->OpenFileL(iUriPtr));
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);
                /* Wait until we receive moscostatechanged callback */
                if (iActiveSchedulerWait && !iActiveSchedulerWait->IsStarted())
                    {
                    iActiveSchedulerWait->Start();
                    }
                RET_IF_ERR(iErrorCode, XA_RESULT_INTERNAL_ERROR);
                }
            }
        }
    else
        {
        /* The second one is needed for dynamic source interface */
        if ((iMediaPlayerState == XA_PLAYSTATE_PLAYERUNINITIALIZED)
                || (iMediaPlayerState == XA_PLAYSTATE_STOPPED))
            {
            iFileFormat = format;
            iAPIBeingUsed = DetermineAPIToUse(uri, EPlay);
            err = XA_RESULT_INTERNAL_ERROR;
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                /* Initialize URI as needed by MMF utilities */
                err = InitializeURIForMMFUtil(uri);
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);
                /* Initalize Player related objects */
                TRAP(err, InitAudioPlayerUtilityL());
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);

                TAG_TIME_PROFILING_BEGIN;
                TRAP(err, iAudioPlayer->OpenFileL(iUriPtr));
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR); 
                TAG_TIME_PROFILING_END; 
                PRINT_TO_CONSOLE_TIME_DIFF;

                /* Wait until we receive mapc init complete */
                PRINT_TO_CONSOLE_HOME_TIME;
                if (iActiveSchedulerWait && !iActiveSchedulerWait->IsStarted())
                    {
                    iActiveSchedulerWait->Start();
                    }
                RET_IF_ERR(iErrorCode, XA_RESULT_INTERNAL_ERROR);
                }
            else if (iAPIBeingUsed == EVideoPlayerUtility)
                {
                /* Initialize URI as needed by MMF utilities */
                err = InitializeURIForMMFUtil(uri);
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);
                /* Initalize Player related objects */
                TRAP(err, InitVideoPlayerUtilityL());
                RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR);

                if (iUriType == ELocal)
                    {
                    /* Open file */
                    TAG_TIME_PROFILING_BEGIN;
                    TRAP(err, iVideoPlayer->OpenFileL(iUriPtr));
                    RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR); 
                    TAG_TIME_PROFILING_END; 
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                else
                    {
                    /* Open URL */
                    TAG_TIME_PROFILING_BEGIN;
                    TRAP(err, iVideoPlayer->OpenUrlL(iUriPtr));
                    RET_IF_ERR(err, XA_RESULT_INTERNAL_ERROR); 
                    TAG_TIME_PROFILING_END; 
                    PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                /* Wait until we receive  MvpuoOpenComplete */
                PRINT_TO_CONSOLE_HOME_TIME;
                if (iActiveSchedulerWait && !iActiveSchedulerWait->IsStarted())
                    {
                    iActiveSchedulerWait->Start();
                    }
                RET_IF_ERR(iErrorCode, XA_RESULT_INTERNAL_ERROR);

                /* Prepare utility */
                TAG_TIME_PROFILING_BEGIN;
                iVideoPlayer->Prepare();
                TAG_TIME_PROFILING_END; 
                PRINT_TO_CONSOLE_TIME_DIFF;

                /* Wait until we receive  MvpuoPrepareComplete */
                PRINT_TO_CONSOLE_HOME_TIME_NO_VAR_DEF;
                if (iActiveSchedulerWait && !iActiveSchedulerWait->IsStarted())
                    {
                    iActiveSchedulerWait->Start();
                    }
                RET_IF_ERR(iErrorCode, XA_RESULT_INTERNAL_ERROR);
                }
            }
        }
    if (err != KErrNone)
        {
        err = XA_RESULT_INTERNAL_ERROR;
        }
    return err;
    }

TInt CMMFBackendEngine::DetermineAPIToUse(char* uri, TFuncInUse aFunc)
    {
    char* dotPtr = NULL;
    char ext[MAX_EXTENSION_SIZE] =
        {
        0
        };
    int extLen;

    int colpos;
    char urischeme[MAX_EXTENSION_SIZE] =
        {
        0
        };
    int urischemeLen;

    dotPtr = strrchr(uri, (int) '.');
    if (!dotPtr)
        {
        return KErrNotFound;
        }

    strncpy(ext, dotPtr, strlen(dotPtr));
    /*Null terminate the string*/
    ext[strlen(dotPtr)] = '\0';
    extLen = sizeof(ext);
    for (unsigned int i = 0; i < extLen; i++)
        {
        ext[i] = tolower(ext[i]);
        }

    colpos = strcspn(uri, ":");

    strncpy(urischeme, uri, colpos + 1);
    /*Null terminate the string*/
    urischeme[colpos + 1] = '\0';
    urischemeLen = sizeof(urischeme);
    for (unsigned int i = 0; i < urischemeLen; i++)
        {
        urischeme[i] = tolower(urischeme[i]);
        }

    if (aFunc == ERecord)
        {
        return EAudioRecorderUtility;
        }
    else
        {
        if (!strcmp(urischeme, "file:"))
            {
            if (!strcmp(ext, ".mp3") || !strcmp(ext, ".amr")
                    || !strcmp(ext, ".aac") || !strcmp(ext, ".mid")
                    || !strcmp(ext, ".wav") || !strcmp(ext, ".awb"))
                {
                return EAudioPlayerUtility;
                }
            else
                {
                return EVideoPlayerUtility;
                }
            }
        else
            {
            return EVideoPlayerUtility;
            }
        }
    }

//From MVidePlayerUtilityObserver
void CMMFBackendEngine::MvpuoOpenComplete(TInt aError)
    {
    PRINT_TO_CONSOLE_HOME_TIME;

    iErrorCode = aError;
    if (iErrorCode == KErrNone)
        {
        iMMFPlayerState = EPlayerOpened;
        }
    if (iActiveSchedulerWait && iActiveSchedulerWait->IsStarted())
        {
        iActiveSchedulerWait->AsyncStop();
        }
    }

void CMMFBackendEngine::MvpuoPrepareComplete(TInt aError)
    {
    PRINT_TO_CONSOLE_HOME_TIME;

    TAutoScaleType autoScale = EAutoScaleBestFit;
    iErrorCode = aError;
    iMediaPlayerState = XA_PLAYSTATE_PLAYERUNINITIALIZED;
    iMediaDuration = 0;
    iMarkerPositionTimer->ResetPlayer();
    iPlayItfPositionUpdateTimer->ResetPlayer();
    iPrefetchLevelUpdateTimer->ResetPlayer();
    if (iErrorCode == KErrNone)
        {
        iMMFPlayerState = EPlayerPrepared;
        TAG_TIME_PROFILING_BEGIN;
        TRAP(iErrorCode, iMediaDuration = iVideoPlayer->DurationL()); 
        TAG_TIME_PROFILING_END; 
        PRINT_TO_CONSOLE_TIME_DIFF;
        if (iErrorCode == KErrNone)
            {
            iPlaybackHead = 0;
            iMediaPlayerState = XA_PLAYSTATE_STOPPED;
            iMarkerPositionTimer->UseVideoPlayer();
            iPlayItfPositionUpdateTimer->UseVideoPlayer();
			iPrefetchLevelUpdateTimer->UseVideoPlayer();
            if (m_pWs && m_pScr && m_pWindow)
                {
                TRect videoExtent = TRect(m_pWindow->Size());
                TRect clipRect = TRect(m_pWindow->Size());
                TAG_TIME_PROFILING_BEGIN;
                TRAP_IGNORE(iVideoPlayer->AddDisplayWindowL(*m_pWs, *m_pScr, *m_pWindow, videoExtent, clipRect));
                TRAP_IGNORE(iVideoPlayer->SetAutoScaleL(*m_pWindow, autoScale)); 
                TAG_TIME_PROFILING_END; 
                PRINT_TO_CONSOLE_TIME_DIFF;
                }
			SetStreamInfo();
            }
        }
    if (iActiveSchedulerWait && iActiveSchedulerWait->IsStarted())
        {
        iActiveSchedulerWait->AsyncStop();
        }
    }

void CMMFBackendEngine::MvpuoFrameReady(CFbsBitmap& /*aFrame*/, TInt /*aError*/)
    {

    }
void CMMFBackendEngine::MvpuoPlayComplete(TInt aError)
    {
    iErrorCode = aError;
    if (iErrorCode == KErrNone)
        {
        iPlaybackHead = iMediaDuration;
        /* Per OpenMAX AL Spec, when playback is completed, media player stays in
         * paused state with playhead at the end of the media clip */
        iMediaPlayerState = XA_PLAYSTATE_PAUSED;
        DoPostEvent(XA_PLAYEVENT_HEADATEND);
        }
    else
        {
        iPlaybackHead = 0;
        iMediaPlayerState = XA_PLAYSTATE_STOPPED;
        XAAdaptEvent event =
            {
            XA_PLAYITFEVENTS, XA_OBJECT_EVENT_RESOURCES_LOST, 0, NULL
            };
        XAAdaptationBase_SendAdaptEvents(
                (XAAdaptationBaseCtx*) iAdaptContext, &event);
        }
    iMMFPlayerState = EPlayerPrepared;
    iMarkerPositionTimer->Stop();
    iPlayItfPositionUpdateTimer->Stop();
    }

void CMMFBackendEngine::MvpuoEvent(class TMMFEvent const & event)
    {
    //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent (0x%x %d)"), event.iEventType, event.iErrorCode);

    if (event.iEventType == KMMFEventCategoryVideoPlayerGeneralError
            && event.iErrorCode == KErrHardwareNotAvailable)
        {
        //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent: Hardware Not Available"));
        }

    else if (event.iEventType == KMMFEventCategoryVideoPlayerGeneralError
            && event.iErrorCode == KErrMMPartialPlayback)
        {
        //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent: Partial playback"));
        }
    if (event.iEventType == KMMFEventCategoryVideoPlayerGeneralError
            && event.iErrorCode == -12014)
        {
        //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent: Audio Device taken"));
        PausePlayback();
        XAAdaptEvent event1 =
            {
            XA_PLAYITFEVENTS, XA_OBJECT_EVENT_RESOURCES_LOST, 0, NULL
            };
        XAAdaptationBase_SendAdaptEvents(
                (XAAdaptationBaseCtx*) iAdaptContext, &event1);
        }
    else if (event.iEventType == KMMFRefreshMetaData)
        {
        //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent: EventType == KMMFRefreshMetaData"));
        }
    else
        {
        //RDebug::Print(_L("CMMFBackendEngine::MvpuoEvent: Unhandled event"));
        }

    }

// From MRebufferCallback
void CMMFBackendEngine::MvloLoadingStarted()
    {
    	iPrefetchStatus = XA_PREFETCHSTATUS_UNDERFLOW;
		
    	if(bStatusChangeMask)
    	{
			XAAdaptEvent event =
			{
				XA_PREFETCHITFEVENTS, XA_PREFETCHEVENT_STATUSCHANGE, 0, NULL
			};
			XAAdaptationBase_SendAdaptEvents(
				(XAAdaptationBaseCtx*) iAdaptContext, &event);
    	}

		iPrefetchLevelUpdateTimer->Start();
    }

void CMMFBackendEngine::MvloLoadingComplete()
    {
		iPrefetchStatus = XA_PREFETCHSTATUS_SUFFICIENTDATA;

		if(bStatusChangeMask)
    	{
			XAAdaptEvent event =
				{
					XA_PREFETCHITFEVENTS, XA_PREFETCHEVENT_STATUSCHANGE, 0, NULL
				};
			XAAdaptationBase_SendAdaptEvents(
				(XAAdaptationBaseCtx*) iAdaptContext, &event);
    	}

		iPrefetchLevelUpdateTimer->Stop();
    }

//MMdaAudioPlayerCallback
void CMMFBackendEngine::MapcInitComplete(TInt aError,
        const TTimeIntervalMicroSeconds& aDuration)
    {
    PRINT_TO_CONSOLE_HOME_TIME;

    iErrorCode = aError;
    iMediaPlayerState = XA_PLAYSTATE_PLAYERUNINITIALIZED;
    iMMFPlayerState = EPlayerClosed;
    iMediaDuration = 0;
    iMarkerPositionTimer->ResetPlayer();
    iPlayItfPositionUpdateTimer->ResetPlayer();
    iPrefetchLevelUpdateTimer->ResetPlayer();
    if (iErrorCode == KErrNone)
        {
        iMediaDuration = aDuration;
        iPlaybackHead = 0;
        iMediaPlayerState = XA_PLAYSTATE_STOPPED;
        iMMFPlayerState = EPlayerOpened;
        iMarkerPositionTimer->UseAudioPlayer();
        iPlayItfPositionUpdateTimer->UseAudioPlayer();
		iPrefetchLevelUpdateTimer->UseAudioPlayer();
		//set stream parameters
		SetStreamInfo();
        }
    if (iActiveSchedulerWait && iActiveSchedulerWait->IsStarted())
        {
        iActiveSchedulerWait->AsyncStop();
        }
    }

void CMMFBackendEngine::MapcPlayComplete(TInt aError)
    {
    iErrorCode = aError;
    if (iErrorCode == KErrNone)
        {
        /* Now we should have better value. Start using that */
        iAudioPlayer->Duration(iMediaDuration);
        iPlaybackHead = iMediaDuration;
        /* Per OpenMAX AL Spec, when playback is completed, media player stays in
         * paused state with playhead at the end of the media clip */
        iMediaPlayerState = XA_PLAYSTATE_PAUSED;
        DoPostEvent(XA_PLAYEVENT_HEADATEND);
        iMMFPlayerState = EPlayerOpened;
        }
    else
        {
        iAudioPlayer->GetPosition(iPlaybackHead);
        iMediaPlayerState = XA_PLAYSTATE_PAUSED;
        iMMFPlayerState = EPlayerPaused;
        XAAdaptEvent event =
            {
            XA_PLAYITFEVENTS, XA_OBJECT_EVENT_RESOURCES_LOST, 0, NULL
            };
        XAAdaptationBase_SendAdaptEvents(
                (XAAdaptationBaseCtx*) iAdaptContext, &event);
        }
    iMarkerPositionTimer->Stop();
    iPlayItfPositionUpdateTimer->Stop();
    }

// from MMdaObjectStateChangeObserver
void CMMFBackendEngine::MoscoStateChangeEvent(CBase* /*aObject*/,
        TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
    {
    TInt err(KErrNone);
    iPreviousRecordState = aPreviousState;
    iCurrentRecordState = aCurrentState;
    iErrorCode = aErrorCode;
    //RDebug::Print(_L("CMMFBackendEngine::MoscoStateChangeEvent 1 Error[%d]"),aErrorCode);
    if (iCurrentRecordState == CMdaAudioClipUtility::EOpen) //EOpen
        {
        //outputfile is open and ready for recording
        iRecordState = CMMFBackendEngine::ERecorderOpen;
        if (iErrorCode == KErrNone)
            {
            if (iPreviousRecordState == CMdaAudioClipUtility::ENotReady)
                {
                //RDebug::Print(_L("CMMFBackendEngine::MoscoStateChangeEvent 2"));
                TRAP(err,iAudioInputRecord = CAudioInput::NewL( *iAudioRecorder ));
                if (err == KErrNone)
                    {
                    RArray<CAudioInput::TAudioInputPreference> inputArray;
                    inputArray.Append(CAudioInput::EDefaultMic);
                    // Set Audio Input
                    TRAP(err, iAudioInputRecord->SetAudioInputL( inputArray.Array( ) ));
                    inputArray.Close();
                    }
                TMMFMessageDestination destination(
                        KUidMetaDataWriteCustomCommand);
                TMMFMessageDestinationPckg pckg = TMMFMessageDestinationPckg(
                        destination);
                TInt ret = iAudioRecorder->RecordControllerCustomCommandSync(
                        pckg, 0, KNullDesC8, KNullDesC8);
                //RDebug::Print(_L("CMMFBackendEngine::MoscoStateChangeEvent 3 [%d]"),ret);
                if (ret != KErrNone && iFileFormat == XA_CONTAINERTYPE_MP4)
                    {
                    iPauseSupportMP4 = FALSE;
                    }
                if (iActiveSchedulerWait && iActiveSchedulerWait->IsStarted())
                    {
                    iActiveSchedulerWait->AsyncStop();
                    }
                }
            }
        else
            {
            XAAdaptEvent event =
                {
                XA_RECORDITFEVENTS, XA_OBJECT_EVENT_RESOURCES_LOST, 0, NULL
                };
            XAAdaptationBase_SendAdaptEvents(
                    (XAAdaptationBaseCtx*) iAdaptContext, &event);
            }
        }
    else if (iCurrentRecordState == CMdaAudioClipUtility::ERecording) //ERecording
        {
        iRecordState = CMMFBackendEngine::ERecorderRecording;
        iPositionUpdateTimer->Start(iTimerDelay);
        XAAdaptEvent event =
            {
            XA_RECORDITFEVENTS, XA_RECORDEVENT_HEADMOVING, 0, NULL
            };
        XAAdaptationBase_SendAdaptEvents(
                (XAAdaptationBaseCtx*) iAdaptContext, &event);
        }
    else //ENotReady
        {
        //outputfile is not open
        iRecordState = CMMFBackendEngine::ERecorderNotReady;
        }
    }

TInt CMMFBackendEngine::SetRecorderState(TRecorderState state,
        XAboolean stopCalled)
    {
    TInt err(KErrNone);

    if (iAPIBeingUsed != EAudioRecorderUtility)
        {
        return XA_RESULT_INTERNAL_ERROR;
        }

    switch (state)
        {
        case ERecorderNotReady:
            iAudioRecorder->Close();
            iRecordState = ERecorderNotReady;
            break;
        case ERecorderOpen:
            if (iFileFormat == XA_CONTAINERTYPE_MP4 && !iPauseSupportMP4
                    && !stopCalled)
                {
                err = KErrNotSupported;
                return err;
                }
            iPositionUpdateTimer->Stop();
            iAudioRecorder->Stop();
            iRecordState = ERecorderOpen;
            break;
        case ERecorderRecording:
            TRAP(err, iAudioRecorder->RecordL())
            ;
            break;
        }
    return err;
    }

void CMMFBackendEngine::Close()
    {
    if (iMarkerPositionTimer)
        {
        iMarkerPositionTimer->Stop();
        }
    if (iPlayItfPositionUpdateTimer)
        {
        iPlayItfPositionUpdateTimer->Stop();
        }
    if (iPrefetchLevelUpdateTimer)
        {
        iPrefetchLevelUpdateTimer->Stop();
        }

    if (iBaseVideoPlayer && iVideoPlayer)
        {
        switch (iMMFPlayerState)
            {
            case EPlayerPlaying:
            case EPlayerPaused:
            case EPlayerPrepared:
                iVideoPlayer->Stop();
            case EPlayerOpened:
                if (m_pWs && m_pScr && m_pWindow)
                    {
                    iVideoPlayer->RemoveDisplayWindow(*m_pWindow);
                    }
                iVideoPlayer->Close();
            case EPlayerClosed:
            default:
                break;
            };
        }

    // deleting the AudioInput object
    if (iAudioInputRecord)
        {
        delete iAudioInputRecord;
        iAudioInputRecord = NULL;
        }

    if (iBaseAudioPlayer && iAudioPlayer)
        {
        iAudioPlayer->Close();
        }

    if (iBaseAudioRecorder)
        {
        iAudioRecorder->Close();
        }

    if (iPositionUpdateTimer)
        {
        iPositionUpdateTimer->Stop();
        }

    if (iActiveSchedulerWait && iActiveSchedulerWait->IsStarted())
        {
        iActiveSchedulerWait->AsyncStop();
        }
    }

void CMMFBackendEngine::Destroy()
    {
    delete iPositionUpdateTimer;
    iPositionUpdateTimer = NULL;
    delete iUri;
    iUri = NULL;
    delete iMarkerPositionTimer;
    iMarkerPositionTimer = NULL;
    delete iPlayItfPositionUpdateTimer;
    iPlayItfPositionUpdateTimer = NULL;
	delete iPrefetchLevelUpdateTimer; 
	iPrefetchLevelUpdateTimer = NULL;
    delete iBaseVideoPlayer;
    iBaseVideoPlayer = NULL;
    iVideoPlayer = NULL;
    delete iBaseAudioPlayer;
    iBaseAudioPlayer = NULL;
    iAudioPlayer = NULL;
    delete iBaseAudioRecorder;
    iBaseAudioRecorder = NULL;
    iAudioRecorder = NULL;
    delete iActiveSchedulerWait;
    iActiveSchedulerWait = NULL;
    delete m_pScr;
    m_pScr = NULL;
    }

TInt CMMFBackendEngine::GetRecordPosition(XAuint64* position)
    {
    TInt err(KErrNone);
    *position = iAudioRecorder->Position().Int64();
    return err;
    }

TInt CMMFBackendEngine::SetPositionUpdatePerioed(XAmillisecond period)
    {
    TInt err(KErrNone);
    iTimerDelay = period;
    return err;
    }

TInt CMMFBackendEngine::SetAdaptContext(void* adaptcontext)
    {
    TInt err(KErrNone);
    iAdaptContext = adaptcontext;
    TRAP(err, iPositionUpdateTimer = new (ELeave) LocalTimer(this, iAdaptContext));
    if (!err)
        {
        iPositionUpdateTimer->PostInit();
        }
    return err;
    }

TInt CMMFBackendEngine::GetCodecId(XAuint32* codecid)
    {
    TInt err(KErrNone);

    if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        if (iRecordState != CMMFBackendEngine::ERecorderNotReady)
            {
            TFourCC dest;
            TRAP(err, dest = iAudioRecorder->DestinationDataTypeL());
            if (err == KErrNone)
                {
                *codecid = dest.FourCC();
                }
            }
        }
    else if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        if (iMediaPlayerState != XA_PLAYSTATE_PLAYERUNINITIALIZED)
            {
            TMMFMessageDestinationPckg pckg(KUidInterfaceMMFAudioController);
            TPckgBuf<TMMFAudioConfig> configPackage;
            TInt err = iAudioPlayer->CustomCommandSync(pckg,
                    EMMFAudioControllerGetSourceDataType, KNullDesC8,
                    KNullDesC8, configPackage);
            if (err == KErrNone)
                {
                *codecid = configPackage().iSourceDataTypeCode.FourCC();
                }
            }
        }
    return err;
    }

TInt CMMFBackendEngine::GetBitRate(XAuint32* bitrate)
    {
    TInt err(KErrNone);
    TUint br(0);
    if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        if (iRecordState != CMMFBackendEngine::ERecorderNotReady)
            {
            TRAP(err, br = iAudioRecorder->DestinationBitRateL());
            if (err == KErrNone)
                {
                *bitrate = br;
                }
            }
        }
    else if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        if (iMediaPlayerState != XA_PLAYSTATE_PLAYERUNINITIALIZED)
            {
            TInt err = iAudioPlayer->GetBitRate(br);
            *bitrate = br;
            }
        }
    return err;
    }

TInt CMMFBackendEngine::GetSampleRate(XAmilliHertz* samplerate)
    {
    TInt err(KErrNone);
    TUint sr(0);
    if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        if (iRecordState != CMMFBackendEngine::ERecorderNotReady)
            {
            TRAP(err, sr = iAudioRecorder->DestinationSampleRateL());
            if (err == KErrNone)
                {
                *samplerate = sr * 1000;
                }
            }
        }
    else if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        if (iMediaPlayerState != XA_PLAYSTATE_PLAYERUNINITIALIZED)
            {
            TMMFMessageDestinationPckg pckg(KUidInterfaceMMFAudioController);
            TPckgBuf<TMMFAudioConfig> configPackage;
            TInt err = iAudioPlayer->CustomCommandSync(pckg,
                    EMMFAudioControllerGetSourceSampleRate, KNullDesC8,
                    KNullDesC8, configPackage);
            if (err == KErrNone)
                {
                *samplerate = configPackage().iSampleRate * 1000;
                }
            }
        }
    return err;
    }

TInt CMMFBackendEngine::GetChannels(XAuint32* channels)
    {
    TInt err(KErrNone);
    TUint ch(0);
    if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        if (iRecordState != CMMFBackendEngine::ERecorderNotReady)
            {
            TRAP(err,ch = iAudioRecorder->DestinationNumberOfChannelsL());
            if (err == KErrNone)
                {
                *channels = ch;
                }
            }
        }
    else if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        if (iMediaPlayerState != XA_PLAYSTATE_PLAYERUNINITIALIZED)
            {
            TMMFMessageDestinationPckg pckg(KUidInterfaceMMFAudioController);
            TPckgBuf<TMMFAudioConfig> configPackage;
            TInt err = iAudioPlayer->CustomCommandSync(pckg,
                    EMMFAudioControllerGetSourceNumChannels, KNullDesC8,
                    KNullDesC8, configPackage);
            if (err == KErrNone)
                {
                *channels = configPackage().iChannels;
                }
            }
        }
    return err;
    }

TInt CMMFBackendEngine::SetDestinationBitRate(XAuint32* bitrate)
    {
    TInt err(KErrNone);
    if (iRecordState == CMMFBackendEngine::ERecorderOpen)
        {
        TRAP(err, iAudioRecorder->SetDestinationBitRateL(*bitrate));
        if (err != KErrNone)
            {
            return XA_RESULT_PARAMETER_INVALID;
            }
        }
    return err;
    }

TInt CMMFBackendEngine::SetDestinationSampleRate(XAmilliHertz* samplerate)
    {
    TInt err(KErrNone);
    if (iRecordState == CMMFBackendEngine::ERecorderOpen)
        {
        TRAP(err, iAudioRecorder->SetDestinationSampleRateL(*samplerate/1000));
        if (err != KErrNone)
            {
            return XA_RESULT_PARAMETER_INVALID;
            }
        }
    return err;
    }

TInt CMMFBackendEngine::SetDestinationChannels(XAuint32* channels)
    {
    TInt err(KErrNone);
    if (iRecordState == CMMFBackendEngine::ERecorderOpen)
        {
        TRAP(err,iAudioRecorder->SetDestinationNumberOfChannelsL(*channels));
        if (err != KErrNone)
            {
            return XA_RESULT_PARAMETER_INVALID;
            }
        }
    return err;
    }

/*
 XAresult CMMFBackendEngine::SetWindowHandle(void* display_info)
 {
 XADataLocator_NativeDisplay* nativeDisplay;
 XADataSink* videoSink = (XADataSink*)display_info;

 nativeDisplay = (XADataLocator_NativeDisplay*) (videoSink->pLocator);

 m_pWindow = ((RWindow*)(nativeDisplay->hWindow));
 m_pWs =     ((RWsSession*)(nativeDisplay->hDisplay));

 m_bWindowReferencePassed = TRUE;
 return XA_RESULT_SUCCESS;
 }

 */
XAresult CMMFBackendEngine::CreateAndConfigureWindowL()
    {
#ifdef USE_LOCAL_WINDOW_RESOURCES
    // create window for attaching the surface as its background
    //RWsSession ws;
    //TInt err2(KErrNone);
    TInt err2 = m_ws.Connect();
    m_pScr = new(ELeave) CWsScreenDevice(m_ws);
    err2 = m_pScr->Construct();
    CWindowGc* gc = NULL;
    err2 = m_pScr->CreateContext(gc);
    RWindowGroup grp(m_ws);
    err2 = grp.Construct(0xf00f00);
    const TSize KWinSize(320, 240);
    m_pWindow = new (ELeave) RWindow(m_ws);
    err2 = m_pWindow->Construct(grp, 0xfeefee);
    m_pWindow->SetExtent(TPoint(), KWinSize);
    m_pWindow->SetBackgroundColor(TRgb(255,0,0,128));
    //win.SetBackgroundColor(TRgb(0,0,0,0));
    m_pWindow->Activate();
    m_pWindow->Invalidate();
    m_pWindow->BeginRedraw();
    gc->Activate(*m_pWindow);
    m_pWindow->EndRedraw();
    m_ws.Flush();

    //Create MediaClientUitlity for NGA Surfaces
    TInt displayId = m_pScr->GetScreenNumber();

    // Add the display window
    m_cropRegion = TRect(m_pWindow->Size());
    m_clipRect = TRect(m_pWindow->Size());
    m_videoExtent = TRect(m_pWindow->Size());
    m_rotation = EVideoRotationNone;
#endif /* USE_LOCAL_WINDOW_RESOURCES */

    return XA_RESULT_SUCCESS;
    }

XAresult CMMFBackendEngine::SetNativeDisplayInformation(void* display_info)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    //display_info is of type XADataSink
    //display_info.pLocator is of type XADataLocator_NativeDisplay
    XADataLocator_NativeDisplay* nativeDisplay;
    XADataSink* videoSink = (XADataSink*) display_info;

    if (videoSink)
        {
        nativeDisplay = (XADataLocator_NativeDisplay*) (videoSink->pLocator);
        m_pWindow = ((RWindow*) (nativeDisplay->hWindow));
        m_pWs = ((RWsSession*) (nativeDisplay->hDisplay));
        /*
         m_cropRegion = TRect(m_pWindow->Size());
         m_videoExtent = TRect(m_pWindow->Size());
         m_cropRect = TRect(m_pWindow->Size());
         m_clipRect = TRect(m_pWindow->Size());
         m_cropRegion = TRect(m_pWindow->Size());
         */
        if (m_pWs)
            {
            delete m_pScr;
            m_pScr = NULL;
            TRAPD(err, m_pScr = new CWsScreenDevice(*m_pWs));
            RET_IF_ERR(err, XA_RESULT_MEMORY_FAILURE);
            m_pScr->Construct();
            }
        }
    return retVal;
    }

XAresult CMMFBackendEngine::ResumePlayback()
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    TBool postHeadMovingEvent(EFalse);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
            /* If we are already at the end of clip, do nothing
             * check ::MapcPlayComplete for documentation */
            if ((iPlaybackHead < iMediaDuration) && ((iAPIBeingUsed
                    == EAudioPlayerUtility) || (iAPIBeingUsed
                    == EVideoPlayerUtility)))
                {
                if (iAPIBeingUsed == EAudioPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    iAudioPlayer->Play();
                    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                else if (iAPIBeingUsed == EVideoPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    //iVideoPlayer->Play( iPlaybackHead, iMediaDuration);
                    iVideoPlayer->Play();
                    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                postHeadMovingEvent = ETrue;
                iMediaPlayerState = XA_PLAYSTATE_PLAYING;
                iMMFPlayerState = EPlayerPlaying;
                iMarkerPositionTimer->Start();
                iPlayItfPositionUpdateTimer->Start();
                }
            break;
        case XA_PLAYSTATE_PLAYING:
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            retVal = XA_RESULT_INTERNAL_ERROR;
            break;
        }

    if (postHeadMovingEvent)
        {
        DoPostEvent(XA_PLAYEVENT_HEADMOVING);
        }
    return retVal;
    }

XAresult CMMFBackendEngine::PausePlayback()
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_PLAYING:
        case XA_PLAYSTATE_STOPPED:
            if ((iAPIBeingUsed == EAudioPlayerUtility) || (iAPIBeingUsed
                    == EVideoPlayerUtility))
                {
                TInt pauseOpRes(KErrNone);
                if (iAPIBeingUsed == EAudioPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    pauseOpRes = iAudioPlayer->Pause();
                    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                else if (iAPIBeingUsed == EVideoPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    TRAP(pauseOpRes, iVideoPlayer->PauseL());
                    if (pauseOpRes == KErrNone)
                        {
                        TRAPD(err, iPlaybackHead = iVideoPlayer->PositionL());
                        if (err != KErrNone)
                            {
                            iPlaybackHead = 0;
                            }
                        } TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                    }
                if (pauseOpRes == KErrNone)
                    {
                    iMediaPlayerState = XA_PLAYSTATE_PAUSED;
                    iMMFPlayerState = EPlayerPaused;
                    iMarkerPositionTimer->Stop();
                    iPlayItfPositionUpdateTimer->Stop();
                    }
                }
            break;
        case XA_PLAYSTATE_PAUSED:
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            retVal = XA_RESULT_INTERNAL_ERROR;
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::StopPlayback()
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if ((iAPIBeingUsed == EAudioPlayerUtility) || (iAPIBeingUsed
                    == EVideoPlayerUtility))
                {
                if (iAPIBeingUsed == EAudioPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    iAudioPlayer->Stop();
                    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;

                    iMMFPlayerState = EPlayerOpened;
                    }
                else if (iAPIBeingUsed == EVideoPlayerUtility)
                    {
                    TAG_TIME_PROFILING_BEGIN;
                    iVideoPlayer->Stop();
                    TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;

                    iMMFPlayerState = EPlayerPrepared;
                    }
                iMediaPlayerState = XA_PLAYSTATE_STOPPED;
                iPlaybackHead = 0;
                iMarkerPositionTimer->Stop();
                iPlayItfPositionUpdateTimer->Stop();
                }
            break;
        case XA_PLAYSTATE_STOPPED:
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            retVal = XA_RESULT_INTERNAL_ERROR;
            break;
        }
    return retVal;
    }
XAresult CMMFBackendEngine::GetPlayState(XAuint32 *pState)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            *pState = iMediaPlayerState;
            retVal = XA_RESULT_SUCCESS;
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::GetDuration(XAmillisecond *pMsec)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    /* to convert from microseconds to milliseconds */
    TInt64 divider(1000);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            /* TODO Check if we query from MMF*/
            *pMsec = iMediaDuration.Int64() / divider;
            retVal = XA_RESULT_SUCCESS;
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::GetPosition(XAmillisecond *pMsec)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    TInt64 divider(1000);
    TTimeIntervalMicroSeconds pos;
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                if (iAudioPlayer->GetPosition(pos) == KErrNone)
                    {
                    *pMsec = pos.Int64() / divider;
                    retVal = XA_RESULT_SUCCESS;
                    }
                }
            else if (iAPIBeingUsed == EVideoPlayerUtility)
                {
                TInt err(KErrNone);
                TRAP(err, pos = iVideoPlayer->PositionL());
                if (err == KErrNone)
                    {
                    *pMsec = pos.Int64() / divider;
                    retVal = XA_RESULT_SUCCESS;
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetPosition(XAmillisecond pMsec)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    TInt64 multiplier(1000);
    TTimeIntervalMicroSeconds pos;
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                pos = pMsec * multiplier;
                TAG_TIME_PROFILING_BEGIN;
                iAudioPlayer->SetPosition(pos);
                TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;

                retVal = XA_RESULT_SUCCESS;
                }
            else if (iAPIBeingUsed == EVideoPlayerUtility)
                {
                pos = pMsec * multiplier;
                TAG_TIME_PROFILING_BEGIN;
                TRAPD(err, iVideoPlayer->SetPositionL(pos)); 
                TAG_TIME_PROFILING_END; 
                PRINT_TO_CONSOLE_TIME_DIFF;
                if (err == KErrNone)
                    {
                    retVal = XA_RESULT_SUCCESS;
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetRepeats(XAboolean repeat)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    TInt numRepeats(0);
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                numRepeats = repeat ? -2 : 0;
                iAudioPlayer->SetRepeats(numRepeats, 0);
                retVal = XA_RESULT_SUCCESS;
                }
            else
                {
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetPlayWindow(XAmillisecond start,
        XAmillisecond end)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);
    TInt64 multiplier(1000);
    TTimeIntervalMicroSeconds startpos(0);
    TTimeIntervalMicroSeconds endpos(0);

    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                startpos = start * multiplier;
                endpos = end * multiplier;
                retVal = iAudioPlayer->SetPlayWindow(startpos, endpos);
                }
            else
                {
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetPlayAdaptContext(void * adaptcontext)
    {
    XAresult retVal(XA_RESULT_SUCCESS);

    iAdaptContext = adaptcontext;

    return retVal;
    }

XAresult CMMFBackendEngine::RegisterCallback(xaPlayCallback cbFunction)
    {
    iPlayItfCBFunction = cbFunction;
    return DoHandlePlayItfAttributesChanged();
    }

XAresult CMMFBackendEngine::SetCallbackEventsMask(XAuint32 eventflags)
    {
    iPlayItfEventFlags = eventflags;
    return DoHandlePlayItfAttributesChanged();
    }

XAresult CMMFBackendEngine::SetMarkerPosition(XAmillisecond mSec)
    {
    iMarkerPosition = mSec;
    return DoHandlePlayItfAttributesChanged();
    }

XAresult CMMFBackendEngine::ClearMarkerPosition()
    {
    iMarkerPosition = XA_TIME_UNKNOWN;
    DoHandlePlayItfAttributesChanged();
    return XA_RESULT_SUCCESS;
    }

XAresult CMMFBackendEngine::SetPositionUpdatePeriod(XAmillisecond mSec)
    {
    iPositionUpdatePeriod = mSec;
    DoHandlePlayItfAttributesChanged();
    return XA_RESULT_SUCCESS;
    }

XAresult CMMFBackendEngine::DoHandlePlayItfAttributesChanged()
    {
    XAresult retVal(XA_RESULT_SUCCESS);

    iMarkerPositionTimer->SetCallbackEventMask(iPlayItfEventFlags);
    iMarkerPositionTimer->RegisterCallback(iPlayItfCBFunction);
    iMarkerPositionTimer->SetMarkerPosition(iMarkerPosition);

    iPlayItfPositionUpdateTimer->SetCallbackEventMask(iPlayItfEventFlags);
    iPlayItfPositionUpdateTimer->RegisterCallback(iPlayItfCBFunction);
    iPlayItfPositionUpdateTimer->SetPositionUpdatePeriod(
            iPositionUpdatePeriod);

    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
            break;
        case XA_PLAYSTATE_PLAYING:
            iMarkerPositionTimer->Start();
            iPlayItfPositionUpdateTimer->Start();
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

void CMMFBackendEngine::DoPostEvent(XAuint32 event)
    {
    if ((iPlayItfEventFlags & event) && (iPlayItfCBFunction != NULL))
        {
        XAAdaptEvent xaevent =
            {
            XA_PLAYITFEVENTS, event, 0, 0
            };
        XAAdaptationBase_SendAdaptEvents(
                (XAAdaptationBaseCtx*) iAdaptContext, &xaevent);
        }
    }

XAresult CMMFBackendEngine::GetNumStreams(XAuint32* numstreams)
{
    *numstreams = iNumStreams;
    return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::GetStreamInfo(XAuint32 streamindex,
        XAuint32* streamtype)
    {
    XAresult retVal(XA_RESULT_SUCCESS);

	switch (streamindex)
	{
		case 1: 			
			if(iAudioOnly)
			{
				*streamtype = XA_DOMAINTYPE_AUDIO;
			}
			else
			{
				*streamtype = XA_DOMAINTYPE_VIDEO;
			}
			break;
		case 2:
			*streamtype = XA_DOMAINTYPE_AUDIO;
			break;
		default:
			retVal = XA_RESULT_PARAMETER_INVALID;
			break;
	}

    return retVal;
    }

XAresult CMMFBackendEngine::GetVideoFrameSize(XAuint32* height,
        XAuint32* width, XAuint32* frameRate)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    if (iAPIBeingUsed == EVideoPlayerUtility)
        {
        if (iMediaPlayerState != XA_PLAYSTATE_PLAYERUNINITIALIZED)
            {
            TSize size;
            TReal32 framerate(0);
            TRAPD(err, iVideoPlayer->VideoFrameSizeL(size));
            if (!err)
                {
                *height = size.iHeight;
                *width = size.iWidth;
                }
            else
                {
                retVal = err;
                }

            TRAP(err, framerate = iVideoPlayer->VideoFrameRateL());
            if (!err)
                {
                *frameRate = framerate;
                }
            }
        else
            {
            retVal = XA_RESULT_PRECONDITIONS_VIOLATED;
            }
        }
    else
        {
        retVal = XA_RESULT_FEATURE_UNSUPPORTED;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetActiveState(XAuint32 streamindex,
        XAboolean active)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    TInt err(KErrNone);
    if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        retVal = XA_RESULT_FEATURE_UNSUPPORTED;
        }
    else if (iAPIBeingUsed == EVideoPlayerUtility)
        {
        switch (streamindex)
            {
            case 1:
                TRAP(err, iVideoPlayer->SetVideoEnabledL(active));
                retVal = err;
                break;
            case 2:
                TRAP(err, iVideoPlayer->SetAudioEnabledL(active));
                retVal = err;
                break;
            default:
                retVal = XA_RESULT_PARAMETER_INVALID;
                break;
            }
        }
    return retVal;
    }

TInt CMMFBackendEngine::InitializeURIForMMFUtil(char *uri)
    {
    /* Initializes and save uri param into iUri structure */
    TInt err(KErrNone);
    TInt uriLen;

    if (!uri)
        return KErrArgument;

    if (iUri)
        {
        delete iUri;
        iUri = NULL;
        }

    uriLen = strlen(uri);
    TPtr8 uriParam((TUint8*) uri, uriLen, uriLen);

    TRAP(err, iUri = HBufC::NewL(uriLen));
    if (err != KErrNone)
        return err;

    iUriPtr.Set(iUri->Des());
    iUriPtr.Copy(uriParam); /* Copy data*/
    //iUriPtr.LowerCase();
    /* For file scheme convert from file:///c:/folder/file.ext
     * format to c:\\folder\\file.ext using TUriParser. */
    _LIT(KFileScheme,"file:///");
    if (iUriPtr.Find(KFileScheme) >= 0)
        {
        iUriType = ELocal;
        TPtr tmp(const_cast<TUint16 *> (iUriPtr.Ptr())
                + KFileScheme().Length(), iUriPtr.Length(), iUriPtr.Length());
        /* Convert from c:/folder/file.ext format to
         * c:\\folder\\file.ext using TUriParser.
         * TUriParser8 accepts uri in format file:///c/folder/file.ext,
         * so get rid of ':' after drive letter (if any) */
        TInt pos = tmp.Find(_L(":"));
        if (pos != KErrNotFound)
            {
            iUriPtr.Delete(KFileScheme().Length() + pos, 1);
            }

        TUriParser16 uriParser;
        err = uriParser.Parse(iUriPtr);
        if (err != KErrNone)
            return err;

        HBufC* file = NULL;
        TRAP(err, file = uriParser.GetFileNameL());
        if (err != KErrNone)
            return err;

        iUriPtr.Copy(file->Des());
        delete file;
        file = NULL;

        }
    else
        {
        iUriType = EStreaming;
        }
    return err;
    }

XAresult CMMFBackendEngine::SetVolume(XAuint32 volume)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    TInt err(KErrNone);
    if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        retVal = iAudioPlayer->SetVolume(volume);
        }
    else if (iAPIBeingUsed == EVideoPlayerUtility)
        {
        TRAP(err, iVideoPlayer->SetVolumeL(volume));
        retVal = err;
        }
    else if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        retVal = iAudioRecorder->SetVolume(volume);
        }
    return retVal;
    }

XAresult CMMFBackendEngine::GetMaxVolume(XAuint32* maxvolume)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        *maxvolume = iAudioPlayer->MaxVolume();
        }
    else if (iAPIBeingUsed == EVideoPlayerUtility)
        {
        *maxvolume = iVideoPlayer->MaxVolume();
        }
    else if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        *maxvolume = iAudioRecorder->MaxVolume();
        }
    return retVal;
    }

XAresult CMMFBackendEngine::GetVolume(XAuint32* volume)
    {
    XAresult retVal(XA_RESULT_SUCCESS);
    TInt mmfvolume(0);
    if (iAPIBeingUsed == EAudioPlayerUtility)
        {
        retVal = iAudioPlayer->GetVolume(mmfvolume);
        if (retVal == XA_RESULT_SUCCESS)
            {
            *volume = mmfvolume;
            }
        }
    else if (iAPIBeingUsed == EVideoPlayerUtility)
        {
        *volume = iVideoPlayer->Volume();
        }
    else if (iAPIBeingUsed == EAudioRecorderUtility)
        {
        retVal = iAudioRecorder->GetVolume(mmfvolume);
        if (retVal == XA_RESULT_SUCCESS)
            {
            *volume = mmfvolume;
            }
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetPlaybackRate(XAint16 rate)
    {
    XAresult retVal(XA_RESULT_INTERNAL_ERROR);

    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TRAPD(err, iVideoPlayer->SetPlayVelocityL(rate));
                if(!err)
                    {
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::GetPlaybackRateCapabilities(XAboolean* forward,
                                                        XAboolean* backward)
    {
    XAresult retVal(XA_RESULT_PARAMETER_INVALID);
    
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TVideoPlayRateCapabilities capability;
                TRAPD(err, iVideoPlayer->GetPlayRateCapabilitiesL(capability));
                if(!err)
                    {
                    *forward = capability.iPlayForward;
                    *backward = capability.iPlayBackward;
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }
	
XAresult CMMFBackendEngine::RegisterPrefetchCallback(xaPrefetchCallback callback)
{
	if(*callback == NULL)
	{
		bPrefetchCallbackRegistered = EFalse;
	}
	else
	{
		bPrefetchCallbackRegistered = ETrue;
	}
	
    return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::SetPrefetchLevelUpdatePeriod(XApermille pfPeriod)
{
	if(iPrefetchLevelUpdateTimer)
	{
		iPrefetchLevelUpdateTimer->SetUpdateIncrement(pfPeriod); //In AL its effectively update increment and not time period
	}
		
    return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::SetPrefetchCallbackEventsMask(XAuint32 eventflags)
{
	if(eventflags & XA_PREFETCHEVENT_STATUSCHANGE)
	{
		bStatusChangeMask = ETrue;
	}

	if(iPrefetchLevelUpdateTimer)
	{
		iPrefetchLevelUpdateTimer->SetCallbackEventMask(eventflags);
	}
	
    return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::GetPrefetchStatus(XAuint32* status)
{
	if(!status)
	{
		return XA_RESULT_PARAMETER_INVALID;
	}

	*status = iPrefetchStatus;

    return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::GetPrefetchFillLevel(XApermille * level)
{
	if(!level)
	{
		return XA_RESULT_PARAMETER_INVALID;
	}

	TInt fillLevel = 0;
	iPrefetchLevelUpdateTimer->GetLoadingProgress(fillLevel);
	*level = fillLevel*10; //convert to permille

	return XA_RESULT_SUCCESS;	
}

void CMMFBackendEngine::SetStreamInfo()
{
    TInt bitRate(0);

	TInt numStreams = iNumStreams;
	bool bAudioOnly = iAudioOnly;
	
	//reset
	iNumStreams = 0;
	iAudioOnly = ETrue;
	
    if (iAPIBeingUsed == EAudioPlayerUtility)
	{
        iNumStreams = 1;
    }
    else if (iAPIBeingUsed == EVideoPlayerUtility)
    {
        TRAPD(err, bitRate = iVideoPlayer->VideoBitRateL());
        if (!err && bitRate)
        {
            iNumStreams++;
            iAudioOnly = EFalse;
        }

        bitRate = 0;
        TRAP(err, bitRate = iVideoPlayer->AudioBitRateL());
        if (!err && bitRate)
        {
            iNumStreams++;
        }
    }

	if(numStreams != iNumStreams)
	{
		SendStreamInfoEvent(0);
	}
	else if(bAudioOnly != iAudioOnly)
	{
		SendStreamInfoEvent(1);
	}
}

void CMMFBackendEngine::SendStreamInfoEvent(TInt eventData)
{
	if(iStreamInfoEventSubscribed)
	{
		XAAdaptEvent event =
			{
				XA_STREAMINFOEVENTS, eventData, 0, NULL
			};
		XAAdaptationBase_SendAdaptEvents(
			(XAAdaptationBaseCtx*) iAdaptContext, &event);
	}
}

XAresult CMMFBackendEngine::RegisterStreamInfoCallback(xaStreamEventChangeCallback cb)
{
	if(*cb)
	{
		iStreamInfoEventSubscribed = ETrue;
	}
	else
	{
		iStreamInfoEventSubscribed = EFalse;
	}

	return XA_RESULT_SUCCESS;
}

XAresult CMMFBackendEngine::SetSourceRect(const XARectangle* rect)
    {
    XAresult retVal(XA_RESULT_PARAMETER_INVALID);
    
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TRect inputrect(rect->left, rect->top, rect->left + rect->width, rect->top + rect->height);
                TAG_TIME_PROFILING_BEGIN;
                TRAPD(err, iVideoPlayer->SetCropRegionL(inputrect));
                TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                if(!err)
                    {
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetDestinationRect(const XARectangle* rect)
    {
    XAresult retVal(XA_RESULT_PARAMETER_INVALID);
    
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TRect inputrect(rect->left, rect->top, rect->left + rect->width, rect->top + rect->height);
                TAG_TIME_PROFILING_BEGIN;
                TRAPD(err, iVideoPlayer->SetVideoExtentL(*m_pWindow,inputrect));
                TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                if(!err)
                    {
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetRotation(XAmillidegree rotation)
    {
    XAresult retVal(XA_RESULT_PARAMETER_INVALID);
    
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TVideoRotation rot(EVideoRotationNone);
                switch(rotation)
                    {
                    case 0:
                        rot = EVideoRotationNone; 
                        break;
                    case 90000:
                        rot = EVideoRotationClockwise90;
                        break;
                    case 180000:
                        rot = EVideoRotationClockwise180;
                        break;
                    case 270000:
                        rot = EVideoRotationClockwise270;
                        break;
                    default:
                        rot = EVideoRotationNone; 
                        break;
                    }
                TAG_TIME_PROFILING_BEGIN;
                TRAPD(err, iVideoPlayer->SetRotationL(*m_pWindow, rot));
                TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                if(!err)
                    {
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

XAresult CMMFBackendEngine::SetScaleOptions(XAuint32 options)
    {
    XAresult retVal(XA_RESULT_PARAMETER_INVALID);
    
    switch (iMediaPlayerState)
        {
        case XA_PLAYSTATE_STOPPED:
        case XA_PLAYSTATE_PAUSED:
        case XA_PLAYSTATE_PLAYING:
            if (iAPIBeingUsed == EAudioPlayerUtility)
                {
                retVal = XA_RESULT_FEATURE_UNSUPPORTED;
                }
            else
                {
                TAutoScaleType scale(EAutoScaleBestFit);
                switch(options)
                    {
                    case XA_VIDEOSCALE_STRETCH:
                        scale = EAutoScaleStretch;
                        break;
                    case XA_VIDEOSCALE_FIT:
                        scale = EAutoScaleBestFit;
                        break;
                    case XA_VIDEOSCALE_CROP:
                        scale = EAutoScaleClip;
                        break;
                    default:
                        scale = EAutoScaleBestFit;
                        break;
                    }
                TAG_TIME_PROFILING_BEGIN;
                TRAPD(err, iVideoPlayer->SetAutoScaleL(*m_pWindow, scale));
                TAG_TIME_PROFILING_END; PRINT_TO_CONSOLE_TIME_DIFF;
                if(!err)
                    {
                    retVal = XA_RESULT_SUCCESS; 
                    }
                }
            break;
        case XA_PLAYSTATE_PLAYERUNINITIALIZED:
        default:
            break;
        }
    return retVal;
    }

extern "C"
    {

    int mmf_backend_engine_init(void** engine)
        {
        TRAPD(err, *engine = CMMFBackendEngine::NewL());
        return err;
        }

    void mmf_backend_engine_deinit(void* engine)
        {
        delete ((CMMFBackendEngine*) engine);
        }

    int mmf_set_recorder_uri(void* context, char* uri, XAuint32 format)
        {
        return ((CMMFBackendEngine*) (context))->SetFileName(uri, format,
                CMMFBackendEngine::ERecord);
        }

    int mmf_set_adapt_context(void* context, void* adaptcontext)
        {
        return ((CMMFBackendEngine*) (context))->SetAdaptContext(adaptcontext);
        }

    void mmf_close(void* context)
        {
        ((CMMFBackendEngine*) context)->Close();
        }

    int mmf_start_recording(void* context)
        {
        return ((CMMFBackendEngine*) (context))->SetRecorderState(
                CMMFBackendEngine::ERecorderRecording, FALSE);
        }

    int mmf_stop_recording(void* context, XAboolean stopCalled)
        {
        return ((CMMFBackendEngine*) (context))->SetRecorderState(
                CMMFBackendEngine::ERecorderOpen, stopCalled);
        }

    int mmf_get_record_position(void* context, XAuint64* position)
        {
        return ((CMMFBackendEngine*) (context))->GetRecordPosition(position);
        }

    int mmf_set_record_position_update_period(void* context,
            XAmillisecond msec)
        {
        return ((CMMFBackendEngine*) (context))->SetPositionUpdatePerioed(
                msec);
        }

    int mmf_get_codec_id(void* context, XAuint32* encoderId)
        {
        return ((CMMFBackendEngine*) (context))->GetCodecId(encoderId);
        }

    int mmf_get_channels(void* context, XAuint32* channelsIn)
        {
        return ((CMMFBackendEngine*) (context))->GetChannels(channelsIn);
        }

    int mmf_get_samplerate(void* context, XAmilliHertz* sampleRate)
        {
        return ((CMMFBackendEngine*) (context))->GetSampleRate(sampleRate);
        }

    int mmf_get_bitrate(void* context, XAuint32* bitRate)
        {
        return ((CMMFBackendEngine*) (context))->GetBitRate(bitRate);
        }

    int mmf_set_destination_channels(void* context, XAuint32* channelsIn)
        {
        return ((CMMFBackendEngine*) (context))->SetDestinationChannels(
                channelsIn);
        }

    int mmf_set_destination_samplerate(void* context,
            XAmilliHertz* sampleRate)
        {
        return ((CMMFBackendEngine*) (context))->SetDestinationSampleRate(
                sampleRate);
        }

    int mmf_set_destination_bitrate(void* context, XAuint32* bitRate)
        {
        return ((CMMFBackendEngine*) (context))->SetDestinationBitRate(
                bitRate);
        }

    XAresult mmf_set_play_adapt_context(void * context, void * adaptcontext)
        {
        return ((CMMFBackendEngine *) (context))->SetPlayAdaptContext(
                adaptcontext);
        }

    XAresult mmf_set_player_uri(void * context, char * uri, XAuint32 format)
        {
        return ((CMMFBackendEngine *) (context))->SetFileName(uri, format,
                CMMFBackendEngine::EPlay);
        }

    /*
     XAresult mmf_set_window_handle(void * context, void *  display_info)
     {
     return ((CMMFBackendEngine *)(context))->SetWindowHandle(display_info);
     }

     */
    XAresult mmf_setup_native_display(void * context, void * display_info)
        {
        return ((CMMFBackendEngine *) (context))->SetNativeDisplayInformation(
                display_info);
        }

    XAresult mmf_playitf_resume_playback(void * context)
        {
        return ((CMMFBackendEngine *) (context))->ResumePlayback();
        }

    XAresult mmf_playitf_pause_playback(void * context)
        {
        return ((CMMFBackendEngine *) (context))->PausePlayback();
        }

    XAresult mmf_playitf_stop_playback(void * context)
        {
        return ((CMMFBackendEngine *) (context))->StopPlayback();
        }

    XAresult mmf_playitf_get_play_state(void * context, XAuint32 * pState)
        {
        return ((CMMFBackendEngine *) (context))->GetPlayState(pState);
        }

    XAresult mmf_playitf_get_duration(void * context, XAmillisecond * pMsec)
        {
        return ((CMMFBackendEngine *) (context))->GetDuration(pMsec);
        }

    XAresult mmf_playitf_get_position(void * context, XAmillisecond * pMsec)
        {
        return ((CMMFBackendEngine *) (context))->GetPosition(pMsec);
        }

    XAresult mmf_playitf_register_callback(void * context,
            xaPlayCallback callback)
        {
        return ((CMMFBackendEngine *) (context))->RegisterCallback(callback);
        }

    XAresult mmf_playitf_set_callback_events_mask(void * context,
            XAuint32 eventflags)
        {
        return ((CMMFBackendEngine *) (context))->SetCallbackEventsMask(
                eventflags);
        }

    XAresult mmf_playitf_set_marker_position(void * context,
            XAmillisecond mSec)
        {
        return ((CMMFBackendEngine *) (context))->SetMarkerPosition(mSec);
        }

    XAresult mmf_playitf_clear_marker_position(void * context)
        {
        return ((CMMFBackendEngine *) (context))->ClearMarkerPosition();

        }

    XAresult mmf_playitf_set_position_update_period(void * context,
            XAmillisecond mSec)
        {
        return ((CMMFBackendEngine *) (context))->SetPositionUpdatePeriod(
                mSec);
        }

    XAresult mmf_seekitf_set_position(void * context, XAmillisecond pMsec)
        {
        return ((CMMFBackendEngine *) (context))->SetPosition(pMsec);
        }

    XAresult mmf_seekitf_set_playwindow(void * context, XAmillisecond start,
            XAmillisecond end)
        {
        return ((CMMFBackendEngine *) (context))->SetPlayWindow(start, end);
        }

    XAresult mmf_seekitf_set_repeats(void * context, XAboolean repeat)
        {
        return ((CMMFBackendEngine *) (context))->SetRepeats(repeat);
        }

    XAresult mmf_streaminformationitf_get_streaminfo(void * context,
            XAuint32 streamindex, XAuint32* streamtype)
        {
        return ((CMMFBackendEngine *) (context))->GetStreamInfo(streamindex,
                streamtype);
        }

    XAresult mmf_streaminformationitf_get_numstreams(void * context,
            XAuint32* numstreams)
        {
        return ((CMMFBackendEngine *) (context))->GetNumStreams(numstreams);
        }

    XAresult mmf_streaminformationitf_get_videoframesize(void * context,
            XAuint32* height, XAuint32* width, XAuint32* frameRate)
        {
        return ((CMMFBackendEngine *) (context))->GetVideoFrameSize(height,
                width, frameRate);
        }

    XAresult mmf_streaminformationitf_set_activestream(void * context,
            XAuint32 streamindex, XAboolean active)
        {
        return ((CMMFBackendEngine *) (context))->SetActiveState(streamindex,
                active);
        }

    XAresult mmf_volumeitf_set_volume(void * context, XAuint32 volume)
        {
        return ((CMMFBackendEngine *) (context))->SetVolume(volume);
        }

    XAresult mmf_volumeitf_get_maxvolume(void * context, XAuint32* volume)
        {
        return ((CMMFBackendEngine *) (context))->GetMaxVolume(volume);
        }

    XAresult mmf_volumeitf_get_volume(void * context, XAuint32* volume)
        {
        return ((CMMFBackendEngine *) (context))->GetVolume(volume);
        }

    XAresult mmf_playbackrateitf_set_playbackrate(void * context, XAint16 rate)
        {
        return ((CMMFBackendEngine *) (context))->SetPlaybackRate(rate);
        }

    XAresult mmf_playbackrateitf_get_playbackratecaps(void * context, XAboolean* forward, XAboolean* backward)
        {
        return ((CMMFBackendEngine *) (context))->GetPlaybackRateCapabilities(forward,backward);
        }
    XAresult mmf_prefetchstatusitf_register_callback(	void * context,
            												xaPrefetchCallback callback)
	{
        return ((CMMFBackendEngine *) (context))->RegisterPrefetchCallback(callback);
    }
    XAresult mmf_prefetchstatusitf_set_fill_level_update_period(	void * context,
            														XAmillisecond mSec)
	{
        return ((CMMFBackendEngine *) (context))->SetPrefetchLevelUpdatePeriod(mSec);
    }

	
    XAresult mmf_prefetchstatusitf_set_callback_events_mask(void * context,
            														XAuint32 evtMask)
	{
        return ((CMMFBackendEngine *) (context))->SetPrefetchCallbackEventsMask(evtMask);
    }

	
	XAresult mmf_prefetchstatusitf_get_status(void *context, XAuint32* status)
	{
		return ((CMMFBackendEngine *) (context))->GetPrefetchStatus(status);
	}
	
	XAresult mmf_prefetchstatusitf_get_fill_level(void *context, XApermille * fillLevel)
	{
		return ((CMMFBackendEngine *) (context))->GetPrefetchFillLevel(fillLevel);
	}
	
	XAresult mmf_streaminfoitf_register_callback(void * context, xaStreamEventChangeCallback callback)
	{
		return ((CMMFBackendEngine *) (context))->RegisterStreamInfoCallback(callback);
	}
	
    XAresult mmf_videoppitf_set_sourcerectangle(void * context, const XARectangle* rect)
        {
        return ((CMMFBackendEngine *) (context))->SetSourceRect(rect);
        }

    XAresult mmf_videoppitf_set_destinationrectangle(void * context, const XARectangle* rect)
        {
        return ((CMMFBackendEngine *) (context))->SetDestinationRect(rect);
        }

    XAresult mmf_videoppitf_set_rotation(void * context, XAmillidegree rotation)
        {
        return ((CMMFBackendEngine *) (context))->SetRotation(rotation);
        }

    XAresult mmf_videoppitf_set_scalingoptions(void * context, XAuint32 options)
        {
        return ((CMMFBackendEngine *) (context))->SetScaleOptions(options);
        }
    }
