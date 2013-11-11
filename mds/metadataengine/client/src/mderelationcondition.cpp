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

#include "mderelationcondition.h"
#include "mderelationdef.h"
#include "mdelogiccondition.h"
#include "mdscommoninternal.h"
#include "mdcserializationbuffer.h"
#include "mdcquery.h"

CMdERelationCondition* CMdERelationCondition::NewL( TItemId aRelationId,
		const RArray<TItemId>* aRelationIds, TRelationConditionSide aSide,
		const CMdERelationDef* aRelationDef, 
		const TMdEIntRange* aParameterRange )
	{
	CMdERelationCondition* self = CMdERelationCondition::NewLC(
			aRelationId, aRelationIds, aSide, aRelationDef, aParameterRange );
    CleanupStack::Pop( self );
    return self;
	}

CMdERelationCondition* CMdERelationCondition::NewLC( TItemId aRelationId,
		const RArray<TItemId>* aRelationIds, TRelationConditionSide aSide,
		const CMdERelationDef* aRelationDef, 
		const TMdEIntRange* aParameterRange )
	{
	CMdERelationCondition* self = new ( ELeave ) CMdERelationCondition( 
			aRelationId, aSide, aRelationDef );
    CleanupStack::PushL( self );
    self->ConstructL( aRelationIds, aParameterRange );
    return self;
	}

CMdERelationCondition::CMdERelationCondition( TItemId aRelationId, 
		TRelationConditionSide aSide, const CMdERelationDef* aRelationDef )
		: CMdECondition( EConditionTypeRelation ), iRelationId( aRelationId ),
		  iRelationDef( aRelationDef ), iSide( aSide )
	{
	iRelationIds = NULL;
	iParameterRange = NULL;
	iRightCondition = NULL;
	iLeftCondition = NULL;
	}

void CMdERelationCondition::ConstructL(const RArray<TItemId>* aRelationIds, 
		const TMdEIntRange* aParameterRange)
    {
    // Base class construction.
    ConditionConstruct();

    if( aRelationIds )
    	{
    	iRelationIds = new (ELeave) RArray<TItemId>();
    	
    	const TInt relationIdCount = aRelationIds->Count();
    	iRelationIds->ReserveL( relationIdCount );
    	
    	for( TInt i = 0; i < relationIdCount; i++ )
    		{
    		iRelationIds->AppendL( (*aRelationIds)[i] );
    		}
    	}

    if( aParameterRange )
        {
        // Make a copy of the parameter range.
        iParameterRange = new (ELeave) TMdEIntRange(*aParameterRange);
        }
    }

CMdERelationCondition::~CMdERelationCondition()
    {
   	delete iLeftCondition;
   	iLeftCondition = NULL;
   	delete iRightCondition;
   	iRightCondition = NULL;
   	delete iParameterRange;
   	iParameterRange = NULL;
	delete iLastModifiedDateRange;
	iLastModifiedDateRange = NULL;

	if( iRelationIds )
		{
		iRelationIds->Close();
		delete iRelationIds;
		iRelationIds = NULL;
		}
    }

EXPORT_C TItemId CMdERelationCondition::RelationId() const
    {   
    return iRelationId;
    }

EXPORT_C const RArray<TItemId>* CMdERelationCondition::RelationIds() const
	{   
	return iRelationIds;
	}

EXPORT_C const CMdERelationDef* CMdERelationCondition::RelationDef() const
    {   
    return iRelationDef;
    }

EXPORT_C CMdELogicCondition& CMdERelationCondition::LeftL()
    {
    if ( !iLeftCondition )
    	{
    	iLeftCondition = CMdELogicCondition::NewL(ELogicConditionOperatorAnd);
    	}
    
    return *iLeftCondition;
    }

EXPORT_C CMdELogicCondition& CMdERelationCondition::RightL()
    {
    if ( !iRightCondition )
    	{
    	iRightCondition = CMdELogicCondition::NewL(ELogicConditionOperatorAnd);
    	}

    return *iRightCondition;
    }

TRelationConditionSide CMdERelationCondition::Side() const
	{
	return iSide;
	}

EXPORT_C void CMdERelationCondition::SetSide(TRelationConditionSide aSide)
    {
    iSide = aSide;
    }


EXPORT_C const TMdEIntRange* CMdERelationCondition::ParameterRange() const
    {
    return iParameterRange;
    }

EXPORT_C void CMdERelationCondition::SetGuid(const TInt64& aGuidHigh, const TInt64& aGuidLow)
	{
	iGuidHigh = aGuidHigh;
	iGuidLow = aGuidLow;
	}

EXPORT_C TBool CMdERelationCondition::Guid(TInt64& aGuidHigh, TInt64& aGuidLow) const
	{
	if( iGuidHigh == 0 && iGuidLow == 0 )
		{
		return EFalse;
		}
	
	aGuidHigh = iGuidHigh;
	aGuidLow = iGuidLow;
	return ETrue;
	}

EXPORT_C void CMdERelationCondition::SetLastModifiedDateRangeL(const TMdETimeRange& aLastModifiedDateRange)
	{
	if( !iLastModifiedDateRange )
		{
		iLastModifiedDateRange = new (ELeave) TMdETimeRange( aLastModifiedDateRange );
		}
	}

