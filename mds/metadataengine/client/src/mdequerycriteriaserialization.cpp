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
* Description:  Serialize query criteria
*
*/


#include "mdequerycriteriaserialization.h"
#include "mdelogiccondition.h"
#include "mdepropertydef.h"
#include "mdenamespacedef.h"
#include "mdeobjectdef.h"
#include "mdcquery.h"
#include "mdcserializationbuffer.h"


CMdEQueryCriteriaSerialization* CMdEQueryCriteriaSerialization::NewL(
	TQueryResultMode aResultMode, TQueryType aQueryType, 
	CMdENamespaceDef& aNamespaceDef, 
	CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
	TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags,
	CMdELogicCondition& aRootCondition, RArray<TMdEOrderRule>& aOrderRule, 
	RPointerArray<CMdEPropertyDef>* aPropertyFilter)
	{
    CMdEQueryCriteriaSerialization* serialization = 
    	CMdEQueryCriteriaSerialization::NewLC( aResultMode, aQueryType,
    										   aNamespaceDef, aObjectDef, aObjectDefs, 
    										   aLimit, aOffset, aQueryOptimizationFlags,
    										   aRootCondition, aOrderRule,
    										   aPropertyFilter );
    CleanupStack::Pop( serialization );
    return serialization;
    }

CMdEQueryCriteriaSerialization* CMdEQueryCriteriaSerialization::NewLC(
	TQueryResultMode aResultMode, TQueryType aQueryType, 
	CMdENamespaceDef& aNamespaceDef, 
	CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
	TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags, 
	CMdELogicCondition& aRootCondition, RArray<TMdEOrderRule>& aOrderRule, 
	RPointerArray<CMdEPropertyDef>* aPropertyFilter)
	{
    CMdEQueryCriteriaSerialization* serialization = 
    	new( ELeave ) CMdEQueryCriteriaSerialization( ETrue ); // Owns serialized buffer
    CleanupStack::PushL( serialization );
    serialization->ConstructL( aResultMode, aQueryType,
    						   aNamespaceDef, aObjectDef, aObjectDefs, 
    						   aLimit, aOffset, aQueryOptimizationFlags,
    						   aRootCondition, aOrderRule,
    						   aPropertyFilter );
    return serialization;
	}

CMdEQueryCriteriaSerialization* CMdEQueryCriteriaSerialization::NewL(
	CMdCSerializationBuffer& aSerializationBuffer)
	{
    CMdEQueryCriteriaSerialization* serialization = 
    	CMdEQueryCriteriaSerialization::NewLC( aSerializationBuffer );
    CleanupStack::Pop( serialization );
    return serialization;
	}

CMdEQueryCriteriaSerialization* CMdEQueryCriteriaSerialization::NewLC(
	CMdCSerializationBuffer& aSerializationBuffer)
	{
    CMdEQueryCriteriaSerialization* serialization = 
    	new( ELeave ) CMdEQueryCriteriaSerialization( EFalse ); // Doesn't own serialized buffer
    CleanupStack::PushL( serialization );
    serialization->iSerializedBuffer = &aSerializationBuffer;
    return serialization;
	}

CMdEQueryCriteriaSerialization::~CMdEQueryCriteriaSerialization()
	{
	if( iOwnSerializedBuffer )
		{
		delete iSerializedBuffer;
		iSerializedBuffer = NULL;
		}
	}
	
TPtr8 CMdEQueryCriteriaSerialization::Buffer() const
	{
	return iSerializedBuffer->Buffer();
	}

const TPtr8* CMdEQueryCriteriaSerialization::BufferPtr() const
	{
	return iSerializedBuffer->BufferPtr();
	}

CMdCSerializationBuffer& CMdEQueryCriteriaSerialization::SerializationBuffer()
	{
	return *iSerializedBuffer;
	}

CMdEQueryCriteriaSerialization::CMdEQueryCriteriaSerialization(TBool aOwnSerializedBuffer)
	: iSerializedBuffer( NULL ), iOwnSerializedBuffer( aOwnSerializedBuffer )
	{
	
	}

