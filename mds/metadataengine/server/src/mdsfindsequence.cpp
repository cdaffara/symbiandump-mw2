/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Manages object search from database*
*/

#include <badesca.h>
#include <mdequery.h>

#include "mdsfindsequence.h"

#include "mdcitem.h"
#include "mdcresult.h"
#include "mdcserializationbuffer.h"
#include "mdsschema.h"
#include "mdslogger.h"
#include "mdsfindengine.h"
#include "mdssqlfindoperation.h"
#include "mdssqliteconnection.h"
#include "mdsfindsqlclause.h"
#include "mdsdbconnectionpool.h"
#include "mdsclausebuffer.h"


/** logging instance */
__USES_LOGGER


// ------------------------------------------------
// NewL
// ------------------------------------------------
//
CMdSFindSequence* CMdSFindSequence::NewL( 
	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserver )
    {
    CMdSFindSequence* self = CMdSFindSequence::NewLC( aServer, aSchema, aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ------------------------------------------------
// NewLC
// ------------------------------------------------
//
CMdSFindSequence* CMdSFindSequence::NewLC( 
	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserver )
    {
    CMdSFindSequence* self = new(ELeave) CMdSFindSequence( aServer, aSchema, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ------------------------------------------------
// Default constructor
// ------------------------------------------------
//
CMdSFindSequence::CMdSFindSequence( 
	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserver )
    : CActive( CActive::EPriorityStandard )
    , iServer( aServer )
    , iSchema( aSchema )
    , iObserver( &aObserver )
    , iUserLevel( EUserLevelNone )
    {
	iNotifyCount = KMdEQueryDefaultMaxCount;

    iFindOperation = NULL;
    }

// ------------------------------------------------
// ConstructL
// ------------------------------------------------
//
void CMdSFindSequence::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

// ------------------------------------------------
// Destructor
// ------------------------------------------------
//
CMdSFindSequence::~CMdSFindSequence()
    {
    CleanUp();
    
    Cancel();
    }

void CMdSFindSequence::SetFindParams( TUint32 aNotifyCount )
	{
	iNotifyCount = aNotifyCount;
	}

// ------------------------------------------------
// FindL
// ------------------------------------------------
//
TInt CMdSFindSequence::FindL(
    CMdCSerializationBuffer& aSerializedCriteria,
    TUserLevel aUserLevel )
    {
    __ASSERT_DEBUG( !iFindOperation, MMdCCommon::Panic( KErrCorrupt ) );

	iUserLevel = aUserLevel;

	iSerializedCriteria = &aSerializedCriteria;

	if( iFindResults )
		{
		delete iFindResults;
		iFindResults = NULL;
		}
 
    iFindOperation = CreateOperationL( aSerializedCriteria );

    const TInt result( iFindOperation->ExecuteL() );
    
    iFindResults = iFindOperation->Results();
    
    if( iFindResults )
    	{
    	PostProcessL( *iFindResults );
    	}

    return result;
    }

// ------------------------------------------------
// ContinueL
// ------------------------------------------------
//
TInt CMdSFindSequence::ContinueL()
    {
    __ASSERT_DEBUG( iFindOperation, MMdCCommon::Panic( KErrCorrupt ) );

	if( iFindResults )
		{
		delete iFindResults;
		iFindResults = NULL;
		}
    
    const TInt result = iFindOperation->ContinueL();

    if ( result == KErrNone )
        {
        iFindResults = iFindOperation->Results();

		if( iFindResults )
			{
			PostProcessL( *iFindResults );
			}
        }

    return result;
    }

// ------------------------------------------------
// FindAsync
// ------------------------------------------------
//
void CMdSFindSequence::FindAsync(CMdCSerializationBuffer& aSerializedCriteria,
		TUserLevel aUserLevel )
    {
    __ASSERT_DEBUG( !iFindOperation, MMdCCommon::Panic( KErrCorrupt ) );
    
    iUserLevel = aUserLevel;

	if( iFindResults )
		{
		delete iFindResults;
		iFindResults = NULL;
		}

    iSerializedCriteria = &aSerializedCriteria;

    SetActive();
    TRequestStatus* pStatus = &iStatus;
    User::RequestComplete( pStatus, EAsyncFind );
    }

// ------------------------------------------------
// ContinueAsync
// ------------------------------------------------
//
void CMdSFindSequence::ContinueAsync()
    {
    __ASSERT_DEBUG( iFindOperation, MMdCCommon::Panic( KErrCorrupt ) );

	if( iFindResults )
		{
		delete iFindResults;
		iFindResults = NULL;
		}

    SetActive();
    TRequestStatus* pStatus = &iStatus;
    User::RequestComplete( pStatus, EContinueAsyncFind );
    }

// ------------------------------------------------
// Results
// ------------------------------------------------
//
CMdCSerializationBuffer& CMdSFindSequence::ResultsL() const
    {
    if( !iFindResults )
    	{
    	User::Leave( KErrCorrupt );
    	}

    return *iFindResults;
    }

// ------------------------------------------------
// SetResultMode
// ------------------------------------------------
//
void CMdSFindSequence::SetResultMode( TBool aResultModeItems )
	{
	iLastResultModeItems = aResultModeItems;
	iObserver->SetResultMode( aResultModeItems );
	}

// ------------------------------------------------
// RunL
// ------------------------------------------------
//
void CMdSFindSequence::RunL()
    {
    TInt result = 0;

    switch ( iStatus.Int() )
        {
        case EAsyncFind:
            {
            iFindOperation = CreateOperationL( *iSerializedCriteria );
            result = iFindOperation->ExecuteL();
            break;
            }
        case EContinueAsyncFind:
            {
            if( iFindOperation )
            	{
            	result = iFindOperation->ContinueL();
            	}
            else
            	{
            	result = KErrNotFound;
            	}
            break;
            }
        default:
            {
#ifdef _DEBUG
            User::Panic( _L("MdSFSRun") , KErrCorrupt );
#endif            
            User::Leave( KErrCorrupt );
            }
        }
    if( result == KErrNone || result == KFindSetReady )
        {
        iFindResults = iFindOperation->Results();

        if( iFindResults )
        	{        	
        	PostProcessL( *iFindResults );
        	}
        }

    if ( result == KFindSetReady )
        {
        CMdSFindEngine* obs = iObserver;
        if ( obs )
        	{
        	obs->SetComplete( KErrNone, ETrue );
        	}
        }
    else if ( result == KErrCancel )
        {
        CleanUp();
        // do NOT notify observer
        }
    else
        {
        CMdSFindEngine* obs = iObserver;
        if ( obs )
        	{
        	obs->SetComplete( result, EFalse );
        	}
        }
    }

// ------------------------------------------------
// RunError
// ------------------------------------------------
//
TInt CMdSFindSequence::RunError( TInt aError )
    {
    // Cleanup if RunL() leaves
    CMdSFindEngine* obs = iObserver;
    CleanUp();
    obs->SetComplete( aError, EFalse );
    return KErrNone;
    }

// ------------------------------------------------
// DoCancel
// ------------------------------------------------
//
void CMdSFindSequence::DoCancel()
    {
    if ( !iFindOperation )
        {
        // already finished
        CleanUp();
        return;
        }
    const TInt state = iFindOperation->State();
    if ( state == CMdSSqlFindOperation::EStateIdle )
        {
        // loop is idle - safe to clean up.
        CleanUp();
        }
    else if ( state == CMdSSqlFindOperation::EStateRunning )
        {
        // interrupt loop.
        iFindOperation->Cancel();
        }
    else if ( state == CMdSSqlFindOperation::EStateDead )
        {
        // loop is already ending.
        }
    return;
    }

// ------------------------------------------------
// CreateOperationL
// ------------------------------------------------
//
CMdSSqlFindOperation* CMdSFindSequence::CreateOperationL(
		CMdCSerializationBuffer& aSerializedCriteria )
    {
    CMdSSqlFindOperation* operation = CreateOperationLC( aSerializedCriteria );
    CleanupStack::Pop( operation );
    return operation;
    }

// ------------------------------------------------
// CreateOperationLC
// ------------------------------------------------
//
CMdSSqlFindOperation* CMdSFindSequence::CreateOperationLC(
    CMdCSerializationBuffer& aSerializedCriteria )
    {
    CMdSSqlFindOperation* operation = CMdSSqlFindOperation::NewLC( *this, iNotifyCount );

	CMdSFindSqlClause& findSqlClause = operation->FindCriteria();

	if( aSerializedCriteria.Buffer().Ptr() && aSerializedCriteria.Buffer().Length() != 0 )
		{		
   		findSqlClause.CreateL( aSerializedCriteria, iUserLevel );

		operation->SetLimit( findSqlClause.Limit() );

   		__LOGQUERY_16( _L("Execute query:"), operation->FindCriteria().AsTextL(), operation->FindCriteria().Variables() );
		}
	else
		{
		User::Leave( KErrBadDescriptor );
		}

    return operation;
    }

struct TObjectHitCount
	{
	TInt iCount;
	TUint32 iObjectOffset;
	};

static TInt SortValues(const TObjectHitCount& aFirst, const TObjectHitCount& aSecond)
	{
	TInt result = aSecond.iCount - aFirst.iCount;
	if (result == 0)
		{
		result = aFirst.iObjectOffset - aSecond.iObjectOffset;
		}
	return result;
	}

void CMdSFindSequence::GetFreeTextForObjectL( CDesCArray& aResultWordBuffer,
		TDefId aNamespaceDefId, TItemId aObjectId )
	{
	_LIT( KMdSFindSeqWords, "SELECT Word FROM TextSearch%u AS ts, TextSearchDictionary%u AS tsd ON tsd.WordId = ts.WordId WHERE ObjectId = ? ORDER BY Position ASC;" );

	CMdsClauseBuffer* buffer = CMdsClauseBuffer::NewLC( KMdSFindSeqWords.iTypeLength + 20 ); // two int
	buffer->BufferL().Format( KMdSFindSeqWords, aNamespaceDefId, aNamespaceDefId );
	
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	
	RRowData data;
    CleanupClosePushL( data );
	data.AppendL( TColumn( aObjectId ) );
	RMdsStatement query;
    CleanupClosePushL( query );
	connection.ExecuteQueryL( buffer->ConstBufferL(), query, data );
	TPtrC16 word;
	data.Column(0).Set( word );
	while (connection.NextRowL(query, data))
		{
		data.Column(0).Get( word );
		aResultWordBuffer.AppendL( word );
		data.Free();
		}

	CleanupStack::PopAndDestroy( 3, buffer ); // query, data, buffer
	}

// ------------------------------------------------
// PostProcessL
// ------------------------------------------------
//
void CMdSFindSequence::PostProcessL( CMdCSerializationBuffer& aSerializedResultBuffer )
    {
#ifdef _DEBUG    
	_LIT( KFindFunctionName, "CMdSFindSequence::PostProcessL" );
#endif

    // process only on items result
    if (!iLastResultModeItems)
    	{
    	return;
    	}
    
    if(iFindOperation && iFindOperation->FindCriteria().IncludesFreetexts() == EFalse )
   		{
    	return;
    	}
    
    if (!iFindOperation)
        {
        return;
        }
    
    RPointerArray<HBufC>& searchFreeText = iFindOperation->QueryFreeText();
    // to through every object and check freetext
    aSerializedResultBuffer.PositionL( KNoOffset );
    const TMdCItems& items = TMdCItems::GetFromBufferL( aSerializedResultBuffer );
    const TBool needToSort = searchFreeText.Count() != 0 
    		&& items.iObjects.iPtr.iCount > 1;

    RArray<TObjectHitCount> hitCountArray;
    CleanupClosePushL( hitCountArray );

    TObjectHitCount hitCount;
    for( TUint32 i = 0; i < items.iObjects.iPtr.iCount; ++i )
    	{
    	aSerializedResultBuffer.PositionL( items.iObjects.iPtr.iOffset 
    			+ i * sizeof(TMdCObject) );
    	const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedResultBuffer );
    	// check all objects

		// jump to freetext
		if ( object.iFreeTexts.iPtr.iCount == 0 )
			{
			continue;
			}

		CDesC16ArrayFlat* resultWordBuffer = new(ELeave) CDesC16ArrayFlat( object.iFreeTexts.iPtr.iCount );
		CleanupStack::PushL( resultWordBuffer );
		// get freetext for object
		GetFreeTextForObjectL( *resultWordBuffer, items.iNamespaceDefId, object.iId );
		__ASSERT_DEBUG( object.iFreeTexts.iPtr.iCount == resultWordBuffer->Count(), User::Panic( KFindFunctionName, KErrCorrupt) );

	    if (needToSort)
	    	{
 		   	hitCount.iObjectOffset = items.iObjects.iPtr.iOffset + i * sizeof(TMdCObject);
			hitCount.iCount = GetFreeTextHitCountL( *resultWordBuffer, searchFreeText );
    		hitCountArray.AppendL( hitCount );
	    	}
		
		aSerializedResultBuffer.PositionL( object.iFreeTexts.iPtr.iOffset );
		for ( TUint32 f = 0; f < object.iFreeTexts.iPtr.iCount; ++f )
			{
			// insert freeText here
			TPtrC16 word = (*resultWordBuffer)[f];
			aSerializedResultBuffer.InsertL( word );
			}
		CleanupStack::PopAndDestroy( resultWordBuffer );
		}

    if ( needToSort && hitCountArray.Count() > 1 )
    	{
		hitCountArray.Sort( TLinearOrder<TObjectHitCount>( SortValues ) );

		RArray<TMdCObject> objectArray;
		CleanupClosePushL( objectArray );
		objectArray.ReserveL( items.iObjects.iPtr.iCount );
		// store objects in array in correct order
		for( TInt i = 0; i < items.iObjects.iPtr.iCount; ++i )
			{
			aSerializedResultBuffer.PositionL( hitCountArray[i].iObjectOffset );
			const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedResultBuffer );
			objectArray.AppendL( object );
			}
		// set them back in serialized buffer
		aSerializedResultBuffer.PositionL( items.iObjects.iPtr.iOffset );
		for (TInt i = 0; i < items.iObjects.iPtr.iCount; ++i)
			{
			objectArray[i].SerializeL( aSerializedResultBuffer );
			}
		CleanupStack::PopAndDestroy( &objectArray );
    	}

	CleanupStack::PopAndDestroy( &hitCountArray );
    }

// ------------------------------------------------
// CleanUp
// ------------------------------------------------
//
void CMdSFindSequence::CleanUp()
    {
    // clean up rubbish
	if( iFindOperation )
		{
	   	delete iFindOperation;
		iFindOperation = NULL;
		}

    iObserver = NULL;

	if( iFindResults )
		{
		delete iFindResults;
		iFindResults = NULL;
		}
    }

// ------------------------------------------------
// IsCleaned
// ------------------------------------------------
//
TBool CMdSFindSequence::IsComplete() const
	{
	// not failed or query still running
	if( //iObserver || 
		( iFindOperation && iFindOperation->State() != CMdSSqlFindOperation::EStateDead ) )
		{
		return EFalse;
		}
		
	return ETrue;
	}
    
// ------------------------------------------------
// IsComplete
// ------------------------------------------------
//
TBool CMdSFindSequence::IsQueryComplete() const
	{
	if ( iFindOperation )
		{
		return ( iFindOperation->State() == CMdSSqlFindOperation::EStateDead );
		}
	else
		{
		return ETrue;
		}
	}

const CMdsSchema& CMdSFindSequence::Schema() const
	{
	return iSchema;
	}

CMdSServer& CMdSFindSequence::Server() const
	{
	return iServer;
	}

// ------------------------------------------------
// GetFreeTextHitCountL
// ------------------------------------------------
//
TUint32 CMdSFindSequence::GetFreeTextHitCountL(
		const CDesCArray& aObjectFreeText, 
		const RPointerArray<HBufC>& aSearchFreeText)
	{
	TUint32 hitCount = 0;

	const TInt objectFreeTextCount = aObjectFreeText.Count();

	for(TInt i = 0; i < objectFreeTextCount; ++i)
		{
		TInt length = aObjectFreeText[i].Length();
		const TDesC& objectText = aObjectFreeText[i];
		const TInt32 objectTextLength = objectText.Length();
		const TInt searchFreeTextCount = aSearchFreeText.Count();

    	for( TInt j = 0; j < searchFreeTextCount; ++j )
	    	{
			const TDesC& searchText = *aSearchFreeText[j];
			const TInt32 searchTextLength = searchText.Length();

			if (searchTextLength > objectTextLength)
				{
				continue;
				}
			TInt32 searchStart = 0;
			while(objectTextLength - searchStart >= searchTextLength)
				{
				const TInt retValue = objectText.Mid(searchStart, 
						objectTextLength - searchStart).FindF(searchText);

				if(retValue != KErrNotFound)
					{
					searchStart += retValue+searchTextLength;
					hitCount++;
					}
				else
					{
					break;
					}
				}
	    	}
    	}
    return hitCount;
	}
