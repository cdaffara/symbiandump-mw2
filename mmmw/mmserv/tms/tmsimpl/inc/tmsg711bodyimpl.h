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

#ifndef TMS_G711_FORMAT_BODY_IMPL_H
#define TMS_G711_FORMAT_BODY_IMPL_H

#include <tms.h>
#include "tmsformatbodyimpl.h"
#include "tmsg711formatbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatObserver;
class TMSCallProxy;

// TMSG711FormatBodyImpl class
class TMSG711FormatBodyImpl : public TMSG711FormatBody,
                              public TMSFormatBodyImpl
    {
public:
    static gint Create(TMSG711FormatBody*& bodyimpl);
    virtual ~TMSG711FormatBodyImpl();

    // From TMSG711FormatBody
    virtual gint SetMode(const TMSG711CodecMode aMode);
    virtual gint GetMode(TMSG711CodecMode& aMode);
    virtual gint SetCNG(const gboolean aCng);
    virtual gint GetCNG(gboolean& aCng);
    virtual gint SetPlc(const gboolean aPlc);
    virtual gint GetPlc(gboolean& aPlc);
    virtual gint SetVADMode(const gboolean aVad);
    virtual gint GetVADMode(gboolean& aVad);
    virtual gint GetType(TMSFormatType& Formattype);
    virtual void SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSG711FormatBodyImpl();
    gint PostConstruct();

private:
    TMSFormatObserver* iObserver;
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSStreamType iStreamType;
    };

} //namespace TMS

#endif // TMS_G711_FORMAT_BODY_IMPL_H

// End of file
