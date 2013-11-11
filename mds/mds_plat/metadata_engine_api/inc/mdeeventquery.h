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
* Description:  Represents event query
*
*/


#ifndef __MDEEVENTQUERY_H__
#define __MDEEVENTQUERY_H__


#include <mdequery.h>


/* Forward declarations. */
class CMdEEvent;
class CMdENamespaceDef;

/**
 * A query that retrieves a set of events from the metadata engine database.
 * Most functionality is provided by the CMdEQuery base class.
 */
NONSHARABLE_CLASS(CMdEEventQuery) : public CMdEQuery
	{
public:
	
	/**
	 * Destructor.
	 */
	virtual ~CMdEEventQuery();


	/* Methods. */

    /**
	 * Returns one of the result events.
     * 
     * @param aIndex  Index number of the result item.
	 *
	 * @return  The result event that was requested. The query retains 
     *          ownership of the returned event item.
	 */
	IMPORT_C CMdEEvent& Result(TInt aIndex) const;


protected:

	/* Constructors. */

	/**
	 * Constructor. 
	 *
	 * @param aSession  Session.
	 */
	CMdEEventQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef);

	/**
	 * Second-phase constructor.
	 */
	void EventQueryConstructL();
	
    virtual void DoCancel();
    };

#endif  // __MDEEVENTQUERY_H__
