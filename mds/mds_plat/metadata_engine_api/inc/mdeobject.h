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


#ifndef __MDEOBJECT_H__
#define __MDEOBJECT_H__

//#include <uri16.h>

#include <mdeinstanceitem.h>

// CMdEProperty compatibility wrappers
#include <mdetextproperty.h>
#include <mdenumberproperty.h>

/* Forward declarations. */
class CMdEObjectDef;
class CMdEPropertyDef;
class CMdEProperty;
class CMdCSerializationBuffer;
class CMdENamespaceDef;

NONSHARABLE_CLASS( TMdEObject )
	{
public:
	/* Constructors and destructor. */

	/**
	 * Constructs a new object.
	 */
	IMPORT_C TMdEObject();

	/**
	 * Returns the ID of the object.
	 *
	 * @return  Object ID.
	 */
	IMPORT_C TItemId Id() const;

	/**
	 * Returns the definition of the object. Leaves with error code 
	 * KErrNotFound if object definition is missing.
	 *
	 * @return Object definition.
	 */
	IMPORT_C const CMdEObjectDef& DefL() const;
	
	/**
	 * Returns whether object is confidential or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to confidential.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool Confidential() const;
	
	/**
	 * Returns whether object is placeholder or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to placeholder.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool Placeholder() const;

	/**
	 * Returns whether object is removed or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to removed.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool Removed() const;
	
	/**
	 * Returns whether object is present or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to not present.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool NotPresent() const;
	
public:
	/**
	 * Calculates required size for buffer to serialize object
	 *
	 * @return necessary buffer size
	 */
	static TUint32 RequiredBufferSize();

	/**
	 * Retrieve object from serialized buffer
	 *
	 * @param aBuffer buffer where serialized object is
	 * @param aNamespaceDef namespace definition of namespace where object is 
	 *                      located
	 */
	void DeSerializeL(CMdCSerializationBuffer& aBuffer, 
			CMdENamespaceDef& aNamespaceDef);

protected:
	/** object ID */
	TItemId iId;

	/** Pointer to object definition, this doesn't own it. */
	CMdEObjectDef* iDef;

	/** various flags */
	TUint32 iFlags;
	};

/**
 * Object.
 */
