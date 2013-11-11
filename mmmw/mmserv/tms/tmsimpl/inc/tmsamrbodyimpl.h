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

#ifndef TMS_AMR_FORMAT_BODY_IMPL_H
#define TMS_AMR_FORMAT_BODY_IMPL_H

#include <tms.h>
#include "tmsformatbodyimpl.h"
#include "tmsamrformatbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatObserver;
class TMSCallProxy;

// Class declaration
class TMSAMRFormatBodyImpl : public TMSAMRFormatBody,
                             public TMSFormatBodyImpl
    {
public:
    static gint Create(TMSAMRFormatBody*& bodyimpl);
    virtual ~TMSAMRFormatBodyImpl();

    // From TMSAMRFormatBody begins
    virtual gint GetType(TMSFormatType& Formattype);
    // From TMSAMRFormatBody ends

    virtual void SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSAMRFormatBodyImpl();
    gint PostConstruct();

private:
    TMSFormatObserver* iObserver;
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSStreamType iStreamType;
    };

} //namespace TMS

#endif // TMS_AMR_FORMAT_BODY_IMPL_H

// End of file
