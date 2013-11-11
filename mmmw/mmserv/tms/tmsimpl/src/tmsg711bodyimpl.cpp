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
#include "tmscallproxy.h"
#include "tmsqueuehandler.h"
#include "tmsutility.h"
#include "tmsglobalcontext.h"
#include "tmsg711bodyimpl.h"

using namespace TMS;

TMSG711FormatBodyImpl::TMSG711FormatBodyImpl()
    {
    iProxy = NULL;
    }

TMSG711FormatBodyImpl::~TMSG711FormatBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSG711FormatBodyImpl::Create(TMSG711FormatBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSG711FormatBodyImpl* self = new TMSG711FormatBodyImpl();
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

gint TMSG711FormatBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatBodyImpl::PostConstruct(TMS_FORMAT_G711);
    return ret;
    }

gint TMSG711FormatBodyImpl::SetMode(const TMSG711CodecMode mode)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetFormatCodecMode(TMS_FORMAT_G711, iStreamType, mode);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::GetMode(TMSG711CodecMode& mode)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetFormatCodecMode(TMS_FORMAT_G711, iStreamType, mode);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::SetCNG(const gboolean aCng)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetCNG(TMS_FORMAT_G711, aCng);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::GetCNG(gboolean& aCng)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetCNG(TMS_FORMAT_G711, aCng);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::SetPlc(const gboolean aPlc)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetPlc(TMS_FORMAT_G711, aPlc);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::GetPlc(gboolean& aPlc)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetPlc(TMS_FORMAT_G711, aPlc);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::SetVADMode(const gboolean aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetVADMode(TMS_FORMAT_G711, aVad);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::GetVADMode(gboolean& aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetVADMode(TMS_FORMAT_G711, aVad);
        }
    return ret;
    }

gint TMSG711FormatBodyImpl::GetType(TMSFormatType& fmttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    fmttype = TMS_FORMAT_G711;
    return ret;
    }

void TMSG711FormatBodyImpl::SetProxy(TMSGlobalContext* context,
        gpointer queuehandler)
    {
    if (context && queuehandler)
        {
        iProxy = context->CallProxy;
        iStreamType = context->StreamType;
        TMSFormatBodyImpl::SetProxy(iProxy, queuehandler);
        }
    }

// End of file
