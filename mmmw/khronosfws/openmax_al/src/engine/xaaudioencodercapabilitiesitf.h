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
 * Description: Audio Encoder capabilities Itf Header
 *
 */

#ifndef XAAUDIOENCODERCAPABILITIESITF_H
#define XAAUDIOENCODERCAPABILITIESITF_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xacapabilitiesmgr.h"
/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAAudioEncoderCapabilitiesItf implementation */
typedef struct XAAudioEncoderCapabilitiesItfImpl_
    {
    /* parent interface */
    struct XAAudioEncoderCapabilitiesItf_ itf;
    /* pointer to self */
    struct XAAudioEncoderCapabilitiesItfImpl_* self;
    /* variables */
    XAuint32 numCodecs;
    XAmilliHertz sampleRateArray[10];
    XAuint32 bitRateArray[10];
    void* mmfEngine;
    XACapabilities* capslist;
    } XAAudioEncoderCapabilitiesItfImpl;

/* Base interface XAAudioEncoderCapabilitiesItf implementation */
XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoders(
        XAAudioEncoderCapabilitiesItf self, XAuint32* pNumEncoders,
        XAuint32* pEncoderIds);

XAresult XAAudioEncoderCapabilitiesItfImpl_GetAudioEncoderCapabilities(
        XAAudioEncoderCapabilitiesItf self, XAuint32 encoderId,
        XAuint32* pIndex, XAAudioCodecDescriptor* pCapabilities);

/* XAAudioEncoderCapabilitiesItfImpl -specific methods */
XAAudioEncoderCapabilitiesItfImpl* XAAudioEncoderCapabilitiesItfImpl_Create(
        XACapabilities* caps);
void XAAudioEncoderCapabilitiesItfImpl_Free(
        XAAudioEncoderCapabilitiesItfImpl* self);

#endif /* XAAUDIOENCODERCAPABILITIESITF_H */
