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

#ifndef TMS_SPEAKER_SINK_BODY_H
#define TMS_SPEAKER_SINK_BODY_H

#include <tms.h>

namespace TMS {

// TMSSpeakerSinkBody class
class TMSSpeakerSinkBody
    {
public:
    virtual ~TMSSpeakerSinkBody() {}
    virtual gint GetType(TMSSinkType& sinktype) = 0;
    };

} //namespace TMS

#endif //TMS_SPEAKER_SINK_BODY_H