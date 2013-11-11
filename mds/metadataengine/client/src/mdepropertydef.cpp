/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/

#include "mdepropertydef.h"

#include "mdcdef.h"
#include "mdeobjectdef.h"
#include "mdenamespacedef.h"
#include "mdscommoninternal.h"
#include "mdcserializationbuffer.h"


CMdEPropertyDef::CMdEPropertyDef( const TMdCPropertyDef& aPropertyDef, CMdEObjectDef& aObjectDef )
    : iPropertyDef(aPropertyDef), iObjectDef(aObjectDef)
    {
    }

CMdEPropertyDef* CMdEPropertyDef::NewL( const TMdCPropertyDef& aPropertyDef,
    CMdCSerializationBuffer& aSchemaBuffer, CMdEObjectDef& aObjectDef )
    {
    CMdEPropertyDef* def = CMdEPropertyDef::NewLC( aPropertyDef, aSchemaBuffer, aObjectDef );
    CleanupStack::Pop( def );
    return def;
    }

CMdEPropertyDef* CMdEPropertyDef::NewLC( const TMdCPropertyDef& aPropertyDef,
    CMdCSerializationBuffer& aSchemaBuffer, CMdEObjectDef& aObjectDef )
    {
    CMdEPropertyDef* def = new( ELeave ) CMdEPropertyDef( aPropertyDef, aObjectDef );
    CleanupStack::PushL( def );
    def->ConstructL( aSchemaBuffer );
    return def;
    }

void CMdEPropertyDef::ConstructL( CMdCSerializationBuffer& aSchemaBuffer )
    {
	aSchemaBuffer.PositionL( iPropertyDef.iName.iPtr.iOffset );
	iName.Set( aSchemaBuffer.ReceivePtr16L() );
    }

CMdEPropertyDef::~CMdEPropertyDef()
    {
    }

TDefId CMdEPropertyDef::Id() const
    {
    return iPropertyDef.iDefId;
    }

TDefId CMdEPropertyDef::ObjectDefId() const
	{
	return iObjectDef.Id();
	}

EXPORT_C CMdEObjectDef& CMdEPropertyDef::ObjectDef() const
    {
    return iObjectDef;
    }
    
EXPORT_C const TDesC& CMdEPropertyDef::Name() const
    {
    return iName;
    }

EXPORT_C TInt CMdEPropertyDef::Compare( const CMdEPropertyDef& aPropertyDef ) const
    {
    TInt result = iObjectDef.NamespaceDef().Compare( aPropertyDef.iObjectDef.NamespaceDef() );
    if ( result == 0 )
        {
        result = iPropertyDef.iDefId - aPropertyDef.Id();
        }
    return result;
    }

EXPORT_C TPropertyType CMdEPropertyDef::PropertyType() const
    {
    return (TPropertyType)iPropertyDef.iValueType;
    }

EXPORT_C TBool CMdEPropertyDef::ReadOnly() const
    {
    return iPropertyDef.iFlags & EPropertyReadOnly ? ETrue : EFalse;
    }

EXPORT_C TBool CMdEPropertyDef::Mandatory() const
	{
	return iPropertyDef.iFlags & EPropertyMandatory ? ETrue : EFalse;
	}

EXPORT_C TInt32 CMdEPropertyDef::MinInt32ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyInt8 &&
    	 type != EPropertyInt16 &&
    	 type != EPropertyInt32 )
    	{
         User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMinValue.iInt32;
    }

EXPORT_C TInt32 CMdEPropertyDef::MaxInt32ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyInt8 &&
    	 type != EPropertyInt16 &&
    	 type != EPropertyInt32 )
    	{
        User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMaxValue.iInt32;
    }

EXPORT_C TUint32 CMdEPropertyDef::MinUint32ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyUint8 &&
    	 type != EPropertyUint16 &&
    	 type != EPropertyUint32 )
    	{
        User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMinValue.iUint32;
    }

EXPORT_C TUint32 CMdEPropertyDef::MaxUint32ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyUint8 &&
    	 type != EPropertyUint16 &&
     	 type != EPropertyUint32 )
    	{
        User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMaxValue.iUint32;
    }

EXPORT_C TInt64 CMdEPropertyDef::MinInt64ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyInt64 )
    	{
         User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMinValue.iInt64;
    }

EXPORT_C TInt64 CMdEPropertyDef::MaxInt64ValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyInt64 )
    	{
         User::Leave( KErrNotSupported );        
        }
    return iPropertyDef.iMaxValue.iInt64;
    }

EXPORT_C TReal CMdEPropertyDef::MinRealValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyReal32 && type != EPropertyReal64 )
    	{
         User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMinValue.iReal;
    }

EXPORT_C TReal CMdEPropertyDef::MaxRealValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyReal32 && type != EPropertyReal64 )
    	{
         User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMaxValue.iReal;
    }

EXPORT_C TTime CMdEPropertyDef::MinTimeValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyTime )
        {
        User::Leave( KErrNotSupported );
        }
    return TTime(iPropertyDef.iMinValue.iInt64);
    }

EXPORT_C TTime CMdEPropertyDef::MaxTimeValueL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyTime )
        {
        User::Leave( KErrNotSupported );
        }
    return TTime(iPropertyDef.iMaxValue.iInt64);
    }

EXPORT_C TInt CMdEPropertyDef::MinTextLengthL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyText )
        {
        User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMinValue.iInt32;
    }

EXPORT_C TInt CMdEPropertyDef::MaxTextLengthL() const
    {
    const TPropertyType type = PropertyType();
    if ( type != EPropertyText )
        {
        User::Leave( KErrNotSupported );
        }
    return iPropertyDef.iMaxValue.iInt32;
    }

EXPORT_C CMdENamespaceDef& CMdEPropertyDef::NamespaceDef() const
    {
    return iObjectDef.NamespaceDef();
    }
