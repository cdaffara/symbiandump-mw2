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
#include "tmsg729bodyimpl.h"

using namespace TMS;

TMSG729FormatBodyImpl::TMSG729FormatBodyImpl()
    {
    iProxy = NULL;
    }

TMSG729FormatBodyImpl::~TMSG729FormatBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSG729FormatBodyImpl::Create(TMSG729FormatBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSG729FormatBodyImpl* self = new TMSG729FormatBodyImpl();
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

gint TMSG729FormatBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatBodyImpl::PostConstruct(TMS_FORMAT_G729);
    return ret;
    }

gint TMSG729FormatBodyImpl::SetVADMode(const gboolean aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetVADMode(TMS_FORMAT_G729, aVad);
        }
    return ret;
    }

gint TMSG729FormatBodyImpl::GetVADMode(gboolean& aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetVADMode(TMS_FORMAT_G729, aVad);
        }
    return ret;
    }

gint TMSG729FormatBodyImpl::GetType(TMSFormatType& fmttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    fmttype = TMS_FORMAT_G729;
    return ret;
    }

void TMSG729FormatBodyImpl::SetProxy(TMSGlobalContext* context,
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
