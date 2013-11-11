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
 * Description: Base Adaptation Macros common between adaptations.
 *
 */

#ifndef XAADPTBASECTX_H_
#define XAADPTBASECTX_H_

#include <glib.h>
#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaframeworkmgr.h"
#include "xacapabilitiesmgr.h"
#include "xaplatform.h"
/* ENUMERATIONS */

/**
 GLOBAL ENUMERATIONS
 */

typedef enum
    {
    XAMediaPlayerAdaptation = 0,
    XAMediaRecorderAdaptation,
    XARadioAdaptation,
    XACameraAdaptation,
    XAOutputMixAdaptation,
    XAVibraAdaptation,
    XALedArrayAdaptation,
    XAMDAdaptation,
    XAEngineAdaptation
    } AdaptationContextIDS;

#define XA_ADAPT_PU_INTERVAL 50                     /* position update interval */
#define XA_ADAPT_ASYNC_TIMEOUT 3000                 /* timeout to wait async events */
#define XA_ADAPT_ASYNC_TIMEOUT_SHORT 1000           /* timeout to wait async events */
#define XA_ADAPT_ASYNC_TIMEOUT_SHORT_NSEC 1000000   /* timeout to wait async events */

#define XA_ADAPT_POSITION_UPDATE_EVT 0xf0           /* position update event */
#define XA_ADAPT_SNAPSHOT_TAKEN 0xf1                /* snapshot taken event */
#define XA_ADAPT_SNAPSHOT_INITIATED 0xf2            /* snapshot intitiated event */
#define XA_ADAPT_MDE_TAGS_AVAILABLE 0xf3            /* metadata taglist changed */
#define XA_ADAPT_OMIX_DEVICESET_CHANGED 0xf4        /* Output mix device changed event */
#define XA_ADAPT_RADIO_FREQUENCY_CHANGED 0xf6       /* Radio frequency changed event */
#define XA_ADAPT_RADIO_FREQUENCY_RANGE_CHANGED 0xf7 /* Radio frequency range changed event */
#define XA_ADAPT_RADIO_SEEK_COMPLETE 0xf8           /* Radio seek complete changed event */
#define XA_ADAPT_RDS_GET_ODA_GROUP_DONE 0xf9        /* RDS get oda group done event */
#define XA_ADAPT_BUFFERING 0xfa
#define XA_ADAPT_MDE_TAGS_WRITTEN 0xfb
/* TYPEDEFS */

#define RADIO_DEFAULT_FREQ_RANGE XA_FREQRANGE_FMEUROAMERICA
#define RADIO_DEFAULT_FREQ 88000000
#define RADIO_DEFAULT_MIN_FREQ 87500000
#define RADIO_DEFAULT_MAX_FREQ 108000000

/* TYPEDEFS */

/* Adaptation device IDs */
#define XA_ADAPTID_DEVSOUNDSRC_MIC          0xAD7E5001
#define XA_ADAPTID_DEVSOUNDSINK             0xAD7E5002
#define XA_ADAPTID_AUTOAUDIOSINK            0xAD7E5003
#define XA_ADAPTID_AUDIOTESTSRC             0xAD7E5004
#define XA_ADAPTID_AUTOVIDEOSINK            0xAD7E5005
#define XA_ADAPTID_VIDEOTESTSRC             0xAD7E5006
#define XA_ADAPTID_V4L2SRC                  0xAD7E5007
#define XA_ADAPTID_JACKSINK                 0xAD7E5008
#define XA_ADAPTID_MOTIONJPEG               0xAD7E5009
#define XA_ADAPTID_THEORA                   0xAD7E500A
#define XA_ADAPTID_VORBIS                   0xAD7E500B
#define XA_ADAPTID_VIBRA                    0xAD7E500C
#define XA_ADAPTID_LEDARRAY                 0xAD7E500D
#define XA_ADAPTID_RAWVIDEO                 0xAD7E500E
#define XA_ADAPTID_DEVSOUNDSRC_FMRX         0xAD7E500F
#define XA_ADAPTID_DEVSOUNDSRC_CALL         0xAD7E5010    
    
#define XA_ADAPT_DEFAULTAUDIOOUTPUT XA_ADAPTID_DEVSOUNDSINK

