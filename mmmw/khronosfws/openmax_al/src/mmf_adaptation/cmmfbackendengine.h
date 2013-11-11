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
 * Description: MMF Backend Engine Header
 *
 */

#ifndef CMMFBackendEngine_H
#define CMMFBackendEngine_H

#define ENABLE_GRAPHICS_SURFACE_INTEGRATION

#include "openmaxalwrapper.h"

#ifdef __cplusplus

#include <e32base.h>
#include <mdaaudiosampleplayer.h>  
#include <f32file.h>
#include <videoplayer.h>
#include <videoplayer2.h>
#include <e32std.h>
#include <mmf/common/mmferrors.h>
#include <mmf/common/mmfbase.h>
#include <MMFMetadataEventConfigConstants.h>
#include <mmf/common/mmfstandardcustomcommands.h>
#ifdef ENABLE_GRAPHICS_SURFACE_INTEGRATION
#include <graphics/surface.h> //TSurfaceId
#include <mmf/common/mmfvideo.h> //TVideoAspectRatio
#include <mediaclientvideodisplay.h>
#include <surfaceeventhandler.h>
#include <mdaaudiosampleeditor.h>
#include <AudioInput.h>
#include "sfmrlocaltimer.h"
#endif

// MetaDataWrite Custom Command UID
const TUid KUidMetaDataWriteCustomCommand =
    {
    0x10207af9
    };

#define MAX_EXTENSION_SIZE 10

class CMarkerPositionTimer;
class CPositionUpdateTimer;
class CPrefetchLevelUpdateTimer;
NONSHARABLE_CLASS(CMMFBackendEngine) : public CBase,
        public MVideoPlayerUtilityObserver,
        public MVideoLoadingObserver,
        public MMdaAudioPlayerCallback,
        public MMdaObjectStateChangeObserver
    {
public:
    enum TFuncInUse
        {
                EPlay = 0, 
                ERecord
        };

    enum TUriType
        {
                ELocal = 0, 
                EStreaming
        };

    enum TMMFUtiltyInUse
        {
        ENoUtility,
        EVideoPlayerUtility,
        EAudioPlayerUtility,
        EAudioRecorderUtility
        };

    enum TRecorderState
        {
                ERecorderNotReady = 0, 
                ERecorderOpen, 
                ERecorderRecording
        };

public:

    static CMMFBackendEngine* NewL();
    ~CMMFBackendEngine();

    TInt SetFileName(char* uri, XAuint32 format, TFuncInUse func);
    void Close();
    void Destroy();
    TInt SetRecorderState(TRecorderState state, XAboolean stopCalled);
    TInt GetRecordPosition(XAuint64* position);
    TInt SetPositionUpdatePerioed(XAmillisecond period);
    TInt SetAdaptContext(void* adaptcontext);
    TInt GetCodecId(XAuint32* encoderId);
    TInt GetBitRate(XAuint32* bitrate);
    TInt GetSampleRate(XAmilliHertz* samplerate);
    TInt GetChannels(XAuint32* channels);
    TInt SetDestinationBitRate(XAuint32* bitrate);
    TInt SetDestinationSampleRate(XAmilliHertz* samplerate);
    TInt SetDestinationChannels(XAuint32* channels);

    XAresult CreateAndConfigureWindowL();
    XAresult SetNativeDisplayInformation(void* display_info);
    XAresult ResumePlayback();
    XAresult PausePlayback();
    XAresult StopPlayback();
    XAresult GetPlayState(XAuint32 *pState);
    XAresult GetDuration(XAmillisecond *pMsec);
    XAresult GetPosition(XAmillisecond *pMsec);
    XAresult SetPlayAdaptContext(void * adaptcontext);
    XAresult RegisterCallback(xaPlayCallback cbFunction);
    XAresult SetCallbackEventsMask(XAuint32 eventflags);
    XAresult SetMarkerPosition(XAmillisecond mSec);
    XAresult ClearMarkerPosition();
    XAresult SetPositionUpdatePeriod(XAmillisecond mSec);

    XAresult SetPosition(XAmillisecond pMsec);
    XAresult SetRepeats(XAboolean repeat);
    XAresult SetPlayWindow(XAmillisecond start, XAmillisecond end);

    XAresult GetNumStreams(XAuint32* numstreams);
    XAresult GetStreamInfo(XAuint32 streamindex, XAuint32* streamtype);
    XAresult GetVideoFrameSize(XAuint32* height, XAuint32* width,
            XAuint32* frameRate);
    XAresult SetActiveState(XAuint32 streamindex, XAboolean active);

    XAresult SetVolume(XAuint32 volume);
    XAresult GetVolume(XAuint32* volume);
    XAresult GetMaxVolume(XAuint32* maxvolume);

    XAresult SetPlaybackRate(XAint16 rate);
    XAresult GetPlaybackRateCapabilities(XAboolean* forward, XAboolean* backward);
	//PrefetchStatus
	XAresult RegisterPrefetchCallback(xaPrefetchCallback);
	XAresult SetPrefetchLevelUpdatePeriod(XApermille);
	XAresult SetPrefetchCallbackEventsMask(XAuint32 eventflags);
	
	XAresult GetPrefetchStatus(XAuint32* status);
	XAresult GetPrefetchFillLevel(XApermille* level);

	XAresult RegisterStreamInfoCallback(xaStreamEventChangeCallback);

    XAresult SetSourceRect(const XARectangle* rect);
    XAresult SetDestinationRect(const XARectangle* rect);
    XAresult SetRotation(XAmillidegree rotation);
    XAresult SetScaleOptions(XAuint32 options);

public:

    // MMdaObjectStateChangeObserver
    void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState,
            TInt aCurrentState, TInt aErrorcCode);

    //MMdaAudioPlayerCallback
    void MapcInitComplete(TInt aError,
            const TTimeIntervalMicroSeconds& aDuration);
    void MapcPlayComplete(TInt aError);

    //From MVidePlayerUtilityObserver
    void MvpuoOpenComplete(TInt aError);
    void MvpuoPrepareComplete(TInt aError);
    void MvpuoFrameReady(CFbsBitmap& aFrame, TInt aError);
    void MvpuoPlayComplete(TInt aError);
    void MvpuoEvent(const TMMFEvent& aEvent);

    // From MRebufferCallback
    void MvloLoadingStarted();
    void MvloLoadingComplete();

