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

#ifndef TMSASYNCREQOBSRV_H
#define TMSASYNCREQOBSRV_H

namespace TMS {

// CLASS DECLARATION

/**
 *  Virtual interface to active object handling asynchronous requests.
 */
NONSHARABLE_CLASS(TMSAsyncReqObsrv)
    {
public:
    /**
     * Allows to gracefully handle error on leave during RunL operation.
     */
    virtual void HandleError(TInt aError) = 0;

    /**
     * Calls method handling asynchronous request.
     */
    virtual void HandleAsyncReq() = 0;
    };

} //namespace TMS

#endif // TMSASYNCREQOBSRV_H

//End of file