NONSHARABLE_CLASS(CMdEObject) : public CMdEInstanceItem
	{
public:

	/* Constructors and destructor. */

	/**
	 * Constructs a new object from serialized buffer
	 *
	 * @param aBuffer  buffer which contains serialized object
	 * @param aNamespaceDef  reference to namespace where to object is created
	 */
	static CMdEObject* NewL( CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef );

	/**
	 * Constructs a new object from serialized buffer and leave it on stack
	 *
	 * @param aBuffer  buffer which contains serialized object
	 * @param aNamespaceDef  reference to namespace where to object is created
	 */
	static CMdEObject* NewLC( CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef );

	/**
	 * Constructs a new object.
	 *
	 * @param aDef  Definition of the new object.
	 * @param aUri URI of the new object.
	 * @param aMediaId mediaid of object. Defaults to 0.
	 */

	static CMdEObject* NewL( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId = 0 );

	/**
	 * Constructs a new object.
	 *
	 * @param aDef  Definition of the new object.
	 * @param aUri URI of the new object.
	 * @param aMediaId mediaid of object. Defaults to 0.
	 */
	static CMdEObject* NewLC( CMdEObjectDef& aDef, const TDesC& aUri, TUint32 aMediaId = 0 );


	/**
	 * Destructor.
	 */
	virtual ~CMdEObject();

	/* Methods. */

	/**
	 * Returns the definition of the object.
	 *
	 * @return  Object definition.
	 */
	IMPORT_C CMdEObjectDef& Def() const;

	/**
	 * Sets the definition of the object. Object's old object definition must 
	 * be "Object" and object must not belong to any session, otherwise method 
	 * leaves with error code KErrNotSupported.
	 * 
	 * @param aDef  New object definition.
	 */
	IMPORT_C void SetDefL(CMdEObjectDef& aDef);

	/**
	 * Returns the URI of the object.
	 *
	 * @return  URI.
	 */
	IMPORT_C const TDesC& Uri() const;

	/**
	 * Sets the URI of the object.
	 *
	 * @param aUri  New URI.
	 */
	IMPORT_C void SetUriL(const TDesC& aUri);

	/**
	 * Returns whether object is confidential or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to confidential.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool Confidential() const;

	/**
	 * Returns whether object is confidential or not.
	 *
	 * @param aValue  <code>ETrue</code>, if the object is confidential.
	 *                <code>EFalse</code>, otherwise.
	 */
	IMPORT_C void SetConfidential(TBool aValue);

	/**
	 * Returns whether object is placeholder or not.
	 *
	 * @return  <code>ETrue</code>, if the object has been set to placeholder.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool Placeholder() const;

	/**
	 * Returns whether object is placeholder or not.
	 *
	 * @param aValue  <code>ETrue</code>, if the object is placeholder.
	 *                <code>EFalse</code>, otherwise.
	 */
	IMPORT_C void SetPlaceholder(TBool aValue);

	
	/**
	 * Returns the Media ID of the object.
	 *
	 * @return  Media ID.
	 */
	IMPORT_C TUint32 MediaId() const;
	
	/**
	 * Sets the Media ID of the object.
	 *
	 * @param aMediaId  New Media ID.
	 */
	IMPORT_C void SetMediaId(TUint32 aMediaId);

	/**
	 * Returns the usage count of the object.
	 *
	 * @return usage count
	 */
	IMPORT_C TUint32 UsageCount() const;
	
	/**
	 * Returns the Guid of the object.
	 *
	 * @param aGuidHigh reference where GuidHigh will be stored
	 * @param aGuidLow reference where GuidLow will be stored
	 */
	IMPORT_C void Guid( TInt64& aGuidHigh, TInt64& aGuidLow ) const;

	/**
	 * Sets the Guid of the object.
	 *
	 * @param aGuidHigh new GuidHigh value
	 * @param aGuidLow new GuidLow value
	 */
	IMPORT_C void SetGuid( const TInt64& aGuidHigh, const TInt64& aGuidLow );

	/**
	 * Returns whether the object is open for modifications or not.
	 *
	 * @return  <code>ETrue</code>, if the object is open for modifications.
	 *          <code>EFalse</code>, otherwise.
	 */
	IMPORT_C TBool OpenForModifications() const;

	/* Methods for accessing properties. */

	/**
	 * Returns the number of properties.
	 *
	 * @return  number of properties
	 */
	IMPORT_C TInt PropertyCount() const;

	/**
	 * Returns the property at the specified index.
	 *
	 * @param aIndex  index
	 *
	 * @return  property
	 */
	IMPORT_C CMdEProperty& PropertyL(TInt aIndex) const;

	/**
	 * Returns the property by given property def
	 *
	 * Example:
	 *  CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
     *	CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
	 *  CMdEObject* image = iMdESession->OpenObjectL( 17, imageDef );
	 * 
	 *  CMdEPropertyDef& lastModDatePropDef = imageDef.GetPropertyDefL(
     *   		Object::KLastModifiedDateProperty );
     * 
     *  CMdEProperty* lastModDateProp = NULL;
     *  image->Property( lastModDatePropDef, lastModDateProp );
	 * 
	 * @param aDef      property definition
	 * @param aProperty ref to pointer that will receive property address, if such is found.
	 * @param aStartAt  index where to begin search(by default starts from beginning)
	 *
	 * @return  Index, or <code>KErrNotFound</code>.
	 */
    IMPORT_C TInt Property(const CMdEPropertyDef& aDef, CMdEProperty*& aProperty, TInt aStartAt=0) const;

	/**
	 * Returns the number of properties of the specified definition.
	 *
	 * @param aDef  property definition
	 *
	 * @return  number of properties
	 */
	IMPORT_C TInt PropertyCount(const CMdEPropertyDef& aDef) const;

	/**
	 * Adds a new bool property to the object. If property definition doesn't 
	 * match with value, method leaves with error code KErrArgument.
	 * 
	 * Example:
	 *   CMdEPropertyDef& receivedDef = objectDef.GetPropertyDefL( MdeConstants::Message::KReceivedProperty );
	 *   metadataObject.AddBoolPropertyL( receivedDef, ETrue );
	 *
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddBoolPropertyL(CMdEPropertyDef& aDef, TBool aValue);

	/**
	 * Adds a new signed 8-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 * 
	 * Example:
	 *   CMdEPropertyDef& eightBitDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddInt8PropertyL( eightBitDef, value );
	 *
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddInt8PropertyL(CMdEPropertyDef& aDef, TInt8 aValue);

	/**
	 * Adds a new unsigned 8-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& eightBitDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddUint8PropertyL( eightBitDef, value );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddUint8PropertyL(CMdEPropertyDef& aDef, TUint8 aValue);

	/**
	 * Adds a new signed 16-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddInt16PropertyL( propertyDef, value ); 
	 *
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddInt16PropertyL(CMdEPropertyDef& aDef, TInt16 aValue);

	/**
	 * Adds a new unsigned 16-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddUint16PropertyL( propertyDef, value );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */	 
	IMPORT_C CMdEProperty& AddUint16PropertyL(CMdEPropertyDef& aDef, TUint16 aValue);

	/**
	 * Adds a new signed 32-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 * 
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddInt32PropertyL( propertyDef, value );
	 *
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddInt32PropertyL(CMdEPropertyDef& aDef, TInt32 aValue);

	/**
	 * Adds a new unsigned 32-bit integer property to the object. If property 
	 * definition doesn't match with value, method leaves with error code 
	 * KErrArgument.
	 * 
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddUint32PropertyL( propertyDef, value );
	 *
	 * @param aDef         Definition of the new property. If property definition doesn't 
	 * match with value, method leaves with error code KErrArgument.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddUint32PropertyL(CMdEPropertyDef& aDef, TUint32 aValue);

	/**
	 * Adds a new 64-bit integer property to the object. If property definition 
	 * doesn't match with value, method leaves with error code KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddInt64PropertyL( propertyDef, value );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddInt64PropertyL(CMdEPropertyDef& aDef, TInt64 aValue);

	/**
	 * Adds a new 32-bit real property to the object. If property definition 
	 * doesn't match with value, method leaves with error code KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddReal32PropertyL( propertyDef, realValue );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddReal32PropertyL(CMdEPropertyDef& aDef, TReal32 aValue);

	/**
	 * Adds a new 64-bit real property to the object. If property definition 
	 * doesn't match with value, method leaves with error code KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddReal64PropertyL( propertyDef, realValue );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddReal64PropertyL(CMdEPropertyDef& aDef, TReal64 aValue);

	/**
	 * Adds a new text property to the object. If property definition doesn't 
	 * match with value, method leaves with error code KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddTextPropertyL( propertyDef, text );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddTextPropertyL(CMdEPropertyDef& aDef, const TDesC& aValue);

	/**
	 * Adds a new time property to the object. If property definition doesn't 
	 * match with value, method leaves with error code KErrArgument.
	 *
	 * Example:
	 *   CMdEPropertyDef& propertyDef = objectDef.GetPropertyDefL( MdeConstants::Object::KProperty );
	 *   metadataObject.AddTimePropertyL( propertyDef, time );
	 * 
	 * @param aDef         Definition of the new property.
	 * @param aValue       Value of the new property.
	 *
	 * @return  New property.
	 */
	IMPORT_C CMdEProperty& AddTimePropertyL(CMdEPropertyDef& aDef, TTime aValue);

	/**
	 * Removes the property at the specified index.
	 *
	 * @param aIndex  index
	 */
	IMPORT_C void RemoveProperty(TInt aIndex);


	/* Methods for accessing free text keywords. */

	/**
	 * Returns the number of free text keywords.
	 *
	 * @return  number of free text keywords
	 */
	IMPORT_C TInt FreeTextCount() const;

	/**
	 * Returns the free text keyword at the specified index
	 *
	 * @param aIndex  index
	 *
	 * @return  free text keyword
	 */
	IMPORT_C TPtrC FreeText(TInt aIndex) const;
    
    /**
     * Returns the index of the specified free text.
     *
     * @param aFreeText  Free text.
     *
     * @return  Index, or <code>KErrNotFound</code>.
     */
    IMPORT_C TInt FreeTextIndex(const TDesC& aFreeText) const;

	/**
	 * Adds a new free text keyword.
	 *
	 * @param aFreeText  free text keyword
	 */
	IMPORT_C void AddFreeTextL(const TDesC& aFreeText);

	/**
	 * Removes the free text keyword at the specified index.
	 *
	 * @param aIndex  index
	 */
	IMPORT_C void RemoveFreeText(TInt aIndex);

	/**
	 * Move all properties from object which is given as a paremeter to this object
	 *
	 * @param aObject object from where move properties
	 */
	IMPORT_C void MovePropertiesL(CMdEObject& aObject);

	/**
	 * Returns whether the object has been modified or not.
	 *
	 * @return  <code>ETrue</code>, if the URI, MediaId or Guids  has been modified.
	 *          <code>EFalse</code>, otherwise.
	 */
	TBool ObjectModified() const;

	/**
	 * Returns whether the free text array has been modified or not.
	 *
	 * @return  <code>ETrue</code>, if FreeTexts has been modified.
	 *          <code>EFalse</code>, otherwise.
	 */
	TBool FreeTextModified() const;

	/**
	 * Returns whether one of the properties has been modified or not.
	 *
	 * @return  Are properties modified.
	 */
	TBool PropertyModified() const;

	/*
	 * clear all modification flags and removes removed properties
	 */
	void ClearObject( TBool aClearFlags = ETrue );

	/**
	 * Sets that the object is not open for modifications.
	 */
	void SetNotOpenForModifications();

	/**
	 * Left locked automatically after it's added to DB.
	 */
	void AutoLockL() const;
	
	/**
	 * Calculates required size for buffer to serialize object
	 *
	 * @return necessary buffer size
	 */
	TUint32 RequiredBufferSize() const;

	/**
	 * Serialize object
	 * RequiredBufferSize() MUST be always called before this function!
	 *
	 * @param aBuffer buffer where serialized object should go
	 */
	TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const;

protected:

	virtual TMdEInstanceType InstanceType() const;

private:

	/**
	 * Constructs a new object.
	 *
	 * @param aSession session owner
	 * @param aId object id
	 * @param aDef  Definition of the new object.
	 */
	CMdEObject( CMdESession* aSession, TItemId aId, CMdEObjectDef& aDef );
	
	/**
	 * Second-phase constructor.
	 * @param aUri URI of the new object.
	 * @param aMediaId mediaid of object. Defaults to 0.
	 */
	void ConstructL( const TDesC& aUri, TUint32 aMediaId = 0 );
	
	/**
	 * Sets the URI of the object.
	 *
	 * @param aUri  New URI.
	 */
	void DoSetUriL(const TDesC& aUri);

	/**
	 * Adds a new property to the object.
	 *
	 * @param aProperty  New property.
	 * @param aState     Modification state of the new property.
	 */
	void DoAddPropertyL(CMdEProperty& aProperty);
	
	/**
	 * Compares two properties.
	 *
	 * @param aFirst   First property.
	 * @param aSecond  Second property.
	 *
	 * @return  0, if the properties are equal;
	 *		    a negative value, if the first property is less than the second;
	 *			a positive value, if the first property is greater than the second.
	 */
	static TInt CompareProperties(const CMdEProperty& aFirst, 
						          const CMdEProperty& aSecond);

	/**
	 * Adds a new free text to the object.
	 *
	 * @param aFreeText  New free text.
	 */
	TInt DoAddFreeTextL(const HBufC& aFreeText);

	/**
	 * Compares two free texts.
	 *
	 * @param aFirst   First free text.
	 * @param aSecond  Second free text.
	 *
	 * @return  0, if the texts are equal;
	 *		    a negative value, if the first text is less than the second;
	 *			a positive value, if the first text is greater than the second.
	 */
	static TInt CompareFreeTexts(const HBufC& aFirst, const HBufC& aSecond);

	/**
	 * Returns how many properties is modified and should be send to server
	 */
	TUint32 ModifiedPropertiesCount() const;

private:

	/** Object definition.
	 * This pointer must be initialized in constructor and 
	 * not must be NULL in any point. */
	CMdEObjectDef* iDef;

	/** Array for properties. */
	RPointerArray<CMdEProperty> iPropertyArray;	

	/** Array for free text. */
	RPointerArray<HBufC> iFreeTextArray;	

	/** various flags */
	mutable TUint32 iFlags;
	
	/** memory card Id */
	TUint32 iMediaId;

	/** usage count (read-only) */
	TUint32 iUsageCount;
	
	/** Guid Low and High */
	TInt64 iGuidHigh;
	TInt64 iGuidLow;

	/** URI descriptor. */
	HBufC* iUri;
	};


#endif  // __MDEOBJECT_H__
