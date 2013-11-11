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
 * Description: AudioEncoder Itf Header
 *
 */

#ifndef XAAUDIOENCODERITF_H
#define XAAUDIOENCODERITF_H

#include "xamediarecorder.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAAudioEncoderItf implementation */
typedef struct XAAudioEncoderItfImpl_
    {
    /* parent interface */
    struct XAAudioEncoderItf_ itf;
    /* pointer to self */
    struct XAAudioEncoderItfImpl_* self;

    XAMediaRecorderImpl* pObjImpl;

    /* variables */
    XAAdaptationBaseCtx *adapCtx;

    } XAAudioEncoderItfImpl;

/** METHODS **/

/* Base interface XAAudioEncoderItf implementation
 * See API Specification for method documentation
 */
XAresult XAAudioEncoderItfImpl_SetEncoderSettings(XAAudioEncoderItf self,
        XAAudioEncoderSettings *pSettings);

XAresult XAAudioEncoderItfImpl_GetEncoderSettings(XAAudioEncoderItf self,
        XAAudioEncoderSettings *pSettings);

/* XAAudioEncoderItfImpl -specific methods */
XAAudioEncoderItfImpl
        * XAAudioEncoderItfImpl_Create(XAMediaRecorderImpl* impl);
void XAAudioEncoderItfImpl_Free(XAAudioEncoderItfImpl* self);

#endif /* XAAUDIOENCODERITF_H */
