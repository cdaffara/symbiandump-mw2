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
* Description:  Class to hold description about one object and it's properties
*
*/

#include "mdsobjectdef.h"

#include "mdcdef.h"
#include "mdspropertydef.h"
#include "mdsdbconnectionpool.h"
#include "mdcserializationbuffer.h"
#include "mdsindexer.h"
#include "mdeinternalerror.h"

/**
 * NewLC
 */
CMdsObjectDef* CMdsObjectDef::NewLC(const TDesC& aName, const CMdsObjectDef* aParent )
	{
	CMdsObjectDef* ret = new( ELeave ) CMdsObjectDef( aParent );
	CleanupStack::PushL( ret );
	ret->ConstructL( aName );
	return ret;
	}
	
/**
 * NewL
 */
CMdsObjectDef* CMdsObjectDef::NewL( const TDesC& aName, const CMdsObjectDef* aParent )
	{
	CMdsObjectDef* ret = CMdsObjectDef::NewLC( aName, aParent );
	CleanupStack::Pop( ret );
	return ret;
	}

/**
 * default constructor
 */
inline CMdsObjectDef::CMdsObjectDef( const CMdsObjectDef* aParent )
	: iParent ( aParent ), iFlags( EObjectDefFlagsNone ), iMandatoryPropertyCount( 0 )
	{}

/**
 * ConstructL
 */
void CMdsObjectDef::ConstructL( const TDesC& aName )
	{
	// check that only allowed characters (A-Z, a-z) are used
	CheckAllowerCharatersL( aName, EFalse );

	CMdsItemDef::ConstructL( aName );
	}

/**
 * Destructor
 */
CMdsObjectDef::~CMdsObjectDef()
	{
	// deleting properties
	iPropertyDefs.ResetAndDestroy();
	iPropertyDefs.Close();

	iCol2Prop.Close();
	}

void CMdsObjectDef::AddPropertyL( const TDesC& aName, TPropertyType aType,
								  const TMdCValueUnion& aMinAdd, const TMdCValueUnion& aMaxAdd,
								  TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* newProp = CommonAddPropertyL( aName, aType, aReadOnly, aMandatory, aIndexed );
	if (!newProp)
		{
		User::Leave( KErrMdEUnknownPropertyDef );
		}
	switch (aType)
		{									
		case EPropertyUint32:
		case EPropertyText:
		case EPropertyBool:		
			{									
			newProp->AddMinMaxValueL(aMinAdd.iUint32,aMaxAdd.iUint32);
			break;
			}
		case EPropertyInt32:
			{	
			newProp->AddMinMaxValueL(aMinAdd.iInt32,aMaxAdd.iInt32);
			break;
			}
		case EPropertyInt64:
		case EPropertyTime:
			{	
			newProp->AddMinMaxValueL(aMinAdd.iInt64,aMaxAdd.iInt64);
			break;
			}
		case EPropertyReal64:
			{									
			newProp->AddMinMaxValueL(aMinAdd.iReal,aMaxAdd.iReal);
			break;
			}

	    default:
	    	{
#ifdef _DEBUG
            User::Panic( _L("MdSODAdd") , KErrMdEUnknownPropertyType );
#endif		    	
	    	User::Leave( KErrMdEUnknownPropertyType );
	    	}
		}
	}
	
/**
 * AddPropertyL
 */
void CMdsObjectDef::AddPropertyL( const TDesC& aName, TPropertyType aType,
	                              TInt32 aMinAdd, TInt32 aMaxAdd,
	                              TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* newProp = CommonAddPropertyL( aName, aType, aReadOnly, aMandatory, aIndexed );
	newProp->AddMinMaxValueL( aMinAdd, aMaxAdd );
	}

/**
 * AddPropertyL
 */
void CMdsObjectDef::AddPropertyL( const TDesC& aName, TPropertyType aType,
	                              TUint32 aMinAdd, TUint32 aMaxAdd,
	                              TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* newProp = CommonAddPropertyL( aName, aType, aReadOnly, aMandatory, aIndexed );
	newProp->AddMinMaxValueL( aMinAdd, aMaxAdd );
	}

/**
 * AddPropertyL
 */
void CMdsObjectDef::AddPropertyL( const TDesC& aName, TPropertyType aType,
	                              const TInt64& aMinAdd, const TInt64& aMaxAdd,
	                              TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* newProp = CommonAddPropertyL( aName, aType, aReadOnly, aMandatory, aIndexed );
	newProp->AddMinMaxValueL( aMinAdd, aMaxAdd );
	}

/**
 * AddPropertyL
 */
