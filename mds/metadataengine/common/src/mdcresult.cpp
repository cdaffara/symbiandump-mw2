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
* Description:  common item result definitions for MdE*
*/

#include "mdcresult.h"

/////////////////
//  TMdCItems  //
/////////////////

EXPORT_C TMdCItems::TMdCItems() : TMdCSerializationType( EMdCItems )
	{
	}

EXPORT_C void TMdCItems::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCItems::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCItems& TMdCItems::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCItems* items = (TMdCItems*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (items->iType != EMdCItems)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCItems!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCItems) % 2 ? sizeof(TMdCItems)+1 : sizeof(TMdCItems);
	aBuffer.PositionL( bufferPosition + size );
	return *items;
	}


///////////////////
//  TMdCItemIds  //
///////////////////

EXPORT_C TMdCItemIds::TMdCItemIds() : TMdCSerializationType( EMdCItemIds )
	{
	}

EXPORT_C void TMdCItemIds::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCItemIds::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCItemIds& TMdCItemIds::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCItemIds* itemIds = (TMdCItemIds*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (itemIds->iType != EMdCItemIds)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCItemIds!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCItemIds) % 2 ? sizeof(TMdCItemIds)+1 : sizeof(TMdCItemIds);
	aBuffer.PositionL( bufferPosition + size );
	return *itemIds;
	}


//////////////////////
//  TMdCItemCounts  //
//////////////////////

EXPORT_C TMdCItemCounts::TMdCItemCounts() : TMdCSerializationType( EMdCItemCount )
	{
	}

EXPORT_C void TMdCItemCounts::SerializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::SerializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C void TMdCItemCounts::DeserializeL(CMdCSerializationBuffer& aBuffer)
	{
	TMdCSerializationType::DeserializeL( aBuffer, sizeof(*this) );
	}

EXPORT_C const TMdCItemCounts& TMdCItemCounts::GetFromBufferL( CMdCSerializationBuffer& aBuffer )
	{
	const TInt bufferPosition = aBuffer.Position();
	TMdCItemCounts* itemCounts = (TMdCItemCounts*)(aBuffer.Buffer().Ptr() + bufferPosition);
	if (itemCounts->iType != EMdCItemCount)
		{
		__ASSERT_DEBUG( EFalse, User::Panic( _L("No TMdCItemCounts!"), KErrCorrupt ) );
	
		User::Leave( KErrCorrupt );
		}
	const TInt size = sizeof(TMdCItemCounts) % 2 ? sizeof(TMdCItemCounts)+1 : sizeof(TMdCItemCounts);
	aBuffer.PositionL( bufferPosition + size );
	return *itemCounts;
	}