private:
    enum TPlayerState
        {
        EPlayerClosed,
        EPlayerOpened,
        EPlayerPrepared,
        EPlayerPaused,
        EPlayerPlaying,
        };

private:
    CMMFBackendEngine();
    void ConstructL();
    void InitAudioRecorderUtilityL();
    void InitAudioPlayerUtilityL();
    void InitVideoPlayerUtilityL();
    void InitPlayerTimersL();
    TInt DetermineAPIToUse(char *uri, TFuncInUse aFunc);
    XAresult DoHandlePlayItfAttributesChanged();
    void DoPostEvent(XAuint32 event);
    TInt InitializeURIForMMFUtil(char *uri);

	//StreamInfo
	void SetStreamInfo();
	void SendStreamInfoEvent(TInt);	

private:
    CVideoPlayerUtility2* iVideoPlayer;
    CBase* iBaseVideoPlayer;
    CMdaAudioPlayerUtility* iAudioPlayer;
    CBase* iBaseAudioPlayer;
    CMdaAudioRecorderUtility* iAudioRecorder;
    CBase* iBaseAudioRecorder;
    TFileName iFileName;
    TInt iAPIBeingUsed;
    TInt iPreviousRecordState;
    TInt iCurrentRecordState;
    TInt iErrorCode;
    TRecorderState iRecordState;
    LocalTimer* iPositionUpdateTimer;
    TUint64 iTimerDelay;
    void* iAdaptContext;
    XAuint32 iFileFormat;
    TInt iPauseSupportMP4;
    CActiveSchedulerWait* iActiveSchedulerWait;
    CAudioInput* iAudioInputRecord;
#ifdef USE_LOCAL_WINDOW_RESOURCES
    CMediaClientVideoDisplay* iMediaClientVideoDisplay;
    TRect m_clipRect;
    TRect m_videoExtent;
    TRect m_cropRegion;
    TVideoAspectRatio m_pixelAspectRatio;
    RWindow m_Window;
    TRect m_cropRect;
    TSurfaceId m_surfaceId;
    TVideoRotation m_rotation;
    TReal32 m_scaleWidth;
    TReal32 m_scaleHeight;
    TInt m_horizPos;
    TInt m_vertPos;
    TInt m_displayId;
    /*TBool m_bWindowReferencePassed;*/
    RWsSession m_ws;
#endif /* USE_LOCAL_WINDOW_RESOURCES */

    /* Holds duration of the media clip opend for playback */
    TTimeIntervalMicroSeconds iMediaDuration;
    /* Holds the playback position to be used for next play command */
    TTimeIntervalMicroSeconds iPlaybackHead;
    /* Holds current XA state of media player object */
    XAuint32 iMediaPlayerState;
    /* Owns */
    CMarkerPositionTimer* iMarkerPositionTimer;
    CPositionUpdateTimer* iPlayItfPositionUpdateTimer;
    /* Owns */
    CWsScreenDevice* m_pScr;
    /* Holds mmf state of media player object */
    TPlayerState iMMFPlayerState;

    /* Property set by client */
    XAuint32 iPlayItfEventFlags;
    /* Property set by client */
    xaPlayCallback iPlayItfCBFunction;
    /* Property set by client */
    XAmillisecond iMarkerPosition;
    /* Property set by client */
    XAmillisecond iPositionUpdatePeriod;

	//PrefetchStatus Interface variables
	TBool bPrefetchCallbackRegistered;
	TBool bStatusChangeMask;
    CPrefetchLevelUpdateTimer* iPrefetchLevelUpdateTimer;
    XAmillisecond iPrefetchLevelUpdatePeriod;
	XAuint32 iPrefetchStatus;

	//StreamInfo
	TInt iNumStreams;
	TBool iAudioOnly; 
	TBool iStreamInfoEventSubscribed;

    /* Property set by client (for file without file:///) */
    HBufC* iUri; /* owns */
    TPtr iUriPtr;
    TUriType iUriType;
    /* Property set by client */
    RWsSession* m_pWs;
    /* Property set by client */
    RWindow* m_pWindow;
    };

