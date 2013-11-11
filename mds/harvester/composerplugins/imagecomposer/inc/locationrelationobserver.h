/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Image Composer's listener for changes in db location relations
 *
*/


#ifndef LOCATIONRELATIONOBSERVER_H
#define LOCATIONRELATIONOBSERVER_H

#include "mdeharvestersession.h"
#include "mdequery.h"

// forward declarations
class CMdENamespaceDef;
class CComposerImagePlugin;

/**
 *  CLocationRelationObserver
 * 
 */
class CLocationRelationObserver : public CBase, public MMdERelationObserver, public MMdEQueryObserver
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CLocationRelationObserver();

	/**
	 * Two-phased constructor.
	 */
	static CLocationRelationObserver* NewL( CMdESession* aSession, CComposerImagePlugin* aComposer );

protected:

	/**
	 * Called to notify the observer that new relations has been
	 * added/modified/removed in the metadata engine database.
	 *
	 * @param aSession session
	 * @param aType defines if relation was added/modified/remove
	 * @param aRelationIdArray IDs of relations
	 */

	void HandleRelationNotification(CMdESession& aSession,
			TObserverNotificationType aType,
			const RArray<TItemId>& aRelationIdArray);
	
    /**
     * Called to notify the observer that new results have been received 
     * in the query.
     * From MMdEQueryObserver.
     *
     * @param aQuery              Query instance that received new results.
     * @param aFirstNewItemIndex  Index of the first new item that was added
     *                            to the result item array.
     * @param aNewItemCount       Number of items added to the result item 
     *                            array.
     *
     * No implementation needed!
     */
    void HandleQueryNewResults( CMdEQuery& /*aQuery*/,
        TInt /*aFirstNewItemIndex*/, TInt /*aNewItemCount*/ );

    /**
     * Handle query results.
     * From MMdEQueryObserver.
     *
     * @param aQuery   Query instance that received new results.
     * @param aError   Error code.
     */
    void HandleQueryCompleted( CMdEQuery& aQuery, TInt aError );
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CLocationRelationObserver();

	/**
	 * default constructor for performing 2nd stage construction
	 */
	void ConstructL( CMdESession* aSession, CComposerImagePlugin* aComposer );
	
    /**
     * Query the related image object ids from MdE database.
     * @param aObjectIdArray  Array of Location object database ids.
     */
    void QueryRelationItemsL( );
	
	
private:

	/**
	 * Pointer to default Mde namespace.
	 */
	CMdENamespaceDef* iDefaultNamespace;

	/**
	 * Pointer to Mde session.
	 */
	CMdESession* iSession;

	/**
	 * Pointer to a composer.
	 */
	CComposerImagePlugin* iComposer;	
	
    /**
     * MdE query object to use for querying relations
     */
    CMdERelationQuery* iQuery;	
    
    TBool iQueryStarted;
    
    RArray<TItemId> iRelationIdArray;

	};

#endif // LOCATIONRELATIONOBSERVER_H

