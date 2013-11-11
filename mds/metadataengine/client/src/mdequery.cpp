/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Query base class
*
*/


#include "mdequery.h"

#include "mdeorderrule.h"
#include "mdeinstanceitem.h"
#include "mdelogiccondition.h"
#include "mdenamespacedef.h"
#include "mdepanic.h"


CMdEQuery::CMdEQuery(TQueryType aType, CMdESession& aSession, CMdENamespaceDef& aNamespaceDef)
        : iQueryId( 0 ), iSession(aSession), iNamespaceDef(aNamespaceDef), iType(aType), 
          iResultMode(EQueryResultModeItem), iState(EStateNew), iError(KErrNone), 
          iObjectResult(NULL), iCountResult(0), iDistinctResults(NULL), iDestroyed(EFalse)
    {
    }


void CMdEQuery::QueryConstructL()
    {
    iConditions = CMdELogicCondition::NewL(ELogicConditionOperatorAnd);
    }


CMdEQuery::~CMdEQuery()
	{
    iDestroyed = ETrue;
	Cancel();
  
    // This will destroy the entire conditions tree.
   	delete iConditions;
   	iConditions = NULL;
   	
    // Destroy all result items this query instance owns.
   	for( TInt i = iResults.Count() - 1; i >=0; i-- )
        {
        if(iResults[i].iOwned)
            {
            delete iResults[i].iItem;
            iResults[i].iItem = NULL;
            }
        }

    // Destroy object result item this query instance owns  
    if(iObjectResult.iItem)
        {
        if(iObjectResult.iOwned)
        	{
        	delete iObjectResult.iItem;
        	iObjectResult.iItem = NULL;
        	}
        }

	iCountResult = 0;

	iResults.Reset();
	iResults.Close();
	iIdResults.Reset();
	iIdResults.Close();

    iOrderRules.Close();
    iObservers.Close();

   	delete iDistinctResults;
   	iDistinctResults = NULL;
	}


EXPORT_C TQueryType CMdEQuery::Type() const
    {
    return iType;
    }

EXPORT_C CMdENamespaceDef& CMdEQuery::NamespaceDef() const
	{
	return iNamespaceDef;
	}
    
EXPORT_C CMdESession& CMdEQuery::Session() const
    {
    return iSession;
    }


EXPORT_C CMdELogicCondition& CMdEQuery::Conditions() const
    {
    __ASSERT_DEBUG(iConditions , 
                    TMdEPanic::Panic(TMdEPanic::EInternal));
    return *iConditions;
    }
    
    
EXPORT_C void CMdEQuery::AppendOrderRuleL(const TMdEOrderRule& aRule)
    {
    AssertInState(EStateNew);
    iOrderRules.AppendL( aRule );
    }


EXPORT_C void CMdEQuery::InsertOrderRuleL(const TMdEOrderRule& aRule, TInt aPos)
    {
    AssertInState(EStateNew);
    User::LeaveIfError( iOrderRules.Insert(aRule, aPos) );   
    }


EXPORT_C TInt CMdEQuery::OrderRuleCount() const
    {
    return iOrderRules.Count();    
    }


EXPORT_C void CMdEQuery::RemoveOrderRule(TInt aIndex)
    {
    AssertInState(EStateNew);
    iOrderRules.Remove(aIndex);
    }


EXPORT_C TMdEOrderRule CMdEQuery::OrderRule(TInt aIndex) const
    {
    return iOrderRules[aIndex];        
    }
    

EXPORT_C void CMdEQuery::FindL(TUint aMaxCount, TUint aNotifyCount)
    {
    AssertNotInState(EStateError);

	// leave if query already running    
    if(iState == EStateSearching)
        {
        User::Leave(KErrNotReady);
        }
    
	// clear old results
	// Destroy all result items this query instance owns.
    for( TInt i = iResults.Count() - 1; i >=0; i-- )
        {
        if(iResults[i].iOwned)
            {
            delete iResults[i].iItem;
            iResults[i].iItem = NULL;
            }
        }
	iResults.Reset();

    // Destroy object result item this query instance owns  
    if( iObjectResult.iItem )
        {
        if( iObjectResult.iOwned )
        	{
        	delete iObjectResult.iItem;
        	iObjectResult.iItem = NULL;
        	}
        }

	iIdResults.Reset();

    if( iDistinctResults )
    	{    	
    	delete iDistinctResults;
    	iDistinctResults = NULL;
    	}

	iCountResult = 0;
	// end of clearing

    DoFindL(aMaxCount, aNotifyCount);
    }


EXPORT_C void CMdEQuery::Cancel()
    {
    // is query incomplete before canceling
    TBool incomplete = !IsComplete();
    
    if( incomplete )
    	{
    	DoCancel();
    	}

	if( iConditions )
		{		
    	iConditions->SetLocked( EFalse );
		}
    
    if( incomplete || IsComplete() == EFalse )
    	{
    	iState = EStateError;
    	if( !iDestroyed )
    	    {
            NotifyCompleted( KErrCancel );
    	    }
    	}
    }


EXPORT_C TBool CMdEQuery::IsComplete() const
    {
    return iState != EStateSearching ? ETrue : EFalse;
    }


