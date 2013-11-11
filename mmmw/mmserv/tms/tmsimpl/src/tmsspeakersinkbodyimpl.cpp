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
#include "tmsspeakersinkbodyimpl.h"

using namespace TMS;

TMSSpeakerSinkBodyImpl::TMSSpeakerSinkBodyImpl() :
    iProxy(NULL)
    {
    }

TMSSpeakerSinkBodyImpl::~TMSSpeakerSinkBodyImpl()
    {
    }

gint TMSSpeakerSinkBodyImpl::Create(TMSSpeakerSinkBody*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSSpeakerSinkBodyImpl* self = new TMSSpeakerSinkBodyImpl();
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

gint TMSSpeakerSinkBodyImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_SUCCESS);
    return ret;
    }

gint TMSSpeakerSinkBodyImpl::GetType(TMSSinkType& sourcetype)
    {
    gint ret(TMS_RESULT_SUCCESS);
    sourcetype = TMS_SINK_SPEAKER;
    return ret;
    }

void TMSSpeakerSinkBodyImpl::SetProxy(TMSCallProxy* aProxy,
        gpointer /*queuehandler*/)
    {
    iProxy = aProxy;
    }

// End of file
