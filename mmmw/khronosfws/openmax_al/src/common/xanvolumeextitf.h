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

#ifndef XANVOLUMEEXTITF_H
#define XANVOLUMEEXTITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaadptbasectx.h"

/** MACROS **/

/* Max volume level is implementation-dependent but must be at least 0mB
 * now used max volume 10 mB
 */
#define MAX_VOLUME_LEVEL 10
#define MAX_VOLUME_AT_LEAST 0

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XANokiaVolumeExtItf implementation */
typedef struct XANokiaVolumeExtItfImpl_
    {
    /* parent interface */
    struct XANokiaVolumeExtItf_ itf;
    /* pointer to self */
    struct XANokiaVolumeExtItfImpl_* self;

    XAmillibel volumeLevel;
    XAboolean mute;
    XAboolean enableStereoPos;
    XApermille stereoPosition;
    XAuint32 eventFlags;
    XANokiaVolumeExtItf cbPtrToSelf;
    xaNokiaVolumeExtCallback callback;
    void *context;

    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;

    } XANokiaVolumeExtItfImpl;

/** METHODS **/

/* Base interface XANokiaVolumeExtItf implementation */
XAresult XANokiaVolumeExtItfImpl_SetVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel level);

XAresult XANokiaVolumeExtItfImpl_GetVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel *pLevel);

XAresult XANokiaVolumeExtItfImpl_GetMaxVolumeLevel(XANokiaVolumeExtItf self,
        XAmillibel *pMaxLevel);

XAresult XANokiaVolumeExtItfImpl_SetMute(XANokiaVolumeExtItf self,
        XAboolean mute);

XAresult XANokiaVolumeExtItfImpl_GetMute(XANokiaVolumeExtItf self,
        XAboolean *pMute);

XAresult XANokiaVolumeExtItfImpl_EnableStereoPosition(
        XANokiaVolumeExtItf self, XAboolean enable);

XAresult XANokiaVolumeExtItfImpl_IsEnabledStereoPosition(
        XANokiaVolumeExtItf self, XAboolean *pEnable);

XAresult XANokiaVolumeExtItfImpl_SetStereoPosition(XANokiaVolumeExtItf self,
        XApermille stereoPosition);

XAresult XANokiaVolumeExtItfImpl_GetStereoPosition(XANokiaVolumeExtItf self,
        XApermille *pStereoPosition);

XAresult XANokiaVolumeExtItfImpl_RegisterVolumeCallback(
        XANokiaVolumeExtItf self, xaNokiaVolumeExtCallback callback,
        void * pContext);
XAresult XANokiaVolumeExtItfImpl_SetCallbackEventsMask(
        XANokiaVolumeExtItf self, XAuint32 eventFlags);
XAresult XANokiaVolumeExtItfImpl_GetCallbackEventsMask(
        XANokiaVolumeExtItf self, XAuint32 * pEventFlags);

/* XANokiaVolumeExtItfImpl -specific methods */
XANokiaVolumeExtItfImpl* XANokiaVolumeExtItfImpl_Create(
        XAAdaptationBaseCtx *adapCtx);
void XANokiaVolumeExtItfImpl_Free(XANokiaVolumeExtItfImpl* self);
void XANokiaVolumeExtItfImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event);

#endif /* XANVOLUMEEXTITF_H */
