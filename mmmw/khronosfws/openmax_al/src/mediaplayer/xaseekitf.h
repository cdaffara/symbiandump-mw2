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
* Description: Seek Interface Header
*
*/

#ifndef XASEEKITF_H
#define XASEEKITF_H

#include "xamediaplayer.h"
/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XASeekItf implementation */
typedef struct XASeekItfImpl_
{
    /* parent interface */
    struct XASeekItf_ itf;
    /* pointer to self */
    struct XASeekItfImpl_* self;

    /* variables */
    XAmillisecond playbackPosition;
    XAuint32 seekMode;
    XAboolean loopEnable;
    XAmillisecond startPos;
    XAmillisecond endPos;

    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;
}XASeekItfImpl;

/** METHODS **/

/* Base interface XASeekItf implementation */
XAresult XASeekItfImpl_SetPosition(XASeekItf self, XAmillisecond pos,
                                   XAuint32 seekMode);

XAresult XASeekItfImpl_SetLoop(XASeekItf self, XAboolean loopEnable,
                               XAmillisecond startPos,
                               XAmillisecond endPos);

XAresult XASeekItfImpl_GetLoop(XASeekItf self, XAboolean *pLoopEnabled,
                               XAmillisecond *pStartPos,
                               XAmillisecond *pEndPos);

/* XASeekItfImpl -specific methods */
XASeekItfImpl* XASeekItfImpl_Create( XAMediaPlayerImpl* impl );
void XASeekItfImpl_Free(XASeekItfImpl* self);

#endif /* XASEEKITF_H */
