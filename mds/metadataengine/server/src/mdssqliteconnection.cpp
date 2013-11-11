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

// INCLUDE FILES
#include "mdssqliteconnection.h"
#include "mdslogger.h"

__USES_LOGGER

CMdSSqLiteConnection* CMdSSqLiteConnection::NewL()
    {    
    CMdSSqLiteConnection* self = CMdSSqLiteConnection::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CMdSSqLiteConnection* CMdSSqLiteConnection::NewLC()
    {
    CMdSSqLiteConnection* self = new ( ELeave ) CMdSSqLiteConnection( );
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;  
    }


CMdSSqLiteConnection::CMdSSqLiteConnection()
	: iDbFileName( NULL ), iEnableTransaction( ETrue ),iNotFinishFindQuery( NULL )
    {
    }


CMdSSqLiteConnection::~CMdSSqLiteConnection()
    {
    CloseDb();

    delete iDbFileName;
    iDbFileName = NULL;    

    iNotFinishFindQuery = NULL;
    }

void CMdSSqLiteConnection::DeleteDb( TDesC16* aName )
	{
	if( aName )
		{
		RSqlDatabase::Delete( *aName );
		}
	else
		{
		RSqlDatabase::Delete( KMdsSqlDbDefaultName );
		}
	}

void CMdSSqLiteConnection::CloseDb()
	{
	iMdeSqlDb.Close();
	}

void CMdSSqLiteConnection::ConstructL()
    {
    }

void CMdSSqLiteConnection::OpenDbL( const TDesC& aDbFileName )
    {
    _LIT8( KMdsSqlDbaConfig, "cache_size=2000; page_size=2048; encoding=\"UTF-16\";");
    _LIT8( KBlacklistSqlDbaConfig, "cache_size=1500; page_size=1024; encoding=\"UTF-16\";");

    delete iDbFileName;
    iDbFileName = NULL; // in case AllocL leaves
    iDbFileName = aDbFileName.AllocL();

    TBool setupForMdsServer( EFalse );
    // Check if it is MDS server DB that is accessed, otherwise setup will be for Blacklist Server
    if( iDbFileName->Des().FindF( KMdsSqlDbDefaultName ) != KErrNotFound )
        {
        setupForMdsServer = ETrue;
        }
    
    TInt err = KErrNone;
    
    // we need to set up policy, because we use secure database
	TSecurityPolicy defaultPolicy(TSecurityPolicy::EAlwaysPass);
    RSqlSecurityPolicy sqlSecurityPolicy;
    CleanupClosePushL( sqlSecurityPolicy );
    err = sqlSecurityPolicy.Create( defaultPolicy );

    if ( err != KErrNone )
    	{
    	_LIT( KMdsSecurityCheckFail, "Security check fail" );
    	TraceAndLeaveL( KMdsSecurityCheckFail, err );
    	}
    /**
     * Open database:
     *   First we try to open db. If this fails because db not creater yer, then
     *   try to create it. Otherwise we cannot open it and we leave
     */
    if( setupForMdsServer )
        {
        err = iMdeSqlDb.Open( *iDbFileName, &KMdsSqlDbaConfig );
        }
    else
        {
        err = iMdeSqlDb.Open( *iDbFileName, &KBlacklistSqlDbaConfig );
        }
    if ( err != KErrNone )
        {
        __LOG1( ELogDb, "Cannot open database %d", err );
            
        if( err == KErrNotFound )
            {
            __LOG1( ELogDb, "Cannot find database %d", err );
            if( setupForMdsServer )
                {
                err = iMdeSqlDb.Create( *iDbFileName, sqlSecurityPolicy, &KMdsSqlDbaConfig );
                }
            else
                {
                err = iMdeSqlDb.Create( *iDbFileName, sqlSecurityPolicy, &KBlacklistSqlDbaConfig );
                }
            if( err != KErrNone )
                {
                __LOG1( ELogDb, "Unknown error while creating %d", err );
                User::LeaveIfError( err );
                }
            }
        else if( err == KErrCorrupt ||
                err == KSqlErrCorrupt )
            {
            __LOGLB( ELogDb, "Warning: Database is corrupted, will delete and re-create it." );
            if( setupForMdsServer )
                {
                err = DeleteAndReCreateDB( iDbFileName, sqlSecurityPolicy, &KMdsSqlDbaConfig );
                }
            else
                {
                err = DeleteAndReCreateDB( iDbFileName, sqlSecurityPolicy, &KBlacklistSqlDbaConfig );
                }
        
            if ( KErrNone == err  )
                {
                if( setupForMdsServer )
                    {
                    err = iMdeSqlDb.Open( *iDbFileName, &KMdsSqlDbaConfig );
                    }
                else
                    {
                    err = iMdeSqlDb.Open( *iDbFileName, &KBlacklistSqlDbaConfig );
                    }
                if ( err != KErrNone )
                    {
                    __LOG1( ELogDb, "Cannot open database again after delete and re-create %d", err );
                    User::LeaveIfError( err );
                    }
                } 
            }
        else 
            {
            __LOG1( ELogDb, "Unknown error while accessing database %d", err );
            User::LeaveIfError( err );
            }
        }
    CleanupStack::PopAndDestroy( &sqlSecurityPolicy );
    }

TInt CMdSSqLiteConnection::ExecuteL( const TDesC& aCommand,
                                     const RRowData& aVariables,
                                     RMdsStatement* aStatement )
    {
    TInt err = KErrNone;

    if ( aVariables.Size() == 0 )
        {
        // no variables
        err = iMdeSqlDb.Exec( aCommand );

		if ( err < KErrNone )
        	{
            _LIT( KMdSExec, "Exec (no variables)" );
            TraceAndLeaveL( KMdSExec, err );
            }
		}
    else if ( aStatement )
    	{
        if ( aStatement->iPrepared == EFalse )
            {
            err = aStatement->iStatement.Prepare( iMdeSqlDb, aCommand );

        	if ( err < KErrNone )
                {
                _LIT( KMdSPrepare, "Prepare" );
                TraceAndLeaveL( KMdSPrepare, err );
                }
            aStatement->iPrepared = ETrue;
            }
        else
            {
            err = aStatement->iStatement.Reset();
         	if ( err < KErrNone )
                {
                _LIT( KMdSResume, "Resume" );
                TraceAndLeaveL( KMdSResume, err );
                }
            }
           
        DoBindL( aStatement->iStatement, aVariables );
        err = aStatement->iStatement.Exec();

        if ( err < KErrNone )
            {
            aStatement->iStatement.Reset();
            aStatement->iPrepared = EFalse;
			_LIT( KMdSExec, "Exec" );
            TraceAndLeaveL( KMdSExec, err );
            }
    	}
    else
        {
        RSqlStatement mdeSqlDbStmt;
        CleanupClosePushL( mdeSqlDbStmt );
        err = mdeSqlDbStmt.Prepare( iMdeSqlDb, aCommand );

        if ( err < KErrNone )
            {
            _LIT( KMdsPrepare, "Prepare (no statement)" );
            TraceAndLeaveL( KMdsPrepare, err );
            }

        DoBindL( mdeSqlDbStmt, aVariables );
        
        err = mdeSqlDbStmt.Exec();

        if ( err < KErrNone )
            {
            _LIT( KMdsExec, "Exec (no statement)" );
            TraceAndLeaveL( KMdsExec, err );
            }

        CleanupStack::PopAndDestroy( &mdeSqlDbStmt );
        }
    return err;
    }

void CMdSSqLiteConnection::ExecuteQueryL( const TDesC& aQuery,
										  RMdsStatement& aStatement,
										  const RRowData& aVariables )
    {
    TInt stmterr;
    if ( aStatement.iPrepared == EFalse )
    	{
    	stmterr = aStatement.iStatement.Prepare( iMdeSqlDb, aQuery );
    	if ( stmterr != KErrNone )
        	{
        	_LIT( KMdSQueryPrepare, "Query Prepare" );
        	TraceAndLeaveL( KMdSQueryPrepare, stmterr );
        	}
    	aStatement.iPrepared = ETrue;
    	}
    else
    	{
    	stmterr = aStatement.iStatement.Reset();
    	if ( stmterr != KErrNone )
        	{
        	_LIT( KMdSQueryReset, "Query Reset" );
        	TraceAndLeaveL( KMdSQueryReset, stmterr );
        	}
    	}
        
    DoBindL( aStatement.iStatement, aVariables );
    }

void CMdSSqLiteConnection::DoBindL( RSqlStatement& aStatement, const RRowData& aVariables )
    {
    const TInt count( aVariables.Size() );
    for( TInt i=0; i < count; ++i )
        {
        switch ( aVariables.Column( i ).Type() )
            {
            case EColumnInt32:
                {
                TInt32 val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindInt( i, val );
                break;
                }
            case EColumnUint32:
                {
                TUint32 val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindInt64( i, (TInt64)val ); 
                break;
                }
            case EColumnBool:
                {
                TBool val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindInt( i, val );
                break;
                }
            case EColumnInt64:
                {
                TInt64 val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindInt64( i, val );
                break;
                }
            case EColumnReal32:
                {
                TReal32 val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindReal( i, val );
                break;
                }
            case EColumnReal64:
                {
                TReal64 val = 0;
                aVariables.Column( i ).Get( val );
                aStatement.BindReal( i, val );
                break;
                }
            case EColumnTime:
                {
                TTime val = TInt64(0);
                aVariables.Column( i ).Get( val );
                aStatement.BindInt64( i, val.Int64() );
                break;
                }
            case EColumnDes16:
                {
                TPtrC16 val = TPtr16((TUint16*)0, 0); //KNullPtr16;
                aVariables.Column( i ).Get( val );
                aStatement.BindText( i, val );
                break;
                }
            case EColumnNotUsed:
                // skip this variable
                break;
            default:
#ifdef _DEBUG
            	User::Panic( _L( "MdSSCDoB" ), KErrCorrupt );
#endif
                User::Leave( KErrCorrupt );
            }
        }
    }

TBool CMdSSqLiteConnection::NextRowL( RMdsStatement& aQuery, RRowData& aRow )
    {
    const TInt qerr = aQuery.iStatement.Next();
	if ( qerr == KSqlAtEnd )
		{
		return EFalse;
		}
	else if ( qerr != KSqlAtRow )
		{
		_LIT( KMdsQueryNextRow, "Query NextRow" );
		TraceAndLeaveL( KMdsQueryNextRow, qerr );
		}

    // obtain column data into local storage and row data pointer
    ColumnsL( aQuery.iStatement, aRow );

    return ETrue;
    }

void CMdSSqLiteConnection::CurrentRowL( const RMdsStatement& aQuery, RRowData& aRow )
    {
    // obtain column data into local storage and row data pointer
    if ( EFalse != aQuery.iStatement.AtRow() )
    	{
    	ColumnsL( aQuery.iStatement, aRow );
    	}
    else
    	{
    	_LIT( KMdsNoProcessableRow, "Wrong row to process" );
    	TraceAndLeaveL( KMdsNoProcessableRow, KSqlErrNotFound );
    	}
    }

TItemId CMdSSqLiteConnection::LastInsertedRowId()
    {
    return iMdeSqlDb.LastInsertedRowId();
    }

void CMdSSqLiteConnection::ColumnsL( const RSqlStatement& aStatement, RRowData& aRow )
    {
    const TInt count( aRow.Size() );
    for( TInt i=0; i < count; ++i )
        {
        // get data in column, check for type
   		const TSqlColumnType actual = aStatement.ColumnType( i );
   		
   		if( actual == ESqlNull )
   			{
            aRow.Column( i ).Set( (const HBufC16*)NULL );

            continue;
   			}
   		
        const TColumnDataType coltype = aRow.Column( i ).Type();
        switch ( coltype )
            {
            case EColumnBool:
                {
                TInt valInt = aStatement.ColumnInt( i );
                const TBool valBool = valInt ? ETrue : EFalse;
                aRow.Column( i ).Set( valBool );
                break;
                }
            case EColumnInt32:
                {
                TInt32 valInt = aStatement.ColumnInt( i );
                aRow.Column( i ).Set( valInt );
                break;
                }
            case EColumnUint32:
                {
                TInt64 valInt64 = aStatement.ColumnInt64( i );           
                aRow.Column( i ).Set( (TUint32)valInt64 );
                break;
                }
            case EColumnInt64:
                {
                TInt64 valInt64 = aStatement.ColumnInt64( i );
                aRow.Column( i ).Set( valInt64 );
                break;
                }
            case EColumnReal32:
                {
                TReal valReal = aStatement.ColumnReal( i );
                aRow.Column( i ).Set(  static_cast<TReal32>( valReal ) );
                break;
                }
            case EColumnReal64:
                {
                TReal valReal = aStatement.ColumnReal( i );
                aRow.Column( i ).Set( valReal );
                break;
                }
            case EColumnTime:
                {
                TTime valTime = aStatement.ColumnInt64( i );
                aRow.Column( i ).Set( valTime );
                break;
                }
           	case EColumnDes16:
           		{
           		switch ( actual )
           		{
           		case ESqlText:
                    {
                    TPtrC16 valTPtrC16 = aStatement.ColumnTextL( i );
                    HBufC16* valHBuf16 = HBufC16::NewL( valTPtrC16.Length() );
                    *valHBuf16 = valTPtrC16;
                	aRow.Column( i ).Set( valHBuf16 );
                	break;
                    }
           		case ESqlInt:
                    {
                    HBufC16* valHBuf16int32 = HBufC16::NewL( 30 );
                    TInt valInt = aStatement.ColumnInt( i );
                    valHBuf16int32->Des().Num( valInt );
                    aRow.Column( i ).Set( valHBuf16int32 );
                    break;
                    }
           		case ESqlInt64:
                    {
                    HBufC16* valHBuf16int64 = HBufC16::NewL( 30 );
                	TInt64 valInt64 = aStatement.ColumnInt64( i );
                    valHBuf16int64->Des().Num( valInt64 );
                    aRow.Column( i ).Set( valHBuf16int64 );
                    break;
                    }
           		case ESqlReal:
                    {
                    HBufC16* valHBuf16real64 = HBufC16::NewL( 40 );
                	TReal valReal = aStatement.ColumnReal( i );
                	TRealFormat realFormat;
                	realFormat.iType |= KAllowThreeDigitExp;
                    valHBuf16real64->Des().Num( valReal, realFormat );
                    aRow.Column( i ).Set( valHBuf16real64 );
                    break;
                    }
           		case ESqlNull:
                    {
                    aRow.Column( i ).Set( (HBufC16*)NULL );
                    break;
                    }
           		default:
                    {
#ifdef _DEBUG
            		User::Panic( _L( "MdSSCCo1" ), KErrCorrupt );
#endif
                    User::Leave( KErrCorrupt );
                    }
           		}
           		
                break;
                }
            case EColumnNotUsed:
                // skip this round
                break;

            default:
#ifdef _DEBUG
				User::Panic( _L( "MdSSCCo2" ), KErrCorrupt );
#endif
				User::Leave( KErrCorrupt );
            }
        }
    }

void CMdSSqLiteConnection::Terminate( RMdsStatement& aQuery )
    {    
    aQuery.Close();
    EnableTransaction( ETrue, aQuery );
    }

void CMdSSqLiteConnection::TransactionBeginL()
    {
    if ( !iEnableTransaction || iMdeSqlDb.InTransaction() )
        {
        return;
        }
    iEnableTransaction = EFalse;
    iTransactionOngoing = ETrue;
    _LIT(KBeginTransaction, "BEGIN;");
    RRowData emptyRow;
    CleanupClosePushL( emptyRow );
    TRAPD( err, ExecuteL(KBeginTransaction, emptyRow) );
    if (err != KErrNone)
    	{
    	_LIT( KMdsTransactionBegin, "Transaction begin error" );
        if( !iNotFinishFindQuery )
            {
            iEnableTransaction = ETrue;
            }
        iTransactionOngoing = EFalse;
    	TraceAndLeaveL( KMdsTransactionBegin, err );
    	}
	CleanupStack::PopAndDestroy( &emptyRow );
    }

void CMdSSqLiteConnection::TransactionCommitL()
    {
    if ( !iTransactionOngoing )
        {
        return;
        }
    _LIT(KCommit, "COMMIT;");
    RRowData emptyRow;
    CleanupClosePushL( emptyRow );
    TRAPD( err, ExecuteL(KCommit, emptyRow) );
    if (err != KErrNone)
    	{
    	_LIT( KMdsTransactionCommit, "Transaction commit error" );
    	TraceAndLeaveL( KMdsTransactionCommit, err );
    	}
    if( !iNotFinishFindQuery )
        {
        iEnableTransaction = ETrue;
        }
    iTransactionOngoing = EFalse;
	CleanupStack::PopAndDestroy( &emptyRow );
    }

void CMdSSqLiteConnection::TransactionRollbackL()
    {
    if ( !iTransactionOngoing )
        {
        return;
        }
    _LIT(KRollback, "ROLLBACK;");
    RRowData emptyRow;
    CleanupClosePushL( emptyRow );
    TRAPD( err, ExecuteL(KRollback, emptyRow) );
    if (err != KErrNone)
        {
        _LIT( KMdsTransactionRollback, "Transaction rollback error" );
        TraceAndLeaveL( KMdsTransactionRollback, err );
        }    
    if( !iNotFinishFindQuery )
        {
        iEnableTransaction = ETrue;
        }
    iTransactionOngoing = EFalse;
	CleanupStack::PopAndDestroy( &emptyRow );
    }

#ifdef _DEBUG
void CMdSSqLiteConnection::TraceAndLeaveL( const TDesC& aFailedCommand, TInt aSqliteError )
    {
	TPtrC errorMsg = iMdeSqlDb.LastErrorMessage();
    if ( aFailedCommand.Length() > 0 )
        {
        __LOG2( ELogDb, "%S %d", &aFailedCommand, aSqliteError );
        }
	__LOG1( ELogDb, "SQLiteError description: %S", &errorMsg );
    User::Leave( aSqliteError );
    }
#else
void CMdSSqLiteConnection::TraceAndLeaveL( const TDesC& /*aFailedCommand*/, TInt aSqliteError )
    {
    User::Leave( aSqliteError );
    }
#endif

const TDesC& CMdSSqLiteConnection::DbFileName() const
	{
	return *iDbFileName;
	}

void CMdSSqLiteConnection::EnableTransaction( TBool aEnable, RMdsStatement& aQuery ) 
    {
    if( aEnable )
        {
        // check whether current query is the query which disabled the transaction when enabling transaction
        if( !iEnableTransaction && ( iNotFinishFindQuery == &aQuery ) )
            {
            if( !iTransactionOngoing )
                {
                iEnableTransaction = aEnable;
                }
            iNotFinishFindQuery = NULL;
            }
        }
    else
        {
        iEnableTransaction = aEnable;
        // save current find operation which will continue when diable transaction
        iNotFinishFindQuery = &aQuery;
        }
    }

void CMdSSqLiteConnection::DoAnalyzeL() 
    {
    _LIT( KAnalyze, "ANALYZE;");
    
    RRowData emptyRow;
    CleanupClosePushL( emptyRow );
    ExecuteL(KAnalyze, emptyRow);
    CleanupStack::PopAndDestroy( &emptyRow ); 
    }

TInt CMdSSqLiteConnection::DeleteAndReCreateDB( const HBufC* aDbFileName,
                                                const RSqlSecurityPolicy& asqlSecurityPolicy,
                                                const TDesC8* aKMdsSqlDbaConfig ) 
    {    
    TInt err = iMdeSqlDb.Delete( *aDbFileName );
    if( err!= KErrNone )
        {
        __LOG1( ELogDb, "delete database err=%d.", err );
        return err;
        }

    err = iMdeSqlDb.Create( *aDbFileName, asqlSecurityPolicy, aKMdsSqlDbaConfig );

    return err;
    }

