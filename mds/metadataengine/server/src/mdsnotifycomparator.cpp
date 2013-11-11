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
* Description:  Comparator algorithms for notifier*
*/

#include "mdsnotifycomparator.h"

#include "mdcresult.h"
#include "mdcitem.h"
#include "mdccommon.pan"
#include "mdcserializationbuffer.h"
#include "mderange.h"
#include "mdscommoninternal.h"
#include "mdcquery.h"
#include "mdccommon.h"
#include "mdeinternalerror.h"

CMdSNotifyComparator* CMdSNotifyComparator::NewL()
    {
    CMdSNotifyComparator* that = CMdSNotifyComparator::NewLC();
    CleanupStack::Pop( that );
    return that;
    }

CMdSNotifyComparator* CMdSNotifyComparator::NewLC()
	{
	CMdSNotifyComparator* that = new(ELeave) CMdSNotifyComparator();
	CleanupStack::PushL( that );
	that->ConstructL();
	return that;
	}


CMdSNotifyComparator::~CMdSNotifyComparator()
    {
    
    }

CMdSNotifyComparator::CMdSNotifyComparator()
    {
    
    }

void CMdSNotifyComparator::ConstructL()
    {
    }

TBool CMdSNotifyComparator::MatchL(
	TDefId aNamespaceDefId, TUint32 aType,
    CMdCSerializationBuffer& aSerializedCondition,
    CMdCSerializationBuffer& aSerializedItems,
    CMdCSerializationBuffer& aSerializedItemIds,
    RArray<TItemId>& aMatchingItemIdArray,
    RArray<TItemId>& aAllItemsIdArray,
    RPointerArray<HBufC>& aMatchingItemUriArray,
    RPointerArray<HBufC>& aAllItemsUriArray,
    TBool aAllowConfidential,
    TBool uriNotify,
    TBool& aAllMatched,
    TBool& aAllItemsFetched )
    {
    const TMdCItems& items = TMdCItems::GetFromBufferL( aSerializedItems );

    const TMdCItemIds& itemIds = TMdCItemIds::GetFromBufferL( aSerializedItemIds );

	// check if some namespace definitions are not the same
    if( itemIds.iNamespaceDefId != aNamespaceDefId )
    	{
    	return EFalse;
    	}

    aAllMatched = EFalse;
    
    if( ( items.iObjects.iPtr.iCount > 0 ) && 
    	( itemIds.iObjectIds.iPtr.iCount > 0 ) && 
    	( aType & ( EObjectNotifyAdd | EObjectNotifyModify | EObjectNotifyAddWithUri | EObjectNotifyModifyWithUri ) ) )
    	{
    	// object ID count and object item count should match
		__ASSERT_DEBUG( items.iObjects.iPtr.iCount == itemIds.iObjectIds.iPtr.iCount, MMdCCommon::Panic( KErrCorrupt ) );

    	aSerializedItemIds.PositionL( itemIds.iObjectIds.iPtr.iOffset );
    	TBool allItemsFetched( aAllItemsFetched );
    	for( TUint32 i = 0; i < itemIds.iObjectIds.iPtr.iCount; i++ )
    		{
    		TItemId objectId;
    		aSerializedItemIds.ReceiveL( objectId );

    		if( aSerializedCondition.Size() > 0 )
    			{
	            TBool results( EFalse );
	            TBool succeed( EFalse );
    			
	            // move condition buffer's position beginnig of the buffer
	            aSerializedCondition.PositionL( KNoOffset  );
	            
	            const TMdCLogicCondition& logicCondition = 
	            		TMdCLogicCondition::GetFromBufferL( aSerializedCondition );

	            for ( TUint32 j = 0; j < logicCondition.iChildConditions.iPtr.iCount; j++ )
	                {
		    		// set correct position to item buffer
	    	    	aSerializedItems.PositionL( items.iObjects.iPtr.iOffset + 
	    	    			i * sizeof(TMdCObject) );

	    	    	// set correct positin to condition buffer
	    	    	aSerializedCondition.PositionL( 
	    	    			logicCondition.iChildConditions.iPtr.iOffset + 
	    	    			j * CMdCSerializationBuffer::KRequiredSizeForTUint32 );
	    	    	TUint32 conditionOffset;
	    	    	aSerializedCondition.ReceiveL( conditionOffset );
	    	    	aSerializedCondition.PositionL( conditionOffset );

	                results = MatchObjectL( aSerializedCondition, 
	                		aSerializedItems, objectId, aAllowConfidential );

	                succeed = results;
	                if ( logicCondition.iOperator == ELogicConditionOperatorAnd )
	                    {
	                    if( !results )
	                        {
	                        break;
	                        }
	                    }

	                if ( logicCondition.iOperator == ELogicConditionOperatorOr )
	                    {
	                    if( results )
	                        {
	                        break;
	                        }
	                    }
	                }
    		
	            if ( logicCondition.iNegated )    
	                {
	                succeed = !( succeed );
	                }

	    	    if ( succeed )    
	                {
	                if( uriNotify )
	                    {
	                    // set correct position to item buffer
	                    aSerializedItems.PositionL( items.iObjects.iPtr.iOffset + 
	                               i * sizeof(TMdCObject) );
	                    
	                   const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedItems );

	                    aSerializedItems.PositionL( object.iUri.iPtr.iOffset );
	                    HBufC* uriBuf = aSerializedItems.ReceiveDes16L();
	                    aMatchingItemUriArray.AppendL( uriBuf );
	                    }
	                aMatchingItemIdArray.AppendL( objectId );
	                }
    			}
    		else
    			{
                aAllMatched = ETrue;		
                if( uriNotify && (!aAllItemsFetched || !allItemsFetched) )
                    {
                    // set correct position to item buffer
                    aSerializedItems.PositionL( items.iObjects.iPtr.iOffset + 
                               i * sizeof(TMdCObject) );
                    
                   const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedItems );

                    aSerializedItems.PositionL( object.iUri.iPtr.iOffset );
                    HBufC* uriBuf = aSerializedItems.ReceiveDes16L();
                    aAllItemsUriArray.AppendL( uriBuf );
                    aAllItemsIdArray.AppendL( objectId );
                    allItemsFetched = ETrue;
                    }
                else if( !aAllItemsFetched || !allItemsFetched )
                    {
                    aAllItemsIdArray.AppendL( objectId );
                    allItemsFetched = ETrue;
                    }
                }
    		}
    	if( allItemsFetched )
    	    {
    	    aAllItemsFetched = ETrue;
    		}
    	}
    else if( ( items.iEvents.iPtr.iCount > 0 ) && 
    		 ( itemIds.iEventIds.iPtr.iCount > 0 ) && 
    		 ( aType & ( EEventNotifyAdd ) ) )
    	{
    	// event ID count and event item count should match
		__ASSERT_DEBUG( items.iEvents.iPtr.iCount == itemIds.iEventIds.iPtr.iCount, MMdCCommon::Panic( KErrCorrupt ) );

		aSerializedItemIds.PositionL( itemIds.iEventIds.iPtr.iOffset );
    	for( TUint32 i = 0; i < itemIds.iEventIds.iPtr.iCount; i++ )
    		{
    		TItemId eventId;
    		aSerializedItemIds.ReceiveL( eventId );
    		
    		if( aSerializedCondition.Size() > 0 )
    			{
	    		// set correct position to item buffer
	    		aSerializedItems.PositionL( items.iEvents.iPtr.iOffset + i * sizeof(TMdCEvent) );

	    		// move condition buffer's position to the begin of the buffer
	    		aSerializedCondition.PositionL( KNoOffset );

	    		if( MatchEventL( aSerializedCondition, aSerializedItems, eventId ) )
	    			{
	    			aMatchingItemIdArray.AppendL( eventId );
	    			}
    			}
    		else
    			{
    			aMatchingItemIdArray.AppendL( eventId );
    			}
    		}
    	}
    else if( ( items.iRelations.iPtr.iCount > 0 ) && 
    		 ( itemIds.iRelationIds.iPtr.iCount > 0 ) && 
    		 ( aType & ( ERelationNotifyAdd | ERelationNotifyModify ) ) )
    	{
    	// relation ID count and relation item count should match
		__ASSERT_DEBUG( items.iRelations.iPtr.iCount == itemIds.iRelationIds.iPtr.iCount, MMdCCommon::Panic( KErrCorrupt ) );

    	aSerializedItemIds.PositionL( itemIds.iRelationIds.iPtr.iOffset );
    	for( TUint32 i = 0; i < itemIds.iRelationIds.iPtr.iCount; i++ )
    		{
    		TItemId relationId;
    		aSerializedItemIds.ReceiveL( relationId );

			if( aSerializedCondition.Size() > 0 )
				{
	    		// set correct position to item buffer
	    		aSerializedItems.PositionL( items.iRelations.iPtr.iOffset
	    				+ i * sizeof(TMdCRelation) );

	    		// move condition buffer's position to the begin of the buffer
	    		aSerializedCondition.PositionL( KNoOffset );

	    		if( MatchRelationL( aSerializedCondition, aSerializedItems, relationId ) )
	    			{
	    			aMatchingItemIdArray.AppendL( relationId );
	    			}
				}
			else
				{
				aMatchingItemIdArray.AppendL( relationId );
				}
    		}
    	}

    if( aMatchingItemIdArray.Count() > 0 || ( aAllItemsIdArray.Count() > 0 && aAllMatched ))
    	{
    	return ETrue;
    	}
    else
    	{
    	return EFalse;
    	}
    }

