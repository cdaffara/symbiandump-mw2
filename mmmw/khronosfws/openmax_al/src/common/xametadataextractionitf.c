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
 * Description: Metadata Extraction Interface Implementation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xametadataextractionitf.h"
#include "xadebug.h"
#include "xaadaptationmmf.h"
#include "xametadataadaptctxmmf.h"
#include "xamediaplayeradaptctxmmf.h"
#include "cmetadatautilityitf.h"

/* XAMetadataExtractionItfImpl* GetImpl(XAMetadataExtractionItf self)
 * Description: Validate interface pointer and cast it to implementation pointer.
 **/
static XAMetadataExtractionItfImpl* GetImpl(XAMetadataExtractionItf self)
    {
    if (self)
        {
        XAMetadataExtractionItfImpl* impl =
                (XAMetadataExtractionItfImpl*) (*self);
        if (impl && (impl == impl->self))
            {
            return impl;
            }
        }
    return NULL;
    }

static void* GetMetadataUtilityContext(XAAdaptationMMFCtx* adaptCtx)
    {
    if (adaptCtx)
        {
        switch (adaptCtx->baseObj.ctxId)
            {
            case XAMediaPlayerAdaptation:
                return ((XAMediaPlayerAdaptationMMFCtx*) adaptCtx)->mmfMetadataContext;
            case XAMDAdaptation:
                return ((XAMetadataAdaptationMMFCtx*) adaptCtx)->mmfContext;
            default:
                break;
            }

        }
    return NULL;
    }
/*****************************************************************************
 * Base interface XAMetadataExtractionItf implementation
 *****************************************************************************/

/*
 * Returns the number of metadata items within the current scope of the object.
 * @XAuint32 *pItemCount
 *      Number of metadata items.  Must be non-NULL
 */
XAresult XAMetadataExtractionItfImpl_GetItemCount(
        XAMetadataExtractionItf self, XAuint32 *pItemCount)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;
    DEBUG_API("->XAMetadataExtractionItfImpl_GetItemCount");

    impl = GetImpl(self);
    /* check parameters */
    if (!impl || !pItemCount)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        }
    else
        {

        if (impl->adaptCtx)
            {

            if (impl->adaptCtx->fwtype == FWMgrFWMMF)
                {
                void *mmfCtx = GetMetadataUtilityContext(
                        (XAAdaptationMMFCtx*) impl->adaptCtx);
                if (mmfCtx)
                    {
                    res = mmf_get_item_count(mmfCtx, pItemCount);
                    }
                else
                    {
                    res = XA_RESULT_PARAMETER_INVALID;
                    }
                }
            else
                {
                res = XA_RESULT_PARAMETER_INVALID;
                }
            }
        else
            {
            res = XA_RESULT_INTERNAL_ERROR;
            }

        DEBUG_INFO_A1("itemCount = %d", (int)*pItemCount);
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_GetItemCount (%d)", (int)res);
    return res;
    }

/*
 * Returns the byte size of a given metadata key
 *
 * @XAuint32 index
 *      Metadata item Index. Range is [0, GetItemCount)
 * @XAuint32 *pKeySize
 *      Address to store key size. size must be greater than 0.  Must be non-NULL
 */
