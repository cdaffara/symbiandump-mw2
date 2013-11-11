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
 * Description: Seek Itf Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "xaseekitf.h"
#include "xathreadsafety.h"
#include "xaplayitfadaptationmmf.h"
#include "xaseekitfadaptationmmf.h"
/**
 * XASeekItfImpl* GetImpl(XASeekItf self)
 * Description: Validate interface pointer and cast it to implementation pointer.
 */
static XASeekItfImpl* GetImpl(XASeekItf self)
    {
    if (self)
        {
        XASeekItfImpl* impl = (XASeekItfImpl*) (*self);
        if (impl && impl == impl->self)
            {
            return impl;
            }
        }
    return NULL;
    }

/**
 * Base interface XASeekItf implementation
 */

/**
 * XAresult XASeekItfImpl_SetPosition(XASeekItf self, XAmillisecond pos,
 *                                    XAuint32 seekMode)
 * Description: Sets the position of the playback head.
 **/
XAresult XASeekItfImpl_SetPosition(XASeekItf self, XAmillisecond pos,
        XAuint32 seekMode)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAmillisecond duration = 0;
    XASeekItfImpl* impl = GetImpl(self);
    DEBUG_API("->XASeekItfImpl_SetPosition");
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );

    if (!impl)
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XASeekItfImpl_SetPosition");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        /* Get duration of the content */
        if (XAPlayItfAdaptMMF_GetDuration(
                (XAAdaptationBaseCtx*) impl->adapCtx, &duration)
                != XA_RESULT_SUCCESS)
            {
            /* invalid parameter */
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            DEBUG_API("<-XASeekItfImpl_SetPosition");
            return XA_RESULT_PARAMETER_INVALID;
            }
        if (pos > duration)
            {
            /* invalid parameter */
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            DEBUG_API("<-XASeekItfImpl_SetPosition");
            return XA_RESULT_PARAMETER_INVALID;
            }

        if (seekMode != XA_SEEKMODE_FAST && seekMode != XA_SEEKMODE_ACCURATE)
            {
            /* seek mode unsupported */
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_ERR("XA_RESULT_FEATURE_UNSUPPORTED");
            DEBUG_API("<-XASeekItfImpl_SetPosition");
            return XA_RESULT_FEATURE_UNSUPPORTED;
            }

        ret = XASeekItfAdaptMMF_SetPosition(impl->adapCtx, pos, seekMode);
        if (ret == XA_RESULT_SUCCESS)
            {
            impl->playbackPosition = pos;
            impl->seekMode = seekMode;
            }
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XASeekItfImpl_SetPosition");
    return ret;
    }

/**
 * XAresult XASeekItfImpl_SetLoop(XASeekItf self, XAboolean loopEnable,
 *                                XAmillisecond startPos, XAmillisecond endPos)
 * Description: Enables or disables looping and sets the start and end points of looping.
 **/
XAresult XASeekItfImpl_SetLoop(XASeekItf self, XAboolean loopEnable,
        XAmillisecond startPos, XAmillisecond endPos)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAmillisecond duration = 0;
    XASeekItfImpl* impl = GetImpl(self);
    DEBUG_API_A2("->XASeekItfImpl_SetLoop, startPos:%lu, endPos:%lu", startPos, endPos);
    XA_IMPL_THREAD_SAFETY_ENTRY( XATSMediaPlayer );

    if (!impl || (startPos > endPos))
        {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XASeekItfImpl_SetLoop");
        return XA_RESULT_PARAMETER_INVALID;
        }

    if (impl->adapCtx->fwtype == FWMgrFWMMF)
        {
        /* Get duration of the content */
        if (XAPlayItfAdaptMMF_GetDuration(
                (XAAdaptationBaseCtx*) impl->adapCtx, &duration)
                != XA_RESULT_SUCCESS)
            {
            /* invalid parameter */
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            DEBUG_API("<-XASeekItfImpl_SetLoop");
            return XA_RESULT_PARAMETER_INVALID;
            }
        if (endPos > duration && endPos != XA_TIME_UNKNOWN)
            {
            /* invalid parameter */
            XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            DEBUG_API("<-XASeekItfImpl_SetLoop");
            return XA_RESULT_PARAMETER_INVALID;
            }

        ret = XASeekItfAdaptMMF_SetLoop(impl->adapCtx, loopEnable, startPos,
                endPos);
        if (ret == XA_RESULT_SUCCESS)
            {
            impl->loopEnable = loopEnable;
            impl->startPos = startPos;
            impl->endPos = endPos;
            }
        }

    XA_IMPL_THREAD_SAFETY_EXIT( XATSMediaPlayer );
    DEBUG_API("<-XASeekItfImpl_SetLoop");
    return ret;
    }

/**
 * XAresult XASeekItfImpl_GetLoop(XASeekItf self, XAboolean *pLoopEnabled,
 *                                XAmillisecond *pStartPos,
 *                                XAmillisecond *pEndPos)
 * Description: Queries whether looping is enabled or disabled, and retrieves loop points.
 **/
XAresult XASeekItfImpl_GetLoop(XASeekItf self, XAboolean *pLoopEnabled,
        XAmillisecond *pStartPos, XAmillisecond *pEndPos)
    {
    XAresult ret = XA_RESULT_SUCCESS;

    XASeekItfImpl* impl = GetImpl(self);
    DEBUG_API("->XASeekItfImpl_GetLoop");

    if (!impl || !pLoopEnabled || !pStartPos || !pEndPos)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XASeekItfImpl_GetLoop");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    *pLoopEnabled = impl->loopEnable;
    *pStartPos = impl->startPos;
    *pEndPos = impl->endPos;

    DEBUG_API("<-XASeekItfImpl_GetLoop");
    return ret;
    }

/**
 * XASeekItfImpl -specific methods
 **/

/**
 * XASeekItfImpl* XASeekItfImpl_Create()
 * Description: Allocate and initialize SeekItfImpl.
 **/
XASeekItfImpl* XASeekItfImpl_Create(XAMediaPlayerImpl* impl)
    {
    XASeekItfImpl *self = (XASeekItfImpl*) calloc(1, sizeof(XASeekItfImpl));
    DEBUG_API("->XASeekItfImpl_Create");
    if (self)
        {
        /* init itf default implementation */
        self->itf.GetLoop = XASeekItfImpl_GetLoop;
        self->itf.SetLoop = XASeekItfImpl_SetLoop;
        self->itf.SetPosition = XASeekItfImpl_SetPosition;

        /* init variables */
        self->playbackPosition = 0;
        self->seekMode = 0;
        self->loopEnable = XA_BOOLEAN_FALSE;
        self->startPos = 0;
        self->endPos = 0;

        self->adapCtx = impl->curAdaptCtx;

        self->self = self;
        }
    DEBUG_API("<-XASeekItfImpl_Create");
    return self;
    }

/**
 * void XASeekItfImpl_Free(XASeekItfImpl* self)
 * Description: Free all resources reserved at XASeekItfImpl_Create.
 **/
void XASeekItfImpl_Free(XASeekItfImpl* self)
    {
    DEBUG_API("->XASeekItfImpl_Free");
    if(self)
        {
        free(self);
        }
    DEBUG_API("<-XASeekItfImpl_Free");
    }
