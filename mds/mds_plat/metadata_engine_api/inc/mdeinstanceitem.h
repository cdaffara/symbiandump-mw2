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


#ifndef __MDEINSTANCEITEM_H__
#define __MDEINSTANCEITEM_H__


#include <mdeitem.h>

enum TMdEInstanceType
	{
	EMdETypeObject = 1,
	EMdETypeRelation,
	EMdETypeEvent,
	EMdETypeProperty
	};

/**
 * Abstract base class for all instance items stored in the metadata engine database.
 */
NONSHARABLE_CLASS(CMdEInstanceItem) : public CMdEItem
	{
public:
	
	/* Destructor. */

	/**
	 * Destructor.
	 */
	virtual ~CMdEInstanceItem();


	/* Methods. */

	virtual TMdEInstanceType InstanceType() const = 0;
	
protected:

	/* Constructors. */

	/**
	 * Constructor.
	 */
	CMdEInstanceItem(CMdESession* aSession, TItemId aId);

	/**
	 * Second-phase constructor.
	 */
	void InstanceItemBaseConstruct();
	};

#endif  // __MDEINSTANCEITEM_H__
