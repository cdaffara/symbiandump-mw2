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
 * Description: Volume Interface Header
 *
 */

#ifndef XAVOLUMEITF_H
#define XAVOLUMEITF_H

#include "xaadptbasectx.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAVolumeItf implementation */
typedef struct XAVolumeItfImpl_
    {
    /* parent interface */
    struct XAVolumeItf_ itf;
    /* pointer to self */
    struct XAVolumeItfImpl_* self;

    XAmillibel volumeLevel;
    XAboolean mute;
    XAboolean enableStereoPos;
    XApermille stereoPosition;
    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;
    } XAVolumeItfImpl;

/** METHODS **/

/* Base interface XAVolumeItf implementation */
XAresult XAVolumeItfImpl_SetVolumeLevel(XAVolumeItf self, XAmillibel level);

XAresult XAVolumeItfImpl_GetVolumeLevel(XAVolumeItf self, XAmillibel *pLevel);

XAresult XAVolumeItfImpl_GetMaxVolumeLevel(XAVolumeItf self,
        XAmillibel *pMaxLevel);

XAresult XAVolumeItfImpl_SetMute(XAVolumeItf self, XAboolean mute);

XAresult XAVolumeItfImpl_GetMute(XAVolumeItf self, XAboolean *pMute);

XAresult XAVolumeItfImpl_EnableStereoPosition(XAVolumeItf self,
        XAboolean enable);

XAresult XAVolumeItfImpl_IsEnabledStereoPosition(XAVolumeItf self,
        XAboolean *pEnable);

XAresult XAVolumeItfImpl_SetStereoPosition(XAVolumeItf self,
        XApermille stereoPosition);

XAresult XAVolumeItfImpl_GetStereoPosition(XAVolumeItf self,
        XApermille *pStereoPosition);

/* XAVolumeItfImpl -specific methods */
XAVolumeItfImpl* XAVolumeItfImpl_Create(XAAdaptationBaseCtx *adapCtx);
void XAVolumeItfImpl_Free(XAVolumeItfImpl* self);

#endif /* XAVOLUMEITF_H */
