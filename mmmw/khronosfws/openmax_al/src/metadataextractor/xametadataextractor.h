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
 * Description: Metadata Extractor Header
 *
 */

#ifndef XAMETADATAEXTRACTOR_H
#define XAMETADATAEXTRACTOR_H

#include "openmaxalwrapper.h"
#include "xaobjectitf.h"
#include "xaglobals.h"
#include "xaadptbasectx.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/
/* Enumeration for interfaces that MetadataExtractor supports.  */
typedef enum
    {
    MDE_OBJECTITF, /* <-keep this first */
    MDE_DIMITF,
    MDE_METADATAEXTRACTIONITF,
    MDE_DYNAMICSOURCEITF,
    MDE_ITFCOUNT
    } XAMetadataExtractorInterfaces;

/** STRUCTURES **/
/* Specification for XAMetadataExtractorImpl
 */
typedef struct XAMetadataExtractorImpl_
    {
    /* Parent for XAMetadataExtractorImpl */
    XAObjectItfImpl baseObj; /* <-keep this first */

    /* variables */
    XADataSource *dataSrc;

    XAAdaptationBaseCtx* curAdaptCtx;
    XAAdaptationBaseCtx* adaptationCtxGst;
    XAAdaptationBaseCtx* adaptationCtxMMF;

    } XAMetadataExtractorImpl;

/** METHODS **/

/* base object XAObjectItfImpl methods */
XAresult XAMetadataExtractorImpl_DoRealize(XAObjectItf self);
XAresult XAMetadataExtractorImpl_DoResume(XAObjectItf self);
void XAMetadataExtractorImpl_FreeResources(XAObjectItf self);

/* MetadataExtractorImpl -specific methods */
XAresult XAMetadataExtractorImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMetadataExtractorImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMetadataExtractorImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);

#endif /* XAMETADATAEXTRACTOR_H */
