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
 * Description: Header File
 *
 */

#ifndef XACAPABILITIESMGR_H
#define XACAPABILITIESMGR_H

#include "xaframeworkmgr.h"
#include "openmaxalwrapper.h"
#include "xaglobals.h"

#define AUD_D (XACAP_DECODER|XACAP_AUDIO)
#define AUD_E (XACAP_ENCODER|XACAP_AUDIO)
#define VID_D (XACAP_DECODER|XACAP_VIDEO)
#define VID_E ((XACapsType)(XACAP_ENCODER|XACAP_VIDEO))
#define IMG_D (XACAP_DECODER|XACAP_IMAGE)
#define IMG_E ((XACapsType)(XACAP_ENCODER|XACAP_IMAGE))
#define AUD_O ((XACapsType)(XACAP_DEVSNK|XACAP_AUDIO))
#define AUD_I (XACAP_DEVSRC|XACAP_AUDIO)
#define VID_O (XACAP_DEVSNK|XACAP_VIDEO)
#define VID_I (XACAP_DEVSRC|XACAP_VIDEO)
#define ANY 0x7FFFFFFF

/* Structure to hold commonly used capabilities of codec/device.
 */
typedef struct XACapabilities_ XACapabilities;

struct XACapabilities_
    {
    XAuint32 capsType;
    XAuint32 xaid;      /* OpenMAX id (e.g. XA_VIDEOCODEC macro) */
    XAchar* adaptId;    /* Internal Adaptation Device IDs */
    XAuint32 noOfEntries;
    void* pEntry;

    /*add if/when needed*/
    XACapabilities* next;
    };

XAresult XACapabilitiesMgr_CreateCapabilitieList(FrameworkMap* frameworkMap,
        XACapabilities** ppListHead);

XAresult XACapabilitiesMgr_DeleteCapabilitieList(XACapabilities** ppListHead);

XAresult XACapabilitiesMgr_GetCapsCount(XACapabilities* pListHead,
        XACapsType filter, XAuint32* count);

XAresult XACapabilitiesMgr_GetCapsById(XACapabilities* pListHead,
        XACapsType filter, XAuint32 maxId, XACapabilities* data);

XAresult XACapabilitiesMgr_GetCapsByIdx(XACapabilities* pListHead,
        XACapsType filter, XAuint32 idx, XACapabilities* data);

XAresult XACapabilitiesMgr_QueryColorFormats(XACapabilities* pListHead,
        XAuint32* pIndex, XAuint32* pColorFormats);

#endif /* XACAPABILITIESMGR_H */
