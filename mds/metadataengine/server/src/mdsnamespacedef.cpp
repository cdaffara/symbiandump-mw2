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
* Description:  Class to hold description about one namespace and every object
*                it holds.
*
*/

#include "mdsnamespacedef.h"

#include "mdcdef.h"
#include "mdsobjectdef.h"
#include "mdsrelationdef.h"
#include "mdseventdef.h"
#include "mdssqliteconnection.h"
#include "mdsdbconnectionpool.h"
#include "mdcserializationbuffer.h"
#include "mdsschema.h"
#include "mdsindexer.h"
#include "mdeinternalerror.h"

#include <uriutils.h>

/**
 * NewLC
 */
CMdsNamespaceDef* CMdsNamespaceDef::NewLC( TDefId aId,
    const TDesC& aName, TBool aReadOnly, TUint32 aVendorId )
	{
	CMdsNamespaceDef* ret = new( ELeave ) CMdsNamespaceDef( aId, aReadOnly, aVendorId );
	CleanupStack::PushL( ret );
	ret->ConstructL( aName );
	return ret;
	}
	
/**
 * NewL
 */
CMdsNamespaceDef* CMdsNamespaceDef::NewL( TDefId aId, const TDesC& aName,
    TBool aReadOnly, TUint32 aVendorId )
	{
	CMdsNamespaceDef* ret = CMdsNamespaceDef::NewLC( aId, aName, aReadOnly, aVendorId );
	CleanupStack::Pop( ret );
	return ret;
	}

/**
 * ConstructL
 */
void CMdsNamespaceDef::ConstructL( const TDesC& aName )
	{
	// validate the URI of namespace
	TBool invalidChars = UriUtils::HasInvalidChars( aName );
	if( invalidChars )
		{
		User::Leave( KErrGeneral );
		}

	CMdsItemDef::ConstructL( aName );
	}

/**
 * Destructor
 */
CMdsNamespaceDef::~CMdsNamespaceDef()
	{
	// deleting objects
	iObjectDefs.ResetAndDestroy();
	iObjectDefs.Close();

	// deleting relations...
	iRelationDefs.ResetAndDestroy();
   	iRelationDefs.Close();

   	// deleting events...
    iEventDefs.ResetAndDestroy();
   	iEventDefs.Close();
	}
	
/**
 * AddObjectDefL
 */
CMdsObjectDef* CMdsNamespaceDef::AddObjectDefL( const TDesC& aObjectName, const TDesC& aParentName,
                                                CMdsSchema* aDefaultSchema )
	{
	if ( GetObjectDef( aObjectName ) )
		{
		// duplicate object
		User::Leave( KErrAlreadyExists );
		}
	CMdsObjectDef* parent = GetObjectDef( aParentName );
	if ( !parent )
		{
		// found namespace in default schema
		if ( aDefaultSchema )
			{
			CMdsNamespaceDef* namespaceDef = aDefaultSchema->GetNamespace( GetName() );
			if ( !namespaceDef )
				{
				User::Leave( KErrMdEUnknownNamespaceDef );
				}
			parent = namespaceDef->GetObjectDef( aParentName );
			}
		if( !parent )
			{
			User::Leave( KErrNotFound );
			}
		}
	CMdsObjectDef* object = CMdsObjectDef::NewLC( aObjectName, parent );
	User::LeaveIfError( iObjectDefs.InsertInOrderAllowRepeats(object, TLinearOrder<CMdsObjectDef>(CMdsNamespaceDef::CompareObjectDefId) ) );
	CleanupStack::Pop( object );
	return object;
	}

/**
 * AddObjectDefL (private - add from DB)
 */
void CMdsNamespaceDef::AddObjectDefL( TDefId aId, TDefId aParentId, TInt aFlags, const TDesC& aName )
	{
	const CMdsObjectDef* parent = GetObjectByIdL( aParentId );
	if ( !parent )
		{
		User::Leave( KErrNotFound );
		}
	CMdsObjectDef* object = CMdsObjectDef::NewLC( aName, parent );
	object->SetFlags( (CMdsObjectDef::TObjectDefFlags)aFlags );
	object->SetId(aId);
	User::LeaveIfError( iObjectDefs.InsertInOrderAllowRepeats(object, TLinearOrder<CMdsObjectDef>(CMdsNamespaceDef::CompareObjectDefId) ) );
	object->SetStoredInDB();
	CleanupStack::Pop( object );
	}

/**
 * FindObjectDefParent
 */
