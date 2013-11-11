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

#include "xaradioadaptctx.h"
#include "xaadaptationmmf.h"
#include "cmmfradiobackendengine.h"

/*
 * XAAdaptationMMFCtx* XARadioAdapt_Create()
 * Allocates memory for Radio Adaptation Context and makes 1st phase initialization
 * @returns XARadioAdaptationCtx* - Pointer to created context
 */
XAAdaptationBaseCtx* XARadioAdapt_Create()
{
    XARadioAdaptationCtx *pSelf = (XARadioAdaptationCtx*)calloc(1, sizeof(XARadioAdaptationCtx));
    DEBUG_API("->XARadioAdapt_Create");

    if ( pSelf)
    {
        if( XAAdaptationBase_Init(&(pSelf->baseObj.baseObj),XARadioAdaptation) != XA_RESULT_SUCCESS )
        {
            DEBUG_ERR("Failed to init base context!!!");
            free(pSelf);
            pSelf = NULL;
            return (XAAdaptationBaseCtx*)pSelf;
        }
        else
        {
            pSelf->range = RADIO_DEFAULT_FREQ_RANGE;
            pSelf->frequency = RADIO_DEFAULT_FREQ;
            pSelf->rdsEmulationThread = 0;
            pSelf->emulationThread = 0;
        }
    }

    DEBUG_API("<-XARadioAdapt_Create");
    return (XAAdaptationBaseCtx*)&pSelf->baseObj;
}

/*
 * XAresult XARadioAdapt_PostInit()
 * 2nd phase initialization of Radio Adaptation Context
 */
XAresult XARadioAdapt_PostInit(XAAdaptationBaseCtx* bCtx)
{
    XAresult ret = XA_RESULT_SUCCESS;
    XARadioAdaptationCtx* ctx = NULL;
    DEBUG_API("->XARadioAdapt_PostInit");
    if(bCtx == NULL || bCtx->ctxId != XARadioAdaptation )
    {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XARadioAdapt_PostInit");
        return XA_RESULT_PARAMETER_INVALID;
    }
    ctx = (XARadioAdaptationCtx*)bCtx;
    ret = XAAdaptationBase_PostInit( &ctx->baseObj.baseObj );
    if( ret!=XA_RESULT_SUCCESS )
    {
        DEBUG_ERR("Base context postinit failed!!");
        DEBUG_API("<-XARadioAdapt_PostInit");
        return ret;
    }

    if (cmmfradiobackendengine_init() == NULL)
    {
        DEBUG_ERR("Creation of Radio Backend Engine failed!!");
        DEBUG_API("<-XARadioAdapt_PostInit");
        return XA_RESULT_RESOURCE_ERROR;
    }

    DEBUG_API("<-XARadioAdapt_PostInit");
    return ret;
}

/*
 * void XARadioAdapt_Destroy(XAAdaptationMMFCtx* bCtx)
 * Destroys Radio Adaptation Context
 * @param ctx - Radio Adaptation context to be destroyed
 */
void XARadioAdapt_Destroy(XAAdaptationBaseCtx* bCtx)
{
    XARadioAdaptationCtx* ctx = NULL;
    DEBUG_API("->XARadioAdapt_Destroy");

    cmmfradiobackendengine_delete(cmmfradiobackendengine_init());

    if(bCtx == NULL || bCtx->ctxId != XARadioAdaptation )
    {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XARadioAdapt_Destroy");
        return;
    }
    ctx = (XARadioAdaptationCtx*)bCtx;
    XAAdaptationBase_Free(&(ctx->baseObj.baseObj));

    free(ctx);

    DEBUG_API("<-XARadioAdapt_Destroy");
}
