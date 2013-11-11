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

#include "mdeobjectcondition.h"
#include "mdcserializationbuffer.h"
#include "mdeobjectdef.h"
#include "mdepanic.h"
#include "mdscommoninternal.h"
#include "mdcquery.h"


CMdEObjectCondition* CMdEObjectCondition::NewL( TObjectConditionCompareMethod aCompareMethod,
		TItemId aObjectId, const CMdEObjectDef* aObjectDef,
		TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow )
	{
	CMdEObjectCondition* self = CMdEObjectCondition::NewLC( aCompareMethod,
			aObjectId, aObjectDef, aObjectGuidHigh, aObjectGuidLow );
	CleanupStack::Pop( self );
	return self;	
	}

CMdEObjectCondition* CMdEObjectCondition::NewLC( TObjectConditionCompareMethod aCompareMethod,
		TItemId aObjectId, const CMdEObjectDef* aObjectDef,
		TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow )
	{
	CMdEObjectCondition* self = new (ELeave) CMdEObjectCondition( aCompareMethod,
			aObjectId, aObjectDef, aObjectGuidHigh, aObjectGuidLow );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

CMdEObjectCondition* CMdEObjectCondition::NewL( TObjectConditionCompareMethod aCompareMethod,
		const RArray<TItemId>* aObjectIds, const TDesC* aString, const TMdEUintRange* aRange )
	{
	CMdEObjectCondition* self = CMdEObjectCondition::NewLC( aCompareMethod, aObjectIds, aString, aRange );
	CleanupStack::Pop( self );
	return self;	
	}

CMdEObjectCondition* CMdEObjectCondition::NewLC( TObjectConditionCompareMethod aCompareMethod,
		const RArray<TItemId>* aObjectIds, const TDesC* aString, const TMdEUintRange* aRange )
	{
	CMdEObjectCondition* self = new (ELeave) CMdEObjectCondition( aCompareMethod );
	CleanupStack::PushL( self );
	self->ConstructL( aObjectIds, aString, aRange );
	return self;
	}


CMdEObjectCondition::CMdEObjectCondition( TObjectConditionCompareMethod aCompareMethod,
									TItemId aObjectId, const CMdEObjectDef* aObjectDef,
									TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow )
	: CMdECondition(EConditionTypeObject), iObjectDef( aObjectDef ), 
	  iObjectId( aObjectId ), iObjectIds( NULL ), 
	  iString( NULL ), iCompareMethod( aCompareMethod ), iFlags( 0 ),
	  iConfidentialityLevel(EObjectConditionLevelNormal),
	  iGuidHigh( aObjectGuidHigh ), iGuidLow( aObjectGuidLow ),
	  iRange( NULL )
	{
	#ifdef _DEBUG
	if ( iObjectDef )
		{
		__ASSERT_DEBUG(aCompareMethod == EObjectConditionCompareObjectDef, TMdEPanic::Panic(TMdEPanic::EInternal));
		}
	else if ( iObjectId != KNoId )
		{
		__ASSERT_DEBUG(aCompareMethod == EObjectConditionCompareId, TMdEPanic::Panic(TMdEPanic::EInternal));
		}
	else if ( iGuidHigh != 0 || iGuidLow != 0 )
		{
		__ASSERT_DEBUG(aCompareMethod == EObjectConditionCompareGuid, TMdEPanic::Panic(TMdEPanic::EInternal));
		}
	else
		{
		__ASSERT_DEBUG(aCompareMethod == EObjectConditionCompareNone, TMdEPanic::Panic(TMdEPanic::EInternal));
		}
	
	#endif
	}

CMdEObjectCondition::CMdEObjectCondition( TObjectConditionCompareMethod aCompareMethod )
	: CMdECondition(EConditionTypeObject), iObjectDef( NULL ), 
	  iObjectId( KNoId ), iObjectIds( NULL ), 
	  iString( NULL ), iCompareMethod( aCompareMethod ), iFlags( 0 ),
	  iConfidentialityLevel(EObjectConditionLevelNormal), iGuidHigh( 0 ), iGuidLow( 0 )
	{
	__ASSERT_DEBUG(iCompareMethod > EObjectConditionCompareFirst && iCompareMethod < EObjectConditionCompareLast, TMdEPanic::Panic(TMdEPanic::EInternal));
	}

void CMdEObjectCondition::ConstructL()
    {
    // Base class construction.
    ConditionConstruct();
    
    // Non-confidential objects are queried by default
    iConfidentialityLevel = EObjectConditionLevelNormal;
    }

void CMdEObjectCondition::ConstructL( const RArray<TItemId>* aObjectIds,
		const TDesC* aString, const TMdEUintRange* aRange )
	{
	if ( aString )
		{
		iString = HBufC::NewL(aString->Length());
		iString->Des().CopyLC( *aString );
		}
	else if ( aObjectIds )
		{
	    iObjectIds = new (ELeave) RArray<TItemId>;
		
	    const TInt objectIdCount = aObjectIds->Count();
	    iObjectIds->ReserveL( objectIdCount );
	    
		for( TInt i = 0; i < objectIdCount; i++ )
			{
			iObjectIds->AppendL( (*aObjectIds)[i] );
			}
		}
	else if ( aRange )
		{
		iRange = new(ELeave) TMdEUintRange(*aRange);
		}
	else
		{
		User::Leave( KErrArgument );
		}
	}

CMdEObjectCondition::~CMdEObjectCondition()
	{
   	delete iString;
   	iString = NULL;

   	if( iObjectIds )
    	{
    	iObjectIds->Close();
    	
    	delete iObjectIds;
    	iObjectIds = NULL;
    	}
    
   	delete iRange;
   	iRange = NULL;
	}

EXPORT_C const CMdEObjectDef* CMdEObjectCondition::ObjectDef() const
    {
    return iObjectDef;
    }

EXPORT_C TItemId CMdEObjectCondition::ObjectId() const
    {
    return iObjectId;
    }

EXPORT_C const RArray<TItemId>* CMdEObjectCondition::ObjectIds() const
	{
	return iObjectIds;
	}

EXPORT_C const TDesC* CMdEObjectCondition::String() const
    {
    return iString;
    }


EXPORT_C TObjectConditionCompareMethod CMdEObjectCondition::CompareMethod() const
    {
    return iCompareMethod;
    }


EXPORT_C TObjectConditionConfidentialityLevel CMdEObjectCondition::ConfidentialityLevel() const
    {
    return iConfidentialityLevel;
    }

EXPORT_C void CMdEObjectCondition::SetConfidentialityLevel( TObjectConditionConfidentialityLevel aLevel )
    {
    iConfidentialityLevel = aLevel;
    }

EXPORT_C TBool CMdEObjectCondition::NotPresent() const
	{
	return iFlags & EMdEObjectFlagNotPresent ? ETrue : EFalse;
	}

EXPORT_C void CMdEObjectCondition::SetNotPresent(TBool aNotPresent)
	{
	if( aNotPresent )
		{
		iFlags |= EMdEObjectFlagNotPresent; 
		}
	else
		{
		iFlags &= ~EMdEObjectFlagNotPresent;
		}
	}

EXPORT_C TBool CMdEObjectCondition::NotPlaceholder() const
	{
	return iFlags & EMdEObjectFlagPlaceholder ? ETrue : EFalse;
	}

EXPORT_C void CMdEObjectCondition::SetNotPlaceholder(TBool aNotPlaceholder)
	{
	if( aNotPlaceholder )
		{
		iFlags |= EMdEObjectFlagPlaceholder; 
		}
	else
		{
		iFlags &= ~EMdEObjectFlagPlaceholder;
		}
	}

EXPORT_C TBool CMdEObjectCondition::PlaceholderOnly() const
    {
    return iPlaceholdersOnly;
    }

EXPORT_C void CMdEObjectCondition::SetPlaceholderOnly(TBool aPlaceholderOnly )
    {
    iPlaceholdersOnly = aPlaceholderOnly;
    }

TUint32 CMdEObjectCondition::InternalQueryOptimizationFlags(TUint32& aFlags)
	{
	TUint32 optimizationFlags = EContainsObjectCondition;

	if( iFlags & EMdEObjectFlagNotPresent )
		{
		optimizationFlags |= EContainsNotPresentCondition;
		}

	switch( iCompareMethod )
		{
        case EObjectConditionCompareFreeText:
        case EObjectConditionCompareFreeTextContains:
        case EObjectConditionCompareFreeTextBeginsWith:
        case EObjectConditionCompareFreeTextEndsWith:
			optimizationFlags |= EContainsFreetextCondition;
			break;
		default:
			break;
		}
	
	if( iPlaceholdersOnly )
	    {
	    optimizationFlags |= EContainsPlaceholdersOnly;
	    }

	aFlags |= optimizationFlags;
	return optimizationFlags;
	}

TUint32 CMdEObjectCondition::RequiredBufferSize() const
	{
	TUint32 bufferSize = sizeof( TMdCObjectCondition );

	switch( iCompareMethod )
		{
		case EObjectConditionCompareNone:
			break;

		case EObjectConditionCompareId:
			{
			// Required size for object ID
			bufferSize += CMdCSerializationBuffer::KRequiredSizeForTItemId;
			}
			break;

		case EObjectConditionCompareIds:
			{
			// Required size for object ID
			bufferSize += CMdCSerializationBuffer::KRequiredSizeForTUint32; // count
			if( iObjectIds )
				{
				bufferSize += CMdCSerializationBuffer::KRequiredSizeForTItemId * iObjectIds->Count();
				}
			}
			break;

	    case EObjectConditionCompareGuid:
	    	{
			// Required size for object GUID high (TInt64), object GUID low (TInt64)
			bufferSize += 2 * CMdCSerializationBuffer::KRequiredSizeForTInt64;
	    	}
			break;

	    case EObjectConditionCompareObjectDef:
	    	{
			// Required size for object def ID (TUint32)
			bufferSize += CMdCSerializationBuffer::KRequiredSizeForTDefId;
	    	}
			break;

	    case EObjectConditionCompareUri:
	    case EObjectConditionCompareUriBeginsWith:
	    case EObjectConditionCompareUriContains:
	    case EObjectConditionCompareFreeText:
	    case EObjectConditionCompareFreeTextContains:
	    case EObjectConditionCompareFreeTextBeginsWith:
	    case EObjectConditionCompareFreeTextEndsWith:
	    	{
			// Required size for string
			bufferSize += CMdCSerializationBuffer::RequiredSize( *iString );
	    	}
			break;

	    case EObjectConditionCompareUsageCount:
	    	{
	    	__ASSERT_DEBUG( iRange, User::Panic( _L("Incorrect range condition"), KErrArgument ) );
	    	bufferSize += iRange->RequiredBufferSize();
	    	}
	    	break;

	    default:
			TMdEPanic::Panic( TMdEPanic::EInternal );
			break;
		}

	return bufferSize;
	}

void CMdEObjectCondition::SerializeL(CMdCSerializationBuffer& aBuffer, 
		TMdCOffset& aFreespaceOffset) const
	{
	const TUint32 conditionOffset = aBuffer.Position();
	const TUint32 subConditionOffset = 
			conditionOffset + sizeof( TMdCObjectCondition );

	TMdCObjectCondition condition; 

	condition.iConditionType = iType;
	condition.iNegated = iNegated;
	condition.iConfidentialityLevel = iConfidentialityLevel;
	condition.iCompareMethod = iCompareMethod;
	condition.iFlags = iFlags;
	condition.iCondition = subConditionOffset;

	aBuffer.PositionL( subConditionOffset );

	switch( iCompareMethod )
		{
		case EObjectConditionCompareNone:
			{
			// nothing to compare, so "clear" condition offset
			condition.iCondition = KNoOffset;
			}
			break;
	
		case EObjectConditionCompareId:
			{
			aBuffer.InsertL( iObjectId );
			}
			break;
	
		case EObjectConditionCompareIds:
			{
			if( iObjectIds )
				{
				const TInt count = iObjectIds->Count();
				
				aBuffer.InsertL( (TInt32)count );
				
				for( TInt i = 0; i < count; i++ )
					{
					aBuffer.InsertL( (*iObjectIds)[i] );
					}
				}
			else
				{
				aBuffer.InsertL( TInt32( 0 ) );
				}
			}
			break;
	
	    case EObjectConditionCompareGuid:
			{
			aBuffer.InsertL( iGuidHigh );
			aBuffer.InsertL( iGuidLow );
			}
			break;

	    case EObjectConditionCompareObjectDef:
			{
			aBuffer.InsertL( iObjectDef->Id() );
			}
			break;

	    case EObjectConditionCompareUsageCount:
			{
			__ASSERT_DEBUG( iRange, User::Panic( 
					_L("Incorrect range condition"), KErrArgument ) );
			iRange->SerializeL( aBuffer );
			}
			break;

	    case EObjectConditionCompareUri:
	    case EObjectConditionCompareUriBeginsWith:
	    case EObjectConditionCompareUriContains:
	    case EObjectConditionCompareFreeText:
	    case EObjectConditionCompareFreeTextContains:
	    case EObjectConditionCompareFreeTextBeginsWith:
	    case EObjectConditionCompareFreeTextEndsWith:
			{
			aBuffer.InsertL( *iString );
			}
			break;

		default:
			{
#ifdef _DEBUG
			TMdEPanic::Panic(TMdEPanic::EInternal);
#endif
			}
			break;
		}

	// get position after sub condition
	aFreespaceOffset = aBuffer.Position();

	// serialize condition
	aBuffer.PositionL( conditionOffset );
	condition.SerializeL( aBuffer );
	
	// and move back to after sub condition
	aBuffer.PositionL( aFreespaceOffset );
	}
