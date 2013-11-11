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
* Description:  common definition for MdE*
*/

#include "mdcdef.h"

/////////////////////
//  TMdCObjectDef  //
/////////////////////

EXPORT_C TMdCObjectDef::TMdCObjectDef() : TMdCDef( EMdCObject )
	{
	}

EXPORT_C void TMdCObjectDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCObjectDef::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}
EXPORT_C const TMdCObjectDef& TMdCObjectDef::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCObjectDef* objectDef = (TMdCObjectDef*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (objectDef->iType != EMdCObject)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCObjectDef!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCObjectDef) % 2 ? sizeof(TMdCObjectDef)+1 : sizeof(TMdCObjectDef);
	aBuffer.PositionL( bufferPosition + size );
	return *objectDef;
	}


///////////////////////
//  TMdCRelationDef  //
///////////////////////

EXPORT_C TMdCRelationDef::TMdCRelationDef() : TMdCDef( EMdCRelationDef )
	{
	}

EXPORT_C void TMdCRelationDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCRelationDef::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}
EXPORT_C const TMdCRelationDef& TMdCRelationDef::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCRelationDef* relationDef = (TMdCRelationDef*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (relationDef->iType != EMdCRelationDef)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCRelationDef!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCRelationDef) % 2 ? sizeof(TMdCRelationDef)+1 : sizeof(TMdCRelationDef);
	aBuffer.PositionL( bufferPosition + size );
	return *relationDef;
	}


////////////////////
//  TMdCEventDef  //
////////////////////

EXPORT_C TMdCEventDef::TMdCEventDef() : TMdCDef( EMdCEventDef )
	{
	}

EXPORT_C void TMdCEventDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCEventDef::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}
EXPORT_C const TMdCEventDef& TMdCEventDef::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCEventDef* eventDef = (TMdCEventDef*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (eventDef->iType != EMdCEventDef)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCEventDef!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCEventDef) % 2 ? sizeof(TMdCEventDef)+1 : sizeof(TMdCEventDef);
	aBuffer.PositionL( bufferPosition + size );
	return *eventDef;
	}


///////////////////////
//  TMdCPropertyDef  //
///////////////////////

EXPORT_C TMdCPropertyDef::TMdCPropertyDef() : TMdCDef( EMdCPropertyDef )
	{
	}

EXPORT_C void TMdCPropertyDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCPropertyDef::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}
EXPORT_C const TMdCPropertyDef& TMdCPropertyDef::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCPropertyDef* propertyDef = (TMdCPropertyDef*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (propertyDef->iType != EMdCPropertyDef)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCPropertyDef!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCPropertyDef) % 2 ? sizeof(TMdCPropertyDef)+1 : sizeof(TMdCPropertyDef);
	aBuffer.PositionL( bufferPosition + size );
	return *propertyDef;
	}


////////////////////////
//  TMdCNamespaceDef  //
////////////////////////

EXPORT_C TMdCNamespaceDef::TMdCNamespaceDef() : TMdCDef( EMdCNamespaceDef )
	{
	}

EXPORT_C void TMdCNamespaceDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCNamespaceDef::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCNamespaceDef& TMdCNamespaceDef::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCNamespaceDef* namespaceDef = (TMdCNamespaceDef*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (namespaceDef->iType != EMdCNamespaceDef)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCNamespaceDef!"), KErrCorrupt ) );

		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCNamespaceDef) % 2 ? sizeof(TMdCNamespaceDef)+1 : sizeof(TMdCNamespaceDef);
	aBuffer.PositionL( bufferPosition + size );
	return *namespaceDef;
	}


//////////////////
//  TMdCSchema  //
//////////////////

EXPORT_C TMdCSchema::TMdCSchema() : TMdCSerializationType( EMdCSchema )
	{
	}

EXPORT_C void TMdCSchema::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCSchema::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}
EXPORT_C const TMdCSchema& TMdCSchema::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCSchema* schema = (TMdCSchema*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (schema->iType != EMdCSchema)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCSchema!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCSchema) % 2 ? sizeof(TMdCSchema)+1 : sizeof(TMdCSchema);
	aBuffer.PositionL( bufferPosition + size );
	return *schema;
	}

