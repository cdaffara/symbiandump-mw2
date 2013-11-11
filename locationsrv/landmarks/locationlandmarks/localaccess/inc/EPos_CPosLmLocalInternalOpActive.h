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
* Description: Active help object for running an internal operation.
*
*
*/


#ifndef CPOSLMLOCALINTERNALOPACTIVE_H
#define CPOSLMLOCALINTERNALOPACTIVE_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS
class MPosLmLocalInternalOpExecutor;
class CPosLmOperation;

// CLASS DECLARATION

/**
* Active help object for running an internal operation.
*/
NONSHARABLE_CLASS(CPosLmLocalInternalOpActive) : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aExecutor An internal operation executor.
        * @param aOp An internal operation.
        */
        static CPosLmLocalInternalOpActive* NewL(
        /* IN */        MPosLmLocalInternalOpExecutor& aExecutor,
        /* IN */        CPosLmOperation* aOp
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalInternalOpActive();

     public: // New functions

        /**
        * Performs a single step of the internal operation asynchronously using
        * this active object.
        *
        * @param aClientStatus Client status; will be sent back to the executor
        *   when the asynchronous call to the internal operation completes.
        * @param aClientProgress Client progress; will be sent back to the
        *   executor when the asynchronous call to the internal operation
        *   completes.
        */
        void DoNext(
        /* IN */        TRequestStatus& aClientStatus,
        /* IN */        TReal32& aClientProgress
        );

        /**
        * Performs a single step of the internal operation without using this
        * active object to run it.
        *
        * @param aStatus A request status.
        * @param aProgress Progress of the internal operation.
        */
        void InternalOpNextStep(
        /* IN */        TRequestStatus& aStatus,
        /* IN */        TReal32& aProgress
        );

    public: // Functions from base classes

        /**
        * From CActive.
        *
        * Implements cancellation of an outstanding request.
        */
        void DoCancel();

        /**
        * From CActive.
        *
        * Handles request completion event.
        */
        void RunL();

    private:

        /**
        * C++ default constructor.
        * @param aExecutor An internal operation executor.
        * @param aOp An internal operation.
        */
        CPosLmLocalInternalOpActive(
        /* IN */        MPosLmLocalInternalOpExecutor& aExecutor,
        /* IN */        CPosLmOperation* aOp
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalInternalOpActive(
            const CPosLmLocalInternalOpActive&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalInternalOpActive& operator=(
            const CPosLmLocalInternalOpActive&);

    private:    // Data

        // Internal op executor
        MPosLmLocalInternalOpExecutor* iExecutor;

        // Internal op
        CPosLmOperation* iOp;

        TReal32 iProgress;

         // Client status
        TRequestStatus* iClientStatus;

        // Client progress
        TReal32* iClientProgress;

    };

#endif      // CPOSLMLOCALINTERNALOPACTIVE_H

// End of File
