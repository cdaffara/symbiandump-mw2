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
#include "tmsformatbodyimpl.h"

using namespace TMS;

TMSFormatBodyImpl::TMSFormatBodyImpl()
    {
    iProxy = NULL;
    }

TMSFormatBodyImpl::~TMSFormatBodyImpl()
    {
    }

gint TMSFormatBodyImpl::PostConstruct(TMSFormatType formatType)
    {
    gint ret(TMS_RESULT_SUCCESS);
    iFormatType = formatType;
    return ret;
    }

gint TMSFormatBodyImpl::GetSupportedBitRates(BitRateVector& aVector)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    if (iProxy)
        {
        ret = iProxy->GetSupportedBitRates(aVector);
        }
    return ret;
    }

gint TMSFormatBodyImpl::SetBitRate(const guint aBitrate)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    if (iProxy)
        {
        ret = iProxy->SetBitRate(aBitrate);
        }
    return ret;
    }

gint TMSFormatBodyImpl::GetBitRate(guint& aBitrate)
    {
    gint ret(TMS_RESULT_INVALID_ARGUMENT);
    if (iProxy)
        {
        ret = iProxy->GetBitRate(aBitrate);
        }
    return ret;
    }

gint TMSFormatBodyImpl::GetType(TMSFormatType& fmttype)
    {
    fmttype = iFormatType;
    return TMS_RESULT_SUCCESS;
    }

void TMSFormatBodyImpl::SetProxy(TMSCallProxy* aProxy, gpointer /*queuehandler*/)
    {
    iProxy = aProxy;
    //static_cast<TMSQueueHandler*>(queuehandler)->AddObserver(*this, TMS_FORMAT_TYPE);
    }

// End of file
