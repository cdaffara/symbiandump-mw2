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

#ifndef _XANOKIAVOLUMEEXTITF_H_
#define _XANOKIAVOLUMEEXTITF_H_

#include "OpenMAXAL.h"

#ifdef __cplusplus
extern "C" {
#endif


/* NOKIA VOLUME */

#define XA_NOKIAVOLUMEEXT_EVENT_VOLUME_CHANGED          ((XAuint32) 0x00000001)
#define XA_NOKIAVOLUMEEXT_EVENT_MUTE_CHANGED      		((XAuint32) 0x00000002)
#define XA_NOKIAVOLUMEEXT_EVENT_STEREO_POSITION_CHANGED ((XAuint32) 0x00000004)

XA_API extern const XAInterfaceID XA_IID_NOKIAVOLUMEEXT;

struct XANokiaVolumeExtItf_;
typedef const struct XANokiaVolumeExtItf_ * const * XANokiaVolumeExtItf;

typedef void (XAAPIENTRY * xaNokiaVolumeExtCallback) (
    XANokiaVolumeExtItf caller,
    void * pContext,
    XAuint32 event,
    XAboolean eventBooleanData
);

struct XANokiaVolumeExtItf_ {
    XAresult (*SetVolumeLevel) (
        XANokiaVolumeExtItf self,
        XAmillibel level
    );
    XAresult (*GetVolumeLevel) (
        XANokiaVolumeExtItf self,
        XAmillibel * pLevel
    );
    XAresult (*GetMaxVolumeLevel) (
        XANokiaVolumeExtItf self,
        XAmillibel * pMaxLevel
    );
    XAresult (*SetMute) (
        XANokiaVolumeExtItf self,
        XAboolean mute
    );
    XAresult (*GetMute) (
        XANokiaVolumeExtItf self,
        XAboolean * pMute
    );
    XAresult (*EnableStereoPosition) (
        XANokiaVolumeExtItf self,
        XAboolean enable
    );
    XAresult (*IsEnabledStereoPosition) (
        XANokiaVolumeExtItf self,
        XAboolean * pEnable
    );
    XAresult (*SetStereoPosition) (
        XANokiaVolumeExtItf self,
        XApermille stereoPosition
    );
    XAresult (*GetStereoPosition) (
        XANokiaVolumeExtItf self,
        XApermille * pStereoPosition
    );
    XAresult (*RegisterVolumeCallback) (
        XANokiaVolumeExtItf self,
        xaNokiaVolumeExtCallback callback, 
        void * pContext
    );
    XAresult (*SetCallbackEventsMask) (
        XANokiaVolumeExtItf self,
        XAuint32 eventFlags
    );
    XAresult (*GetCallbackEventsMask) (
        XANokiaVolumeExtItf self,
        XAuint32 * pEventFlags
    );
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _XANOKIAVOLUMEEXTITF_H_ */
