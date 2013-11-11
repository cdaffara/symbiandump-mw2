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


#include <mdeobject.h>

#include <mdeproperty.h>
#include <mdeobjectdef.h>
#include <mdenamespacedef.h>
#include <mdesession.h>
#include "mdcitem.h"
#include "mdscommoninternal.h"
#include "mdcserializationbuffer.h"

//////////////////////////////
//                          //
//        TMdEObject        //
//                          //
//////////////////////////////

EXPORT_C TMdEObject::TMdEObject() : 
	iDef( NULL )
	{
	}

EXPORT_C TItemId TMdEObject::Id() const
	{
	return iId;
	}

EXPORT_C const CMdEObjectDef& TMdEObject::DefL() const
	{
	if( !iDef )
		{
		User::Leave( KErrNotFound );
		}

	return *iDef;
	}

EXPORT_C TBool TMdEObject::Confidential() const
	{
    return iFlags & EMdEObjectFlagConfidential ? ETrue : EFalse;
	}

EXPORT_C TBool TMdEObject::Placeholder() const
	{
	return iFlags & EMdEObjectFlagPlaceholder ? ETrue : EFalse;
	}

EXPORT_C TBool TMdEObject::Removed() const
	{
	return iFlags & EMdEObjectFlagRemoved ? ETrue : EFalse;
	}

EXPORT_C TBool TMdEObject::NotPresent() const
	{
	return iFlags & EMdEObjectFlagNotPresent ? ETrue : EFalse;
	}

TUint32 TMdEObject::RequiredBufferSize()
	{
	return sizeof(TMdCObject);
	}

void TMdEObject::DeSerializeL(CMdCSerializationBuffer& aBuffer, 
		CMdENamespaceDef& aNamespaceDef)
	{
	const TMdCObject& object = TMdCObject::GetFromBufferL(aBuffer);

	// ID
	if ( object.iId == KNoId )
		{
		User::Leave( KErrNotFound );
		}
	iId = object.iId;

	// definition
	if ( object.iDefId == KNoDefId )
		{
		User::Leave( KErrNotFound );
		}
	iDef = aNamespaceDef.GetObjectDefL( object.iDefId );

	// object flags	
	iFlags = object.iFlags;
	}

//////////////////////////////
//                          //
//        CMdEObject        //
//                          //
//////////////////////////////

CMdEObject* CMdEObject::NewL( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId )
	{
	CMdEObject* self = CMdEObject::NewLC( aDef, aUri, aMediaId );
    CleanupStack::Pop( self );
    return self;
	}

CMdEObject* CMdEObject::NewLC( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId )
	{
	CMdEObject* self = new ( ELeave ) CMdEObject( NULL, KNoId, aDef );
    CleanupStack::PushL( self );
    self->ConstructL( aUri, aMediaId );
    return self;
	}

CMdEObject::CMdEObject(CMdESession* aSession, TItemId aId, CMdEObjectDef& aDef)
		: CMdEInstanceItem( aSession, aId ), iDef( &aDef ), iUri( NULL )
	{
	}

void CMdEObject::ConstructL( const TDesC& aUri, TUint32 aMediaId )
	{
	InstanceItemBaseConstruct();

	DoSetUriL( aUri );
	
	iMediaId = aMediaId;

    iFlags = EMdEObjectFlagModOpen;
   	}

CMdEObject::~CMdEObject()
	{
	if ( OpenForModifications() && BelongsToSession() )
		{
		TRAP_IGNORE( Session().CancelObjectL( *this ) );
		}

	delete iUri;
	iUri = NULL;

	iPropertyArray.ResetAndDestroy();
	iPropertyArray.Close();

	iFreeTextArray.ResetAndDestroy();
	iFreeTextArray.Close();
	}

EXPORT_C CMdEObjectDef& CMdEObject::Def() const
	{
	__ASSERT_DEBUG( iDef, User::Panic( _L("Object def is NULL!"), KErrBadHandle ) );
	
	return *iDef;
	}