EXPORT_C TInt CMdEQuery::Error() const
    {
    return iError;
    }


EXPORT_C TInt CMdEQuery::Count() const
    {
    if ( iResultMode == EQueryResultModeItem || iResultMode == EQueryResultModeObjectWithFreetexts )
        {
        return iResults.Count();
        }
    else if ( iResultMode == EQueryResultModeId )
        {
        return iIdResults.Count();
        }
    else if ( iResultMode == EQueryResultModeDistinctValues )
    	{
    	if( iDistinctResults )
    		{    		
    		return iDistinctResults->Count();
    		}
    	else
    		{
    		return 0;
    		}
    	}
    else // count query
        {
        return iCountResult;
        }
    }

EXPORT_C CMdEItem& CMdEQuery::ResultItem(TInt aIndex) const
    {
    return *iResults[aIndex].iItem;
    }


EXPORT_C TItemId CMdEQuery::ResultId(TInt aIndex) const
    {
    return iIdResults[aIndex];
    }

EXPORT_C const RArray<TItemId>& CMdEQuery::ResultIds() const
	{
	return iIdResults;
	}

EXPORT_C CMdEItem* CMdEQuery::TakeOwnershipOfResult(TInt aIndex)
    {
    // Panic if an item is taken twice.
    __ASSERT_DEBUG(iResults[aIndex].iOwned,
                    TMdEPanic::Panic(TMdEPanic::EQueryDoesntOwnResult));

    // Applies only instance ownership
    if ( iResultMode == EQueryResultModeItem || iResultMode == EQueryResultModeObjectWithFreetexts )
        {
        iResults[aIndex].iOwned = EFalse;
        return static_cast<CMdEItem*>( iResults[aIndex].iItem );
        }
    else
    	{
    	return NULL;
    	}
    }


EXPORT_C TBool CMdEQuery::OwnsResult(TInt aIndex)
    {
    return iResults[aIndex].iOwned;
    }


EXPORT_C CMdEQuery::TState CMdEQuery::State() const
    {
    return iState;
    }

void CMdEQuery::DoCancel()
    {
    SetState( EStateError );
    }

void CMdEQuery::SetState(TState aState)
    {
    if(aState == iState)
        {
        // Already in the specified state.
        return;
        }
        
    // It's impossible to get out of the Error state.
    AssertNotInState(EStateError);

    // It's impossible to return to the New state.
    __ASSERT_ALWAYS(aState > EStateFirst && aState < EStateLast && 
                    aState != EStateNew, 
                    TMdEPanic::Panic(TMdEPanic::EInternal));

    // Move out of the old state.
    switch(iState)
        {
        case EStateNew:
            // Moving out of the New state causes the search conditions to 
            // be locked.
            iConditions->SetLocked();
            break;    
            
        case EStateSearching:
            // Searching has been completed.
            break;

        case EStateCompleted:
            // Searching is been restarted.
            break;
            
        default:
            break;
        }

    // Set the new state.
    iState = aState;

    // Move into the new state.
    switch(iState)
        {
        case EStateSearching:
            // Searching has been started.
            break;

        case EStateCompleted:
            // Searching has been completed.
            break;

        case EStateError:
            // The query has entered the Error state.
            break;
            
        default:
            break;
        }    
    }


void CMdEQuery::AssertInState(TState aState)
    {
    __ASSERT_ALWAYS(aState == iState,
                    TMdEPanic::Panic(TMdEPanic::EQueryStateIllegalOperation));
    }
    

void CMdEQuery::AssertNotInState(TState aState)
    {
    __ASSERT_ALWAYS(aState != iState,
                    TMdEPanic::Panic(TMdEPanic::EQueryStateIllegalOperation));
    }
    

void CMdEQuery::NotifyNewResultsL(const RPointerArray<CMdEInstanceItem>& aNewResults)
    {
    __ASSERT_ALWAYS(iState != EStateNew && iState != EStateError,
                    TMdEPanic::Panic(TMdEPanic::EInternal));

	TInt firstNewItemIndex = iResults.Count();
	TInt i = 0;

    // Query gets ownership of the results only if don't leave.
    // If a leave occurs, the results array is restored to its original
    // condition before any new results were added.

    const TInt oldCount = iResults.Count();

    TRAPD(err, AppendResultsL(aNewResults));
    if (err != KErrNone)
        {
        // Cleanup: remove the ones that were added.
        while (iResults.Count() > oldCount)
            {
            iResults.Remove(iResults.Count() - 1);
            }
        // Leave with the same error.
        User::Leave(err);
        }

    // After this no leave can occur.

    const TInt newCount = aNewResults.Count();
    
    // Transfer ownership of the results to the query.
    for (i = 0; i < newCount; i++)
        {
        iResults[firstNewItemIndex + i].iOwned = ETrue;
        }
    
    for( TInt i = iObservers.Count() - 1; i >=0; i-- )
		{
		MMdEQueryObserver* observer = iObservers[i];
		observer->HandleQueryNewResults( *this, firstNewItemIndex, newCount );
		}
    }


