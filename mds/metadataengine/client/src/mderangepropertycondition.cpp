/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Serialization for mderange template class
*
*/

#include "mderangepropertycondition.h"
#include "mdcserializationbuffer.h"
#include "mdepropertydef.h"
#include "mdcquery.h"

void TMdESerializeRangePropertyCondition::SerializeL( 
		CMdCSerializationBuffer& aBuffer, 
		const CMdEPropertyCondition& aPropertyCondition, TMdERangeType aType, 
		const void* aMin, const void* aMax, TInt aValueSize, TMdCOffset& aFreespaceOffset )
	{
	if ( aValueSize > sizeof(TMdCValueUnion) )
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("Incorrect type"), KErrMdEIncorrectType ) );
		User::Leave( KErrMdEIncorrectType );
		}

	TMdCPropertyCondition condition;

	condition.iConditionType = aPropertyCondition.Type();
	condition.iNegated = aPropertyCondition.Negate();
	condition.iObjectDefId = aPropertyCondition.PropertyDef().ObjectDefId();
	condition.iPropertyDefId = aPropertyCondition.PropertyDef().Id();
	condition.iCondition = aBuffer.Position() + sizeof( TMdCPropertyCondition );

	condition.SerializeL( aBuffer );
	
	aBuffer.InsertL( (TInt32)aType );

	TMdCValueUnion min;
	Mem::Copy( &min, aMin, aValueSize );
	TMdCValueUnion max;
	Mem::Copy( &max, aMax, aValueSize );

	aBuffer.InsertL( min );
	aBuffer.InsertL( max );
	
	aFreespaceOffset = aBuffer.Position();
	}

TUint32 TMdESerializeRangePropertyCondition::RequiredBufferSize( TInt  
#ifdef _DEBUG	
		aValueSize
#endif
		)
	{
#ifdef _DEBUG
	if ( aValueSize > sizeof(TMdCValueUnion) )
		{
		User::Panic( _L("Incorrect type"), KErrMdEIncorrectType );
		}
#endif

	TUint32 bufferSize = sizeof( TMdCPropertyCondition );

	// Required size for type (TInt32), iMin and iMax (fixed size)
	bufferSize += CMdCSerializationBuffer::KRequiredSizeForTInt32
			+ 2 * CMdCSerializationBuffer::KRequiredSizeForTMdCValueUnion;

	return bufferSize;
	}
