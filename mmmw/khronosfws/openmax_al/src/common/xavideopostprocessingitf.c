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

#include <stdio.h>
#include <stdlib.h>
#include "xavideopostprocessingitf.h"
 
#include "xavideopostprosessingitfadaptationmmf.h"

/**
 * XAVideoPostProcessingItfImpl* GetImpl(XAVideoPostProcessingItf self)
 * Description: Validated interface pointer and cast it to implementations pointer.
 **/
static XAVideoPostProcessingItfImpl* GetImpl(XAVideoPostProcessingItf self)
{
    if(self)
    {
        XAVideoPostProcessingItfImpl* impl = (XAVideoPostProcessingItfImpl*)(*self);
        if(impl && (impl == impl->self))
        {
            return impl;
        }
    }
    return NULL;
}

/**
 * Base interface XAVideoPostProcessingItf implementation
 */

/**
 * XAresult XAVideoPostProcessingItfImpl_SetRotation(XAVideoPostProcessingItf self,
 *                                                   XAmillidegree rotation)
 * Description: Sets post-prosessing options for rotation.
 **/
XAresult XAVideoPostProcessingItfImpl_SetRotation(XAVideoPostProcessingItf self,
                                                  XAmillidegree rotation)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    XAboolean isSupported = XA_BOOLEAN_FALSE;
    DEBUG_API("->XAVideoPostProcessingItfImpl_SetRotation");

    if(!impl)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_SetRotation");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    /* Check is arbitrary rotation supported */
    ret = XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported( self, &isSupported );

    if(ret == XA_RESULT_SUCCESS)
    {
        if( isSupported == XA_BOOLEAN_FALSE )
        {
            /* check that wanted angle is integer multiple of 90 degrees */
            if( rotation % 90000 != 0 )
            {
                /* feature unsupported */
                ret = XA_RESULT_FEATURE_UNSUPPORTED;
                return ret;
            }
        }

        impl->rotation = rotation;
        impl->isRotate = XA_BOOLEAN_TRUE;
    }

    DEBUG_API("<-XAVideoPostProcessingItfImpl_SetRotation");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported(XAVideoPostProcessingItf self,
 *                                                                    XAboolean *pSupported)
 * Description: Determines if arbitrary rotation angles are supported by the implementation.
 */
XAresult XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported(XAVideoPostProcessingItf self,
                                                                   XAboolean *pSupported)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported");
    if(!impl || !pSupported)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    impl->supported = XA_BOOLEAN_FALSE;
    *pSupported = XA_BOOLEAN_FALSE;

    DEBUG_API("<-XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_SetScaleOptions(XAVideoPostProcessingItf self,
 *                                                       XAuint32 scaleOptions,
 *                                                       XAuint32 backgroundColor,
 *                                                       XAuint32 renderingHints)
 * Description: Sets the options for scaling
 */
XAresult XAVideoPostProcessingItfImpl_SetScaleOptions(XAVideoPostProcessingItf self,
                                                      XAuint32 scaleOptions,
                                                      XAuint32 backgroundColor,
                                                      XAuint32 renderingHints)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_SetScaleOptions");

    if(!impl || (scaleOptions != XA_VIDEOSCALE_STRETCH && scaleOptions != XA_VIDEOSCALE_FIT
        && scaleOptions != XA_VIDEOSCALE_CROP))
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_SetScaleOptions");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    impl->scaleOptions = scaleOptions;
    impl->backgroundColor = backgroundColor;
    impl->renderingHints = renderingHints;
    impl->isScaleOptions = XA_BOOLEAN_TRUE;

    DEBUG_API("<-XAVideoPostProcessingItfImpl_SetScaleOptions");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_SetSourceRectangle(XAVideoPostProcessingItf self,
 *                                                          const XARectangle *pSrcRect)
 * Description: Defines the rectangle in the original frame that is to be used for further processing
 */
XAresult XAVideoPostProcessingItfImpl_SetSourceRectangle(XAVideoPostProcessingItf self,
                                                         const XARectangle *pSrcRect)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_SetSourceRectangle");

    if(!impl || !pSrcRect)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_SetSourceRectangle");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    impl->srcRect = *pSrcRect;
    impl->isSrcRect = XA_BOOLEAN_TRUE;

    DEBUG_API("<-XAVideoPostProcessingItfImpl_SetSourceRectangle");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_SetDestinationRectangle(XAVideoPostProcessingItf self,
 *                                                               const XARectangle *pDestRect)
 *
 * Description:  Defines the destination rectangle for the processed frame. This rectangle,
 * in conjunction with the scaling options used  (fit, crop, stretch) determines
 * the scaling applied to the frame.
 */
XAresult XAVideoPostProcessingItfImpl_SetDestinationRectangle(XAVideoPostProcessingItf self,
                                                              const XARectangle *pDestRect)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_SetDestinationRectangle");

    if(!impl || !pDestRect)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_SetDestinationRectangle");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    impl->destRect = *pDestRect;
    impl->isDestRect = XA_BOOLEAN_TRUE;

    DEBUG_API("<-XAVideoPostProcessingItfImpl_SetDestinationRectangle");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_SetMirror(XAVideoPostProcessingItf self,
 *                                                 XAuint32 mirror)
 * Description: Sets post-prosessing options for mirroring.
 */