void CMdsObjectDef::AddPropertyL( const TDesC& aName, TPropertyType aType,
	                              const TReal& aMinAdd, const TReal& aMaxAdd,
	                              TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* newProp = CommonAddPropertyL( aName, aType, aReadOnly, aMandatory, aIndexed );
	newProp->AddMinMaxValueL( aMinAdd, aMaxAdd );
	}

/**
 * CommonAddPropertyL
 */
CMdsPropertyDef* CMdsObjectDef::CommonAddPropertyL( const TDesC& aName, TPropertyType aType,
		TBool aReadOnly, TBool aMandatory, TBool aIndexed )
	{
	CMdsPropertyDef* propertyDef = CMdsPropertyDef::NewLC( aName, aType, aReadOnly, aMandatory,
			aIndexed );
	iPropertyDefs.AppendL( propertyDef );
	CleanupStack::Pop( propertyDef );
	return propertyDef;
	}

CMdsPropertyDef* CMdsObjectDef::CommonAddPropertyL( const TDesC& aName, TPropertyType aType,
		TUint32 aFlags, TDefId aId )
	{
	CMdsPropertyDef* propertyDef = CMdsPropertyDef::NewLC( aName, aType, aFlags );
	propertyDef->SetId(aId);
	iPropertyDefs.AppendL( propertyDef );
	CleanupStack::Pop( propertyDef );
	return propertyDef;
	}

CMdsPropertyDef* CMdsObjectDef::GetProperty( TUint32 aIndex ) const
	{
	return iPropertyDefs[aIndex];
	}

CMdsPropertyDef* CMdsObjectDef::GetProperty( const TDesC& aName ) const
	{
	const TInt count = iPropertyDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if( iPropertyDefs[i]->GetName().Compare( aName ) == 0 )
			{
			return iPropertyDefs[i];
			}
		}
	if ( iParent )
		{
		return iParent->GetProperty( aName );
		}
	return NULL;
	}


const CMdsPropertyDef* CMdsObjectDef::GetPropertyByIdL( TDefId aId ) const
	{
	const TInt propertyDefCount = iPropertyDefs.Count();
	for ( TInt i = 0; i < propertyDefCount; ++i )
		{
		const CMdsPropertyDef* propDef = iPropertyDefs[i];
		if( propDef->GetId() == aId )
			{
			return propDef;
			}
		}
	if ( iParent )
		{
		return iParent->GetPropertyByIdL( aId );
		}
	return NULL;
	}


void CMdsObjectDef::StoreToDBL( TDefId aNamespaceDefId )
	{
	_LIT( KMdsSqlClauseTriggerObjectDef, "CREATE TRIGGER Delete%S%u DELETE ON Object%u BEGIN DELETE FROM %S%u WHERE ObjectId=OLD.ObjectId; END;" );
	_LIT( KMdsSqlClauseAddObjectDef, "INSERT INTO ObjectDef(NamespaceDefId, ParentDefId, Flags, Name) Values(?,?,?,?);" );

	RRowData rowData;
    CleanupClosePushL( rowData );

	if ( !GetStoredInDB() )
		{
		rowData.AppendL( TColumn( aNamespaceDefId ) );
		rowData.AppendL( TColumn( iParent ? iParent->GetId() : KNoDefId ) );
		rowData.AppendL( TColumn( (TInt32)iFlags ) );
		rowData.AppendL( TColumn( GetName().AllocLC() ) );
		CleanupStack::Pop(); // name

		TDefId id = MMdSIndexer::ExecuteAndGetIndexL( KMdsSqlClauseAddObjectDef,rowData );

	    SetId( id );
       	SetStoredInDB();

    	if ( GetId() != KBaseObjectDefId )
    		{
	    	// Create trigger
	        HBufC* sqlQueryCreateTrigger = HBufC::NewLC( KMdsSqlClauseTriggerObjectDef().Size() + ( KMaxUintValueLength * 3 )  + ( GetName().Length() * 2 ) );
            TPtr sqlPtr = sqlQueryCreateTrigger->Des();
            sqlPtr.Format( KMdsSqlClauseTriggerObjectDef, &GetName(), aNamespaceDefId, aNamespaceDefId, &GetName(), aNamespaceDefId );
            RRowData rowDataDummy;
		    CleanupClosePushL( rowDataDummy );
		    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
            connection.ExecuteL( *sqlQueryCreateTrigger, rowDataDummy );
            CleanupStack::PopAndDestroy( 2, sqlQueryCreateTrigger ); // rowDataDummy, sqlQueryCreateTrigger
    		}
		}

	// add propertyDef to DB
	for( TInt counter = 0; counter < iPropertyDefs.Count(); ++counter )
		{
		iPropertyDefs[counter]->StoreToDBL( GetId() );
		}
	
	CleanupStack::PopAndDestroy( &rowData );
	}

