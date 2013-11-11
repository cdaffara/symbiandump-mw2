/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provides asyncronous waiter.
*
*/



#ifndef SENDUIOPERATIONWAIT_H
#define SENDUIOPERATIONWAIT_H

//  INCLUDES
class CBaseMtm;
class CAknInputBlock;

/**
 *  CSendUiOperationWait
 *
 * We cannot use CMuiuOperationWait because it calls
 * CAknEnv::StopSchedulerWaitWithBusyMessage. When e.g. New message
 * pop-up is opened from Phone idle while we are waiting previous
 * editor launch completion, a Busy dialog is shown and it cannot
 * be closed at all.
 *
 *  This class is otherwise same as CMuiuOperationWait but iWait.AsyncStop
 *  is called instead of CAknEnv::StopSchedulerWaitWithBusyMessage.
 *
 *  @lib senduidatautils
 *  @since S60 S60 v3.0
 */

NONSHARABLE_CLASS (CSendUiOperationWait) : public CActive
    {

public:
  
    IMPORT_C static CSendUiOperationWait* NewLC(TInt aPriority = EPriorityStandard);

    virtual ~CSendUiOperationWait();

    /**
     * Starts active wait operation
     *
     * @since S60 v3.1
     * @param aOperation CActive derived operation, that is running
     * @return ETrue, if wait was not interrupted. Otherwise EFalse.
     */
    IMPORT_C TBool Start( CActive* aOperation );

    /**
     * Starts active wait operation
     *
     * @since S60 v3.1
     * @param aOperation CBaseMtmOperation derived operation, that is 
     *                   running
     * @return ETrue, if wait was not interrupted. Otherwise EFalse.
     */
    IMPORT_C TBool Start( CBaseMtm* aBaseMtmOperation );
    
    /**
     * Starts active wait operation
     *
     * Use this, if the operation to be waited can not be canceled.
     * Prefer using the above Start methods.
     *
     * @since S60 v3.0
     * @return ETrue, For compatibility returns always.
     */
    IMPORT_C TBool Start();

protected: // from base class CActive

    CSendUiOperationWait(TInt aPriority);

    void RunL();

    void DoCancel();

private:

    void ConstructL();
    
protected:

    /**
     * Waiter
     */
    CActiveSchedulerWait iWait;

    /**
     * Inputblocker harvests exit events.
     * Own.
     */
    CAknInputBlock* iInputBlock;

private:

    /**
     * This is ETrue, if operation has not been interrupted.
     */
    TBool iSuccess;

    /**
     * Operation, which can be canceled
     * Not own.
     */
    CActive* iOperation;

    /**
     * Operation, which can be canceled
     * Not own.
     */
    CBaseMtm* iBaseMtmOperation;
    };

#endif

// End of file
