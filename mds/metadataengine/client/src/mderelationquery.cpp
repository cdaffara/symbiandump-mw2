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


#include "mderelationquery.h"

#include "mdepanic.h"
#include "mderelation.h"


/* Constuctors */

CMdERelationQuery::CMdERelationQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef)
        : CMdEQuery(EQueryTypeRelation, aSession, aNamespaceDef)
    {
    }


void CMdERelationQuery::RelationQueryConstructL()
    {
    QueryConstructL();
    }


/* Destructor */

CMdERelationQuery::~CMdERelationQuery()
	{
	}


/* Methods */

EXPORT_C CMdERelation& CMdERelationQuery::Result(TInt aIndex) const
    {
    return static_cast<CMdERelation&>( ResultItem(aIndex) );
    }
	
void CMdERelationQuery::DoCancel() 
    {
    SetState( EStateError );
    }

