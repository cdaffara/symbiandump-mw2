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
* Description:  Class to deserialize client query cliteria
*
*/

#ifndef __MDSQUERYCRITERIADESERIALIZATION_H__
#define __MDSQUERYCRITERIADESERIALIZATION_H__

#include <e32cmn.h>
#include <e32def.h>
#include <e32des8.h>

#include "mdccommon.h"
#include "mdcserializationbuffer.h"
// added for serialization definitions !!
#include "mdscommoninternal.h"
// end

/**
 * Query criteria serialization.
 */
class CMdSQueryCriteriaDeserialization : public CBase
	{
	friend class CMdSFindSqlClause;
	
public:

	/* Constructors and destructor. */

	/**
	 * Constructs a new serialized query criteria.
	 * @param aCondition  root logic condition to serialize
	 *
	 * @return  query criteria serialization instance
	 *
	 * @leave KErrNotSupported result mode and query type don't match
	 * @leave KErrCompletion   result mode is EModeDistinctValues and query 
	 *                         criteria is incorrect
	 */
	static CMdSQueryCriteriaDeserialization* NewL(CMdCSerializationBuffer& aSerializationBuffer);

	/**
	 * Constructs a new serialized query criteria and leaves it in the
	 * cleanup stack.
	 * @param aCondition  root logic condition to serialize
	 *
	 * @return  query criteria serialization instance
	 *
	 * @leave KErrNotSupported result mode and query type don't match
	 * @leave KErrCompletion   result mode is EModeDistinctValues and query 
	 *                         criteria is incorrect
	 */
	static CMdSQueryCriteriaDeserialization* NewLC(CMdCSerializationBuffer& aSerializationBuffer);

	/**
	 * Destructor.
	 */
	virtual ~CMdSQueryCriteriaDeserialization();

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

protected:

	/**
	* Returns query's result mode.
	*
	* @return query result mode
	*/
	TQueryResultMode ResultModeL();

	/**
	* Returns query's type.
	*
	* @return query type
	*/
	TQueryType QueryTypeL();

	/**
	* Returns query's namespace definition ID.
	*
	* @return namespace definition ID
	*/
	TDefId NamespaceDefIdL();

	/**
	* Returns query's object definition ID.
	*
	* @return object definition ID
	*/
	TDefId ObjectDefIdL();

	/**
	* Moves position to begin of query's object definitions IDs.
	* 
	* @return if offset is KNoOffset return EFalse, else return ETrue and move 
	*         to that position
	*/
	TBool ObjectDefsIdsL();

	/**
	* Returns query's result's limit value.
	*
	* @return limit value
	*/
	TUint32 LimitL();

	/**
	* Returns query's internal optimization flags.
	*
	* @return query's internal optimization flags
	*/
	TUint32 OptimizationFlagsL();

	/**
	* Returns query's result's offset value.
	*
	* @return offset value
	*/
	TUint32 OffsetL();

	/**
	* Moves position to begin of query's root condition.
	*/
	void RootConditionL();

	/**
	* Moves position to begin of query's order rules.
	*/
	void OrderRulesL();

	/**
	* Moves position to begin of query's property filter.
	*/
	void PropertyFiltersL();

private:

	/* Constructors. */

	/**
	 * Constructor.
	 */
	CMdSQueryCriteriaDeserialization(CMdCSerializationBuffer& aSerializedBuffer);
	
private:

    /** Serialized query criteria. */
    CMdCSerializationBuffer& iSerializedBuffer;
	};


#endif  // __MDEQUERYCRITERIASERIALIZATION_H__
