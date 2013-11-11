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
#include <string.h>
#include "xaradioitf.h"
#include "xaradioitfadaptation.h"
#include "xathreadsafety.h"

#define FREQINTERVAL 200000

/**
 * XARadioItfImpl* GetImpl(XARadioItf self)
 * Description: Validated interface pointer and cast it to implementations pointer.
 **/
static XARadioItfImpl* GetImpl(XARadioItf self)
{
    if(self)
    {
        XARadioItfImpl *impl = (XARadioItfImpl*)(*self);
        if(impl && impl == impl->self)
        {
            return impl;
        }
    }
    return NULL;
}

/**
 * Base interface XARadioItf implementation
 */

/**
 * XAresult XARadioItfImpl_SetFreqRange(XARadioItf self, XAuint8 range)
 * Description: Sets the frequency range. Asynchronous – xaRadioCallback() callback with
 *              XA_RADIO_EVENT_FREQUENCY_RANGE_CHANGED event is used for notifying of the result.
 **/
XAresult XARadioItfImpl_SetFreqRange(XARadioItf self, XAuint8 range)
{
    XAresult ret = XA_RESULT_PARAMETER_INVALID;
    XAboolean isSupported = XA_BOOLEAN_FALSE;
    XARadioItfImpl* impl = GetImpl(self);

    DEBUG_API("->XARadioItfImpl_SetFreqRange");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_SetFreqRange");

        return ret;
    }

    ret = XARadioItfAdapt_IsFreqRangeSupported(range, &isSupported);

    if ( ret == XA_RESULT_SUCCESS && isSupported == XA_BOOLEAN_TRUE )
    {
        ret = XARadioItfAdapt_SetFreqRange((XAAdaptationMMFCtx*)impl->adapCtx, range);
    }
    else if (!isSupported)
    {
    	ret = XA_RESULT_PARAMETER_INVALID;
    }

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_SetFreqRange");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetFreqRange(XARadioItf self, XAuint8 * pRange)
 * Description: Gets the current frequency range.
 **/
XAresult XARadioItfImpl_GetFreqRange(XARadioItf self, XAuint8 * pRange)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_GetFreqRange");

    if(!impl || !pRange)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_GetFreqRange");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_GetFreqRange(pRange);

    DEBUG_API("<-XARadioItfImpl_GetFreqRange");
    return ret;
}

/**
 * XAresult XARadioItfImpl_IsFreqRangeSupported(XARadioItf self,
 *                                              XAuint8 range,
 *                                              XAboolean * pSupported)
 * Description: Queries if the given frequency range is supported.
 **/
XAresult XARadioItfImpl_IsFreqRangeSupported(XARadioItf self,
                                             XAuint8 range,
                                             XAboolean * pSupported)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_IsFreqRangeSupported");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl || !pSupported)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_IsFreqRangeSupported");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_IsFreqRangeSupported( range, pSupported );

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_IsFreqRangeSupported");
    return ret;
}


/**
 * XAresult XARadioItfImpl_GetFreqRangeProperties(XARadioItf self,
 *                                                XAuint8  range,
 *                                                XAuint32 * pMinFreq,
 *                                                XAuint32 * pMaxFreq,
 *                                                XAuint32 * pFreqInterval)
 * Description: Returns the minimum and maximum supported frequencies and the
 *              modulation of the given frequency range.
 **/
XAresult XARadioItfImpl_GetFreqRangeProperties(XARadioItf self,
                                               XAuint8  range,
                                               XAuint32 * pMinFreq,
                                               XAuint32 * pMaxFreq,
                                               XAuint32 * pFreqInterval)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);

    DEBUG_API("->XARadioItfImpl_GetFreqRangeProperties");

    if(!impl || !pMinFreq || !pMaxFreq || !pFreqInterval)
    {
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_GetFreqRangeProperties");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    ret = XARadioItfAdapt_GetFreqRangeProperties( (XAAdaptationMMFCtx*)impl->adapCtx,
            range, pMinFreq, pMaxFreq );
    if (ret == XA_RESULT_SUCCESS)
    {
        *pFreqInterval = FREQINTERVAL;
    }

    DEBUG_API("<-XARadioItfImpl_GetFreqRangeProperties");
    return ret;
}

