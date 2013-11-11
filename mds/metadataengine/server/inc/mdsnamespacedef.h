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
* Description:  Class to hold description about one namespace and every objects,
*                properties and events it holds.
*
*/

#ifndef __MDSNAMESPACE_H__
#define __MDSNAMESPACE_H__

#include <e32base.h>
#include <s32file.h>

#include "mdsitemdef.h"

class CMdsObjectDef;
class CMdsEventDef;
class CMdsRelationDef;
class CMdCSerializationBuffer;
class CMdsPropertyDef;
class CMdsSchema;

/**
 *  Metadata namespace class
 *
 *  This class is responsible for holding information about everything
 *  which namespace can contain (name, objects, event, relations)
 *
 */
class CMdsNamespaceDef : public CMdsItemDef
    {
    friend class CMdsSchema;
    friend class CMdsImportExport;

public:
	/**
	 * Create new namespace object
	 * @param aId id of namespace
	 * @param aName name of namespace
	 * @param aReadOnly read-only namespace flag
	 * @return pointer to new created namespace
	 */
    static CMdsNamespaceDef* NewL( TDefId aId, const TDesC& aName, TBool aReadOnly,
    		TUint32 aVendorId );

	/**
	 * Create new namespace object
	 * @param aId id of namespace
	 * @param aName name of namespace
	 * @param aReadOnly read-only namespace flag
	 * @return pointer to new created namespace
	 */
    static CMdsNamespaceDef* NewLC( TDefId aId, const TDesC& aName, TBool aReadOnly,
    		TUint32 aVendorId );

    virtual ~CMdsNamespaceDef();

	inline TBool GetReadOnly() const;

	inline TBool GetFirstRead() const;

	inline void UnsetFirstRead();

	inline void SetBaseObject( CMdsObjectDef* const aBaseObject );

	inline TInt32 GetVendorId() const;

	inline void SetVendorId(TInt32 aVendorId);

	/**
	 * Add object to namespace and return pointer to it,
	 * if object already exist function leaves with KErrAlreadyExists
	 * @param objName name of the object definition
	 * @param parName name of the object parent
	 * @return pointer to new object definition or leave with error
	 */
	CMdsObjectDef* AddObjectDefL( const TDesC& aObjectName, const TDesC& aParentName, CMdsSchema* aDefaultSchema );
	
	/**
	 * Add relation to array
	 * @param relName name of relation
	 */
	void AddRelationDefL( const TDesC& aRelationName );
	
	/**
	 * Add event to event array
	 * @param objName name of event
	 * @param aPriority priority of event
	 */
	void AddEventDefL( const TDesC& aEventName, TInt32 aPriority );
	
	/**
	 * Gets object def by object def id.
	 * @param aId objectdef id
	 * @return a pointer to object definition
	 */
	const CMdsObjectDef* GetObjectByIdL( TDefId aId ) const;
	
	/**
	 * Gets event def by devent def id.
	 * @param aId eventdef id
	 * @return a pointer to event definition
	 */
	const CMdsEventDef* GetEventByIdL( TDefId aId ) const;
	
	/**
	 * Gets relation def by relation def id.
	 * @param aId relationdef id
	 * @return a pointer to relation definition
	 */
	const CMdsRelationDef* GetRelationByIdL( TDefId aId ) const;
	
	/**
	 * Gets property def by property def id.
	 * @param aId property id
	 * @return a pointer to relation property
	 */
	const CMdsPropertyDef* GetPropertyL( TDefId aId ) const;
	
	/**
	 * Import object, event and relation defs from default DB.
	 */
	void ImportFromDBL();
	
	/**
	 * Adds new object definition to DB.
	 * @param aId object def id
	 * @param aParentId parent object def id
	 * @param aFlags flags for the new object
	 * @param aName new object def name
	 */
	void AddObjectDefL( TDefId aId, TDefId aParentId, TInt aFlags, const TDesC& aName );
	
	/**
	 * Adds new relation definition to DB.
	 * @param aId relation def id
	 * @param aRelationName new relation name
	 */
	void AddRelationDefL( TDefId aId, const TDesC& aRelationName );
	
	/**
	 * Adds new event definition to DB.
	 * @param aId event def id
	 * @param aEventName new event name
	 * @param aPriority
	 */
	void AddEventDefL( TDefId aId, const TDesC& aEventName, TInt32 aPriority );
	
    /**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
     */
    TUint32 RequiredBufferSize();

    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreeSpace);
    
	/**
	 * Search for relation, using name
	 * @param objName name of object to search for
	 * @return pointer to found object or NULL
	 */
	CMdsRelationDef* GetRelationDef( const TDesC& aRelationName ) const;

	/**
	 * Search for event, using name
	 * @param objName name of object to search for
	 * @return pointer to found object or NULL
	 */
	CMdsEventDef* GetEventDef( const TDesC& aEventName ) const;

	/**
	 * Search for object, using name
	 * @param objName name of object to search for
	 * @return pointer to found object or NULL
	 */
	CMdsObjectDef* GetObjectDef( const TDesC& aObjectName ) const;


private:

	/**
	 * default constructor
	 */
    inline CMdsNamespaceDef( TDefId aId, TBool aReadOnly,
    		                 TUint32 aVendorId );

	/**
	 * Second phase constructor
	 * @param aName name of namespace
	 */
    void ConstructL( const TDesC& aName );

	void StoreToDBL(TBool aStoreOnlyNamespace = EFalse );
	void MergeObjectsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun );
	void MergeRelationsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun );
	void MergeEventsL( CMdsNamespaceDef* aNamespace, const TBool& aDryRun );

    static TInt CompareObjectDefId(const CMdsObjectDef& aFirst, const CMdsObjectDef& aSecond);
    static TInt CompareEventDefId(const CMdsEventDef& aFirst, const CMdsEventDef& aSecond);
    static TInt CompareRelationDefId(const CMdsRelationDef& aFirst, const CMdsRelationDef& aSecond);
	
private:
enum TNamespaceDefFlags
	{
	ENamespaceDefFlagsReadOnly  = 0x0001,
	ENamespaceDefFlagsFirstRead = 0x0002,
	};

private:
    /**
     * Store every namespace read from schema file.
     */
	RPointerArray<CMdsObjectDef> iObjectDefs;

   /**
     * Store every namespace read from schema file.
     */
	RPointerArray<CMdsEventDef> iEventDefs;

   /**
     * Store every namespace read from schema file.
     */
	RPointerArray<CMdsRelationDef> iRelationDefs;

	TInt32 iVendorId;

	/**
	 * Namespace flags: readOnly, firstRead
	 */
	TUint32 iFlags;

	/**
	 * Pointer to base object
	 * DO NOT DELETE, NOT OWN !!!
	 */
	static CMdsObjectDef* iBaseObject;
    };

#include "mdsnamespacedef.inl"

#endif // __MDSNAMESPACE_H__
