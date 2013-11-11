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

#include "mdetextpropertycondition.h"
#include "mdcserializationbuffer.h"
#include "mdepropertydef.h"
#include "mdcquery.h"

CMdETextPropertyCondition* 
CMdETextPropertyCondition::NewL(const CMdEPropertyDef& aPropertyDef,
                                TTextPropertyConditionCompareMethod aCompareMethod,
                                const TDesC& aText)
    {
    CMdETextPropertyCondition* self = NewLC(aPropertyDef, aCompareMethod, aText);
    CleanupStack::Pop(self);
    return self;
    }


CMdETextPropertyCondition* 
CMdETextPropertyCondition::NewLC(const CMdEPropertyDef& aPropertyDef,
                                 TTextPropertyConditionCompareMethod aCompareMethod,
                                 const TDesC& aText)
    {
    CMdETextPropertyCondition* self 
        = new(ELeave) CMdETextPropertyCondition(aPropertyDef, aCompareMethod);
    CleanupStack::PushL(self);
    self->ConstructL(aText);
    return self;
    }


CMdETextPropertyCondition::CMdETextPropertyCondition(const CMdEPropertyDef& aPropertyDef, 
                                                     TTextPropertyConditionCompareMethod aCompareMethod)
        : CMdEPropertyCondition(EConditionTypePropertyText, aPropertyDef), 
          iCompareMethod(aCompareMethod)
    {
    }


void CMdETextPropertyCondition::ConstructL(const TDesC& aText)
    {
    // Make a copy of the string.
    iText = aText.AllocL();
    }


CMdETextPropertyCondition::~CMdETextPropertyCondition()
	{
    delete iText;
    iText = NULL;
	}

TUint32 CMdETextPropertyCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	return CMdEPropertyCondition::InternalQueryOptimizationFlags(aFlags);
	}

TUint32 CMdETextPropertyCondition::RequiredBufferSize() const
	{
	// Required size for property condition
	TUint32 bufferSize = sizeof( TMdCPropertyCondition );

	// Required size for compare method (TUint32) and text
	bufferSize += CMdCSerializationBuffer::KRequiredSizeForTUint32;
	bufferSize += CMdCSerializationBuffer::RequiredSize( *iText );

	return bufferSize;
	}

void CMdETextPropertyCondition::SerializeL(CMdCSerializationBuffer& aBuffer,
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCPropertyCondition property;

	property.iConditionType = iType;
	property.iNegated = iNegated;
	property.iObjectDefId = iPropertyDef.ObjectDefId();
	property.iPropertyDefId = iPropertyDef.Id();
	property.iCondition = aBuffer.Position() + sizeof( TMdCPropertyCondition );

	property.SerializeL( aBuffer );

	aBuffer.InsertL( (TUint32)iCompareMethod );
	aBuffer.InsertL( *iText );

	aFreespaceOffset = aBuffer.Position();
	}

const TDesC& CMdETextPropertyCondition::Text() const
	{
	return *iText;
	}

TTextPropertyConditionCompareMethod CMdETextPropertyCondition::CompareMethod() const
	{
	return iCompareMethod;
	}