EXPORT_C TBool CMdERelationCondition::LastModifiedDateRange(TMdETimeRange& aLastModifiedDateRange) const
	{
	if( iLastModifiedDateRange )
		{		
		aLastModifiedDateRange.SetType( iLastModifiedDateRange->Type() );
		aLastModifiedDateRange.SetMin( iLastModifiedDateRange->Min() );
		aLastModifiedDateRange.SetMax( iLastModifiedDateRange->Max() );
		return ETrue;
		}
	
	return EFalse;
	}

TUint32 CMdERelationCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	aFlags |= EContainsRelationCondition;
	return EContainsRelationCondition;
	}

TUint32 CMdERelationCondition::RequiredBufferSize() const
	{
	TUint32 bufferSize = sizeof( TMdCRelationCondition );

	if( iRelationIds )
		{
		// count and IDs
		bufferSize += CMdCSerializationBuffer::KRequiredSizeForTUint32 + 
			iRelationIds->Count() * CMdCSerializationBuffer::KRequiredSizeForTItemId;
		}	

	if( iGuidHigh != 0 || iGuidLow != 0 )
		{
		bufferSize += 2 * CMdCSerializationBuffer::KRequiredSizeForTInt64;
		}

	if ( iParameterRange )
		{
		bufferSize += iParameterRange->RequiredBufferSize();
		}

	if ( iLeftCondition && iLeftCondition->Count() > 0 )
		{
		bufferSize += iLeftCondition->RequiredBufferSize();
		}

	if ( iRightCondition && iRightCondition->Count() > 0  )
		{
		bufferSize += iRightCondition->RequiredBufferSize();
		}

	if( iLastModifiedDateRange )
		{
		bufferSize += iLastModifiedDateRange->RequiredBufferSize();
		}

	return bufferSize;
	}

void CMdERelationCondition::SerializeL(CMdCSerializationBuffer& aBuffer,
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCRelationCondition condition;

	condition.iConditionType = iType;
	condition.iNegated = iNegated;
	condition.iObjectSide = iSide;

	if( iRelationDef )
		{
		condition.iRelationDefId = iRelationDef->Id();
		}
	else
		{
		condition.iRelationDefId = KNoDefId;
		}

	condition.iRelationId = iRelationId;

	// updated later, if relation IDs exists
	condition.iRelationIds.iPtr.iCount = 0;
	condition.iRelationIds.iPtr.iOffset = KNoOffset;
	// updated later, if GUID exists
	condition.iGuid = KNoOffset;
	// updated later, if parameter range exists
	condition.iParameterRange = KNoOffset;
	// updated later, if left object condition exists
	condition.iLeftObjectCondition = KNoOffset;
	// updated later, if right object condition exists
	condition.iRightObjectCondition = KNoOffset;
	// updated later, if last modified date range exists
	condition.iLastModifiedDateRange = KNoOffset;

	const TUint32 conditionOffset = aBuffer.Position();

	// get position after condition
	aFreespaceOffset = conditionOffset + sizeof( TMdCRelationCondition );
	aBuffer.PositionL( aFreespaceOffset );

	if( iRelationIds )
		{
		condition.iRelationIds.iPtr.iCount = iRelationIds->Count();
		condition.iRelationIds.iPtr.iOffset = aFreespaceOffset;
		
		const TInt relationIdCount = iRelationIds->Count();
		for( TInt i = 0; i < relationIdCount; i++ )
			{
			const TItemId relationId = (*iRelationIds)[i];
			aBuffer.InsertL( relationId );
			}
		
		aFreespaceOffset = aBuffer.Position();
		}
	
	if( iGuidHigh != 0 || iGuidLow != 0 )
		{
		condition.iGuid = aFreespaceOffset;

		aBuffer.InsertL( iGuidHigh );
		aBuffer.InsertL( iGuidLow );
		
		aFreespaceOffset = aBuffer.Position();
		}

	if ( iParameterRange )
		{
		condition.iParameterRange = aFreespaceOffset;

		iParameterRange->SerializeL( aBuffer );

		aFreespaceOffset = aBuffer.Position();
		}

	if ( iLeftCondition && iLeftCondition->Count() > 0  )
		{
		condition.iLeftObjectCondition = aFreespaceOffset;

		iLeftCondition->SerializeL( aBuffer, aFreespaceOffset );

		aFreespaceOffset = aBuffer.Position();
		}

	if ( iRightCondition && iRightCondition->Count() > 0  )
		{
		condition.iRightObjectCondition = aFreespaceOffset;

		iRightCondition->SerializeL( aBuffer, aFreespaceOffset );

		aFreespaceOffset = aBuffer.Position();
		}

	if ( iLastModifiedDateRange )
		{
		condition.iLastModifiedDateRange = aFreespaceOffset;

		iLastModifiedDateRange->SerializeL( aBuffer );

		aFreespaceOffset = aBuffer.Position();
		}

	aBuffer.PositionL( conditionOffset );
	
	condition.SerializeL( aBuffer );
	
	// move back to after sub condition
	aBuffer.PositionL( aFreespaceOffset );
	}
