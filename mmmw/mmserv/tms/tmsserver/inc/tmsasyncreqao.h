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

#ifndef TMSASYNCREQAO_H
#define TMSASYNCREQAO_H

//INCLUDES
#include <e32base.h>
#include "tmsasyncreqobsrv.h"

namespace TMS {

// CLASS DECLARATION

/**
 *  Active object handling asynchronous RingTone requests.
 */
NONSHARABLE_CLASS(TMSAsyncReqAO) : public CActive
    {
public:

    /**
     * Two-phased constructor.
     */
    static TMSAsyncReqAO* NewL(TMSAsyncReqObsrv* aObserver);

    /**
     * Destructor.
     */
    virtual ~TMSAsyncReqAO();

    /**
     * Starts A/O.
     */
    void Start();

protected:
    // From CActive

    /**
     * Cancels asyncronous request(s).
     */
    void DoCancel();

    /**
     * Standard RunError() method
     * @param
     *      aError RunL leave code.
     *
     * @return KErrNone.
     */
    TInt RunError(TInt aError);

    /**
     * Standard RunL() method
     */
    void RunL();

private:

    /**
     * C++ default constructor.
     */
    TMSAsyncReqAO(TMSAsyncReqObsrv* aObserver);

private:
    //Data

    // Pointer to the observer
    TMSAsyncReqObsrv* iObserver;
    };

} //namespace TMS

#endif // TMSASYNCREQAO_H

// End of File
