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
* Description:  A singleton that provides system unique ID's*
*/

#ifndef __MDSINDEXER_H__
#define __MDSINDEXER_H__

// INCLUDE FILES
#include <e32base.h>
#include <e32cmn.h>

#include "mdccommon.h"

// FORWARD DECLARATIONS
class RRowData;


// CLASS DECLARATION
/**
* A static class that is used to provide system unique
* ID's for database storage purposes. 
*/
class MMdSIndexer
    {
    public: // Public functions

    	/**
    	 * Gets last object id from DB
    	 * MUST be called once, when connection to DB is established
    	 */
    	static TItemId StartIndexL();

    	/**
	     * Provides a new system unique ID
	     * @return a new system-wide unique ID
	     */
	    static TItemId GetIndex( );

	    /**
	     * Reverts system ID to previous state
	     * used when add item failed
	     */
	    static void RevertIndex( );

	    /**
	     * Executes a SQL clause and returns the ID of the last object in DB.
	     * @param aSqlClause SQL clause to execute
	     * @param aRowData query parameters
	     * @return result of last_insert_rowid()
	     */
	    static TItemId ExecuteAndGetIndexL(const TDesC &aSqlClause, const RRowData &aRowData );	    	

	private:
		static TItemId GetIndexL();

		static void GetLastItemIdL();

		static TItemId iId;
	};

class RSQLIndex
	{
	public:
		RSQLIndex();

		const TItemId& GetId() const;

		void Commit();

		void Close();

	private:
		const TItemId iId;
		TBool iCommit;
	};

#endif // __MDSINDEXER_H__
