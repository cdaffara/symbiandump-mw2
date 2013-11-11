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
#include <tmsdtmfobsrvr.h>
#include "tmsutility.h"
#include "tmsproxy.h"
#include "tmsdtmfbodyimpl.h"

using namespace TMS;

TMSDTMFBodyImpl::TMSDTMFBodyImpl() :
    iObserver(NULL),
    iProxy(NULL),
    iParent(NULL),
    iString(NULL)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSDTMFBodyImpl::~TMSDTMFBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    if (iProxy)
        {
        iProxy->Close();
        }
    delete iProxy;

    if (iString)
        {
        g_string_free(iString, TRUE);
        }
    iObserver = NULL;
    iParent = NULL;
    iUserData = NULL;
    TRACE_PRN_FN_EXT;
    }

gint TMSDTMFBodyImpl::Create(TMSStreamType streamtype, TMSDTMF& parent,
        TMSDTMFBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSDTMFBodyImpl* self = new TMSDTMFBodyImpl();
    if (self)
        {
        ret = self->PostConstruct(streamtype, parent);
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    bodyimpl = self;
    return ret;
    }

gint TMSDTMFBodyImpl::PostConstruct(TMSStreamType streamtype, TMSDTMF& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    iClientId = 1;
    iParent = &parent;
    iStreamType = streamtype;
    iProxy = new TMSProxy();
    if (!iProxy)
        {
        ret = TMS_RESULT_INSUFFICIENT_MEMORY;
        }
    else
        {
        if (iProxy->Connect() == TMS_RESULT_SUCCESS)
            {
            ret = iProxy->InitDTMFPlayer(iStreamType);
            }
        else
            {
            delete iProxy;
            iProxy = NULL;
            ret = TMS_RESULT_FATAL_ERROR;
            }
        }
    RET_REASON_IF_ERR(ret);
    return ret;
    }

gint TMSDTMFBodyImpl::AddObserver(TMSDTMFObserver& obsrvr, gpointer user_data)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (!iObserver)
        {
        iObserver = &obsrvr;
        iUserData = user_data;
        if (iProxy)
            {
            ret = iProxy->SetMsgQueueNotifier(EMsgQueueDTMFType, iObserver,
                    iParent, iClientId);
            }
        else
            {
            ret = TMS_RESULT_DOES_NOT_EXIST;
            }
        }
    else
        {
        ret = TMS_RESULT_ALREADY_EXIST;
        }
    return ret;
    }

gint TMSDTMFBodyImpl::RemoveObserver(TMSDTMFObserver& obsrvr)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && (&obsrvr == iObserver))
        {
        ret = iProxy->RemoveMsgQueueNotifier(EMsgQueueDTMFType, iObserver);
        iObserver = NULL;
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

gint TMSDTMFBodyImpl::Start()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy && iString)
        {
        if (iString->len)
            {
            ret = iProxy->StartDTMF(iStreamType, iString);
            }
        else
            {
            ret = TMS_RESULT_INVALID_ARGUMENT;
            }
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSDTMFBodyImpl::Stop()
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->StopDTMF(iStreamType);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

gint TMSDTMFBodyImpl::SetTone(GString* string)
    {
    __ASSERT_ALWAYS(string, PANIC(TMS_RESULT_NULL_ARGUMENT));

    gint ret(TMS_RESULT_SUCCESS);
    if (iString)
        {
        if (iString->len)
            {
            g_string_free(iString, TRUE);
            }
        }

    iString = g_string_new_len(string->str, string->len);
    return ret;
    }

gint TMSDTMFBodyImpl::ContinueDTMFStringSending(gboolean sending)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (iProxy)
        {
        ret = iProxy->ContinueDTMFStringSending(sending);
        }
    else
        {
        ret = TMS_RESULT_DOES_NOT_EXIST;
        }
    return ret;
    }

