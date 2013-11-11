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

#ifndef TMS_STREAM_BODY_H
#define TMS_STREAM_BODY_H

#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSStreamObserver;
class TMSSource;
class TMSSink;
class TMSFormat;
class TMSEffect;
class TMSBuffer;

// Class declaration
class TMSStreamBody
    {
public:
    virtual ~TMSStreamBody() {}

    virtual gint AddObserver(TMSStreamObserver& obsrvr,
            gpointer user_data) = 0;
    virtual gint RemoveObserver(TMSStreamObserver& obsrvr) = 0;
    virtual gint AddSource(TMSSource* source) = 0;
    virtual gint RemoveSource(TMSSource* source) = 0;
    virtual gint AddSink(TMSSink* sink) = 0;
    virtual gint RemoveSink(TMSSink* sink) = 0;
    virtual gint SetFormat(TMSFormat* format) = 0;
    virtual gint ResetFormat(TMSFormat* format) = 0;
    virtual gint AddEffect(TMSEffect* effect) = 0;
    virtual gint RemoveEffect(TMSEffect* effect) = 0;
    virtual gint GetState() = 0;
    virtual gint GetStreamType() = 0;
    virtual gint GetStreamId() = 0;
    virtual gint Init(gint retrytime) = 0;
    virtual gint Pause() = 0;
    virtual gint Start(gint retrytime) = 0;
    virtual gint Stop() = 0;
    virtual void Deinit() = 0;
    };

} //namespace TMS

#endif //TMS_STREAM_BODY_H

