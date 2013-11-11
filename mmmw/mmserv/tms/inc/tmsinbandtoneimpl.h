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

#ifndef TMS_INBAND_IMPL_H
#define TMS_INBAND_IMPL_H

#include <tms.h>
#include <tmsinbandtone.h>

namespace TMS {

// TMSDTMFImpl class
class TMSInbandToneImpl : public TMSInbandTone
    {
public:
    virtual ~TMSInbandToneImpl();
    IMPORT_C static gint Create(TMSInbandTone*& inbandtone);
    IMPORT_C static gint Delete(TMSInbandTone*& inbandtone);

private:
    TMSInbandToneImpl();
    gint PostConstruct();
    gint SetParent(TMSInbandTone*& parent);

private:
    TMSInbandToneType iInbandToneType;
    };

} //namespace TMS

#endif
