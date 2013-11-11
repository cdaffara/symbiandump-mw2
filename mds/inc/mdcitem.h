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
* Description:  common item object for MdE
*
*/

#ifndef __MDCITEM_H__
#define __MDCITEM_H__

#include "mdscommoninternal.h"
#include "mdcserializationtype.h"

NONSHARABLE_CLASS(TMdCItem) : protected TMdCSerializationType
	{
	protected:
		TMdCItem(TMdCStructTypes aType) : TMdCSerializationType( aType ), iId( 0 ), iDefId( 0 )
			{
			}
		
	public:
		TItemId iId;
		TDefId  iDefId;
	};

NONSHARABLE_CLASS(TMdCObject) : public TMdCItem
	{
	public:
		IMPORT_C TMdCObject();

		IMPORT_C static const TMdCObject& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TUint32 iFlags;
		TUint32 iMediaId;
		TUint32 iUsageCount;
		TInt64 iGuidHigh;
		TInt64 iGuidLow;
		TMdCValueUnion iUri;
		TMdCValueUnion iProperties;
		TMdCValueUnion iFreeTexts;
	};

NONSHARABLE_CLASS(TMdCRelation) : public TMdCItem
	{
	public:
		IMPORT_C TMdCRelation();

		IMPORT_C static const TMdCRelation& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TItemId iLeftObjectId;
		TItemId iRightObjectId;
		TInt32 iParameter;
		TInt64 iGuidHigh;
		TInt64 iGuidLow;
		TTime iLastModifiedDate;
	};

NONSHARABLE_CLASS(TMdCEvent) : public TMdCItem
	{
	public:
		IMPORT_C TMdCEvent();

		IMPORT_C static const TMdCEvent& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TItemId iObjectId;
		TTime iTime;
		TMdCValueUnion iSourceText;
		TMdCValueUnion iParticipantText;
	};

NONSHARABLE_CLASS(TMdCProperty) : public TMdCSerializationType
	{
	public:
		IMPORT_C TMdCProperty();
		
		IMPORT_C static const TMdCProperty& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iPropertyDefId;
		TUint32 iModFlags;
		TMdCValueUnion iValue;
	};

#endif /* __MDCITEM_H__ */
