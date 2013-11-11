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

#ifndef TMS_DTMF_IMPL_H
#define TMS_DTMF_IMPL_H

#include <tms.h>
#include <tmsdtmf.h>

namespace TMS {

// TMSDTMFImpl class
class TMSDTMFImpl : public TMSDTMF
    {
public:
    virtual ~TMSDTMFImpl();
    IMPORT_C static gint Create(TMSStreamType streamtype, TMSDTMF*& dtmf);
    IMPORT_C static gint Delete(TMSDTMF*& dtmf);

private:
    TMSDTMFImpl();
    gint PostConstruct(TMSStreamType streamtype);
    };

} //namespace TMS

#endif //TMS_DTMF_IMPL_H

