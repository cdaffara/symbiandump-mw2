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

#include "tmsutility.h"
#include "tmsilbcbodyimpl.h"
#include "tmsilbcformatimpl.h"

using namespace TMS;

TMSILBCFormatImpl::TMSILBCFormatImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSILBCFormatImpl::~TMSILBCFormatImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSILBCFormatImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSILBCFormatBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSILBCFormatBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        this->iFormatBody = static_cast<TMSILBCFormatBodyImpl*>(bodyimpl);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSILBCFormatImpl::Create(TMSFormat*& tmsformat)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSILBCFormatImpl *self = new TMSILBCFormatImpl();

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

gint TMSILBCFormatImpl::SetProxy(TMSGlobalContext* context,
        gpointer queuehandler)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSILBCFormatBodyImpl*>(this->iBody)->SetProxy(context,
                queuehandler);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

// End of file
