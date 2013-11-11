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

#ifndef TMS_STREAM_IMPL_H
#define TMS_STREAM_IMPL_H

#include <tms.h>
#include <tmsstream.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSStreamImpl class
class TMSStreamImpl : public TMSStream
    {
public:
    virtual ~TMSStreamImpl();
    static gint Create(TMSCallType callType, TMSStreamType ctype,
            TMSCallProxy* proxy, TMSStream*& tmsstream);
    static gint Delete(TMSStream*& tmsrt);

private:
    TMSStreamImpl();
    gint PostConstruct(TMSCallType callType, TMSStreamType ctype,
            TMSCallProxy* proxy);
    };

} //namespace TMS

#endif // TMS_STREAM_IMPL_H

