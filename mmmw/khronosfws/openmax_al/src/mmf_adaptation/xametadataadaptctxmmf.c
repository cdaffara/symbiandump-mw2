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
 * Description: Metadata Adaptation MMF
 *
 */

#include <stdlib.h>
#include "xametadataadaptctxmmf.h"
#include "xaobjectitf.h"
#include "cmetadatautilityitf.h"

XAAdaptationBaseCtx* XAMetadataAdaptCtxMMF_Create(XADataSource * pDataSrc)
    {

    XAMetadataAdaptationMMFCtx *pSelf = NULL;
    DEBUG_API("->XAMetadataAdaptCtxMMF_Create");

    pSelf = calloc(1, sizeof(XAMetadataAdaptationMMFCtx));
    if(!pSelf)
        {
        /* memory allocation failed */
        DEBUG_ERR("Failed to allocate memory");
        DEBUG_API("<-XAMetadataAdaptCtxMMF_Create");
        return NULL;
        }

    if (XAAdaptationBaseMMF_Init(&(pSelf->baseObj), XAMDAdaptation)
            != XA_RESULT_SUCCESS)
        {
        free(pSelf);
        DEBUG_ERR("Failed to init base context!!!");
        DEBUG_API("<-XAMetadataAdaptCtxMMF_Create");
        return NULL;
        }

    pSelf->baseObj.baseObj.fwtype = FWMgrFWMMF;
    pSelf->xaSource = pDataSrc;

    if (pDataSrc)
        {
        pSelf->mmfContext
                = mmf_metadata_utility_init(
                        (char *) (((XADataLocator_URI*) (pDataSrc->pLocator))->URI));
        if (!pSelf->mmfContext)
            {
            free(pSelf);
            DEBUG_ERR("Failed to init mmf context!!!");
            DEBUG_API("<-XAMetadataAdaptCtxMMF_Create");
            return NULL;
            }
        }

    DEBUG_API("<- XAMetadataAdaptCtxMMF_Create");
    return (XAAdaptationBaseCtx*) (&pSelf->baseObj.baseObj);
    }

/*
 * XAresult XAMediaPlayerAdaptMMF_PostInit()
 * 2nd phase initialization of Media Player Adaptation Context
 * @param XAMediaPlayerAdaptationMMFCtx* ctx - pointer to Media Player adaptation context
 * @return XAresult - Success value
 */
XAresult XAMetadataAdaptCtxMMF_PostInit(XAAdaptationMMFCtx* bCtx)
    {
    XAresult ret = XA_RESULT_SUCCESS;

    DEBUG_API("<-XAMetadataAdaptCtxMMF_PostInit");
    return ret;
    }

/*
 * void XAMediaPlayerAdaptMMF_Destroy( XAMediaPlayerAdaptationMMFCtx* ctx )
 * Destroys Media Player Adaptation Context
 * @param ctx - Media Player Adaptation context to be destroyed
 */
void XAMetadataAdaptCtxMMF_Destroy(XAAdaptationMMFCtx* bCtx)
    {
    DEBUG_API("->XAMetadataAdaptCtxMMF_Destroy");
    if (bCtx == NULL)
        {
        DEBUG_ERR("Invalid parameter!!");
        DEBUG_API("<-XAMetadataAdaptCtxMMF_Destroy");
        return;
        }

    if (((XAMetadataAdaptationMMFCtx*) bCtx)->mmfContext)
        {
        mmf_metadata_utility_destroy(
                ((XAMetadataAdaptationMMFCtx*) bCtx)->mmfContext);
        }

    XAAdaptationBase_Free(&bCtx->baseObj);
    free(bCtx);
    bCtx = NULL;

    DEBUG_API("<-XAMetadataExtractorAdaptMMF_Destroy");
    }

