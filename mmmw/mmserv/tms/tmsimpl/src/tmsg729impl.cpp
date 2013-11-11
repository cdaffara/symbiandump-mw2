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

#include "tmsg729bodyimpl.h"
#include "tmsutility.h"
#include "tmsg729formatimpl.h"

using namespace TMS;

TMSG729FormatImpl::TMSG729FormatImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSG729FormatImpl::~TMSG729FormatImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSG729FormatImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSG729FormatBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSG729FormatBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        this->iFormatBody = static_cast<TMSG729FormatBodyImpl*>(bodyimpl);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSG729FormatImpl::Create(TMSFormat*& tmsformat)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSG729FormatImpl *self = new TMSG729FormatImpl();

    TRACE_PRN_FN_ENT;
    if (self)
        {
        ret = self->PostConstruct();
        if (ret != TMS_RESULT_SUCCESS)
            {
            delete self;
            self = NULL;
            }
        }
    tmsformat = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSG729FormatImpl::SetProxy(TMSGlobalContext* context,
        gpointer queuehandler)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSG729FormatBodyImpl*>(this->iBody)->SetProxy(context,
                queuehandler);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

// End of file
