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
* Description:  This file implements the Location triggering container
*                interface
*
*/


#include "lbtdboperationao.h"
#include "lbtdbtriggersmanager.h"
#include "lbtlogger.h"


// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtDbOperationAO* CLbtDbOperationAO::NewL(
    RDbDatabase& aDb,
    CLbtDbTriggersManager* aObserver)
    {
    CLbtDbOperationAO* self = new( ELeave ) CLbtDbOperationAO(aDb, aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    

// ---------------------------------------------------------------------------
// Execute a SQL query asynchronously
// ---------------------------------------------------------------------------
//
void CLbtDbOperationAO::ExecuteQuery(
    RDbView& aView,
    TDesC& aQuery)
    {
    FUNC_ENTER("CLbtDbOperationAO::ExecuteQuery");
    if (  IsActive() )
        {
        // We are already executing an asynchronous operation so return error
        TRAP_IGNORE( iObserver->DbSqlOperationCompletedL( KErrInUse ) );
        return;
        }
    
    iView = &aView;
    TInt err =iView->Prepare( iDb, TDbQuery(aQuery, EDbCompareFolded) );
    if( err != KErrNone )
 	   {
 	        TRequestStatus* status = &iStatus;
          User::RequestComplete(status, err);
     }
    iView->Evaluate(iStatus);
    SetActive();
    iOperation = EDbSqlQuery;
    }


// ---------------------------------------------------------------------------
// CLbtDbOperationAO::ExecuteSyncQuery
// ---------------------------------------------------------------------------
//
TInt CLbtDbOperationAO::ExecuteSyncQuery(
    RDbView& aView,
    TDesC& aQuery)
    {
    FUNC_ENTER("CLbtDbOperationAO::ExecuteSyncQuery");
    TInt err = aView.Prepare( iDb, TDbQuery(aQuery, EDbCompareFolded),KDbUnlimitedWindow );
    // This should evaluate the query fully
    if ( err == KErrNone )
        {
        err = aView.EvaluateAll();
        }
    return err;
    }

// ---------------------------------------------------------------------------
// Compact the database
// ---------------------------------------------------------------------------
//
void CLbtDbOperationAO::CompactDb()
    {
    FUNC_ENTER("CLbtDbOperationAO::CompactDb");
    if (  IsActive() )
        {
        // We are already executing an asynchronous operation so return error
        iObserver->DbIncrementalOperationCompleted( KErrInUse );
        return;
        }

    TInt error = iDbIncrOp.Compact(iDb, iSteps);
    if(error != KErrNone)
    	{
    	ERROR("iDbIncrOp.Compact returned %d", error);
    	iDbIncrOp.Close();
    	return;
    	}
    error = iDbIncrOp.Next( iSteps );
    iStatus = KRequestPending;
    SetActive();
    iOperation = EDbCompaction;
    iStatus = KRequestPending;
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, error);
    }

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
TBool CLbtDbOperationAO::IsCompactionOngoing()
	{
	if( EDbCompaction == iOperation )
		{
		return ETrue;
		}
	return EFalse;
	}


// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CLbtDbOperationAO::DoCancel()
    {
    FUNC_ENTER("CLbtDbOperationAO::DoCancel");
    switch( iOperation )
        {
        case EDbSqlQuery:
            iView->Cancel();
            break;
        case EDbCompaction:
            // Nothing to do here since the step is already completed.
            break;
        default:
            break;
        }
    iOperation = EDbOperationNone;
    }


// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CLbtDbOperationAO::RunL()
    {
    FUNC_ENTER("CLbtDbOperationAO::RunL");
    switch( iOperation )
        {
        case EDbSqlQuery:
            if ( iStatus.Int() > 0 )
                {
                iView->Evaluate(iStatus);
                SetActive();
                }
            else
                {
                iOperation = EDbOperationNone;
                iObserver->DbSqlOperationCompletedL( iStatus.Int() );
                }
            break;
        case EDbCompaction:
            if ( iStatus.Int() == KErrNone && iSteps )
                {
                // If the steps till now have been successful and if there
                // are more steps to be executed
                TInt error = iDbIncrOp.Next( iSteps );
                iStatus = KRequestPending;
                SetActive();
                TRequestStatus* status = &iStatus;
                User::RequestComplete(status, error);
                }
            else
                {
                // Either the operation was fully completed or
                // there was an error during database compaction.
                // So just close the incremental operation and
                // return the error (in case of the error).
                iDbIncrOp.Close();
                iOperation = EDbOperationNone;
                iObserver->DbIncrementalOperationCompleted( iStatus.Int() );
                }
            break;
        default:
            // Nothing to do in this case
            break;
        }
    }


// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//
TInt CLbtDbOperationAO::RunError(TInt /*aError*/)
    {
    FUNC_ENTER("CLbtDbOperationAO::RunError");
    // RunL doesn't have any Leaving calls. So no implementation here.
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CLbtDbOperationAO::~CLbtDbOperationAO()
    {
    FUNC_ENTER("CLbtDbOperationAO::~CLbtDbOperationAO");
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CLbtDbOperationAO::CLbtDbOperationAO(
    RDbDatabase& aDb,
    CLbtDbTriggersManager* aObserver)
    :CActive(CActive::EPriorityStandard),
     iDb(aDb),
     iObserver(aObserver),
     iOperation(EDbOperationNone)
    {
    CActiveScheduler::Add(this);
    }


// ---------------------------------------------------------------------------
// The 2nd phase Symbian Constructor
// ---------------------------------------------------------------------------
//
void CLbtDbOperationAO::ConstructL()
    {
    }


