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

#include <unistd.h>
#include "xadebug.h"
#include "xaadaptationmmf.h"
#include "xaradioitfadaptation.h"
#include "xaradioadaptctx.h"
#include "cmmfradiobackendengine.h"


/*
 * XAresult XARadioItfAdapt_SetFreqRange(XAAdaptationMMFCtx *bCtx, XAuint8 range)
 */
XAresult XARadioItfAdapt_SetFreqRange(XAAdaptationMMFCtx *bCtx, XAuint8 range)
{
    XAresult ret = XA_RESULT_SUCCESS;  
    DEBUG_API("->XARadioItfAdapt_SetFreqRange");
    set_freq_range(cmmfradiobackendengine_init(), bCtx, range);
    DEBUG_API("<-XARadioItfAdapt_SetFreqRange");
    return ret;
}

/*
 * XAresult XARadioItfAdapt_GetFreqRange(XAuint8 * pFreqRange)
 *
 */
XAresult XARadioItfAdapt_GetFreqRange(XAuint8 * pFreqRange)
{
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_GetFreqRange");
    res = get_freq_range(cmmfradiobackendengine_init(), pFreqRange);
    DEBUG_API("<-XARadioItfAdapt_GetFreqRange");
    return res;
}

/*
 *  XAresult XARadioItfAdapt_IsFreqRangeSupported(XAuint8 range,
 *                                                XAboolean * pSupported)
 */
XAresult XARadioItfAdapt_IsFreqRangeSupported(XAuint8 range,
                                             XAboolean * pSupported)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_IsFreqRangeSupported");
    if ((range == XA_FREQRANGE_FMEUROAMERICA) || (range == XA_FREQRANGE_FMJAPAN)) 
    {
        *pSupported = XA_BOOLEAN_TRUE;
    }
    else
    {
        *pSupported = XA_BOOLEAN_FALSE;
    }
    DEBUG_API("<-XARadioItfAdapt_IsFreqRangeSupported");
    return ret;
}

/*
 * XAresult XARadioItfAdapt_GetFreqRangeProperties(XAAdaptationMMFCtx *bCtx,
 *                                                 XAuint8 range,
 *                                                 XAuint32 * pMinFreq,
 *                                                 XAuint32 * pMaxFreq)
 */
XAresult XARadioItfAdapt_GetFreqRangeProperties(XAAdaptationMMFCtx *bCtx,
                                               XAuint8  range,
                                               XAuint32 * pMinFreq,
                                               XAuint32 * pMaxFreq)
{
    XAresult res = XA_RESULT_SUCCESS;    
    DEBUG_API("->XARadioItfAdapt_GetFreqRangeProperties");
    res = get_freq_range_properties(cmmfradiobackendengine_init(), range, pMinFreq, pMaxFreq);	
    DEBUG_API("<-XARadioItfAdapt_GetFreqRangeProperties");
    return res;
}

/*
 * XAresult XARadioItfAdapt_SetFrequency(XAAdaptationMMFCtx *bCtx, XAuint32 freq)
 */
XAresult XARadioItfAdapt_SetFrequency(XAAdaptationMMFCtx *bCtx, XAuint32 freq)
{
    XAresult ret = XA_RESULT_SUCCESS; 
    DEBUG_API("->XARadioItfAdapt_SetFrequency");
    set_frequency(cmmfradiobackendengine_init(), bCtx, freq);
    DEBUG_API("<-XARadioItfAdapt_SetFrequency");
    return ret;
}


/*
 * XAresult XARadioItfAdapt_GetFrequency(XAAdaptationMMFCtx *bCtx,
 *                                       XAuint32* pFrequency)
 */
XAresult XARadioItfAdapt_GetFrequency(XAuint32* pFrequency)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_GetFrequency");
    ret = get_frequency(cmmfradiobackendengine_init(), pFrequency);
    DEBUG_API("<-XARadioItfAdapt_GetFrequency");
    return ret;
}


