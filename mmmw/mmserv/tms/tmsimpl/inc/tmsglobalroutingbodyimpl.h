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

#ifndef TMS_GLOBAL_ROUTING_BODY_IMPL_H
#define TMS_GLOBAL_ROUTING_BODY_IMPL_H

#include <tms.h>
#include "tmsglobalroutingbody.h"
#include "tmsqueuehandler.h"
#include "tmsclientserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSGlobalRoutingObserver;
class TMSGlobalRouting;
class TMSProxy;

// TMSGlobalRoutingBodyImpl class
class TMSGlobalRoutingBodyImpl : public TMSGlobalRoutingBody
    {
public:
    static gint Create(TMSGlobalRoutingBody*& bodyimpl);
    virtual ~TMSGlobalRoutingBodyImpl();

    // From TMSGlobalRoutingBody
    virtual gint AddObserver(TMSGlobalRoutingObserver& obsrvr,
            gpointer user_data);
    virtual gint RemoveObserver(TMSGlobalRoutingObserver& obsrvr);
    virtual gint SetOutput(const TMSAudioOutput output);
    virtual gint GetOutput(TMSAudioOutput& output);
    virtual gint GetPreviousOutput(TMSAudioOutput& output);
    virtual gint GetAvailableOutputs(OutputVector& outputs);

    void SetParent(TMSGlobalRouting*& parent);

private:
    TMSGlobalRoutingBodyImpl();
    gint PostConstruct();

private:
    TMSGlobalRoutingObserver* iObserver;
    gint iClientId;
    TPckgBuf<TInt> iIntPckg;
    RPointerArray<TMSGlobalRoutingObserver> iObserversList; // (not owned)
    gpointer iUserData;
    TMSProxy* iProxy;
    TMSGlobalRouting* iParent;
    };

} //namespace TMS

#endif // TMS_GLOBAL_ROUTING_BODY_IMPL_H