#else  /* __cplusplus */

extern int mmf_backend_engine_init(void** engine);
extern int mmf_backend_engine_deinit(void* engine);

extern int mmf_set_recorder_uri(void* context, char* uri, XAuint32 format);

extern void mmf_close(void* context);

extern int mmf_set_adapt_context(void* context, void* adaptcontext);

extern int mmf_start_recording(void* context);

extern int mmf_stop_recording(void* context, XAboolean stopCalled);

extern int mmf_get_record_position(void* context, XAuint64* position);

extern int mmf_set_record_position_update_period(void* context, XAmillisecond msec);

extern int mmf_get_codec_id(void* context, XAuint32* encoderId);
extern int mmf_get_channels(void* context, XAuint32* channelsIn);
extern int mmf_get_samplerate(void* context, XAmilliHertz* sampleRate);
extern int mmf_get_bitrate(void* context, XAuint32* bitRate);
extern int mmf_set_destination_channels(void* context, XAuint32* channelsIn);
extern int mmf_set_destination_samplerate(void* context, XAmilliHertz* sampleRate);
extern int mmf_set_destination_bitrate(void* context, XAuint32* bitRate);

extern XAresult mmf_set_play_adapt_context(void * context, void * adaptcontext);
extern XAresult mmf_set_player_uri(void * context, char * uri, XAuint32 format);
/*extern XAresult mmf_set_window_handle(void * context, void * display_infos);*/
extern XAresult mmf_setup_native_display(void * context, void * display_info);

/*Functions for XAPlayItf*/
extern XAresult mmf_playitf_resume_playback(void * context);
extern XAresult mmf_playitf_pause_playback(void * context);
extern XAresult mmf_playitf_stop_playback(void * context);
extern XAresult mmf_playitf_get_play_state(void * context, XAuint32 * pState);
extern XAresult mmf_playitf_get_duration(void * context, XAmillisecond * pMsec);
extern XAresult mmf_playitf_get_position(void * context, XAmillisecond * pMsec);
extern XAresult mmf_playitf_register_callback(void * context, xaPlayCallback callback);
extern XAresult mmf_playitf_set_callback_events_mask(void * context, XAuint32 eventflags);
extern XAresult mmf_playitf_set_marker_position(void * context, XAmillisecond mSec);
extern XAresult mmf_playitf_clear_marker_position(void * context);
extern XAresult mmf_playitf_set_position_update_period(void * context, XAmillisecond mSec);

extern XAresult mmf_seekitf_set_position(void * context, XAmillisecond mSec);
extern XAresult mmf_seekitf_set_playwindow(void * context, XAmillisecond start, XAmillisecond end);
extern XAresult mmf_seekitf_set_repeats(void * context, XAboolean repeat);

extern XAresult mmf_streaminformationitf_get_numstreams(void * context, XAuint32* numstreams);
extern XAresult mmf_streaminformationitf_get_streaminfo(void * context, XAuint32 streamindex,
        XAuint32* streamtype);
extern XAresult mmf_streaminformationitf_get_videoframesize(void * context, XAuint32* height,
        XAuint32* width, XAuint32* frameRate);
extern XAresult mmf_streaminformationitf_set_activestream(void * context, XAuint32 streamindex,
        XAboolean active);

extern XAresult mmf_volumeitf_set_volume(void * context, XAuint32 volume);
extern XAresult mmf_volumeitf_get_volume(void * context, XAuint32* volume);
extern XAresult mmf_volumeitf_get_maxvolume(void * context, XAuint32* volume);
extern XAresult mmf_playbackrateitf_set_playbackrate(void * context, XAint16 rate);
extern XAresult mmf_playbackrateitf_get_playbackratecaps(void * context,
                                XAboolean* forward, XAboolean* backward);

extern XAresult mmf_prefetchstatusitf_register_callback(void * context, xaPrefetchCallback callback);
extern XAresult mmf_prefetchstatusitf_set_callback_events_mask(void * context,	XAuint32 cbMask);
extern XAresult mmf_prefetchstatusitf_set_fill_level_update_period( void * context, XAmillisecond mSec);

extern XAresult mmf_prefetchstatusitf_get_status(void *context, XAuint32* status);
extern XAresult mmf_prefetchstatusitf_get_fill_level(void *context,XApermille* fillLevel);

extern XAresult mmf_streaminfoitf_register_callback(void * context, xaStreamEventChangeCallback callback);

extern XAresult mmf_videoppitf_set_sourcerectangle(void * context, const XARectangle* rect);
extern XAresult mmf_videoppitf_set_destinationrectangle(void * context, const XARectangle* rect);
extern XAresult mmf_videoppitf_set_rotation(void * context, XAmillidegree rotation);
extern XAresult mmf_videoppitf_set_scalingoptions(void * context, XAuint32 options);

#endif /* __cplusplus */

#endif /* CMMFBackendEngine_H */
