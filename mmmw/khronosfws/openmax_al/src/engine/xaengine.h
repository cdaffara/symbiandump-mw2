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
 * Description: Engine Implementation Header
 *
 */

#ifndef XAENGINE_H
#define XAENGINE_H

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaobjectitf.h"
#include "xaframeworkmgr.h"
#include "xacapabilitiesmgr.h"

/** MACROS **/

/** TYPES **/

/** ENUMERATIONS **/
/* Enumeration for interfaces that Engine supports.  */
typedef enum
    {
    ENGINE_OBJECTITF,
    ENGINE_ENGINEITF,
    ENGINE_DIMITF,
    ENGINE_THREADSYNCITF,
    /*
     ENGINE_CONFIGEXTENSIONSITF,
     ENGINE_DEVICEVOLUMEITF,
     */
    ENGINE_AUDIOIODEVICECAPAITF,
    /*    ENGINE_AUDIODECODERCAPAITF,*/
    ENGINE_AUDIOENCODERCAPAITF,
    /*
     ENGINE_CAMERACAPAITF,
     ENGINE_IMAGEDECODERCAPAITF,
     ENGINE_IMAGEENCODERCAPAITF,
     ENGINE_VIDEODECODERCAPAITF,
     ENGINE_VIDEOENCODERCAPAITF,
     */
    ENGINE_ITFCOUNT
    } XAEngineInterfaces;

/** STRUCTURES **/
/* Specification for XAEngineImpl.
 */
typedef struct XAEngineImpl_
    {
    /* Parent for XAEngineImpl */
    XAObjectItfImpl baseObj;

    /* engine variables */
    XAboolean isThreadSafe;

    //actual adpatation context can point to either MMF or GST implementations
    FrameworkMap *frameworkMap;
    XACapabilities* capabilities;

    /*    void* adaptationGstCtx;
     void* adaptationMmfCtx;*/
    } XAEngineImpl;

/** METHODS **/

/* base object XAObjectItfImpl methods */
XAresult XAEngineImpl_DoRealize(XAObjectItf self);
XAresult XAEngineImpl_DoResume(XAObjectItf self);
void XAEngineImpl_FreeResources(XAObjectItf self);

/* XAEngineImpl -specific methods */

#endif /* XAENGINE_H */