/**
 * XAresult XARadioItfImpl_SetFrequency(XARadioItf self, XAuint32 freq)
 * Description: Sets the frequency asynchronously – xaRadioCallback() callback
 *              with XA_RADIO_EVENT_FREQUENCY_CHANGED event is used for notifying
 *              of the result. The implementation rounds the given value to the
 *              nearest supported one. See pFreqInterval parameter of
 *              GetFreqRangeProperties() method.
 **/
XAresult XARadioItfImpl_SetFrequency(XARadioItf self, XAuint32 freq)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint8 range = 1; // Default to EuroAmerica
    XAuint32 minFreq;
    XAuint32 maxFreq;
   	XAuint32 freqInterval;
        
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_SetFrequency");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_SetFrequency");
        return XA_RESULT_PARAMETER_INVALID;
    }
    
    // Check for valid entries: 
 		ret = XARadioItfImpl_GetFreqRangeProperties(self, range, &minFreq, &maxFreq, &freqInterval);   
    if (ret != XA_RESULT_SUCCESS)
    {
    	XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    	DEBUG_API("<-XARadioItfImpl_SetFrequency");    	
    	return ret;
    }		
    
    if ( (freq < minFreq) || (freq > maxFreq) )
    {
    	XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    	DEBUG_API("<-XARadioItfImpl_SetFrequency");    	
    	return XA_RESULT_PARAMETER_INVALID;
    }	  
           
   	ret = XARadioItfAdapt_SetFrequency( (XAAdaptationMMFCtx*)impl->adapCtx, freq );

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_SetFrequency");
    return ret;
}

/**
 * XAresult XARadioItfImpl_CancelSetFrequency(XARadioItf self)
 * Description: Cancels an outstanding SetFrequency() request. The method
 *              blocks while canceling the outstanding request. Has not effect
 *              if no set frequency operation is ongoing.
 **/
XAresult XARadioItfImpl_CancelSetFrequency(XARadioItf self)

{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_CancelSetFrequency");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_CancelSetFrequency");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_CancelSetFrequency();

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_CancelSetFrequency");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetFrequency(XARadioItf self, XAuint32 * pFreq)
 * Description: Gets the current frequency.
 **/
XAresult XARadioItfImpl_GetFrequency(XARadioItf self, XAuint32 * pFreq)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_GetFrequency");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl || !pFreq)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_GetFrequency");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_GetFrequency(pFreq);

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_GetFrequency");
    return ret;
}

/**
 * XAresult XARadioItfImpl_SetSquelch(XARadioItf self, XAboolean squelch)
 * Description: Toggles the squelch (muting in frequencies without broadcast).
 **/
XAresult XARadioItfImpl_SetSquelch(XARadioItf self, XAboolean squelch)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_SetSquelch");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_SetSquelch");
        return XA_RESULT_PARAMETER_INVALID;
    }

    if (impl->squelch != squelch)
    {
        ret = XARadioItfAdapt_SetSquelch( (XAAdaptationMMFCtx*)impl->adapCtx, squelch );
        if ( ret == XA_RESULT_SUCCESS )
        {
            impl->squelch = squelch;
        }
    }

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_SetSquelch");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetSquelch(XARadioItf self, XAboolean * pSquelch)
 * Description: Queries the squelch setting (muting in frequencies without broadcast).
 **/
XAresult XARadioItfImpl_GetSquelch(XARadioItf self, XAboolean * pSquelch)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_GetSquelch");
    if(!impl || !pSquelch)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XARadioItfImpl_GetSquelch");
        return XA_RESULT_PARAMETER_INVALID;
    }

  	ret = XARadioItfAdapt_GetSquelch( pSquelch );

    DEBUG_API("<-XARadioItfImpl_GetSquelch");
    return ret;
}

/**
 * XAresult XARadioItfImpl_SetStereoMode(XARadioItf self, XAuint32 mode)
 * Description: Sets the current stereo mode.
 **/
