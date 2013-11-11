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

#ifndef __MDENAMESPACEDEF_H__
#define __MDENAMESPACEDEF_H__


#include <mdccommon.h>

class CMdESession;
class CMdCSerializationBuffer;
class CMdERelationDef;
class CMdEEventDef;
class CMdEObjectDef;

class TMdCNamespaceDef;


/**
 * Namespace definition.
 */
NONSHARABLE_CLASS(CMdENamespaceDef) : public CBase
	{
public:
	/**
	 * Returns whether the namespace is read only.
	 *
	 * @return  <code>ETrue</code>, if the namespace is read only;
	 *          <code>EFalse</code>, otherwise
	 */
	IMPORT_C TBool ReadOnly() const;

	/**
	 * Returns the name of the item.
	 *
	 * @return  name
	 */
	IMPORT_C const TDesC& Name() const;

	/**
	 * Compare this namespace definition with other namespace definition.
	 *
	 * @param aNamespaceDef other namespace definition
	 *
	 * @return  0 if namespaces are same, 1 or more if this is greater than other 
	 *          and -1 or less if this is less than other.
	 */
	IMPORT_C TInt Compare(const CMdENamespaceDef& aNamespaceDef) const;
	
	/**
	 * Returns the number of object definitions.
	 *
	 * @return  number of object definitions
	 */
	IMPORT_C TInt ObjectDefCount() const;

	/**
	 * Returns the object definition at the specified index.
	 *
	 * @param aIndex  index
	 *
	 * @return  object definition or leave, if there is no object definition
	 *          with the specified name.	
	 *
	 * @panics if aIndex is out of bounds	 
	 */
	IMPORT_C CMdEObjectDef& ObjectDefL(TInt aIndex);

	/**
	 * Returns the object definition with the specified name.
	 *
	 * @param aName       name
	 *
	 * @return  object definition or leave, if there is no object definition
	 *          with the specified name.	 
	 */
	IMPORT_C CMdEObjectDef& GetObjectDefL(const TDesC& aName);
		
	/**
	 * Returns the number of relation definitions.
	 *
	 * @return  number of relation definitions
	 */
	IMPORT_C TInt RelationDefCount() const;

	/**
	 * Returns the relation definition at the specified index.
	 *
	 * @param aIndex  index
	 *
	 * @return  relation definition or leave, if there is no object definition
	 *          with the specified name.	
	 */
	IMPORT_C CMdERelationDef& RelationDefL(TInt aIndex);

	/**
	 * Returns the relation definition with the specified name.
	 *
	 * @param aName       name
	 *
	 * @return  relation definition or leave, if there is no object definition
	 *          with the specified name.	
	 *
	 * @panics if aIndex is out of bounds	 
	 */
	IMPORT_C CMdERelationDef& GetRelationDefL(const TDesC& aName);

	/**
	 * Returns the number of event definitions. 
	 *
	 * @return  number of event definitions
	 */
	IMPORT_C TInt EventDefCount() const;

	/**
	 * Returns the event definition at the specified index.
	 *
	 * @param aIndex  index
	 *
	 * @return  event definition or leave, if there is no object definition
	 *          with the specified name.	
	 * 
	 * @panics if aIndex is out of bounds
	 */
	IMPORT_C CMdEEventDef& EventDefL(TInt aIndex);

	/**
	 * Returns the event definition with the specified name.
	 *
	 * @param aName       name
	 *
	 * @return  event definition or leave, if there is no object definition
	 *          with the specified name.	
	 */
	IMPORT_C CMdEEventDef& GetEventDefL(const TDesC& aName);

public:
	static CMdENamespaceDef* NewL(const CMdESession& aMdeSession, 
			const TMdCNamespaceDef& aNamespaceDef, 
			CMdCSerializationBuffer& aSchemaBuffer);

	static CMdENamespaceDef* NewLC(const CMdESession& aMdeSession, 
			const TMdCNamespaceDef& aNamespaceDef, 
			CMdCSerializationBuffer& aSchemaBuffer);
		
	/**
	 * Destructor.
	 */
	virtual ~CMdENamespaceDef();
	
	TDefId Id() const;		
	
	/**
	 * Returns the object definition with the specified id.
	 *
	 * @param aId       id
	 *
	 * @return  object definition; or 0, if there is no object definition
	 *          with the specified id
	 */
	CMdEObjectDef* GetObjectDefL(TDefId aId);	

	/**
	 * Returns the relation definition with the specified id.
	 *
	 * @param aId       id
	 *
	 * @return  relation definition; or 0, if there is no relation definition
	 *          with the specified id
	 */
	CMdERelationDef* GetRelationDefL(TDefId aId);

	/**
	 * Returns the event definition with the specified id.
	 *
	 * @param aId       id
	 *
	 * @return  event definition; or 0, if there is no event definition
	 *          with the specified id
	 */
	CMdEEventDef* GetEventDefL(TDefId aId);
	
	/**
	 * Returns the MdE session where namespace definition belongs.
	 *
	 * @return MdE session
	 */
	const CMdESession& Session() const;

private:
	CMdENamespaceDef(const CMdESession& aMdeSession, 
			const TMdCNamespaceDef& aNamespaceDef);

	void ConstructL(CMdCSerializationBuffer& aSchemaBuffer);

private:
	const CMdESession& iMdeSession;

	const TMdCNamespaceDef& iNamespaceDef;

	TPtrC16 iName;

	RPointerArray<CMdEObjectDef> iObjectDefs;
	RPointerArray<CMdERelationDef> iRelationDefs;
	RPointerArray<CMdEEventDef> iEventDefs;
	};

#endif  // __MDENAMESPACEDEF_H__
