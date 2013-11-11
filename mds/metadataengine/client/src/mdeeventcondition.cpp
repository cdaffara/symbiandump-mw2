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
* Description:  Representation of query event condition
*
*/

#include "mdeeventcondition.h"
#include "mdelogiccondition.h"
#include "mdeeventdef.h"
#include "mdscommoninternal.h"
#include "mdepanic.h"
#include "mdcserializationbuffer.h"
#include "mdcquery.h"

CMdEEventCondition* CMdEEventCondition::NewL( TItemId aEventId, const CMdEEventDef* aEventDef,
		const TMdETimeRange* aCreationTimeRange, TEventConditionCompareMethod aCompareMethod,
        const TDesC* aString )
    {
    CMdEEventCondition* self = CMdEEventCondition::NewLC( aEventId, aEventDef, aCreationTimeRange,
    		aCompareMethod, aString );
    CleanupStack::Pop( self );
    return self;
    }
	
CMdEEventCondition* CMdEEventCondition::NewLC(TItemId aEventId, const CMdEEventDef* aEventDef,
		const TMdETimeRange* aCreationTimeRange, TEventConditionCompareMethod aCompareMethod,
        const TDesC* aString )
    {
    CMdEEventCondition* self = new (ELeave) CMdEEventCondition( aEventId, aEventDef );
	CleanupStack::PushL( self );
	self->ConstructL( aCreationTimeRange, aCompareMethod, aString );
	return self;
    }

CMdEEventCondition::CMdEEventCondition(TItemId aEventId, const CMdEEventDef* aEventDef)
	: CMdECondition(EConditionTypeEvent), iEventId(aEventId), iEventDef(aEventDef)
	{
    iCompareMethod = EEventConditionCompareNone;
    iCreationTimeRange = NULL;
    iObjectConditions = NULL;
    iSourceConditions = NULL;
    iParticipantConditions = NULL;
    iURI = NULL;
	}

void CMdEEventCondition::ConstructL(const TMdETimeRange* aCreationTimeRange,
                                    TEventConditionCompareMethod aCompareMethod,
                                    const TDesC* aString)
    {
    iCompareMethod = aCompareMethod;
    
    if(aCreationTimeRange)
        {
        // Make a copy of the creation time range.
        iCreationTimeRange = new (ELeave) TMdETimeRange(*aCreationTimeRange);
        }

    if(aString)
        {            
        switch(aCompareMethod)
            {
            case EEventConditionCompareNone:
                // No comparison specified.
                break;
            
            case EEventConditionCompareSourceURI:
                // Make a copy of the source URI descriptor.
                iURI = aString->AllocL();
                break;

            case EEventConditionCompareParticipantURI:
                // Make a copy of the participant URI descriptor.
                iURI = aString->AllocL();
                break;

            default:
                __ASSERT_DEBUG( EFalse, TMdEPanic::Panic(TMdEPanic::EInternal) );
            }
        }
    }


CMdEEventCondition::~CMdEEventCondition()
	{
    delete iCreationTimeRange;
    iCreationTimeRange = NULL;
    delete iObjectConditions;
    iObjectConditions = NULL;
    delete iSourceConditions;
    iSourceConditions = NULL;
    delete iParticipantConditions;
    iParticipantConditions = NULL;
    delete iURI;
    iURI = NULL;
	}


EXPORT_C CMdELogicCondition& CMdEEventCondition::ObjectConditionsL()
    {
	if( !iObjectConditions )
		{
		iObjectConditions = CMdELogicCondition::NewL( ELogicConditionOperatorAnd ); 
		}

    return *iObjectConditions;
    }


EXPORT_C CMdELogicCondition& CMdEEventCondition::SourceObjectConditionsL()
	{
	if( !iSourceConditions )
		{
		iSourceConditions = CMdELogicCondition::NewL( ELogicConditionOperatorAnd ); 
		}

    return *iSourceConditions;
	}


EXPORT_C CMdELogicCondition& CMdEEventCondition::ParticipantObjectConditionsL()
	{
	if( !iParticipantConditions )
		{
		iParticipantConditions = CMdELogicCondition::NewL( ELogicConditionOperatorAnd ); 
		}

    return *iParticipantConditions;
	}


