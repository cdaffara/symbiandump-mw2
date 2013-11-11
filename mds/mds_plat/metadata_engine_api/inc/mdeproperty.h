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
* Description:  ?Description
*
*/


#ifndef __MDEPROPERTY_H__
#define __MDEPROPERTY_H__

#include <e32base.h>

#include <mdeinstanceitem.h>


/* Forward declarations. */
class CMdEObject;
class CMdEPropertyDef;
class CMdCSerializationBuffer;

/**
 * Abstract base class for properties.
 */
NONSHARABLE_CLASS(CMdEProperty) : public CMdEInstanceItem
	{
	friend class CMdEObject;
public:
	/**
	 * Destructor.
	 */
	virtual ~CMdEProperty();


	/* Methods. */

	/**
	 * Constructs a new property.
	 *
	 * @param aObject owner object of property
	 * @param aBuffer buffer where property was serialized
	 * @return new numeric property
	 */
	static CMdEProperty* NewL( const CMdEObject& aObject, CMdCSerializationBuffer& aBuffer );

	/**
	 * Constructs a new property and leave it on the cleanup stack.
	 *
	 * @param aObject owner object of property
	 * @param aBuffer buffer where property was serialized
	 * @return new numeric property
	 */
	static CMdEProperty* NewLC( const CMdEObject& aObject, CMdCSerializationBuffer& aBuffer );

	/**
	 * Constructs a new property.
	 *
	 * @param aDef property definition
	 * @param aObject owner object of property
	 * @return new numeric property
	 */
	static CMdEProperty* NewL(const CMdEPropertyDef& aDef, const CMdEObject& aObject);

	/**
	 * Constructs a new property.
	 *
	 * @param aDef property definition
	 * @param aObject owner object of property
	 * @return new numeric property
	 */
	static CMdEProperty* NewLC(const CMdEPropertyDef& aDef, const CMdEObject& aObject);


	/**
	 * Returns the definition of the integer property.
	 *
	 * @return  property definition
	 */
	IMPORT_C const CMdEPropertyDef& Def() const;

	/**
	 * Returns the object which the property belongs to.
	 *
	 * @return  Object.
	 */
	IMPORT_C const CMdEObject& Object() const;

	/**
	 * @return ETrue if property was modified,
	 *         EFalse otherwise
	 */
	IMPORT_C TBool Modified() const;

	
	/** value getters */

	/**
	 * Returns the TBool value of the property.
	 * @return  value
	 */
	IMPORT_C TBool BoolValueL() const;

	/**
	 * Returns the TInt8 value of the property.
	 * @return  value
	 */
	IMPORT_C TInt8 Int8ValueL() const;

	/**
	 * Returns the TUint8 value of the property.
	 * @return  value
	 */

	IMPORT_C TUint8 Uint8ValueL() const;
	
	/**
	 * Returns the TInt16 value of the property.
	 * @return  value
	 */
	IMPORT_C TInt16 Int16ValueL() const;
	
	/**
	 * Returns the TUint16 value of the property.
	 * @return  value
	 */
	IMPORT_C TUint16 Uint16ValueL() const;
	
	/**
	 * Returns the TInt32 value of the property.
	 * @return  value
	 */
	IMPORT_C TInt32 Int32ValueL() const;
	
	/**
	 * Returns the TUint32 value of the property.
	 * @return  value
	 */
	IMPORT_C TUint32 Uint32ValueL() const;
	
	/**
	 * Returns the TInt64 value of the property.
	 * @return  value
	 */
	IMPORT_C TInt64 Int64ValueL() const;
	
	/**
	 * Returns the TReal32 value of the property.
	 * @return  value
	 */
	IMPORT_C TReal32 Real32ValueL() const;
	
	/**
	 * Returns the TReal64 value of the property.
	 * @return  value
	 */
	IMPORT_C TReal64 Real64ValueL() const;
	
	/**
	 * Returns the TTime value of the property.
	 * @return  value
	 */
	IMPORT_C TTime TimeValueL() const;

	/**
	 * Returns the text value of the property.
	 *
	 * @return  value
	 */
	IMPORT_C const TDesC& TextValueL() const;
	

	/** setter methods */
	/**
	 * Sets the TBool value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetBoolValueL(TBool aValue);

	/**
	 * Sets the TInt8 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetInt8ValueL(TInt8 aValue);

	/**
	 * Sets the TUint8 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetUint8ValueL(TUint8 aValue);

	/**
	 * Sets the TInt16 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetInt16ValueL(TInt16 aValue);

	/**
	 * Sets the TUint16 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetUint16ValueL(TUint16 aValue);

	/**
	 * Sets the TInt32 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetInt32ValueL(TInt32 aValue);

	/**
	 * Sets the TUint32 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetUint32ValueL(TUint32 aValue);

	/**
	 * Sets the TInt64 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetInt64ValueL(TInt64 aValue);

	/**
	 * Sets the TReal32 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetReal32ValueL(TReal32 aValue);

	/**
	 * Sets the TReal64 value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetReal64ValueL(TReal64 aValue);

	/**
	 * Sets the TTime value of the property.
	 * @param aValue       New value.
	 */
	IMPORT_C void SetTimeValueL(const TTime& aValue);

	/**
	 * Sets the text value of the property.
	 *
	 * @param aValue       New value.
	 */
	IMPORT_C void SetTextValueL(const TDesC& aValue);

	
	/**
	 * Return required size for property serialization
	 *
	 * @return size of serialized property
	 */
	TUint32 RequiredBufferSize() const;

	/**
	 * Serialize object
	 *
	 * @param aBuffer buffer where serialized object should go
	 */
	TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const;
	
private:

	TUint8 ModFlags() const;

	void SetRemoved();
	TBool Removed();

	void ClearFlags();

	void SetModified();


	/* Constructors. */

	/**
	 * Constructor.
	 *
	 * @param aDef         Definition of the new property.	 
	 * @param aObject object where this property belongs to.
	 */
	CMdEProperty(const CMdEPropertyDef& aDef, const CMdEObject& aObject);

	/**
	 * Second-phase constructor.
	 *
	 * @param aSource  Source of the new property.
	 */
	void BaseConstructL();


	/* Methods. */

	/**
	 * Check if property has value is between min and max
	 * and leave if not!
	 */
#ifdef _DEBUG
	void CheckValueMinMaxL( const TMdCValueUnion& aValue ) const;
#endif
	
	void DoSetTextValueL(const TDesC& aValue);

	TMdEInstanceType InstanceType() const;

protected:
	/** property modifications flags */
	TUint8 iModFlags;

private:
	/** Property definition. */
	const CMdEPropertyDef& iDef;

	/** Object this property belongs to. */
	const CMdEObject& iObject;

	/** Value. */
	TMdCValueUnion iValue;
	};

#endif  // __MDEPROPERTY_H__
