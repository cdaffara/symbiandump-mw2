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
* Description:  Class to hold description about one property
*
*/

#include "mdspropertydef.h"

#include "mdcdef.h"
#include "mdssqliteconnection.h"
#include "mdsindexer.h"
#include "mdcserializationbuffer.h"
#include "mdeinternalerror.h"

/**
 * NewLC
 */
CMdsPropertyDef* CMdsPropertyDef::NewLC( const TDesC& aName, TPropertyType aType,
		TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* ret = new( ELeave ) CMdsPropertyDef( aType );
	CleanupStack::PushL( ret );
	ret->ConstructL( aName, aReadOnly, aMandatory, aIndexed );
	return ret;
	}
	
/**
 * NewL
 */
CMdsPropertyDef* CMdsPropertyDef::NewL( const TDesC& aName, TPropertyType aType,
		TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* ret = CMdsPropertyDef::NewLC( aName, aType, aReadOnly, aMandatory, aIndexed );
	CleanupStack::Pop( ret );
	return ret;
	}

CMdsPropertyDef* CMdsPropertyDef::NewL( const TDesC& aName, TPropertyType aType, TUint32 aFlags )
	{
	return CMdsPropertyDef::NewL( aName, aType, aFlags & EPropertyReadOnly, aFlags & EPropertyMandatory, EFalse );
	}

CMdsPropertyDef* CMdsPropertyDef::NewLC( const TDesC& aName, TPropertyType aType, TUint32 aFlags )
	{
	return CMdsPropertyDef::NewLC( aName, aType, aFlags & EPropertyReadOnly, aFlags & EPropertyMandatory, EFalse );
	}

/**
 * default constructor
 */
inline CMdsPropertyDef::CMdsPropertyDef( TPropertyType aType )
	: iType( aType )
	{}

/**
 * ConstructL
 */
void CMdsPropertyDef::ConstructL( const TDesC& aName, TBool aReadOnly, TBool aMandatory, 
		TBool aIndexed )
	{
	// check that only allowed characters (A-Z, a-z, 0-9) are used
	CheckAllowerCharatersL( aName, ETrue );
	
	CMdsItemDef::ConstructL( aName );

	if ( aMandatory )
		{
		iPropertyFlags |= EPropertyMandatory;
		}

	if ( aReadOnly )
		{
		iPropertyFlags |= EPropertyReadOnly;
		}
	
	iIndexed = aIndexed;
	}

/**
 * Destructor
 */
CMdsPropertyDef::~CMdsPropertyDef()
	{
	}
	
/**
 * AddMinMaxValueL
 */
void CMdsPropertyDef::AddMinMaxValueL( const TInt32& aMinValue, const TInt32& aMaxValue )
	{
	if( iType == EPropertyBool || iType == EPropertyInt8 || iType ==  EPropertyUint8 ||
    	iType == EPropertyInt16 || iType == EPropertyUint16 || iType == EPropertyInt32 ||
    	iType == EPropertyText )
		{
		iMinValue.iInt32 = aMinValue;
		iMaxValue.iInt32 = aMaxValue;
		}
	else
		{
#ifdef _DEBUG
		User::Panic( _L("MdSPDAd1") , KErrMdEUnknownPropertyType );
#endif
		User::Leave( KErrMdEUnknownPropertyType );
		}
	}

/**
 * AddMinMaxValueL
 */
void CMdsPropertyDef::AddMinMaxValueL( const TUint32& aMinValue, const TUint32& aMaxValue )
	{
	if( iType == EPropertyUint32 )
		{
		iMinValue.iUint32 = aMinValue;
		iMaxValue.iUint32 = aMaxValue;
		}
	else
		{
#ifdef _DEBUG
		User::Panic( _L("MdSPDAd2") , KErrMdEUnknownPropertyType );
#endif
		User::Leave( KErrMdEUnknownPropertyType );
		}
	}

/**
 * AddMinMaxValueL
 */
