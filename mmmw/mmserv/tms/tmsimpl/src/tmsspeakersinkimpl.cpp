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
#include "tmsspeakersinkimpl.h"
#include "tmsspeakersinkbodyimpl.h"

using namespace TMS;

TMSSpeakerSinkImpl::TMSSpeakerSinkImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSSpeakerSinkImpl::~TMSSpeakerSinkImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSSpeakerSinkImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSSpeakerSinkBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSSpeakerSinkBodyImpl::Create(bodyimpl);
    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSSpeakerSinkImpl::Create(TMSSink*& bodyimpl)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSSpeakerSinkImpl* self = new TMSSpeakerSinkImpl();
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

gint TMSSpeakerSinkImpl::SetProxy(TMSCallProxy* aProxy, gpointer queuehandler)
    {
    gint ret(TMS_RESULT_SUCCESS);
    static_cast<TMSSpeakerSinkBodyImpl*>(this->iBody)->SetProxy(aProxy,
             queuehandler);
    return ret;
    }

// End of file