TBool CMdSNotifyComparator::MatchObjectL(
		CMdCSerializationBuffer& aSerializedCondition,
		CMdCSerializationBuffer& aSerializedItem, TItemId aObjectId, 
		TBool aAllowConfidential )
	{
 	// check if object is failed
	if( aObjectId == KNoId )
		{
		return EFalse;
		}

	// Check type
	const TMdCCondition& condition = 
			TMdCCondition::GetFromBufferL( aSerializedCondition );

	switch( condition.iConditionType )
		{
	    case EConditionTypeObject:
	        {
	    	return ( CheckObjectL( aSerializedCondition, aSerializedItem, 
	    			aAllowConfidential ) );
	    	}
		case EConditionTypeProperty:
		case EConditionTypePropertyIntRange:
		case EConditionTypePropertyInt64Range:
		case EConditionTypePropertyUintRange:
		case EConditionTypePropertyRealRange:
		case EConditionTypePropertyTimeRange:
	    case EConditionTypePropertyText:
    	case EConditionTypePropertyBool:
	        {
   			return ( CheckPropertyL( aSerializedCondition, aSerializedItem ) );
	    	}
    	default:
#ifdef _DEBUG
    		User::Panic( _L("MdSNCMaO") , KErrMdEUnknownConditionType );
#endif    		
    	    break;
		}

	return EFalse;
	}

