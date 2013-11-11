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
* Description:  Class to deserialize client query cliteria
*
*/

#include "mdsquerycriteriadeserialization.h"


CMdSQueryCriteriaDeserialization* CMdSQueryCriteriaDeserialization::NewL(CMdCSerializationBuffer& aSerializationBuffer)
	{
    CMdSQueryCriteriaDeserialization* serialization = 
    	CMdSQueryCriteriaDeserialization::NewLC( aSerializationBuffer );
    CleanupStack::Pop( serialization );
    return serialization;
	}

CMdSQueryCriteriaDeserialization* CMdSQueryCriteriaDeserialization::NewLC(CMdCSerializationBuffer& aSerializationBuffer)
	{
    CMdSQueryCriteriaDeserialization* serialization = 
    	new( ELeave ) CMdSQueryCriteriaDeserialization( aSerializationBuffer );
    CleanupStack::PushL( serialization );
    return serialization;
	}

CMdSQueryCriteriaDeserialization::~CMdSQueryCriteriaDeserialization()
	{
	// Doesn't own serialized buffer
	}

TPtr8 CMdSQueryCriteriaDeserialization::Buffer() const
	{
	return iSerializedBuffer.Buffer();
	}

const TPtr8* CMdSQueryCriteriaDeserialization::BufferPtr() const
	{
	return iSerializedBuffer.BufferPtr();
	}

CMdCSerializationBuffer& CMdSQueryCriteriaDeserialization::SerializationBuffer()
	{
	return iSerializedBuffer;
	}

TQueryResultMode CMdSQueryCriteriaDeserialization::ResultModeL()
	{
	iSerializedBuffer.PositionL( KOffsetResultMode );
	
	TInt32 resultMode;
	iSerializedBuffer.ReceiveL( resultMode );
	
	return (TQueryResultMode)resultMode;
	}

TQueryType CMdSQueryCriteriaDeserialization::QueryTypeL()
	{
	iSerializedBuffer.PositionL( KOffsetQueryType );
	
	TInt32 queryType;
	iSerializedBuffer.ReceiveL( queryType );
	
	return (TQueryType)queryType;
	}

TDefId CMdSQueryCriteriaDeserialization::NamespaceDefIdL()
	{
	iSerializedBuffer.PositionL( KOffsetNamespaceDefId );
	
	TDefId namespaceDefId;
	iSerializedBuffer.ReceiveL( namespaceDefId );

	return namespaceDefId;
	}

TDefId CMdSQueryCriteriaDeserialization::ObjectDefIdL()
	{
	iSerializedBuffer.PositionL( KOffsetObjectDefId );
	
	TDefId objectDefId;
	iSerializedBuffer.ReceiveL( objectDefId );

	return objectDefId;
	}

TBool CMdSQueryCriteriaDeserialization::ObjectDefsIdsL()
	{
	iSerializedBuffer.PositionL( KOffsetObjectDefsIds );

	TUint32 offset;
	iSerializedBuffer.ReceiveL( offset );

	if( KNoOffset == offset )
		{
		return EFalse;
		}
	else
		{
		iSerializedBuffer.PositionL( offset );
		return ETrue;
		}
	}

TUint32 CMdSQueryCriteriaDeserialization::LimitL()
	{
	iSerializedBuffer.PositionL( KOffsetLimit );
	
	TUint32 limit;
	iSerializedBuffer.ReceiveL( limit );

	return limit;
	}

TUint32 CMdSQueryCriteriaDeserialization::OffsetL()
	{
	iSerializedBuffer.PositionL( KOffsetOffset );

	TUint32 offset;
	iSerializedBuffer.ReceiveL( offset );

	return offset;
	}

TUint32 CMdSQueryCriteriaDeserialization::OptimizationFlagsL()
	{
	iSerializedBuffer.PositionL( KOffsetOptimizationFlags );

	TUint32 optimizationFlags;
	iSerializedBuffer.ReceiveL( optimizationFlags );

	return optimizationFlags;
	}

void CMdSQueryCriteriaDeserialization::RootConditionL()
	{
	iSerializedBuffer.PositionL( KOffsetRootCondition );
	
	TUint32 offset;
	iSerializedBuffer.ReceiveL( offset );
	
	iSerializedBuffer.PositionL( offset );
	}

void CMdSQueryCriteriaDeserialization::OrderRulesL()
	{
	iSerializedBuffer.PositionL( KOffsetOrderRules );

	TUint32 offset;
	iSerializedBuffer.ReceiveL( offset );
	
	iSerializedBuffer.PositionL( offset );
	}

void CMdSQueryCriteriaDeserialization::PropertyFiltersL()
	{
	iSerializedBuffer.PositionL( KOffsetPropertyFilters );

	TUint32 offset;
	iSerializedBuffer.ReceiveL( offset );
	
	iSerializedBuffer.PositionL( offset );
	}

CMdSQueryCriteriaDeserialization::CMdSQueryCriteriaDeserialization(CMdCSerializationBuffer& aSerializationBuffer)
	: iSerializedBuffer( aSerializationBuffer )
	{}

