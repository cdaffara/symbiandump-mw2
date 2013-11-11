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
#include "tmspcmbodyimpl.h"

using namespace TMS;

TMSPCMFormatBodyImpl::TMSPCMFormatBodyImpl()
    {
    iProxy = NULL;
    }

TMSPCMFormatBodyImpl::~TMSPCMFormatBodyImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSPCMFormatBodyImpl::Create(TMSPCMFormatBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSPCMFormatBodyImpl* self = new TMSPCMFormatBodyImpl();
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

gint TMSPCMFormatBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    ret = TMSFormatBodyImpl::PostConstruct(TMS_FORMAT_PCM);
    return ret;
    }

gint TMSPCMFormatBodyImpl::GetType(TMSFormatType& fmttype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    fmttype = TMS_FORMAT_PCM;
    return ret;
    }

void TMSPCMFormatBodyImpl::SetProxy(TMSGlobalContext* context,
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