TBool CMdSNotifyComparator::CheckPropertyL( 
		CMdCSerializationBuffer& aSerializedCondition,
    	CMdCSerializationBuffer& aSerializedItem )
    {
    const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedItem );
    
    const TMdCPropertyCondition& propertyCondition = 
    		TMdCPropertyCondition::GetFromBufferL( aSerializedCondition );

    for( TUint32 i=0; i < object.iProperties.iPtr.iCount; i++ )
        {
        aSerializedItem.PositionL( object.iProperties.iPtr.iOffset
        		+ i * sizeof(TMdCProperty) );
        const TMdCProperty& property = TMdCProperty::GetFromBufferL( aSerializedItem );

        // check propertyDefId doesn't match -> skip it
        if( property.iPropertyDefId == propertyCondition.iPropertyDefId )
            {
            // move condition buffer to begin of the condition
            aSerializedCondition.PositionL( propertyCondition.iCondition );

            switch( propertyCondition.iConditionType )
	            {
	            case EConditionTypeProperty:
	            	{
		            // check only if existing condition is negated
	            	if( propertyCondition.iNegated )
	            		{
	            		return EFalse;
	            		}
	            	else
	            		{
	            		return ETrue;
	            		}
	            	}
	            
   	            case EConditionTypePropertyIntRange:
	                {
		            TInt32 rangeType32( 0 );
		            aSerializedCondition.ReceiveL( rangeType32 );
		            const TMdERangeType rangeType = ( TMdERangeType )rangeType32;

		            TMdCValueUnion minValue;
                    aSerializedCondition.ReceiveL( minValue );
                    TMdCValueUnion maxValue;
                    aSerializedCondition.ReceiveL( maxValue );

		            // init range for next test
                    TMdEIntRange range( minValue.iInt32, maxValue.iInt32, 
                    		rangeType );
		            
   	                if( range.InRange( property.iValue.iInt32 ) == propertyCondition.iNegated )
                        {
                        return EFalse;
                        }   
	                break;
	                }
	            case EConditionTypePropertyInt64Range:
	                {
		            TInt32 rangeType32( 0 );
		            aSerializedCondition.ReceiveL( rangeType32 );
		            const TMdERangeType rangeType = ( TMdERangeType )rangeType32;

		            TMdCValueUnion minValue;
                    aSerializedCondition.ReceiveL( minValue );
                    TMdCValueUnion maxValue;
                    aSerializedCondition.ReceiveL( maxValue );

                    // init range for next test
                    TMdEInt64Range range( minValue.iInt64, maxValue.iInt64, 
                    		rangeType );
                    
                    // test if valueType is in the range
                    if( range.InRange( property.iValue.iInt64 ) == propertyCondition.iNegated )
                        {
                        return EFalse;
                        }	                
	                break;
	                }
	            case EConditionTypePropertyUintRange:
	                {
		            TInt32 rangeType32( 0 );
		            aSerializedCondition.ReceiveL( rangeType32 );
		            const TMdERangeType rangeType = ( TMdERangeType )rangeType32;

		            TMdCValueUnion minValue;
                    aSerializedCondition.ReceiveL( minValue );
                    TMdCValueUnion maxValue;
                    aSerializedCondition.ReceiveL( maxValue );

                    // init range for next test
                    TMdEUintRange range( minValue.iUint32, maxValue.iUint32, 
                    		rangeType );
	               
   	                if( range.InRange( property.iValue.iUint32 ) == propertyCondition.iNegated )
                        {
                        return EFalse;
                        }   
	                break;
	                }
	            case EConditionTypePropertyRealRange:
	                {
		            TInt32 rangeType32( 0 );
		            aSerializedCondition.ReceiveL( rangeType32 );
		            const TMdERangeType rangeType = ( TMdERangeType )rangeType32;

		            TMdCValueUnion minValue;
                    aSerializedCondition.ReceiveL( minValue );
                    TMdCValueUnion maxValue;
                    aSerializedCondition.ReceiveL( maxValue );
                    
                    // init range for next test
                    TMdERealRange range( minValue.iReal, maxValue.iReal, rangeType );

                    // test if valueType is in the range
                    if( range.InRange( property.iValue.iReal ) == propertyCondition.iNegated )
                        {
                        return EFalse;
                        }	                
	                break;
	                }
	            case EConditionTypePropertyTimeRange:
                    {
		            TInt32 rangeType32( 0 );
		            aSerializedCondition.ReceiveL( rangeType32 );
		            TMdERangeType rangeType = ( TMdERangeType )rangeType32;

		            TMdCValueUnion minValue;
                    aSerializedCondition.ReceiveL( minValue );
                    TMdCValueUnion maxValue;
                    aSerializedCondition.ReceiveL( maxValue );

                    // init range for next test
                    TMdETimeRange range( minValue.iInt64, maxValue.iInt64, 
                    		rangeType );

	                TTime value( property.iValue.iInt64 );
                    // test if valueType is in the range
                    if( range.InRange( property.iValue.iInt64 ) == propertyCondition.iNegated )
                        {
                        return EFalse;
                        }	                
                    break;
                    }
                case EConditionTypePropertyText:
	                {
	                // SerializedItem
	                aSerializedItem.PositionL( property.iValue.iPtr.iOffset );
	                TPtrC16 value = aSerializedItem.ReceivePtr16L();

	                // ConditionItem
	                TUint32 condCompareMethod( 0 );
	                aSerializedCondition.ReceiveL( condCompareMethod );
	                TPtrC16 condValue = aSerializedCondition.ReceivePtr16L();

                    //Check, that compare method is in range
#ifdef _DEBUG
	                if( condCompareMethod <= ETextPropertyConditionCompareFirst || condCompareMethod >= ETextPropertyConditionCompareLast )
	                	{
						User::Panic( _L("MdSNCCP1") , KErrMdEUnknownConditionCompareMethod );
	                	}
#endif
	                TBool compareResult( EFalse );

	                switch( condCompareMethod )
		                {
	                    case ETextPropertyConditionCompareEquals:
	   		                {
	   		                if( (value == condValue ) )
	   		                    {
	   		                    compareResult = EFalse;
	   		                    }
	   		                break;
	   		                }
	                    case ETextPropertyConditionCompareContains:
	   		                {
	   		                compareResult = FindDes( value, condValue ); 
	   		                break;
	   		                }
	                    case ETextPropertyConditionCompareBeginsWith:
	   		                {
	   		                compareResult = CompareDesBeginsWith( value, condValue ); 
                            break;	   		               
	   		                }
	                    case ETextPropertyConditionCompareEndsWith:
	                        {
	   		                compareResult = CompareDesEndsWith( value, condValue );
                            break;	   		               
	   		                }
                        default: 
                            {
#ifdef _DEBUG
                            User::Panic( _L("MdSNCCP2") , KErrMdEUnknownConditionCompareMethod );
#endif
   		                    }
		                }          
	                
	                if( compareResult == propertyCondition.iNegated )
	                    {
	                    return EFalse;
	                    }
	                break;
	                }
	            case EConditionTypePropertyBool:
    		        {
			        const TBool value( property.iValue.iInt32 );

			        TBool condValue( EFalse );
			        aSerializedCondition.ReceiveL( condValue );

			        if( ( value == condValue ) == propertyCondition.iNegated )
				        {
				        return EFalse;
				        }
			        break;
    		        }
	            default:
	            	{
#ifdef _DEBUG
                    User::Panic( _L("MdSNCCP3") , KErrMdEUnknownConditionType );
#endif
	            	User::Leave( KErrMdEUnknownConditionType );
	            	}
	            } // switch

            return ETrue;

            } // if

        } // for

    return EFalse;
    }

