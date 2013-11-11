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
#include "tmsilbcbodyimpl.h"

using namespace TMS;

TMSILBCFormatBodyImpl::TMSILBCFormatBodyImpl()
    {
    iProxy = NULL;
    }

TMSILBCFormatBodyImpl::~TMSILBCFormatBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSILBCFormatBodyImpl::Create(TMSILBCFormatBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSILBCFormatBodyImpl* self = new TMSILBCFormatBodyImpl();
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

gint TMSILBCFormatBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatBodyImpl::PostConstruct(TMS_FORMAT_ILBC);
    return ret;
    }

gint TMSILBCFormatBodyImpl::SetMode(const TMSILBCCodecMode aMode)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetFormatCodecMode(TMS_FORMAT_ILBC, iStreamType, aMode);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::GetMode(TMSILBCCodecMode& aMode)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetFormatCodecMode(TMS_FORMAT_ILBC, iStreamType, aMode);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::SetCNG(const gboolean aCng)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetCNG(TMS_FORMAT_ILBC, aCng);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::GetCNG(gboolean& aCng)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetCNG(TMS_FORMAT_ILBC, aCng);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::SetVADMode(const gboolean aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->SetVADMode(TMS_FORMAT_ILBC, aVad);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::GetVADMode(gboolean& aVad)
    {
    gint ret(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iProxy)
        {
        ret = iProxy->GetVADMode(TMS_FORMAT_ILBC, aVad);
        }
    return ret;
    }

gint TMSILBCFormatBodyImpl::GetType(TMSFormatType& fmttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    fmttype = TMS_FORMAT_ILBC;
    return ret;
    }

void TMSILBCFormatBodyImpl::SetProxy(TMSGlobalContext* context,
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
