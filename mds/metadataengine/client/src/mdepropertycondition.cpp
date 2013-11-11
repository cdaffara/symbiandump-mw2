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
* Description:  Represent property condition
*
*/

#include "mdepropertycondition.h"
#include "mdcserializationbuffer.h"
#include "mdepropertydef.h"
#include "mdeobjectdef.h"
#include "mdscommoninternal.h"
#include "mdcquery.h"

CMdEPropertyCondition* CMdEPropertyCondition::NewL( const CMdEPropertyDef& aPropertyDef )
    {
    CMdEPropertyCondition* self = CMdEPropertyCondition::NewLC( aPropertyDef );
    CleanupStack::Pop( self );
    return self;
    }

CMdEPropertyCondition* CMdEPropertyCondition::NewLC( const CMdEPropertyDef& aPropertyDef )
    {
    CMdEPropertyCondition* self = new ( ELeave ) CMdEPropertyCondition( aPropertyDef );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CMdEPropertyCondition::CMdEPropertyCondition(const CMdEPropertyDef& aPropertyDef)
        : CMdECondition(EConditionTypeProperty), iPropertyDef(aPropertyDef)
    {
    }

CMdEPropertyCondition::CMdEPropertyCondition(TConditionType aConditionType, 
                                             const CMdEPropertyDef& aPropertyDef)
        : CMdECondition(aConditionType), iPropertyDef(aPropertyDef)
    {
    }

void CMdEPropertyCondition::ConstructL()
    {
    // Base class construction.
    ConditionConstruct();
    }

CMdEPropertyCondition::~CMdEPropertyCondition()
	{
	}

EXPORT_C const CMdEPropertyDef& CMdEPropertyCondition::PropertyDef() const
    {
    return iPropertyDef;
    }

TUint32 CMdEPropertyCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	aFlags |= EContainsObjectCondition;
	return EContainsObjectCondition;
	}

TUint32 CMdEPropertyCondition::RequiredBufferSize() const
	{
	// Required size for property condition
	return sizeof( TMdCPropertyCondition );
	}

void CMdEPropertyCondition::SerializeL(CMdCSerializationBuffer& aBuffer,
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCPropertyCondition condition;

	condition.iConditionType = iType;
	condition.iNegated = iNegated;
	condition.iObjectDefId = iPropertyDef.ObjectDefId();
	condition.iPropertyDefId = iPropertyDef.Id();
	condition.iCondition = KNoOffset;

	condition.SerializeL( aBuffer );

	aFreespaceOffset = aBuffer.Position();
	}
