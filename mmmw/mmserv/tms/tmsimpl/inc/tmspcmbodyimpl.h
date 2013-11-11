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

#ifndef TMS_PCM_FORMAT_BODY_IMPL_H
#define TMS_PCM_FORMAT_BODY_IMPL_H

#include <tms.h>
#include "tmsformatbodyimpl.h"
#include "tmspcmformatbody.h"
#include "tmsqueuehandler.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSFormatObserver;
class TMSCallProxy;

// TMSPCMFormatBodyImpl class
class TMSPCMFormatBodyImpl : public TMSPCMFormatBody,
                             public TMSFormatBodyImpl
    {
public:
    static gint Create(TMSPCMFormatBody*& bodyimpl);
    virtual ~TMSPCMFormatBodyImpl();

    // From TMSPCMFormatBody
    virtual gint GetType(TMSFormatType& Formattype);


    virtual void SetProxy(TMSGlobalContext* context, gpointer queuehandler);

private:
    TMSPCMFormatBodyImpl();
    gint PostConstruct();

private:
    TMSFormatObserver* iObserver;
    gpointer iUserData;
    TMSCallProxy* iProxy;
    TMSStreamType iStreamType;
    };

} //namespace TMS

#endif // TMS_PCM_FORMAT_BODY_IMPL_H

// End of file
