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
* Description:  This header file describes the class that handles the 
*                asynchronous operations on the triggers in database.
*
*/


#ifndef C_LBTDB_OPERATION_AO_H
#define C_LBTDB_OPERATION_AO_H

#include <e32base.h>
#include <d32dbms.h>

// forward declaration
class CLbtDbTriggersManager;

/**
 *  Execute the Database operations asynchronously
 */
NONSHARABLE_CLASS( CLbtDbOperationAO ) : public CActive
    {
public:
    static CLbtDbOperationAO* NewL(
        RDbDatabase& aDb,
        CLbtDbTriggersManager* aObserver);

    virtual ~CLbtDbOperationAO();

    void ExecuteQuery(
        RDbView& aView,
        TDesC& aQuery);

    TInt ExecuteSyncQuery(
        RDbView& aView,
        TDesC& aQuery);
    void CompactDb();
    
    TBool IsCompactionOngoing();
    
protected:
    /**
     * Implements cancellation of an outstanding request.
     */
	virtual void DoCancel();

    /**
     * Handles completion of the Db operation request.
     */
	virtual void RunL();
	
	/**
	 * Handles error in the execution of the Db operation
	 */
	virtual TInt RunError(TInt aError);

private:

    CLbtDbOperationAO(
        RDbDatabase& aDb,
        CLbtDbTriggersManager* aObserver);

    void ConstructL();

private:
    enum TDbOperationType
        {
            EDbOperationNone = 0, // Currently ready to execute a Db operation
            EDbSqlQuery = 1, // Executing a Sql Query on the Database
            EDbCompaction = 2 // Executing a compaction operation on the database
        };
    
private: // data
    /** Handle to the Database */
    RDbDatabase& iDb;

    /** 
     * The Database view which provides the interface to execute 
     * the db operations.
     * Does not own the Handle. This is just a reference.
     */
    RDbView* iView;
    
    /**
     * The observer for the asynchronous operation.
     * Does not own the observer object.
     */
    CLbtDbTriggersManager* iObserver; 

    /**
     * The Db handle to execute incremental operations on the database.
     * Typical operations are on the database itself like compaction and
     * recovery..
     */
    RDbIncremental iDbIncrOp;

    /**
     * Counts the remaining steps in the Incremental operation
     */
    TInt iSteps;

    /**
     * Indicates the type of async Db operation being performed by this object
     */
    TDbOperationType iOperation;
    };

#endif // C_LBTDB_OPERATION_AO_H
