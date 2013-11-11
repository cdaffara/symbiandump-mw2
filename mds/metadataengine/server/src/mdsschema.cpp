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
* Description:  Class to hold description about schema
*
*/

#include <badesca.h>
#include "mdsschema.h"

#include "mdcdef.h"
#include "mdsnamespacedef.h"
#include "mdsobjectdef.h"
#include "mdsclausebuffer.h"
#include "mdsfindsqlclausedef.h"
#include "mdspropertydef.h"
#include "mdsdbconnectionpool.h"
#include "mdssqliteconnection.h"
#include "mdcserializationbuffer.h"
#include <mdeconstants.h>


/**
 * NewLC
 */
CMdsSchema* CMdsSchema::NewLC( )
	{
	CMdsSchema* ret = new( ELeave ) CMdsSchema( );
	CleanupStack::PushL( ret );
	ret->ConstructL( );
	return ret;
	}
	
/**
 * NewL
 */
CMdsSchema* CMdsSchema::NewL( )
	{
	CMdsSchema* ret = CMdsSchema::NewLC( );
	CleanupStack::Pop( ret );
	return ret;
	}

/**
 * ConstructL
 */
void CMdsSchema::ConstructL( )
	{
	}

/**
 * Constructor
 */
CMdsSchema::CMdsSchema() : iBaseObject( NULL ),
        iSerialized( EFalse ), iSchemaHandle( 0 )
	{
	}

/**
 * Destructor
 */
CMdsSchema::~CMdsSchema()
	{
	iNamespaceDefs.ResetAndDestroy();
	iNamespaceDefs.Close();

	iSerializedSchema.Close();
	iProperties.Close();

	delete iBaseObject;
	iBaseObject = NULL;
	}

void CMdsSchema::Reset()
	{
	// deleting namespaces
	iNamespaceDefs.ResetAndDestroy();
	}

void CMdsSchema::AddDefaultObjectL()
	{
	
	iBaseObject = CMdsObjectDef::NewL( MdeConstants::Object::KBaseObject, NULL );
	iBaseObject->SetId(KBaseObjectDefId);
	
	// name, type, min, max, read-only, mandatory, indexed
	iBaseObject->AddPropertyL( MdeConstants::Object::KOriginProperty, EPropertyUint8, TInt32(0), TInt32(KMaxTUint8), EFalse, EFalse, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KSizeProperty, EPropertyUint32, TUint32(0), KMaxTUint32, EFalse, EFalse, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KTimeOffsetProperty, EPropertyInt16, TInt32(KMinTInt16), TInt32(KMaxTInt16), ETrue, EFalse, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KCreationDateProperty, EPropertyTime, KMinTInt64, KMaxTInt64, EFalse, ETrue, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KLastModifiedDateProperty, EPropertyTime, KMinTInt64, KMaxTInt64, EFalse, ETrue, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KItemTypeProperty, EPropertyText, TInt32(0), TInt32(255), EFalse, EFalse, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KTitleProperty, EPropertyText, TInt32(0), TInt32(255), EFalse, EFalse, EFalse );
	iBaseObject->AddPropertyL( MdeConstants::Object::KInDefaultFolder, EPropertyBool, TInt32(0), TInt32(1), EFalse, EFalse, EFalse );
	
	TInt propcount = iBaseObject->GetPropertiesCount();
	iBaseObject->iCol2Prop.Reset();
	
	for (TInt i = 0 ; i < propcount; i++)
		{
		CMdsPropertyDef* propertyDef = iBaseObject->GetProperty(i);
		propertyDef->SetId(i+1);
		CMdsObjectDef::TMdsColumnOrder col = 
			{ i + KBaseObjectBasicValueColumnOffset, *propertyDef };
		iBaseObject->iCol2Prop.AppendL( col );
		if( propertyDef->GetMandatory() )
			{
			++iBaseObject->iMandatoryPropertyCount;
			}
		}
	}

void CMdsSchema::AddDefaultNamespaceL()
	{
	NamespaceAddL( KMdEDefaultNameSpace, EFalse, KNokiaVendorId, KDefaultNamespaceDefId );
	}

CMdsNamespaceDef* CMdsSchema::NamespaceAddL( const TDesC& aName, const TBool aReadOnly, TInt32 aVendorId, TDefId aNamespaceDefId )
	{
	CMdsNamespaceDef* newNamespace = CMdsNamespaceDef::NewLC( aNamespaceDefId, aName, aReadOnly, aVendorId );
	newNamespace->SetBaseObject( iBaseObject );
	iNamespaceDefs.AppendL( newNamespace );
	CleanupStack::Pop( newNamespace );
	return newNamespace;
	}

