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

#ifndef TMS_GLOBAL_ROUTING_BODY_H
#define TMS_GLOBAL_ROUTING_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalRoutingObserver;

// CLASS DECLARATION
class TMSGlobalRoutingBody
    {
public:
    virtual ~TMSGlobalRoutingBody() {}

    virtual gint AddObserver(TMSGlobalRoutingObserver& obsrvr,
            gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSGlobalRoutingObserver& obsrvr) = 0;
    virtual gint SetOutput(const TMSAudioOutput output) = 0;
    virtual gint GetOutput(TMSAudioOutput& output) = 0;
    virtual gint GetPreviousOutput(TMSAudioOutput& output) = 0;
    virtual gint GetAvailableOutputs(OutputVector& outputs) = 0;
    };

} //namespace TMS

#endif //TMS_GLOBAL_ROUTING_BODY_H
