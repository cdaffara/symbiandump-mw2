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
* Description:  Property conditions for boolean property values
*
*/

#include "mdeboolpropertycondition.h"
#include "mdcserializationbuffer.h"
#include "mdepropertydef.h"
#include "mdcquery.h"


	/* Constructors and destructor. */
CMdEBoolPropertyCondition* CMdEBoolPropertyCondition::NewL( 
		const CMdEPropertyDef& aPropertyDef, TBool aValue )
	{
	CMdEBoolPropertyCondition* self = CMdEBoolPropertyCondition::NewLC( aPropertyDef, aValue );
	CleanupStack::Pop( self );
	return self;
	}


CMdEBoolPropertyCondition* CMdEBoolPropertyCondition::NewLC( 
		const CMdEPropertyDef& aPropertyDef, TBool aValue )
	{
	CMdEBoolPropertyCondition* self = new ( ELeave ) CMdEBoolPropertyCondition( aPropertyDef, aValue );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CMdEBoolPropertyCondition::CMdEBoolPropertyCondition(const CMdEPropertyDef& aPropertyDef, TBool aValue)
        : CMdEPropertyCondition(EConditionTypePropertyBool, aPropertyDef), iValue(aValue)
    {
    }


void CMdEBoolPropertyCondition::ConstructL()
    {
    // Base class construction.
    CMdEPropertyCondition::ConstructL();
    }
    

CMdEBoolPropertyCondition::~CMdEBoolPropertyCondition()
    {
    }


EXPORT_C TBool CMdEBoolPropertyCondition::Value() const
    {
    return iValue;
    }

TUint32 CMdEBoolPropertyCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	return CMdEPropertyCondition::InternalQueryOptimizationFlags(aFlags);
	}

TUint32 CMdEBoolPropertyCondition::RequiredBufferSize() const
	{
	// Required size for property condition
	TUint32 bufferSize = sizeof( TMdCPropertyCondition );

	//Required size for bool value (coverted to TUint8)
	bufferSize += CMdCSerializationBuffer::KRequiredSizeForTBool;

	return bufferSize;
	}

void CMdEBoolPropertyCondition::SerializeL(CMdCSerializationBuffer& aBuffer,
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCPropertyCondition property;

	property.iConditionType = iType;
	property.iNegated = iNegated;
	property.iObjectDefId = iPropertyDef.ObjectDefId();
	property.iPropertyDefId = iPropertyDef.Id();
	property.iCondition = aBuffer.Position() + sizeof( TMdCPropertyCondition );

	property.SerializeL( aBuffer );

	aBuffer.InsertL( iValue );

	aFreespaceOffset = aBuffer.Position();
	}