XAresult XAVideoPostProcessingItfImpl_SetMirror(XAVideoPostProcessingItf self,
                                                XAuint32 mirror)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_SetMirror");

    if(!impl )
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_SetMirror");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

    switch (mirror)
    {
        case XA_VIDEOMIRROR_NONE:
            ret = XA_RESULT_SUCCESS;
            break;
        case XA_VIDEOMIRROR_VERTICAL:
        case XA_VIDEOMIRROR_HORIZONTAL:
        case XA_VIDEOMIRROR_BOTH:
        default:
            ret = XA_RESULT_PARAMETER_INVALID;
            break;
    }

    DEBUG_API("<-XAVideoPostProcessingItfImpl_SetMirror");
    return ret;
}

/**
 * XAresult XAVideoPostProcessingItfImpl_Commit(XAVideoPostProcessingItf self)
 * Description: Commit all video post-processing changes since the last Commit().
 */
XAresult XAVideoPostProcessingItfImpl_Commit(XAVideoPostProcessingItf self)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAVideoPostProcessingItfImpl* impl = GetImpl(self);
    DEBUG_API("->XAVideoPostProcessingItfImpl_Commit");
    if(!impl)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAVideoPostProcessingItfImpl_Commit");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
    }

 
    ret = XAAdaptationBase_ThreadEntry(impl->adapCtx);
    if( ret == XA_RESULT_PARAMETER_INVALID || ret == XA_RESULT_PRECONDITIONS_VIOLATED )
    {
    	DEBUG_API("<-XAVideoPostProcessingItfImpl_Commit");
    	return ret;
    }

    ret = XAVideoPostProcessingItfAdapt_Commit((XAAdaptationMMFCtx*)impl->adapCtx,
                                               impl->rotation,
                                               impl->scaleOptions,
                                               impl->backgroundColor,
                                               impl->renderingHints,
                                               &impl->srcRect,
                                               &impl->destRect,
                                               impl->mirror,
                                               impl->isMirror,
                                               impl->isRotate,
                                               impl->isDestRect,
											   impl->isSrcRect,
											   impl->isScaleOptions);

    if( ret == XA_RESULT_SUCCESS )
    {
        impl->isMirror = XA_BOOLEAN_FALSE;
        impl->isRotate = XA_BOOLEAN_FALSE;
		impl->isDestRect = XA_BOOLEAN_FALSE;
		impl->isSrcRect = XA_BOOLEAN_FALSE;
		impl->isScaleOptions = XA_BOOLEAN_FALSE;
    }

    XAAdaptationBase_ThreadExit(impl->adapCtx);
  
    DEBUG_API("<-XAVideoPostProcessingItfImpl_Commit");
    return ret;
}

/**
 * XAVideoPostProcessingItfImpl -specific methods
 **/
/**
 * XAVideoPostProcessingItfImpl* XAVideoPostProcessingItfImpl_Create()
 * @return  XAVideoPostProcessingItfImpl* - Pointer to  VideoPostProcessingItf interface implementation
 **/
XAVideoPostProcessingItfImpl* XAVideoPostProcessingItfImpl_Create(XAAdaptationBaseCtx *adapCtx)
{
    XAVideoPostProcessingItfImpl* self = (XAVideoPostProcessingItfImpl*)
        calloc(1,sizeof(XAVideoPostProcessingItfImpl));
    DEBUG_API("->XAVideoPostProcessingItfImpl_Create");
    if(self)
    {
        XARectangle emptyRect = {0,0,0,0};
        /* init itf default implementation */
        self->itf.Commit = XAVideoPostProcessingItfImpl_Commit;
        self->itf.IsArbitraryRotationSupported = XAVideoPostProcessingItfImpl_IsArbitraryRotationSupported;
        self->itf.SetDestinationRectangle = XAVideoPostProcessingItfImpl_SetDestinationRectangle;
        self->itf.SetMirror = XAVideoPostProcessingItfImpl_SetMirror;
        self->itf.SetRotation = XAVideoPostProcessingItfImpl_SetRotation;
        self->itf.SetScaleOptions = XAVideoPostProcessingItfImpl_SetScaleOptions;
        self->itf.SetSourceRectangle = XAVideoPostProcessingItfImpl_SetSourceRectangle;

        /* init variables */
        self->rotation = 0;
        self->scaleOptions = XA_VIDEOSCALE_FIT;
        self->mirror = XA_VIDEOMIRROR_NONE;
        self->backgroundColor = 0;
        self->renderingHints = XA_RENDERINGHINT_NONE;
        self->adapCtx = adapCtx;
        self->srcRect = emptyRect;
        self->destRect = emptyRect;
        self->isMirror = XA_BOOLEAN_FALSE;
        self->isRotate = XA_BOOLEAN_FALSE;
        self->isDestRect = XA_BOOLEAN_FALSE;
        self->isSrcRect = XA_BOOLEAN_FALSE;
        self->isScaleOptions = XA_BOOLEAN_FALSE;

        self->self = self;
    }
    DEBUG_API("<-XAVideoPostProcessingItfImpl_Create");
    return self;
}

/**
 * void XAVideoPostProcessingItfImpl_Free(XAVideoPostProcessingItfImpl* self);
 * @param  XAVideoPostProcessingItfImpl* self -
 **/
void XAVideoPostProcessingItfImpl_Free(XAVideoPostProcessingItfImpl* self)
{
    DEBUG_API("->XAVideoPostProcessingItfImpl_Free");
    free(self);
    DEBUG_API("<-XAVideoPostProcessingItfImpl_Free");
}
