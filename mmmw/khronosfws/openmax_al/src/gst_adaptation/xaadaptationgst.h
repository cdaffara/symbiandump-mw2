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
 * Description: Base Gstreamer Adaptation Source
 *
 */

#ifndef XAADAPTATIONGST_H_
#define XAADAPTATIONGST_H_

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <gst/gst.h>
#include "openmaxalwrapper.h"
#include "xaadptbasectx.h"


#ifdef XA_IMPL_MEASURE_GST_DELAY
#include <time.h>
#endif /* XA_IMPL_MEASURE_GST_DELAY */

/* TYPEDEFS */
typedef gboolean (*GstBusCb)(GstBus *bus, GstMessage *message, gpointer data);

/* STRUCTURES */

/* Forward declaration of adaptation basecontext */
typedef struct XAAdaptationGstCtx_ XAAdaptationGstCtx;

/*
 * Structure that holds all common variables for every
 * Gst-Adaptation context structures.
 */
typedef struct XAAdaptationGstCtx_
    {
    /* Common Variables for all adaptation elements */
    XAAdaptationBaseCtx baseObj;

    GstState binWantedState; /** requested gst-bin target state **/
    GstElement *bin; /** Container for all gst elements **/

    GstBus *bus; /** Gst-bus where gst sends messages **/
    GMainLoop *busloop; /** Gst-bus listener loop **/
    pthread_t busloopThr;
    GstBusCb busCb; /** Gst-Bus callback funtion*/
    GError* busError;
    
    XAboolean waitingasyncop;
    sem_t semAsyncWait;
    guint asynctimer;

#ifdef XA_IMPL_MEASURE_GST_DELAY
    clock_t startTime;
    clock_t endTime;
    double diff;
#endif /*XA_IMPL_MEASURE_GST_DELAY*/

    XAboolean thread_launched;
    pthread_mutex_t ds_mutex;
    pthread_cond_t ds_condition;
    XAboolean cond_mutx_inited;
    /* FUNCTIONS*/

    } XAAdaptationGstCtx_;

/* FUNCTIONS */
/*
 * gboolean XAAdaptationBase_GstBusCb( GstBus *bus, GstMessage *message, gpointer data );
 * Default CallBack handler for gst-bus messages. This will be called if object specific callback is
 * not implemented.
 */

XAresult XAAdaptationGst_Init(XAAdaptationGstCtx* pSelf, XAuint32 ctxId);
XAresult XAAdaptationGst_PostInit(XAAdaptationGstCtx* ctx);
void XAAdaptationGst_Free(XAAdaptationGstCtx* ctx);

gboolean XAAdaptationGst_GstBusCb(GstBus *bus, GstMessage *message,
        gpointer data);

XAresult XAAdaptationGst_InitGstListener(XAAdaptationGstCtx* ctx);
void* XAAdaptationGst_LaunchGstListener(void* args);
void XAAdaptationGst_StopGstListener(XAAdaptationGstCtx* ctx);

void XAAdaptationGst_PrepareAsyncWait(XAAdaptationGstCtx* ctx);
void XAAdaptationGst_StartAsyncWait(XAAdaptationGstCtx* ctx);
gboolean XAAdaptationGst_CancelAsyncWait(gpointer ctx);
void XAAdaptationGst_CompleteAsyncWait(XAAdaptationGstCtx* ctx);

GstElement* XAAdaptationGst_CreateGstSource(XADataSource* xaSrc,
        const char *name, XAboolean *isPCM );
GstElement* XAAdaptationGst_CreateGstSink(XADataSink* xaSrc,
        const char *name);

void XAAdaptationGst_PadBlockCb(GstPad *pad, gboolean blocked,
        gpointer user_data);

#endif /* XAADAPTATIONGST_H_ */