XAresult XARadioItfImpl_SetStereoMode(XARadioItf self, XAuint32 mode)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_SetStereoMode");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if( !impl || mode > XA_STEREOMODE_AUTO )
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_SetStereoMode");
        return XA_RESULT_PARAMETER_INVALID;
    }
    if ( impl->stereoMode != mode)
    {
        ret = XARadioItfAdapt_SetStereoMode( (XAAdaptationMMFCtx*)impl->adapCtx, mode );
       	if ( ret == XA_RESULT_SUCCESS )
        {
         	  impl->stereoMode = mode;
        }
    }    
    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_SetStereoMode");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetStereoMode(XARadioItf self, XAuint32 * pMode)
 * Description: Queries the current stereo mode.
 **/
XAresult XARadioItfImpl_GetStereoMode(XARadioItf self, XAuint32 * pMode)
{

    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_GetStereoMode");
    if(!impl || !pMode)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XARadioItfImpl_GetStereoMode");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_GetStereoMode( pMode );

    DEBUG_API("<-XARadioItfImpl_GetStereoMode");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetSignalStrength(XARadioItf self, XAuint32 * pStrength)
 * Description: Returns the signal strength in per cents.
 **/
XAresult XARadioItfImpl_GetSignalStrength(XARadioItf self, XAuint32 * pStrength)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_GetSignalStrength");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl || !pStrength)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_GetSignalStrength");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_GetSignalStrength( pStrength );

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_GetSignalStrength");
    return ret;
}

/**
 * XAresult XARadioItfImpl_Seek(XARadioItf self, XAboolean upwards)
 * Description: Starts the seek from the current frequency to the given direction.
 *              Asynchronous – xaRadioCallback() callback with XA_RADIO_EVENT_SEEK_COMPLETED
 *              event is used for notifying of the result. If the end of the tuner’s
 *              frequency band is reached before a signal was found, the scan continues
 *              from the other end until a signal is found or the original frequency is reached.
 **/
XAresult XARadioItfImpl_Seek(XARadioItf self, XAboolean upwards)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_Seek");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        /* invalid parameter */
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_Seek");
        return XA_RESULT_PARAMETER_INVALID;
    }

    ret = XARadioItfAdapt_Seek( (XAAdaptationMMFCtx*)impl->adapCtx, upwards );

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_Seek");
    return ret;
}

/**
 * XAresult XARadioItfImpl_StopSeeking(XARadioItf self)
 * Description: Cancels an outstanding seek request. The method blocks while canceling
 *              the outstanding request. After cancellation, the frequency is the one
 *              where seeking stopped. Has not effect if no seek operation is ongoing.
 **/
XAresult XARadioItfImpl_StopSeeking(XARadioItf self)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_StopSeeking");
    XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);

    if(!impl)
    {
        /* invalid parameter */
        XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARadioItfImpl_StopSeeking");
        return XA_RESULT_PARAMETER_INVALID;
    }

    XARadioItfAdapt_StopSeeking( (XAAdaptationMMFCtx*)impl->adapCtx );

    XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_StopSeeking");
    return ret;
}

/**
 * XAresult XARadioItfImpl_GetNumberOfPresets(XARadioItf self, XAuint32 * pNumPresets)
 * Description: Returns the number of preset slots the device has for storing the presets.
 **/
XAresult XARadioItfImpl_GetNumberOfPresets(XARadioItf self, XAuint32* pNumPresets)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint32 presetValue = 0;
    *pNumPresets = presetValue;
    DEBUG_API("->XARadioItfImpl_GetNumberOfPresets");

    return ret;
}

/**
 * XAresult XARadioItfImpl_SetPreset(XARadioItf self,
 *                                   XAuint32 preset,
 *                                   XAuint32 freq,
 *                                   XAuint8 range,
 *                                   XAuint32 mode,
 *                                   const XAchar * name)
 * Description: Sets the preset.
 **/
XAresult XARadioItfImpl_SetPreset(XARadioItf self,
                                  XAuint32 preset,
                                  XAuint32 freq,
                                  XAuint8 range,
                                  XAuint32 mode,
                                  const XAchar * name)
{
  //  XAresult ret = XA_RESULT_FEATURE_UNSUPPORTED; Currently, this is not a valid error code for OpenMaxAL. Is proposed to be added.
  
    XAresult ret = XA_RESULT_PARAMETER_INVALID; // For now, use this error code.

    return ret;
}