TBool CMdSNotifyComparator::CheckObjectL( CMdCSerializationBuffer& aSerializedCondition,
    			   						  CMdCSerializationBuffer& aSerializedItem,
    			   						  TBool aAllowConfidential )
    {
    const TMdCObject& object = TMdCObject::GetFromBufferL( aSerializedItem );

	const TBool confidential = object.iFlags & EMdEObjectFlagConfidential;

	// check if object is confidential and confidentials are not allowed
	if( confidential && ( aAllowConfidential == EFalse ) )
		{
		return EFalse;
		}

	// check if no condition defined
	if( aSerializedCondition.Size() <= 0 )
		{
		// pass all successful objects
		return ETrue;
		}

	const TMdCObjectCondition& condition = 
			TMdCObjectCondition::GetFromBufferL( aSerializedCondition );

	switch( condition.iConfidentialityLevel )
		{
	    case EObjectConditionLevelNormal:
	    	if( confidential )
	    		{
	    		return EFalse;
	    		}
			break;
		case EObjectConditionLevelConfidential:
	    	if( confidential == EFalse )
	    		{
	    		return EFalse;
	    		}
			break;

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSNCCO1") , KErrMdEUnknownConfidentialityLevel );
#endif	    	
	    	User::Leave( KErrMdEUnknownConfidentialityLevel );
	    	}
		}

	if( ( condition.iFlags & EMdEObjectFlagPlaceholder ) && 
			( object.iFlags & EMdEObjectFlagPlaceholder ) )
		{
		return EFalse;
		}

	// move position to begin of sub condition
	aSerializedCondition.PositionL( condition.iCondition );
	
	switch( condition.iCompareMethod )
		{
		case EObjectConditionCompareNone:
			break;

		case EObjectConditionCompareId:
			{
			TItemId conditionObjectId;
			aSerializedCondition.ReceiveL( conditionObjectId );

			if( ( conditionObjectId == object.iId ) == condition.iNegated )
				{
				return EFalse;
				}
			}
			break;

        case EObjectConditionCompareUsageCount:
			{
			TInt32 rangeType32;
			aSerializedCondition.ReceiveL( rangeType32 );
			const TMdERangeType rangeType = (TMdERangeType)rangeType32;

			TMdCValueUnion minValue;
			aSerializedCondition.ReceiveL( minValue );
			TMdCValueUnion maxValue;
			aSerializedCondition.ReceiveL( maxValue );

			// init range for next test
			TMdEUintRange range( minValue.iUint32, maxValue.iUint32, 
					rangeType );

			// test if event's creation time is in the range
			if( BoolEqual( range.InRange( object.iUsageCount ), condition.iNegated ) )
				{
				return EFalse;
				}
			}
			break;

		case EObjectConditionCompareGuid:
			{
			TInt64 conditionGuidHigh;
			aSerializedCondition.ReceiveL( conditionGuidHigh );
			TInt64 conditionGuidLow;
			aSerializedCondition.ReceiveL( conditionGuidLow );

			if( ( ( conditionGuidHigh == object.iGuidHigh ) && 
				( conditionGuidLow == object.iGuidLow ) ) == 
				condition.iNegated )
				{
				return EFalse;
				}
			}
			break;

        case EObjectConditionCompareObjectDef:
			{
			TDefId conditionObjectDefId;
			aSerializedCondition.ReceiveL( conditionObjectDefId );

			if( ( conditionObjectDefId == object.iDefId ) == condition.iNegated )
				{
				return EFalse;
				}
			}
			break;

        case EObjectConditionCompareUri:
			{
			aSerializedItem.PositionL( object.iUri.iPtr.iOffset );
			TPtrC16 uri = aSerializedItem.ReceivePtr16L();

			TPtrC16 conditionUri = aSerializedCondition.ReceivePtr16L();

			if( ( conditionUri.Compare( uri ) == 0 ) == condition.iNegated )
				{
				return EFalse;
				}
			}
			break;

        case EObjectConditionCompareUriBeginsWith:
			{
			aSerializedItem.PositionL( object.iUri.iPtr.iOffset );
			TPtrC16 uri = aSerializedItem.ReceivePtr16L();

			TPtrC16 conditionUri = aSerializedCondition.ReceivePtr16L();

			if( CompareDesBeginsWith( uri, conditionUri ) == condition.iNegated )
				{
				return EFalse;
				}
			}
			break;

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSNCCO2") , KErrMdEUnknownConditionCompareMethod );
#endif	    	
	    	User::Leave( KErrMdEUnknownConditionCompareMethod );
	    	}
		}

	return ETrue;
    }