/*
 * XAresult XARadioItfAdapt_CancelSetFrequency()
 */
XAresult XARadioItfAdapt_CancelSetFrequency()
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_CancelSetFrequency");
    cancel_set_frequency(cmmfradiobackendengine_init());
    DEBUG_API("<-XARadioItfAdapt_CancelSetFrequency");
    return ret;
}

/*
 * XAresult XARadioItfAdapt_SetSquelch(XAboolean squelch)
 */
XAresult XARadioItfAdapt_SetSquelch(XAAdaptationMMFCtx *bCtx, XAboolean squelch)
{
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_SetSquelch");
    set_squelch(cmmfradiobackendengine_init(), bCtx, squelch);
    DEBUG_API("->XARadioItfAdapt_SetSquelch");
    return res;
}

/*
 * XAresult XARadioItfAdapt_GetSquelch(XAboolean *squelch)
 */
XAresult XARadioItfAdapt_GetSquelch(XAboolean* pSquelch)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_GetSquelch");
    get_squelch(cmmfradiobackendengine_init(), pSquelch);
    DEBUG_API("->XARadioItfAdapt_GetSquelch");
    return ret;
}

/*
 * XAresult XARadioItfAdapt_SetStereoMode(XAAdaptationMMFCtx *bCtx, XAuint32 mode)
 */
XAresult XARadioItfAdapt_SetStereoMode(XAAdaptationMMFCtx *bCtx, XAuint32 mode)
{
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_SetStereoMode");
    res = set_stereo_mode(cmmfradiobackendengine_init(), bCtx, mode);
    DEBUG_API("<-XARadioItfAdapt_SetStereoMode");
    return res;
}

/*
 * XAresult XARadioItfAdapt_GetSignalStrength(XAuint32 * pStrength)
 */
XAresult XARadioItfAdapt_GetSignalStrength(XAuint32 * pStrength)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_GetSignalStrength");
    ret = get_signal_strength(cmmfradiobackendengine_init(), pStrength);
    DEBUG_API("<-XARadioItfAdapt_GetSignalStrength");
    return ret;
}

/*
 * XAresult XARadioItfAdapt_Seek(XAAdaptationMMFCtx *bCtx, XAboolean upwards)
 */
XAresult XARadioItfAdapt_Seek(XAAdaptationMMFCtx *bCtx, XAboolean upwards)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_Seek");
    station_seek(cmmfradiobackendengine_init(), bCtx, upwards);
    DEBUG_API("<-XARadioItfAdapt_Seek");
    return ret;
}

/*
 * void XARadioItfAdapt_StopSeeking(XAAdaptationMMFCtx *bCtx)
 */
void XARadioItfAdapt_StopSeeking(XAAdaptationMMFCtx *bCtx)
{
    DEBUG_API("->XARadioItfAdapt_StopSeeking");	
    cancel_station_seek(cmmfradiobackendengine_init());
    DEBUG_API("<-XARadioItfAdapt_StopSeeking");
}

/*
 * XAresult XARadioItfAdapt_GetStereoMode(XAuint32 * pMode)
 */
XAresult XARadioItfAdapt_GetStereoMode(XAuint32 * pMode)
{
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XARadioItfAdapt_GetStereoMode");
    ret = get_stereo_mode(cmmfradiobackendengine_init(), pMode);
    DEBUG_API("<-XARadioItfAdapt_GetStereoMode");
    return ret;
}

/*
 * void XARadioItfAdapt_Free()
 */
void XARadioItfAdapt_Free()
{
}

