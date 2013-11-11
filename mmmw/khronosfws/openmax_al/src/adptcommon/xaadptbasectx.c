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
 * Description: Base Adaptation Initialization Functionality
 *
 */

#include <string.h>
#include "xaadptbasectx.h"
#include "xathreadsafety.h"

/*
 * XAAdaptationBaseCtx* XAAdaptationBase_Create()
 * 1st phase initialization function for Adaptation Base context structure.
 * Reserves memory for base context and initializes GStreamer FW.
 */
XAresult XAAdaptationBase_Init(XAAdaptationBaseCtx* pSelf, XAuint32 ctxId)
    {
    DEBUG_API("->XAAdaptationBase_Init");

    if (pSelf)
        {
        if (!pSelf->evtHdlrs)
            {
            pSelf->evtHdlrs = g_array_new(FALSE, FALSE,
                    sizeof(XAAdaptEvtHdlr));
            if (!pSelf->evtHdlrs)
                {
                DEBUG_ERR("Memory allocation failure in Base Ctx.");
                return XA_RESULT_MEMORY_FAILURE;
                }
            }
        pSelf->ctxId = ctxId;
        }
    else
        {
        DEBUG_ERR("Invalid Adaptation Base Context.");
        return XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API("<-XAAdaptationBase_Init");
    return XA_RESULT_SUCCESS;
    }

/*
 * XAresult XAAdaptationBase_PostInit()
 * 2nd phase initialization for Adaptation Base.
 */
XAresult XAAdaptationBase_PostInit(XAAdaptationBaseCtx* ctx)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAdaptationBase_PostInit");
    DEBUG_API("<-XAAdaptationBase_PostInit");
    return ret;
    }

/*
 * void XAAdaptationBase_Free( XAAdaptationBaseCtx* ctx )
 * Frees all Base context variables .
 */
void XAAdaptationBase_Free(XAAdaptationBaseCtx* ctx)
    {
    g_array_free(ctx->evtHdlrs, TRUE);
    ctx->evtHdlrs = NULL;
    DEBUG_API("<-XAAdaptationBase_Free");
    }

/*
 * XAresult XAAdaptationBase_AddEventHandler
 * Adds event handler for certain event types.
 */
XAresult XAAdaptationBase_AddEventHandler(XAAdaptationBaseCtx* ctx,
        xaAdaptEventHandler evtHandler, XAuint32 evtTypes, void *pHandlerCtx)
    {
    XAuint32 i;
    XAAdaptEvtHdlr tmp;
    DEBUG_API("->XAAdaptationBase_AddEventHandler");
    if (!ctx)
        {
        DEBUG_ERR("no context");
        return XA_RESULT_PARAMETER_INVALID;
        }

    for (i = 0; i < ctx->evtHdlrs->len; i++)
        {
        if ((g_array_index(ctx->evtHdlrs, XAAdaptEvtHdlr, i)).handlerfunc
                == evtHandler)
            {
            return XA_RESULT_PARAMETER_INVALID;
            }
        }
    tmp.handlerfunc = evtHandler;
    tmp.handlercontext = pHandlerCtx;
    tmp.eventtypes = evtTypes;
    g_array_append_val(ctx->evtHdlrs, tmp);
    DEBUG_API("<-XAAdaptationBase_AddEventHandler");
    return XA_RESULT_SUCCESS;
    }

/*
 * XAresult XAAdaptationBase_RemoveEventHandler
 * Removes event handler for certain event types.
 */
XAresult XAAdaptationBase_RemoveEventHandler(XAAdaptationBaseCtx* ctx,
        xaAdaptEventHandler evtHandler)
    {
    XAuint32 i;
    DEBUG_API("->XAAdaptationBase_RemoveEventHandler");
    if (!ctx)
        {
        DEBUG_ERR("no context");
        return XA_RESULT_PARAMETER_INVALID;
        }
    for (i = 0; i < ctx->evtHdlrs->len; i++)
        {
        if ((g_array_index(ctx->evtHdlrs, XAAdaptEvtHdlr, i)).handlerfunc
                == evtHandler)
            {
            g_array_remove_index(ctx->evtHdlrs, i);
            return XA_RESULT_SUCCESS;
            }
        }
    DEBUG_API("<-XAAdaptationBase_RemoveEventHandler");
    /*did not find, return error*/
    return XA_RESULT_PARAMETER_INVALID;
    }

void XAAdaptationBase_SendAdaptEvents(XAAdaptationBaseCtx* ctx,
        XAAdaptEvent* event)
    {
    XAuint32 i;
    XAAdaptEvtHdlr* tmp;
    for (i = 0; i < ctx->evtHdlrs->len; i++)
        {
        tmp = &g_array_index(ctx->evtHdlrs, XAAdaptEvtHdlr, i);
        if (tmp->eventtypes & event->eventtype)
            {
            (tmp->handlerfunc)(tmp->handlercontext, event);
            }
        }
    }

/*
 * XAresult XAEqualizerItfAdapt_ThreadEntry(XAAdaptationGstCtx *adapCtx)
 * Description: Safety thread entry
 */
XAresult XAAdaptationBase_ThreadEntry(XAAdaptationBaseCtx *adaptCtx)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAdaptationBase_ThreadEntry");
    if (!adaptCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAAdaptationBase_ThreadEntry");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    switch (adaptCtx->ctxId)
        {
        case XAMediaPlayerAdaptation:
            XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaPlayer);
            break;
        case XAMediaRecorderAdaptation:
            XA_IMPL_THREAD_SAFETY_ENTRY(XATSMediaRecorder);
            break;
        case XARadioAdaptation:
            XA_IMPL_THREAD_SAFETY_ENTRY(XATSRadio);
            break;
        case XAMDAdaptation:
            XA_IMPL_THREAD_SAFETY_ENTRY(XATSMetaDataExtractor);
            break;
        case XAEngineAdaptation:
            XA_IMPL_THREAD_SAFETY_ENTRY(XATSEngine);
            break;
        default:
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            /* invalid parameter */
            res = XA_RESULT_PARAMETER_INVALID;
            break;
        }

    DEBUG_API("<-XAAdaptationBase_ThreadEntry");
    return res;
    }
/*
 * XAresult XAEqualizerItfAdapt_ThreadExit(XAAdaptationGstCtx *adaptCtx)
 * Description: Safety thread exit
 */
XAresult XAAdaptationBase_ThreadExit(XAAdaptationBaseCtx *adaptCtx)
    {
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAdaptationBase_ThreadExit");
    if (!adaptCtx)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAAdaptationBase_ThreadExit");
        /* invalid parameter */
        return XA_RESULT_PARAMETER_INVALID;
        }

    switch (adaptCtx->ctxId)
        {
        case XAMediaPlayerAdaptation:
            XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaPlayer);
            break;
        case XAMediaRecorderAdaptation:
            XA_IMPL_THREAD_SAFETY_EXIT(XATSMediaRecorder);
            break;
        case XARadioAdaptation:
            XA_IMPL_THREAD_SAFETY_EXIT(XATSRadio);
            break;
        case XAMDAdaptation:
            XA_IMPL_THREAD_SAFETY_EXIT(XATSMetaDataExtractor);
            break;
        case XAEngineAdaptation:
            XA_IMPL_THREAD_SAFETY_EXIT(XATSEngine);
            break;
        default:
            DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
            /* invalid parameter */
            res = XA_RESULT_PARAMETER_INVALID;
            break;
        }

    DEBUG_API("<-XAAdaptationBase_ThreadExit");
    return res;
    }
