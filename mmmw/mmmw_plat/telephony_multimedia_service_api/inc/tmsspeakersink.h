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

#ifndef TMS_SPEAKER_SINK_H
#define TMS_SPEAKER_SINK_H

#include <tms.h>
#include <tmssink.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSSpeakerSinkBody;

/**
 * TMSSpeakerSink class
 *
 * This class instantiates TMS speaker sink object.
 *
 * @lib tmsapi.lib
 *
 */
class TMSSpeakerSink : public TMSSink
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSSpeakerSink();

    /**
     * Return sink type.
     *
     * This function can be called at any time.
     *
     * @param  sinktype
     *      The type of sink object (TMS_SINK_SPEAKER).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSSinkType& sinktype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSSpeakerSink();

protected:
    TMSSpeakerSinkBody *iBody;
    };

} //namespace TMS

#endif //TMS_SPEAKER_SINK_H

// End of file