CMdsPropertyDef* CMdsObjectDef::FindProperty( const TDesC& aName ) const
	{
	const TInt count = iPropertyDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if ( iPropertyDefs[i]->GetName().Compare( aName ) == 0 )
			{
			return iPropertyDefs[i];
			}
		}
	return NULL;
	}

void CMdsObjectDef::MergeL( CMdsObjectDef* aObjectDef, const TBool& /*aDryRun*/ )
	{
	const TInt count = aObjectDef->iPropertyDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsPropertyDef* lPropertyDef = FindProperty( aObjectDef->iPropertyDefs[i]->GetName() );
		if ( lPropertyDef )
			{
			if ( *lPropertyDef != *(aObjectDef->iPropertyDefs[i]) )
				{
				User::Leave( KErrAlreadyExists );
				}
			}
		else
			{
			User::Leave( KErrAccessDenied );
			}
		}
	}

void CMdsObjectDef::AddPropertyL( const TDefId aId, TPropertyType aType, TUint32 aFlags,
	                              const TDesC& aMinAdd, const TDesC& aMaxAdd, const TDesC& aName )
	{
	CMdsPropertyDef* prop = CommonAddPropertyL( aName, aType, aFlags, aId );
	prop->SetStoredInDB();
	switch( aType )
		{
		case EPropertyBool: case EPropertyInt8: case EPropertyUint8: case EPropertyInt16:
        case EPropertyUint16: case EPropertyInt32: case EPropertyText:
        	{
        	TInt32 minValue = 0;
            TInt32 maxValue = 0;
        	TLex lex( aMinAdd );
        	lex.Val( minValue );
        	lex.Assign( aMaxAdd );
        	lex.Val( maxValue );
        	prop->AddMinMaxValueL( minValue, maxValue );
        	break;
        	}
        case EPropertyUint32:
        	{
        	TUint32 minValue = 0;
        	TUint32 maxValue = 0;
        	TLex lex( aMinAdd );
        	lex.Val( (TInt64&)minValue );
        	lex.Assign( aMaxAdd );
        	lex.Val( (TInt64&)maxValue );
        	prop->AddMinMaxValueL( minValue, maxValue );
        	break;
        	}
        case EPropertyInt64:
        case EPropertyTime:
        	{
        	TInt64 minValue, maxValue;
        	TLex lex( aMinAdd );
        	lex.Val( minValue );
        	lex.Assign( aMaxAdd );
        	lex.Val( maxValue );
        	prop->AddMinMaxValueL( minValue, maxValue );
        	break;
        	}
        case EPropertyReal32:
        case EPropertyReal64:
        	{
        	TReal minValue, maxValue;
        	TLex lex( aMinAdd );
        	lex.Val( minValue );
        	lex.Assign( aMaxAdd );
        	lex.Val( maxValue );
        	prop->AddMinMaxValueL( minValue, maxValue );
        	break;
        	}
        default:
        	{
#ifdef _DEBUG
            User::Panic( _L("MdSODAd2") , KErrMdEUnknownPropertyType );
#endif		    	
        	
        	User::Leave( KErrMdEUnknownPropertyType );
        	}
		}
	}