/**
 * XAresult XARadioItfImpl_GetPreset(XARadioItf self,
 *                                   XAuint32 preset,
 *                                   XAuint32 * pFreq,
 *                                   XAuint8 * pRange,
 *                                   XAuint32 * pMode,
 *                                   XAchar * pName,
 *                                   XAuint16 * pNameLength)
 * Description: Gets the settings stored into a preset.
 **/
XAresult XARadioItfImpl_GetPreset(XARadioItf self,
                                  XAuint32 preset,
                                  XAuint32 * pFreq,
                                  XAuint8 * pRange,
                                  XAuint32 * pMode,
                                  XAchar * pName,
                                  XAuint16 * pNameLength)
{
 
    XAresult ret = XA_RESULT_SUCCESS;
    if (preset != 0)
        ret = XA_RESULT_PARAMETER_INVALID;
        
    DEBUG_API("<-XARadioItfImpl_GetPreset");
    return ret;
}

/**
 * XAresult XARadioItfImpl_RegisterRadioCallback(XARadioItf self,
 *                                               xaRadioCallback callback,
 *                                               void * pContext)
 * Description: Sets or clears the xaRadioCallback.
 **/
XAresult XARadioItfImpl_RegisterRadioCallback(XARadioItf self,
                                              xaRadioCallback callback,
                                              void * pContext)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioItfImpl* impl = GetImpl(self);
    DEBUG_API("->XARadioItfImpl_RegisterRadioCallback");

    if(!impl)
    {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        /* invalid parameter */
        DEBUG_API("<-XARadioItfImpl_RegisterRadioCallback");
        return XA_RESULT_PARAMETER_INVALID;
    }

    impl->callback = callback;
    impl->context = pContext;
    impl->cbPtrToSelf = self;

    DEBUG_API("<-XARadioItfImpl_RegisterRadioCallback");
    return ret;
}

/**
 * XARadioItfImpl -specific methods
 **/

/**
 * XARadioItfImplImpl* XARadioItfImpl_Create()
 * Description: Allocate and initialize RadioItfImpl
 **/
XARadioItfImpl* XARadioItfImpl_Create(XAAdaptationBaseCtx *adapCtx)
{

    XARadioItfImpl *self = (XARadioItfImpl*)
        calloc(1,sizeof(XARadioItfImpl));

    DEBUG_API("->XARadioItfImpl_Create");

    if(self)
    {
        /* init itf default implementation */
        self->itf.SetFreqRange = XARadioItfImpl_SetFreqRange;
        self->itf.GetFreqRange = XARadioItfImpl_GetFreqRange;
        self->itf.IsFreqRangeSupported = XARadioItfImpl_IsFreqRangeSupported;
        self->itf.GetFreqRangeProperties = XARadioItfImpl_GetFreqRangeProperties;
        self->itf.SetFrequency = XARadioItfImpl_SetFrequency;
        self->itf.CancelSetFrequency = XARadioItfImpl_CancelSetFrequency;
        self->itf.GetFrequency = XARadioItfImpl_GetFrequency;
        self->itf.SetSquelch = XARadioItfImpl_SetSquelch;
        self->itf.GetSquelch = XARadioItfImpl_GetSquelch;
        self->itf.SetStereoMode = XARadioItfImpl_SetStereoMode;
        self->itf.GetStereoMode = XARadioItfImpl_GetStereoMode;
        self->itf.GetSignalStrength = XARadioItfImpl_GetSignalStrength;
        self->itf.Seek = XARadioItfImpl_Seek;
        self->itf.StopSeeking = XARadioItfImpl_StopSeeking;
        self->itf.GetNumberOfPresets = XARadioItfImpl_GetNumberOfPresets;
        self->itf.SetPreset = XARadioItfImpl_SetPreset;
        self->itf.GetPreset = XARadioItfImpl_GetPreset;
        self->itf.RegisterRadioCallback = XARadioItfImpl_RegisterRadioCallback;

        /* init variables */

        self->squelch = XA_BOOLEAN_FALSE;
        self->stereoMode = XA_STEREOMODE_STEREO;
        self->callback = NULL;
        self->context = NULL;
        self->cbPtrToSelf = NULL;
        self->adapCtx = adapCtx;

        XAAdaptationBase_AddEventHandler( adapCtx, &XARadioItfImpl_AdaptCb, XA_RADIOITFEVENTS, self );

        self->self = self;
    }

    DEBUG_API("<-XARadioItfImpl_Create");
    return self;
}

