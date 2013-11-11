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

#ifndef XAPREFETCHSTATUSITF_H_
#define XAPREFETCHSTATUSITF_H_

#include "openmaxalwrapper.h"
#include "xaglobals.h"
#include "xaadptbasectx.h"
#include "xamediaplayer.h"

#define PREFETCHSTATUSITF_DEFAULT_UPDATE_PERIOD 100

/*
 * implementation declaration
 */
typedef struct XAPrefetchStatusItfImpl_
{
    /* "inherit" interface */
    struct XAPrefetchStatusItf_ itf;
    /* pointer to self */
    struct XAPrefetchStatusItfImpl_* self;

    /* variables */

    XAuint32 pStatus;
    XApermille pLevel;

    XAPrefetchStatusItf cbPtrToSelf;
    xaPrefetchCallback  callback;
    void                *cbcontext;
    XAuint32            eventFlags;
    XApermille          fillUpdatePeriod;
    XAAdaptationBaseCtx* adaptationCtx;
}XAPrefetchStatusItfImpl;

/**
 * XAPrefetchStatusItf interface implementation
 */
XAresult XAPrefetchStatusItfImpl_GetPrefetchStatus(XAPrefetchStatusItf self,
                                                     XAuint32 *pStatus);

XAresult XAPrefetchStatusItfImpl_GetFillLevel(XAPrefetchStatusItf self,
                                                XApermille *pLevel);

XAresult XAPrefetchStatusItfImpl_RegisterCallback(XAPrefetchStatusItf self,
                                                    xaPrefetchCallback callback,
                                                    void *pContext);

XAresult XAPrefetchStatusItfImpl_SetCallbackEventsMask(XAPrefetchStatusItf self,
                                                         XAuint32 eventFlags);

XAresult XAPrefetchStatusItfImpl_GetCallbackEventsMask(XAPrefetchStatusItf self,
                                                         XAuint32 *pEventFlags);

XAresult XAPrefetchStatusItfImpl_SetFillUpdatePeriod(XAPrefetchStatusItf self,
                                                       XApermille period);

XAresult XAPrefetchStatusItfImpl_GetFillUpdatePeriod(XAPrefetchStatusItf self,
                                                       XApermille *pPeriod);



/*
 * implementation-specific methods
 */
XAPrefetchStatusItfImpl* XAPrefetchStatusItfImpl_Create(  XAMediaPlayerImpl* impl );
void XAPrefetchStatusItfImpl_AdaptCb( void *pHandlerCtx, XAAdaptEvent *event );
void XAPrefetchStatusItfImpl_Free(XAPrefetchStatusItfImpl* self);

#endif /* XAPREFETCHSTATUSITF_H_ */