void CMdsObjectDef::ImportFromDBL()
	{
	_LIT( KMdsQueryGetPropertyDefs, "SELECT PropertyDefId,Flags,Type,MinValue,MaxValue,Name FROM PropertyDef WHERE ObjectDefId=?;" );
	_LIT( KMdsQueryGetCol2Prop,     "SELECT PropertyDefId,ColumnId FROM Col2Prop WHERE ObjectDefId=?;" );

	RMdsStatement query;
	CleanupClosePushL( query );

	// importing properties
	RRowData objectNumber;
	CleanupClosePushL( objectNumber );
	objectNumber.AppendL( TColumn( GetId() ) );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	TDefId propertyDefId = 0;
	TPropertyType type;
	TInt32 typeValue = 0;
	TUint32 propertyFlags = 0;
	TPtrC16 name;
    TPtrC16 minValue;
    TPtrC16 maxValue;
	RRowData getData;
	CleanupClosePushL( getData );
	getData.AppendL( TColumn( propertyDefId ) );
	getData.AppendL( TColumn( propertyFlags ) );
	getData.AppendL( TColumn( typeValue ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	connection.ExecuteQueryL( KMdsQueryGetPropertyDefs, query, objectNumber );
	while( connection.NextRowL( query, getData ) )
		{
		getData.Column( 0 ).Get( propertyDefId );
		getData.Column( 1 ).Get( propertyFlags );
		getData.Column( 2 ).Get( typeValue );
		type = (TPropertyType)typeValue;
		getData.Column( 3 ).Get( minValue );
		getData.Column( 4 ).Get( maxValue );
		getData.Column( 5 ).Get( name );
		AddPropertyL( propertyDefId, type, propertyFlags, minValue, maxValue, name );
		getData.Column( 5 ).Free( );
		getData.Column( 4 ).Free( );
		getData.Column( 3 ).Free( );
		}

	iCol2Prop.Reset();
	RMdsStatement queryCol;
	CleanupClosePushL( queryCol );
	RRowData getColData;
	CleanupClosePushL( getColData );
	TUint32 columnId = 0;
	getColData.AppendL( TColumn( propertyDefId ) );
	getColData.AppendL( TColumn( columnId ) );
	connection.ExecuteQueryL( KMdsQueryGetCol2Prop, queryCol, objectNumber );
	while( connection.NextRowL( queryCol, getColData ) )
		{
		getColData.Column( 0 ).Get( propertyDefId );
		getColData.Column( 1 ).Get( columnId );
		const CMdsPropertyDef* propertyDef = GetPropertyByIdL( propertyDefId );
		
		if( !propertyDef )
			{
			User::Leave( KErrMdEUnknownPropertyDef );
			}

		CMdsObjectDef::TMdsColumnOrder col = { columnId, *propertyDef };
		iCol2Prop.AppendL( col );
		
		if( propertyDef->GetMandatory() )
			{
			++iMandatoryPropertyCount;
			}
		}

	// everything is ok, so set the flags
	SetStoredInDB();
	SetTableStoredInDB();
	SetC2PStoredInDB();

	CleanupStack::PopAndDestroy( 5, &query ); // getColData, queryCol, getData, objectNumber, query
	}
	
TUint32 CMdsObjectDef::RequiredBufferSize()
	{
	TUint32 bufferSize = sizeof(TMdCObjectDef) + CMdsItemDef::RequiredBufferSize();

	// propertydefs
	const TInt propertyDefsCount = iPropertyDefs.Count();
	bufferSize += propertyDefsCount * sizeof(TMdCPropertyDef);
	for ( TInt i = 0; i < propertyDefsCount; ++i )
		{
		bufferSize += iPropertyDefs[i]->RequiredBufferSize();
		}

	return bufferSize;
	}

TMdCOffset CMdsObjectDef::SerializeL( CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace )
	{
	const TMdCOffset objectDefOffset = aBuffer.Position();
	TMdCObjectDef objectDef;
	// get const data and store Name
	objectDef.iDefId = GetId();
	objectDef.iParentId = iParent ? iParent->GetId() : KNoDefId;
	objectDef.iName.iPtr.iCount = GetName().Length();
	objectDef.iName.iPtr.iOffset = aFreeSpace;
	aBuffer.PositionL( aFreeSpace );
	aFreeSpace = CMdsItemDef::SerializeL( aBuffer );

	// calculate necessary stuff for propertyDefs
	const TInt propertyDefsCount = iPropertyDefs.Count();
	objectDef.iPropertyDefs.iPtr.iCount = propertyDefsCount;
	objectDef.iPropertyDefs.iPtr.iOffset = aFreeSpace;

	// create space for propertyDefs
	aFreeSpace += propertyDefsCount * sizeof(TMdCPropertyDef);

	for ( TInt i = 0; i < propertyDefsCount; ++i )
		{
		aBuffer.PositionL( objectDef.iPropertyDefs.iPtr.iOffset + i * sizeof(TMdCPropertyDef) );
		// write object
		aFreeSpace = iPropertyDefs[i]->SerializeL( aBuffer, aFreeSpace );
		}

	// store objectDef itself
	aBuffer.PositionL( objectDefOffset );
	objectDef.SerializeL( aBuffer );

	return aFreeSpace;
	}

TInt CMdsObjectDef::GetAllPropertiesCount() const
	{
#ifdef _DEBUG
	_LIT( KCountPanicError, "CMdsObjectDef::GetAllPropertiesCount()" );
	TInt temp = iPropertyDefs.Count() + ( iParent ? iParent->GetAllPropertiesCount() : 0 );
	__ASSERT_DEBUG( temp == iCol2Prop.Count(), User::Panic( KCountPanicError, KErrCorrupt ) );
#endif
	return iCol2Prop.Count();
	}

TInt CMdsObjectDef::GetMandatoryPropertyCount() const
	{
	return iMandatoryPropertyCount;
	}

void CMdsObjectDef::SetStoredEveryInDB()
	{
	SetStoredInDB();
	
	const TInt count = iPropertyDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		iPropertyDefs[i]->SetStoredInDB();
		}
	}

void CMdsObjectDef::SetNotStoredEveryInDB()
	{
	SetAllNotStoredInDB();
	
	const TInt count = iPropertyDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		iPropertyDefs[i]->SetAllNotStoredInDB();
		}
	}