/**
 * void XARadioItfImpl_Free(XARadioItfImpl* self)
 * Description: Free all resources reserved at XARadioItfImpl_Create
 **/
void XARadioItfImpl_Free(XARadioItfImpl* self)
{
    DEBUG_API("->XARadioItfImpl_Free");
    XA_IMPL_THREAD_SAFETY_ENTRY_FOR_VOID_FUNCTIONS(XATSRadio);
    XAAdaptationBase_RemoveEventHandler( self->adapCtx, &XARadioItfImpl_AdaptCb );

    XARadioItfAdapt_Free();
    free(self);

    XA_IMPL_THREAD_SAFETY_EXIT_FOR_VOID_FUNCTIONS(XATSRadio);
    DEBUG_API("<-XARadioItfImpl_Free");
}

/* void XARadioItfimpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event )
 * Description: Event handler for adaptation events
 */
void XARadioItfImpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event )
{
    XARadioItfImpl* impl =(XARadioItfImpl*)pHandlerCtx;
    XAuint32 eventData = 0;
    XAboolean eventBoolean = XA_BOOLEAN_FALSE;
    
    DEBUG_API("->XARadioItfimpl_AdaptCb");

    if(!impl)
    {
        DEBUG_ERR("XARadioItfImpl_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XARadioItfImpl_AdaptCb");
        return;
    }
    
    if (event)
    {
        if( event->eventid == XA_ADAPT_RADIO_FREQUENCY_CHANGED && impl->callback )
        {
            DEBUG_API("Frequency changed in adaptation"); 
            eventData = *(XAuint32*)event->data;
            impl->callback( impl->cbPtrToSelf, impl->context, XA_RADIO_EVENT_FREQUENCY_CHANGED, eventData, eventBoolean );
        }
        else if( event->eventid == XA_ADAPT_RADIO_FREQUENCY_RANGE_CHANGED && impl->callback )
        {
            DEBUG_API("Frequency range changed in adaptation");
            impl->callback( impl->cbPtrToSelf, impl->context, XA_RADIO_EVENT_FREQUENCY_RANGE_CHANGED, eventData, eventBoolean  );
        }
        else if( event->eventid == XA_ADAPT_RADIO_SEEK_COMPLETE && impl->callback )
        {
            DEBUG_API("Seek complete in adaptation");
       	    eventBoolean = *(XAboolean*)event->data;        
            impl->callback( impl->cbPtrToSelf, impl->context, XA_RADIO_EVENT_SEEK_COMPLETED, eventData, eventBoolean  );     
        }
        else if( event->eventid == XA_ADAPT_RADIO_STEREO_STATUS_CHANGED && impl->callback )
        {
            DEBUG_API("Stereo status change in adaptation");
            eventBoolean = *(XAboolean*)event->data;          
            impl->callback( impl->cbPtrToSelf, impl->context, XA_RADIO_EVENT_STEREO_STATUS_CHANGED, eventData, eventBoolean  );
        } 
        else if( event->eventid == XA_ADAPT_RADIO_SIGNAL_STRENGTH_CHANGED && impl->callback )
        {
            DEBUG_API("Signal Strength Change in adaptation");
            impl->callback( impl->cbPtrToSelf, impl->context, XA_RADIO_EVENT_SIGNAL_STRENGTH_CHANGED, eventData, eventBoolean  );
        }
    }      
    else
    {
        /* do nothing */
    }
    DEBUG_API("<-XARadioItfimpl_AdaptCb");
}