void CMdEQueryCriteriaSerialization::ConstructL(
	TQueryResultMode aResultMode, TQueryType aQueryType, 
	CMdENamespaceDef& aNamespaceDef, 
	CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
	TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags, 
	CMdELogicCondition& aRootCondition, RArray<TMdEOrderRule>& aOrderRules, 
	RPointerArray<CMdEPropertyDef>* aPropertyFilter)
	{
	// If aResultMode is EModeDistinctValues aQueryType must be CMdEQuery::ETypeObject
	if( EQueryResultModeDistinctValues == aResultMode )
		{
		if( EQueryTypeObject != aQueryType )
			{
			User::Leave( KErrNotSupported );
			}
		else
			{
			// Order rules might also contain one order rule and it must be 
			// same as defined in propery condition.
			if( aOrderRules.Count() > 1 )
				{
				User::Leave( KErrCompletion );
				}

			// One property filter must exist and it must be same as property condition
			if( aPropertyFilter )
				{
				if( aPropertyFilter->Count() != 1 )
					{
					User::Leave( KErrCompletion );
					}
				}
			else
				{
				User::Leave( KErrCompletion);
				}
			}
		}
	
	TUint32 criteriaBufferSize = sizeof( TMdCSearchCriteria );
	
	// Required size for object defs IDs
	TUint32 objectDefIdCount = 0;
	if( aObjectDefs )
		{
		objectDefIdCount = aObjectDefs->Count();

		criteriaBufferSize += objectDefIdCount * 
				CMdCSerializationBuffer::KRequiredSizeForTDefId;
		}

	// Required size for conditions
	criteriaBufferSize += aRootCondition.RequiredBufferSize();

	// Required size for order rule offsets
	const TInt orderRuleCount = aOrderRules.Count();
	criteriaBufferSize += orderRuleCount * 
			CMdCSerializationBuffer::KRequiredSizeForTUint32;

	// Required size for order rules
	for( TInt i = orderRuleCount - 1; i >=0; i-- )
		{
		criteriaBufferSize += aOrderRules[i].RequiredBufferSize();
		}

	// Required size for property filters
	TUint32 propertyFilterCount = 0;
	if( aPropertyFilter )
		{
		propertyFilterCount = aPropertyFilter->Count();
		
		criteriaBufferSize += propertyFilterCount * 
			CMdCSerializationBuffer::KRequiredSizeForTDefId;
		}

	// Reserve buffer
	iSerializedBuffer = CMdCSerializationBuffer::NewL( criteriaBufferSize );

	// Insert data to search criteria

	TMdCSearchCriteria criteria;
	criteria.iQueryResultType = aResultMode;
	criteria.iQueryType = aQueryType;
	criteria.iNamespaceDefId = aNamespaceDef.Id();

	if( aObjectDef )
		{
		criteria.iObjectDefId = aObjectDef->Id();
		}
	else
		{
		criteria.iObjectDefId = KNoDefId;
		}

	criteria.iObjectDefIds.iPtr.iCount = objectDefIdCount;
	// init offset to "zero", will be updated later if needed
	criteria.iObjectDefIds.iPtr.iOffset = KNoOffset;

	criteria.iLimit = aLimit;
	criteria.iOffset = aOffset;
	criteria.iOptimizationFlags = aQueryOptimizationFlags;

	// init offset to "zero", will be updated later
	criteria.iRootCondition = KNoOffset;

	criteria.iOrderRules.iPtr.iCount = orderRuleCount;
	// init offset to "zero", will be updated later if needed
	criteria.iOrderRules.iPtr.iOffset = KNoOffset;

	criteria.iPropertyFilters.iPtr.iCount = propertyFilterCount;
	// init offset to "zero", will be updated later if needed
	criteria.iPropertyFilters.iPtr.iOffset = KNoOffset;

	// Insert data to serialized buffer

	// Position after search criteria
	const TUint32 afterCriteriaOffset = sizeof( TMdCSearchCriteria );

	// Move to position after search criteria
	iSerializedBuffer->PositionL( afterCriteriaOffset );

	if( aObjectDefs )
		{
		// Update object defs offset to search criteria
		criteria.iObjectDefIds.iPtr.iOffset = afterCriteriaOffset;

		// Update object defs count
		for( TUint32 i = 0; i < objectDefIdCount; i++ )
			{
			TDefId defId = (*aObjectDefs)[i]->Id();
			iSerializedBuffer->InsertL( defId );
			}
		}

	// Update root condition offset to position after search criteria 
	// (and possible object defs IDs)
	criteria.iRootCondition = iSerializedBuffer->Position();

	TUint32 offset = KNoOffset;
	// Insert root condition to serialized buffer
	aRootCondition.SerializeL( *iSerializedBuffer, offset );

	// move position after conditions
	iSerializedBuffer->PositionL( offset );
	
	if( orderRuleCount > 0 )
		{
		// Update order rule offset to position after search criteria 
		// (and possible object defs IDs) and root condition
		criteria.iOrderRules.iPtr.iOffset = iSerializedBuffer->Position();

		for( TUint32 i = 0; i < orderRuleCount; i++)
			{
			// Insert current order rule
			aOrderRules[i].SerializeL( *iSerializedBuffer, offset );
			}
		}

	// move position after order rules
	iSerializedBuffer->PositionL( offset );
	
	if( aPropertyFilter )
		{
		// Update order property filter offset to position after search 
		// criteria (and possible object defs IDs), 
		// root condition and order rules
		criteria.iPropertyFilters.iPtr.iOffset = iSerializedBuffer->Position();

		for( TUint32 i = 0; i < propertyFilterCount; i++ )
			{
			CMdEPropertyDef* propertyDef = (*aPropertyFilter)[i];
			iSerializedBuffer->InsertL( propertyDef->Id() );
			}
		}

	// move buffer to begin and serialize search criteria
	iSerializedBuffer->PositionL( KNoOffset );
	criteria.SerializeL( *iSerializedBuffer );
	}
