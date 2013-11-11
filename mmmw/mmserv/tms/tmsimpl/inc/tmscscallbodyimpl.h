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

#ifndef TMS_CS_CALL_BODY_IMPL_H
#define TMS_CS_CALL_BODY_IMPL_H

#include <tms.h>
#include "tmscallbody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSStream;
class TMSDTMF;
class TMSRingTone;
class TMSCallProxy;

typedef std::vector<TMSStream*> StreamsVector;
typedef std::vector<TMSCallProxy*> SessionsVector;

// TMSCSCallBodyImpl class
class TMSCSCallBodyImpl : public TMSCallBody
    {
public:
    static gint Create(TMSCallBody*& bodyimpl);
    virtual ~TMSCSCallBodyImpl();

    // From TMSCallBody
    virtual TMSCallType GetCallType();
    virtual gint GetCallContextId(guint& ctxid);
    virtual gint CreateStream(TMSStreamType type, TMSStream*& strm);
    virtual gint DeleteStream(TMSStream*& strm);

private:
    TMSCSCallBodyImpl();
    gint PostConstruct();

    gint AddStreamToList(TMSStream* strm);
    gint RemStreamFromList(TMSStream*& strm);

private:
    TMSCallProxy* iSession;
    StreamsVector iStreamsVector;
    SessionsVector iSessionsVector;
    };

} //namespace TMS

#endif // TMS_CS_CALL_BODY_IMPL_H

// End of file
