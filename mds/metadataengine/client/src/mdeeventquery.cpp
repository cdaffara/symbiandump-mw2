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

#include "mdeeventquery.h"
#include "mdeevent.h"


CMdEEventQuery::CMdEEventQuery(CMdESession& aSession, CMdENamespaceDef& aNamespaceDef)
        : CMdEQuery(EQueryTypeEvent, aSession, aNamespaceDef)
    {
    }


void CMdEEventQuery::EventQueryConstructL()
    {
    QueryConstructL();
    }


CMdEEventQuery::~CMdEEventQuery()
	{
	}


EXPORT_C CMdEEvent& CMdEEventQuery::Result(TInt aIndex) const
    {
    return static_cast<CMdEEvent&>( ResultItem(aIndex) );
    }
	
void CMdEEventQuery::DoCancel() 
    {
    SetState( EStateError );
    }

