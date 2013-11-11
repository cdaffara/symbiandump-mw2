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
* Description:  Base class for all search condition tree nodes.
*
*/

#include "mdecondition.h"
#include "mdcquery.h"
#include "mdepanic.h"
#include "mdcserializationbuffer.h"

CMdECondition::CMdECondition(TConditionType aType)
        : iType(aType), iNegated(EFalse)
    {
	__ASSERT_DEBUG(aType > EConditionTypeFirst && aType < EConditionTypeLast,
					TMdEPanic::Panic(TMdEPanic::EInternal));
    }

void CMdECondition::ConditionConstruct()
    {
    }

CMdECondition::~CMdECondition()
	{
	}

EXPORT_C TConditionType CMdECondition::Type() const
    {
    return iType;
    }

EXPORT_C TBool CMdECondition::Negate() const
    {
    return iNegated;
    }

EXPORT_C void CMdECondition::SetNegate(TBool aNegate)
    {
    AssertNotLocked();
    iNegated = aNegate;
    }

EXPORT_C TBool CMdECondition::Locked() const
    {
    return iLocked;
    }

EXPORT_C void CMdECondition::SetLocked(TBool aLocked)
    {
    iLocked = aLocked;
    }

void CMdECondition::AssertNotLocked() const
    {
    __ASSERT_DEBUG(!iLocked, TMdEPanic::Panic(TMdEPanic::EConditionLocked));
    }
