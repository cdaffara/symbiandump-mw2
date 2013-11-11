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
 * Description: Record Itf Impl
 *
 */

#ifndef XARECORDITF_H
#define XARECORDITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"

#include "xamediarecorder.h"
/** MACROS **/
#define NO_POSITION XA_TIME_UNKNOWN
#define RECORDITF_DEFAULT_UPDATE_PERIOD 1000
#define START_POSITION 0
#define DURATION_LIMIT_NOT_SETTED 0

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XARecordItf implementation */
typedef struct XARecordItfImpl_
    {
    /* parent interface */
    struct XARecordItf_ itf;
    /* pointer to self */
    struct XARecordItfImpl_* self;

    XAMediaRecorderImpl* pObjImpl;
    /* variables */
    XAuint32 recordState;
    XAboolean durationLimitSetted;
    XAmillisecond durationLimit;
    XAmillisecond markerPosition;
    XAmillisecond positionUpdatePeriod;
    void *cbcontext;
    XAuint32 eventFlags;
    XAboolean positionupdateOn;
    XAmillisecond lastPosition;

    xaRecordCallback callback;
    void* context;
    XARecordItf cbPtrToSelf;

    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;

    } XARecordItfImpl;

/** METHODS **/

/* Base interface XARecordItf implementation
 * See API Specification for method documentation
 */
XAresult XARecordItfImpl_SetRecordState(XARecordItf self, XAuint32 state);

XAresult XARecordItfImpl_GetRecordState(XARecordItf self, XAuint32 *pState);

XAresult XARecordItfImpl_SetDurationLimit(XARecordItf self,
        XAmillisecond msec);

XAresult XARecordItfImpl_GetPosition(XARecordItf self, XAmillisecond *pMsec);

XAresult XARecordItfImpl_RegisterCallback(XARecordItf self,
        xaRecordCallback callback, void *pContext);

XAresult XARecordItfImpl_SetCallbackEventsMask(XARecordItf self,
        XAuint32 eventFlags);

XAresult XARecordItfImpl_GetCallbackEventsMask(XARecordItf self,
        XAuint32 *pEventFlags);

XAresult XARecordItfImpl_SetMarkerPosition(XARecordItf self,
        XAmillisecond mSec);

XAresult XARecordItfImpl_ClearMarkerPosition(XARecordItf self);

XAresult XARecordItfImpl_GetMarkerPosition(XARecordItf self,
        XAmillisecond *pMsec);

XAresult XARecordItfImpl_SetPositionUpdatePeriod(XARecordItf self,
        XAmillisecond mSec);

XAresult XARecordItfImpl_GetPositionUpdatePeriod(XARecordItf self,
        XAmillisecond *pMsec);

/* XARecordItfImpl -specific methods */
XARecordItfImpl* XARecordItfImpl_Create(XAMediaRecorderImpl* impl);
void XARecordItfImpl_Free(XARecordItfImpl* self);
void XARecordItfImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event);

#endif
/* XARECORDITF_H */
