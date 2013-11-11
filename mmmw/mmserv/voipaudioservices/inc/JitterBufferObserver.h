/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Observer for Jitter buffer.
 *
 */

#ifndef JITTERBUFFEROBSERVER_H
#define JITTERBUFFEROBSERVER_H

class MJitterBufferObserver
    {
public:
    enum TJBEvent
        {
        EGeneralError,
        EBufferUnderflow,
        EBufferOverflow,
        EBufferConsumed,
        EBufferReadyToPlay,
        EConcealErrorForNextBuffer
        };

    /**
     * Callback function to receive jitter buffer error events.
     */
    virtual void EventJB(TInt aEventType, TInt aError = KErrNone) = 0;

    /**
     * Callback function for codec reconfiguration for dynamic
     * buffer size adjustment.
     * @param aBufferSize Buffer size request in samples.
     *
     virtual void DynamicBufferChangeRequest(TInt aBufferSize) = 0;
     */
    };

#endif //JITTERBUFFEROBSERVER_H

// End of file
