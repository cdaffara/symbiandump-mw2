/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Adaptation layer to SQLite database*
*/

#ifndef __MDSSQLITECONNECTION_H__
#define __MDSSQLITECONNECTION_H__

#include <e32base.h>
#include <sqldb.h>

#include "mdssqlrow.h"


// FORWARD DECLARATION
class RMdsStatement;



// CLASS DECLARATION
/**
* Class represents methods to communication with SQLite database API.
*/
NONSHARABLE_CLASS(CMdSSqLiteConnection): public CBase
	{
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param 
        * @return 
        */
        static CMdSSqLiteConnection* NewL();
        
        /**
        * Two-phased constructor.
        * @param 
        * @return 
        */
        static CMdSSqLiteConnection* NewLC();

        /**
        * Destructor.
        */   
        virtual ~CMdSSqLiteConnection();
        
        /**
         * Deletes database from disc
         *
         * @param aName name of database or NULL to delete default one
         */
        static void DeleteDb( TDesC16* aName = NULL );
        
    private: // Private constructors

        /**
        * CSqLiteConnection.
        * C++ default constructor.
        */        
        CMdSSqLiteConnection();

        /**
        * ConstructL.
        * 2nd phase constructor.
        */        
        void ConstructL();

        /**
        * Delete and re-create database.
        * @param aDbFileName Database filename and path
        * @param asqlSecurityPolicy sql policy
        * @param aKMdsSqlDbaConfig database connection configure
        */    
        TInt DeleteAndReCreateDB( const HBufC* aDbFileName,
                                  const RSqlSecurityPolicy& asqlSecurityPolicy,
                                  const TDesC8* aKMdsSqlDbaConfig );
        
    public: // Methods from CMdSSqLiteConnection

		/**
		 * Open database (creates if doesn't exist)
		 * @param aFile Database filename and path
		 */
        void OpenDbL( const TDesC& aDbFileName );

        /**
         * Close opened database
         */
        void CloseDb();

        /**
         * Execute sql command (not a query)
         * @param aCommand SQL clause to be executed (can include wildcards)
         * @param aVariables data that will replace wildcards in the command
         * @return KErrNone, if successful, otherwise one of the error codes.
         */
        TInt ExecuteL( const TDesC& aCommand,
                       const RRowData& aVariables,
                       RMdsStatement* aStatement = NULL );

        /**
         * Prepare and execute sql query
         * @param aQuery the SQL query to perform
         * @param aStatement ptr to statement id, which must be initially NULL.
         *        The statement can be used for executing the same query several
         *        times with different variables. Note that finally the statement
         *        must be terminated.
         * @param aVariables data that will replace wildcards in the command
         */
        void ExecuteQueryL( const TDesC& aQuery,
        					RMdsStatement& aStatement,
            				const RRowData& aVariables );

        /**
         * Get next row in query
         * @param aQuery ID of the query
         * @param aRow specifies target for the data to receive
         * @return true if there are more rows available
         */
        TBool NextRowL( RMdsStatement& aQuery, RRowData& aRow );

        /**
         * Update the current row in query
         * (in case some of the datatypes need to change)
         * @param aQuery ID of the query
         * @param aRow specifies target for the data to receive
         * @return true if there are more rows available
         */
        void CurrentRowL( const RMdsStatement& aQuery, RRowData& aRow );

        /**
         * Returns id of the last inserted row
         */
        TItemId LastInsertedRowId();

        /**
         * terminates a running query or execution loop
         * @param aStatement ID of the statement
         */
        void Terminate( RMdsStatement& aQuery );
        
        /**
         * Rolls back the transaction explicitly
         * - possibility to receive leave
         * eliminates actions on destructor.
         */
        void TransactionBeginL();

        /**
         * Resets the transaction
         * (does not commit or rollback or do anything)
         */
        void TransactionCommitL();

        /**
         * error within transaction
         */
        void TransactionRollbackL();

        /**
         * Returns DB filename of the connection.
         */
        const TDesC& DbFileName() const;
        
        void EnableTransaction( TBool aEnable, RMdsStatement& aQuery );

        void DoAnalyzeL();
        
    protected: // Personal methods     

        /**
        * Binds variables in SQLite interface
        * @param aStatement SQLite statement handle
        * @param aVariables the list of variables to bind
        */
        void DoBindL( RSqlStatement& aStatement, const RRowData& aVariables );

        /**
        * Reads row column variables from SQLite interface
        * @param aStatement SQLite statement handle
        * @param aRow the row buffer where to read the column data
        */
        void ColumnsL( const RSqlStatement& aStatement, RRowData& aRow );

        /**
        * Saves trace of last SQL error and leaves.
        * @param aFailedCommand a C-style representation of the failed SqLite command (for logging)
        * @param aSqliteError the error reported by sqlite
        */
        void TraceAndLeaveL( const TDesC& aFailedCommand, TInt aSqliteError );

    private: // Member data

        /**
         * iMdeSqlDb, interface to the Sqlite database.
         */
        RSqlDatabase iMdeSqlDb;

        HBufC* iDbFileName;
        
        TBool iEnableTransaction;
        
        TBool iTransactionOngoing;
		
	    RMdsStatement* iNotFinishFindQuery;
	};

