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


#include "mdeproperty.h"

#include "mdcitem.h"
#include "mdscommoninternal.h"
#include "mdepropertydef.h"
#include "mdcserializationbuffer.h"
#include "mdeobject.h"
#include "mdeobjectdef.h"
#include "mdeerror.h"
#include "mdesession.h"

CMdEProperty::CMdEProperty(const CMdEPropertyDef& aDef, const CMdEObject& aObject)
		: CMdEInstanceItem( CONST_CAST( CMdESession*, &aDef.NamespaceDef().Session() ), KNoId), 
		iModFlags(EMdEPropertyModChange), iDef(aDef), iObject(aObject)
	{
	}

CMdEProperty* CMdEProperty::NewL(const CMdEPropertyDef& aDef, const CMdEObject& aObject)
    {
    CMdEProperty* self = NewLC(aDef, aObject);
    CleanupStack::Pop(self);
    return self;
    }


CMdEProperty* CMdEProperty::NewLC(const CMdEPropertyDef& aDef, const CMdEObject& aObject)
    {
    CMdEProperty* self = new (ELeave) CMdEProperty( aDef, aObject );
    CleanupStack::PushL(self);
    self->BaseConstructL();
    return self;
    }


CMdEProperty* CMdEProperty::NewL( const CMdEObject& aObject, CMdCSerializationBuffer& aBuffer )
	{
	CMdEProperty* ret = CMdEProperty::NewLC( aObject, aBuffer );
	CleanupStack::Pop( ret );
	return ret;
	}

CMdEProperty* CMdEProperty::NewLC( const CMdEObject& aObject, CMdCSerializationBuffer& aBuffer )
	{
	const TMdCProperty& serializedProperty = TMdCProperty::GetFromBufferL( aBuffer );
	CMdEPropertyDef* propDef = aObject.Def().GetPropertyDefL( serializedProperty.iPropertyDefId );
	CMdEProperty* property = new(ELeave) CMdEProperty( *propDef, aObject );
	CleanupStack::PushL( property );
	property->BaseConstructL();

	if ( propDef->PropertyType() == EPropertyText  && property )
		{
		__ASSERT_DEBUG( serializedProperty.iValue.iPtr.iCount > 0, User::Panic( _L("Empty text property!"), KErrCorrupt ));
		aBuffer.PositionL( serializedProperty.iValue.iPtr.iOffset );
		property->iValue.iText = aBuffer.ReceiveDes16L();
		}
	else if( property )
		{
		property->iValue = serializedProperty.iValue;
		}
	
	// reset flags
	if (property)
		{
		property->iModFlags = EMdEPropertyModNone;
		}

	return property;
	}

void CMdEProperty::BaseConstructL()
	{
	InstanceItemBaseConstruct();
	}

CMdEProperty::~CMdEProperty()
	{
	if (iDef.PropertyType() == EPropertyText)
		{
		delete iValue.iText;
		iValue.iText = NULL;
		}
	}

EXPORT_C const CMdEPropertyDef& CMdEProperty::Def() const
	{
	return iDef;
	}

EXPORT_C const CMdEObject& CMdEProperty::Object() const
	{
	return iObject;
	}

EXPORT_C TBool CMdEProperty::Modified() const
	{
	return iModFlags != EMdEPropertyModNone;
	}

TUint8 CMdEProperty::ModFlags() const
	{
	return iModFlags;
	}

void CMdEProperty::SetRemoved()
	{
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
	iModFlags |= EMdEPropertyModRemove;
	}

TBool CMdEProperty::Removed()
	{
	return iModFlags & EMdEPropertyModRemove;
	}

void CMdEProperty::ClearFlags()
	{
	// set ID to property -> CMdEItem::InDatabase returns ETrue
	SetId( ~KNoId );
	
	iModFlags = EMdEPropertyModNone;
	}

void CMdEProperty::SetModified()
	{
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
	iModFlags |= EMdEPropertyModChange;
	}

TMdEInstanceType CMdEProperty::InstanceType() const
	{
	return EMdETypeProperty;
	}

EXPORT_C TBool CMdEProperty::BoolValueL() const
	{
	if ( iDef.PropertyType() != EPropertyBool )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iInt32;
	}

EXPORT_C TInt8 CMdEProperty::Int8ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyInt8 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iInt32;
	}

