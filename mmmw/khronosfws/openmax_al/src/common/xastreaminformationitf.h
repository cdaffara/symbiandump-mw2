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
 * Description: Stream Information Interface Header
 *
 */

#ifndef XASTREAMINFORMATIONITF_H
#define XASTREAMINFORMATIONITF_H

#include "openmaxalwrapper.h"
#include "xaadptbasectx.h"
#include "xamediaplayer.h"


/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAStreamInformationItf implementation */
typedef struct XAStreamInformationItfImpl_
    {
    /* parent interface */
    struct XAStreamInformationItf_ itf;
    /* pointer to self */
    struct XAStreamInformationItfImpl_* self;

    /*Adaptation variables*/

    XAStreamInformationItf cbPtrToSelf;
    xaStreamEventChangeCallback callback;
    void *cbcontext;
    XAAdaptationBaseCtx *adapCtx;

    } XAStreamInformationItfImpl;

/** METHODS **/

/* Base interface XAStreamInformationItf implementation */
XAresult XAStreamInformationItfImpl_QueryMediaContainerInformation(
        XAStreamInformationItf self, XAMediaContainerInformation * info);

XAresult XAStreamInformationItfImpl_QueryStreamType(
        XAStreamInformationItf self, XAuint32 streamIndex, XAuint32 *domain);

XAresult XAStreamInformationItfImpl_QueryStreamInformation(
        XAStreamInformationItf self, XAuint32 streamIndex, void * info);

XAresult XAStreamInformationItfImpl_QueryStreamName(
        XAStreamInformationItf self, XAuint32 streamIndex,
        XAuint16 * pNameSize, XAchar * pName);

XAresult XAStreamInformationItfImpl_RegisterStreamChangeCallback(
        XAStreamInformationItf self, xaStreamEventChangeCallback callback,
        void * pContext);

XAresult XAStreamInformationItfImpl_QueryActiveStreams(
        XAStreamInformationItf self, XAuint32 *numStreams,
        XAboolean *activeStreams);

XAresult XAStreamInformationItfImpl_SetActiveStream(
        XAStreamInformationItf self, XAuint32 streamNum, XAboolean active,
        XAboolean commitNow);

/* XAStreamInformationItfImpl -specific methods */
XAStreamInformationItfImpl* XAStreamInformationItfImpl_Create(
        XAMediaPlayerImpl* impl);
void XAStreamInformationItfImpl_Free(XAStreamInformationItfImpl* self);

void XAStreamInformationItfImpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event );


#endif /* XASTREAMINFORMATIONITF_H */