CMdsNamespaceDef* CMdsSchema::GetNamespace( const TDesC& aName )
	{
	const TInt count = iNamespaceDefs.Count();
	
	for( TInt i = 0; i < count; ++i )
		{
		if ( iNamespaceDefs[i]->GetName().Compare( aName ) == 0 )
			{
			return iNamespaceDefs[i];
			}
		}
	return NULL;
	}

const CMdsNamespaceDef* CMdsSchema::GetNamespaceByIdL( TDefId aId )
	{
	const TInt namespaceCount = iNamespaceDefs.Count();
	for ( TInt i = 0; i < namespaceCount; ++i )
		{
		if( iNamespaceDefs[i]->GetId() == aId )
			{
			return iNamespaceDefs[i];
			}
		}
	return NULL;
	}

TInt CMdsSchema::NamespaceDefsCount() const
	{
	return iNamespaceDefs.Count();
	}

const RPointerArray<CMdsNamespaceDef>& CMdsSchema::NamespaceDefs() const
	{
	return iNamespaceDefs;
	}

void CMdsSchema::AddObjectToSqlClauseL( CMdsObjectDef* aObject, CMdsClauseBuffer* aClause,
                                        const TBool& aAddText )
	{
	if ( !aObject->iParent )
		{
		return;
		}
	AddObjectToSqlClauseL( (CMdsObjectDef*)aObject->iParent, aClause, aAddText);

	const TInt objectPropertyCount = aObject->iPropertyDefs.Count();
	for ( TInt i = 0; i < objectPropertyCount; ++i )
		{
		CMdsPropertyDef* propertyDef = aObject->iPropertyDefs[i];
		// to set up text columns as a last one
		if ( propertyDef->GetType() == EPropertyText && aAddText == EFalse )
			{
			continue;
			}
		if ( propertyDef->GetType() != EPropertyText && aAddText )
			{
			continue;
			}
		// end

		// make object table definition
		aClause->AppendL( propertyDef->GetName() );
		aClause->AppendL( KSpace );
		aClause->AppendL( propertyDef->GetSqlTypeName() );
		aClause->AppendL( KComma );
		}
	}

