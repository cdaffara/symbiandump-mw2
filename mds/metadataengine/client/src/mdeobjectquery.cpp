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


#include "mdeobjectquery.h"
#include "mdeobject.h"


CMdEObjectQuery::CMdEObjectQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef, 
		CMdEObjectDef& aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs)
        : CMdEQuery(EQueryTypeObject, aSession, aNamespaceDef), 
          iObjectDef(aObjectDef), iObjectDefs(aObjectDefs),
          iFreetextsInResult( ETrue )
    {
    }

void CMdEObjectQuery::ObjectQueryConstructL()
    {
    QueryConstructL();
    }

CMdEObjectQuery::~CMdEObjectQuery()
	{
	// doesn't own property definitions, so only delete pointer array
	iPropertyFilters.Close();
	}

EXPORT_C CMdEObject& CMdEObjectQuery::Result(TInt aIndex) const
    {
    return static_cast<CMdEObject&>( ResultItem(aIndex) );
    }

EXPORT_C CMdEObjectDef& CMdEObjectQuery::ObjectDef() const
	{
	return iObjectDef;
	}

EXPORT_C void CMdEObjectQuery::AddPropertyFilterL(const CMdEPropertyDef* aProperty)
    {
    iPropertyFilters.AppendL( aProperty );
    }


EXPORT_C void CMdEObjectQuery::ResetPropertyFilter()
    {
    iPropertyFilters.Reset();
    }
    
RPointerArray<CMdEPropertyDef>& CMdEObjectQuery::PropertyFilter()
	{
	return iPropertyFilters;
	}

RPointerArray<CMdEObjectDef>* CMdEObjectQuery::ObjectDefs() const
	{
	return iObjectDefs;
	}

void CMdEObjectQuery::DoCancel() 
    {
    SetState( EStateError );
    }

