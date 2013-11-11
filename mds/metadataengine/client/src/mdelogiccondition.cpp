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

#include "mdelogiccondition.h"
#include "mdeobjectcondition.h"
#include "mdeboolpropertycondition.h"
#include "mderelationcondition.h"
#include "mdeeventcondition.h"
#include "mdcserializationbuffer.h"
#include "mdcquery.h"

#include "mdepanic.h"

EXPORT_C CMdELogicCondition* CMdELogicCondition::NewL(TLogicConditionOperator aOperator)
    {
    CMdELogicCondition *self = NewLC(aOperator);
    CleanupStack::Pop(self);
    return self;
    }


EXPORT_C CMdELogicCondition* CMdELogicCondition::NewLC(TLogicConditionOperator aOperator)
    {
    CMdELogicCondition *self = new(ELeave) CMdELogicCondition(aOperator);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;    
    }


CMdELogicCondition::CMdELogicCondition(TLogicConditionOperator aOperator)
        : CMdECondition(EConditionTypeLogic), iOperator(aOperator)
    {
    }


void CMdELogicCondition::ConstructL()
    {
    // Base class construction.
    ConditionConstruct();
    }



CMdELogicCondition::~CMdELogicCondition()
    {
    iChildren.ResetAndDestroy();
    iChildren.Close();
    }


EXPORT_C TLogicConditionOperator CMdELogicCondition::Operator() const
    {
    return iOperator;
    }


EXPORT_C void CMdELogicCondition::SetOperator(TLogicConditionOperator aLogicOperator)
    {
    AssertNotLocked();
    
    __ASSERT_DEBUG(aLogicOperator > ELogicConditionOperatorFirst 
                    && aLogicOperator < ELogicConditionOperatorLast,
                    TMdEPanic::Panic(TMdEPanic::EInternal));
    
    iOperator = aLogicOperator;
    }


EXPORT_C TInt CMdELogicCondition::Count() const
    {
    return iChildren.Count();
    }
    

EXPORT_C CMdECondition& CMdELogicCondition::Condition(TInt aIndex) const
    {
    __ASSERT_DEBUG(aIndex >= 0 && aIndex < Count(),
                    TMdEPanic::Panic(TMdEPanic::EInternal));
    
    return *iChildren[aIndex];
    }


EXPORT_C void CMdELogicCondition::Remove(TInt aIndex)
    {
    AssertNotLocked();

    __ASSERT_DEBUG(aIndex >= 0 && aIndex < Count(),
                    TMdEPanic::Panic(TMdEPanic::EInternal));
                    
    iChildren.Remove(aIndex);
    }

void CMdELogicCondition::SetLocked(TBool aLocked)
    {
    // Base class locking.
    CMdECondition::SetLocked(aLocked);    
    
    // Lock children as well.
    for( TInt i = iChildren.Count()- 1; i >=0; i-- )
        {
        iChildren[i]->SetLocked(aLocked);
        }
    }

TUint32 CMdELogicCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	// get flag from childrens
	for( TInt i = iChildren.Count() - 1; i >=0; i-- )
		{
		iOptimizationFlags |= iChildren[i]->InternalQueryOptimizationFlags(aFlags);
		}

	return 0;
	}


TUint32 CMdELogicCondition::RequiredBufferSize() const
	{
	TUint32 bufferSize = sizeof( TMdCLogicCondition );

	const TInt count = iChildren.Count();

	// required size for children offsets (children count * TUint32)
	bufferSize += count * CMdCSerializationBuffer::KRequiredSizeForTUint32;

	// Required size for childrens
	for( TInt i = count - 1; i >=0; i-- )
		{
		bufferSize += iChildren[i]->RequiredBufferSize();
		}

	return bufferSize;
	}

void CMdELogicCondition::SerializeL(CMdCSerializationBuffer& aBuffer, 
		TMdCOffset& aFreespaceOffset) const
	{
	const TUint32 childrenCount = (TUint32)iChildren.Count();

	// position after logic condition
	const TUint32 conditionOffset = aBuffer.Position() + sizeof( TMdCLogicCondition );

	TMdCLogicCondition condition;

	condition.iConditionType = iType;
	condition.iNegated = iNegated;
	condition.iOperator = iOperator;
	condition.iOptimizationFlags = iOptimizationFlags;
	condition.iChildConditions.iPtr.iCount = childrenCount;
	condition.iChildConditions.iPtr.iOffset = conditionOffset;

	condition.SerializeL( aBuffer );

	// begin position of child condition offsets
	TUint32 childOffsetPosition = conditionOffset;

	// begin position of child conditions
	TUint32 childPosition = childOffsetPosition + 
			CMdCSerializationBuffer::KRequiredSizeForTUint32 * childrenCount;

	for( TInt i = 0 ; i < childrenCount; i++)
		{
		// Move to correct position of children offsets
		aBuffer.PositionL( childOffsetPosition );

		// Insert offset for current child
		childOffsetPosition = aBuffer.InsertL( childPosition );

		// Move to correct position of child
		aBuffer.PositionL( childPosition );

		// Insert current child
		iChildren[i]->SerializeL( aBuffer, childPosition );		
		}

	// take position after subconditions
	aFreespaceOffset = aBuffer.Position();
	}

EXPORT_C CMdELogicCondition& 
CMdELogicCondition::AddLogicConditionL(TLogicConditionOperator aLogicOperator)
    {
    AssertNotLocked();

    CMdELogicCondition* condition = NewLC(aLogicOperator);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(const CMdEObjectDef& aObjectDef)
    {
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		EObjectConditionCompareObjectDef,
    		KNoId, &aObjectDef );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;    
    }


EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(TItemId aObjectId)
    {
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		EObjectConditionCompareId, aObjectId );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;    
    }

EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(const RArray<TItemId>& aObjectIds)
    {
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		EObjectConditionCompareIds, &aObjectIds );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;    
    }

EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow)
	{
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		EObjectConditionCompareGuid, KNoId, NULL, aObjectGuidHigh, aObjectGuidLow);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
	}

EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(TObjectConditionCompareMethod aCompareMethod, 
                                        const TDesC& aString)
    {
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		aCompareMethod, NULL, &aString );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdEObjectCondition& 
CMdELogicCondition::AddObjectConditionL(const TMdEUintRange& aRange)
    {
    AssertNotLocked();

    CMdEObjectCondition* condition = CMdEObjectCondition::NewLC( 
    		EObjectConditionCompareUsageCount, NULL, NULL, &aRange );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEPropertyCondition& 
CMdELogicCondition::AddPropertyConditionL(const CMdEPropertyDef& aPropertyDef)
    {
    AssertNotLocked();

    CMdEPropertyCondition* condition 
        = CMdEPropertyCondition::NewLC(aPropertyDef);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEBoolPropertyCondition& 
CMdELogicCondition::AddPropertyConditionL(const CMdEPropertyDef& aPropertyDef,
                                          TBool aBoolValue)
    {
    AssertNotLocked();

    CMdEBoolPropertyCondition* condition 
        = CMdEBoolPropertyCondition::NewLC(aPropertyDef, aBoolValue);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }
    

#define MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(RangeType) \
EXPORT_C CMdE##RangeType##RangePropertyCondition& \
CMdELogicCondition::AddPropertyConditionL(const CMdEPropertyDef& aPropertyDef, \
                                          const TMdE##RangeType##Range& aRange) \
    { \
    AssertNotLocked(); \
    CMdE##RangeType##RangePropertyCondition* condition  \
        = CMdE##RangeType##RangePropertyCondition::NewLC(aPropertyDef, aRange); \
    AddL(condition); \
    CleanupStack::Pop(condition); \
    return *condition; \
    }

MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(Int)
MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(Int64)
MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(Uint)
MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(Real)
MDE_IMPLEMENT_ADD_RANGE_PROPERTY_CONDITION(Time)

EXPORT_C CMdETextPropertyCondition& CMdELogicCondition::AddPropertyConditionL(
                                          const CMdEPropertyDef& aPropertyDef,
                                          TTextPropertyConditionCompareMethod aCompareMethod,
                                          const TDesC& aText)
    {
    AssertNotLocked();

    CMdETextPropertyCondition* condition = CMdETextPropertyCondition::NewLC( aPropertyDef, 
                                                                                                                      aCompareMethod, aText );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdERelationCondition& 
CMdELogicCondition::AddRelationConditionL(TItemId aRelationId, TRelationConditionSide aSide)
    {
    AssertNotLocked();

    CMdERelationCondition* condition = CMdERelationCondition::NewLC(aRelationId, NULL, aSide);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdERelationCondition& CMdELogicCondition::AddRelationConditionL(
		RArray<TItemId>& aRelationIds, TRelationConditionSide aSide)
    {
    AssertNotLocked();

    CMdERelationCondition* condition = CMdERelationCondition::NewLC(
    		KNoId, &aRelationIds, aSide);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdERelationCondition& 
CMdELogicCondition::AddRelationConditionL(TRelationConditionSide aSide)
    {
    AssertNotLocked();

    CMdERelationCondition* condition = CMdERelationCondition::NewLC(KNoId, NULL, aSide);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }
    
    
EXPORT_C CMdERelationCondition& 
CMdELogicCondition::AddRelationConditionL(const CMdERelationDef& aRelationDef,
                                          TRelationConditionSide aSide)
    {
    AssertNotLocked();

    CMdERelationCondition* condition = CMdERelationCondition::NewLC( 
    		KNoId, NULL, aSide, &aRelationDef );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdERelationCondition& 
CMdELogicCondition::AddRelationConditionL(const CMdERelationDef& aRelationDef,
    const TMdEIntRange& aParameterRange,
    TRelationConditionSide aSide)
    {
    AssertNotLocked();

    CMdERelationCondition* condition = CMdERelationCondition::NewLC( KNoId, 
    		NULL, aSide, &aRelationDef, &aParameterRange );
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL(TItemId aEventId)
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC(aEventId);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }

EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL(const CMdEEventDef& aEventDef)
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC(KNoId,&aEventDef);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL()
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC();
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL(const TMdETimeRange& aCreationTimeRange)
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC(KNoId, NULL, &aCreationTimeRange);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL(TEventConditionCompareMethod aCompareMethod, 
                                       const TDesC& aString)
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC(KNoId, NULL, NULL, aCompareMethod, &aString);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


EXPORT_C CMdEEventCondition& 
CMdELogicCondition::AddEventConditionL(const TMdETimeRange& aCreationTimeRange,
                                       TEventConditionCompareMethod aCompareMethod, 
                                       const TDesC& aString)
    {
    AssertNotLocked();

    CMdEEventCondition* condition = CMdEEventCondition::NewLC(KNoId, NULL, &aCreationTimeRange, aCompareMethod, &aString);
    AddL(condition);
    CleanupStack::Pop(condition);
    return *condition;
    }


CMdECondition& CMdELogicCondition::AddL(CMdECondition* aCondition)
    {
    AssertNotLocked();

    iChildren.AppendL( aCondition );
    return *aCondition;
    }
