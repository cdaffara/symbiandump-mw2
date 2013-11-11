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
 * Description: Metadata Extraction Implementation Header
 *
 */

#ifndef XAMETADATAEXTRACTIONITF_H
#define XAMETADATAEXTRACTIONITF_H

#include "xaadptbasectx.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/

/** STRUCTURES **/
/* Definition of XAMetadataExtractionItf implementation */
typedef struct XAMetadataExtractionItfImpl_
    {
    /* parent interface */
    struct XAMetadataExtractionItf_ itf;
    /* pointer to self */
    struct XAMetadataExtractionItfImpl_* self;

    /* variables */
    XAAdaptationBaseCtx *adaptCtx;

    } XAMetadataExtractionItfImpl;

/** METHODS **/

/* Base interface XAMetadataExtractionItf implementation */
XAresult XAMetadataExtractionItfImpl_GetItemCount(
        XAMetadataExtractionItf self, XAuint32 *pItemCount);

XAresult XAMetadataExtractionItfImpl_GetKeySize(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 *pKeySize);

XAresult XAMetadataExtractionItfImpl_GetKey(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 keySize, XAMetadataInfo *pKey);

XAresult XAMetadataExtractionItfImpl_GetValueSize(
        XAMetadataExtractionItf self, XAuint32 index, XAuint32 *pValueSize);

XAresult XAMetadataExtractionItfImpl_GetValue(XAMetadataExtractionItf self,
        XAuint32 index, XAuint32 valueSize, XAMetadataInfo *pValue);

XAresult XAMetadataExtractionItfImpl_AddKeyFilter(
        XAMetadataExtractionItf self, XAuint32 keySize, const void *pKey,
        XAuint32 keyEncoding, const XAchar *pValueLangCountry,
        XAuint32 valueEncoding, XAuint8 filterMask);

XAresult XAMetadataExtractionItfImpl_ClearKeyFilter(
        XAMetadataExtractionItf self);

/* XAMetadataExtractionItfImpl -specific methods */
XAMetadataExtractionItfImpl* XAMetadataExtractionItfImpl_Create(
        XAAdaptationBaseCtx *adaptCtx);
void XAMetadataExtractionItfImp_AdaptCb(void *pHandlerCtx,
        XAAdaptEvent *event);
void XAMetadataExtractionItfImpl_Free(XAMetadataExtractionItfImpl* self);

/* internal methods */
/*XAresult CheckAndUnfilterIndex(XAMetadataExtractionItfImpl *impl,
        XAuint32 oldidx, XAuint32 *newidx);*/

#endif /* XAMETADATAEXTRACTIONITF_H */
