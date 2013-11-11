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

#ifndef XANLINEARVOLUMEITF_H
#define XANLINEARVOLUMEITF_H

#include "openmaxalwrapper.h"
#include "xaadaptationmmf.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XANokiaLinearVolumeItf implementation */
typedef struct XANokiaLinearVolumeItfImpl_
    {
    /* parent interface */
    struct XANokiaLinearVolumeItf_ itf;
    /* pointer to self */
    struct XANokiaLinearVolumeItfImpl_* self;

    XAmillibel volumeLevel;
    XAuint32 eventFlags;

    XANokiaLinearVolumeItf cbPtrToSelf;
    xaNokiaLinearVolumeCallback callback;
    void *context;

    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;

    } XANokiaLinearVolumeItfImpl;

/** METHODS **/

/* Base interface XANokiaLinearVolumeItf implementation */
XAresult XANokiaLinearVolumeItfImpl_SetVolumeLevel(
        XANokiaLinearVolumeItf self, XAuint32 *percentage);

XAresult XANokiaLinearVolumeItfImpl_GetVolumeLevel(
        XANokiaLinearVolumeItf self, XAuint32 *percentage);

XAresult XANokiaLinearVolumeItfImpl_GetStepCount(XANokiaLinearVolumeItf self,
        XAuint32 *pStepCount);

XAresult XANokiaLinearVolumeItfImpl_RegisterVolumeCallback(
        XANokiaLinearVolumeItf self, xaNokiaLinearVolumeCallback callback,
        void * pContext);
XAresult XANokiaLinearVolumeItfImpl_SetCallbackEventsMask(
        XANokiaLinearVolumeItf self, XAuint32 eventFlags);
XAresult XANokiaLinearVolumeItfImpl_GetCallbackEventsMask(
        XANokiaLinearVolumeItf self, XAuint32 * pEventFlags);
/* XANokiaLinearVolumeItfImpl -specific methods */
XANokiaLinearVolumeItfImpl* XANokiaLinearVolumeItfImpl_Create(
        XAAdaptationBaseCtx *adapCtx);
void XANokiaLinearVolumeItfImpl_Free(XANokiaLinearVolumeItfImpl* self);
void XANokiaLinearVolumeItfImpl_AdaptCb(void *pHandlerCtx,
        XAAdaptEvent *event);

#endif /* XANLINEARVOLUMEITF_H */