void CMdsPropertyDef::AddMinMaxValueL( const TInt64& aMinValue, const TInt64& aMaxValue )
	{
	if( iType == EPropertyInt64 || iType == EPropertyTime )
		{
		iMinValue.iInt64 = aMinValue;
		iMaxValue.iInt64 = aMaxValue;
		}
	else
		{
#ifdef _DEBUG
		User::Panic( _L("MdSPDAd3") , KErrMdEUnknownPropertyType );
#endif
		User::Leave( KErrMdEUnknownPropertyType );
		}
	}

/**
 * AddMinMaxValueL
 */
void CMdsPropertyDef::AddMinMaxValueL( const TReal& aMinValue, const TReal& aMaxValue )
	{
	if( iType == EPropertyReal32 || iType == EPropertyReal64 )
		{
		iMinValue.iReal = aMinValue;
		iMaxValue.iReal = aMaxValue;
		}
	else
		{
#ifdef _DEBUG
		User::Panic( _L("MdSPDAd4") , KErrMdEUnknownPropertyType );
#endif
		User::Leave( KErrMdEUnknownPropertyType );
		}
	}


/**
 * Get SQLite type name
 * @return type name
 */
const TDesC& CMdsPropertyDef::GetSqlTypeName() const
	{
	_LIT( KMdsSqlTypeInt,    "INTEGER" );
	_LIT( KMdsSqlTypeBigInt, "LARGEINT" );
	_LIT( KMdsSqlTypeReal,   "REAL" );
	_LIT( KMdsSqlTypeText,   "TEXT" );
	
	switch( iType )
		{
		case EPropertyBool: case EPropertyInt8: case EPropertyUint8: case EPropertyInt16:
        case EPropertyUint16: case EPropertyInt32:
        	{
        	return KMdsSqlTypeInt;
        	}
        case EPropertyUint32: case EPropertyInt64: case EPropertyTime:
        	{
        	return KMdsSqlTypeBigInt;
        	}
        case EPropertyReal32:
        case EPropertyReal64:
        	{
        	return KMdsSqlTypeReal;
        	}
        case EPropertyText:
        	{
        	return KMdsSqlTypeText;
        	}
        default:
        	return KMdsSqlTypeText;	
		}
	}

TColumnDataType CMdsPropertyDef::GetSqlType() const
	{	
	switch( iType )
		{
		case EPropertyBool:
			return EColumnBool;
		case EPropertyInt8:
		case EPropertyInt16:
        case EPropertyInt32:
			return EColumnInt32;
		case EPropertyUint8:
        case EPropertyUint16:
        case EPropertyUint32: 
        	return EColumnUint32;
        case EPropertyInt64:
        	return EColumnInt64;
        case EPropertyTime:
        	return EColumnTime;
        case EPropertyReal32:
        	return EColumnReal32;
        case EPropertyReal64:
        	return EColumnReal64;
        case EPropertyText:
        	return EColumnDes16;
        default:
        	return EColumnDes16;
		}
	}
	
