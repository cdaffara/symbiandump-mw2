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


#ifndef __MDERELATIONCONDITION_H__
#define __MDERELATIONCONDITION_H__


#include <mdccommon.h>
#include <mdecondition.h>
#include <mderange.h>


/* Forward declarations. */
class CMdELogicCondition;
class CMdERelationDef;

/**
 * Search condition for relations.
 */
NONSHARABLE_CLASS(CMdERelationCondition) : public CMdECondition
	{
public:

	/* Constructors and destructor. */

	static CMdERelationCondition* NewL( TItemId aRelationId = KNoId,
			const RArray<TItemId>* aRelationIds = NULL, 
			TRelationConditionSide aSide = ERelationConditionSideEither,
			const CMdERelationDef* aRelationDef = NULL,
			const TMdEIntRange* aParameterRange = NULL );

	static CMdERelationCondition* NewLC( TItemId aRelationId = KNoId,
			const RArray<TItemId>* aRelationIds = NULL, 
			TRelationConditionSide aSide = ERelationConditionSideEither,
			const CMdERelationDef* aRelationDef = NULL,
			const TMdEIntRange* aParameterRange = NULL );

	/**
	 * Destructor.
	 */
	virtual ~CMdERelationCondition();

	/* Methods. */

    /**
     * Returns the relation ID.
     *
     * @return Relation ID.
     */
    IMPORT_C TItemId RelationId() const;

    /**
     * Returns the relation IDs.
     *
     * @return Relation IDs.
     */
    IMPORT_C const RArray<TItemId>* RelationIds() const;
    
    /**
     * Returns a pointer to the relation definition.
     *
     * @return  Relation definition; or <code>NULL</code>,
     *          if no relation definition has been associated with this
     *          condition.
     */
    IMPORT_C const CMdERelationDef* RelationDef() const;

    /**
     * Returns the conditions for the object on the left side of the relation.
     *
     * @return  Logic condition node.
     */
    IMPORT_C CMdELogicCondition& LeftL();

    /**
     * Returns the conditions for the object on the right side of the 
     * relation.
     *
     * @return  Logic condition node.
     */
    IMPORT_C CMdELogicCondition& RightL();

    /**
     * Returns the side on which a matched object must be.
     * @return The side on which an object must be.
     */
    TRelationConditionSide Side() const;

    /**
     * Sets the side on which a matched object must be.
     * @return The side on which an object must be.
     */
    IMPORT_C void SetSide(TRelationConditionSide aSide);

    /**
     * Returns the relation parameter range.
     * @return Parameter's range conditions 
     */
    IMPORT_C const TMdEIntRange* ParameterRange() const;

	/**
	 * Sets the GUID which a relation must match.
	 *
	 * @param aGuidHigh new GuidHigh value
	 * @param aGuidLow new GuidLow value
	 */
	IMPORT_C void SetGuid(const TInt64& aGuidHigh, const TInt64& aGuidLow);

	/**
	 * Gets the GUID which a relation must match.
	 *
	 * @param aGuidHigh reference where GuidHigh will be stored
	 * @param aGuidLow reference where GuidLow will be stored
	 * @return returns ETrue if relation condition contains GUID condition, 
	 *         otherwise doesn't change parameters' values and returns EFalse
	 */
	IMPORT_C TBool Guid(TInt64& aGuidHigh, TInt64& aGuidLow) const;

	/**
	 * Sets the last modified date range which a relation must match.
	 *
	 * @param aLastModifiedTimeRange last modified date range
	 */
	IMPORT_C void SetLastModifiedDateRangeL(const TMdETimeRange& aLastModifiedDateRange);

	/**
	 * Gets the last modified date range which a relation must match.
	 *
	 * @param aLastModifiedDateRange reference where last modified date range will be stored
	 * @return returns ETrue if relation condition contains last modified date range condition, 
	 *         otherwise doesn't change parameter's value and returns EFalse
	 */
	IMPORT_C TBool LastModifiedDateRange(TMdETimeRange& aLastModifiedDateRange) const;

    /**
     * Get possible query's internal optimization flags from condition.
     *
     * @param aFlaga query's internal optimization flags
     * @return last level internal optimization flags
     */
    TUint32 InternalQueryOptimizationFlags(TUint32& aFlags);

	/**
     * Get required size of serialized buffer when it is serialized.
     *
     * @return required serialized buffer size
     */
	TUint32 RequiredBufferSize() const;
	
	/**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling).
	 *
     * @param aBuffer  serialized buffer.
     */
    void SerializeL(CMdCSerializationBuffer& aBuffer,
    		TMdCOffset& aFreespaceOffset) const;

protected:

    /* Constructor. */


	/* Utility methods. */


private:

    /* Private methods. */

  	/**
	 * Constructor.
	 *
     * @param aRelationId  The relation ID which an relation must match.
     * @param aSide  The side on which an object must be.
	 * @param aRelationDef  relation definition
	 */
	CMdERelationCondition(TItemId aRelationId, TRelationConditionSide aSide, 
			const CMdERelationDef* aRelationDef);

    /**
	 * Second-phase constructor. Creates two LogicConditions as children of 
     * this relation condition and creates range condition for parameter. 
     * Ownership of the parameter stays with caller.
     *
     * @param aRelationIds  The relation IDs which an relation must match.
     * @param aParameterRange parameter range conditions
	 */
    void ConstructL(const RArray<TItemId>* aRelationIds, 
    		const TMdEIntRange* aParameterRange);


private:

	const TItemId iRelationId;
	
	RArray<TItemId>* iRelationIds;

    /** Relation definition. */
    const CMdERelationDef* iRelationDef;

    /** Conditions for the left object. */
    CMdELogicCondition* iLeftCondition;

    /** Conditions for the right object. */
    CMdELogicCondition* iRightCondition;

    /** The side on which an object must be. */
    TRelationConditionSide iSide;

    /** Conditions for the relation parameter. */
    TMdEIntRange* iParameterRange;
    
    /** Conditions for the relation GUID. */
    TInt64 iGuidHigh;
    TInt64 iGuidLow;
    
    /** Conditions for the relation last modified date. */
    TMdETimeRange* iLastModifiedDateRange;
    };

#endif  // __MDERELATIONCONDITION_H__
