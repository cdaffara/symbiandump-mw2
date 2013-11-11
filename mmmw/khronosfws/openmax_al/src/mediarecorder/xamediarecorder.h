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
 * Description: MediaRecorder Object Header
 *
 */

#ifndef XAMEDIARECORDER_H
#define XAMEDIARECORDER_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaobjectitf.h"
#include "xaadptbasectx.h"
#include "xacapabilitiesmgr.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/
/* Enumeration for interfaces that MediaRecorder supports.  */
typedef enum
    {
    MR_OBJECTITF,
    MR_AUDIOENCODERITF,
    MR_DIMITF,
    MR_RECORDITF,
    MR_ITFCOUNT
    } MRInterfaces;

/** STRUCTURES **/
/* Specification for  MediaRecorderImpl. */
typedef struct XAMediaRecorderImpl_
    {
    /* Parent for XAMediaRecorder */
    XAObjectItfImpl baseObj;

    /* variables */
    XADataSource *audioSrc;
    XADataSource *imageVideoSrc;
    XADataSink *dataSnk;
    XAuint8 recModes;
    XAAdaptationBaseCtx* adaptationCtx;

    } XAMediaRecorderImpl;

/** METHODS **/

/* base object XAObjectItfImpl methods */
XAresult XAMediaRecorderImpl_DoRealize(XAObjectItf self);
XAresult XAMediaRecorderImpl_DoResume(XAObjectItf self);
void XAMediaRecorderImpl_FreeResources(XAObjectItf self);
void XAMediaRecorderImpl_AdaptCb(void *pHandlerCtx, XAAdaptEvent *event);

/* MediaRecorderImpl -specific methods*/
XAresult XAMediaRecorderImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMediaRecorderImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMediaRecorderImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);

#endif /* XAMEDIARECORDER_H */
