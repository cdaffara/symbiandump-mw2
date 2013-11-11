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
* Description:  Interface for object query
*
*/


#ifndef __MDEOBJECTQUERY_H__
#define __MDEOBJECTQUERY_H__


#include <mdequery.h>

/* Forward declarations. */
class CMdEObject;
class CMdEObjectDef;
class CMdEPropertyDef;
class CMdEObjectDef;
class CMdESession;

/**
 * A query that fetches a set of objects from the metadata engine database.
 */
NONSHARABLE_CLASS(CMdEObjectQuery) : public CMdEQuery
	{
public:
	
	/* Constructors and destructor. */

	/**
	 * Destructor.
	 */
	virtual ~CMdEObjectQuery();


	/* Methods. */

    
    /**
    * Adds new propertyfilter. Propertyfilters determines which properties
    * are included in result.
    
    * @param aPropertyDef property definition of property which is wanted to
      be part of result.
    */
    IMPORT_C void AddPropertyFilterL( const CMdEPropertyDef* aPropertyDef );
    
    /**
    Clears propertyfilters. After call to this function all properties are 
    returned unless call to <code>AddPropertyFilterL</code> is done.
    */
    
    IMPORT_C void ResetPropertyFilter();

    /**
	 * Returns one of the result objects.
	 *
	 * @return  The result object that was requested. The query retains 
     *          ownership of the returned object.
	 */
	IMPORT_C CMdEObject& Result(TInt aIndex) const;

    /**
	 * Returns query's object definition.
	 *
	 * @return  object definition.
	 */
	IMPORT_C CMdEObjectDef& ObjectDef() const;

public:
	RPointerArray<CMdEPropertyDef>& PropertyFilter();	

	RPointerArray<CMdEObjectDef>* ObjectDefs() const;

protected:

	/* Constructors. */

    /**
     * Constuctor.
     *
     * @param aSession  Session.
     */
    CMdEObjectQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef, 
    		CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs);

	/**
	 * Second-phase constructor.
	 */
	void ObjectQueryConstructL();

    virtual void DoCancel();

private:

    /* Private methods. */


private:

    /** 
     * Properties to include when fetching results. Empty, if
     * all available properties should be fetched. 
     */
    RPointerArray<CMdEPropertyDef> iPropertyFilters;

	/**
	 * Query's object definition.
	 */
	CMdEObjectDef& iObjectDef;

	/**
	 * Query's object definition.
	 */
	RPointerArray<CMdEObjectDef>* iObjectDefs;
	
	/**
	 * Speed optimization to drop freetexts from results.
	 */
	TBool iFreetextsInResult;
    };

#endif  // __MDEOBJECTQUERY_H__
