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
#include "tmsmodemsinkimpl.h"
#include "tmsmodemsinkbodyimpl.h"

using namespace TMS;

TMSModemSinkImpl::TMSModemSinkImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSModemSinkImpl::~TMSModemSinkImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSModemSinkImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSModemSinkBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSModemSinkBodyImpl::Create(bodyimpl);
    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSModemSinkImpl::Create(TMSSink*& tmssink)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSModemSinkImpl *self = new TMSModemSinkImpl();

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
    tmssink = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSModemSinkImpl::SetProxy(TMSCallProxy* aProxy, gpointer queuehandler)
    {
    gint ret = TMS_RESULT_SUCCESS;
    static_cast<TMSModemSinkBodyImpl*>(this->iBody)->SetProxy(aProxy,
            queuehandler);
    return ret;
    }

// End of file
