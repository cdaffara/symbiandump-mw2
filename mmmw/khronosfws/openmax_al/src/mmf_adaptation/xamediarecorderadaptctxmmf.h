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
 * Description: MediaRecorder Adaptation MMF header
 *
 */

#ifndef XAMEDIARECORDERADAPTCTXMMF_H_
#define XAMEDIARECORDERADAPTCTXMMF_H_

#include "xaadaptationmmf.h"

/* TYPEDEFS */
typedef struct XAMediaRecorderAdaptationMMFCtx_
        XAMediaRecorderAdaptationMMFCtx;

typedef struct XAMediaRecorderAdaptationMMFCtx_
    {
    /* Parent*/
    XAAdaptationMMFCtx_ baseObj;

    /* OMX-AL Variables */
    XADataSource *xaAudioSource, *xaVideoSource;
    XADataSink *xaSink;
    XAuint8 recModes;

    /* GST elements */
    //XAboolean isobjsink; /*is sink another XA object?*/
    XAboolean isobjasrc; /*is audio source another XA object?*/
    XAboolean isobjvsrc; /*is video source another XA object?*/
    XAboolean encodingchanged;

    XAboolean mute;
    XAuint32 premutevol;
    XAuint32 imageEffectID;
    XAboolean isStereoPosition;
    XAuint32 xaRecordState;
    XAmillidegree curRotation;
    XAuint32 curMirror;
    XAboolean isRecord;

    /* internals */
    XAboolean trackpositionenabled;
    gboolean runpositiontimer;

    XAImplThreadHandle recordingEventThr;

    /* Variables for encoders */
    XAAudioEncoderSettings audioEncSettings;
    XAVideoSettings videoEncSettings;
    XAImageSettings imageEncSettings;

    void* mmfContext;

    } XAMediaRecorderAdaptationMMFCtx_;

/* FUNCTIONS */
XAAdaptationBaseCtx* XAMediaRecorderAdaptMMF_Create(XADataSource* pAudioSrc,
        XADataSource* pImageVideoSrc, XADataSink* pDataSnk, XAuint8 recModes);
XAresult XAMediaRecorderAdaptMMF_PostInit(XAAdaptationMMFCtx* bCtx);
void XAMediaRecorderAdaptMMF_Destroy(XAAdaptationMMFCtx* bCtx);

#endif /* XAMEDIARECORDERADAPTCTXMMF_H_ */
