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

#include "xamediarecorderadaptctxmmf.h"
#include "xarecorditfadaptationmmf.h"
#include "cmmfbackendengine.h"
#include "xaadaptationmmf.h"
#include "xaadptbasectx.h"

/*
 * XAresult XARecordItfAdaptMMF_SetRecordState(XAAdaptationMMFCtx *ctx, XAuint32 state)
 * Sets record state to GStreamer.
 * @param XAAdaptationBaseCtx *ctx - Adaptation context
 * XAuint32 state - Record state to be set
 * @return XAresult ret - Success value
 */
XAresult XARecordItfAdaptMMF_SetRecordState(XAAdaptationMMFCtx *bCtx,
        XAuint32 state)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    int mmfretCode;
    XAAdaptEvent stalledevent =
        {
        XA_RECORDITFEVENTS, XA_RECORDEVENT_HEADSTALLED, 0, NULL
        };
    XAMediaRecorderAdaptationMMFCtx* mCtx = NULL;
    DEBUG_API_A1_STR("->XARecordItfAdaptMMF_SetRecordState %s",RECORDSTATENAMEMMF(state));
    if (!bCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfAdaptMMF_SetRecordState");
        return XA_RESULT_PARAMETER_INVALID;
        }

    mCtx = (XAMediaRecorderAdaptationMMFCtx*) bCtx;

    mCtx->isRecord = XA_BOOLEAN_TRUE;

    switch (state)
        {
        case XA_RECORDSTATE_STOPPED:
            {
            mmfretCode = mmf_stop_recording(mCtx->mmfContext, TRUE);
            if (mmfretCode != XA_RESULT_SUCCESS)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            break;
            }
        case XA_RECORDSTATE_PAUSED:
            {
            mmfretCode = mmf_stop_recording(mCtx->mmfContext, FALSE);
            if (mmfretCode != XA_RESULT_SUCCESS)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            else
                {
                XAAdaptationBase_SendAdaptEvents(&bCtx->baseObj,
                        &stalledevent);
                }
            break;
            }
        case XA_RECORDSTATE_RECORDING:
            {
            mmfretCode = mmf_start_recording(mCtx->mmfContext);
            if (mmfretCode != XA_RESULT_SUCCESS)
                {
                ret = XA_RESULT_PARAMETER_INVALID;
                }
            break;
            }
        default:
            DEBUG_ERR("Unhandled state")
            ;
            ret = XA_RESULT_PARAMETER_INVALID;
            break;
        }

    if (ret == XA_RESULT_SUCCESS)
        {
        mCtx->xaRecordState = state;
        }

    DEBUG_API("<-XARecordItfAdaptMMF_SetRecordState %d");
    return ret;
    }

/*
 * XAresult XARecordItfAdapt_GetRecordState(XAAdaptationBaseCtx *bCtx, XAuint32 *state)
 * Description: Return record state
 */
XAresult XARecordItfAdaptMMF_GetRecordState(XAAdaptationMMFCtx *bCtx,
        XAuint32 *state)
    {
    XAMediaRecorderAdaptationMMFCtx* mCtx = NULL;
    DEBUG_API("->XARecordItfAdaptMMF_GetRecordState");

    if (!bCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfAdaptMMF_GetRecordState");
        return XA_RESULT_PARAMETER_INVALID;
        }

    mCtx = (XAMediaRecorderAdaptationMMFCtx*) bCtx;

    *state = mCtx->xaRecordState;

    DEBUG_API("<-XARecordItfAdaptMMF_GetRecordState");
    return XA_RESULT_SUCCESS;
    }

/*
 * XAresult XARecordItfAdapt_GetPosition(XAAdaptationBaseCtx *ctx, AdaptationContextIDS ctxIDs, XAmillisecond *pMsec)
 * @param XAAdaptationBaseCtx *ctx - Adaptation context, this will be casted to correct type regarding to contextID value given as 2nd parameter
 * XAmillisecond *pMsec - Pointer where to store current position in stream.
 * @return XAresult ret - Success value
 */
