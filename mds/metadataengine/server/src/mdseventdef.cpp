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

#include "mdseventdef.h"

#include "mdcdef.h"
#include "mdssqliteconnection.h"
#include "mdsindexer.h"

/**
 * NewLC
 */
CMdsEventDef* CMdsEventDef::NewLC( const TDesC& aName, const TInt32 aPriority )
	{
	CMdsEventDef* ret = new( ELeave ) CMdsEventDef(  );
	CleanupStack::PushL( ret );
	ret->ConstructL( aName, aPriority );
	return ret;
	}
	
/**
 * NewL
 */
CMdsEventDef* CMdsEventDef::NewL( const TDesC& aName, const TInt32 aPriority )
	{
	CMdsEventDef* ret = CMdsEventDef::NewLC( aName, aPriority );
	CleanupStack::Pop( ret );
	return ret;
	}

/**
 * ConstructL
 */
void CMdsEventDef::ConstructL( const TDesC& aName, TInt32 aPriority )
	{
	// check that only allowed characters (A-Z, a-z) are used
	CheckAllowerCharatersL( aName, EFalse );

	CMdsItemDef::ConstructL( aName );
	iPriority = aPriority;
	}

/**
 * Destructor
 */
CMdsEventDef::~CMdsEventDef()
	{
	}
	
void CMdsEventDef::StoreToDBL( const TInt aNamespaceDefId )
	{
	_LIT( KMdsSqlClauseAddEventDef, "INSERT INTO EventDef (NamespaceDefId, Priority, Name ) Values(?, ?, ?);" );	
	
	if ( GetStoredInDB() )
		{
		return;
		}

    RRowData rowData;
    CleanupClosePushL( rowData );
	rowData.AppendL( TColumn( aNamespaceDefId ) );
	rowData.AppendL( TColumn( iPriority ) );
	rowData.AppendL( TColumn( GetName().AllocLC() ) );
	CleanupStack::Pop(); // name

	TDefId id = MMdSIndexer::ExecuteAndGetIndexL(KMdsSqlClauseAddEventDef, rowData );

	SetId( id );
 	SetStoredInDB();

	CleanupStack::PopAndDestroy( &rowData );
	}

TBool CMdsEventDef::operator==( const CMdsEventDef& aEventDef ) const
	{
	return GetName() == aEventDef.GetName() && iPriority == aEventDef.iPriority;
	}

TUint32 CMdsEventDef::RequiredBufferSize()
	{
	return sizeof(TMdCEventDef) + CMdsItemDef::RequiredBufferSize();
	}

TMdCOffset CMdsEventDef::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace)
	{
	TMdCEventDef eventDef;
	eventDef.iDefId = GetId();
	
	// store name
	eventDef.iName.iPtr.iCount = GetName().Length();
	eventDef.iName.iPtr.iOffset = aFreeSpace;
	eventDef.SerializeL( aBuffer );
	
	aBuffer.PositionL( aFreeSpace );
	return CMdsItemDef::SerializeL( aBuffer );
	}
