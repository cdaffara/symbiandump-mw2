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


#ifndef __MDEQUERY_H__
#define __MDEQUERY_H__


#include <e32base.h>
#include <badesca.h>

#include <mdccommon.h>


/* Forward declarations. */
class CMdESession;
class CMdEQuery;
class CMdELogicCondition;
class TMdEOrderRule;
class CMdENamespaceDef;
class CMdEItem;
class CMdEInstanceItem;


/**
 * Observer interface for MdE database queries.
 * 
 * Example of doing a query to metadata database:
 * 
 * A class needs to implement MMdEQueryObserver interface if it is going to do a query to MdE database.
 * 
 * class CMdEQueryExample : public MMdEQueryObserver
 * {
 *		void HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex, TInt aNewItemCount);
 *		void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError);
 * 		...
 * 		CMdESession* iMdeSession; // session to MdE, created before trying to do the query
 * 		CMdEQuery* iQuery;
 * };
 * 
 * void CMdEQueryExample::DoQuery()
 * {
 * 		CMdENamespaceDef& defNS = iMdeSession->GetDefaultNamespaceDefL();
 *	    
 *	    // In this function we create a query with following conditions:
 *	    // 		Right object in relation must be a location object.
 *	    // 		Left object in relation must have id 6.
 * 
 * 		// First create an object query. We want to find location objects so let's give that
 * 		// as a condition to the query. 
 *	    CMdEObjectDef& rightObjDef = defNS.GetObjectDefL(
 *	        MdeConstants::Location::KLocationObject );
 *	    iQuery = iMdeSession->NewObjectQueryL( defNS, rightObjDef, this );
 *			    
 * 		// Result mode EQueryResultModeItem means we want the query to return items.
 * 		// Other options are: EQueryResultModeId, EQueryResultModeCount, 
 * 		// EQueryResultModeDistinctValues and EQueryResultModeObjectWithFreetexts.
 *	    iQuery->SetResultMode( EQueryResultModeItem );
 * 
 * 		// ELogicConditionOperatorAnd means we want all conditions to be true.
 *	    iQuery->Conditions().SetOperator( ELogicConditionOperatorAnd );
 *	
 * 		// Add a relation condition to the query. The location object is the right side object of
 * 		// the relation.
 *	    CMdERelationCondition& filterCond = iQuery->Conditions().
 *	        AddRelationConditionL( ERelationConditionSideRight );
 *			        
 *		// The object on the left side of the relation must have ID 6.
 *	    filterCond.LeftL().AddObjectConditionL( 6 );
 *			    
 *	    iQuery->FindL( 10, 1 ); // Start the query! The first parameter is maximum number of result items. 
 * 								// The second parameter is number of results per observer
 *								// notification. This query returns maximum of 10 location items
 * 								// and gives a notification (HandleQueryNewResults) for each.
 * }
 * 
 * void CMdEQueryExample::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
 * {
 * 		// query is completed
 * 		if( aQuery.Count() > 0 && aError == KErrNone )
 *		{
 *		// some items were found!
 *		}
 * }
 * 
 * void CMdEQueryExample::HandleQueryNewResults(CMdEQuery& aQuery, TInt aFirstNewItemIndex,
 *                                       TInt aNewItemCount)
 * {
 * 		// query is not yet completed but new results were found
 * }
 * 
 * @see CMdEQuery::FindL
 */
class MMdEQueryObserver 
    {
public:

    /**
     * Called to notify the observer that new results have been received 
     * in the query.
     *
     * @param aQuery              Query instance that received new results.
     * @param aFirstNewItemIndex  Index of the first new item that was added
     *                            to the result item array.
     * @param aNewItemCount       Number of items added to the result item 
     *                            array.
     */
    virtual void HandleQueryNewResults(CMdEQuery& aQuery,
                                       TInt aFirstNewItemIndex,
                                       TInt aNewItemCount) = 0;


            
    /**
     * Called to notify the observer that the query has been completed,
     * or that an error has occured.
     *
     * @param aQuery  Query instance.
     * @param aError  <code>KErrNone</code>, if the query was completed
     *                successfully. Otherwise one of the system-wide error 
     *                codes.
     */
    virtual void HandleQueryCompleted(CMdEQuery& aQuery, TInt aError) = 0;
    
    };


/** Default count for finding items. */
static const TUint KMdEQueryDefaultMaxCount = KMaxTUint;


