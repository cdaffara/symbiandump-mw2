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

#ifndef TMS_CALL_BODY_H
#define TMS_CALL_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSStream;
class TMSDTMF;
class TMSRingTone;

// Class declaration
class TMSCallBody
    {
public:
    virtual ~TMSCallBody() {}
    virtual TMSCallType GetCallType() = 0;
    virtual gint GetCallContextId(guint& ctxid) = 0;
    virtual gint CreateStream(const TMSStreamType type, TMSStream*& strm) = 0;
    virtual gint DeleteStream(TMSStream*& strm) = 0;
    };

} //namespace TMS

#endif // TMS_CALL_BODY_H

// End of file
