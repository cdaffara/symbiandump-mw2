/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Abstract base class for WV engine service APIs.
*
*/



#ifndef M_simpleowncallback_H
#define M_simpleowncallback_H


// INCLUDES
#include <e32base.h>

class MSimpleEngineRequest;


/**
 *  Abstarct API for SIMPLE engine core and SIMPLE SIP LIB communication.
 *
 *  @lib simpleengine
 *  @since S60 v3.2
 */
class MSimpleOwnCallback
    {
public:

    /**
     * Complete the open client request
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus result status
     * @param aReq client request
     */
    virtual void Complete( TInt aOpId, TInt aStatus,
        MSimpleEngineRequest& aReq ) = 0;

    };

#endif

// End of File
