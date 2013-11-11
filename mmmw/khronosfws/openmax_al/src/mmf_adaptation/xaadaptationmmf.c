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
 * Description: MMF Adaptation Context 
 *
 */

#include <string.h>
#include "xaadaptationmmf.h"
#include "xaadaptationmmf.h"
#include "xaobjectitf.h"
#include "xamediaplayeradaptctxmmf.h"

/*
 * XAAdaptationBaseCtx* XAAdaptationBase_Create()
 * 1st phase initialization function for Adaptation Base context structure.
 * Reserves memory for base context and initializes GStreamer FW.
 */
XAresult XAAdaptationBaseMMF_Init(XAAdaptationMMFCtx* pSelf, XAuint32 ctxId)
    {
    DEBUG_API("->XAAdaptationBase_Init");

    if (pSelf)
        {
        if (XAAdaptationBase_Init(&(pSelf->baseObj), ctxId)
                != XA_RESULT_SUCCESS)
            {
            DEBUG_ERR("Failed to init base context!!!");
            free(pSelf);
            pSelf = NULL;
            DEBUG_API("<-XAAdaptationBase_Init");
            return XA_RESULT_MEMORY_FAILURE;
            }
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
XAresult XAAdaptationBaseMMF_PostInit(XAAdaptationMMFCtx* ctx)
    {
    XAresult ret = XA_RESULT_SUCCESS;
    DEBUG_API("->XAAdaptationBase_PostInit");
    if(ctx)
        {
        ret = XAAdaptationBase_PostInit(&ctx->baseObj);
        }
    else
        {
        ret = XA_RESULT_PARAMETER_INVALID;
        }
    DEBUG_API("<-XAAdaptationBase_PostInit");
    return ret;
    }

/*
 * void XAAdaptationBase_Free( XAAdaptationBaseCtx* ctx )
 * Frees all Base context variables .
 */
void XAAdaptationBaseMMF_Free(XAAdaptationMMFCtx* ctx)
    {
    DEBUG_API("->XAAdaptationBaseMMF_Free");
    if(ctx)
        {
        XAAdaptationBase_Free(&ctx->baseObj);
        }
    DEBUG_API("<-XAAdaptationBaseMMF_Free");
    }

