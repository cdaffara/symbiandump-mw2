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
* Description:  Serialize query criteria
*
*/

#ifndef __MDEQUERYCRITERIASERIALIZATION_H__
#define __MDEQUERYCRITERIASERIALIZATION_H__

#include <e32cmn.h>
#include <e32def.h>
#include <e32des8.h>

#include "mdccommon.h"
#include "mdcserializationbuffer.h"
#include "mdeorderrule.h"
#include "mdscommoninternal.h"


// forward declaration
class CMdELogicCondition;
class CMdENamespaceDef;
class CMdEObjectDef;


/**
 * Query criteria serialization.
 */
NONSHARABLE_CLASS(CMdEQueryCriteriaSerialization) : public CBase
	{
	friend class CMdSFindSqlClause;
	
public:

	/* Constructors and destructor. */

	/**
	 * Constructs a new serialized query criteria.
	 * @param aResultMode Query's result type
	 * @param aQueryType  Type of query
	 * @param aNamespaceDef namespace definition
	 * @param aObjectDef 
	 * @param aObjectDefs
	 * @param aLimit 
	 * @param aOffset
	 * @param aQueryOptimizationFlags query optimization flags
	 * @param aRootCondition the root condition
	 * @param aOrderRule array of order rules
	 * @param aPropertyFilter array of property filters
	 * 
	 * @return  query criteria serialization instance
	 *
	 * @leave KErrNotSupported result mode and query type don't match
	 * @leave KErrCompletion   result mode is EModeDistinctValues and query 
	 *                         criteria is incorrect
	 */
	static CMdEQueryCriteriaSerialization* NewL(
		TQueryResultMode aResultMode, 
		TQueryType aQueryType, CMdENamespaceDef& aNamespaceDef, 
		CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
		TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags, 
		CMdELogicCondition& aRootCondition, 
		RArray<TMdEOrderRule>& aOrderRule, RPointerArray<CMdEPropertyDef>* aPropertyFilter);

	/**
	 * Constructs a new serialized query criteria and leaves it in the
	 * cleanup stack.
	 * @param aResultMode Query's result type
	 * @param aQueryType  Type of query
	 * @param aNamespaceDef namespace definition
	 * @param aObjectDef 
	 * @param aObjectDefs
	 * @param aLimit 
	 * @param aOffset
	 * @param aQueryOptimizationFlags query optimization flags
	 * @param aRootCondition the root condition
	 * @param aOrderRule array of order rules
	 * @param aPropertyFilter array of property filters
	 *
	 * @return  query criteria serialization instance
	 *
	 * @leave KErrNotSupported result mode and query type don't match
	 * @leave KErrCompletion   result mode is EModeDistinctValues and query 
	 *                         criteria is incorrect
	 */
	static CMdEQueryCriteriaSerialization* NewLC(
		TQueryResultMode aResultMode, 
		TQueryType aQueryType, CMdENamespaceDef& aNamespaceDef, 
		CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
		TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags, 
		CMdELogicCondition& aRootCondition, 
		RArray<TMdEOrderRule>& aOrderRule, RPointerArray<CMdEPropertyDef>* aPropertyFilter);

	/**
	 * Constructs a new serialized query criteria.
	 * @param aSerializationBuffer serialized query
	 *
	 * @return  query criteria serialization instance
	 */
	static CMdEQueryCriteriaSerialization* NewL(CMdCSerializationBuffer& aSerializationBuffer);

	/**
	 * Constructs a new serialized query criteria and leaves it in the
	 * cleanup stack.
	 * @param aSerializationBuffer serialized query
	 *
	 * @return  query criteria serialization instance
	 */
	static CMdEQueryCriteriaSerialization* NewLC(CMdCSerializationBuffer& aSerializationBuffer);

	/**
	 * Destructor.
	 */
	virtual ~CMdEQueryCriteriaSerialization();

    /**
    * Returns pointer to the serialized data (read only)
    */
    TPtr8 Buffer() const;

    /**
    * Returns pointer to the serialized data (read only)
    */
    const TPtr8* BufferPtr() const;
    
    /**
    * Returns pointer to the serialization buffer
    */
    CMdCSerializationBuffer& SerializationBuffer();

private:

	/* Constructors. */

	/**
	 * Constructor.
	 */
	CMdEQueryCriteriaSerialization(TBool aOwnSerializedBuffer);

	/**
	 * Second-phase constructor.
	 *
	 * @param aResultMode  result mode to serialize
	 * @param aQueryType  query type to serialize
	 * @param aLimit  limit to serialize
	 * @param aOffset  offset to serialize
	 * @param aCondition  root logic condition to serialize
	 * @param aOrderRule  order rules to serialize
	 * @param aPropertyFilter  property filters to serialize
	 */
	void ConstructL( TQueryResultMode aResultMode, TQueryType aQueryType, 
					 CMdENamespaceDef& aNamespaceDef, 
					 CMdEObjectDef* aObjectDef, RPointerArray<CMdEObjectDef>* aObjectDefs, 
					 TUint32 aLimit, TUint32 aOffset, TUint32 aQueryOptimizationFlags, 
					 CMdELogicCondition& aRootCondition, RArray<TMdEOrderRule>& aOrderRules, 
					 RPointerArray<CMdEPropertyDef>* aPropertyFilter );
	
private:

    /** Serialized query criteria. */
    CMdCSerializationBuffer* iSerializedBuffer;

    /** Does own serialized buffer. */
    const TBool iOwnSerializedBuffer;
	};


#endif  // __MDEQUERYCRITERIASERIALIZATION_H__
