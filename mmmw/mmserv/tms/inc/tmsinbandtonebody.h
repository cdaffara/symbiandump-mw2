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

#ifndef TMS_INBAND_BODY_H
#define TMS_INBAND_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSInbandToneObserver;

// CLASS DECLARATION
class TMSInbandToneBody
    {
public:
    virtual ~TMSInbandToneBody() {}

    virtual gint AddObserver(TMSInbandToneObserver& obsrvr, gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSInbandToneObserver& obsrvr) = 0;
    virtual gint Start(TMSInbandToneType inbandtype) = 0;
    virtual gint Stop() = 0;

    };

} //namespace TMS

#endif //TMS_INBAND_BODY_H