void CMdsPropertyDef::StoreToDBL( TDefId aObjectDefId )
	{
	_LIT( KMdsSqlClauseAddPropertyDef, "INSERT INTO PropertyDef(ObjectDefId,Flags,Type,MinValue,MaxValue,Name) Values(?,?,?,?,?,?);" );

	if ( GetStoredInDB() )
		{
		return;
		}

    RRowData rowData;
    CleanupClosePushL( rowData );
	rowData.AppendL( TColumn( aObjectDefId ) );
	rowData.AppendL( TColumn( iPropertyFlags ) );
	rowData.AppendL( TColumn( iType ) );
	switch( iType )
		{
    	case EPropertyBool: case EPropertyInt8: case EPropertyUint8: case EPropertyInt16:
    	case EPropertyUint16: case EPropertyInt32: case EPropertyText:
    		{
    		rowData.AppendL( TColumn( iMinValue.iInt32 ) );
    		rowData.AppendL( TColumn( iMaxValue.iInt32 ) );
    		break;
    		}
    	case EPropertyUint32:
    		{
    		rowData.AppendL( TColumn( (TInt64)iMinValue.iUint32 ) );
    		rowData.AppendL( TColumn( (TInt64)iMaxValue.iUint32 ) );
    		break;
    		}
    	case EPropertyInt64: case EPropertyTime:
    		{
    		rowData.AppendL( TColumn( iMinValue.iInt64 ) );
    		rowData.AppendL( TColumn( iMaxValue.iInt64 ) );
    		break;
    		}
    	case EPropertyReal32: case EPropertyReal64:
    		{
    		rowData.AppendL( TColumn( iMinValue.iReal ) );
    		rowData.AppendL( TColumn( iMaxValue.iReal ) );
    		break;
    		}
    	default:
#ifdef _DEBUG
    		User::Panic( _L("MdSPDSto") , KErrMdEUnknownPropertyType );
#endif
    		User::Leave( KErrMdEUnknownPropertyType );
		}
	rowData.AppendL( TColumn( GetName().AllocLC() ) );
	CleanupStack::Pop(); //name

	TDefId id;
	id =  MMdSIndexer::ExecuteAndGetIndexL( KMdsSqlClauseAddPropertyDef, rowData );
	SetId( id );

	SetStoredInDB();
	CleanupStack::PopAndDestroy( &rowData );
	}

TBool CMdsPropertyDef::operator==( const CMdsPropertyDef& aPropertyDef ) const
	{
	if ( iType != aPropertyDef.iType || iPropertyFlags != aPropertyDef.iPropertyFlags ||
	     GetName() != aPropertyDef.GetName() )
		{
		return EFalse;
		}
	switch( iType )
		{
		case EPropertyBool: case EPropertyInt8: case EPropertyUint8: case EPropertyInt16:
        case EPropertyUint16: case EPropertyInt32: case EPropertyText:
        	{
        	return ( iMinValue.iInt32 == aPropertyDef.iMinValue.iInt32 &&
        	         iMaxValue.iInt32 == aPropertyDef.iMaxValue.iInt32 ) ;
        	}
        case EPropertyUint32:
        	{
        	return ( iMinValue.iUint32 == aPropertyDef.iMinValue.iUint32 &&
        	         iMaxValue.iUint32 == aPropertyDef.iMaxValue.iUint32 ) ;
        	}
        case EPropertyReal32:
        case EPropertyReal64:
        	{
        	return ( iMinValue.iReal == aPropertyDef.iMinValue.iReal &&
        	         iMaxValue.iReal == aPropertyDef.iMaxValue.iReal ) ;
        	}
        case EPropertyInt64:
        case EPropertyTime:
        	{
        	return ( iMinValue.iInt64 == aPropertyDef.iMinValue.iInt64 &&
        	         iMaxValue.iInt64 == aPropertyDef.iMaxValue.iInt64 ) ;
        	}
        default:
            return EFalse;
		}
	
	}

TUint32 CMdsPropertyDef::RequiredBufferSize()
	{
	return sizeof(TMdCPropertyDef) + CMdsItemDef::RequiredBufferSize();
	}

TMdCOffset CMdsPropertyDef::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace)
	{
	TMdCPropertyDef propertyDef;
	propertyDef.iDefId = GetId();
	propertyDef.iFlags = iPropertyFlags;
	propertyDef.iValueType = iType;
	propertyDef.iMinValue = iMinValue;
	propertyDef.iMaxValue = iMaxValue;
	
	// store name
	propertyDef.iName.iPtr.iCount = GetName().Length();
	propertyDef.iName.iPtr.iOffset = aFreeSpace;
	propertyDef.SerializeL( aBuffer );
	
	aBuffer.PositionL( aFreeSpace );
	return CMdsItemDef::SerializeL( aBuffer );
	}
