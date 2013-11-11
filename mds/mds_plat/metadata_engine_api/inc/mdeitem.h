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


#ifndef __MDEITEM_H__
#define __MDEITEM_H__


#include <e32base.h>

#include <mdccommon.h>

/** 
 * Internal identifier for metadata engine database items. Must always be
 * different from 0.
 */
class CMdESession;

/**
 * Abstract base class for all items stored in the metadata engine database.
 */
NONSHARABLE_CLASS(CMdEItem) : public CBase
	{
public:
	
	/**
	 * Destructor.
	 */
	virtual ~CMdEItem();


	/* Methods. */

	/**
	 * Returns whether the item belongs to a session.
	 *
	 * @return  <code>ETrue</code>, if the item belongs to a session;
	 *          <code>EFalse</code>, otherwise
	 */
	IMPORT_C TBool BelongsToSession() const;

	/**
	 * Returns the session the item belongs to. Panics if
	 * the item does not belong to any session.
	 *
	 * @return  session that the item belongs to
	 */
	IMPORT_C CMdESession& Session() const;

	/**
	 * Returns whether the item is stored in the database.
	 *
	 * @return  <code>ETrue</code>, if the item is in the database;
	 *          <code>EFalse</code>, otherwise
	 */
	IMPORT_C TBool InDatabase() const;

	/**
	 * Returns the internal database identifier of the item. 
	 *
	 * @return  internal database identifier or <code>KNoId</code> if item is not stored in database.
	 */
	IMPORT_C TItemId Id() const;

	/**
	 * Sets that the internal database identifier of the object.
	 *
	 * @param aId  internal database identifier
	 */
	void SetId(TItemId aId);

	/**
	 * Sets the session the item belongs to. 
	 *
	 * @param aSession  session
	 */
	void SetSession(CMdESession& aSession);

protected:

	/* Constructors and destructor. */
	
	/**
	 * Constructor. Note that if the new item is in the database
	 * (i.e., it has an internal database identifier), it must also belong 
	 * to a session.
	 *
	 * @param aSession  session that the new item belongs to;
	 *                  or 0, if the new item does not belong to any session
	 * @param aId		internal database identifier of the new item,
	 *                  if the item is in the database (note that then also 
	 *                  <code>aSession</code> must be different from 0);
	 *					or 0, if the item is not in the database
	 */
	CMdEItem(CMdESession* aSession, TItemId aId);
	
	/**
	 * Second-phase constructor.
	 */
	void ItemBaseConstruct();
	
	/* Utility methods. */

	/**
	 * Panics if the item does not belong to any session.
	 */
	void CheckSession() const;


private:

	/** Session. */
	CMdESession* iSession;

	/** Internal database identifier. */
	TItemId iId;
	
	friend class CMdESession;
	};

#endif  // __MDEITEM_H__
