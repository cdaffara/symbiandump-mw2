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
 * Description: Media Recoder Adaptation for GST Header
 *
 */

#ifndef XAMEDIARECORDERADAPTCTX_H_
#define XAMEDIARECORDERADAPTCTX_H_

#include "xaadaptationgst.h"


/* TYPEDEFS */

/* context to track buffer insufficient event */
typedef struct recordItfCtx_
    {
    XAImplSemHandle bufInsufficientSem;
    XAboolean buffer_insufficient;
    } recodtItfCtx;

typedef struct XAMediaRecorderAdaptationCtx_ XAMediaRecorderAdaptationCtx;

typedef struct XAMediaRecorderAdaptationCtx_
    {
    /* Parent*/
    XAAdaptationGstCtx_ baseObj;

    /* OMX-AL Variables */
    XADataSource *xaAudioSource, *xaVideoSource;
    XADataSink *xaSink;
    XAuint8 recModes;

    /* GST elements */
    GstElement *datasink;
    //XAboolean isobjsink; /*is sink another XA object?*/
    GstElement *codecbin;
    //GstElement *audioppbin;
    GstElement *audiofilter;
    //GstElement *videoppbin;
    //GstElement *videofilter;
    //GstElement *videoextract;
    GstElement *audiosource;
    //GstElement *audioqueue;
    //XAboolean isobjasrc; /*is audio source another XA object?*/
    //GstElement *videosource;
    //GstState vsrcOrigState;
    //GstElement *videoqueue;
    //XAboolean isobjvsrc; /*is video source another XA object?*/
    XAboolean encodingchanged;

    //XAboolean mute;
    //XAuint32 imageEffectID;
    //XAboolean isStereoPosition;
    XAuint32 xaRecordState;
    //XAmillidegree curRotation;
    //XAuint32 curMirror;
    XAboolean isRecord;

    /* internals */
    XAboolean trackpositionenabled;
    gboolean runpositiontimer;
    GSourceFunc positionCb;

    //XAImplThreadHandle recordingEventThr;
    //recodtItfCtx recThrCtx;

    /* Variables for snapshot */
    //XASnapshotItfVars snapshotVars;

    //XAMetadataAdaptVars *metadatavars;

    /* Variables for encoders */
    XAAudioEncoderSettings audioEncSettings;
    //XAVideoSettings videoEncSettings;
    //XAImageSettings imageEncSettings;

    /*buffersink variable*/
    //guint64 writepos;

    } XAMediaRecorderAdaptationCtx_;

/* FUNCTIONS */
XAAdaptationBaseCtx* XAMediaRecorderAdapt_Create(XADataSource* pAudioSrc,
        XADataSource* pImageVideoSrc, XADataSink* pDataSnk, XAuint8 recModes);
XAresult XAMediaRecorderAdapt_PostInit(XAAdaptationGstCtx* bCtx);
void XAMediaRecorderAdapt_Destroy(XAAdaptationGstCtx* bCtx);
/*XAresult XAMediaRecorderAdapt_InitContentPipeSink(XAMediaRecorderAdaptationCtx* ctx);*/
XAresult XAMediaRecorderAdapt_ChangeEncoders(
        XAMediaRecorderAdaptationCtx_* mCtx);
XAresult XAMediaRecorderAdapt_CheckCodec(XAMediaRecorderAdaptationCtx_* mCtx,
        XACapsType encType, XAuint32 encoderId);

#endif /* XAMEDIARECORDERADAPTCTX_H_ */
