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
* Description:  ?Description
*
*/


#ifndef __MDERELATIONQUERY_H__
#define __MDERELATIONQUERY_H__


#include <mdequery.h>


/* Forward declarations. */
class CMdERelation;
class CMdENamespaceDef;
class CMdESession;

/**
 * Query that fetches a set of relation items from the metadata engine 
 * database.
 */
NONSHARABLE_CLASS(CMdERelationQuery) : public CMdEQuery
	{
public:
	
	/* Constructors and destructor. */

    /**
	 * Destructor.
	 */
	virtual ~CMdERelationQuery();


	/* Methods. */
 
    /**
	 * Returns one of the result objects.
	 *
	 * @return  The result object that was requested. The query retains 
     *          ownership of the returned object.
	 */
	IMPORT_C CMdERelation& Result(TInt aIndex) const;


protected:

	/* Constructors. */
	
	/**
	 * Constructor. 
	 *
	 * @param aType  type of the condition node
	 */
	CMdERelationQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef);

	/**
	 * Second-phase constructor.
	 */
	void RelationQueryConstructL();

    virtual void DoCancel();

private:

    /* Private methods. */


private:

    };

#endif  // __MDERELATIONQUERY_H__
