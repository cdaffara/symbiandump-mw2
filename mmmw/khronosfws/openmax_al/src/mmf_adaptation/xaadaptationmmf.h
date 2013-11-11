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

#ifndef XAADAPTATIONMMF_H_
#define XAADAPTATIONMMF_H_

#include <pthread.h>
#include <semaphore.h>
#include <garray.h>
#include <string.h>
#include <unistd.h>
#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaplatform.h"
#include "xaadptbasectx.h"
#ifdef XA_IMPL_MEASURE_GST_DELAY
#include <time.h>
#endif /* XA_IMPL_MEASURE_GST_DELAY */

/* MACROS */
#define FLIP_NONE               0
#define FLIP_CLOCKWISE          1 /* Rotate clockwise 90 degrees */
#define FLIP_ROTATE_180         2 /* Rotate 180 degrees */
#define FLIP_COUNTERCLOCKWISE   3 /* Rotate counter-clockwise 90 degrees */
#define FLIP_HORIZONTAL         4 /* Flip image horizontally */
#define FLIP_VERTICAL           5 /* Flip image vertically */

/* TYPEDEFS */

#define CONTENT_PIPE_BUFFER_SIZE 1000
#define TEST_VIDEO_WIDTH     640
#define TEST_VIDEO_HEIGHT    480

/* Forward declaration of adaptation basecontext */
typedef struct XAAdaptationMMFCtx_ XAAdaptationMMFCtx;

/*
 * Structure that holds all common variables for every
 * mmf-Adaptation context structures.
 */
typedef struct XAAdaptationMMFCtx_
    {
    XAAdaptationBaseCtx baseObj;

    } XAAdaptationMMFCtx_;

typedef struct XAEngineAdaptationMMFCtx_
    {
    /* Parent*/
    XAAdaptationMMFCtx_ baseObj;

    } XAEngineAdaptationMMFCtx_;

/*
 * Structure for Media Player specific gst-adaptation.
 */
typedef struct XAMediaPlayerAdaptationMMFCtx_
    {
    /* Parent*/
    XAAdaptationMMFCtx_ baseObj;

    /* OMX-AL Variables */
    XADataSource *xaSource, *xaBankSrc;
    XADataSink *xaAudioSink, *xaVideoSink, *xaLEDArray, *xaVibra;

    /* MMF elements */
    XAboolean isobjsrc; /*is source another XA object?*/

    XAboolean isobjasink; /*is audio sink another XA object?*/
    XAboolean isobjvsink; /*is video sink another XA object?*/

    XAboolean mute;
    XAuint32 premutevol;
    XAuint32 imageEffectID;
    XAboolean isStereoPosition;
    XAmillidegree curRotation;
    XAuint32 curMirror;

    XAint32 buffering;
    XAuint32 rateprops;
    XAint16 playrate;
    /* internals */
    XAboolean trackpositionenabled;

    XAboolean loopingenabled;

    XAboolean cameraSinkSynced;
    void* mmfContext;
    void* mmfMetadataContext;
    XAboolean isForRadio;
    /*
     XAuint32                playerState;
     */

    } XAMediaPlayerAdaptationMMFCtx_;

typedef struct XAMetadataAdaptationMMFCtx_
    {
    /* Parent*/
    XAAdaptationMMFCtx_ baseObj;

    /* OMX-AL Variables */
    XADataSource *xaSource;

    void* mmfContext;
    } XAMetadataAdaptationMMFCtx_;

/*
 * Structure for Radio specific variables
 */
typedef struct XARadioAdaptationCtx_
    {

    /* Parent*/
    XAAdaptationMMFCtx_ baseObj;
    XAuint32 frequency;
    XAuint8 range;

    pthread_t emulationThread;
    pthread_t rdsEmulationThread;

    } XARadioAdaptationCtx_;
/* FUNCTIONS */

XAresult XAAdaptationBaseMMF_Init(XAAdaptationMMFCtx* pSelf, XAuint32 ctxId);
XAresult XAAdaptationBaseMMF_PostInit(XAAdaptationMMFCtx* ctx);
void XAAdaptationBaseMMF_Free(XAAdaptationMMFCtx* ctx);

#endif /* XAADAPTATIONMMF_H_ */
