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


#ifndef __MDEOBJECTDEF_H__
#define __MDEOBJECTDEF_H__


#include <e32base.h>
//#include <uri16.h>

#include <mdccommon.h>

// Forward declaration
class CMdEPropertyDef;
class CMdCSerializationBuffer;
class CMdENamespaceDef;
class TMdCObjectDef;

/**
 * Object definition.
 */
NONSHARABLE_CLASS(CMdEObjectDef) : public CBase
	{
public:	
	IMPORT_C CMdEObjectDef* ParentL();
	
	/**
	 * Returns the name of the item.
	 *
	 * @return  name
	 */
	IMPORT_C const TDesC& Name() const;

	/**
	 * Returns the number of property definitions.
	 *
	 * @return  number of property definitions
	 */
	IMPORT_C TInt PropertyDefCount() const;

	/**
	 * Returns the property definition at the specified index.
	 *
	 * @return  property definition
	 */
	IMPORT_C CMdEPropertyDef& PropertyDefL(TInt aIndex);

	/**
	 * Returns the property definition with the specified name.
	 *
	 * @param aName         name
	 *
	 * @return  property definition; 
	 *          or 0 if there is no property definition with the specified name
	 */
	IMPORT_C CMdEPropertyDef& GetPropertyDefL(const TDesC& aName);

	/**
	 * Compare this object definition with other object definition.
	 *
	 * @param aObjectDef other object definition
	 *
	 * @return  0 if objects are same, 1 or more if this is greater than other 
	 *          and -1 or less if this is less than other.
	 */
	IMPORT_C TInt Compare(const CMdEObjectDef& aObjectDef) const;
	
	/**
	* Returns namespacedef where this objectdef belongs to.
	* @Return namespacedef where this objectdef belongs to.
	*/
	IMPORT_C CMdENamespaceDef& NamespaceDef() const;
	
public:
	static CMdEObjectDef* NewL(const TMdCObjectDef& aObjectDef, CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef );

	static CMdEObjectDef* NewLC(const TMdCObjectDef& aObjectDef, CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef );

	/**
	 * Destructor.
	 */
	virtual ~CMdEObjectDef();

	IMPORT_C TDefId Id() const;	
	
	/**
	 * Returns the property definition with the specified id
	 *
	 * @param aId         id
	 *
	 * @return  property definition; 
	 *          or <code>NULL</code> if there is no property definition with the specified id
	 */
	CMdEPropertyDef* GetPropertyDefL(TDefId aId, CMdEObjectDef* aChild = NULL);

private:
	CMdEObjectDef(const TMdCObjectDef& aObjectDef, CMdENamespaceDef& aNamespaceDef);

	CMdEPropertyDef* PropertyDefL(TInt aIndex, CMdEObjectDef* aChild);

	CMdEPropertyDef* GetPropertyDefL(const TDesC& aName, CMdEObjectDef* aChild);

	void ConstructL(CMdCSerializationBuffer& aSchemaBuffer);

    static TInt CompareProperties(const CMdEPropertyDef& aFirst, const CMdEPropertyDef& aSecond);
	
private:
	const TMdCObjectDef& iObjectDef;
	CMdENamespaceDef& iNamespaceDef;

	TPtrC16 iName;
		
	RPointerArray<CMdEPropertyDef> iPropertyDefs;
	};

#endif  // __MDEOBJECTDEF_H__