EXPORT_C void CMdEObject::SetDefL(CMdEObjectDef& aDef)
	{
	__ASSERT_DEBUG( iDef, User::Panic( _L("Object def is NULL!"), KErrBadHandle ) );

	if( BelongsToSession() || ( iDef->Id() != KBaseObjectDefId ) )
		{
		User::Leave( KErrNotSupported );
		}

	iDef = &aDef;
	}

EXPORT_C const TDesC& CMdEObject::Uri() const
	{
	return *iUri;
	}

EXPORT_C void CMdEObject::SetUriL(const TDesC& aUri)
	{
	DoSetUriL(aUri);
	}

EXPORT_C TUint32 CMdEObject::MediaId() const
	{
	return iMediaId;
	}

EXPORT_C void CMdEObject::SetMediaId(TUint32 aMediaId)
	{
	iFlags |= EMdEObjectFlagModObject;
	iMediaId = aMediaId;
	}

EXPORT_C TUint32 CMdEObject::UsageCount() const
	{
	return iUsageCount;
	}

EXPORT_C void CMdEObject::Guid( TInt64& aGuidHigh, TInt64& aGuidLow ) const
	{
	aGuidHigh = iGuidHigh;
	aGuidLow  = iGuidLow;
	}
	
EXPORT_C void CMdEObject::SetGuid( const TInt64& aGuidHigh, const TInt64& aGuidLow )
	{
	iFlags |= EMdEObjectFlagModObject;
	iGuidHigh = aGuidHigh;
	iGuidLow  = aGuidLow;
	}


EXPORT_C TBool CMdEObject::Confidential() const
	{
    return iFlags & EMdEObjectFlagConfidential ? ETrue : EFalse;
	}

EXPORT_C void CMdEObject::SetConfidential( TBool aValue )
	{
	//check if no change to value
	if (Confidential() && aValue || !Confidential() && !aValue) // Confidential() == aValue
		{
		return;
		}

	if ( aValue )
		{
	    iFlags |= EMdEObjectFlagConfidential;
		}
	else
		{
	    iFlags &= ~EMdEObjectFlagConfidential;
		}
	iFlags |= EMdEObjectFlagModObject;
	}

EXPORT_C TBool CMdEObject::Placeholder() const
	{
	return iFlags & EMdEObjectFlagPlaceholder ? ETrue : EFalse;
	}

EXPORT_C void CMdEObject::SetPlaceholder( TBool aValue )
	{
	//check if no change to value
	if ( Placeholder() && aValue || !Placeholder() && !aValue) // Placeholder() == aValue
		{
		return;
		}

	if ( aValue )
		{
		iFlags |= EMdEObjectFlagPlaceholder;
		}
	else
		{
		iFlags &= ~EMdEObjectFlagPlaceholder;
		}
	iFlags |= EMdEObjectFlagModObject;
	}

void CMdEObject::DoSetUriL(const TDesC& aUri)
	{
	if ( iUri )
		{
		if ( *iUri == aUri )
			{
			return;
			}
		else
			{
			delete iUri;
			iUri = NULL;
			}
		}

	iUri = HBufC::NewL( aUri.Length() );
	// Note: CopyLC doesn't push anything to cleanup stack
	iUri->Des().CopyLC( aUri );
	iFlags |= EMdEObjectFlagModObject;
	}

EXPORT_C TInt CMdEObject::PropertyCount() const
	{
	const TInt propertyCount = iPropertyArray.Count();
	if (!(iFlags & EMdEObjectFlagModObject))
		{
		return propertyCount;
		}

	TInt c = 0;
	for (TInt i = 0; i < propertyCount; ++i)
		{
		if ( ! iPropertyArray[i]->Removed() )
			{
			++c;
			}
		}
	return c;
	}

EXPORT_C CMdEProperty& CMdEObject::PropertyL(TInt aIndex) const
	{
	if ( aIndex < 0 || aIndex > PropertyCount() )
		{
		User::Leave( KErrArgument );
		}
	if (!(iFlags & EMdEObjectFlagModObject))
		{
		return *iPropertyArray[aIndex];
		}
	
	TInt j = 0;
	const TInt count = iPropertyArray.Count();
	
	for (TInt i = 0; i < count; ++i)
		{
		if ( ! iPropertyArray[i]->Removed() )
			{
			if (j == aIndex)
				{
				return *iPropertyArray[j];
				}
			++j;
			}
		}
	User::Leave( KErrNotFound );
	return *iPropertyArray[0]; // <-- just to stop compiler warnings!!
	}

