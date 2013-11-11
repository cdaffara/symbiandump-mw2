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
 * Description: This class provides callback mechanism for TMSClientSource
 * listeners.
 *
 */

#ifndef TMS_CLIENT_SOURCE_OBSRVR_H
#define TMS_CLIENT_SOURCE_OBSRVR_H

// Include files
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSBuffer;

/**
 * TMSClientSourceObserver class
 *
 * This class provides callback mechanism for TMSClientSource listeners.
 *
 * Client class that wants to send data to TMS must inherit from
 * TMSClientSourceObserver class and register itself with the TMSBuffer and
 * TMSClientSource objects.
 *
 * Functions declared in this class are called upon TMSBuffer events.
 *
 * The client must not do any blocking, or perform any CPU-intensive operations
 * in the callback methods. It is recommended that the client saves event data,
 * returns from this function immediately and if necessary, responds to
 * the event in asynchronous way.
 *
 * @lib tmsapi.lib
 *
 */
class TMSClientSourceObserver
    {
public:
    /**
     * This method is called by the TMS when it wants data from the client
     * and provides buffer to be filled by the client in the PULL MODE.
     *
     * @param  buffer
     *      Buffer to be filled by the client.
     *
     */
    virtual void FillBuffer(TMSBuffer& buffer) = 0;

    /**
     * This method is called by the TMS server in the PUSH MODE to indicate
     * previously submitted data buffer has been processed and more data can
     * now be accepted for processing. The processed buffer is returned back
     * to the client.
     *
     * @param  buffer
     *      Buffer which has been processed and returned to the client.
     *
     * @param  reason
     *      The status of the operation.
     *
     */
    virtual void BufferProcessed(const TMSBuffer* buffer, gint reason) = 0;
    };

} //namespace TMS

#endif // TMS_CLIENT_SOURCE_OBSRVR_H

// End of file
