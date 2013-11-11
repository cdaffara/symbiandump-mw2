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
* Description:A proxy operation between the client and another operation.
*
*/


#ifndef CPOSLMLOCALINTERNALPROXYOP_H
#define CPOSLMLOCALINTERNALPROXYOP_H

//  INCLUDES
#include <EPos_CPosLmOperation.h>

// FORWARD DECLARATION

// CLASS DECLARATION
class MPosLmLocalInternalProxyOpClient;

/**
* A proxy operation between the client and another operation.
* This operation redirects client calls to NextStep and ExecuteL to
* the other operation's DoNextStep and DoExecuteL.
*/
NONSHARABLE_CLASS(CPosLmLocalInternalProxyOp) : public CPosLmOperation
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aOpClient Reference to a proxy client operation.
        * @param aOp Pointer to a proxy client operation.
        *   Ownership transferred. Usually the same object as aOpClient.
        * @return A new instance of this class.
        */
        static CPosLmLocalInternalProxyOp* NewL(
        /* IN  */       MPosLmLocalInternalProxyOpClient& aOpClient,
        /* IN  */       CPosLmOperation* aOp
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmLocalInternalProxyOp();

    public: // New functions

        /**
        * Returns a pointer to the operation. Ownership is not transferred.
        */
        CPosLmOperation* Operation();

    public: // From base classes

        /**
        * From CPosLmOperation.
        *
        * @param aStatus The request status. Will be completed when the step
        *   has been performed. The request status will be @p KRequestPending
        *   if the step has not completed. It will be
        *   @p KPosLmOperationNotComplete if the step has completed but more
        *   steps are needed before the operation has finished. The request
        *   status will be @p KErrNone if the operation has finished
        *   successfully. The status will be set to an error code if the
        *   operation has failed.
        * @param aProgress Will be set to the progress of the operation when the
        *   step has finished.
        */
        void NextStep(
        /* OUT */   TRequestStatus& aStatus,
        /* OUT */   TReal32& aProgress
        );

        /**
        * From CPosLmOperation.
        */
        void ExecuteL();

    private:

        /**
        * C++ default constructor.
        *
        * @param aOpClient Reference to a proxy client operation.
        * @param aOp Pointer to a proxy client operation.
        *   Ownership transferred. Usually the same object as aOpClient.
        */
        CPosLmLocalInternalProxyOp(
        /* IN  */       MPosLmLocalInternalProxyOpClient& aOpClient,
        /* IN */        CPosLmOperation* aOp
        );

        // By default, prohibit copy constructor
        CPosLmLocalInternalProxyOp(
            const CPosLmLocalInternalProxyOp& );
        // Prohibit assigment operator
        CPosLmLocalInternalProxyOp& operator= (
            const CPosLmLocalInternalProxyOp& );

    private: // Data

        MPosLmLocalInternalProxyOpClient* iOpClient;
        CPosLmOperation* iOp;

    };

#endif      // CPOSLMLOCALINTERNALPROXYOP_H

// End of File
