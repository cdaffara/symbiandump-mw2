/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  common item object for MdE
*
*/

#ifndef __MDCDEF_H__
#define __MDCDEF_H__

#include "mdscommoninternal.h"
#include "mdcserializationtype.h"


NONSHARABLE_CLASS(TMdCDef) : protected TMdCSerializationType
	{
	protected:
		TMdCDef(TMdCStructTypes aType) : TMdCSerializationType( aType ), iDefId( 0 )
			{
			}
		
	public:
		TDefId iDefId;
		TMdCValueUnion iName;
	};


NONSHARABLE_CLASS(TMdCObjectDef) : public TMdCDef
	{
	public:
		IMPORT_C TMdCObjectDef();

		IMPORT_C static const TMdCObjectDef& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iParentId;
		TMdCValueUnion iPropertyDefs;
	};

NONSHARABLE_CLASS(TMdCRelationDef) : public TMdCDef
	{
	public:
		IMPORT_C TMdCRelationDef();

		IMPORT_C static const TMdCRelationDef& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
	};
	
NONSHARABLE_CLASS(TMdCEventDef) : public TMdCDef
	{
	public:
		IMPORT_C TMdCEventDef();

		IMPORT_C static const TMdCEventDef& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
	};
	
NONSHARABLE_CLASS(TMdCPropertyDef) : public TMdCDef
	{
	public:
		IMPORT_C TMdCPropertyDef();
		
		IMPORT_C static const TMdCPropertyDef& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TUint16 iFlags;
		TUint16 iValueType;
		TMdCValueUnion iMinValue;
		TMdCValueUnion iMaxValue;
	};

NONSHARABLE_CLASS(TMdCNamespaceDef) : public TMdCDef
	{
	public:
		IMPORT_C TMdCNamespaceDef();
		
		IMPORT_C static const TMdCNamespaceDef& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);
	
	public:
		TBool iReadOnly;
		TMdCValueUnion iObjectDefs;
		TMdCValueUnion iRelationDefs;
		TMdCValueUnion iEventDefs;
	};

NONSHARABLE_CLASS(TMdCSchema) : protected TMdCSerializationType
	{
	public:
		IMPORT_C TMdCSchema();

		IMPORT_C static const TMdCSchema& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TMdCValueUnion iNamespaceDefs;
	};

#endif /*__MDCDEF_H__*/
