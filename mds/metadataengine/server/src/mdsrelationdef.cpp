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
* Description:  Class to hold description about one relation
*
*/

#include "mdsrelationdef.h"

#include "mdcdef.h"
#include "mdssqliteconnection.h"
#include "mdcserializationbuffer.h"
#include "mdsindexer.h"


/**
 * NewLC
 */
CMdsRelationDef* CMdsRelationDef::NewLC(const TDesC& aName )
	{
	CMdsRelationDef* ret = new( ELeave ) CMdsRelationDef();
	CleanupStack::PushL( ret );
	ret->ConstructL( aName );
	return ret;
	}
	
/**
 * NewL
 */
CMdsRelationDef* CMdsRelationDef::NewL(const TDesC& aName )
	{
	CMdsRelationDef* ret = CMdsRelationDef::NewLC( aName );
	CleanupStack::Pop( ret );
	return ret;
	}

/**
 * ConstructL
 */
void CMdsRelationDef::ConstructL( const TDesC& aName )
	{
	// check that only allowed characters (A-Z, a-z) are used
	CheckAllowerCharatersL( aName, EFalse );
	
	CMdsItemDef::ConstructL( aName );
	}

/**
 * Destructor
 */
CMdsRelationDef::~CMdsRelationDef()
	{
	}
	
void CMdsRelationDef::StoreToDBL( const TDefId aNamespaceDefId )
	{
	_LIT( KMdsSqlClauseAddRelationDef, "INSERT INTO RelationDef (NamespaceDefId,Name) Values(?,?);" );

	if ( GetStoredInDB() )
		{
		return;
		}

    RRowData rowData;
    CleanupClosePushL( rowData );

	rowData.AppendL( TColumn( aNamespaceDefId ) );
	rowData.AppendL( TColumn( GetName().AllocLC() ) );
	CleanupStack::Pop(); //name
	TDefId id =  MMdSIndexer::ExecuteAndGetIndexL( KMdsSqlClauseAddRelationDef, rowData );
	if ( id != KNoDefId )
		{
		SetId( id );
		}
	SetStoredInDB();
	CleanupStack::PopAndDestroy( &rowData );
	}

TBool CMdsRelationDef::operator==( const CMdsRelationDef& aRelationDef ) const
	{
	return GetName() == aRelationDef.GetName();
	}

TUint32 CMdsRelationDef::RequiredBufferSize()
	{
	return sizeof(TMdCRelationDef) + CMdsItemDef::RequiredBufferSize();
	}

TMdCOffset CMdsRelationDef::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace)
	{
	TMdCRelationDef relationDef;
	relationDef.iDefId = GetId();
	
	// store name
	relationDef.iName.iPtr.iCount = GetName().Length();
	relationDef.iName.iPtr.iOffset = aFreeSpace;
	relationDef.SerializeL( aBuffer );
	
	aBuffer.PositionL( aFreeSpace );
	return CMdsItemDef::SerializeL( aBuffer );
	}