#ifdef XA_USE_TEST_PLUGINS
#define XA_ADAPT_DEFAULTAUDIOINPUT  XA_ADAPTID_AUDIOTESTSRC
#else
#define XA_ADAPT_DEFAULTAUDIOINPUT  XA_ADAPTID_DEVSOUNDSRC_MIC
#endif

#define XA_ADAPT_OGGPROFILE_UNKNOWN 0x1

#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_8   0x1
#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_16  0x2
#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_20  0x4
#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_24  0x8
#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_28  0x10
#define XA_ADAPT_PCMSAMPLEFORMAT_SUPPORTED_32  0x20

#define XA_ADAPT_MAX_BITRATE_COUNT 10

#define XA_ADAPTID_UNINITED 0

#define XA_ADAPT_PLAY_STATE_CHANGED 0xf5			
#define XA_ADAPT_RADIO_FREQUENCY_CHANGED 0xf6       /* Radio frequency changed event */
#define XA_ADAPT_RADIO_FREQUENCY_RANGE_CHANGED 0xf7 /* Radio frequency range changed event */
#define XA_ADAPT_RADIO_SEEK_COMPLETE 0xf8           /* Radio seek complete changed event */
#define XA_ADAPT_RADIO_STEREO_STATUS_CHANGED 	0xf9  /* Radio stereo status changed */
#define XA_ADAPT_RADIO_SIGNAL_STRENGTH_CHANGED 0xfa   /* Radio signal status changed */
#define XA_ADAPT_VOLUME_VOLUME_CHANGED 0xfb					/* volume changed */
#define XA_ADAPT_VOLUME_MUTE_CHANGED 0xfc         	/* mute changed */
#define XA_ADAPT_VULOME_STEREOPOSITION_CHANGED  0xfd /* stereo position changed */

/* Adaptation event structure */
typedef struct XAAdaptEvent_
    {
    XAAdaptEventTypes eventtype; /* what kind of event, e.g. playitf event  */
    XAuint8 eventid; /* eventtype-specific event, e.g. XA_PLAYEVENT macro */
    XAuint32 datasize; /* event data size */
    void* data; /* event data if needed */
    } XAAdaptEvent;

/* Forward declaration of adaptation basecontext */
typedef struct XAAdaptationBaseCtx_ XAAdaptationBaseCtx;

/*
 * Structure that holds all common variables for every
 * Gst-Adaptation context structures.
 */
typedef struct XAAdaptationBaseCtx_
    {
    /*     Common Variables for all adaptation elements */
    XAuint32 ctxId;
    GArray* evtHdlrs; /*array of event handlers */
    FrameworkMap* fwkmapper;
    FWMgrFwType fwtype;
    XACapabilities* capslist;

    } XAAdaptationBaseCtx_;

/* Adaptation event callback */
typedef void (*xaAdaptEventHandler)(void *pHandlerCtx, XAAdaptEvent* event);

typedef struct XAAdaptEvtHdlr_
    {
    xaAdaptEventHandler handlerfunc; /* function handling the callback */
    void *handlercontext; /* context of handler */
    XAuint32 eventtypes; /* what kind of events this handles */
    } XAAdaptEvtHdlr;

/* FUNCTIONS */
XAresult XAAdaptationBase_Init(XAAdaptationBaseCtx* pSelf, XAuint32 ctxId);
XAresult XAAdaptationBase_PostInit(XAAdaptationBaseCtx* ctx);
XAresult XAAdaptationBase_AddEventHandler(XAAdaptationBaseCtx* ctx,
        xaAdaptEventHandler evtHandler, XAuint32 evtTypes, void *pHandlerCtx);
XAresult XAAdaptationBase_RemoveEventHandler(XAAdaptationBaseCtx* ctx,
        xaAdaptEventHandler evtHandler);
void XAAdaptationBase_SendAdaptEvents(XAAdaptationBaseCtx* ctx,
        XAAdaptEvent* event);
XAresult XAAdaptationBase_ThreadEntry(XAAdaptationBaseCtx *adaptCtx);
XAresult XAAdaptationBase_ThreadExit(XAAdaptationBaseCtx *adaptCtx);
void XAAdaptationBase_Free(XAAdaptationBaseCtx* ctx);

#endif /* XAADPTBASECTX_H_ */
