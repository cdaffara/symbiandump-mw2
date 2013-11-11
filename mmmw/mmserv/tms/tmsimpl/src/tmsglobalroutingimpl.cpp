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
#include "tmsglobalroutingbodyimpl.h"
#include "tmsglobalroutingimpl.h"

using namespace TMS;

TMSGlobalRoutingImpl::TMSGlobalRoutingImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSGlobalRoutingImpl::~TMSGlobalRoutingImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSGlobalRoutingImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGlobalRoutingBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSGlobalRoutingBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSGlobalRoutingImpl::Create(TMSGlobalRouting*& globrouting)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSGlobalRoutingImpl *self = new TMSGlobalRoutingImpl();

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
    if (self && ret == TMS_RESULT_SUCCESS)
        {
        globrouting = self;
        ret = self->SetParent(globrouting);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSGlobalRoutingImpl::Delete(TMSGlobalRouting*& globrouting)
    {
    gint ret(TMS_RESULT_SUCCESS);
    TRACE_PRN_FN_ENT;
    delete (static_cast<TMSGlobalRoutingImpl*>(globrouting));
    globrouting = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSGlobalRoutingImpl::SetParent(TMSGlobalRouting*& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSGlobalRoutingBodyImpl*>(this->iBody)->SetParent(parent);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

