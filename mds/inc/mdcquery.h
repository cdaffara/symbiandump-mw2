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
* Description:  common query and condition definitions for MdE
*
*/

#ifndef __MDCQUERY_H__
#define __MDCQUERY_H__

#include "mdscommoninternal.h"
#include "mdcserializationtype.h"


NONSHARABLE_CLASS(TMdCCondition) : protected TMdCSerializationType
	{
	protected:
		TMdCCondition(TMdCStructTypes aType) : TMdCSerializationType( aType )
			{
			}

	public:
		/**
		 * Get condition reference from buffer. Doesn't change position of the 
		 * buffer. Doesn't do type check.
		 * 
		 * @param aBuffer serialization buffer
		 * 
		 * @return reference to condition
		 */
		IMPORT_C static const TMdCCondition& GetFromBufferL( 
				CMdCSerializationBuffer& aBuffer );

	public:
		TInt16 iConditionType;
		TInt16 iNegated;
	};


NONSHARABLE_CLASS(TMdCLogicCondition) : public TMdCCondition
	{
	public:
		IMPORT_C TMdCLogicCondition();

		IMPORT_C static const TMdCLogicCondition& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TInt16  iOperator;
		TUint32 iOptimizationFlags;
		TMdCValueUnion iChildConditions;
	};

NONSHARABLE_CLASS(TMdCObjectCondition) : public TMdCCondition
	{
	public:
		IMPORT_C TMdCObjectCondition();

		IMPORT_C static const TMdCObjectCondition& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TInt16 iConfidentialityLevel;
		TInt16 iCompareMethod;
		TUint32 iFlags;
		TMdCOffset iCondition;
	};
	
NONSHARABLE_CLASS(TMdCEventCondition) : public TMdCCondition
	{
	public:
		IMPORT_C TMdCEventCondition();

		IMPORT_C static const TMdCEventCondition& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TInt16 iCompareMethod;
		TItemId iEventId;
		TDefId iEventDefId;
		TMdCOffset iCreationTimeRange;
		TMdCOffset iObjectCondition;
		TMdCOffset iSourceCondition;
		TMdCOffset iParticipantCondition;
		TMdCOffset iUriCondition;
	};
	
NONSHARABLE_CLASS(TMdCPropertyCondition) : public TMdCCondition
	{
	public:
		IMPORT_C TMdCPropertyCondition();
		
		IMPORT_C static const TMdCPropertyCondition& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TDefId iObjectDefId;
		TDefId iPropertyDefId;
		TMdCOffset iCondition;
	};

NONSHARABLE_CLASS(TMdCRelationCondition) : public TMdCCondition
	{
	public:
		IMPORT_C TMdCRelationCondition();
		
		IMPORT_C static const TMdCRelationCondition& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);

	public:
		TUint16 iObjectSide;
		TDefId iRelationDefId;
		TItemId iRelationId;
		TMdCValueUnion iRelationIds; // IN
		TMdCOffset iGuid;
		TMdCOffset iParameterRange;
		TMdCOffset iLeftObjectCondition;
		TMdCOffset iRightObjectCondition;
		TMdCOffset iLastModifiedDateRange;
	};

NONSHARABLE_CLASS(TMdCSearchCriteria) : protected TMdCSerializationType
	{
	public:
		IMPORT_C TMdCSearchCriteria();
		
		IMPORT_C static const TMdCSearchCriteria& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);
		
	public:
		TInt16 iQueryResultType;
		TInt16 iQueryType;
		TDefId iNamespaceDefId;
		TDefId iObjectDefId;
		TMdCValueUnion iObjectDefIds;
		TUint32 iLimit;
		TUint32 iOffset;
		TUint32 iOptimizationFlags;
		TMdCOffset iRootCondition;
		TMdCValueUnion iOrderRules;
		TMdCValueUnion iPropertyFilters;
	};

NONSHARABLE_CLASS(TMdCOrderRule) : protected TMdCSerializationType
	{
	public:
		IMPORT_C TMdCOrderRule();
		
		IMPORT_C static const TMdCOrderRule& GetFromBufferL( CMdCSerializationBuffer& aBuffer );
		IMPORT_C void SerializeL(CMdCSerializationBuffer& aBuffer);
		IMPORT_C void DeserializeL(CMdCSerializationBuffer& aBuffer);
		
	public:
		TInt16 iOrderRule;
		TBool iAscending;
		TBool iCaseSensitive;
		TDefId iPropertyDefId;
	};

	

#endif /*__MDCQUERY_H__*/
