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


#ifndef __MDEPROPERTYDEF_H__
#define __MDEPROPERTYDEF_H__

#include <mdccommon.h>

// Forward declaration
class CMdCSerializationBuffer;
class CMdEObjectDef;
class CMdENamespaceDef;
class TMdCPropertyDef;

/**
 * Class for property definitions.
 */
NONSHARABLE_CLASS(CMdEPropertyDef) : public CBase
	{
public:	
	/**
	 * Returns the name of the item.
	 *
	 * @return  name
	 */
	IMPORT_C const TDesC& Name() const;
	
	/**
	 * Returns the type of the property.
	 *
	 * @return  property type
	 */
	IMPORT_C TPropertyType PropertyType() const;

	/**
	 * Returns whether the property is read only.
	 *
	 * @return  <code>ETrue</code>, if the property is read only;
	 *          <code>EFalse</code>, otherwise
	 */
	IMPORT_C TBool ReadOnly() const;

	/**
	 * Returns whether the property is mandatory.
	 *
	 * @return  <code>ETrue</code>, if the property is mandatory;
	 *          <code>EFalse</code>, otherwise
	 */
	IMPORT_C TBool Mandatory() const;

	/**
	 * Compare this property definition with other property definition.
	 *
	 * @param aPropertyDef other property definition
	 *
	 * @return  0 if properties are same, 1 or more if this is greater than other 
	 *          and -1 or less if this is less than other.
	 */
	IMPORT_C TInt Compare(const CMdEPropertyDef& aPropertyDef) const;
	

	IMPORT_C CMdEObjectDef& ObjectDef() const;

	IMPORT_C TInt32 MinInt32ValueL() const;

	IMPORT_C TInt32 MaxInt32ValueL() const;

	IMPORT_C TUint32 MinUint32ValueL() const;

	IMPORT_C TUint32 MaxUint32ValueL() const;

	IMPORT_C TInt64 MinInt64ValueL() const;

	IMPORT_C TInt64 MaxInt64ValueL() const;

	IMPORT_C TReal MinRealValueL() const;

	IMPORT_C TReal MaxRealValueL() const;

	IMPORT_C TTime MinTimeValueL() const;

	IMPORT_C TTime MaxTimeValueL() const;	

	IMPORT_C TInt MinTextLengthL() const;

	IMPORT_C TInt MaxTextLengthL() const;
	
	IMPORT_C CMdENamespaceDef& NamespaceDef() const;

public:
	static CMdEPropertyDef* NewL(const TMdCPropertyDef& aPropertyDef, CMdCSerializationBuffer& aSchemaBuffer, CMdEObjectDef& aObjectDef);

	static CMdEPropertyDef* NewLC(const TMdCPropertyDef& aPropertyDef, CMdCSerializationBuffer& aSchemaBuffer, CMdEObjectDef& aObjectDef);
	
	void SerializeL(CMdCSerializationBuffer& aBuffer) const;

	/**
	 * Destructor.
	 */
	virtual ~CMdEPropertyDef();

	TDefId Id() const;
	void SetId(TDefId aId);
		
	TDefId ObjectDefId() const;
	
private:
	CMdEPropertyDef(const TMdCPropertyDef& aPropertyDef, CMdEObjectDef& aObjectDef);

	void ConstructL(CMdCSerializationBuffer& aSchemaBuffer );

private:	

	const TMdCPropertyDef& iPropertyDef;
	CMdEObjectDef& iObjectDef;

	TPtrC16 iName;
    };

#endif  // __MDEPROPERTYDEF_H__
