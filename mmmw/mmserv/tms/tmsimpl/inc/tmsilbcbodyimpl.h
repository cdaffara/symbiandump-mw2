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

#ifndef TMS_ILBC_FORMAT_BODY_IMPL_H
#define TMS_ILBC_FORMAT_BODY_IMPL_H

#include <tms.h>
#include "tmsformatbodyimpl.h"
#include "tmsilbcformatbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatObserver;
class TMSCallProxy;

// TMSILBCFormatBodyImpl class
class TMSILBCFormatBodyImpl : public TMSILBCFormatBody,
                              public TMSFormatBodyImpl
    {
public:
    static gint Create(TMSILBCFormatBody*& bodyimpl);
    virtual ~TMSILBCFormatBodyImpl();

    // From TMSILBCFormatBody
    virtual gint SetMode(const TMSILBCCodecMode aMode);
    virtual gint GetMode(TMSILBCCodecMode& aMode);
    virtual gint SetCNG(const gboolean aCng);
    virtual gint GetCNG(gboolean& aCng);
    virtual gint SetVADMode(const gboolean aVad);
    virtual gint GetVADMode(gboolean& aVad);
    virtual gint GetType(TMSFormatType& Formattype);
    virtual void SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSILBCFormatBodyImpl();
    gint PostConstruct();

private:
    TMSFormatObserver* iObserver;
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSStreamType iStreamType;
    };

} //namespace TMS

#endif // TMS_ILBC_FORMAT_BODY_IMPL_H

// End of file