EXPORT_C TInt CMdEObject::PropertyCount(const CMdEPropertyDef& aDef) const
	{
	TInt propCount = 0;
	
	const TInt count = iPropertyArray.Count();
	
	for (TInt i = 0; i < count; i++)
		{
        if( ! iPropertyArray[i]->Removed() &&
            iPropertyArray[i]->Def().Compare( aDef ) == 0 )
        	{
        	++propCount;
        	}
		}
    return propCount;
	}

EXPORT_C TInt CMdEObject::Property(const CMdEPropertyDef& aDef, CMdEProperty*& aProperty, TInt aStartAt) const
    {
    const TInt count = iPropertyArray.Count();

    if( aStartAt >= 0 )
        {
        TInt low( aStartAt );
        TInt high( count );
    
        while( low < high )
            {
            TInt mid( (low+high)>>1 );
            
            TInt compare( aDef.Compare( iPropertyArray[mid]->Def() ) );
            if( compare == 0 )
                {
                if( !iPropertyArray[mid]->Removed() )
                    {
                    aProperty = iPropertyArray[mid];
                    return mid;
                    }
                }
            else if( compare > 0 )
                {
                low = mid + 1;
                }
            else
                {
                high = mid;
                }
            }
        }
    return KErrNotFound;
    }
    