/**
 * MdE database query. This is the abstract base class for all metadata engine
 * database queries. Instances of a query class own all the result items 
 * fetched from the database; when the query instance is destroyed, the 
 * results will be destroyed as well.
 *
 * If a query is restarted by calling FindL() after a previous query operation 
 * has been completed, any new results are appended to the end of the result 
 * item list. The previous results are not affected by subsequent calls to 
 * FindL().
 *
 * The query parameters (order rules, search conditions, property filters) 
 * must be configured before FindL() is called. FindL() may be called several
 * times, but the query parameters that were in effect for the first Find()
 * are used for all FindL()s.
 */

NONSHARABLE_CLASS(CMdEQuery) : public CBase
	{
public: 

    /** 
     * Query states.
     */
    enum TState
        {
        EStateFirst = 0x0000,
        
        /** The query has been created. The query parameters are specified
            during this state. */
        EStateNew,
        
        /** The query has been started with Find(). All the results received
            so far are available to the user. */
        EStateSearching,
        
        /** All the results have been found and they are available to 
            the user. */
        EStateCompleted,
        
        /** An error has occured. */
        EStateError,
        
        EStateLast
        };

    /* Constants. */
	
	/* Constructors and destructor. */

	/**
	 * Destructor.
	 */
	virtual ~CMdEQuery();


    /* Methods. */

    /**
	 * Returns the type of the query.
	 *
	 * @return  Query type.
	 */
	IMPORT_C TQueryType Type() const;

    /**
	 * Returns the namespace definition of the query.
	 *
	 * @return  Namespace definition.
	 */
	IMPORT_C CMdENamespaceDef& NamespaceDef() const;

    /**
	 * Returns the session of the query.
	 *
	 * @return  Session.
	 */
    IMPORT_C CMdESession& Session() const;

    /**
     * Returns the root of the condition tree.
     *
     * @return  Logic condition that acts as the root of the search conditions
     *          tree.
     */
    IMPORT_C CMdELogicCondition& Conditions() const;
    
    /**
     * Appends a new result ordering rule into the end of list of order rules.
     *
     * Example:
     *   CMdEObjectDef& objdef = iDefaultNamespaceDef->GetObjectDefL( MdeConstants::Object::KBaseObject );
     *   CMdEPropertyDef& propDef = objdef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
     *   TMdEOrderRule rule( *propDef, ETrue );
	 *   iQuery->AppendOrderRuleL( rule ); // iQuery is CMdEQuery*
     * 
     * @param aRule  Order rule.
     */
    IMPORT_C void AppendOrderRuleL(const TMdEOrderRule& aRule);

    /**
     * Insert a new result ordering rule into the list of order rules. 
     * The first rule is at position zero.
     * 
     * Example:
     *   CMdEObjectDef& objdef = iDefaultNamespaceDef->GetObjectDefL( MdeConstants::Object::KBaseObject );
     *   CMdEPropertyDef& propDef = objdef.GetPropertyDefL( MdeConstants::Object::KCreationDateProperty );
     *   TMdEOrderRule rule( *propDef, ETrue );
	 *   iQuery->InsertOrderRuleL( rule, 0 ); // iQuery is CMdEQuery*
     *
     * @param aRule  Order rule.
     * @param aPos   Position in the list of rules to insert into.
     */
    IMPORT_C void InsertOrderRuleL(const TMdEOrderRule& aRule, TInt aPos);
    
    /**
     * Returns the number of order rules currently defined.
     *
     * @return  Number of order rules.
     */
    IMPORT_C TInt OrderRuleCount() const;
    
    /**
     * Removes an order rule.
     *
     * @param aIndex  Index of the rule to remove.
     */
    IMPORT_C void RemoveOrderRule(TInt aIndex);

    /**
     * Gets an order rule.
     *
     * @param aIndex  Index of the rule to return.
     * @param aRule   Reference to the TMdEOrderRule where the rule is stored.
     */
    IMPORT_C TMdEOrderRule OrderRule(TInt aIndex) const;

    /**
	 * Starts a query operation and returns immediately. The observers of 
     * the query instance will be notified when the query is completed, or 
     * if it fails. The query parameters (order rules, search conditions, 
     * property filters) must be configured before FindL() is called. 
     *
     * The caller can perform a find operation in several steps by using a
     * sufficiently small maximum number of result items. Notifications 
     * about progress of query is returned is steps, continuing the previously 
     * started find operation.
     *
     * @param aMaxCount     Maximum number of result items. Defaults to 
     *                      unlimited. 
     * @param aNotifyCount  Maximum number of results per observer
     *                      notification. Defaults to unlimited.
     *
     * @leave  KErrNotReady  The query is in the Searching state.
     *
     * @panic  TMdEPanic::EQueryStateIllegalOperation  
     *         Query is in a state that prohibits calling this method.
	 */
    IMPORT_C void FindL(TUint aMaxCount = KMdEQueryDefaultMaxCount,
                        TUint aNotifyCount = KMdEQueryDefaultMaxCount);

    /**
	 * Cancels the currently running query operation. Does nothing if the 
     * query is not currently running. 
	 */
    IMPORT_C void Cancel();

    /**
     * Returns whether the query has been completed.
     *
     * @return  <code>ETrue</code>, if the query is not currently running.
     *          Otherwise <code>EFalse</code>.
     */
    IMPORT_C TBool IsComplete() const;

    /**
     * Returns the error code of the latest completed query. The same error
     * code has been passed to the query observer.
     *
     * @return  Error code.
     */
    IMPORT_C TInt Error() const;

	/**
	 * Returns the current state of the query.
	 *
	 * @return  Query state.
	 */
	IMPORT_C TState State() const;

    /**
	 * Returns the number of received result items. This can be called at any
     * time during the query instance's lifetime.
	 *
	 * @return  The number of results.
	 */
	IMPORT_C TInt Count() const;
	
    
    /**
	 * Returns one of the result items. 
	 * 
	 * Example:
	 *   void CExampleClass::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
	 *   {
	 *     CMdEItem& mdeItem = aQuery.ResultItem( 0 );
	 *     ...
	 *   }
	 * 
	 * @param aIndex index of the returned item. 
	 * @panics if aIndex is out of bounds
	 * @return  Result item. 
	 */
    IMPORT_C CMdEItem& ResultItem(TInt aIndex) const;

    /**
	 * Returns one of the result ids. 
	 * 
	 * Example:
	 *   void CExampleClass::HandleQueryCompleted( CMdEQuery& aQuery, TInt aError )
	 *   {
	 *     TItemId mdeItemId = aQuery.ResultId( 0 );
	 *     ...
	 *   }
	 * 
	 * @param aIndex index of the returned id.
	 * @panics if aIndex is out of bounds
	 * @return  Result id.
	 */
    IMPORT_C TItemId ResultId(TInt aIndex) const;

    /**
	 * Returns all of the result ids.
	 *
	 * @return  Result ids.
	 */
    IMPORT_C const RArray<TItemId>& ResultIds() const;

    /**
     * Returns one of the result items. Ownership of the item is transferred 
     * to the caller. The results array element at the specified index will 
     * still point to the result item.
     *
     * @param aIndex  Result index.
     *
     * @return  Pointer to result item.
     */
    IMPORT_C CMdEItem* TakeOwnershipOfResult(TInt aIndex);

    /**
     * Determines whether the query owns a result item.
     * @param aIndex index of the result item which ownership is checked.
     * @panics if aIndex is out of bounds
     * @return  <code>ETrue</code>, if the query owns the item. Otherwise
     *          <code>EFalse</code>.
     */        
    IMPORT_C TBool OwnsResult(TInt aIndex);
	
	
    /**
     * Adds a new observer.
     *
     * @param  aObserver to add.
     */
    IMPORT_C void AddObserverL(MMdEQueryObserver& aObserver);

    /**
     * Removes an observer.
     *
     * @param  aObserver to remove.
     */
    IMPORT_C void RemoveObserver(MMdEQueryObserver& aObserver);

    /**
     * Sets type of query results. Whether whole items or only IDs.
     * Default value is EModeItem.
     *
     * @param  aMode Determines type of query results. Can be set of
     *         instance items or set of item IDs
     */
    IMPORT_C void SetResultMode( TQueryResultMode aMode );
    
    /**
     * Returns type of query results, whether whole items or only IDs.
     *
     * @return Type of query results.
     */
    IMPORT_C TQueryResultMode ResultMode() const;
    
    /**
	 * Returns result object item
	 *
	 * @return  Result object item.
	 */
    IMPORT_C CMdEItem& ResultObjectItem() const;

    /**
	 * Returns one of the result distinct values
	 *
	 * @return  Result distinct value
	 */
    IMPORT_C TPtrC16 ResultDistinctValue(TInt aIndex) const;

	/**
	 * Returns order rules
	 *
	 * @return  Order rules
	 */
	RArray<TMdEOrderRule>& OrderRules();

	void SetQueryId( TUint32 aQueryId ) const
		{
		iQueryId = aQueryId;
		}

	TUint32 GetQueryId() const
		{
		return iQueryId;
		}

protected:

	/* Constructors. */

	/**
	 * Constructor. Note that new queries should be created using the factory
	 * methods in CMdESession.
	 *
	 * @param aType     Type of the query.
     * @param aSession
	 */
	CMdEQuery(TQueryType aType, CMdESession& aSession, CMdENamespaceDef& aNamespaceDef);

	/**
	 * Second-phase constructor. Creates the root node of the conditions tree.
	 */
	void QueryConstructL();


    /* Implementation methods. */

    /**
     * As Find().
     */
    virtual void DoFindL(TUint aMaxCount, TUint aNotifyCount) = 0;

    /**
     * As Cancel().
     */
    virtual void DoCancel();


    /* Notification methods. */

    /**
     * Appends new item results to the results array. This query instance takes 
     * ownership of the items. 
	 *
	 * If a leave occurs, the query won't take ownership of any of the new 
     * result items. The caller is responsible for destroying the result
     * items in this case.
     *
     * This operation is atomic: either all of the new results are added to
     * the results array and the query takes ownership of them, or none of
     * results are added to the results array.
     *
     * @param aNewResults contains result items
     */
    virtual void NotifyNewResultsL(const RPointerArray<CMdEInstanceItem>& aNewResults);

    /**
     * Appends new ID results to the results array.
     * 
     * @param aNewResults contains results from ID query
     */
    virtual void NotifyNewResultsL(const RArray<TItemId>& aNewResults);

    /**
     * Appends distinct value results to the results array.
     * 
     * @param aResults contains results from distinct value query
     */
    virtual void NotifyNewResultsL( const CDesCArray& aNewResults );


    /**
     * Gets result from count query.
     * 
     * @param aResults contains result from count query
     */
    virtual void NotifyNewResults(TUint32 aResult);
        
    /**
     * Notifies observers that the query was completed.
     */
    virtual void NotifyCompleted(TInt aError);


	/* Utility methods. */
	
	/**
	 * Sets the state of the query.
	 *
	 * @param aState  Query state.
	 */
    void SetState(TState aState);
    
    /** 
     * Panics if the state of the query is the specified state.
     *
     * @param aState  Query state.
     *
     * @panic TMdEPanic::EQueryStateIllegalOperation  The query was not 
     *        in the given state.
     */
    void AssertInState(TState aState);
    
    /** 
     * Panics if the state of the query is not the specified state.
     *
     * @param aState  Query state.
     *
     * @panic TMdEPanic::EQueryStateIllegalOperation  The query was in the 
     *        given state.
     */
    void AssertNotInState(TState aState);

    
private:

    /* Private data structures. */

    //  Result item for instances
    struct TResult
        {
        /** Result item. */
        CMdEItem* iItem;

        /** Query has the ownership of the result item. */
        TBool iOwned;

        /** Constructor for initializing the struct. */
        TResult(CMdEItem* aItem) : iItem(aItem), iOwned(EFalse) {}
        };


private:

    /* Private methods. */

    /**
     * Appends new result items into the results array. Does not transfer
     * ownership of the new results to the query.
     *
     * @param aNewResults  Array of result items.
     */
    void AppendResultsL(const RPointerArray<CMdEInstanceItem>& aNewResults);

    void AppendResultsL(const RArray<TItemId>& aNewResults);

	/*void AppendResultsL(CMdEInstanceItem* aObjectResult,
    	const RPointerArray<CMdEInstanceItem>& aRelationResults, 
    	const RPointerArray<CMdEInstanceItem>& aEventResults);*/
    	
    void AppendResultsL(const CDesCArray& aNewResults);
    	
private:

	mutable TUint32 iQueryId;

    /** The session of the query. */
    CMdESession& iSession;

	/** The namespace definition of the query */
	CMdENamespaceDef& iNamespaceDef;
    
    /** Type of the query. */
    TQueryType iType;

    /** Type of results. */
    TQueryResultMode iResultMode;

    /** State of the query. */    
    TState iState;

    /** Latest error code. */
    TInt iError;

    /** Root node of the conditions tree.  Always present. */
    CMdELogicCondition* iConditions;
    
    /** Array of result ordering rules. */
    RArray<TMdEOrderRule> iOrderRules;

    /** Instance result items. */
    RArray<TResult> iResults;
    
    /** ID result items. */
    RArray<TItemId> iIdResults;

    /** Instance result object item */
    TResult iObjectResult;
	
    /** Results of count query */
    TInt iCountResult;

    /** Observers. */
    RPointerArray<MMdEQueryObserver> iObservers;
    
    CDesCArray* iDistinctResults;
    
    TBool iDestroyed;
    };

    
// includes only for client more convinient usage
	#include <mdeobjectquery.h>
	#include <mderelationquery.h>
	#include <mdeeventquery.h>
	#include <mdelogiccondition.h>
	#include <mdeobjectcondition.h>
	#include <mderelationcondition.h>
	#include <mdeeventcondition.h>
	#include <mdepropertycondition.h>
	#include <mderange.h>
	#include <mdeorderrule.h>
// end

#endif  // __MDEQUERY_H__