XAresult XAMetadataExtractionItfImpl_GetKeySize(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 *pKeySize)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;
    
    DEBUG_API("->XAMetadataExtractionItfImpl_GetKeySize");

    impl = GetImpl(self);
    if (!impl || !pKeySize)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMetadataExtractionItfImpl_GetKeySize");
        return XA_RESULT_PARAMETER_INVALID;
        }
    *pKeySize = 0;

    if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        void *mmfCtx = GetMetadataUtilityContext(
                (XAAdaptationMMFCtx*) impl->adaptCtx);
        if (mmfCtx)
            {
            res = mmf_get_key_size(mmfCtx, index, pKeySize);
            }
        else
            {
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {
        res = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_GetKeySize (%d)", (int)res);
    return res;
    }

/*
 * Returns a XAMetadataInfo structure and associated data referenced by the structure for a key.
 * @XAuint32 index
 *      Metadata item Index. Range is [0, GetItemCount())
 * @XAuint32 keySize
 *      Size of the memory block passed as key. Range is [1, GetKeySize].
 * @XAMetadataInfo *pKey
 *      Address to store the key.  Must be non-NULL
 */
XAresult XAMetadataExtractionItfImpl_GetKey(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 keySize, XAMetadataInfo *pKey)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractionItfImpl_GetKey");

    impl = GetImpl(self);
    if (!impl || !pKey)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMetadataExtractionItfImpl_GetKey");
        return XA_RESULT_PARAMETER_INVALID;
        }

    memset(pKey, 0, keySize);

    if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        void *mmfCtx = GetMetadataUtilityContext(
                (XAAdaptationMMFCtx*) impl->adaptCtx);
        if (mmfCtx)
            {
            res = mmf_get_key(mmfCtx, index, keySize, pKey);
            }
        else
            {
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {
        res = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_GetKey (%d)", (int)res);
    return res;
    }

/*
 * Returns the byte size of a given metadata value
 * @XAuint32 index
 *      Metadata item Index. Range is [0, GetItemCount())
 * @XAuint32 *pValueSize
 *      Address to store value size. size must be greater than 0.  Must be non-NULL
 */
XAresult XAMetadataExtractionItfImpl_GetValueSize(
        XAMetadataExtractionItf self, XAuint32 index, XAuint32 *pValueSize)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractionItfImpl_GetValueSize");

    impl = GetImpl(self);
    if (!impl || !pValueSize)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMetadataExtractionItfImpl_GetValueSize");
        return XA_RESULT_PARAMETER_INVALID;
        }
    *pValueSize = 0;

    if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        void *mmfCtx = GetMetadataUtilityContext(
                (XAAdaptationMMFCtx*) impl->adaptCtx);
        if (mmfCtx)
            {
            res = mmf_get_value_size(mmfCtx, index, pValueSize);
            }
        else
            {
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {
        res = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_GetValueSize (%d)", (int)res);
    return res;
    }

/*
 * Returns a XAMetadataInfo structure and associated data referenced by the structure for a value.
 *  @XAuint32 index
 *      Metadata item Index. Range is [0, GetItemCount())
 *  @XAuint32 valueSize
 *      Size of the memory block passed as value. Range is [0, GetValueSize]
 *  @XAMetadataInfo *pValue
 *      Address to store the value.  Must be non-NULL
 */
XAresult XAMetadataExtractionItfImpl_GetValue(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 valueSize, XAMetadataInfo *pValue)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractionItfImpl_GetValue");

    impl = GetImpl(self);
    if (!impl || !pValue)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMetadataExtractionItfImpl_GetValue");
        return XA_RESULT_PARAMETER_INVALID;
        }

    memset(pValue, 0, valueSize);

    if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
        {
        void *mmfCtx = GetMetadataUtilityContext(
                (XAAdaptationMMFCtx*) impl->adaptCtx);
        if (mmfCtx)
            {
            res = mmf_get_value(mmfCtx, index, valueSize, pValue);
            }
        else
            {
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }
    else
        {
        res = XA_RESULT_PARAMETER_INVALID;
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_GetValue (%d)",(int)res);
    return res;
    }

/*
 * Adds a filter for a specific key
 * @XAuint32 keySize
 *      Size in bytes, of the pKey parameter. Ignored if filtering by key is disabled
 * @const void *pKey
 *      The key to filter by. The entire key must match. Ignored if filtering by key is disabled.
 * @XAuint32 keyEncoding
 *      Character encoding of the pKey parameter. Ignored if filtering by key is disabled
 * @const XAchar *pValueLangCountry
 *      Language / country code of the value to filter by. Ignored if filtering by language / country is disabled
 * @XAuint32 valueEncoding
 *      Encoding of the value to filter by. Ignored if filtering by encoding is disabled
 * @XAuint8 filterMask
 *      Bitmask indicating which criteria to filter by. Should be one of the XA_METADATA_FILTER macros
 */
XAresult XAMetadataExtractionItfImpl_AddKeyFilter(
        XAMetadataExtractionItf self, XAuint32 keySize, const void *pKey,
        XAuint32 keyEncoding, const XAchar *pValueLangCountry,
        XAuint32 valueEncoding, XAuint8 filterMask)
    {
    XAresult res = XA_RESULT_SUCCESS;

    XAMetadataExtractionItfImpl *impl = NULL;

    impl = GetImpl(self);

    DEBUG_API("->XAMetadataExtractionItfImpl_AddKeyFilter");

    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        DEBUG_API("<-XAMetadataExtractionItfImpl_AddKeyFilter");
        return XA_RESULT_PARAMETER_INVALID;
        }
    else
        {

        if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
            {
            DEBUG_API("<-XAMetadataExtractionItfImpl_AddKeyFilter Not Supported in MMF");
            res = XA_RESULT_PARAMETER_INVALID;
            }
        else
            {
            DEBUG_API("<-XAMetadataExtractionItfImpl_AddKeyFilter Not Supported in GST");
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }
    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_AddKeyFilter (%d)", (int)res);
    return res;
    }

/*
 * Clears the key filter
 */
XAresult XAMetadataExtractionItfImpl_ClearKeyFilter(
        XAMetadataExtractionItf self)
    {
    XAMetadataExtractionItfImpl *impl = NULL;
    XAresult res = XA_RESULT_SUCCESS;

    DEBUG_API("->XAMetadataExtractionItfImpl_ClearKeyFilter");
    impl = GetImpl(self);
    if (!impl)
        {
        DEBUG_ERR("XA_RESULT_PARAMETER_INVALID");
        res = XA_RESULT_PARAMETER_INVALID;
        }
    else
        {

        if (impl->adaptCtx && impl->adaptCtx->fwtype == FWMgrFWMMF)
            {
            DEBUG_API("<-XAMetadataExtractionItfImpl_ClearKeyFilter Not Supported in MMF");
            res = XA_RESULT_PARAMETER_INVALID;
            }
        else
            {
            DEBUG_API("<-XAMetadataExtractionItfImpl_ClearKeyFilter Not Supported in GST");
            res = XA_RESULT_PARAMETER_INVALID;
            }
        }

    DEBUG_API_A1("<-XAMetadataExtractionItfImpl_ClearKeyFilter (%d)", (int)res);
    return res;
    }

/*****************************************************************************
 * XAMetadataExtractionItfImpl -specific methods
 *****************************************************************************/

/* XAMetadataExtractionItfImpl* XAMetadataExtractionItfImpl_Create()
 * Description: Allocate and initialize XAMetadataExtractionItfImpl
 */
XAMetadataExtractionItfImpl* XAMetadataExtractionItfImpl_Create(
        XAAdaptationBaseCtx *adaptCtx)
    {
    XAMetadataExtractionItfImpl *self = NULL;
    DEBUG_API("->XAMetadataExtractionItfImpl_Create");

    self = (XAMetadataExtractionItfImpl*) calloc(1,
            sizeof(XAMetadataExtractionItfImpl));

    if (self)
        {
        /* init itf default implementation */
        self->itf.GetItemCount = XAMetadataExtractionItfImpl_GetItemCount;
        self->itf.GetKeySize = XAMetadataExtractionItfImpl_GetKeySize;
        self->itf.GetKey = XAMetadataExtractionItfImpl_GetKey;
        self->itf.GetValueSize = XAMetadataExtractionItfImpl_GetValueSize;
        self->itf.GetValue = XAMetadataExtractionItfImpl_GetValue;
        self->itf.AddKeyFilter = XAMetadataExtractionItfImpl_AddKeyFilter;
        self->itf.ClearKeyFilter = XAMetadataExtractionItfImpl_ClearKeyFilter;

        /* init variables */
        self->adaptCtx = adaptCtx;

        if (self->adaptCtx->fwtype != FWMgrFWMMF)
            {
            XAAdaptationBase_AddEventHandler(adaptCtx,
                    &XAMetadataExtractionItfImp_AdaptCb, XA_METADATAEVENTS,
                    self);
            }

        self->self = self;
        }

    DEBUG_API("<-XAMetadataExtractionItfImpl_Create");
    return self;
    }

/* void XAMetadataExtractionItfImpl_Free(XAMetadataExtractionItfImpl* self)
 * Description: Free all resources reserved at XAMetadataExtractionItfImpl_Create
 */
void XAMetadataExtractionItfImpl_Free(XAMetadataExtractionItfImpl* self)
    {
    DEBUG_API("->XAMetadataExtractionItfImpl_Free");

    if (self->adaptCtx->fwtype != FWMgrFWMMF)
        {
        XAAdaptationBase_RemoveEventHandler(self->adaptCtx,
                &XAMetadataExtractionItfImp_AdaptCb);
        }

    free(self);
    DEBUG_API("<-XAMetadataExtractionItfImpl_Free");
    }

/* With this method, adaptation infroms that new tags are found (e.g. if source,
 * has changed, live stream contains metadata...)
 */
void XAMetadataExtractionItfImp_AdaptCb(void *pHandlerCtx,
        XAAdaptEvent *event)
    {
    XAMetadataExtractionItfImpl* impl = NULL;
    DEBUG_API("->XAMetadataExtractionItfImp_AdaptCb");
    impl = (XAMetadataExtractionItfImpl*) pHandlerCtx;
    if (!impl)
        {
        DEBUG_ERR("XAMetadataExtractionItfImp_AdaptCb, invalid context pointer!");
        DEBUG_API("<-XAMetadataExtractionItfImp_AdaptCb");
        return;
        }
    DEBUG_INFO("unhandled");
    DEBUG_API("<-XAMetadataExtractionItfImp_AdaptCb");
    }


