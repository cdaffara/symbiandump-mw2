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

#ifndef __MDSOBJECTDEF_H__
#define __MDSOBJECTDEF_H__

#include <e32base.h>
#include "mdsitemdef.h"

class CMdsPropertyDef;


/**
 *  Metadata object class
 *
 *  This class is responsible for holding information about one object
 *
 */
class CMdsObjectDef : public CMdsItemDef
    {
    friend class CMdsSchema;
    friend class CMdsNamespaceDef;

public:

	struct TMdsColumnOrder
		{
		TInt32 iColumnId;
		const CMdsPropertyDef& iPropertyDef;
		};

	enum TObjectDefFlags
		{
		EObjectDefFlagsNone    = 0x0000,
		EObjectDefFlagsContext = 0x0001,
		};

	/**
	 * Create new objectdef object
	 * @param aId id of object
	 * @param aName name of object
	 * @param aParent parent to actual object
	 * @return pointer to new created object
	 */
    static CMdsObjectDef* NewL( const TDesC& aName, const CMdsObjectDef* aParent );

	/**
	 * Create new objectdef object
	 * @param aId id of object
	 * @param aName name of object
	 * @param aParent parent to actual object
	 * @return pointer to new created object
	 */
    static CMdsObjectDef* NewLC( const TDesC& aName, const CMdsObjectDef* aParent );

    virtual ~CMdsObjectDef();
    
	/**
	 * Add property to property array
	 * @see CommonAddPropertyL
	 * @see AddMinMaxValue
	 */
	
	void AddPropertyL( const TDesC& aName, TPropertyType aType,
			  const TMdCValueUnion& aMinAdd, const TMdCValueUnion& aMaxAdd,
			  TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Add property to property array
	 * @see CommonAddPropertyL
	 * @see AddMinMaxValue
	 */
	void AddPropertyL( const TDesC& aName, TPropertyType aType,
            TInt32 aMinAdd, TInt32 aMaxAdd,
            TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Add property to property array
	 * @see CommonAddPropertyL
	 * @see AddMinMaxValue
	 */
	void AddPropertyL( const TDesC& aName, TPropertyType aType,
            TUint32 aMinAdd, TUint32 aMaxAdd,
            TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Add property to property array
	 * @see CommonAddPropertyL
	 * @see AddMinMaxValue
	 */
	void AddPropertyL( const TDesC& aName, TPropertyType aType,
            const TInt64& aMinAdd, const TInt64& aMaxAdd,
            TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Add property to property array
	 * @see CommonAddPropertyL
	 * @see CMdsPropertyDef::AddMinMaxValue
	 */
	void AddPropertyL( const TDesC& aName, TPropertyType aType,
            const TReal& aMinAdd, const TReal& aMaxAdd,
            TBool aReadOnly, TBool aMandatory, TBool aIndexed );
    
    const CMdsPropertyDef* GetPropertyByIdL( TDefId aId ) const;

	CMdsPropertyDef* GetProperty( TUint32 aIndex ) const;

	CMdsPropertyDef* GetProperty( const TDesC& aName ) const;

	/**
	 * How many properties have object
	 * return properties.Count()
	 */
	inline TInt GetPropertiesCount() const;

	/**
	 * Gets property count of object including it's parents
	 *
	 * @return property count
	 */
	TInt GetAllPropertiesCount() const;

	/**
	 * Get property count of mandatory properties.
	 * 
	 * @return mandatory property count
	 */
	TInt GetMandatoryPropertyCount() const;
	
	void SetStoredEveryInDB();

	void SetNotStoredEveryInDB();

	/*
	 * Set that object is already stored in DB
	 */	
	inline void SetC2PStoredInDB();

	inline TBool GetC2PStoredInDB() const;

	/*
	 * Gets parent of the object
	 */
	inline const CMdsObjectDef* GetParent() const;

	/**
	 * Get property column Id
	 *
	 * @param aPropertyNo number of property in property array
	 * @return ColumnId
	 */
	inline const TMdsColumnOrder& GetPropertyColumnL( TInt aIndex ) const;

	void ImportFromDBL();

	/**
	 * Def flags support
	 */
	inline void ResetFlags();

	inline TObjectDefFlags GetFlags() const;

	inline void SetFlags(TObjectDefFlags aFlags);

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
		
protected:

	/**
	 * Create and add property to array
	 * @param aName name of property
	 * @param aType type of property
	 * @param aReadOnly if property is read-only
	 * @param aReadOnly if property is mandatory
	 * @param aId property id
	 * @return pointer to new created property (on error - leave)
	 */
	CMdsPropertyDef* CommonAddPropertyL( const TDesC& aName, TPropertyType aType,
			TBool aReadOnly, TBool aMandatory, TBool aIndexed );
	
	CMdsPropertyDef* CommonAddPropertyL( const TDesC& aName, TPropertyType aType,
			TUint32 aFlags, TDefId aId );

	void MergeL( CMdsObjectDef* aObjectDef, const TBool& aDryRun );

	void AddPropertyL( const TDefId aId, TPropertyType aType, TUint32 aFlags,
            const TDesC& aMinAdd, const TDesC& aMaxAdd, const TDesC& aName );

	void StoreToDBL( TDefId aNamespaceDefId );

	/**
	 * Searching for property only in actual object (not in parents)
	 *
	 * @param aName name of property to find
	 * @return finded property or NULL
	 */
    CMdsPropertyDef* FindProperty( const TDesC& aName ) const;
	
private:
    inline CMdsObjectDef( const CMdsObjectDef* aParent );

	/**
	 * 2nd phase constructor
	 * @param aName name of property
	 */
    void ConstructL( const TDesC& aName );

private: // data

    /**
     * Store every property read from schema file.
     */
	RPointerArray<CMdsPropertyDef> iPropertyDefs;

	/**
	 * store bind between column and property in object
	 */
	RArray<TMdsColumnOrder> iCol2Prop;

	/**
	 * ObjectDef parent, there is no exist object without parent
	 * (exeption is Object as an ancesor to everyone, but it's created in code
	 *  and cannot be read from file)
	 * DO NOT DELETE, NOT OWN !!!
	 */
	const CMdsObjectDef* iParent;

	/**
	 * object flags (e.g. context object)
	 * check TObjectDefFlags
	 */
	TObjectDefFlags iFlags;
	
	/**
	 * Count of mandatory properties.
	 */
	TInt iMandatoryPropertyCount;
    };

#include "mdsobjectdef.inl"

#endif // __MDSOBJECTDEF_H__