CMdsObjectDef* CMdsNamespaceDef::GetObjectDef( const TDesC& aObjectName ) const
	{
	if ( iBaseObject->GetName().Compare( aObjectName ) == 0 )
		{
		return iBaseObject;
		}

	const TInt count = iObjectDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if( iObjectDefs[i]->GetName().Compare( aObjectName ) == 0 )
			{
			return iObjectDefs[i];
			}
		}
	return NULL;
	}

void CMdsNamespaceDef::AddRelationDefL( const TDesC& aRelationName )
	{
	if ( GetRelationDef( aRelationName ) )
		{
		User::Leave( KErrAlreadyExists );
		}
	CMdsRelationDef* relation = CMdsRelationDef::NewLC( aRelationName );
	User::LeaveIfError( iRelationDefs.InsertInOrderAllowRepeats(relation, TLinearOrder<CMdsRelationDef>(CMdsNamespaceDef::CompareRelationDefId) ) );
	CleanupStack::Pop( relation );
	}


void CMdsNamespaceDef::AddRelationDefL( TDefId aId, const TDesC& aRelationName )
	{
	CMdsRelationDef* relation = CMdsRelationDef::NewLC( aRelationName );
	relation->SetId(aId);
	User::LeaveIfError( iRelationDefs.InsertInOrderAllowRepeats(relation, TLinearOrder<CMdsRelationDef>(CMdsNamespaceDef::CompareRelationDefId) ) );
	relation->SetStoredInDB();
	CleanupStack::Pop( relation );
	}


CMdsRelationDef* CMdsNamespaceDef::GetRelationDef( const TDesC& aRelationName ) const
	{
	const TInt count = iRelationDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if ( iRelationDefs[i]->GetName().Compare( aRelationName ) == 0 )
			{
			return iRelationDefs[i];
			}
		}
	return NULL;
	}

void CMdsNamespaceDef::AddEventDefL( const TDesC& aEventName, TInt32 aPriority )
	{
	if ( GetEventDef( aEventName ) )
		{
		User::Leave( KErrAlreadyExists );
		}
	CMdsEventDef* event = CMdsEventDef::NewLC( aEventName, aPriority );
	User::LeaveIfError( iEventDefs.InsertInOrderAllowRepeats(event, TLinearOrder<CMdsEventDef>(CMdsNamespaceDef::CompareEventDefId) ) );
	CleanupStack::Pop( event );
	}

void CMdsNamespaceDef::AddEventDefL( TDefId aId, const TDesC& aEventName, TInt32 aPriority )
	{
	CMdsEventDef* event = CMdsEventDef::NewLC( aEventName, aPriority );
	event->SetId(aId);
	User::LeaveIfError( iEventDefs.InsertInOrderAllowRepeats(event, TLinearOrder<CMdsEventDef>(CMdsNamespaceDef::CompareEventDefId) ) );
	event->SetStoredInDB();
	CleanupStack::Pop( event );
	}

CMdsEventDef* CMdsNamespaceDef::GetEventDef( const TDesC& aEventName ) const
	{
	const TInt count = iEventDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		if ( iEventDefs[i]->GetName().Compare( aEventName ) == 0 )
			{
			return iEventDefs[i];
			}
		}
	return NULL;
	}

