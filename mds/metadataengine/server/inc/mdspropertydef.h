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

#ifndef __MDSPROPERTYDEF_H__
#define __MDSPROPERTYDEF_H__

#include "mdsitemdef.h"
#include "mdssqliteconnection.h"

class CMdCSerializationBuffer;


/**
 *  Metadata namespace class
 *
 *  This class is responsible for holding information about one property
 *
 */
class CMdsPropertyDef : public CMdsItemDef
    {
    friend class CMdsSchema;
    friend class CMdsObjectDef;
public:

	/**
	 * Create new property object
	 * @param aId id of property
	 * @param aName name of property
	 * @param aType type of property
	 * @param aMandatory mandatory flag
	 * @param aReadOnly read-only flag
	 * @param aIndexed should this property be indexed or not
	 * @return pointer to new created property
	 */
    static CMdsPropertyDef* NewL( const TDesC& aName, TPropertyType aType,
    		 TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Create new property object
	 * @param aId id of property
	 * @param aName name of property
	 * @param aType type of property
	 * @param aMandatory mandatory flag
	 * @param aReadOnly read-only flag
	 * @param aIndexed should this property be indexed or not
	 * @return pointer to new created property
	 */
    static CMdsPropertyDef* NewLC( const TDesC& aName, TPropertyType aType,
    		TBool aReadOnly, TBool aMandatory, TBool aIndexed );

	/**
	 * Create new property object
	 * @param aId id of property
	 * @param aName name of property
	 * @param aType type of property
	 * @param aFlags property flag
	 * @return pointer to new created property
	 */
    static CMdsPropertyDef* NewL( const TDesC& aName, TPropertyType aType,
    		TUint32 aFlags );

	/**
	 * Create new property object
	 * @param aId id of property
	 * @param aName name of property
	 * @param aType type of property
	 * @param aFlags property flag
	 * @return pointer to new created property
	 */
    static CMdsPropertyDef* NewLC( const TDesC& aName, TPropertyType aType,
    		TUint32 aFlags );

    
    virtual ~CMdsPropertyDef();
    
    
    void StoreToDBL( TDefId aObjectDefId );
    
	/**
	 * Add minimun and maximum value to property
	 *  leave on type mismatch
	 * @param aMinValue min value
	 * @param aMaxValue max value
	 */
	void AddMinMaxValueL( const TInt32& aMinValue, const TInt32& aMaxValue );

	/**
	 * Add minimun and maximum value to property
	 *  leave on type mismatch
	 * @param aMinValue min value
	 * @param aMaxValue max value
	 */
	void AddMinMaxValueL( const TUint32& aMinValue, const TUint32& aMaxValue );

	/**
	 * Add minimun and maximum value to property
	 *  leave on type mismatch
	 * @param aMinValue min value
	 * @param aMaxValue max value
	 */
	void AddMinMaxValueL( const TInt64& aMinValue, const TInt64& aMaxValue );

	/**
	 * Add minimun and maximum value to property
	 *  leave on type mismatch
	 * @param aMinValue min value
	 * @param aMaxValue max value
	 */
	void AddMinMaxValueL( const TReal& aMinValue, const TReal& aMaxValue );

	/**
	 * Check if property's value is between allowed minimum and maximum value.
	 * 
	 * @param aValue property's value
	 * 
	 * @return is value between allowed minimum and maximum value
	 */
	inline TBool CheckMinMaxValue( TInt32 aValue ) const;

	/**
	 * Check if property's value is between allowed minimum and maximum value.
	 * 
	 * @param aValue property's value
	 * 
	 * @return is value between allowed minimum and maximum value
	 */
	inline TBool CheckMinMaxValue( TUint32 aValue ) const;

	/**
	 * Check if property's value is between allowed minimum and maximum value.
	 * 
	 * @param aValue property's value
	 * 
	 * @return is value between allowed minimum and maximum value
	 */
	inline TBool CheckMinMaxValue( const TInt64& aValue ) const;

	/**
	 * Check if property's value is between allowed minimum and maximum value.
	 * 
	 * @param aValue property's value
	 * 
	 * @return is value between allowed minimum and maximum value
	 */
	inline TBool CheckMinMaxValue( const TReal& aValue ) const;

	/**
	 * Get SQLite type name
	 * @return type name
	 */
	const TDesC& GetSqlTypeName() const;

	/**
	 * Get SQLite property type
	 * @return property type
	 */
	TColumnDataType GetSqlType() const;

	/**
	 * Get property type
	 * @return property type
	 */
	inline TPropertyType GetType() const;

	/**
	 * Get property read-only
	 * @return property read-only
	 */
	inline TBool GetReadOnly() const;

	/**
	 * Get property mandatory
	 * @return is property mandatory
	 */
	inline TBool GetMandatory() const;
	
	/**
	 * Get property indexed flag
	 * @return is property indexed
	 */
	inline TBool GetIndexed() const;

	TBool operator==( const CMdsPropertyDef& aPropertyDef ) const;

	inline TBool operator!=( const CMdsPropertyDef& aPropertyDef ) const;
	
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
	
	
private:
	/**
	 * default constructor
	 */
    inline CMdsPropertyDef(  TPropertyType aType );

	/**
	 * 2nd phase constructor
	 * @param aName name of property
	 * @param aMinCount min count of property
	 * @param aMaxCount max count of property
	 * @param aMandatory mandatory property
	 * @param aIndexed create an index of this property
	 */
    void ConstructL( const TDesC& aName, TBool aReadOnly, TBool aMandatory, TBool aIndexed );


private:

	/**
	 * type of property
	 */
	const TPropertyType iType;
	
	/**
	 * Minimum value for property
	 */	
	TMdCValueUnion iMinValue;

	/**
	 * Maximum value for property
	 */
	TMdCValueUnion iMaxValue;
	
	/**
	 * property read-only flag
	 */
	TUint32 iPropertyFlags;
	
	/**
	 * Should this property be indexed or not.
	 */
	TBool iIndexed;
    };

#include "mdspropertydef.inl"

#endif // __MDSPROPERTYDEF_H__
