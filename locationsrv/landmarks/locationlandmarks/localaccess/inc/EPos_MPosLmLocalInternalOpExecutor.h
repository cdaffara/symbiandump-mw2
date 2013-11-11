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
* Description: Interface for operations that executes an internal operation.
*
*
*/


#ifndef MPOSLMLOCALINTERNALOPEXECUTOR_H
#define MPOSLMLOCALINTERNALOPEXECUTOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS

/**
* Interface for operations that executes an internal operation.
*
* CPosLmLocalInternalOpActive uses this interface to tell the executor when
* an asynchronous step of the internal operation has completed.
*/
class MPosLmLocalInternalOpExecutor
    {

     public:  // New functions

        /**
        * Callback function, which tells when an asynchronous step of the
        * internal operation is completed.
        *
        * @param aStatus Internal operation complete status.
        * @param aProgress Internal operation progress.
        * @param aClientStatus Client request status.
        * @param aClientProgress Client progress.
        */
        virtual void InternalOpCompleted(
        /* IN */        TInt aStatus,
        /* IN */        TReal32 aProgress,
        /* IN/OUT */    TRequestStatus& aClientStatus,
        /* IN/OUT */    TReal32& aClientProgress
        ) = 0;

    };

#endif      // MPOSLMLOCALINTERNALOPEXECUTOR_H

// End of File