void CMdsSchema::CreateObjectTablesL()
	{
	_LIT( KBaseObjectDefinition,  "CREATE TABLE IF NOT EXISTS %S%u(ObjectId INTEGER PRIMARY KEY AUTOINCREMENT,ObjectDefId INTEGER NOT NULL,Flags INTEGER,MediaId LARGEINT,UsageCount LARGEINT DEFAULT 0,GuidHigh LARGEINT,GuidLow LARGEINT,URI TEXT NOT NULL COLLATE NOCASE,Origin INTEGER,Size LARGEINT,TimeOffset INTEGER,CreationDate LARGEINT,LastModifiedDate LARGEINT,ItemType TEXT,Title TEXT,InDefaultFolder INTEGER,UNIQUE(GuidHigh,GuidLow),UNIQUE(URI,MediaId));" );	
	_LIT( KCreateRelationsTable,  "CREATE TABLE IF NOT EXISTS Relations%u(RelationId INTEGER PRIMARY KEY AUTOINCREMENT,Flags INTEGER,RelationDefId INTEGER NOT NULL,LeftObjectId INTEGER NOT NULL,RightObjectId INTEGER NOT NULL,Parameter INTEGER,GuidHigh LARGEINT,GuidLow LARGEINT,LastModifiedDate LARGEINT);" );
	_LIT( KCreateEventsTable,     "CREATE TABLE IF NOT EXISTS Event%u(EventId INTEGER PRIMARY KEY AUTOINCREMENT,ObjectId INTEGER NOT NULL, EventDefId INTEGER NOT NULL, Timestamp INTEGER NOT NULL, Source TEXT, Participant TEXT);" );
	_LIT( KCreateTxtSrchTable,    "CREATE TABLE IF NOT EXISTS TextSearch%u(WordId INTEGER NOT NULL,ObjectId INTEGER NOT NULL,Position INTEGER);" );
	_LIT( KCreateTxtSrchDicTable, "CREATE TABLE IF NOT EXISTS TextSearchDictionary%u(WordId INTEGER PRIMARY KEY,Word TEXT NOT NULL);" );

	_LIT( KMdsSqlClauseObjTableBeg, "CREATE TABLE IF NOT EXISTS %S%u(ObjectId INTEGER NOT NULL," );
	_LIT( KMdsSqlClauseObjTableEnd, " UNIQUE(ObjectId));" );

    // Triggers
	_LIT( KMdsTriggerEvent,      "CREATE TRIGGER DeleteEvent%u BEFORE DELETE ON Object%u BEGIN DELETE FROM Event%u WHERE ObjectId=OLD.ObjectId; END;" );
	_LIT( KMdsTriggerRelation,   "CREATE TRIGGER DeleteRelations%u BEFORE DELETE ON Object%u BEGIN DELETE FROM Relations%u WHERE LeftObjectId=OLD.ObjectId OR RightObjectId=OLD.ObjectId; END;" );
    _LIT( KMdsTriggerTextSearch, "CREATE TRIGGER DeleteTextSearch%u BEFORE DELETE ON Object%u BEGIN DELETE FROM TextSearch%u WHERE ObjectId=OLD.ObjectId; END;" );	

	// UsageCount relation triggers
	_LIT( KMdsTriggerInsertRelation, "CREATE TRIGGER InsertRelations%u INSERT ON Relations%u BEGIN UPDATE Object%u SET UsageCount = UsageCount + 1 WHERE ObjectId=NEW.LeftObjectId OR ObjectId=NEW.RightObjectId; END;" );
	_LIT( KMdsTriggerUpdateRelation, "CREATE TRIGGER UpdateRelations%u UPDATE OF LeftObjectId, RightObjectId ON Relations%u WHEN OLD.Flags=NEW.Flags BEGIN UPDATE Object%u SET UsageCount = UsageCount - 1 WHERE ObjectId=OLD.LeftObjectId OR ObjectId=OLD.RightObjectId; UPDATE Object1 SET UsageCount = UsageCount + 1 WHERE ObjectId=NEW.LeftObjectId OR ObjectId=NEW.RightObjectId; END;" );
	_LIT( KMdsTriggerUpdateNotRemovedRelation, "CREATE TRIGGER UpdateNotRemovedRelations%u UPDATE OF Flags ON Relations%u WHEN OLD.Flags!=NEW.Flags AND NOT NEW.Flags&%u BEGIN UPDATE Object%u SET UsageCount = UsageCount + 1 WHERE ObjectId=NEW.LeftObjectId OR ObjectId=NEW.RightObjectId; END;" );
	_LIT( KMdsTriggerUpdateRemovedRelation,    "CREATE TRIGGER UpdateRemovedRelations%u UPDATE OF Flags ON Relations%u WHEN OLD.Flags!=NEW.Flags AND NEW.Flags&%u AND NOT OLD.Flags&%u BEGIN UPDATE Object%u SET UsageCount = UsageCount - 1 WHERE ObjectId=OLD.LeftObjectId OR ObjectId=OLD.RightObjectId; END;" );

	// Relation indexes
	_LIT( KMdsIndexRelationLeftObjectId,  "CREATE INDEX RelationsLeftObjectIdIndex%u ON Relations%u(LeftObjectId);" );
	_LIT( KMdsIndexRelationRightObjectId, "CREATE INDEX RelationsRightObjectIdIndex%u ON Relations%u(RightObjectId);" );

	// freetext indexes
	_LIT( KMdsIndexFreetextWordId,   "CREATE INDEX FreetextWordIdIndex%u ON TextSearch%u(WordId);" );
	_LIT( KMdsIndexFreetextObjectId, "CREATE INDEX FreetextObjectIdIndex%u ON TextSearch%u(ObjectId);" );
	
	// ObjectTypeDefinition index
	_LIT( KMdsIndexObjectDefId, "CREATE INDEX ObjectDefIndex%u ON Object%u(ObjectDefId);" );
	
    // MediaId index
    _LIT( KMdsIndexMediaId, "CREATE INDEX MediaIdIndex%u ON Object%u(MediaId);" );

	// create tables
	CMdsClauseBuffer* clause = CMdsClauseBuffer::NewLC( 1024 );
	clause->ReserveSpaceL( KBaseObjectDefinition.iTypeLength + 30 ); // maxIntNumberDecimal.Length = 10
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );

	// for triggers
	CMdsClauseBuffer* clauseTrigger = CMdsClauseBuffer::NewLC( 1024 );
	
	const TInt namespaceDefCount = iNamespaceDefs.Count();
	
	// add objectdef[number] to DB
	for( TInt i = 0; i < namespaceDefCount; ++i )
		{
		CMdsNamespaceDef* namespaceDef = iNamespaceDefs[i];
		const TDefId namespaceDefId = namespaceDef->GetId();

		if (!namespaceDef->GetTableStoredInDB())
			{
			// add base objectdef[number]
			clause->BufferL().Format( KBaseObjectDefinition, &MdeConstants::Object::KBaseObject, namespaceDefId );
			connection.ExecuteL( clause->ConstBufferL(), emptyRowData );			

			// add relations[number]
			clause->BufferL().Format( KCreateRelationsTable, namespaceDefId );
			TRAPD( errRel, connection.ExecuteL( clause->ConstBufferL(), emptyRowData ) );
	        if( errRel == KErrNone )
	            {
	            clauseTrigger->ReserveSpaceL( KMdsTriggerRelation().Size() + ( KMaxUintValueLength * 3 ) );  
	            clauseTrigger->BufferL().Format( KMdsTriggerRelation, namespaceDefId, namespaceDefId, namespaceDefId );
	            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
	            }

			// add event[number]
			clause->BufferL().Format( KCreateEventsTable, namespaceDefId );
			TRAPD( errEve, connection.ExecuteL( clause->ConstBufferL(), emptyRowData ) );
	        if( errEve == KErrNone )
	            {
	            clauseTrigger->ReserveSpaceL( KMdsTriggerEvent().Size() + ( KMaxUintValueLength * 3 ) );  
	            clauseTrigger->BufferL().Format( KMdsTriggerEvent, namespaceDefId, namespaceDefId, namespaceDefId );
	            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
	            }

			// add textsearch[number]
			clause->BufferL().Format( KCreateTxtSrchTable, namespaceDefId );
			TRAPD( errTS, connection.ExecuteL( clause->ConstBufferL(), emptyRowData ) );
	        if( errTS == KErrNone )
	            {
	            clauseTrigger->ReserveSpaceL( KMdsTriggerTextSearch().Size() + ( KMaxUintValueLength * 3 ) );  
	            clauseTrigger->BufferL().Format( KMdsTriggerTextSearch, namespaceDefId, namespaceDefId, namespaceDefId );
	            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

				// add KMdsIndexFreetextWordId[number]
	            clauseTrigger->ReserveSpaceL( KMdsIndexFreetextWordId().Size() + ( KMaxUintValueLength * 2 ) );  
	            clauseTrigger->BufferL().Format( KMdsIndexFreetextWordId, namespaceDefId, namespaceDefId );
	            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
				
				// add KMdsIndexFreetextObjectId[number]
	            clauseTrigger->ReserveSpaceL( KMdsIndexFreetextObjectId().Size() + ( KMaxUintValueLength * 2 ) );  
	            clauseTrigger->BufferL().Format( KMdsIndexFreetextObjectId, namespaceDefId, namespaceDefId );
	            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
	            }

			// add textsearchdictionary[number]
			clause->BufferL().Format( KCreateTxtSrchDicTable, namespaceDefId );
			TRAP_IGNORE( connection.ExecuteL( clause->ConstBufferL(), emptyRowData ) );
			
			// add insertrelations[number]
            clauseTrigger->ReserveSpaceL( KMdsTriggerInsertRelation().Size() + ( KMaxUintValueLength * 3 ) );  
            clauseTrigger->BufferL().Format( KMdsTriggerInsertRelation, namespaceDefId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

			// add updaterelations[number]
            clauseTrigger->ReserveSpaceL( KMdsTriggerUpdateRelation().Size() + ( KMaxUintValueLength * 3 ) );  
            clauseTrigger->BufferL().Format( KMdsTriggerUpdateRelation, namespaceDefId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

			// add updatenotremovedrelations[number]
            clauseTrigger->ReserveSpaceL( KMdsTriggerUpdateNotRemovedRelation().Size() + ( KMaxUintValueLength * 4 ) );  
            clauseTrigger->BufferL().Format( KMdsTriggerUpdateNotRemovedRelation, namespaceDefId, namespaceDefId, 
                    EMdERelationFlagNotPresent | EMdERelationFlagDeleted, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

			// add updateremovedrelations[number]
            clauseTrigger->ReserveSpaceL( KMdsTriggerUpdateRemovedRelation().Size() + ( KMaxUintValueLength * 5 ) );  
            clauseTrigger->BufferL().Format( KMdsTriggerUpdateRemovedRelation, namespaceDefId, namespaceDefId, 
                    EMdERelationFlagNotPresent | EMdERelationFlagDeleted, EMdERelationFlagNotPresent | EMdERelationFlagDeleted, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

			// add relationleftobjectidindex[number]
            clauseTrigger->ReserveSpaceL( KMdsIndexRelationLeftObjectId().Size() + ( KMaxUintValueLength * 2 ) );  
            clauseTrigger->BufferL().Format( KMdsIndexRelationLeftObjectId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

			// add relationrightobjectidindex[number]
            clauseTrigger->ReserveSpaceL( KMdsIndexRelationRightObjectId().Size() + ( KMaxUintValueLength * 2 ) );  
            clauseTrigger->BufferL().Format( KMdsIndexRelationRightObjectId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );

            // add objectdefidindex[number]
            clauseTrigger->ReserveSpaceL( KMdsIndexObjectDefId().Size() + ( KMaxUintValueLength * 2 ) );  
            clauseTrigger->BufferL().Format( KMdsIndexObjectDefId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
            
            // add mediaidindex[number]
            clauseTrigger->ReserveSpaceL( KMdsIndexMediaId().Size() + ( KMaxUintValueLength * 2 ) );  
            clauseTrigger->BufferL().Format( KMdsIndexMediaId, namespaceDefId, namespaceDefId );
            connection.ExecuteL( clauseTrigger->ConstBufferL(), emptyRowData );
            
			namespaceDef->SetTableStoredInDB();
			}

		const TInt objectDefCount = namespaceDef->iObjectDefs.Count();
		
		for ( TInt j = 0; j < objectDefCount; ++j )
			{
			CMdsObjectDef* objectDef = namespaceDef->iObjectDefs[j];

			if (!objectDef->GetTableStoredInDB())
				{
				clause->BufferL().Format( KMdsSqlClauseObjTableBeg, &objectDef->GetName(), namespaceDef->GetId() );

				// add no text properties
				AddObjectToSqlClauseL( objectDef, clause, EFalse );
				// add text properties at end
				AddObjectToSqlClauseL( objectDef, clause, ETrue );

				clause->AppendL( KMdsSqlClauseObjTableEnd );
				connection.ExecuteL( clause->ConstBufferL(), emptyRowData );

				objectDef->SetTableStoredInDB();
				}

			const TInt objectPropertyCount = objectDef->iPropertyDefs.Count();
			for ( TInt k = 0; k < objectPropertyCount; ++k )
				{
				CMdsPropertyDef* propertyDef = objectDef->iPropertyDefs[k];
				
				if ( propertyDef->GetIndexed() )
					{
					iProperties.AppendL( TPropertyInfo( objectDef->GetName(), 
							propertyDef->GetName(), namespaceDef->GetId() ) );
					}
				}
			}
        iProperties.AppendL( TPropertyInfo( MdeConstants::Object::KBaseObject, MdeConstants::Object::KItemTypeProperty, namespaceDef->GetId() ) );
		}

	CleanupStack::PopAndDestroy( 3, clause ); // clauseTrigger, emptyRowData, clause
	}

void CMdsSchema::CreatePropertyIndexL( const TDesC& aPropertyName, const TDesC& aTableName, 
		TDefId aNamespaceId )
	{
	_LIT( KMdsPropertyIndexCreate, "CREATE INDEX %S%SIndex%u ON %S%u(%S);" );
	
	const TUint clauseLength = KMdsPropertyIndexCreate.iTypeLength + aTableName.Length() * 2 +
		+ aPropertyName.Length() * 2 + KMaxUintValueLength * 2;
	CMdsClauseBuffer* clause = CMdsClauseBuffer::NewLC( clauseLength );
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );
    clause->BufferL().Format( KMdsPropertyIndexCreate, &aTableName, &aPropertyName, aNamespaceId,
    		&aTableName, aNamespaceId, &aPropertyName );
    // Ignore possible SQL error, MDS can still live without this index
	TRAP_IGNORE( connection.ExecuteL( clause->ConstBufferL(), emptyRowData ) );
	CleanupStack::PopAndDestroy( 2, clause ); // emptyRowData, clause
	}

void CMdsSchema::StoreToDBL()
	{
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
    RMdSTransaction transaction( connection );
    CleanupClosePushL( transaction );
    User::LeaveIfError( transaction.Error() );

	StoreNamespacesAndBaseObjectL();
    CreateObjectTablesL();
	StoreSchemaToDBL();
	CreateCol2PropTableL();
    
    TUint propertyCount = iProperties.Count();
    for ( TUint i = 0; i < propertyCount; ++i )
    	{
    	TPropertyInfo& propertyInfo = iProperties[i];
    	CreatePropertyIndexL( propertyInfo.iPropertyName, propertyInfo.iTableName,
    			propertyInfo.iNamespaceId );
    	}
    
    transaction.CommitL();
    CleanupStack::PopAndDestroy( &transaction );
    
    iProperties.Reset();
	}

void CMdsSchema::AddCol2PropObjectL( CMdsObjectDef* aObject, CMdsObjectDef* aChildObject,
									 TUint32& aPropertyColumnCounter, TBool aText )
	{
	_LIT( KMdsSqlClauseAddCol2Prop, "INSERT INTO Col2Prop(ObjectDefId,PropertyDefId,ColumnId) Values(?,?,?);" );
	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	if ( !aChildObject->iParent )
		{
		if (aText)
			{
			return;
			}
		RRowData rowData;
		CleanupClosePushL( rowData );
		const TInt childObjectPropertyCount = aChildObject->iPropertyDefs.Count();
		for ( TInt i = 0; i < childObjectPropertyCount; ++i )
			{
			CMdsPropertyDef* propertyDef = aChildObject->iPropertyDefs[i];
			CMdsObjectDef::TMdsColumnOrder col = { aChildObject->iCol2Prop[i].iColumnId, *propertyDef };
			aObject->iCol2Prop.AppendL( col );
			
			if( propertyDef->GetMandatory() )
				{
				++aObject->iMandatoryPropertyCount;
				}
			
			++aPropertyColumnCounter;
			rowData.Reset();
			rowData.AppendL( TColumn( aObject->GetId() ) );
			rowData.AppendL( TColumn( propertyDef->GetId() ) );
			rowData.AppendL( TColumn( col.iColumnId ) );
			connection.ExecuteL( KMdsSqlClauseAddCol2Prop, rowData );
			}
		CleanupStack::PopAndDestroy( &rowData );
		return;
		}

	AddCol2PropObjectL( aObject, (CMdsObjectDef*)aChildObject->iParent, aPropertyColumnCounter, aText );

	// if parent is baseobject
	if ( !aText && !aChildObject->iParent->iParent )
		{
		++aPropertyColumnCounter;
		}

	RRowData rowData;
	CleanupClosePushL( rowData );

	const TInt childObjectPropertyCount = aChildObject->iPropertyDefs.Count();
	for ( TInt i = 0; i < childObjectPropertyCount; ++i )
		{
		CMdsPropertyDef* propertyDef = aChildObject->iPropertyDefs[i];
		CMdsObjectDef::TMdsColumnOrder col = { 0, *propertyDef };
		if (  aText && propertyDef->GetType() != EPropertyText ||
			 !aText && propertyDef->GetType() == EPropertyText )
			{
			continue;
			}

		col.iColumnId = aPropertyColumnCounter++;

		aObject->iCol2Prop.AppendL( col );

		if( propertyDef->GetMandatory() )
			{
			++aObject->iMandatoryPropertyCount;
			}

		rowData.Reset();
		rowData.AppendL( TColumn( aObject->GetId() ) );
		rowData.AppendL( TColumn( propertyDef->GetId() ) );
		rowData.AppendL( TColumn( col.iColumnId ) );
		connection.ExecuteL( KMdsSqlClauseAddCol2Prop, rowData );
		}

	CleanupStack::PopAndDestroy( &rowData );
	}

void CMdsSchema::CreateCol2PropTableL()
	{
	_LIT( KMdsSqlClauseCol2Prop,       "CREATE TABLE IF NOT EXISTS Col2Prop(ObjectDefId INTEGER NOT NULL,PropertyDefId INTEGER NOT NULL,ColumnId INTEGER NOT NULL);" );

    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
	// create binding column <--> property table
	connection.ExecuteL( KMdsSqlClauseCol2Prop, emptyRowData );
	TUint32 propertyColumnCounter;

	const TInt count = iNamespaceDefs.Count();
	
	for( TInt i = 0; i < count; ++i )
		{
		CMdsNamespaceDef* namespaceDef = iNamespaceDefs[i];
		const TInt namespaceObjectCount = namespaceDef->iObjectDefs.Count();
		for ( TInt j = 0; j < namespaceObjectCount; ++j )
			{
			CMdsObjectDef* objectDef = namespaceDef->iObjectDefs[j];
			if (!objectDef->GetC2PStoredInDB())
				{
				propertyColumnCounter = KBaseObjectBasicValueColumnOffset;

				objectDef->iCol2Prop.Reset();
				objectDef->iMandatoryPropertyCount = 0;

				AddCol2PropObjectL( objectDef, objectDef, propertyColumnCounter, EFalse );
				AddCol2PropObjectL( objectDef, objectDef, propertyColumnCounter, ETrue );
				objectDef->SetC2PStoredInDB();
				}
			}
		}
	CleanupStack::PopAndDestroy( &emptyRowData );
	}

void CMdsSchema::StoreNamespacesAndBaseObjectL()
	{
	_LIT( KMdsSqlClauseNamespaceDef, "CREATE TABLE IF NOT EXISTS NamespaceDef(NamespaceDefId INTEGER PRIMARY KEY,ReadOnly INTEGER,VendorId INTEGER,Name TEXT);" );
	_LIT( KMdsSqlClauseObjectDef,    "CREATE TABLE IF NOT EXISTS ObjectDef(ObjectDefId INTEGER PRIMARY KEY,NamespaceDefId INTEGER NOT NULL,ParentDefId INTEGER NOT NULL,Flags INTEGER,Name TEXT);" );
	_LIT( KMdsSqlClausePropertyDef,  "CREATE TABLE IF NOT EXISTS PropertyDef(PropertyDefId INTEGER PRIMARY KEY,ObjectDefId INTEGER NOT NULL,Flags INTEGER,Type INTEGER,MinValue NUMERIC,MaxValue NUMERIC,Name TEXT);" );
	_LIT( KMdsSqlClauseRelationDef,  "CREATE TABLE IF NOT EXISTS RelationDef(RelationDefId INTEGER PRIMARY KEY,NamespaceDefId INTEGER NOT NULL,Name TEXT);" );
	_LIT( KMdsSqlClauseEventDef,     "CREATE TABLE IF NOT EXISTS EventDef(EventDefId INTEGER PRIMARY KEY,NamespaceDefId INTEGER NOT NULL,Priority INTEGER,Name TEXT);" );

    RRowData emptyRowData;
    CleanupClosePushL( emptyRowData );

	// create tables
	const TUint KNumClauses = 5;

	CDesCArrayFlat* descarray = new (ELeave) CDesCArrayFlat(KNumClauses);
	CleanupStack::PushL(descarray);
	descarray->AppendL(KMdsSqlClauseNamespaceDef);
	descarray->AppendL(KMdsSqlClauseObjectDef);
	descarray->AppendL(KMdsSqlClausePropertyDef);
	descarray->AppendL(KMdsSqlClauseRelationDef);
	descarray->AppendL(KMdsSqlClauseEventDef);

	CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

	TInt error;
	for (TUint i = 0; i < KNumClauses; i++)
		{
		TRAP( error,connection.ExecuteL( (*descarray)[i], emptyRowData ) );
		if ( error != KErrNone )
			{
			User::Leave( error );
			}
		}
	descarray->Reset();
	CleanupStack::PopAndDestroy( descarray );

	const TInt count = iNamespaceDefs.Count();
	
    RMdSTransaction transaction( connection );
    CleanupClosePushL(transaction);
    const TInt beginError( transaction.Error() );
    if( beginError != KErrNone )
        {
        CleanupStack::PopAndDestroy( &transaction );
        }
	
	// add only namespaceDef to DB
	for( TInt i = 0; i < count; ++i )
		{
		iNamespaceDefs[i]->StoreToDBL( ETrue );
		}

    if( beginError == KErrNone )
        {
        transaction.CommitL();
        CleanupStack::PopAndDestroy( &transaction );
        }	

	CleanupStack::PopAndDestroy( &emptyRowData );
	}

void CMdsSchema::StoreSchemaToDBL()
	{
	_LIT( KMdsSqlClauseDeleteBoFromOd,  "DELETE FROM ObjectDef WHERE ObjectDefId=?;" );
	_LIT( KMdsSqlClauseDeleteBoFromPd,  "DELETE FROM PropertyDef WHERE ObjectDefId=?;" );

    CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();

    RMdSTransaction transaction( connection );
    CleanupClosePushL(transaction);
    const TInt beginError( transaction.Error() );
    if( beginError != KErrNone )
        {
        CleanupStack::PopAndDestroy( &transaction );
        }
    
	iBaseObject->StoreToDBL( KNoDefId );
	
	const TInt count = iNamespaceDefs.Count();
	
	// add namespaceDef to DB
	for( TInt i = 0; i < count; ++i )
		{
		iNamespaceDefs[i]->StoreToDBL();
		}

    if( beginError == KErrNone )
        {
        transaction.CommitL();
        CleanupStack::PopAndDestroy( &transaction );
        }   
	
	RRowData removeBo;
	CleanupClosePushL( removeBo );
	removeBo.AppendL( TColumn( KBaseObjectDefId ) );

	TRAPD( err, connection.ExecuteL( KMdsSqlClauseDeleteBoFromOd,  removeBo ) );
	TRAP ( err, connection.ExecuteL( KMdsSqlClauseDeleteBoFromPd,  removeBo ) );
	
	CleanupStack::PopAndDestroy( &removeBo );	
	}

void CMdsSchema::MergeElementsL( CMdsSchema& aSchema, const TBool& aDryRun )
	{
	const TInt count = aSchema.iNamespaceDefs.Count();
	
	for ( TInt i = 0; i < count; ++i )
		{
		CMdsNamespaceDef* lNamespace = NULL;
		CMdsNamespaceDef* rNamespace = aSchema.iNamespaceDefs[i];
		// reseting after namespace read from file
		rNamespace->UnsetFirstRead();
		// search for namespace
        lNamespace = GetNamespace( rNamespace->GetName() );
		if ( !lNamespace )
			{
			if ( !aDryRun )
				{
				// just add pointer to new schema and remove it from old one
				iNamespaceDefs.AppendL( rNamespace );
				aSchema.iNamespaceDefs[i] = NULL;
				rNamespace->SetAllNotStoredInDB();
				}
			}
		else
			{
			// copy appropriate elements
			if ( lNamespace->GetVendorId() == rNamespace->GetVendorId() && !lNamespace->GetReadOnly() )
				{
				lNamespace->MergeObjectsL( rNamespace, aDryRun );
				lNamespace->MergeEventsL( rNamespace, aDryRun );
				lNamespace->MergeRelationsL( rNamespace, aDryRun );
				}
			else
				{
				User::Leave( KErrAccessDenied );
				}
			}
		}
	}

void CMdsSchema::MergeNamespacesL( CMdsSchema& aSchema )
	{
	if ( iNamespaceDefs.Count() == 0 )
		{// speed optimization
		Reset();
		
		const TInt count = aSchema.iNamespaceDefs.Count();
		
		for ( TInt i = 0; i < count; ++i )
			{
			// reseting after namespace read from file
			aSchema.iNamespaceDefs[i]->UnsetFirstRead();
			iNamespaceDefs.AppendL( aSchema.iNamespaceDefs[i] );
			aSchema.iNamespaceDefs[i] = NULL;
			}
		aSchema.Reset();
		}
	else
		{
		MergeElementsL( aSchema, ETrue );
		MergeElementsL( aSchema, EFalse );
		}
	}

void CMdsSchema::SerializeToSharedMemoryL()
	{
	const TInt namespaceCount = iNamespaceDefs.Count();

	// namespace count, namespace offsets
	TUint32 bufferSize = sizeof(TMdCSchema) + namespaceCount * sizeof(TMdCNamespaceDef);

	for ( TInt i = 0; i < namespaceCount; ++i )
		{
		bufferSize += iNamespaceDefs[i]->RequiredBufferSize();
		}

	// create shared memory chunk
	iSerialized = EFalse;
	iSerializedSchema.Close();
	++iSchemaHandle;
	HBufC* name = HBufC::NewLC( KSchemaChunkName.iTypeLength + KMaxUintValueLength );
	*name = KSchemaChunkName;
	name->Des().AppendNum( iSchemaHandle );

	TInt error = KErrNone;
	while( (error = iSerializedSchema.CreateGlobal( *name, bufferSize, bufferSize )) == KErrAlreadyExists )
		{
		++iSchemaHandle;
		*name = KSchemaChunkName;
		name->Des().AppendNum( iSchemaHandle );
		}
	User::LeaveIfError( error );

	CMdCSerializationBuffer* buffer = CMdCSerializationBuffer::NewLC( iSerializedSchema.Base(), bufferSize );

	TMdCSchema schema;
	schema.iNamespaceDefs.iPtr.iCount = namespaceCount;
	schema.iNamespaceDefs.iPtr.iOffset = sizeof(schema);
	schema.SerializeL( *buffer );

	TMdCOffset freeSpace = schema.iNamespaceDefs.iPtr.iOffset + namespaceCount * sizeof(TMdCNamespaceDef);
	for ( TInt i = 0; i < namespaceCount; ++i )
		{
		buffer->PositionL( schema.iNamespaceDefs.iPtr.iOffset + i * sizeof(TMdCNamespaceDef) );

		// write namespace
		freeSpace = iNamespaceDefs[i]->SerializeL( *buffer, freeSpace );
		}

	CleanupStack::PopAndDestroy( buffer );
	CleanupStack::PopAndDestroy( name );
	iSerialized = ETrue;
	}
	
TInt CMdsSchema::SharedMemoryHandleL()
	{
	if ( !iSerialized )
		{
		User::Leave( KErrNotReady );
		}
	return iSchemaHandle;
	}

