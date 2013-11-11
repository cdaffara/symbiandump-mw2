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
* Description:  Asynchronous query active object
*
*/


// INCLUDE FILES
#include "mdeasynchronousfindao.h"
#include "mdequeryimpl.h"

#include "mdeenginesession.h"
#include "mdelogiccondition.h"
#include "mdesessionimpl.h"
#include "mdequerycriteriaserialization.h"
#include "mdeinstanceitem.h"

// ========================= MEMBER FUNCTIONS ==================================

CMdEAsynchronousFindAO* CMdEAsynchronousFindAO::NewL( CMdEQuery& aQuery,
    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    {
    CMdEAsynchronousFindAO* self = CMdEAsynchronousFindAO::NewLC( aQuery, aSessionImpl, aSession );
    CleanupStack::Pop( self );
    return self;
    }

CMdEAsynchronousFindAO* CMdEAsynchronousFindAO::NewLC( CMdEQuery& aQuery,
	    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    {
    CMdEAsynchronousFindAO* self =
        new ( ELeave ) CMdEAsynchronousFindAO( aQuery, aSessionImpl, aSession );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CMdEAsynchronousFindAO::ConstructL()
    {
    }

CMdEAsynchronousFindAO::CMdEAsynchronousFindAO( CMdEQuery& aQuery,
    CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession )
    : CActive( CActive::EPriorityStandard )
    , iQuery( aQuery )
    , iSessionImpl ( aSessionImpl )
    , iSession( aSession )
    , iCountResult( 0 )
    , iConditionBuffer( NULL )
    , iDistinctResults( NULL )
    {
    CActiveScheduler::Add( this );
    }


void CMdEAsynchronousFindAO::DoFindL( TUint aMaxCount, TUint aNotifyCount )
    {    
	CMdEObjectDef* objectDef = NULL;

	RPointerArray<CMdEObjectDef>* objectDefs = NULL;
	
    RPointerArray<CMdEPropertyDef>* propertyFilter = NULL;

	// get query's internal optimization flags
	TUint32 queryOptimizationFlags = 0;
	iQuery.Conditions().InternalQueryOptimizationFlags(queryOptimizationFlags);

	TQueryResultMode resultMode = iQuery.ResultMode();

    // set property filter and distinct
    if( EQueryTypeObject == iQuery.Type() )
    	{
    	CMdEObjectQuery& objectQuery = (CMdEObjectQuery&)iQuery;

		objectDef = &objectQuery.ObjectDef();
		
		objectDefs = objectQuery.ObjectDefs();

		// if result mode is objects with freetext
		// change it to normal items for server 
		// (freetext including to results is in query optimization flags)
		if( EQueryResultModeObjectWithFreetexts == resultMode )
			{
			resultMode = EQueryResultModeItem;
			queryOptimizationFlags |= EContainsFreetextCondition;
			}

    	if( EQueryResultModeItem == resultMode || 
    		EQueryResultModeDistinctValues == resultMode )
        	{
        	if( objectQuery.PropertyFilter().Count() > 0 )
        		{        		
        		propertyFilter = &objectQuery.PropertyFilter();
        		}
        	}
        }

    iConditionBuffer = 
    	CMdEQueryCriteriaSerialization::NewL( resultMode, iQuery.Type(),     										   
    										   iQuery.NamespaceDef(), 
    										   objectDef, 
    										   objectDefs, 
    										   aMaxCount, 
    										   0, // 0 offset because it's not used currently
    										   queryOptimizationFlags, 
    										   iQuery.Conditions(), iQuery.OrderRules(),
    										   propertyFilter );
    iSession.DoFindAsync( 
    	iQuery.GetQueryId(), 
        *iConditionBuffer,
        &iResultSize, 
        iStatus, 
        aNotifyCount );

    SetActive();
    }


CMdEAsynchronousFindAO::~CMdEAsynchronousFindAO()
    {
    Cancel(); // Causes call to DoCancel()

	delete iDistinctResults;
	iDistinctResults = NULL;

	delete iConditionBuffer;
	iConditionBuffer = NULL;

    iResultList.ResetAndDestroy();
    iResultList.Close();

    iIdResultList.Close();
    }


void CMdEAsynchronousFindAO::RunL()
    {
    // condition buffer can be safely removed now
    if( iConditionBuffer )
    	{
    	delete iConditionBuffer;
    	iConditionBuffer = NULL;
    	}

    // check if client side query is in error state (for example cancelled)
    if( iQuery.State() == CMdEQuery::EStateError )
    	{
    	TRAP_IGNORE( iSession.DoFindCancelL( &iQuery ) );
    	return;
    	}
    
    const TInt status = iStatus.Int();
    switch ( status )
        {
        case EAsyncFindSetReady:
        case EAsyncFindComplete:
            {            
            // fetch results from server
            CMdCSerializationBuffer* results = CMdCSerializationBuffer::NewLC(iResultSize());

            iSession.DoGetDataL( &iQuery, results, status );

			const TQueryResultMode resultMode = iQuery.ResultMode();

            if ( resultMode == EQueryResultModeItem || resultMode == EQueryResultModeObjectWithFreetexts )
                {
                iSessionImpl.DeserializeQueryResultL( *results, iResultList );
                }
            else if ( resultMode == EQueryResultModeId )
                {
                iSessionImpl.GetItemIdL( results, iIdResultList );
                }
            else if ( resultMode == EQueryResultModeCount )
                {
                iSessionImpl.GetCountL( results, iCountResult );
                }
            else if ( resultMode == EQueryResultModeDistinctValues )
            	{
            	if( iDistinctResults )
            		{            		
            		delete iDistinctResults;
            		iDistinctResults = NULL;
            		}

            	iDistinctResults = new (ELeave) CDesCArrayFlat( 16 );
            	iSessionImpl.GetDistinctValuesL( *results, *iDistinctResults );
            	}

            CleanupStack::PopAndDestroy( results );

            // notify the results
            DoNotifyL( ETrue, status );
            
            if ( status == EAsyncFindSetReady )
                {
                iResultSize = 0;
                
                // continue find, fetch next set
                iSession.DoFindContinue( &iQuery, &iResultSize, iStatus );
                SetActive();
                }
            }
            break;

        default:
            {
            // notify of an error
            DoNotifyL( EFalse, status );
            }
            break;
        }
    }

TInt CMdEAsynchronousFindAO::RunError(TInt aError)
	{
	// in error cases make sure that results are cleaned
	iResultList.Reset();
	
	iIdResultList.Reset();
	
	delete iDistinctResults;
	iDistinctResults = NULL;

	if( aError == KErrServerTerminated )
		{
		iSessionImpl.NotifyError( aError );
		}
	else
		{
		// cancel query if it's still running
		if( iQuery.State() != EAsyncFindComplete )
			{
			TRAP_IGNORE( iSession.DoFindCancelL( &iQuery ) );
			}
		}

    const TQueryType mode = iQuery.Type();

    switch( mode )
        {
        case EQueryTypeObject: 
            {
            CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;
            qi.DoNotifyCompleted( aError );
            break;
            }
        case EQueryTypeRelation:
            {
            CMdERelationQueryImpl& qi = (CMdERelationQueryImpl&)iQuery;
            qi.DoNotifyCompleted( aError );
            break;
            }
        case EQueryTypeEvent:
            {
            CMdEEventQueryImpl& qi = (CMdEEventQueryImpl&)iQuery;
            qi.DoNotifyCompleted( aError );
            break;
            }
        default:
            {
            return KErrNone;
            }
        }

	return KErrNone;
	}

void CMdEAsynchronousFindAO::DoNotifyResultsL( TInt aStatus )
    {
    // Notify result instances
    if ( ( aStatus == EAsyncFindSetReady ||
           aStatus == EAsyncFindComplete ) &&
           iResultList.Count() > 0)
        {
        const TQueryType mode = iQuery.Type();

        switch( mode )
            {
            case EQueryTypeObject:
                {
                CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iResultList );
                break;
                }
            case EQueryTypeRelation:
                {
                CMdERelationQueryImpl& qi = (CMdERelationQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iResultList );
                break;
                }
            case EQueryTypeEvent:
                {
                CMdEEventQueryImpl& qi = (CMdEEventQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iResultList );
                break;
                }
            default:
                {
                // Nothing to do
                }
            }

        // get rid of local results list
        iResultList.Reset();
        iResultList.Compress();
        }
    // Notify result ids
    else if ( ( aStatus == EAsyncFindSetReady ||
           aStatus == EAsyncFindComplete ) &&
           iIdResultList.Count() > 0)
        {
        const TQueryType mode = iQuery.Type();
        
        switch( mode )
            {
            case EQueryTypeObject:
                {
                CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iIdResultList );
                break;
                }
            case EQueryTypeRelation:
                {
                CMdERelationQueryImpl& qi = (CMdERelationQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iIdResultList );
                break;
                }
            case EQueryTypeEvent:
                {
                CMdEEventQueryImpl& qi = (CMdEEventQueryImpl&)iQuery;
                qi.DoNotifyResultsL( iIdResultList );
                break;
                }
            default:
                {
                // Nothing to do
                }
            }

        // get rid of local results list
        iIdResultList.Reset();
        }
    else if ( iQuery.ResultMode() == EQueryResultModeCount )
        {
        const TQueryType mode = iQuery.Type();
        
        switch( mode )
            {
            case EQueryTypeObject:
                {
                CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;
                qi.DoNotifyResults( iCountResult );
                break;
                }
            case EQueryTypeRelation:
                {
                CMdERelationQueryImpl& qi = (CMdERelationQueryImpl&)iQuery;
                qi.DoNotifyResults( iCountResult );
                break;
                }
            case EQueryTypeEvent:
                {
                CMdEEventQueryImpl& qi = (CMdEEventQueryImpl&)iQuery;
                qi.DoNotifyResults( iCountResult );
                break;
                }
            default:
                {
                // Nothing to do
                }
            }
        }
    else if ( ( aStatus == EAsyncFindSetReady ||
           aStatus == EAsyncFindComplete ) &&
           iDistinctResults && iQuery.ResultMode() == EQueryResultModeDistinctValues )
    	{
    	CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;    	
    	qi.DoNotifyResultsL( *iDistinctResults );

    	delete iDistinctResults;
    	iDistinctResults = NULL;
    	}
    }

void CMdEAsynchronousFindAO::DoNotifyL( TBool aNewResults, TInt aStatus )
	{
	if( aNewResults )
		{
		DoNotifyResultsL( aStatus );
		}

    // Notify complete
    if ( aStatus != EAsyncFindSetReady )
        {
        const TInt code = ( aStatus == EAsyncFindComplete ? KErrNone : aStatus );
        
        const TQueryType mode = iQuery.Type();

        switch( mode )
            {
            case EQueryTypeObject:
                {
                CMdEObjectQueryImpl& qi = (CMdEObjectQueryImpl&)iQuery;
                qi.DoNotifyCompleted( code );
                break;
                }
            case EQueryTypeRelation:
                {
                CMdERelationQueryImpl& qi = (CMdERelationQueryImpl&)iQuery;
                qi.DoNotifyCompleted( code );
                break;
                }
            case EQueryTypeEvent:
                {
                CMdEEventQueryImpl& qi = (CMdEEventQueryImpl&)iQuery;
                qi.DoNotifyCompleted( code );
                break;
                }
            default:
                {
                // Nothing to do
                }
            }
        }
    
    if( aStatus == KErrServerTerminated )
    	{
    	iSessionImpl.NotifyError( aStatus );
    	}
    }


void CMdEAsynchronousFindAO::DoCancel()
    {
    TRAP_IGNORE( iSession.DoFindCancelL( &iQuery ) );
    }

// End of File
