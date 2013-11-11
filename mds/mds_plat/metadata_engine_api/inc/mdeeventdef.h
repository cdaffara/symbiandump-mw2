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


#ifndef __MDEEVENTDEF_H__
#define __MDEEVENTDEF_H__

#include <e32base.h>

#include <mdccommon.h>

// Forward declaration
class CMdCSerializationBuffer;
class CMdENamespaceDef;
class TMdCEventDef;

/**
 * Event definition.
 */
NONSHARABLE_CLASS(CMdEEventDef) : public CBase
	{
public:	
	/**
	 * Returns the name of the item.
	 *
	 * @return  name
	 */
	IMPORT_C const TDesC& Name() const;
	
	/**
	 * Compare this event definition with other event definition.
	 *
	 * @param aEventDef other event definition
	 *
	 * @return  0 if events are same, 1 or more if this is greater than other 
	 *          and -1 or less if this is less than other.
	 */
	IMPORT_C TInt Compare(const CMdEEventDef& aEventDef) const;	
	
	/**
	Returns namespacedef which is associated with eventdef
	@return namespacedef which is associated with eventdef
	*/
	IMPORT_C CMdENamespaceDef& NamespaceDef() const;
	
public:
	static CMdEEventDef* NewL(const TMdCEventDef& aEventDef, CMdCSerializationBuffer& aSchemaBuffer,
			CMdENamespaceDef& aNamespaceDef);

	static CMdEEventDef* NewLC(const TMdCEventDef& aEventDef, CMdCSerializationBuffer& aSchemaBuffer,
			CMdENamespaceDef& aNamespaceDef);

	/**
	 * Destructor.
	 */
	virtual ~CMdEEventDef();

	TDefId Id() const;	

private:
	CMdEEventDef(const TMdCEventDef& aEventDef, CMdENamespaceDef& aNamespaceDef);

	void ConstructL(CMdCSerializationBuffer& aSchemaBuffer);

private:
	const TMdCEventDef& iEventDef;
	CMdENamespaceDef& iNamespaceDef;

	TPtrC16 iName;
	};

#endif  // __MDEEVENTDEF_H__