TBool CMdSNotifyComparator::MatchEventL(CMdCSerializationBuffer& aSerializedCondition,
					    			    CMdCSerializationBuffer& aSerializedItem,
					    			    TItemId aEventId)
	{
	const TMdCEvent& event = TMdCEvent::GetFromBufferL( aSerializedItem );

	// check if event is failed
	if( aEventId == 0 )
		{
		return EFalse;
		}

	// check if no condition defined
	if( aSerializedCondition.Size() <= 0 )
		{
		// pass all successful events
		return ETrue;
		}

	const TMdCEventCondition& eventCondition = 
			TMdCEventCondition::GetFromBufferL( aSerializedCondition );

	// check if event condition contains event ID comparsion
	if( eventCondition.iEventId != KNoId )
		{
		if( ( eventCondition.iEventId == event.iId ) == eventCondition.iNegated )
			{
			return EFalse;
			}
		}

	// check if event condition contains event def ID comparsion
	if( eventCondition.iEventDefId != KNoDefId )
		{
		if( ( eventCondition.iEventDefId == event.iDefId ) == eventCondition.iNegated )
			{
			return EFalse;
			}
		}

	// check event condition contains event creation time range comparision
	if( eventCondition.iCreationTimeRange != KNoOffset )
		{
		aSerializedCondition.PositionL( eventCondition.iCreationTimeRange );

		TInt32 rangeType32;
		aSerializedCondition.ReceiveL( rangeType32 );
		const TMdERangeType rangeType = (TMdERangeType)rangeType32;

		TMdCValueUnion minValue;
		aSerializedCondition.ReceiveL( minValue );
		TMdCValueUnion maxValue;
		aSerializedCondition.ReceiveL( maxValue );

		// init range for next test
		TMdEInt64Range range( minValue.iInt64, maxValue.iInt64, rangeType );

		// test if event's creation time is in the range
		if( range.InRange( event.iTime.Int64() ) == eventCondition.iNegated )
			{
			return EFalse;
			}
		}

	// check object condition 
	// (only accept OR-logic condition which contains object ID condition(s))
	if( eventCondition.iObjectCondition != KNoOffset )
		{
		aSerializedCondition.PositionL( eventCondition.iObjectCondition );

		if( MatchObjectIdToObjectIdConditionsL( aSerializedCondition, 
				event.iObjectId ) == eventCondition.iNegated )
			{
			return EFalse;
			}
		}

	// check event condition contains source or participant comparsion
	if( ( EEventConditionCompareSourceURI == eventCondition.iCompareMethod ) || 
		( EEventConditionCompareParticipantURI == eventCondition.iCompareMethod ) )
		{
		aSerializedCondition.PositionL( eventCondition.iUriCondition );

		TPtrC16 eventConditionUri = aSerializedCondition.ReceivePtr16L();

		// source comparsion
		if( EEventConditionCompareSourceURI == eventCondition.iCompareMethod )
			{
			TPtrC16 eventSource( KNullDesC );
			if ( event.iSourceText.iPtr.iCount > 0 )
				{
				aSerializedItem.PositionL( event.iSourceText.iPtr.iOffset );
				eventSource.Set( aSerializedItem.ReceivePtr16L() );
				}

			if( ( eventConditionUri.Compare( eventSource ) == 0 ) == eventCondition.iNegated )
				{
				return EFalse;
				}
			}
		// participant comparsion
		else
			{
			TPtrC16 eventParticipant( KNullDesC );
			if (event.iParticipantText.iPtr.iCount > 0)
				{
				aSerializedItem.PositionL( event.iParticipantText.iPtr.iOffset );
				eventParticipant.Set( aSerializedItem.ReceivePtr16L() );
				}

			if( ( eventConditionUri.Compare( eventParticipant ) == 0 ) == eventCondition.iNegated )
				{
				return EFalse;
				}
			}
		}

	return ETrue;
	}

