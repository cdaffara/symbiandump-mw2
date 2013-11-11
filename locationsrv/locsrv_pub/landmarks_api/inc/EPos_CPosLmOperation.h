/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPosLmOperation class
*
*/


#ifndef CPOSLMOPERATION_H
#define CPOSLMOPERATION_H

#include <e32base.h>

class CPosLmOperation;

/**
*  Executes the operation synchronously and then deletes it.
*
*  All functions which return a @ref CPosLmOperation object can be run
*  incrementally. If the client is not interested in running it
*  incrementally, it can call @ref CPosLmOperation::ExecuteL to run the
*  whole operation in one batch. This can be simplified by using
*  @ref ExecuteAndDeleteLD. It runs the operation and deletes the object
*  when it is done.
*
*  For instance, to use this function for
*  @ref CPosLandmarkDatabase::InitializeL, the client would call
*
*  ExecuteAndDeleteLD( database->InitializeL() );
*
*  The operation object should not be on the cleanup stack when this function
*  is called.
*
*  @since S60 3.0
*  @param aOperation The operation to handle. It will be deleted when the
*   function returns.
*/
inline void ExecuteAndDeleteLD( CPosLmOperation* aOperation );


/**
*  Base class for handles to landmark operations.
*
*  Long running operations in the Landmarks API returns an object of this class
*  so that the client can run it incrementally and check the progress of the
*  operation.
*
*  The operation must explicitly be run by the client. The operation can
*  be run incrementally by calling @ref NextStep until it does not return
*  the status @p KPosLmOperationNotComplete. Alternately the operation can be
*  run synchronously by calling @ref ExecuteL.
*
*  @ref ExecuteAndDeleteLD can be used to handle the operation object if
*  it is run in synchronous mode.
*
*  @lib eposlandmarks.lib
*  @since S60 3.0
*/
class CPosLmOperation : public CBase
    {
    public:

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPosLmOperation();

    public:

        /**
        * Incremental execution of the operation.
        *
        * The client should use an active object and call this function until
        * it does not complete with status @p KPosLmOperationNotComplete.
        *
        * When the operation has finished, this function will complete with
        * status @p KErrNone if the operation was successful, or an error code
        * if some error was encountered.
        *
        * This function also returns a progress of the operation. Progress is a
        * floating point number in the interval [0.0,1.0]. 0.0 indicates that
        * the operation has not started and 1.0 indicates that the operation
        * has completed.
        *
        * Note that this function is asynchronous which means that status and
        * progress may not be set when the function returns. They are only
        * guaranteed to be set when the request is completed.
        *
        * The only way to cancel an operation is to delete the operation
        * object. This will also cancel any outstanding request to
        * NextStep().
        *
        * @param[out] aStatus The request status. Upon request completion contains
        *   step status: 
        *   - @p KPosLmOperationNotComplete if the step has completed but more
        *   steps are needed before the operation will be completed. 
        *   - @p KErrNone if the operation has finished successfully. 
        *   - An error code if the operation has failed.
        * @param[out] aProgress Upon request completion is set to the progress 
        *   of the operation.
        *
        * @panic "Landmarks Client"-EPosInvalidOperationMode 
        *   The function is called when the operation is already complete.
        */
        virtual void NextStep(
            TRequestStatus& aStatus,
            TReal32& aProgress
        ) = 0;

        /**
        * Synchronous execution of the operation.
        *
        * When this function returns, the operation has finished.
        *
        * @panic "Landmarks Client"-EPosInvalidOperationMode 
        *   -# This function is called when the operation is already complete
        *   -# The operation has already been started incrementally using @ref NextStep().
        */
        virtual void ExecuteL() = 0;

    protected:

        // C++ constructor.
        IMPORT_C CPosLmOperation();

    private:

        // Prohibit copy constructor
        CPosLmOperation( const CPosLmOperation& );
        // Prohibit assigment operator
        CPosLmOperation& operator= ( const CPosLmOperation& );

    };

#include "EPos_CPosLmOperation.inl"

#endif      // CPOSLMOPERATION_H


