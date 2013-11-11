/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  common query and condition definitions for MdE*
*/

#include "mdcquery.h"


/////////////////////
//  TMdCCondition  //
/////////////////////

EXPORT_C const TMdCCondition& TMdCCondition::GetFromBufferL( 
		CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCCondition* condition = (TMdCCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	return *condition;
	}


//////////////////////////
//  TMdCLogicCondition  //
//////////////////////////

EXPORT_C TMdCLogicCondition::TMdCLogicCondition() : TMdCCondition( EMdCLogicCondition )
	{
	}

EXPORT_C void TMdCLogicCondition::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCLogicCondition::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCLogicCondition& TMdCLogicCondition::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCLogicCondition* logicCondition = (TMdCLogicCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (logicCondition->iType != EMdCLogicCondition)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCLogicCondition!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCLogicCondition) % 2 ? sizeof(TMdCLogicCondition)+1 : sizeof(TMdCLogicCondition);
	aBuffer.PositionL( bufferPosition + size );
	return *logicCondition;
	}


///////////////////////////
//  TMdCObjectCondition  //
///////////////////////////

EXPORT_C TMdCObjectCondition::TMdCObjectCondition() : TMdCCondition( EMdCObjectCondition )
	{
	}

EXPORT_C void TMdCObjectCondition::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCObjectCondition::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCObjectCondition& TMdCObjectCondition::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCObjectCondition* objectCondition = (TMdCObjectCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (objectCondition->iType != EMdCObjectCondition)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCObjectCondition!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCObjectCondition) % 2 ? sizeof(TMdCObjectCondition)+1 : sizeof(TMdCObjectCondition);
	aBuffer.PositionL( bufferPosition + size );
	return *objectCondition;
	}


//////////////////////////
//  TMdCEventCondition  //
//////////////////////////

EXPORT_C TMdCEventCondition::TMdCEventCondition() : TMdCCondition( EMdCEventCondition )
	{
	}

EXPORT_C void TMdCEventCondition::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCEventCondition::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCEventCondition& TMdCEventCondition::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCEventCondition* eventCondition = (TMdCEventCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (eventCondition->iType != EMdCEventCondition)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCEventCondition!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCEventCondition) % 2 ? sizeof(TMdCEventCondition)+1 : sizeof(TMdCEventCondition);
	aBuffer.PositionL( bufferPosition + size );
	return *eventCondition;
	}


/////////////////////////////
//  TMdCPropertyCondition  //
/////////////////////////////

EXPORT_C TMdCPropertyCondition::TMdCPropertyCondition() : TMdCCondition( EMdCPropertyCondition )
	{
	}

EXPORT_C void TMdCPropertyCondition::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCPropertyCondition::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCPropertyCondition& TMdCPropertyCondition::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCPropertyCondition* propertyCondition = (TMdCPropertyCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (propertyCondition->iType != EMdCPropertyCondition)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCPropertyCondition!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCPropertyCondition) % 2 ? sizeof(TMdCPropertyCondition)+1 : sizeof(TMdCPropertyCondition);
	aBuffer.PositionL( bufferPosition + size );
	return *propertyCondition;
	}


/////////////////////////////
//  TMdCRelationCondition  //
/////////////////////////////

EXPORT_C TMdCRelationCondition::TMdCRelationCondition() : TMdCCondition( EMdCRelationCondition )
	{
	}

EXPORT_C void TMdCRelationCondition::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCRelationCondition::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCRelationCondition& TMdCRelationCondition::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCRelationCondition* relationCondition = (TMdCRelationCondition*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (relationCondition->iType != EMdCRelationCondition)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCRelationCondition!"), KErrCorrupt ) );

		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCRelationCondition) % 2 ? sizeof(TMdCRelationCondition)+1 : sizeof(TMdCRelationCondition);
	aBuffer.PositionL( bufferPosition + size );
	return *relationCondition;
	}


//////////////////////////
//  TMdCSearchCriteria  //
//////////////////////////

EXPORT_C TMdCSearchCriteria::TMdCSearchCriteria() : TMdCSerializationType( EMdCSearchCriteria )
	{
	}

EXPORT_C void TMdCSearchCriteria::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCSearchCriteria::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCSearchCriteria& TMdCSearchCriteria::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCSearchCriteria* searchCriteria = (TMdCSearchCriteria*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (searchCriteria->iType != EMdCSearchCriteria)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCSearchCriteria!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCSearchCriteria) % 2 ? sizeof(TMdCSearchCriteria)+1 : sizeof(TMdCSearchCriteria);
	aBuffer.PositionL( bufferPosition + size );
	return *searchCriteria;
	}

/////////////////////
//  TMdCOrderRule  //
/////////////////////

EXPORT_C TMdCOrderRule::TMdCOrderRule() : TMdCSerializationType( EMdCOrderRule )
	{
	}

EXPORT_C void TMdCOrderRule::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCOrderRule::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCOrderRule& TMdCOrderRule::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCOrderRule* orderRule = (TMdCOrderRule*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (orderRule->iType != EMdCOrderRule)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCOrderRule!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCOrderRule) % 2 ? sizeof(TMdCOrderRule)+1 : sizeof(TMdCOrderRule);
	aBuffer.PositionL( bufferPosition + size );
	return *orderRule;
	}