TBool CMdSNotifyComparator::MatchRelationL(CMdCSerializationBuffer& aSerializedCondition,
    				 					   CMdCSerializationBuffer& aSerializedItem,
    				 					   TItemId aRelationId)
	{
    const TMdCRelation& relation = 
    		TMdCRelation::GetFromBufferL( aSerializedItem );

	// check if event is failed
	if( aRelationId == 0 )
		{
		return EFalse;
		}

	// check if no condition defined
	if( aSerializedCondition.Size() <= 0 )
		{
		// pass all successful events
		return ETrue;
		}

	const TMdCRelationCondition& relationCondition = 
			TMdCRelationCondition::GetFromBufferL( aSerializedCondition );

	// check if relation condition contains relation def ID comparsion
	if( relationCondition.iRelationDefId != KNoDefId &&
		BoolEqual( relationCondition.iRelationDefId == relation.iDefId, 
				relationCondition.iNegated ) )
		{
		return EFalse;
		}

	if ( relationCondition.iRelationId != KNoId &&
		 	 BoolEqual( relationCondition.iRelationId == aRelationId, 
		 			 relationCondition.iNegated ) )
		{
		return EFalse;
		}
	
	if ( relationCondition.iRelationIds.iPtr.iOffset != KNoOffset &&
			BoolEqual( MatchRelationIdsL( relationCondition, 
					aSerializedCondition, aRelationId ), 
					relationCondition.iNegated ) )
		{
		return EFalse;
		}

	if( relationCondition.iGuid != KNoOffset )
		{
		TInt64 conditionGuidHigh;
		aSerializedCondition.ReceiveL( conditionGuidHigh );
		TInt64 conditionGuidLow;
		aSerializedCondition.ReceiveL( conditionGuidLow );

		if ( conditionGuidHigh != 0 && conditionGuidLow != 0 &&
			 ( BoolEqual( conditionGuidHigh == relation.iGuidHigh, 
					 relationCondition.iNegated ) ||
			   BoolEqual( conditionGuidHigh == relation.iGuidLow, 
					   relationCondition.iNegated ) ) )
			{
			return EFalse;
			}
		}

	// check relation condition contains relation parameter range comparision
	if( relation.iParameter != KNoOffset )
		{
		aSerializedCondition.PositionL( relation.iParameter );
		
		TInt32 rangeType32;
		aSerializedCondition.ReceiveL( rangeType32 );
		const TMdERangeType rangeType = (TMdERangeType)rangeType32;

		TMdCValueUnion minValue;
		aSerializedCondition.ReceiveL( minValue );
		TMdCValueUnion maxValue;
		aSerializedCondition.ReceiveL( maxValue );

		// init range for next test
		TMdEIntRange range( minValue.iInt32, maxValue.iInt32, rangeType );

		// test if event's creation time is in the range
		if( BoolEqual( range.InRange( relation.iParameter ), 
				relationCondition.iNegated ) )
			{
			return EFalse;
			}
		}

	if( relationCondition.iLeftObjectCondition != KNoOffset && 
			ERelationConditionSideLeft == relationCondition.iObjectSide )
		{
		aSerializedCondition.PositionL( relationCondition.iLeftObjectCondition );

		if( BoolEqual( MatchObjectIdToObjectIdConditionsL(
				aSerializedCondition, relation.iLeftObjectId ), 
				relationCondition.iNegated ) )
			{
			return EFalse;
			}
		}

	if( relationCondition.iRightObjectCondition != KNoOffset && 
			ERelationConditionSideRight  == relationCondition.iObjectSide )
		{
		aSerializedCondition.PositionL( 
				relationCondition.iRightObjectCondition );

		if( BoolEqual( MatchObjectIdToObjectIdConditionsL(
				aSerializedCondition, relation.iRightObjectId ), 
				relationCondition.iNegated ) )
			{
			return EFalse;
			}
		}

	if ( ( relationCondition.iLeftObjectCondition != KNoOffset || 
			relationCondition.iRightObjectCondition != KNoOffset ) && 
			ERelationConditionSideEither == relationCondition.iObjectSide )
		{
		TBool eitherMatches = EFalse;
		
		if ( relationCondition.iLeftObjectCondition != KNoOffset )
			{
			aSerializedCondition.PositionL( 
					relationCondition.iLeftObjectCondition );
	
			if( !BoolEqual( MatchObjectIdToObjectIdConditionsL(
					aSerializedCondition, relation.iLeftObjectId ), 
					relationCondition.iNegated ) )
				{
				eitherMatches = ETrue;
				}
			}
		
		if ( relationCondition.iRightObjectCondition != KNoOffset )
			{
			aSerializedCondition.PositionL( 
					relationCondition.iRightObjectCondition );

			if( !BoolEqual( MatchObjectIdToObjectIdConditionsL(
					aSerializedCondition, relation.iRightObjectId ), 
					relationCondition.iNegated ) )
				{
				eitherMatches = ETrue;
				}
			}

		// left and right condition didn't match
		if( !eitherMatches )
			{
			return EFalse;
			}
		}

	if ( relationCondition.iLastModifiedDateRange != KNoOffset )
		{
		aSerializedCondition.PositionL( 
				relationCondition.iLastModifiedDateRange );

		TInt32 rangeRule;
		aSerializedCondition.ReceiveL( rangeRule );
		const TMdERangeType rangeType = (TMdERangeType)rangeRule;

		TMdCValueUnion minValue;
		aSerializedCondition.ReceiveL( minValue );
		TMdCValueUnion maxValue;
		aSerializedCondition.ReceiveL( maxValue );

		// init range for next test
		TMdEInt64Range range( minValue.iInt64, maxValue.iInt64, rangeType );

		// test if event's creation time is in the range
		if( BoolEqual( range.InRange( relation.iLastModifiedDate.Int64() ), 
				relationCondition.iNegated ) )
			{
			return EFalse;
			}
		}

	return ETrue;
	}

