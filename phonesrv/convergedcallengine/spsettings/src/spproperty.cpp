/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service Provider Settings property
*
*/


#include "spproperty.h"
#include "spdefaultvalues.h"

const TInt KMaxIntLen = 15;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CSPProperty::CSPProperty()
    : iDataType( EDataTypeNotSet ),
      iPropertyType( EItemTypeNotDefined )
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CSPProperty::ConstructL()
    {
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPProperty* CSPProperty::NewL()
    {
    CSPProperty* self = CSPProperty::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPProperty* CSPProperty::NewLC()
    {
    CSPProperty* self = new( ELeave ) CSPProperty();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CSPProperty::~CSPProperty()
    {
    iPropertyValue.Close();
    }


// ---------------------------------------------------------------------------
// Returns name (type) of property
// ---------------------------------------------------------------------------
//
EXPORT_C TServicePropertyName CSPProperty::GetName() const
    {
    return iPropertyName;
    }


// ---------------------------------------------------------------------------
// Sets the name and corresponding datatype of property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::SetName( TServicePropertyName aPropertyName )
    {
    iPropertyName = aPropertyName;
    iDataType = CSPProperty::DataType( aPropertyName );
    iPropertyType = CSPProperty::PropertyType( aPropertyName );
    
    return KErrNone;
    }


// ---------------------------------------------------------------------------
// Return datatype of this property
// ---------------------------------------------------------------------------
//
EXPORT_C TPropertyDataType CSPProperty::GetDataType() const
    {
    return iDataType;
    }


// ---------------------------------------------------------------------------
// Set the datatype of this property
// ---------------------------------------------------------------------------
//
void CSPProperty::SetDataType( TPropertyDataType aDataType )
    {
    iDataType = aDataType;
    }


// ---------------------------------------------------------------------------
// Returns value of property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::GetValue( TInt& aValue ) const
    {
    TLex convert( iPropertyValue );
    TInt err = convert.Val( aValue );
    return err;
    }


// ---------------------------------------------------------------------------
// Sets value of property. Value must match to datatype.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::SetValue( TInt aValue )
    {
    TInt ret( 0 );
    if ( EDataTypeInt != iDataType )
        {
        ret = KErrArgument;        
        }
    else
        {
        TBuf<KMaxIntLen> val;
        val.AppendNum( aValue );
        iPropertyValue.Close();
        ret = iPropertyValue.Create( val );
        }
    return ret;
    }


// ---------------------------------------------------------------------------
// Returns value of property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::GetValue( TOnOff& aValue ) const
    {
    TLex convert( iPropertyValue );
    TInt value;
    TInt err = convert.Val( value );
    if( err == KErrNone )
        {
        if( value != EOONotSet && value != EOff && value != EOn )
            {
            return KErrOverflow;
            }
        else
            {
            aValue = static_cast<TOnOff>( value );
            return KErrNone;
            }
        }
    else 
        {
        return err;
        }
    }


// ---------------------------------------------------------------------------
// Sets value of property. Value must match to datatype.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::SetValue( TOnOff aValue )
    {
    TInt ret( 0 );
    if ( EDataTypeOnOff != iDataType )
        {
        ret = KErrArgument;        
        }
    else
        {
        TBuf<KMaxIntLen> val;
        val.AppendNum( aValue );
        iPropertyValue.Close();
        ret = iPropertyValue.Create( val );
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// Returns value of property
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::GetValue( TDes& aValue ) const
    {
    aValue.Zero();

    if ( aValue.MaxLength() >= iPropertyValue.Length() )
        {
        aValue.Copy( iPropertyValue );
        return KErrNone;
        }
    else
        {
        return KErrOverflow;
        }
    }


// ---------------------------------------------------------------------------
// Sets value of property. Value must match to datatype.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSPProperty::SetValue( const TDesC& aValue )
    {
    TInt ret( 0 );
    if ( aValue.Length() > KSPMaxDesLength )
        {
        ret = KErrOverflow;
        }
    else
        {
        iPropertyValue.Close();
        ret = iPropertyValue.Create( aValue );
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// Copies data from other property
// ---------------------------------------------------------------------------
//
EXPORT_C void CSPProperty::CopyL( const CSPProperty& aSource )
    {
    if( this != &aSource )
        {
        User::LeaveIfError( SetName( aSource.GetName() ) );
        SetDataType( aSource.GetDataType() );
        SetPropertyType( aSource.GetPropertyType() );

        iPropertyValue.Close();
        User::LeaveIfError( iPropertyValue.Create( KSPMaxDesLength ) );
        User::LeaveIfError( aSource.GetValue( iPropertyValue ) );
        }
    }


// ---------------------------------------------------------------------------
// Sets value of property without checking the datatype
// ---------------------------------------------------------------------------
//
TInt CSPProperty::SetValueNoDataTypeCheck( const TDesC& aValue )
    {
    TInt ret( 0 );
    if ( aValue.Length() > KSPMaxDesLength )
        {
        ret = KErrOverflow;
        }
    else
        {
        iPropertyValue.Close();
        ret = iPropertyValue.Create( aValue );
        }

    return ret;
    }


// ---------------------------------------------------------------------------
// Returns datatype by property name
// ---------------------------------------------------------------------------
//
EXPORT_C TPropertyDataType CSPProperty::DataType( TServicePropertyName aProperty )
    {
    if ( aProperty >= EPropertyServiceAttributeMask && aProperty <= ESubPropertyVMBXLaunchUid )
        {
        return EDataTypeInt;
        }
    else if ( aProperty >= EPropertyBrandId && aProperty <= ESubPropertyVMBXAddrScheme )
        {
        return EDataTypeDes;
        }
    else if ( aProperty >= ESubPropertyVoIPEnabled && aProperty <= ESubPropertyVMBXEnabled )
        {
        return EDataTypeOnOff;
        }
    else
        {
        return EDataTypeUnknown;
        }
    }

// ---------------------------------------------------------------------------
// Returns property type by property name
// ---------------------------------------------------------------------------
//
EXPORT_C TSPItemType CSPProperty::PropertyType( TServicePropertyName aProperty )
    {
    if ( ( aProperty >= ESubPropertyVoIPSettingsId && aProperty <= ESubPropertyVoIPTemporaryIAPId ) ||
            ( aProperty >= ESubPropertyVoIPRelNumber && aProperty <= ESubPropertyVoIPAddrScheme  ) ||
            ( aProperty >= ESubPropertyVoIPEnabled && aProperty <= ESubPropertyVoIPEnabled  ) )
        {
        return EItemTypeVoIPSubProperty;
        }
    else if ( ( aProperty >= ESubPropertyPresenceSettingsId && aProperty <= ESubPropertyPresenceLaunchUid ) ||
            ( aProperty >= ESubPropertyPresenceAddrScheme && aProperty <= ESubPropertyPresenceAddrScheme ) ||
            ( aProperty >= ESubPropertyPresenceEnabled && aProperty <= ESubPropertyPresenceRequestPreference ) )
        {
        return EItemTypePresenceSubProperty;
        }
    else if ( ( aProperty >= ESubPropertyIMSettingsId && aProperty <= ESubPropertyIMLaunchUid ) ||
            ( aProperty >= ESubPropertyIMAddrScheme && aProperty <= ESubPropertyIMAddrScheme ) ||
            ( aProperty >= ESubPropertyIMEnabled && aProperty <= ESubPropertyIMEnabled ) )
        {
        return EItemTypeIMSubProperty;
        }
    else if ( ( aProperty >= ESubPropertyVMBXSettingsId && aProperty <= ESubPropertyVMBXLaunchUid ) ||
            ( aProperty >= ESubPropertyVMBXListenAddress && aProperty <= ESubPropertyVMBXAddrScheme ) ||
            ( aProperty >= ESubPropertyVMBXListenRegister && aProperty <= ESubPropertyVMBXEnabled ) )
        {
        return EItemTypeVMBXSubProperty;
        }
    else if( ( aProperty >= EPropertyServiceAttributeMask && aProperty <= EPropertyRingtoneTimeout ) ||
             ( aProperty >= EPropertyBrandId && aProperty <= EPropertyIncomingEmailserver ) )
        {
        return EItemTypeProperty;
        }
    else
        {
        return EItemTypeNotDefined;
        }
    }


// ---------------------------------------------------------------------------
// Compare operator
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CSPProperty::operator==( const CSPProperty& aProperty ) const
    {
    TBool result( EFalse );

    RBuf tmp;
    TInt err = tmp.Create( KSPMaxDesLength );
    if( err != KErrNone )
        {
        return EFalse;
        }
    err = aProperty.GetValue( tmp );
    if( err != KErrNone )
        {
        return EFalse;
        }
    
    result = ( iPropertyType == aProperty.GetPropertyType() &&
               iPropertyName == aProperty.GetName() &&
               iDataType == aProperty.GetDataType() &&
               0 == iPropertyValue.Compare( tmp ) );
    tmp.Close();

    return result;
    }


// ---------------------------------------------------------------------------
// Get property type
// ---------------------------------------------------------------------------
//
EXPORT_C TSPItemType CSPProperty::GetPropertyType() const
    {
    return iPropertyType;
    }

// ---------------------------------------------------------------------------
// Set default value for property
// ---------------------------------------------------------------------------
//
TInt CSPProperty::SetPropertyType( TSPItemType aPropertyType )
    {
    iPropertyType = aPropertyType;
    
    return KErrNone;
    }
    

