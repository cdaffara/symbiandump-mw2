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
* Description:  Implementation of object, relation and event query
*
*/


#ifndef __MDEQUERYIMPL_H__
#define __MDEQUERYIMPL_H__

#include "mdeobjectquery.h"
#include "mderelationquery.h"
#include "mdeeventquery.h"

// Forward declaration
class CMdESessionImpl;
class CMdENamespaceDef;
class CMdEObjectDef;
class RMdEEngineSession;
class CMdEAsynchronousFindAO;

/**
 * Object query implementation.
 */
NONSHARABLE_CLASS(CMdEObjectQueryImpl) : public CMdEObjectQuery
	{
public:

    /* Constructors and destructor. */
	/**
	 * Two-phased constructor.
	 */
	static CMdEObjectQueryImpl* NewL( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef, 
			CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs,
			RMdEEngineSession& aSession );
	
	static CMdEObjectQueryImpl* NewLC( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef, 
			CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs,
			RMdEEngineSession& aSession );

	/**
	 * Destructor.
	 */
	virtual ~CMdEObjectQueryImpl();


    /* Methods. */

    /**
     * As in CMdEQuery.
	 */
    void DoFindL( TUint aMaxCount, TUint aNotifyCount );

    /**
     * Notifies results
	 */
    void DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList );

    /**
     * Notifies results of id query
	 */
    void DoNotifyResultsL( RArray<TItemId>& aResultIdList );

    /**
     * Notifies result of distinct values query
     */
    void DoNotifyResultsL( CDesCArray& aResults );

    /**
     * Notifies result of count query
	 */
    void DoNotifyResults( TInt aResultCount );
    
    /**
     * Notifies query completed
	 */
    void DoNotifyCompleted( TInt aError );

protected:
    
    /**
     * As in CMdEQuery.
     */
    void DoCancel();
    
private:
	/**
	 * Constructor. Note that new queries should be created using the factory
	 * @param aSessionImpl the session
	 */
	CMdEObjectQueryImpl( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef, 
			CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs );
	
	/**
	 * Second-phase constructor.
	 * @param aSession reference to the engine session
	 */
	void ConstructL( CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

    
private:

	/** Asynchronous find support. */
	CMdEAsynchronousFindAO* iAsyncFind;
    };

/**
 * Relation query stub implementation.
 */
class CMdERelationQueryImpl
    : public CMdERelationQuery
	{
public:

    /* Constructors and destructor. */
	/**
	 * Two-phased constructor.
	 */
	static CMdERelationQueryImpl* NewL( CMdESessionImpl& aSessionImpl,
			CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession );
	
	static CMdERelationQueryImpl* NewLC( CMdESessionImpl& aSessionImpl,
			CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession );
	
	/**
	 * Destructor.
	 */
	virtual ~CMdERelationQueryImpl();


    /* Methods from CMdERelationQuery. */

    /**
     * As in CMdEQuery.
	 */
    void DoFindL( TUint aMaxCount, TUint aNotifyCount );

    /**
     * Notifies results
	 */
    void DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList );

    /**
     * Notifies results of id query
	 */
    void DoNotifyResultsL( RArray<TItemId>& aResultIdList );

    /**
     * Notifies result of count query
	 */
    void DoNotifyResults( TUint32 aResultCount );

    /**
     * Notifies query completed
	 */
    void DoNotifyCompleted( TInt aError );

protected:
 
    /**
     * As in CMdEQuery.
     */
    void DoCancel();
    
private:
	/**
	 * Constructs a new relation query in the specified session.
	 * @param aSessionImpl the session
	 */
	CMdERelationQueryImpl( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef );
	
	/**
	 * Second-phase constructor.
	 * @param aSession reference to the engine session
	 */
	void ConstructL( CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

    
private:

	/** Asynchronous find support. */
	CMdEAsynchronousFindAO* iAsyncFind;
    };

/**
 * Event query implementation.
 */
class CMdEEventQueryImpl: public CMdEEventQuery
	{
public:

    /* Constructors and destructor. */

	static CMdEEventQueryImpl* NewL( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession );

	static CMdEEventQueryImpl* NewLC( CMdESessionImpl& aSessionImpl,
		CMdENamespaceDef& aNamespaceDef, RMdEEngineSession& aSession );

	/**
	 * Destructor.
	 */
	virtual ~CMdEEventQueryImpl();


    /* Methods from CMdEEventQuery. */

    /**
     * As in CMdEQuery.
	 */
    void DoFindL( TUint aMaxCount, TUint aNotifyCount );

    /**
     * Notifies results
	 */
    void DoNotifyResultsL( RPointerArray<CMdEInstanceItem>& aResultList );

    /**
     * Notifies results of id query
	 */
    void DoNotifyResultsL( RArray<TItemId>& aResultIdList );

    /**
     * Notifies result of count query
	 */
    void DoNotifyResults( TUint32 aResultCount );

    /**
     * Notifies query completed
	 */
    void DoNotifyCompleted( TInt aError );

protected:
    
    /**
     * As in CMdEQuery.
     */
    void DoCancel();
    
private:
	/**
	 * Constructs a new event query in the specified session.
	 * @param aSessionImpl the session
	 */
	CMdEEventQueryImpl( CMdESessionImpl& aSessionImpl, CMdENamespaceDef& aNamespaceDef );
	
	/**
	 * Second-phase constructor. 
	 * @param aSession reference to the engine session
	 */
	void ConstructL( CMdESessionImpl& aSessionImpl, RMdEEngineSession& aSession );

    
private:

	/** Asynchronous find support. */
	CMdEAsynchronousFindAO* iAsyncFind;
	};

#endif  // __MDEQUERYIMPL_H__