const TInt KDoNothing = 99;

/**
* Transaction class
*/
class RMdSTransaction
    {
    public:

        /**
        * constructor
        * creates a transaction to given connection
        */
        inline RMdSTransaction( CMdSSqLiteConnection& aConnection )
            : iConnection( aConnection )
            {
            TRAP( iErrorState, iConnection.TransactionBeginL() );
            }

        /**
        * cleanup method (from TBase)
        * according to status does Rollback or nothing if commit was succesfull
        */
        inline void Close()
            {
            // Internal error state is KErrNone if the transaction could be created successfully,
            // and the state is resetted if the transaction is committed succesfully. Only if the
            // transaction is started succesfully but not committed, it needs to be rolled back
            // to close the ongoing transaction
            if ( iErrorState == KErrNone )
                {
                TRAP( iErrorState, iConnection.TransactionRollbackL() );
                }
            }

        /**
        * Commits the transaction explicitly
        * - possibility to receive leave
        * eliminates actions on destructor.
        */
        inline void CommitL()
            {
            iConnection.TransactionCommitL();
            Reset();
            }

        /**
        * Resets the transaction
        * (does not commit or rollback or do anything)
        */
        inline void Reset()
            {
            iErrorState = KDoNothing;
            }

        /**
        * error within transaction
        */
        inline TInt Error()
            {
            return iErrorState;
            }

    protected:


        /** the connection */
    	CMdSSqLiteConnection& iConnection;

        /** transaction begin error */
        TInt iErrorState;
    };

/**
* An open statement stack helper
* ensures a statement gets terminated
*/
class RMdsStatement
    {
    friend class CMdSSqLiteConnection;
    public:
        /** constructor */
        inline RMdsStatement( )
            : iPrepared( EFalse )
            {
            }

        inline void Close()
            {
            iStatement.Close();
            }
    private:

		/** SQL statement */
		RSqlStatement iStatement;
		
        /** if statement was already prepared */
        TBool iPrepared;
    };

/*

  how to use the statement stack helper:

  {
  RMdsStatement st( connection );
  CleanupClosePushL( st );
  connection.ExecuteL( clause, variables1, st ); // prepared and executed
  connection.ExecuteL( clause, variables2, st ); // executed normally
  CleanupStack::PopAndDestroy( st );
  } // statement automatically terminated

  {
  RMdsStatement st( connection );
  CleanupClosePushL( st );
  connection.ExecuteQueryL( query, st, variables1 );
  connection.NextRowL( st, results1 ); // fetch a result row
  connection.ExecuteQueryL( query, st, variables2 ); // query with different variable set
  connection.NextRowL( st, results2 ); // fetch another resut row
  CleanupStack::PopAndDestroy( st );
  } // statement automatically terminated

*/
	
#endif	// __MDSSQLITECONNECTION_H__