TBool CMdSNotifyComparator::MatchObjectIdsL(
						    CMdCSerializationBuffer& aSerializedCondition,
						    const RArray<TItemId>& aItemIdArray,
						    RArray<TItemId>& aMatchingItemIdArray
						    )
	{
	// check if condition buffer contains condition
	if( aSerializedCondition.Size() > 0 )
		{
		aSerializedCondition.PositionL( KNoOffset );

		const TMdCLogicCondition& logicCondition = 
				TMdCLogicCondition::GetFromBufferL( aSerializedCondition );

		for( TUint32 i = 0; i < logicCondition.iChildConditions.iPtr.iCount; i++ )
		    {
	       	aSerializedCondition.PositionL( 
	       			logicCondition.iChildConditions.iPtr.iOffset + 
	       			i * CMdCSerializationBuffer::KRequiredSizeForTUint32 );

	 		TUint32 childOffset;
		    aSerializedCondition.ReceiveL( childOffset );
		    aSerializedCondition.PositionL( childOffset );

		    const TMdCCondition& condition = TMdCCondition::GetFromBufferL( 
					aSerializedCondition );
			
			if( EConditionTypeObject == condition.iConditionType )
				{
				const TMdCObjectCondition& objectCondition = 
						TMdCObjectCondition::GetFromBufferL( aSerializedCondition );

			    if ( EObjectConditionCompareNone == objectCondition.iCompareMethod )
			    	{
                    // all matches
                    return ETrue;
			    	}
			    
	    		// only ID conditions are checked
	    		if( EObjectConditionCompareId == objectCondition.iCompareMethod )
	    			{
	    			aSerializedCondition.PositionL( objectCondition.iCondition );
	    			
	    			// get object condition's object ID
	    			TItemId conditionObjectId;
	    			aSerializedCondition.ReceiveL( conditionObjectId );

	    			const TInt objectIdCount = aItemIdArray.Count();
	    			for( TInt i = 0; i < objectIdCount; i++ )
	    				{
	    				const TItemId objectId = aItemIdArray[i];
	
	    				if( ( objectId == conditionObjectId ) != objectCondition.iNegated )
	    					{
	    					aMatchingItemIdArray.AppendL( objectId );
	    					}
	    				}
	
	    			if( aMatchingItemIdArray.Count() < aItemIdArray.Count() )
	    				{
	    				// none or some matches
	    				return EFalse;
	    				}	
	    			else
	    				{
	    				// all matches
	    				return ETrue;
	    				}
	    			}
	    		else
	    			{
                    // if condition is something else than ID, return true so that client receives
                    // needed notification even though also extra notifications will  be received
                    return ETrue;
	    			}
				}
    		}
		}
   return ETrue;
   }

TBool CMdSNotifyComparator::FindDes( TDesC16& aDes, TDesC16& aFindDes )
	{
	if( aDes.Find( aFindDes ) != KErrNotFound  )
	    {
	    return ETrue;
	    }
	return EFalse;
	}

TBool CMdSNotifyComparator::CompareDesEndsWith( TDesC16& aDes, TDesC16& aCompareDes )
	{
	if( aDes.Length() < aCompareDes.Length() )
		{
		return EFalse;
		}

	return ( aDes.Right( aCompareDes.Length() ).Compare( aCompareDes ) == 0 );
	}

TBool CMdSNotifyComparator::CompareDesBeginsWith( TDesC16& aDes, TDesC16& aCompareDes )
	{
	if( aDes.Length() < aCompareDes.Length() )
		{
		return EFalse;
		}

	return ( aDes.Left( aCompareDes.Length() ).Compare( aCompareDes ) == 0 );
	}

TBool CMdSNotifyComparator::MatchObjectIdToObjectIdConditionsL(
	CMdCSerializationBuffer& aSerializedCondition, TItemId aObjectId)
	{
	const TMdCLogicCondition& logicCondition = 
			TMdCLogicCondition::GetFromBufferL( aSerializedCondition );
	
	TBool matchingId = EFalse;

	for( TUint32 i = 0; i < logicCondition.iChildConditions.iPtr.iCount; i++ )
		{
		aSerializedCondition.PositionL( 
				logicCondition.iChildConditions.iPtr.iOffset + 
				i * CMdCSerializationBuffer::KRequiredSizeForTUint32 );

		TUint32 childOffset;
		aSerializedCondition.ReceiveL( childOffset );
		aSerializedCondition.PositionL( childOffset );

		const TMdCCondition& condition = TMdCCondition::GetFromBufferL( 
				aSerializedCondition );
		
		if( EConditionTypeObject == condition.iConditionType )
			{
			const TMdCObjectCondition& objectCondition = 
					TMdCObjectCondition::GetFromBufferL( aSerializedCondition );
			
			if( EObjectConditionCompareId == objectCondition.iCompareMethod )
				{
				aSerializedCondition.PositionL( objectCondition.iCondition );

				TItemId conditionObjectId;
				aSerializedCondition.ReceiveL( conditionObjectId );
				
				TBool currentMatch = ( conditionObjectId == aObjectId ) != objectCondition.iNegated;
	
				if( currentMatch )
					{
					matchingId = ETrue;
					break;
					}
				}
			}
		}
		
	return matchingId != logicCondition.iNegated;
	}

