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

#ifndef TMS_FORMAT_BODY_IMPL_H
#define TMS_FORMAT_BODY_IMPL_H

#include <tms.h>
#include "tmsformatbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatObserver;
class TMSCallProxy;

// TMSFormatBodyImpl class
class TMSFormatBodyImpl : public TMSFormatBody
    {
public:
    virtual ~TMSFormatBodyImpl();

    gint GetSupportedBitRates(BitRateVector& aVector);
    gint SetBitRate(const guint aBitrate);
    gint GetBitRate(guint& aBitrate);
    gint GetType(TMSFormatType& fmttype);

    virtual void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

protected:
    TMSFormatBodyImpl();
    gint PostConstruct(TMSFormatType formatType);

private:
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSFormatType iFormatType;
    };

} //namespace TMS

#endif //TMS_FORMAT_BODY_IMPL_H
