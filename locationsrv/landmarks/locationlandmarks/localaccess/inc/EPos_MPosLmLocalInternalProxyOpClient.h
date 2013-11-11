/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Interface for proxy operations
*
*
*/


#ifndef MPOSLMLOCALINTERNALPROXYOPCLIENT_H
#define MPOSLMLOCALINTERNALPROXYOPCLIENT_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

/**
* Interface for proxy operations, which use this interface to redirect incoming
* calls from NextStep and ExecuteL to DoNextStep and DoExecuteL respectively.
*
* This interface is implemented by operation classes which provide another
* way to perform the operation in asynchronous steps or in one single
* synchronous step, other than using NextStep and ExecuteL (from
* CPosLmOperation).
*/
class MPosLmLocalInternalProxyOpClient
    {

     public:  // New functions

        /**
        * Performs the next step of the operation asynchronously.
        *
        * @param aStatus A request status.
        * @param aProgress The operation progress
        */
        virtual void DoNextStep(
        /* OUT */       TRequestStatus& aStatus,
        /* OUT */       TReal32& aProgress
        ) = 0;

        /**
        * Performs the operation synchronously.
        */
        virtual void DoExecuteL() = 0;

    };

#endif      // MPOSLMLOCALINTERNALPROXYOPCLIENT_H

// End of File