EXPORT_C CMdEProperty& CMdEObject::AddBoolPropertyL(CMdEPropertyDef& aDef, TBool aValue)
	{
    if( aDef.PropertyType() != EPropertyBool )
    	{
    	User::Leave( KErrArgument );
    	}	
	
	CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
	property->SetBoolValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddInt8PropertyL(CMdEPropertyDef& aDef, TInt8 aValue)
	{
    if( aDef.PropertyType() != EPropertyInt8 )
    	{
    	User::Leave( KErrArgument );
    	}	
	
    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetInt8ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddUint8PropertyL(CMdEPropertyDef& aDef, TUint8 aValue)
	{
    if( aDef.PropertyType() != EPropertyUint8 )
    	{
    	User::Leave( KErrArgument );
    	}	
	
    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetUint8ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddInt16PropertyL(CMdEPropertyDef& aDef, TInt16 aValue)
	{
    if( aDef.PropertyType() != EPropertyInt16 )
    	{
    	User::Leave( KErrArgument );
    	}	
	
    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetInt16ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddUint16PropertyL(CMdEPropertyDef& aDef, TUint16 aValue)
	{
    if( aDef.PropertyType() != EPropertyUint16 )
    	{
    	User::Leave( KErrArgument );
    	}	
	
    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetUint16ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddInt32PropertyL(CMdEPropertyDef& aDef, TInt32 aValue)
	{
    if( aDef.PropertyType() != EPropertyInt32 )
    	{
    	User::Leave( KErrArgument );
    	}	

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetInt32ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddUint32PropertyL(CMdEPropertyDef& aDef, TUint32 aValue)
	{
    if( aDef.PropertyType() != EPropertyUint32 )
    	{
    	User::Leave( KErrArgument );
    	}	

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetUint32ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddInt64PropertyL(CMdEPropertyDef& aDef, TInt64 aValue)
	{
    if( aDef.PropertyType() != EPropertyInt64 )
	   	{
    	User::Leave( KErrArgument );
    	}

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetInt64ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddReal32PropertyL(CMdEPropertyDef& aDef, TReal32 aValue)
	{
    if( aDef.PropertyType() != EPropertyReal32 )
    	{
    	User::Leave( KErrArgument );
    	}

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetReal32ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddReal64PropertyL(CMdEPropertyDef& aDef, TReal64 aValue)
	{
    if( aDef.PropertyType() != EPropertyReal64 )
    	{
    	User::Leave( KErrArgument );
    	}

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetReal64ValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddTextPropertyL(CMdEPropertyDef& aDef, const TDesC& aValue)
	{
    if( aDef.PropertyType() != EPropertyText )
    	{
    	User::Leave( KErrArgument );
    	}
	
    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetTextValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

EXPORT_C CMdEProperty& CMdEObject::AddTimePropertyL(CMdEPropertyDef& aDef, TTime aValue)
	{
    if( aDef.PropertyType() != EPropertyTime )
    	{
    	User::Leave( KErrArgument );
    	}	

    CMdEProperty* property = CMdEProperty::NewLC(aDef, *this);
    property->SetTimeValueL( aValue );
	DoAddPropertyL(*property);
	CleanupStack::Pop(property);

	return *property;
	}

void CMdEObject::DoAddPropertyL(CMdEProperty& aProperty)
	{
	TInt err = iPropertyArray.InsertInOrder(&aProperty, 
								TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));
	if (err == KErrAlreadyExists)
		{
		const TInt f = iPropertyArray.FindInOrder(&aProperty,
								TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));

		// this must never happen
		__ASSERT_DEBUG( f >= KErrNone, User::Panic( _L("AddProperty find!"), KErrGeneral ) );

		if( ! iPropertyArray[f]->Removed() )
			{
			User::LeaveIfError( err );
			}

		CMdEProperty* oldProperty = iPropertyArray[f];
		iPropertyArray[f] = &aProperty;
		delete oldProperty;
		oldProperty = NULL;
		}
	else if (err < KErrNone)
		{
		User::Leave(err);
		}

	iFlags |= EMdEObjectFlagModProperty;
	}

EXPORT_C void CMdEObject::RemoveProperty(TInt aIndex)
	{
	const TInt propertyCount = iPropertyArray.Count();
	if ( aIndex < 0 || aIndex >= propertyCount )
		{
		return;
		}

	TInt j = 0;
	TBool found = EFalse;
	for (TInt i = 0; i < propertyCount; ++i)
		{
		if ( ! iPropertyArray[i]->Removed() )
			{
			if (j == aIndex)
				{
				found = ETrue;
				break;
				}
			++j;
			}
		}

	if ( !found )
		{
		return;
		}

	CMdEProperty* property = iPropertyArray[j];
	if( property->Def().ReadOnly() )
    	{
    	return;
    	}

	property->SetRemoved();

	iFlags |= EMdEObjectFlagModProperty;
	}

TInt CMdEObject::CompareProperties(const CMdEProperty& aFirst, const CMdEProperty& aSecond)
	{
	return aFirst.Def().Compare( aSecond.Def() );
	}

EXPORT_C TInt CMdEObject::FreeTextCount() const
	{
	return iFreeTextArray.Count();
	}

EXPORT_C TPtrC CMdEObject::FreeText(TInt aIndex) const
	{
	return *iFreeTextArray[aIndex];
	}

EXPORT_C TInt CMdEObject::FreeTextIndex(const TDesC& aFreeText) const
	{
	const TInt count = iFreeTextArray.Count();
	
	for (TInt i = 0; i < count; i++)
		{
		if (aFreeText.Compare(*iFreeTextArray[i]) == 0)
			{
			return i;
			}
		}

	return KErrNotFound;
	}

EXPORT_C void CMdEObject::AddFreeTextL(const TDesC& aFreeText)
	{
	HBufC* freeText = aFreeText.AllocLC();
	if (DoAddFreeTextL(*freeText) == KErrNone)
		{
		CleanupStack::Pop(freeText);
		}
	else
		{
		CleanupStack::PopAndDestroy(freeText);
		}
	}

TInt CMdEObject::DoAddFreeTextL(const HBufC& aFreeText)
	{
	TInt err = iFreeTextArray.InsertInOrder(&aFreeText, TLinearOrder<HBufC>(CMdEObject::CompareFreeTexts));

	if (err == KErrNone)
		{
		iFlags |= EMdEObjectFlagModFreeText;
		}
	return err;
	}

EXPORT_C void CMdEObject::RemoveFreeText(TInt aIndex)
	{
	delete iFreeTextArray[aIndex];
	iFreeTextArray[aIndex] = NULL;
	iFreeTextArray.Remove(aIndex);
	iFlags |= EMdEObjectFlagModFreeText;
	}

TInt CMdEObject::CompareFreeTexts(const HBufC& aFirst, const HBufC& aSecond)
	{
	return aFirst.Compare(aSecond);
	}

EXPORT_C void CMdEObject::MovePropertiesL(CMdEObject& aObject)
	{
	__ASSERT_DEBUG( iDef, User::Panic( _L("Object def is NULL!"), KErrBadHandle ) );
	
	// definitions must match
	if ( iDef->Compare( *aObject.iDef ) )
		{
		User::Leave( KErrArgument );
		}

	// object must be open
	if ( !OpenForModifications() )
		{
		User::Leave( KErrAccessDenied );
		}

	// and cannot contain any properties
	ClearObject( EFalse );
	if ( iPropertyArray.Count() )
		{
		User::Leave( KErrAlreadyExists );
		}

	const TInt arraySize = aObject.iPropertyArray.Count();
	iPropertyArray.ReserveL( arraySize );

	// add all properties to this object, 
	// but don't remove yet from other object
	for ( TInt i = 0; i < arraySize; ++i )
		{
        TInt err = iPropertyArray.InsertInOrder(aObject.iPropertyArray[i], 
                                TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));
    
        if (err == KErrAlreadyExists)
            {
            const TInt f = iPropertyArray.FindInOrder(aObject.iPropertyArray[i],
                                TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));

            if( !iPropertyArray[f]->Removed() )
                {
				continue;
                }

            CMdEProperty* oldProperty = iPropertyArray[f];
            iPropertyArray[f] = aObject.iPropertyArray[i];
            delete oldProperty;
            oldProperty = NULL;
            }
        else if (err < KErrNone)
            {
            // restore this objects to original state
            iPropertyArray.Reset();
            User::Leave(err);
            }	
		}

	// remove properties from other object
	aObject.iPropertyArray.Reset();

	// mark all moved properties as modified
	for ( TInt i = 0; i < arraySize; ++i )
		{
		CMdEProperty* property = iPropertyArray[i];
		property->SetModified();
		}

	iFlags |= EMdEObjectFlagModProperty;
	}


TBool CMdEObject::ObjectModified() const
    {
    return iFlags & EMdEObjectFlagModObject ? ETrue : EFalse;
    }

TBool CMdEObject::FreeTextModified() const
    {
    return iFlags & EMdEObjectFlagModFreeText ? ETrue : EFalse;
    }

TBool CMdEObject::PropertyModified() const
    {
    return iFlags & EMdEObjectFlagModProperty ? ETrue : EFalse;
    }

void CMdEObject::AutoLockL() const
	{
	if( BelongsToSession() )
		{
		User::Leave( KErrAlreadyExists );
		}
	
	iFlags |= EMdEObjectFlagAutoLock;
	}

EXPORT_C TBool CMdEObject::OpenForModifications() const
	{
	return iFlags & EMdEObjectFlagModOpen ? ETrue : EFalse;
	}

void CMdEObject::ClearObject( TBool aClearFlags )
	{
	// clear mod flags
	if (aClearFlags)
		{
		iFlags &= ~(EMdEObjectFlagModObject | EMdEObjectFlagModProperty | EMdEObjectFlagModFreeText | EMdEObjectFlagModOpen);

		// auto locked object's are marked back to open for modifications
		if ( iFlags & EMdEObjectFlagAutoLock )
			{
			iFlags |= EMdEObjectFlagModOpen;
			}
		}

	// clear mod flags in properties
	for (TInt i = iPropertyArray.Count() - 1; i >= 0 ; --i)
		{
    	CMdEProperty* prop = iPropertyArray[i];
        if ( prop->Removed() )
        	{
        	delete prop;
        	prop = NULL;
        	iPropertyArray.Remove( i );
        	continue;
        	}
        if (aClearFlags)
        	{
        	prop->ClearFlags();
        	}
		}
	}

void CMdEObject::SetNotOpenForModifications()
	{
	iFlags &= ~EMdEObjectFlagModOpen;
	}

TUint32 CMdEObject::ModifiedPropertiesCount() const
	{
	TUint32 modCount = 0;
	const TInt count = iPropertyArray.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if (iPropertyArray[i]->Modified())
			{
			++modCount;
			}
		}
	return modCount;
	}

TUint32 CMdEObject::RequiredBufferSize() const
	{
	TUint32 bufferSize = sizeof( TMdCObject );

	if (iUri)
		{
		bufferSize += CMdCSerializationBuffer::RequiredSize( *iUri );
		}

	TUint32 bufSave = bufferSize;
	// properties
	if ( iPropertyArray.Count() > 0 )
		{
		const TInt count = iPropertyArray.Count();
		for ( TInt i = 0; i < count; ++i )
			{
			if (iPropertyArray[i]->Modified())
				{
				bufferSize += iPropertyArray[i]->RequiredBufferSize();
				iFlags |= EMdEObjectFlagModProperty;
				}
			}
		}
	
	if (!(iFlags & EMdEObjectFlagModProperty))
		{
		bufferSize = bufSave;
		}
	
	// freetext
	if ( iFlags & EMdEObjectFlagModFreeText && iFreeTextArray.Count() > 0 )
		{
		
		const TInt count = iFreeTextArray.Count();
		for ( TInt i = 0; i < count; ++i )
			{
			bufferSize += CMdCSerializationBuffer::RequiredSize( *iFreeTextArray[i] );
			}
		}

	return bufferSize;
	}

TMdCOffset CMdEObject::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const
	{
	__ASSERT_DEBUG( iDef, User::Panic( _L("Object def is NULL!"), KErrBadHandle ) );

	const TMdCOffset objectOffset = aBuffer.Position();
	TMdCObject object;
	
	// objectId
	object.iId = Id();
	// objectDefId
	object.iDefId = iDef->Id();
	// object flags
	object.iFlags = iFlags;
	// mediaId
	object.iMediaId = iMediaId;
	// usageCount
	object.iUsageCount = iUsageCount;
	// guid low
	object.iGuidHigh = iGuidHigh;
	// guid high
	object.iGuidLow = iGuidLow;
	// uri
	object.iUri.iPtr.iCount = iUri->Length();
	object.iUri.iPtr.iOffset = aFreespaceOffset;
	
	aBuffer.PositionL( aFreespaceOffset );
	aFreespaceOffset = aBuffer.InsertL( *iUri );

	// adding properties
	if ( iFlags & EMdEObjectFlagModProperty && iPropertyArray.Count() > 0 )
		{
		// first set up offset and count to properties
		object.iProperties.iPtr.iOffset = aFreespaceOffset;
		object.iProperties.iPtr.iCount = ModifiedPropertiesCount();

		// move after properties table
		aFreespaceOffset += object.iProperties.iPtr.iCount * sizeof( TMdCProperty );

		const TInt count = iPropertyArray.Count();
		TInt modifiedPropertyCounter = 0;
		// insert properties
		for ( TInt i = 0; i < count; ++i )
			{
			if (!iPropertyArray[i]->Modified())
				{
				continue;
				}
			aBuffer.PositionL( object.iProperties.iPtr.iOffset + modifiedPropertyCounter * sizeof( TMdCProperty ) );
			aFreespaceOffset = iPropertyArray[i]->SerializeL(aBuffer, aFreespaceOffset);
			++modifiedPropertyCounter;
			}
		}
	else
		{
		object.iProperties.iPtr.iOffset = KNoOffset;
		object.iProperties.iPtr.iCount = 0;
		}

	// adding freetext
	if ( iFlags & EMdEObjectFlagModFreeText && iFreeTextArray.Count() > 0 )
		{
		// first set up offset to freetext
		object.iFreeTexts.iPtr.iOffset = aFreespaceOffset;
		object.iFreeTexts.iPtr.iCount = iFreeTextArray.Count();

		const TInt count = iFreeTextArray.Count();
		
		// insert freetext
		aBuffer.PositionL( aFreespaceOffset );
		for ( TInt i = 0; i < count; ++i )
			{
			aFreespaceOffset = aBuffer.InsertL( *iFreeTextArray[i] );
			}
		}
	else
		{
		object.iFreeTexts.iPtr.iOffset = KNoOffset;
		object.iFreeTexts.iPtr.iCount = 0;
		}
	
	// store object informations
	aBuffer.PositionL( objectOffset );
	object.SerializeL( aBuffer );

	return aFreespaceOffset;
	}

CMdEObject* CMdEObject::NewL( CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef )
	{
	CMdEObject* ret = CMdEObject::NewLC( aSession, aBuffer, aNamespaceDef );
	CleanupStack::Pop( ret );
	return ret;
	}


CMdEObject* CMdEObject::NewLC( CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef )
	{
	const TMdCObject& serializedObject = TMdCObject::GetFromBufferL( aBuffer );
	
	// objectid
	if (serializedObject.iId == KNoId)
		{
		User::Leave( KErrNotFound );
		}

	// objectDefId
	if (serializedObject.iDefId == KNoDefId)
		{
		User::Leave( KErrNotFound );
		}
	CMdEObjectDef* newObjectDef = aNamespaceDef.GetObjectDefL( serializedObject.iDefId );
	if( !newObjectDef )
		{
		User::Leave( KErrNotFound );
		}

	CMdEObject* newObject = new (ELeave) CMdEObject( aSession, serializedObject.iId, *newObjectDef );
	CleanupStack::PushL( newObject );

	// set correct variables
	// object flags
	newObject->iFlags = serializedObject.iFlags;
	// mediaId
	newObject->iMediaId = serializedObject.iMediaId;
	// usage count
	newObject->iUsageCount = serializedObject.iUsageCount;
	// guid low
	newObject->iGuidHigh = serializedObject.iGuidHigh;
	// guid high
	newObject->iGuidLow = serializedObject.iGuidLow;
	// uri
	if ( serializedObject.iUri.iPtr.iCount == 0 )
		{
		User::Leave( KErrNotFound );
		}
	aBuffer.PositionL( serializedObject.iUri.iPtr.iOffset );
	newObject->iUri = aBuffer.ReceiveDes16L();

	// read properties array
	if ( serializedObject.iProperties.iPtr.iOffset != KNoOffset )
		{
		newObject->iPropertyArray.ReserveL( serializedObject.iProperties.iPtr.iCount );

		for ( TUint32 i = 0; i < serializedObject.iProperties.iPtr.iCount; ++i )
			{
			aBuffer.PositionL( serializedObject.iProperties.iPtr.iOffset
					+ i * sizeof(TMdCProperty) );

			CMdEProperty* property = CMdEProperty::NewLC( *newObject, aBuffer );
		    TInt err = newObject->iPropertyArray.InsertInOrder(property, 
		                                TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));
		    
		    if (err == KErrAlreadyExists)
		        {
		        const TInt f = newObject->iPropertyArray.FindInOrder(property,
		                                TLinearOrder<CMdEProperty>(CMdEObject::CompareProperties));

		        if( !newObject->iPropertyArray[f]->Removed() )
		            {
		            continue;
		            }

		        CMdEProperty* oldProperty = newObject->iPropertyArray[f];
		        newObject->iPropertyArray[f] = property;
		        delete oldProperty;
		        oldProperty = NULL;
		        }
		    else if (err < KErrNone)
		        {
		        User::Leave(err);
		        }
			CleanupStack::Pop( property );
			}
		}

	// read freetext array
	if ( serializedObject.iFreeTexts.iPtr.iOffset != KNoOffset )
		{
		aBuffer.PositionL( serializedObject.iFreeTexts.iPtr.iOffset );

		newObject->iFreeTextArray.ReserveL( serializedObject.iFreeTexts.iPtr.iCount );
		for ( TUint32 i = 0; i < serializedObject.iFreeTexts.iPtr.iCount; ++i )
			{
			HBufC16* hbuf = aBuffer.ReceiveDes16L();
			CleanupStack::PushL( hbuf );
			newObject->iFreeTextArray.AppendL( hbuf );
			CleanupStack::Pop( hbuf );
			}
		}

	return newObject;
	}

TMdEInstanceType CMdEObject::InstanceType() const
	{
	return EMdETypeObject;
	}

