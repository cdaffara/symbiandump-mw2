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

#ifndef _XANOKIALINEARVOLUMEITF_H_
#define _XANOKIALINEARVOLUMEITF_H_

#include "OpenMAXAL.h"

#ifdef __cplusplus
extern "C" {
#endif


/* NOKIA LINEAR VOLUME */

#define XA_NOKIALINEARVOLUME_EVENT_VOLUME_CHANGED          ((XAuint32) 0x00000001)

XA_API extern const XAInterfaceID XA_IID_NOKIALINEARVOLUME;

struct XANokiaLinearVolumeItf_;
typedef const struct XANokiaLinearVolumeItf_ * const * XANokiaLinearVolumeItf;

typedef void (XAAPIENTRY * xaNokiaLinearVolumeCallback) (
    XANokiaLinearVolumeItf caller,
    void * pContext,
    XAuint32 event,
    XAboolean eventBooleanData
);

struct XANokiaLinearVolumeItf_ {
    XAresult (*SetVolumeLevel) (
        XANokiaLinearVolumeItf self,
        XAuint32 * pPercentage
    );
    XAresult (*GetVolumeLevel) (
        XANokiaLinearVolumeItf self,
        XAuint32 * pPercentage
    );
    XAresult (*GetStepCount) (
        XANokiaLinearVolumeItf self,
        XAuint32 * pStepCount
    );
    XAresult (*RegisterVolumeCallback) (
        XANokiaLinearVolumeItf self,
        xaNokiaLinearVolumeCallback callback, 
        void * pContext
    );
    XAresult (*SetCallbackEventsMask) (
        XANokiaLinearVolumeItf self,
        XAuint32 eventFlags
    );
    XAresult (*GetCallbackEventsMask) (
        XANokiaLinearVolumeItf self,
        XAuint32 * pEventFlags
    );
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _XANOKIALINEARVOLUMEITF_H_ */
