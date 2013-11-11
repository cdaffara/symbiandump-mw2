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

#ifndef TMS_SPEAKER_SINK_IMPL_H
#define TMS_SPEAKER_SINK_IMPL_H

#include <glib.h>
#include <tmsspeakersink.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallProxy;

// TMSSpeakerSinkImpl class
class TMSSpeakerSinkImpl : public TMSSpeakerSink
    {
public:
    virtual ~TMSSpeakerSinkImpl();
    static gint Create(TMSSink*& tmssink);
    gint SetProxy(TMSCallProxy* aProxy, gpointer queuehandler);

private:
    TMSSpeakerSinkImpl();
    gint PostConstruct();
    };

} //namespace TMS

#endif // TMS_SPEAKER_SINK_IMPL_H

// End of file
