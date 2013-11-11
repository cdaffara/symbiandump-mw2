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
#include "tmsinbandtonebodyimpl.h"
#include "tmsinbandtoneimpl.h"

using namespace TMS;

TMSInbandToneImpl::TMSInbandToneImpl()
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

TMSInbandToneImpl::~TMSInbandToneImpl(void)
    {
    TRACE_PRN_FN_ENT;
    TRACE_PRN_FN_EXT;
    }

gint TMSInbandToneImpl::PostConstruct()
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSInbandToneBody* bodyimpl(NULL);
    TRACE_PRN_FN_ENT;
    ret = TMSInbandToneBodyImpl::Create(bodyimpl);

    if (ret == TMS_RESULT_SUCCESS)
        {
        this->iBody = bodyimpl;
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSInbandToneImpl::Create(TMSInbandTone*& inbandtone)
    {
    gint ret(TMS_RESULT_INSUFFICIENT_MEMORY);
    TMSInbandToneImpl *self = new TMSInbandToneImpl();

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
        inbandtone = self;
        ret = self->SetParent(inbandtone);
        }
    TRACE_PRN_FN_EXT;
    return ret;
    }

EXPORT_C gint TMSInbandToneImpl::Delete(TMSInbandTone*& inbandtone)
    {
    gint ret(TMS_RESULT_SUCCESS);
    TRACE_PRN_FN_ENT;
    delete (static_cast<TMSInbandToneImpl*>(inbandtone));
    inbandtone = NULL;
    TRACE_PRN_FN_EXT;
    return ret;
    }

gint TMSInbandToneImpl::SetParent(TMSInbandTone*& parent)
    {
    gint ret(TMS_RESULT_SUCCESS);
    if (this->iBody)
        {
        static_cast<TMSInbandToneBodyImpl*>(this->iBody)->SetParent(parent);
        }
    else
        {
        ret = TMS_RESULT_UNINITIALIZED_OBJECT;
        }
    return ret;
    }

