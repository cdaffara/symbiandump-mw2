/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  common item result definitions for MdE
*
*/

#ifndef __MDCRESULT_H__
#define __MDCRESULT_H__

#include "mdscommoninternal.h"
#include "mdcserializationtype.h"

NONSHARABLE_CLASS(TMdCItems) : public TMdCSerializationType
	{
	public:
		IMPORT_C TMdCItems();

		IMPORT_C static const TMdCItems& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iNamespaceDefId;
		TInt32 iErrorCode;
		TMdCValueUnion iObjects;
		TMdCValueUnion iRelations;
		TMdCValueUnion iEvents;
	};

NONSHARABLE_CLASS(TMdCItemIds) : public TMdCSerializationType
	{
	public:
		IMPORT_C TMdCItemIds();

		IMPORT_C static const TMdCItemIds& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iNamespaceDefId;
		TInt32 iErrorCode;
		TMdCValueUnion iObjectIds;
		TMdCValueUnion iObjectUris;
		TMdCValueUnion iRelationIds;
		TMdCValueUnion iEventIds;
	};

NONSHARABLE_CLASS(TMdCItemCounts) : public TMdCSerializationType
	{
	public:
		IMPORT_C TMdCItemCounts();

		IMPORT_C static const TMdCItemCounts& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iNamespaceDefId;
		TInt32 iObjects;
		TInt32 iRelations;
		TInt32 iEvents;
	};

	
#endif /*__MDCRESULT_H__*/