void XARadioItfAdapt_SeekComplete(XAAdaptationBaseCtx *ctx, TInt aError, TInt aFrequency)
{
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_SEEK_COMPLETE,1,0};
    if (ctx)
    {
        if (aError)
        {
            event.data = &aError;
        }
        else
        {
            event.data = &aFrequency;
        }
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_FrequencyChange(XAAdaptationBaseCtx *ctx, TInt aFrequency)
{
    // Currently: Bug in FM Radio Utility: SetFrequency calls this callback as well as SetFrequencyComplete
    // (So client will end up getting 2 events on a SetFrequency call)
    XAuint32 freq = aFrequency;    		
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_FREQUENCY_CHANGED,1,0};
    if (ctx)
    {
        event.data = &freq;
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_FrequencyRangeChange(XAAdaptationBaseCtx* ctx, TInt aFreqRangeChange)
{
    // Currently: Bug in FM Radio Utility: SetFrequencyRange triggers this callback as well as SetFrequencyRangeComplete
   /*	XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_FREQUENCY_RANGE_CHANGED,1,0};        	
		if (baseCtx)
    {
    	event.data = &aFreqRangeChange;
     	XAAdaptationBase_SendAdaptEvents(&baseCtx->baseObj, &event );
   	} 
   	*/
}

void XARadioItfAdapt_SetFrequencyComplete(XAAdaptationBaseCtx *ctx, TInt aError, TInt aFrequency)
{
    // Currently: Qt API has no event for SetFrequencyComplete, so must use this one:
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_FREQUENCY_CHANGED,1,0};   
    XAuint32 freq = aFrequency;
    if (ctx)
    {
        if (!aError)
        {
            event.data = &freq;
        }
        else
        {
            event.data = &aError;
        }
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_SetFrequencyRangeComplete(XAAdaptationBaseCtx *ctx, TInt aError)
{
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_FREQUENCY_RANGE_CHANGED,1,0};
    if (ctx)
    {
        event.data = &aError;
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_StereoStatusChange(XAAdaptationBaseCtx *ctx, XAboolean aStereoStatus)
{
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_STEREO_STATUS_CHANGED,1,0}; 
    DEBUG_INFO_A1("XARadioItfAdapt_StereoStatusChange to: %s", aStereoStatus);
    if (ctx)
    {
        event.data = &aStereoStatus;
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_SignalStrengthChange(XAAdaptationBaseCtx *ctx, TInt aSignalStrength)
{
    XAAdaptEvent event = {XA_RADIOITFEVENTS, XA_ADAPT_RADIO_SIGNAL_STRENGTH_CHANGED,1,0};
    DEBUG_INFO_A1("XARadioItfAdapt_SignalStrengthChange to: %s", aSignalStrength);
    if (ctx)
    {
        event.data = &aSignalStrength;
        XAAdaptationBase_SendAdaptEvents(ctx, &event );
    }
}

void XARadioItfAdapt_StateChange(XAAdaptationBaseCtx *ctx, XAboolean aState)
{
    if (aState) // playing
    {
        XAAdaptEvent event = {XA_PLAYITFEVENTS, XA_PLAYEVENT_HEADMOVING, 1, 0};
        XAAdaptationBase_SendAdaptEvents(ctx, &event);
    }
    else  // idle
    {
        XAAdaptEvent event = {XA_PLAYITFEVENTS, XA_PLAYEVENT_HEADSTALLED, 1, 0};
        XAAdaptationBase_SendAdaptEvents(ctx, &event);
    }
}

void XARadioItfAdapt_VolumeChange(XAAdaptationBaseCtx *ctx, TInt aVolume)
    {
    XAAdaptEvent event =
        {
        XA_NOKIALINEARVOLITFEVENTS, XA_ADAPT_VOLUME_VOLUME_CHANGED, 1, 0
        };
    if (ctx)
        {
        event.data = &aVolume;
        XAAdaptationBase_SendAdaptEvents(ctx, &event);
        }
    }
    

void XARadioItfAdapt_MuteChange(XAAdaptationBaseCtx* ctx, XAboolean aMute)
    {
    XAAdaptEvent event =
        {
        XA_NOKIAEXTVOLITFEVENTS, XA_ADAPT_VOLUME_MUTE_CHANGED, 1, 0
        };
    if (ctx)
        {
        event.data = &aMute;
        XAAdaptationBase_SendAdaptEvents(ctx, &event);
        }
    }
