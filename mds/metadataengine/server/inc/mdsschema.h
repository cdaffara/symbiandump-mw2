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

#ifndef __MDSSCHEMA_H__
#define __MDSSCHEMA_H__

#include <e32base.h>
#include "mdccommon.h"

class CMdsNamespaceDef;
class CMdsObjectDef;
class CMdsClauseBuffer;



/**
 *  Metadata object class
 *
 *  This class is responsible for holding information about one object
 *
 */
class CMdsSchema : public CBase
    {
    friend class CMdsImportExport;

public:
	/**
	 * Create new objectdef object
	 * @param aId id of object
	 * @param aName name of object
	 * @param aParent parent to actual object
	 * @return pointer to new created object
	 */
    static CMdsSchema* NewL( );

	/**
	 * Create new objectdef object
	 * @param aId id of object
	 * @param aName name of object
	 * @param aParent parent to actual object
	 * @return pointer to new created object
	 */
    static CMdsSchema* NewLC( );

    virtual ~CMdsSchema();

    /**
     * Create tables to store whole Schema into DB
     */
    void StoreToDBL();
    
    /**
     * Check and return namespace searching by Id
     */
    const CMdsNamespaceDef* GetNamespaceByIdL( TDefId aId );

    /**
     * Check if namespace exists to avoid duplication
     * @param aName namespace name to search for
     * @return address of existing namespace or NULL
     */
	CMdsNamespaceDef* GetNamespace( const TDesC& aName );

    /**
     * Adds default BaseObject "Object"
     */
    void AddDefaultObjectL();

    /**
     * Adds default KMdEDefaultNameSpace 
     */
    void AddDefaultNamespaceL();

    /**
     * Returns NamespaceDefsCount  
     */
    TInt NamespaceDefsCount() const;

    /**
     * Returns reference to the NamespaceDefs  
     */
    const RPointerArray<CMdsNamespaceDef>& NamespaceDefs() const;

	void SerializeToSharedMemoryL();
	
	TInt SharedMemoryHandleL();

protected:

	/**
	 * Add object and it's parents properties names and types to clause
	 * parameters
	 * @param aObject object which properties should be add
	 * @param aClause clause where add instructions to sql execute
	 */
	void AddObjectToSqlClauseL( CMdsObjectDef* aObject, CMdsClauseBuffer* aClause,
	                            const TBool& aAddText );

    /**
     * Add namespace to array of existing ones
     * @param aName namespace name
     * @param aReadOnly namespace read-only flag
     * @param aNamespaceDefId namespaceDefd, or zero (none) to get automatic one
     * @return new added namespace
     */
	CMdsNamespaceDef* NamespaceAddL( const TDesC& aName, const TBool aReadOnly,
			TInt32 aVendorId, TDefId aNamespaceDefId = 0 );

	/**
	 * Combine two schemas and put it to one in aSchema
	 * @param aSchema schema where to add readed schema
	 */
    void MergeNamespacesL( CMdsSchema& aSchema );

	void MergeElementsL( CMdsSchema& aSchema, const TBool& aDryRun );

    /**
     * Create tables to store whole Schema into DB (internal)
     */
    void StoreSchemaToDBL();

	void StoreNamespacesAndBaseObjectL();

	void CreateCol2PropTableL();

	void AddCol2PropObjectL( CMdsObjectDef* aObject, CMdsObjectDef* aChildObject,
							 TUint32& aPropertyColumnCounter, TBool aText );

    /**
     * Create tables for objects
     */
    void CreateObjectTablesL();

private:
    inline CMdsSchema();

	/**
	 * 2nd phase constructor
	 * @param aName name of property
	 */
    void ConstructL();

	/**
	 * Deletes all namespaces and clean it's array
	 */
	void Reset();
	
	/**
	 * Creates property index for a object table.
	 */
	void CreatePropertyIndexL( const TDesC& aPropertyName, const TDesC& aTableName, TDefId aNamespaceId );
	
	class TPropertyInfo
		{
	public:
		TPropertyInfo( const TDesC& aTableName, const TDesC& aPropertyName, TDefId aNamespaceId ) :
			iTableName( aTableName ), iPropertyName( aPropertyName )
		{
		iNamespaceId = aNamespaceId;
		}
		
		TPtrC iTableName;
		TPtrC iPropertyName;
		TDefId iNamespaceId;
		};

private: // data
    /**
     * Store every namespace read from schema file.
     */
	RPointerArray<CMdsNamespaceDef> iNamespaceDefs;
	
	RArray<TPropertyInfo> iProperties;

	/**
	 * Default base object, parent to all others
	 */
	CMdsObjectDef* iBaseObject;
	
	RChunk iSerializedSchema;
	
	TBool iSerialized;
	
	TInt iSchemaHandle;
    };

#endif // __MDSSCHEMA_H__
