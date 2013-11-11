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
 * Description: MediaPlayer Object Implementation Header
 *
 */

#ifndef XAMEDIAPLAYER_H
#define XAMEDIAPLAYER_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaobjectitf.h"
#include "xamediaplayeradaptctxmmf.h"
#include "xacapabilitiesmgr.h"
/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/
/* Enumeration for interfaces that MediaPlayer supports.  */
typedef enum
    {
    MP_OBJECTITF,
    MP_DIMITF,
    MP_PLAYITF,
    MP_SEEKITF,
    MP_VOLUMEITF,
    MP_PREFETCHSTATUSITF,
    MP_DYNAMICSOURCEITF,
    MP_METADATAEXTRACTIONITF,
    MP_PLAYBACKRATEITF,
    MP_NOKIAVOLUMEEXT,
    MP_NOKIALINEARVOLUME,
    MP_STREAMINFORMATIONITF,
    MP_VIDEOPOSTPROCESSINGITF,
    MP_ITFCOUNT
    } MPInterfaces;

/** STRUCTURES **/
/* Specification for XAMediaPlayerImpl.*/
typedef struct XAMediaPlayerImpl_
    {
    /* Parent for XAMediaPlayerImpl */
    XAObjectItfImpl baseObj;

    /* variables */
    XADataSource *dataSrc;
    XADataSource *bankSrc;
    XADataSink *audioSnk;
    XADataSink *imageVideoSnk;
    XADataSink *vibra;
    XADataSink *LEDArray;
    XAuint32 numRequiredInterfaces;
    XAInterfaceID *requiredItfIds;

    XAAdaptationBaseCtx* curAdaptCtx;
    XAAdaptationBaseCtx* adaptationCtxGst;
    XAAdaptationBaseCtx* adaptationCtxMMF;

    } XAMediaPlayerImpl;

/** METHODS **/

/* base object XAObjectItfImpl methods */
XAresult XAMediaPlayerImpl_DoRealize(XAObjectItf self);
XAresult XAMediaPlayerImpl_DoResume(XAObjectItf self);
void XAMediaPlayerImpl_FreeResources(XAObjectItf self);

/* DynamicInterfaceManagement object-specific methods */
XAresult XAMediaPlayerImpl_DoAddItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMediaPlayerImpl_DoResumeItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);
XAresult XAMediaPlayerImpl_DoRemoveItf(XAObjectItf self,
        XAObjItfMapEntry *mapEntry);

#endif /* XAMEDIAPLAYER_H */
