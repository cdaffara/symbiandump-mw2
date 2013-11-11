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
 * Description: Telephony Multimedia Service
 *
 */

#include <tms.h>
#include <tmsclientsourceobsrvr.h>
#include "tmscallproxy.h"
#include "tmsglobalcontext.h"
#include "tmsmembuffer.h"
#include "tmsqueuehandler.h"
#include "tmsclientsourcebodyimpl.h"

using namespace TMS;

TMSClientSourceBodyImpl::TMSClientSourceBodyImpl() :
    iObserver(NULL),
    iProxy(NULL)
    {
    iQueueMode = false;
    }

TMSClientSourceBodyImpl::~TMSClientSourceBodyImpl()
    {
    }

gint TMSClientSourceBodyImpl::Create(TMSClientSourceBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSClientSourceBodyImpl* self = new TMSClientSourceBodyImpl();
    if (self)
        {
        ret = self->PostConstruct();
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    bodyimpl = self;
    return ret;
    }

gint TMSClientSourceBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
    }

gint TMSClientSourceBodyImpl::AddObserver(TMSClientSourceObserver& obsrvr,
        gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        }
    else
        {
        ret = TMS_RESULT_ALREADY_EXIST;
        }
    return ret;
    }

gint TMSClientSourceBodyImpl::RemoveObserver(TMSClientSourceObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (&obsrvr == iObserver)
        {
        iObserver = NULL;
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

// In pull mode, client calls this.
// TODO: How to identify last buffer.
// Option 1 is to move setlast buffer to TMSBuffer interface.
// Option 2 is to have overloaded function with another parameter.
gint TMSClientSourceBodyImpl::BufferFilled(TMSBuffer& buffer)
    {
    // TODO send stream attributes here
    gint ret(TMS_RESULT_SUCCESS);
    ret = iProxy->BufferFilled(iContext->CallType, iContext->StreamType,
            iContext->StreamId, buffer);
    return ret;
    }

// Push mode
gint TMSClientSourceBodyImpl::ProcessBuffer(TMSBuffer* /*buffer*/)
    {
    gint ret(TMS_RESULT_FEATURE_NOT_SUPPORTED);
    return ret;
    }

// Indicates framework to queue ProcessBuffer. default is off
// unsupported in pull mode??? (atleast initially)
gint TMSClientSourceBodyImpl::SetEnqueueMode(const gboolean /*enable*/)
    {
    gint ret(TMS_RESULT_FEATURE_NOT_SUPPORTED);
    //iQueueMode = enable;
    return ret;
    }

gint TMSClientSourceBodyImpl::GetEnqueueMode(gboolean& enable)
    {
    gint ret(TMS_RESULT_FEATURE_NOT_SUPPORTED);
    enable = iQueueMode;
    return ret;
    }

// Sends batch data to framework and clears queue mode.
// Valid only when queue mode is set, otherwise no-op
gint TMSClientSourceBodyImpl::Flush()
    {
    gint ret(TMS_RESULT_FEATURE_NOT_SUPPORTED);
    return ret;
    }

gint TMSClientSourceBodyImpl::GetType(TMSSourceType& sourcetype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    sourcetype = TMS_SOURCE_CLIENT;
    return ret;
    }

void TMSClientSourceBodyImpl::SetProxy(TMSGlobalContext* context,
        gpointer queuehandler)
    {
    iProxy = context->CallProxy;
    iContext = context;
    if (queuehandler)
        {
        static_cast<TMSQueueHandler*>(queuehandler)->AddObserver(*this,
                TMS_SOURCE_CLIENT);
        }
    }

void TMSClientSourceBodyImpl::QueueEvent(TInt aEventType, TInt aError,
        void* user_data)
    {
    if (iObserver)
        {
        switch (aEventType)
            {
            case TMS_EVENT_SOURCE_FILL_BUFFER:
                iObserver->FillBuffer(*(static_cast<TMSBuffer*>(user_data)));
                break;
            case TMS_EVENT_SOURCE_PROCESSED_BUFFER:
                iObserver->BufferProcessed(static_cast<TMSBuffer*>(user_data),
                        aError);
                break;
            default:
                break;
            }
        }
    }

// End of file