EXPORT_C TUint8 CMdEProperty::Uint8ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyUint8 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iUint32;
	}

EXPORT_C TInt16 CMdEProperty::Int16ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyInt16 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iInt32;
	}

EXPORT_C TUint16 CMdEProperty::Uint16ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyUint16 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iUint32;
	}

EXPORT_C TInt32 CMdEProperty::Int32ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyInt32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iInt32;
	}

EXPORT_C TUint32 CMdEProperty::Uint32ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyUint32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iUint32;
	}

EXPORT_C TInt64 CMdEProperty::Int64ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyInt64 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iInt64;
	}

EXPORT_C TReal32 CMdEProperty::Real32ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyReal32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iReal;
	}

EXPORT_C TReal64 CMdEProperty::Real64ValueL() const
	{
	if ( iDef.PropertyType() != EPropertyReal64 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return iValue.iReal;
	}

EXPORT_C TTime CMdEProperty::TimeValueL() const
	{
	if ( iDef.PropertyType() != EPropertyTime )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return TTime(iValue.iInt64);
	}

EXPORT_C const TDesC& CMdEProperty::TextValueL() const
	{
	if ( iDef.PropertyType() != EPropertyText )
		{
		User::Leave( KErrMdEIncorrectType );
		}
	return *iValue.iText;
	}


EXPORT_C void CMdEProperty::SetBoolValueL(TBool aValue)
	{
	if ( iDef.PropertyType() != EPropertyBool )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iInt32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetInt8ValueL(TInt8 aValue)
	{
	if ( iDef.PropertyType() != EPropertyInt8 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iInt32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetUint8ValueL(TUint8 aValue)
	{
	if ( iDef.PropertyType() != EPropertyUint8 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iUint32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iUint32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}


EXPORT_C void CMdEProperty::SetInt16ValueL(TInt16 aValue)
	{
	if ( iDef.PropertyType() != EPropertyInt16 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iInt32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetUint16ValueL(TUint16 aValue)
	{
	if ( iDef.PropertyType() != EPropertyUint16 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iUint32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iUint32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetInt32ValueL(TInt32 aValue)
	{
	if ( iDef.PropertyType() != EPropertyInt32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iInt32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetUint32ValueL(TUint32 aValue)
	{
	if ( iDef.PropertyType() != EPropertyUint32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iUint32 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iUint32 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetInt64ValueL(TInt64 aValue)
	{
	if ( iDef.PropertyType() != EPropertyInt64 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt64 = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iInt64 = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetReal32ValueL(TReal32 aValue)
	{
	if ( iDef.PropertyType() != EPropertyReal32 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iReal = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iReal = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetReal64ValueL(TReal64 aValue)
	{
	if ( iDef.PropertyType() != EPropertyReal64 )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iReal = aValue;
    CheckValueMinMaxL( value );
#endif
	iValue.iReal = aValue;
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetTimeValueL(const TTime& aValue)
	{
	if ( iDef.PropertyType() != EPropertyTime )
		{
		User::Leave( KErrMdEIncorrectType );
		}
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iInt64 = aValue.Int64();
    CheckValueMinMaxL( value );
#endif
	iValue.iInt64 = aValue.Int64();
	
	// remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
	
    iModFlags = EMdEPropertyModChange;
	}

EXPORT_C void CMdEProperty::SetTextValueL(const TDesC& aValue)
    {
#ifdef _DEBUG
    TMdCValueUnion value;
    value.iText = (HBufC*)&aValue;
    CheckValueMinMaxL( value );
#endif

    DoSetTextValueL( aValue );
    
    // remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
    
    iModFlags = EMdEPropertyModChange;
    }

#ifdef _DEBUG
#include <e32debug.h>

void CMdEProperty::CheckValueMinMaxL( const TMdCValueUnion& aValue ) const
	{
	_LIT( KFunctionName, "CMdEProperty::CheckValueMinMaxL" );
    switch( iDef.PropertyType() )
	    {
	    case EPropertyBool:
	    	break;
	    case EPropertyInt8:
	    case EPropertyInt16:
	    case EPropertyInt32:
	    	if ( !(iDef.MinInt32ValueL() <= aValue.iInt32 && aValue.iInt32 <= iDef.MaxInt32ValueL()) )
	    		{
#ifdef _DEBUG
	    		RDebug::Print( _L("Incorrect property[%S] value: %d"), &iDef.Name(), aValue );
#endif
	    		User::Panic( KFunctionName, KErrArgument );
	    		}
	    	break;
	    case EPropertyUint8:
	    case EPropertyUint16:
	    case EPropertyUint32:
	    	if ( !(iDef.MinUint32ValueL() <= aValue.iUint32 && aValue.iUint32 <= iDef.MaxUint32ValueL()) )
	    		{
#ifdef _DEBUG
	    		RDebug::Print( _L("Incorrect property[%S] value: %u"), &iDef.Name(), aValue );
#endif
	    		User::Panic( KFunctionName, KErrArgument );
	    		}
    		break;
	    case EPropertyInt64:
	    	if ( !(iDef.MinInt64ValueL() <= aValue.iInt64 && aValue.iInt64 <= iDef.MaxInt64ValueL()) )
	    		{
#ifdef _DEBUG
	    		RDebug::Print( _L("Incorrect property[%S] value: %Ld"), &iDef.Name(), aValue );
#endif
	    		User::Panic( KFunctionName, KErrArgument );
	    		}
			break;
	    case EPropertyTime:
	    	if ( !(iDef.MinTimeValueL().Int64() <= aValue.iInt64 && aValue.iInt64 <= iDef.MaxTimeValueL().Int64()) )
	    		{
#ifdef _DEBUG
	    		RDebug::Print( _L("Incorrect property[%S] value: %Ld"), &iDef.Name(), aValue );
#endif
	    		User::Panic( KFunctionName, KErrArgument );
	    		}
			break;
	    case EPropertyReal32:
	    case EPropertyReal64:
	    	if ( !(iDef.MinRealValueL() <= aValue.iReal && aValue.iReal <= iDef.MaxRealValueL()) )
	    		{
#ifdef _DEBUG
	    		RDebug::Print( _L("Incorrect property[%S] value: %.2f"), &iDef.Name(), aValue );
#endif
	    		User::Panic( KFunctionName, KErrArgument );
	    		}
			break;
	    case EPropertyText:
			const TInt valueLength = aValue.iText->Length();
			if ( !(iDef.MinTextLengthL() <= valueLength && valueLength <= iDef.MaxTextLengthL()) )
				{
#ifdef _DEBUG
				RDebug::Print( _L("Incorrect property[%S] value: \"%S\", length: %d"), &iDef.Name(), aValue.iText, valueLength );
#endif
				User::Panic( KFunctionName, KErrArgument );
				}
			break;
	    }
	}
#endif

void CMdEProperty::DoSetTextValueL(const TDesC& aValue)
    {
    if ( iValue.iText )
    	{
    	if ( iValue.iText->Size() == aValue.Size() )
    		{
    		TPtr16 ptr(iValue.iText->Des());
    		ptr.Copy( aValue );
    		}
    	else
    		{
		    delete iValue.iText;
		    iValue.iText = NULL;
    		}
    	}

    if ( !iValue.iText )
    	{
    	iValue.iText = aValue.AllocL();
    	}
    
    // remove ID from property -> CMdEItem::InDatabase returns EFalse
	SetId( KNoId );
    
    iModFlags = EMdEPropertyModChange;
    }

TUint32 CMdEProperty::RequiredBufferSize() const
	{
	TUint32 size = sizeof( TMdCProperty );
	if ( iDef.PropertyType() == EPropertyText )
		{
		size += CMdCSerializationBuffer::RequiredSize( *iValue.iText );
		}
	return size;
	}

TMdCOffset CMdEProperty::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const
	{
	const TMdCOffset propertyOffset = aBuffer.Position();
	TMdCProperty property;
	property.iPropertyDefId = Def().Id();
	property.iModFlags = iModFlags;
	if ( iDef.PropertyType() == EPropertyText )
		{
		property.iValue.iPtr.iCount = iValue.iText->Length();
		property.iValue.iPtr.iOffset = aFreespaceOffset;
		aBuffer.PositionL( aFreespaceOffset );
		aFreespaceOffset = aBuffer.InsertL( *iValue.iText );
		}
	else
		{
		property.iValue = iValue;
		}
	
	// serialize property itself
	aBuffer.PositionL( propertyOffset );
	property.SerializeL( aBuffer );

	return aFreespaceOffset;
	}

