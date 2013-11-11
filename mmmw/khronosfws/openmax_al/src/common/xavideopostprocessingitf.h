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

#ifndef XAVIDEOPOSTPROCESSINGITF_H
#define XAVIDEOPOSTPROCESSINGITF_H

#include "xaadptbasectx.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAVideoPostProcessingItf implementation */
typedef struct XAVideoPostProcessingItfImpl_
{
    /* parent interface */
    struct XAVideoPostProcessingItf_ itf;
    /* pointer to self */
    struct XAVideoPostProcessingItfImpl_* self;

    /* variables */
    XAboolean       supported;
    XARectangle     srcRect;
    XARectangle     destRect;
    XAuint32        scaleOptions;
    XAuint32        backgroundColor;
    XAuint32        renderingHints;
    XAuint32        mirror;
    XAmillidegree   rotation;
    XAboolean       isRotate;
    XAboolean       isMirror;
    XAboolean		isDestRect;
    XAboolean		isSrcRect;
    XAboolean		isScaleOptions;

    /*Adaptation variables*/
    XAAdaptationBaseCtx *adapCtx;

} XAVideoPostProcessingItfImpl;

/** METHODS **/

/* Base interface XAVideoPostProcessingItf implementation */
XAresult XAVideoPostProcessingItfImpl_SetRotation(XAVideoPostProcessingItf self,
                                                  XAmillidegree rotation);

XAresult XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported(XAVideoPostProcessingItf self,
                                                                   XAboolean *pSupported);

XAresult XAVideoPostProcessingItfImpl_SetScaleOptions(XAVideoPostProcessingItf self,
                                                      XAuint32 scaleOptions,
                                                      XAuint32 backgroundColor,
                                                      XAuint32 renderingHints);

XAresult XAVideoPostProcessingItfImpl_SetSourceRectangle(XAVideoPostProcessingItf self,
                                                         const XARectangle *pSrcRect);

XAresult XAVideoPostProcessingItfImpl_SetDestinationRectangle(XAVideoPostProcessingItf self,
                                                              const XARectangle *pDestRect);

XAresult XAVideoPostProcessingItfImpl_SetMirror(XAVideoPostProcessingItf self,
                                                XAuint32 mirror);

XAresult XAVideoPostProcessingItfImpl_Commit(XAVideoPostProcessingItf self);
 
/* XAVideoPostProcessingItfImpl -specific methods */
XAVideoPostProcessingItfImpl* XAVideoPostProcessingItfImpl_Create(XAAdaptationBaseCtx *adapCtx);
void XAVideoPostProcessingItfImpl_Free(XAVideoPostProcessingItfImpl* self);

#endif /* XAVIDEOPOSTPROCESSINGITF_H */