EXPORT_C TItemId CMdEEventCondition::EventId() const
    {
    return iEventId;
    }


EXPORT_C const CMdEEventDef* CMdEEventCondition::EventDef() const
    {
    return iEventDef;
    }
    

EXPORT_C const TMdETimeRange* CMdEEventCondition::TimeRange() const
    {
    return iCreationTimeRange;
    }


EXPORT_C const TDesC* CMdEEventCondition::SourceURI() const
    {
    if ( iCompareMethod == EEventConditionCompareSourceURI )
    	{
    	return iURI;
    	}
    
    return NULL;
    }


EXPORT_C const TDesC* CMdEEventCondition::ParticipantURI() const
    {
    if ( iCompareMethod == EEventConditionCompareParticipantURI )
    	{
    	return iURI;
    	}
    
    return NULL;
    }

TUint32 CMdEEventCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	aFlags |= EContainsEventCondition;
	return EContainsEventCondition;
	}

TUint32 CMdEEventCondition::RequiredBufferSize() const
	{
	TUint32 bufferSize = sizeof( TMdCEventCondition );

	// size of each object to be serialized
	if( iCreationTimeRange )
		{		
		bufferSize += iCreationTimeRange->RequiredBufferSize();
		}

	if( iObjectConditions )
		{
		bufferSize += iObjectConditions->RequiredBufferSize();
		}

	if( iSourceConditions )
		{
		bufferSize += iSourceConditions->RequiredBufferSize();
		}

	if( iParticipantConditions )
		{
		bufferSize += iParticipantConditions->RequiredBufferSize();
		}

	if ( iURI )
		{
		bufferSize += CMdCSerializationBuffer::RequiredSize( *iURI );
		}

	return bufferSize;
	}


void CMdEEventCondition::SerializeL(CMdCSerializationBuffer& aBuffer,
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCEventCondition condition;

	condition.iConditionType = iType;
	condition.iNegated = iNegated;
	condition.iCompareMethod = iCompareMethod;
	condition.iEventId = iEventId;

	if( iEventDef )
		{	
		condition.iEventDefId = iEventDef->Id();
		}
	else
		{
		condition.iEventDefId = KNoDefId;
		}
	
	// init offsets to "zero", updated later if needed
	condition.iCreationTimeRange = KNoOffset;
	condition.iObjectCondition = KNoOffset;
	condition.iSourceCondition = KNoOffset;
	condition.iParticipantCondition = KNoOffset;
	condition.iUriCondition = KNoOffset;

	const TUint32 conditionOffset = aBuffer.Position();

	// get position after condition
	aFreespaceOffset = conditionOffset + sizeof( TMdCEventCondition );
	aBuffer.PositionL( aFreespaceOffset );

	if( iCreationTimeRange )
		{		
		condition.iCreationTimeRange = aFreespaceOffset;

		iCreationTimeRange->SerializeL( aBuffer );

		aFreespaceOffset = aBuffer.Position();
		}

	if( iObjectConditions )
		{		
		condition.iObjectCondition = aFreespaceOffset;
		
		iObjectConditions->SerializeL( aBuffer, aFreespaceOffset );
		
		aFreespaceOffset = aBuffer.Position();
		}

	if( iSourceConditions )
		{		
		condition.iSourceCondition = aFreespaceOffset;

		iSourceConditions->SerializeL( aBuffer, aFreespaceOffset );

		aFreespaceOffset = aBuffer.Position();
		}

	if( iParticipantConditions )
		{		
		condition.iParticipantCondition = aFreespaceOffset;

		iParticipantConditions->SerializeL( aBuffer, aFreespaceOffset );

		aFreespaceOffset = aBuffer.Position();
		}

	if ( ( EEventConditionCompareSourceURI == iCompareMethod ) || 
		( EEventConditionCompareParticipantURI == iCompareMethod ) )
		{
		condition.iUriCondition = aFreespaceOffset;

		aBuffer.InsertL( *iURI );

		aFreespaceOffset = aBuffer.Position();
		}

	aBuffer.PositionL( conditionOffset );

	condition.SerializeL( aBuffer );
	}
