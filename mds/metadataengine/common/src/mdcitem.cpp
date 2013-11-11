/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  common item object for MdE*
*/

#include "mdcitem.h"

///////////////////
//  TMdCObject
///////////////////

EXPORT_C TMdCObject::TMdCObject() : TMdCItem( EMdCObject ), iFlags( 0 )
	{
	}

EXPORT_C void TMdCObject::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCObject::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCObject& TMdCObject::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCObject* object = (TMdCObject*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (object->iType != EMdCObject)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCObject!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCObject) % 2 ? sizeof(TMdCObject)+1 : sizeof(TMdCObject);
	aBuffer.PositionL( bufferPosition + size );
	return *object;
	}

///////////////////
//  TMdCRelation
///////////////////

EXPORT_C TMdCRelation::TMdCRelation() : TMdCItem( EMdCRelation ),
        iLeftObjectId( 0 ), iRightObjectId( 0 )
	{
	}

EXPORT_C void TMdCRelation::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCRelation::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCRelation& TMdCRelation::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCRelation* relation = (TMdCRelation*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (relation->iType != EMdCRelation)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCRelation!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCRelation) % 2 ? sizeof(TMdCRelation)+1 : sizeof(TMdCRelation);
	aBuffer.PositionL( bufferPosition + size );
	return *relation;
	}


///////////////////
//  TMdCEvent
///////////////////

EXPORT_C TMdCEvent::TMdCEvent() : TMdCItem( EMdCEvent )
	{
	}

EXPORT_C void TMdCEvent::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCEvent::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCEvent& TMdCEvent::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCEvent* event = (TMdCEvent*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (event->iType != EMdCEvent)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCEvent!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCEvent) % 2 ? sizeof(TMdCEvent)+1 : sizeof(TMdCEvent);
	aBuffer.PositionL( bufferPosition + size );
	return *event;
	}


///////////////////
//  TMdCProperty
///////////////////

EXPORT_C TMdCProperty::TMdCProperty() : TMdCSerializationType( EMdCProperty )
	{
	}

EXPORT_C void TMdCProperty::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCProperty::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCProperty& TMdCProperty::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCProperty* property = (TMdCProperty*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (property->iType != EMdCProperty)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCProperty!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCProperty) % 2 ? sizeof(TMdCProperty)+1 : sizeof(TMdCProperty);
	aBuffer.PositionL( bufferPosition + size );
	return *property;
	}
