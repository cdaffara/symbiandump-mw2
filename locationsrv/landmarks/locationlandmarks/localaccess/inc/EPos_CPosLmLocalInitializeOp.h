/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for initializing a landmark database.
*
*
*/



#ifndef CPOSLMLOCALINITIALIZEOP_H
#define CPOSLMLOCALINITIALIZEOP_H

#include <EPos_CPosLmOperation.h>
#include <epos_poslmservercommon.h>
#include "EPos_TPosOperationStatus.h"

class CPosLmLocalDatabase;

/**
* Operation for initializing a landmark database. This operation can be
* either synchronous or asynchronous.
*
*  @lib eposlmlocalaccess.lib
*/
NONSHARABLE_CLASS(CPosLmLocalInitializeOp) : public CPosLmOperation
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param[in] aDb A landmark database.
        */
        static CPosLmLocalInitializeOp* NewL( CPosLmLocalDatabase& aDb );

        virtual ~CPosLmLocalInitializeOp();

    public:  // Data types

        /**
        *  Specifies the type of the initialize operation.
        */
        enum TInitializeType
            {
            EInitialize = 0,
            ELangSwitch = 1,
            EIndexBuild = 2,
            ERecover = 4
            };

    public: // From CPosLmOperation.

        /*
        * This function sends an asynchronous message to the Landmarks Server.
        * Upon the first call to this function, the server starts to initialize
        * the database. The server then continues to initialize the database
        * even if this function is never called again. Subsequent calls to this
        * function will report progress.
        *
        * @param[out] aStatus The request status. Will be completed when the step
        *   has been performed. The request status will be @p KRequestPending
        *   if the step has not completed. It will be
        *   @p KPosLmOperationNotComplete if the step has completed but more
        *   steps are needed before the operation has finished, The request
        *   status will be @p KErrNone if the operation has finished
        *   successfully. The status will be set to an error code if the
        *   operation has failed.
        * @param[out] aProgress Will be set to he progress of the operation when the
        *   step has finished.
        */
        void NextStep( TRequestStatus& aStatus, TReal32& aProgress );

        /**
        * Synchonous execution of the database initialization.
        * When this function returns, the operation has finished.
        */
        void ExecuteL();

    private:

        /**
        * C++ default constructor.
        * @param[in] aDb A landmark db.
        */
        CPosLmLocalInitializeOp( CPosLmLocalDatabase& aDb );

        void ConstructL();

        void CompleteRequest(TRequestStatus& aStatus, TInt aReason);

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmLocalInitializeOp(const CPosLmLocalInitializeOp&);
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmLocalInitializeOp& operator=(const CPosLmLocalInitializeOp&);

    private:    // Data

        // Landmark database (no ownership).
        CPosLmLocalDatabase* iDb;

        // Init type
        TInitializeType iInitType;

        TBool iInitNotNeeded;

        TPosOperationStatus iOperationStatus;

        // Receive package for the status
        TPckgBuf<TInt> iStatusPckg;

        // Receive package for the progress
        TPckg<TReal32>* iProgressPckg;

        TPckg<TUint>* iInitFlagPckg;
        TUint* iInitFlag;
    };

#endif      // CPOSLMLOCALINITIALIZEOP_H

