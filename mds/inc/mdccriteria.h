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
* Description:  common criteria object for MdE
*
*/

#ifndef MDCCRITERIA_H_
#define MDCCRITERIA_H_

#include "mdscommoninternal.h"
#include "mdcserializationtype.h"

TMdCSearchCriteria : protected TMdCSerializationType
{
	TInt16 iQueryResultType;
	TInt16 iQueryType;
	TDefId iNamespaceDefId;
	TDefId iObjectDefId;
	TMdCValueUnion iObjectDefIds;
	TUint32 iLimit;
	//TUint32 iOffset; NOT USED
	TUint32 iOptimizationFlags;
	TUint32 iRootConditionOffset;
	TMdCValueUnion iOrderRules;
	TMdCValueUnion iPropertyFilters;
};

TMdCOrderRule : protected TMdCSerializationType
{
	TInt16 iOrderRule;
	TUint16 iFlags;
	TDefId iPropertyDefId;
};

TMdCCondition : protected TMdCSerializationType
{
	TInt16 iConditionType;
	TInt16 iNegated;
};

TMdCLogicCondition : public TMdCCondition
{
	TInt16 iOperator;
	TUint32 iOptimizationFlags;
	TMdCValueUnion iChildConditions;
};

TMdCObjectCondition : public TMdCCondition
{
	TInt16 iConfidentialityLevel;
	TInt16 iCompareMethod;
	TUint32 iFlags;
	TUint32 iConditionOffset;
};

TMdCPropertyCondition : public TMdCCondition
{
	TDefId iObjectDefId;
	TDefId iPropertyDefId;
	TUint32 iConditionOffset;
};

TMdCRelationCondition : public TMdCCondition
{
	TInt16 iObjectSide;
	TItemId iRelationId;
	TMdCValueUnion iRelationIds;
	TDefId iRelationDefId;
	TUint32 iGuidOffset;
	TUint32 iParameterRangeOffset;
	TUint32 iLeftObjectConditionOffset;
	TUint32 iRightObjectConditionOffset;
	TUint32 iLastModifiedDateRangeOffset;
};

TMdCEventCondition : public TMdCCondition
{
	TInt16 iCompareMethod;
	TItemId iEventId;
	TDefId iEventDefId;
	TUint32 iCreationTimeOffset;
	TUint32 iObjectConditionOffset;
	TUint32 iSourceConditionOffset;
	TUint32 iParticipantConditionOffset;
	TUint32 iUriConditionOffset;
};

#endif /*MDCCRITERIA_H_*/
