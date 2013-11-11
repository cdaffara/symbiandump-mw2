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
#include "tmsclientsourcebodyimpl.h"
#include "tmsclientsourceimpl.h"

using namespace TMS;

TMSClientSourceImpl::TMSClientSourceImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSClientSourceImpl::~TMSClientSourceImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSClientSourceImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSClientSourceBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSClientSourceBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSClientSourceImpl::Create(TMSSource*& tmssource)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSClientSourceImpl *self = new TMSClientSourceImpl();

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
    tmssource = self;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSClientSourceImpl::SetProxy(TMSGlobalContext* context,
        gpointer queuehandler)
    {
    gint ret = TMS_RESULT_SUCCESS;
    static_cast<TMSClientSourceBodyImpl*>(this->iBody)->SetProxy(context,
            queuehandler);
    return ret;
    }

// End of file
