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
* Description: Declaration of Active object wrapping an async operation.
*
*
*/


#ifndef CPOSLMACTIVEOPERATION_H
#define CPOSLMACTIVEOPERATION_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CPosLmOperation;

// CLASS DECLARATION

/**
* Active object wrapping an async operation.
*
*/
NONSHARABLE_CLASS(CPosLmActiveOperation) : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aOperation An operation object to execute.
        * @param aDatabaseIndex The index of the database connected to the
        *   operation to execute.
        * @param aOwner The owner of this active operation.
        * @return A new instance of this class.
        */
        static CPosLmActiveOperation* NewL(
        /* IN */        CPosLmOperation* aOperation,
        /* IN */        TUint aDatabaseIndex,
        /* IN */        TAny* aOwner
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmActiveOperation();

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

    public: // New functions

        /*
        * Execute the next step of the operation object.
        *
        * @param callBack The function that will be called when the next step
        *   has been executed.
        */
        void ExecuteStep(TCallBack callBack);

        /*
        * Returns the index of the database connected to the executed
        * operation.
        *
        * @return The database index.
        */
        TUint DatabaseIndex();

        /*
        * Returns the owner of this object.
        *
        * @return A pointer to the owner.
        */
        TAny* Owner();

        /*
        * Returns the status of the executed operation.
        *
        * @return The status. The status will be @p KRequestPending if the step
        *   has not completed. It will be @p KPosLmOperationNotComplete if the
        *   step has completed but more steps are needed before the operation
        *   will be completed. The status will be @p KErrNone if the operation
        *   has finished successfully. The status will be set to an error code
        *   if the operation has failed.
        */
        TInt Status();

        /*
        * Returns the progress of the executed operation.
        *
        * @return The progress. Progress is a floating point number in the
        *   interval[0.0, 1.0]. 0.0 indicates that the operation has not
        *   started and 1.0 indicates that the operation has completed.
        */
        TReal32 Progress();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmActiveOperation(
        /* IN */        CPosLmOperation* aOperation,
        /* IN  */       TUint aDatabaseIndex,
        /* IN */        TAny* aOwner
        );

    private:    // Data

        CPosLmOperation*    iOperation;
        TUint               iDatabaseIndex;
        TAny*               iOwner;

        TCallBack           iCallBack;
        TReal32             iProgress;
    };

#endif      // CPOSLMACTIVEOPERATION_H

// End of File
