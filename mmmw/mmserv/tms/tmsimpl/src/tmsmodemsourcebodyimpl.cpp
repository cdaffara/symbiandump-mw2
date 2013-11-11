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
#include "tmsmodemsourcebodyimpl.h"

using namespace TMS;

TMSModemSourceBodyImpl::TMSModemSourceBodyImpl() :
    iProxy(NULL)
    {
    }

TMSModemSourceBodyImpl::~TMSModemSourceBodyImpl()
    {
    }

gint TMSModemSourceBodyImpl::Create(TMSModemSourceBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSModemSourceBodyImpl* self = new TMSModemSourceBodyImpl();
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

gint TMSModemSourceBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
    }

gint TMSModemSourceBodyImpl::GetType(TMSSourceType& sourcetype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    sourcetype = TMS_SOURCE_MODEM;
    return ret;
    }

void TMSModemSourceBodyImpl::SetProxy(TMSCallProxy* aProxy,
        gpointer /*queuehandler*/)
    {
    iProxy = aProxy;
    }

// End of file