const CMdsObjectDef* CMdsNamespaceDef::GetObjectByIdL( TDefId aId ) const
	{
	if ( aId == KBaseObjectDefId ) // BaseObject (hardcoded)
		{
		return iBaseObject;
		}

    TInt low( 0 );
    TInt high( iObjectDefs.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( aId - iObjectDefs[mid]->GetId() );
        if( compare == 0 )
            {
            return iObjectDefs[mid];
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
	return NULL;
	}

const CMdsEventDef* CMdsNamespaceDef::GetEventByIdL( TDefId aId ) const
	{
    TInt low( 0 );
    TInt high( iEventDefs.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( aId - iEventDefs[mid]->GetId() );
        if( compare == 0 )
            {
            return iEventDefs[mid];
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
	return NULL;
	}

const CMdsRelationDef* CMdsNamespaceDef::GetRelationByIdL( TDefId aId ) const
	{
    TInt low( 0 );
    TInt high( iRelationDefs.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( aId - iRelationDefs[mid]->GetId() );
        if( compare == 0 )
            {
            return iRelationDefs[mid];
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
	return NULL;
	}

void CMdsNamespaceDef::StoreToDBL( TBool aStoreOnlyNamespace )
	{
	_LIT( KMdsSqlClauseAddNamespaceDef, "INSERT INTO NamespaceDef(ReadOnly,VendorId,Name) Values(?,?,?);" );
    RRowData rowData;
    CleanupClosePushL( rowData );

	if ( !GetStoredInDB() )
		{
		rowData.AppendL( TColumn( GetReadOnly() ) );
		rowData.AppendL( TColumn( iVendorId ) );
		rowData.AppendL( TColumn( GetName().AllocL() ) );

		TDefId id = KNoDefId;
		id = MMdSIndexer::ExecuteAndGetIndexL(KMdsSqlClauseAddNamespaceDef,rowData);
		SetId( id );

		SetStoredInDB();
		}
	
	if (aStoreOnlyNamespace)
		{
		CleanupStack::PopAndDestroy( &rowData );
		return;
		}
	
	const TInt objectDefCount = iObjectDefs.Count();
	
	// add objectDef to DB
	for( TInt i = 0; i < objectDefCount; ++i )
		{
		iObjectDefs[i]->StoreToDBL( GetId() );
		}
	iObjectDefs.Sort( TLinearOrder<CMdsObjectDef>(CMdsNamespaceDef::CompareObjectDefId) );
	
	const TInt eventDefCount = iEventDefs.Count();
	
	// add relationDef to DB
	for( TInt i = 0; i < eventDefCount; ++i )
		{
		iEventDefs[i]->StoreToDBL( GetId() );
		}
	iEventDefs.Sort( TLinearOrder<CMdsEventDef>(CMdsNamespaceDef::CompareEventDefId) );
	
	const TInt relationDefCount = iRelationDefs.Count();
	
	// add eventDef to DB
	for( TInt i = 0; i < relationDefCount; ++i )
		{
		iRelationDefs[i]->StoreToDBL( GetId() );
		}
	iRelationDefs.Sort( TLinearOrder<CMdsRelationDef>(CMdsNamespaceDef::CompareRelationDefId) );

	CleanupStack::PopAndDestroy( &rowData );
	}
	

void CMdsNamespaceDef::MergeObjectsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun )
	{
	const TInt count = aNamespace->iObjectDefs.Count();	
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsObjectDef* lObjectDef = GetObjectDef( aNamespace->iObjectDefs[i]->GetName() );
		if( lObjectDef )
			{
			lObjectDef->MergeL( aNamespace->iObjectDefs[i], aDryRun );
			}
		else if ( !aDryRun )
			{
		    User::LeaveIfError( iObjectDefs.InsertInOrderAllowRepeats(aNamespace->iObjectDefs[i], 
		                                  TLinearOrder<CMdsObjectDef>(CMdsNamespaceDef::CompareObjectDefId) ) );
			aNamespace->iObjectDefs[i]->SetAllNotStoredInDB();
			aNamespace->iObjectDefs[i] = NULL;
			}
		}
	}
	
void CMdsNamespaceDef::MergeRelationsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun )
	{
	const TInt count = aNamespace->iRelationDefs.Count();	
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsRelationDef* relation = GetRelationDef( aNamespace->iRelationDefs[i]->GetName() );
		if ( relation )
			{
			if ( *aNamespace->iRelationDefs[i] != *relation )
				{
				User::Leave( KErrAlreadyExists );
				}
			}
		else if ( !aDryRun )
			{
		    User::LeaveIfError( iRelationDefs.InsertInOrderAllowRepeats(aNamespace->iRelationDefs[i], 
		                                        TLinearOrder<CMdsRelationDef>(CMdsNamespaceDef::CompareRelationDefId) ) );
			aNamespace->iRelationDefs[i]->SetAllNotStoredInDB();
			aNamespace->iRelationDefs[i] = NULL;
			}
		}
	}
	
void CMdsNamespaceDef::MergeEventsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun )
	{
	const TInt count = aNamespace->iEventDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsEventDef* event = GetEventDef( aNamespace->iEventDefs[i]->GetName() );
		if ( event )
			{
			if ( *aNamespace->iEventDefs[i] != *event )
				{
				User::Leave( KErrAlreadyExists );
				}
			}
		else if ( !aDryRun )
			{
		    User::LeaveIfError( iEventDefs.InsertInOrderAllowRepeats(aNamespace->iEventDefs[i], TLinearOrder<CMdsEventDef>(CMdsNamespaceDef::CompareEventDefId) ) );
			aNamespace->iEventDefs[i]->SetAllNotStoredInDB();
			aNamespace->iEventDefs[i] = NULL;
			}
		}
	}

