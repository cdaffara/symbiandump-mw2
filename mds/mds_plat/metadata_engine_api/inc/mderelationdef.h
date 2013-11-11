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

#ifndef __MDERELATIONDEF_H__
#define __MDERELATIONDEF_H__


#include <e32base.h>

#include <mdccommon.h>

// Forward declaration
class CMdCSerializationBuffer;
class CMdENamespaceDef;
class TMdCRelationDef;


/**
 * Relation definition.
 */
NONSHARABLE_CLASS(CMdERelationDef) : public CBase
	{
public:	

	static CMdERelationDef* NewL(const TMdCRelationDef& aRelationDef, CMdCSerializationBuffer& aSchemaBuffer,
			CMdENamespaceDef& aNamespaceDef);

	static CMdERelationDef* NewLC(const TMdCRelationDef& aRelationDef, CMdCSerializationBuffer& aSchemaBuffer,
			CMdENamespaceDef& aNamespaceDef);

	/**
	 * Returns the name of the item.
	 *
	 * @return  name
	 */
	IMPORT_C const TDesC& Name() const;

	/**
	 * Compare this relation definition with other relation definition.
	 *
	 * @param aRelationDef other relation definition
	 *
	 * @return  0 if relations are same, 1 or more if this is greater than other 
	 *          and -1 or less if this is less than other.
	 */
	IMPORT_C TInt Compare(const CMdERelationDef& aRelationDef) const;

	/**
	 * Returns namespace definition of namespace which contains this relation
	 * @return namespace definition of relation
	 */
	IMPORT_C CMdENamespaceDef& NamespaceDef() const;

public:		

	/**
	 * Destructor.
	 */
	virtual ~CMdERelationDef();

	TDefId Id() const;

private:
	CMdERelationDef(const TMdCRelationDef& aRelationDef, CMdENamespaceDef& aNamespaceDef);

	void ConstructL(CMdCSerializationBuffer& aSchemaBuffer);

private:
	const TMdCRelationDef& iRelationDef;
	CMdENamespaceDef& iNamespaceDef;

	TPtrC16 iName;
	};

#endif  // __MDERELATIONDEF_H__
