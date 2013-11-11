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

#ifndef TMS_SPEAKER_SINK_BODY_IMPL_H
#define TMS_SPEAKER_SINK_BODY_IMPL_H

#include <tms.h>
#include "tmsspeakersinkbody.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSSpeakerSinkBodyImpl class
class TMSSpeakerSinkBodyImpl : public TMSSpeakerSinkBody
    {
public:
    static gint Create(TMSSpeakerSinkBody*& bodyimpl);
    virtual ~TMSSpeakerSinkBodyImpl();

    // From TMSSpeakerSinkBody
    virtual gint GetType(TMSSinkType& sinktype);

    void SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSSpeakerSinkBodyImpl();
    gint PostConstruct();

private:
    TMSCallProxy* iProxy;
    };

} //namespace TMS

#endif // TMS_SPEAKER_SINK_BODY_IMPL_H

// End of file