void CMdsNamespaceDef::ImportFromDBL()
	{
	_LIT( KMdsQueryGetObjectDefs, "SELECT ObjectDefId,ParentDefId,Flags,Name FROM ObjectDef WHERE NamespaceDefId=?;" );
	_LIT( KMdsQueryGetRelationDefs, "SELECT RelationDefId,Name FROM RelationDef WHERE NamespaceDefId=?;" );
	_LIT( KMdsQueryGetEventDefs, "SELECT EventDefId,Priority,Name FROM EventDef WHERE NamespaceDefId=?;" );
	TDefId eId = KNoDefId;
	TInt32 objectParent = 0;
	TInt32 flags = 0;
	TPtrC name;

	// importing namespaces
	RRowData namespaceNumber;
	CleanupClosePushL( namespaceNumber );
	namespaceNumber.AppendL( TColumn( GetId() ) );
	RRowData getData;
	CleanupClosePushL( getData );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	// OBJECTS
	RMdsStatement queryObject;
	CleanupClosePushL( queryObject );
	getData.AppendL( TColumn( eId ) );
	getData.AppendL( TColumn( objectParent ) );
	getData.AppendL( TColumn( flags ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	connection.ExecuteQueryL( KMdsQueryGetObjectDefs, queryObject, namespaceNumber );
	while( connection.NextRowL( queryObject, getData ) )
		{
		getData.Column( 0 ).Get( eId );
		getData.Column( 1 ).Get( objectParent );
		getData.Column( 2 ).Get( flags );
		getData.Column( 3 ).Get( name );
		AddObjectDefL( eId, objectParent, flags, name );
		getData.Column( 3 ).Free( );
		}
	CleanupStack::PopAndDestroy( &queryObject );
	getData.Reset();
	
	// RELATIONS
	RMdsStatement queryRelation;
	CleanupClosePushL( queryRelation );
	getData.AppendL( TColumn( eId ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	connection.ExecuteQueryL( KMdsQueryGetRelationDefs, queryRelation, namespaceNumber );
	while( connection.NextRowL( queryRelation, getData ) )
		{
		getData.Column( 0 ).Get( eId );
		getData.Column( 1 ).Get( name );
		AddRelationDefL( eId, name );
		getData.Column( 1 ).Free( );
		}
	CleanupStack::PopAndDestroy( &queryRelation );
	getData.Reset();
	
	// EVENTS
	TInt32 priority = 0;
	RMdsStatement queryEvent;
	CleanupClosePushL( queryEvent );
	getData.AppendL( TColumn( eId ) );
	getData.AppendL( TColumn( priority ) );
	getData.AppendL( TColumn( EColumnHBuf16 ) );
	connection.ExecuteQueryL( KMdsQueryGetEventDefs, queryEvent, namespaceNumber );
	while( connection.NextRowL( queryEvent, getData ) )
		{
		getData.Column( 0 ).Get( eId );
		getData.Column( 1 ).Get( priority );
		getData.Column( 2 ).Get( name );
		AddEventDefL( eId, name, priority );
		getData.Column( 2 ).Free( );
		}
	CleanupStack::PopAndDestroy( &queryEvent );

	CleanupStack::PopAndDestroy( 2, &namespaceNumber ); // getData, namespaceNumber
	
	const TInt count = iObjectDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		iObjectDefs[i]->ImportFromDBL();
		}
	iObjectDefs.Sort( TLinearOrder<CMdsObjectDef>(CMdsNamespaceDef::CompareObjectDefId) );

	// everything is ok, so set the flags
	SetStoredInDB();
	SetTableStoredInDB();
	}
	
TUint32 CMdsNamespaceDef::RequiredBufferSize()
	{
	TUint32 bufferSize = sizeof(TMdCNamespaceDef) + CMdsItemDef::RequiredBufferSize();

	const TInt objectDefsCount = iObjectDefs.Count();
	bufferSize += (objectDefsCount + 1) * sizeof( TMdCObjectDef);
	// base object def
	bufferSize += iBaseObject->RequiredBufferSize();

	// objectdefs
	for ( TInt i = 0; i < objectDefsCount; ++i )
		{
		bufferSize += iObjectDefs[i]->RequiredBufferSize();
		}

	// eventdefs
	const TInt eventDefsCount = iEventDefs.Count();
	bufferSize += eventDefsCount * sizeof( TMdCEventDef );
	for ( TInt i = 0; i < iEventDefs.Count(); ++i )
		{
		bufferSize += iEventDefs[i]->RequiredBufferSize();
		}

	// relationdefs
	const TInt relationDefsCount = iRelationDefs.Count();
	bufferSize += relationDefsCount * sizeof( TMdCRelationDef);
	for ( TInt i = 0; i < iRelationDefs.Count(); ++i )
		{
		bufferSize += iRelationDefs[i]->RequiredBufferSize();
		}

	return bufferSize;
	}

TMdCOffset CMdsNamespaceDef::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace)
	{
	const TMdCOffset namespaceDefOffset = aBuffer.Position();
	TMdCNamespaceDef namespaceDef;
	// get const data and store Name
	namespaceDef.iDefId = GetId();
	namespaceDef.iReadOnly = GetReadOnly();
	namespaceDef.iName.iPtr.iCount = GetName().Length();
	namespaceDef.iName.iPtr.iOffset = aFreeSpace;
	aBuffer.PositionL( aFreeSpace );
	aFreeSpace = CMdsItemDef::SerializeL( aBuffer );

	// calculate necessary stuff for objectDefs
	const TInt objectDefsCount = iObjectDefs.Count(); 
	namespaceDef.iObjectDefs.iPtr.iCount = objectDefsCount + 1; // one extra for base object def
	namespaceDef.iObjectDefs.iPtr.iOffset = aFreeSpace;

	// create space for objectDefs
	aFreeSpace += (objectDefsCount + 1) * sizeof(TMdCObjectDef);
	
	aBuffer.PositionL( namespaceDef.iObjectDefs.iPtr.iOffset ); // one extra for base object def
	aFreeSpace = iBaseObject->SerializeL( aBuffer, aFreeSpace );

	for ( TInt i = 0; i < objectDefsCount; ++i )
		{
		aBuffer.PositionL( namespaceDef.iObjectDefs.iPtr.iOffset
				+ (i+1) * sizeof(TMdCObjectDef) ); // one extra for base object def
		// write object
		aFreeSpace = iObjectDefs[i]->SerializeL( aBuffer, aFreeSpace );
		}

	// calculate necessary stuff for eventDefs
	const TInt eventDefsCount = iEventDefs.Count();
	namespaceDef.iEventDefs.iPtr.iCount = eventDefsCount;
	namespaceDef.iEventDefs.iPtr.iOffset = aFreeSpace;

	// create space for eventDefs
	aFreeSpace += eventDefsCount * sizeof(TMdCEventDef);

	for ( TInt i = 0; i < eventDefsCount; ++i )
		{
		aBuffer.PositionL( namespaceDef.iEventDefs.iPtr.iOffset + i * sizeof(TMdCEventDef) );
		// write event
		aFreeSpace = iEventDefs[i]->SerializeL( aBuffer, aFreeSpace );
		}

	// calculate necessary stuff for relationDefs
	const TInt relationDefsCount = iRelationDefs.Count();
	namespaceDef.iRelationDefs.iPtr.iCount = relationDefsCount;
	namespaceDef.iRelationDefs.iPtr.iOffset = aFreeSpace;

	// create space for eventDefs
	aFreeSpace += relationDefsCount * sizeof(TMdCRelationDef);

	for ( TInt i = 0; i < relationDefsCount; ++i )
		{
		aBuffer.PositionL( namespaceDef.iRelationDefs.iPtr.iOffset + i * sizeof(TMdCRelationDef) );
		// write relation
		aFreeSpace = iRelationDefs[i]->SerializeL( aBuffer, aFreeSpace );
		}
	
	// store namespaceDef itself
	aBuffer.PositionL( namespaceDefOffset );
	namespaceDef.SerializeL( aBuffer );

	return aFreeSpace;
	}
	

const CMdsPropertyDef* CMdsNamespaceDef::GetPropertyL( TDefId aId ) const
	{
	const CMdsPropertyDef* propertyDef = iBaseObject->GetPropertyByIdL( aId );

	if ( propertyDef )
		{
		return propertyDef;
		}

	const TInt count = iObjectDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		const CMdsPropertyDef* objectPropertyDef = iObjectDefs[i]->GetPropertyByIdL( aId );
		if ( objectPropertyDef )
			{
			return objectPropertyDef;
			}
		}

	return NULL;
	}

TInt CMdsNamespaceDef::CompareObjectDefId(const CMdsObjectDef& aFirst, const CMdsObjectDef& aSecond)
    {
    return aFirst.GetId() - aSecond.GetId();
    }

TInt CMdsNamespaceDef::CompareEventDefId(const CMdsEventDef& aFirst, const CMdsEventDef& aSecond)
    {
    return aFirst.GetId() - aSecond.GetId();
    }

TInt CMdsNamespaceDef::CompareRelationDefId(const CMdsRelationDef& aFirst, const CMdsRelationDef& aSecond)
    {
    return aFirst.GetId() - aSecond.GetId();
    }

/**
 * Initialize static variables
 */

CMdsObjectDef* CMdsNamespaceDef::iBaseObject = NULL;