XAresult XARecordItfAdaptMMF_GetPosition(XAAdaptationMMFCtx *bCtx,
        XAmillisecond *pMsec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAuint64 position;
    XAMediaRecorderAdaptationMMFCtx* mCtx = NULL;
    DEBUG_API("->XARecordItfAdaptMMF_GetPosition");

    if (!bCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfAdaptMMF_GetPosition");
        /* invalid parameter */
        ret = XA_RESULT_PARAMETER_INVALID;
        return ret;
        }
    mCtx = (XAMediaRecorderAdaptationMMFCtx*) bCtx;
    ret = mmf_get_record_position(mCtx->mmfContext, &position);

    *pMsec = (XAmillisecond) position;

    DEBUG_API("<-XARecordItfAdaptMMF_GetPosition");
    return ret;
    }

/*
 * gboolean XARecordItfAdapt_PositionUpdate(gpointer ctx)
 * callback.
 * If position tracking enabled, periodic timer calls this method every XA_ADAPT_PU_INTERVAL msecs
 * @return false to stop periodic calls
 */
gboolean XARecordItfAdaptMMF_PositionUpdate(gpointer ctx, XAuint64 position)
    {
    XAAdaptationMMFCtx *bCtx = (XAAdaptationMMFCtx*) ctx;
    XAMediaRecorderAdaptationMMFCtx* mCtx =
            (XAMediaRecorderAdaptationMMFCtx*) ctx;
    XAAdaptEvent event =
        {
        XA_RECORDITFEVENTS, XA_ADAPT_POSITION_UPDATE_EVT, 1, NULL
        };
    DEBUG_API("->XARecordItfAdapt_PositionUpdate");

    if (mCtx && mCtx->trackpositionenabled)
        {
        XAmillisecond posInMsec = 0;
            {
            XARecordItfAdaptMMF_GetPosition(bCtx, &posInMsec);
            event.data = &posInMsec;
            DEBUG_API_A1("XARecordItfAdapt_PositionUpdate: pos %lu ms", posInMsec);
            XAAdaptationBase_SendAdaptEvents(&bCtx->baseObj, &event);
            }
            DEBUG_API_A1("<-XARecordItfAdapt_PositionUpdate: %d", mCtx->runpositiontimer);
        return (mCtx->runpositiontimer);
        }
    /* return false to stop timer */
    return FALSE;
    }

/*
 * XAresult XARecordItfAdapt_EnablePositionTracking
 * Enable/disable periodic position tracking callbacks
 */
XAresult XARecordItfAdaptMMF_EnablePositionTracking(XAAdaptationMMFCtx *bCtx,
        XAboolean enable)
    {
    XAMediaRecorderAdaptationMMFCtx* mCtx =
            (XAMediaRecorderAdaptationMMFCtx*) bCtx;
    DEBUG_API_A1("->XARecordItfAdapt_EnablePositionTracking (enable: %lu)", enable);

    if (!bCtx || bCtx->baseObj.ctxId != XAMediaRecorderAdaptation)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfAdapt_EnablePositionTracking");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    /* create a timer to track position of playback */
    if (enable && !(mCtx->trackpositionenabled))
        {
        mCtx->trackpositionenabled = XA_BOOLEAN_TRUE;
        //mCtx->positionCb = &XARecordItfAdaptMMF_PositionUpdate;

        }
    else if (!enable && (mCtx->trackpositionenabled))
        {
        mCtx->trackpositionenabled = XA_BOOLEAN_FALSE;
        if (mCtx->runpositiontimer > 0)
            {
            mCtx->runpositiontimer = 0;
            }
        }

    DEBUG_API("<-XARecordItfAdapt_EnablePositionTracking");
    return XA_RESULT_SUCCESS;
    }

XAresult XARecordItfAdaptMMF_SetPositionUpdatePeriod(
        XAAdaptationMMFCtx *bCtx, XAmillisecond pMsec)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    XAMediaRecorderAdaptationMMFCtx* mCtx =
            (XAMediaRecorderAdaptationMMFCtx*) bCtx;
    DEBUG_API_A1("->XARecordItfAdaptMMF_SetPositionUpdatePeriod (pMsec: %lu)", pMsec);

    if (!bCtx || bCtx->baseObj.ctxId != XAMediaRecorderAdaptation)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XARecordItfAdapt_EnablePositionTracking");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    ret = mmf_set_record_position_update_period(mCtx->mmfContext, pMsec);
    return ret;
    }
