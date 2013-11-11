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

#ifndef TMS_MIC_SOURCE_BODY_IMPL_H
#define TMS_MIC_SOURCE_BODY_IMPL_H

#include <tms.h>
#include "tmsmicsourcebody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSMicSourceBodyImpl class
class TMSMicSourceBodyImpl : public TMSMicSourceBody
    {
public:
    static gint Create(TMSMicSourceBody*& bodyimpl);
    virtual ~TMSMicSourceBodyImpl();

    // From TMSMicSourceBody begins
    virtual gint GetType(TMSSourceType& sourcetype);

    void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSMicSourceBodyImpl();
    gint PostConstruct();

private:
    TMSCallProxy* iProxy;
    };

} //namespace TMS

#endif // TMS_MIC_SOURCE_BODY_IMPL_H

// End of file