TBool CMdSNotifyComparator::MatchRelationIdsL(
		const TMdCRelationCondition& aRelationCondition, 
		CMdCSerializationBuffer& aSerializedCondition, TItemId aRelationId)
	{
	if( aRelationCondition.iRelationIds.iPtr.iCount > 0 &&
			aRelationCondition.iRelationIds.iPtr.iOffset != KNoOffset )
		{
		aSerializedCondition.PositionL( aRelationCondition.iRelationIds.iPtr.iOffset );

		for (TUint i = 0; i < aRelationCondition.iRelationIds.iPtr.iCount; ++i)
			{
			TItemId relationId;
			aSerializedCondition.ReceiveL( relationId );
			if ( relationId == aRelationId )
				{
				return ETrue;
				}
			}
		}

	return EFalse;
	}

TBool CMdSNotifyComparator::MatchRelationItemsL(
		    CMdCSerializationBuffer& aSerializedCondition,
		    CMdCSerializationBuffer& aSerializedItems,
		    RArray<TItemId>& aMatchingItemIdArray)
    {
    aSerializedItems.PositionL( KNoOffset );
    
    const TMdCItems& items = TMdCItems::GetFromBufferL( aSerializedItems );

    if( items.iRelations.iPtr.iCount == 0 )
    	{
    	return EFalse;
    	}

	for( TUint32 i = 0; i < items.iRelations.iPtr.iCount; i++ )
		{
		aSerializedItems.PositionL( items.iRelations.iPtr.iOffset + 
				+ i * sizeof(TMdCRelation) );
	    const TMdCRelation& relation = TMdCRelation::GetFromBufferL( aSerializedItems );

		if( aSerializedCondition.Size() > 0 )
			{
    		// move condition buffer's position to the begin of the buffer
    		aSerializedCondition.PositionL( KNoOffset );

    		if( MatchRelationItemL( aSerializedCondition, relation ) )
    			{
    			aMatchingItemIdArray.AppendL( relation.iId );
    			}
			}
		else
			{
			aMatchingItemIdArray.AppendL( relation.iId );
			}
		}

	if( aMatchingItemIdArray.Count() > 0 )
    	{
    	return ETrue;
    	}
    else
    	{
    	return EFalse;
    	}
    }

TBool CMdSNotifyComparator::MatchRelationItemL(CMdCSerializationBuffer& aSerializedCondition,
		   const TMdCRelation& aRelation)
	{
	// check if event is failed
	if( aRelation.iId == 0 )
		{
		return EFalse;
		}
	
	const TMdCRelationCondition& relationCondition = 
			TMdCRelationCondition::GetFromBufferL( aSerializedCondition );

	// check if relation condition contains relation def ID comparsion
	if( relationCondition.iRelationDefId != KNoDefId &&
		BoolEqual( relationCondition.iRelationDefId == aRelation.iDefId, 
				relationCondition.iNegated ) )
		{
		return EFalse;
		}
	
	// check if relation condition contains relation ID comparsion
	if ( relationCondition.iRelationId != KNoId &&
	 	 BoolEqual( relationCondition.iRelationId == aRelation.iId, 
	 			relationCondition.iNegated ) )
		{
		return EFalse;
		}

	// check if relation condition contains relation IDs comparsion
	if ( relationCondition.iRelationIds.iPtr.iOffset != KNoOffset &&
			BoolEqual( MatchRelationIdsL( relationCondition, 
					aSerializedCondition, aRelation.iId ), 
					relationCondition.iNegated ) )
		{
		return EFalse;
		}

	// check relation condition contains left and/or right object condition comparisions
	if ( ( relationCondition.iLeftObjectCondition != KNoOffset || 
			relationCondition.iRightObjectCondition != KNoOffset ) && 
			ERelationConditionSideEither == relationCondition.iObjectSide )
		{
		TBool eitherMatches = EFalse;
		
		if ( relationCondition.iLeftObjectCondition != KNoOffset )
			{
			aSerializedCondition.PositionL( 
					relationCondition.iLeftObjectCondition );
	
			if( !BoolEqual( MatchObjectIdToObjectIdConditionsL(
					aSerializedCondition, aRelation.iLeftObjectId ), 
					relationCondition.iNegated ) )
				{
				eitherMatches = ETrue;
				}
			}
		
		if ( relationCondition.iRightObjectCondition != KNoOffset )
			{
			aSerializedCondition.PositionL( 
					relationCondition.iRightObjectCondition );

			if( !BoolEqual( MatchObjectIdToObjectIdConditionsL(
					aSerializedCondition, aRelation.iRightObjectId ), 
					relationCondition.iNegated ) )
				{
				eitherMatches = ETrue;
				}
			}

		// left and right condition didn't match
		if( !eitherMatches )
			{
			return EFalse;
			}
		}

	return ETrue;
	}