void CMdEQuery::NotifyNewResultsL(const RArray<TItemId>& aNewResults)
    {
    __ASSERT_ALWAYS(iState != EStateNew && iState != EStateError,
                    TMdEPanic::Panic(TMdEPanic::EInternal));

	TInt firstNewItemIndex = iIdResults.Count();

    const TInt oldCount = iIdResults.Count();

    TRAPD(err, AppendResultsL(aNewResults));
    if (err != KErrNone)
        {
        // Cleanup: remove the ones that were added.
        while (iIdResults.Count() > oldCount)
            {
            iIdResults.Remove(iIdResults.Count() - 1);
            }
        // Leave with the same error.
        User::Leave(err);
        }
    
    for( TInt i = iObservers.Count() - 1; i >=0; i-- )
		{
		MMdEQueryObserver* observer = iObservers[i];
		observer->HandleQueryNewResults(*this, firstNewItemIndex, 
                                        aNewResults.Count());
		}
    }

void CMdEQuery::NotifyNewResultsL( const CDesCArray& aNewResults )
    {
	TInt firstNewItemIndex = aNewResults.Count();

	TInt oldCount = 0;
	if( iDistinctResults )
		{
		oldCount = iDistinctResults->Count();
		}
	else
		{
	    iDistinctResults = new (ELeave) CDesCArrayFlat( 8 );
		}

    TRAPD( err, AppendResultsL( aNewResults ) );
    if( err != KErrNone )
        {
        // Cleanup: remove the ones that were added.
        while( iDistinctResults->Count() > oldCount )
            {
            iDistinctResults->Delete( iDistinctResults->Count() - 1 );
            }
        // Leave with the same error.
        User::Leave(err);
        }

    for( TInt i = iObservers.Count() - 1; i >=0; i-- )
		{
		MMdEQueryObserver* observer = iObservers[i];
		observer->HandleQueryNewResults( *this, firstNewItemIndex, 
                                         aNewResults.Count() );
		}
    }

void CMdEQuery::NotifyNewResults(TUint32 aResult)
	{
    // Don't call observer here, since only one result
    iCountResult = aResult;
    }

void CMdEQuery::AppendResultsL(const RPointerArray<CMdEInstanceItem>& aNewResults)
    {
    const TInt newResultCount = aNewResults.Count();
    iResults.ReserveL( iResults.Count() + newResultCount );

	for (TInt i = 0; i < newResultCount; ++i)
        {
        // At this point the ownership isn't transferred yet.
        TResult result( aNewResults[i] );
		iResults.AppendL( result );
		}
    }

void CMdEQuery::AppendResultsL(const RArray<TItemId>& aNewResults)
    {
    const TInt newResultCount = aNewResults.Count();
    iIdResults.ReserveL( iResults.Count() + newResultCount );

	for (TInt i = 0; i < newResultCount; ++i)
        {
        // Add new results to query's list
		iIdResults.AppendL( aNewResults[i] );
		}
    }

void CMdEQuery::AppendResultsL( const CDesCArray& aNewResults )
    {
    const TInt count = aNewResults.Count();
    
    for ( TInt i = 0; i < count; ++i )
    	{
    	iDistinctResults->AppendL( aNewResults[i] );
    	}
    }
    
void CMdEQuery::NotifyCompleted(TInt aError)
    {
    // The implementation must move out of the Searching state before
    // notifying the base class.
    __ASSERT_ALWAYS(iState != EStateSearching, TMdEPanic::Panic(TMdEPanic::EInternal));

    // Update the latest error code.
    iError = aError;
    for (TInt i = iObservers.Count() - 1; i >= 0; --i)
        {
        MMdEQueryObserver* observer = iObservers[i];
        if( observer )
            {
            observer->HandleQueryCompleted(*this, aError);
            }
        }
    }


EXPORT_C void CMdEQuery::AddObserverL(MMdEQueryObserver& aObserver)
	{
	__ASSERT_ALWAYS(iObservers.Find(&aObserver) == KErrNotFound,
				    TMdEPanic::Panic(TMdEPanic::EQueryDuplicateObserver));

	iObservers.AppendL( &aObserver );
	}


EXPORT_C void CMdEQuery::RemoveObserver(MMdEQueryObserver& aObserver)
	{
	TInt index = iObservers.Find(&aObserver);

	__ASSERT_ALWAYS(index != KErrNotFound,
				    TMdEPanic::Panic(TMdEPanic::EQueryUnknownObserver));

	iObservers.Remove(index);
	}


EXPORT_C void CMdEQuery::SetResultMode( TQueryResultMode aMode )
    {    
    iResultMode = aMode;
    }

EXPORT_C TQueryResultMode CMdEQuery::ResultMode() const
    {
    return iResultMode;
    }
EXPORT_C CMdEItem& CMdEQuery::ResultObjectItem() const
    {
    return *iObjectResult.iItem;
    }

EXPORT_C TPtrC16 CMdEQuery::ResultDistinctValue(TInt aIndex) const
	{
	return (*iDistinctResults)[aIndex];
	}

RArray<TMdEOrderRule>& CMdEQuery::OrderRules()
	{
	return iOrderRules;
	}
