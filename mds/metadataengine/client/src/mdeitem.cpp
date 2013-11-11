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


#include "mdeitem.h"
#include "mdepanic.h"


CMdEItem::CMdEItem(CMdESession* aSession, TItemId aId)
		: iSession(aSession), iId(aId)
	{

	}


void CMdEItem::ItemBaseConstruct()
	{
	}


CMdEItem::~CMdEItem()
	{
	}


EXPORT_C TBool CMdEItem::BelongsToSession() const
	{
	return iSession ? ETrue : EFalse;
	}


EXPORT_C CMdESession& CMdEItem::Session() const
	{
	CheckSession();

	return *iSession;
	}


void CMdEItem::CheckSession() const
	{
	__ASSERT_DEBUG(BelongsToSession(),
					TMdEPanic::Panic(TMdEPanic::EItemDoesNotBelongToSession));
	}


void CMdEItem::SetSession(CMdESession& aSession)
	{
	iSession = &aSession;
	}


EXPORT_C TBool CMdEItem::InDatabase() const
	{
	if (iId != KNoId)
		{
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}


EXPORT_C TItemId CMdEItem::Id() const
	{
	return iId;
	}


void CMdEItem::SetId(TItemId aId)
	{
	iId = aId;
	}

