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


#ifndef __MDELOGICCONDITION_H__
#define __MDELOGICCONDITION_H__


#include <e32std.h>

#include <mdecondition.h>
#include <mderange.h>
#include <mderangepropertycondition.h>
#include <mdetextpropertycondition.h>


/* Forward declarations. */
class CMdEBoolPropertyCondition;
class CMdEObjectDef;
class CMdEPropertyDef;
class CMdERelationDef;
class CMdEEventDef;
class CMdCSerializationBuffer;
class CMdEEventCondition;
class CMdEObjectCondition;
class CMdEPropertyCondition;
class CMdERelationCondition;

/**
 * Logic conditions are used for combining other conditions with boolean
 * AND or OR operators. Each logic condition can have an unlimited number of
 * child conditions. The same operator is applied to all of the child 
 * conditions: (X and Y and Z), (X or Y or Z).
 *
 * CMdELogicCondition provides a number of factory methods for creating and
 * adding new nodes as children of the logic condition node. Always use the
 * factory methods when constructing a query condition tree.
 *
 * A logic condition node owns all of its children.
 */
NONSHARABLE_CLASS(CMdELogicCondition) : public CMdECondition
	{
public:

	/* Constructors and destructor. */

    /**
     * Constructs a new logic condition node.
     *
     * @param aOperator  Operator to use for the child conditions.
     */
    IMPORT_C static CMdELogicCondition* NewL(TLogicConditionOperator aOperator);

    /**
     * Constructs a new logic condition node, leaving the node on the 
     * cleanup stack.
     *
     * @param aOperator  Operator to use for the child conditions.
     */
    IMPORT_C static CMdELogicCondition* NewLC(TLogicConditionOperator aOperator);


	/**
	 * Destructor.
	 */
	virtual ~CMdELogicCondition();


	/* Methods. */

    /**
     * Returns the operator of the logic condition.
     *
     * @return  Logic operator.
     */
    IMPORT_C TLogicConditionOperator Operator() const;

    /**
     * Changes the operator of the logic condition.
     *
     * @param aLogicOperator  The new logic operator for the condition.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C void SetOperator(TLogicConditionOperator aLogicOperator);

    /**
     * Determines the number of child conditions.
     *
     * @return  Number of conditions in the logic operation.
     */
    IMPORT_C TInt Count() const;

    /**
     * Returns a child condition node.
     *
     * @param aIndex  Index of the condition to return.
     *
     * @return  Condition node at the specified index.
     */
    IMPORT_C CMdECondition& Condition(TInt aIndex) const;

    /**
     * Removes a condition node from the logic condition.
     *
     * @param aIndex  Index of the condition to remove.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C void Remove(TInt aIndex);
    

    /* Factory methods for creating and adding new condition nodes into 
     * the tree. */

    /** 
     * Creates a new logic condition and makes it a child condition of this
     * logic condition node.
     *
     * @param aLogicOperator  Operator of the new logic condition.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdELogicCondition& AddLogicConditionL(TLogicConditionOperator aLogicOperator);

    /** 
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. The object condition node sets a requirement for
     * the object class.
     *
     * Example:
     *   // We want the condition to match with image objects.
     *   CMdELogicCondition* condition = CMdELogicCondition::NewL( ELogicConditionOperatorAnd );
     *   CleanupStack::PushL( condition );
     *
     *   CMdENamespaceDef& defaultNamespace = iMdeSession->GetDefaultNamespaceDefL();
     *   CMdEObjectDef& imageDef = defaultNamespace.GetObjectDefL( MdeConstants::Image::KImageObject );
     *   condition->AddObjectConditionL( imageDef );
     * 
     * @param aObjectDef     Object definition.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEObjectCondition& 
        AddObjectConditionL(const CMdEObjectDef& aObjectDef);

    /**
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. The object condition node sets a requirement for
     * the object's ID number.
     *
     * Example:
     *   CMdERelationQuery* relationQuery = iMdeSession->NewRelationQueryL( defNamespace, this );
     *   CMdERelationCondition& filterCond = relationQuery->Conditions().
     *	     AddRelationConditionL( ERelationConditionSideRight );
     *   // Left object in relation must have this ID.
     *   filterCond.LeftL().AddObjectConditionL( aObjectId );
     * 
     * @param aObjectId  ID number of the object.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEObjectCondition& AddObjectConditionL(TItemId aObjectId);

    /**
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. The object condition node sets a requirement for
     * the objects' ID numbers.
     *
     * @param aObjectId  ID numbers of objects.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     * 
     */
    IMPORT_C CMdEObjectCondition& AddObjectConditionL(
    		const RArray<TItemId>& aObjectIds);

    /**
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. The object condition node sets a requirement for
     * the object's GUID.
     *
     * @param aObjectGuidHigh  GUID high part of the object.
     * @param aObjectGuidLow  GUID low part of the object.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEObjectCondition& AddObjectConditionL(
    		TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow);

    /**
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. 
     *
     * @param aCompareMethod  Comparison method.
     * @param aString         Text string to look for.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEObjectCondition& AddObjectConditionL(
    		TObjectConditionCompareMethod aCompareMethod, 
    		const TDesC& aString);

    /**
     * Creates a new object condition and makes it a child condition of this
     * logic condition node. 
     *
     * @param aRange  range for usage count
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEObjectCondition& AddObjectConditionL(
    		const TMdEUintRange& aRange);

    /**
     * Creates a new property condition and makes it a child condition of this
     * logic condition node. The property condition tests if a property 
     * exists in an object.
     *	
     * Example:
	 *   CMdEPropertyDef& commentDef = objDef.GetPropertyDefL( MdeConstants::MediaObject::KCommentProperty );
	 *   iQuery->Conditions().AddPropertyConditionL( commentDef );				
	 *
     * @param aPropertyDef  The property to look for.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEPropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef);

    /**
     * Creates a new boolean property condition and makes it a child 
     * condition of this logic condition node. The property condition
     * tests the value of a boolean property.
     *
     * Example:
	 *   CMdEPropertyDef& commentDef = objDef.GetPropertyDefL( MdeConstants::MediaObject::KProtectedProperty );
	 *   iQuery->Conditions().AddPropertyConditionL( commentDef, ETrue );	
     * 
     * @param aPropertyDef  Property definition.
     * @param aBoolValue    Boolean value.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEBoolPropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef, TBool aBoolValue);

    /**
     * Creates a new integer property condition and makes it a child 
     * condition of this logic condition node. The property condition defines
     * a range of integer values. If the property's value is within the range
     * the condition evaluates to True.
     *
     * Example:
	 *   CMdEPropertyDef& frameCountDef = objDef.GetPropertyDefL( MdeConstants::Image::FrameCount );
	 *   iQuery->Conditions().AddPropertyConditionL( frameCountDef, TMdEIntRange(0, 5, rangeType) );
     *
     * @param aPropertyDef  Property definition.
     * @param aIntRange     Range of integer values.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEIntRangePropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef, 
    		const TMdEIntRange& aIntRange);

    /**
     * Creates a new 64 bit integer property condition and makes it a child 
     * condition of this logic condition node. The property condition defines
     * a range of integer values. If the property's value is within the range
     * the condition evaluates to True.
     *
     * @param aPropertyDef  Property definition.
     * @param aInt64Range     Range of 64 bit integer values.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEInt64RangePropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef,
    		const TMdEInt64Range& aIntRange);

    /**
     * Creates a new unsigned integer property condition and makes it a child 
     * condition of this logic condition node. The property condition defines
     * a range of unsigned integer values. If the property's value is within 
     * the range the condition evaluates to True.
     *
     * @param aPropertyDef  Property definition.
     * @param aUintRange    Range of unsigned integer values.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEUintRangePropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef,
            const TMdEUintRange& aUintRange);

    /**
     * Creates a new floating-point property condition and makes it a child 
     * condition of this logic condition node. The property condition defines
     * a range of floating-point values. If the property's value is within 
     * the range the condition evaluates to True.
     *
     * @param aPropertyDef  Property definition.
     * @param aRealRange    Range of floating-point values.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERealRangePropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef,
            const TMdERealRange& aRealRange);

    /**
     * Creates a new time property condition and makes it a child 
     * condition of this logic condition node. The property condition defines
     * a range of time values. If the property's value is within the range
     * the condition evaluates to True.
     *
     * @param aPropertyDef  Property definition.
     * @param aTimeRange    Range of time values.
     *
     * @return  Reference to the new condition node.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdETimeRangePropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef,
            const TMdETimeRange& aTimeRange);

    /**
     * Creates a new text property condition and makes it a child condition 
     * of this logic condition node.
     *
     * @param aPropertyDef   Property definition.
     * @param aCompareMethod Comparison method.
     * @param aText          Text string to look for.
     * 
     * @return Reference to the new condition node.
     * 
     * @panic TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdETextPropertyCondition& AddPropertyConditionL(
    		const CMdEPropertyDef& aPropertyDef,
    		TTextPropertyConditionCompareMethod aCompareMethod,
            const TDesC& aText);

    /**
     * Creates a new relation condition and makes it a child condition of this
     * logic condition node. The new relation condition contains no parameters
     * by default. It is assumed that the relation condition is augmented
     * with further conditions for the objects to which the relation is 
     * attached.
     *
     * Example:
     *   TItemId relationId( 1 );
     *   CMdERelationCondition& relationCond = relationQuery->Conditions().
     *		AddRelationConditionL( relationId, ERelationConditionSideRight );
     * 
     * @param aRelationId  Relation ID.
     * @param aSide The side on which an object must be.
     * 
     * @return Reference to the new condition node.
     * 
     * @panic TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERelationCondition& AddRelationConditionL(TItemId aRelationId, 
    		TRelationConditionSide aSide = ERelationConditionSideEither);

    /**
     * Creates a new relation condition and makes it a child condition of this
     * logic condition node. The new relation condition contains no parameters
     * by default. It is assumed that the relation condition is augmented
     * with further conditions for the objects to which the relation is 
     * attached.
     * 
     * Example:
     *   RArray<TItemId> arrayOfIds;
     *   arrayOfIds.Append( TItemId( 1 ) );
     *   arrayOfIds.Append( TItemId( 2 ) );
     *   CMdERelationCondition& relationCond = relationQuery->Conditions().
     *		AddRelationConditionL( arrayOfIds, ERelationConditionSideRight );
     *
     * @param aRelationIds Array of Relation IDs.
     * @param aSide The side on which an object must be.
     * 
     * @return Reference to the new condition node.
     * 
     * @panic TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERelationCondition& AddRelationConditionL(
    		RArray<TItemId>& aRelationIds, 
        	TRelationConditionSide aSide = ERelationConditionSideEither);
    
    /**
     * Creates a new relation condition and makes it a child condition of this
     * logic condition node. The new relation condition contains no parameters
     * by default. It is assumed that the relation condition is augmented
     * with further conditions for the objects to which the relation is 
     * attached.
     * 
     * Example:
     *   CMdERelationCondition& relationCond = relationQuery->Conditions().
     *		AddRelationConditionL( ERelationConditionSideRight );
     *
     * @param aSide The side on which an object must be.
     *
     * @return Reference to the new condition node.
     * 
     * @panic TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERelationCondition& AddRelationConditionL(
        		TRelationConditionSide aSide = ERelationConditionSideEither);

    /**
     * Creates a new relation condition and makes it a child condition of this
     * logic condition node. The new relation condition tests the relation's 
     * type. The relation condition can be augmented with further conditions
     * for the objects to which the relation is attached.
     *
	 * @param aRelationDef  relation definition
     * @param aSide The side on which an object must be. Defaults to 
     *              ERelationConditionSideEither.
     * 
     * @return Reference to the new condition node.
     * 
     * @panic TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERelationCondition& AddRelationConditionL(
    		const CMdERelationDef& aRelationDef, 
    		TRelationConditionSide aSide = ERelationConditionSideEither);

    /**
     * Creates a new relation condition and makes it a child condition of this
     * logic condition node. The new relation condition tests the relation's 
     * type. The relation condition can be augmented with further conditions
     * for the objects to which the relation is attached.
     *
	 * @param aRelationDef  relation definition
     * @param aIntRange parameter range conditions
     * @param aSide  The side on which an object must be.
     * 
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdERelationCondition& AddRelationConditionL(
    		const CMdERelationDef& aRelationDef, const TMdEIntRange& aIntRange,
    		TRelationConditionSide aSide = ERelationConditionSideEither);


    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition contains no parameters
     * by default. It is assumed that the event condition is augmented
     * with further conditions for the object to which the event is attached.
     * 
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL();

    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition tests the event's type.
     * The event condition can be augmented with further conditions
     * for the object to which the event is attached.
     * 
     * @param aEventId  Event ID.
     *
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL(TItemId aEventId);

    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition tests the event's type.
     * The event condition can be augmented with further conditions
     * for the object to which the event is attached.
     * 
     * @param aEventDef  Event definition.
     *
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL(
    		const CMdEEventDef& aEventDef);

    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition tests the event's 
     * creation time. The event condition can be augmented with further 
     * conditions for the object to which the event is attached.
     * 
     * @param aCreationTimeRange  Time range.
     *
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL(
    		const TMdETimeRange& aCreationTimeRange);

    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition tests the event's 
     * source or participant. The event condition can be augmented with 
     * further conditions for the object to which the event is attached.
     * 
     * @param aCompareMethod  Comparison method.
     * @param aString         Text string to look for.
     *
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL(
    		TEventConditionCompareMethod aCompareMethod, const TDesC& aString);

    /**
     * Creates a new event condition and makes it a child condition of this
     * logic condition node. The new event condition tests the event's 
     * creation time. The new event condition also tests the event's 
     * source or participant. The event condition can be augmented with 
     * further conditions for the object to which the event is attached.
     * 
     * @param aCreationTimeRange  Time range.
     * @param aCompareMethod      Comparison method.
     * @param aString             Text string to look for.
     *
     * @return  Reference to the new condition node.
     * 
     * @panic  TMdEPanic::EConditionLocked  Condition node is locked.
     */
    IMPORT_C CMdEEventCondition& AddEventConditionL(
    		const TMdETimeRange& aCreationTimeRange,
            TEventConditionCompareMethod aCompareMethod, const TDesC& aString);

    /**
     * Locking a logic condition causes all children to be locked as well.
     * Attempting to modify a locked condition causes a panic.
     *
     * @param aLocked  <code>ETrue</code> to lock; 
     *                 <code>EFalse</code> to unlock.
     */
    void SetLocked(TBool aLocked = ETrue);
    
    /**
     * Get possible query's internal optimization flags from condition.
     *
     * @param aFlaga query's internal optimization flags
     * @return last level internal optimization flags
     */
    TUint32 InternalQueryOptimizationFlags(TUint32& aFlags);

    /**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
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

	/* Constructors. */

	/**
	 * Constructor. 
	 *
	 * @param aOperator  Logic operator.
	 */
	CMdELogicCondition(TLogicConditionOperator aOperator);

	/**
	 * Second-phase constructor.
	 */
	void ConstructL();


	/* Utility methods. */

    /**
     * Makes an existing condition node a child node of this logic condition.
     * The logic condition takes ownership of the node.
     *
     * @param aCondition  Condition node.
     *
     * @return  Reference to the added node (aCondition).
     */
    CMdECondition& AddL(CMdECondition* aCondition);


private:

    /** Type of the condition. */
    TLogicConditionOperator iOperator;

    /** Children of the condition. The logic condition owns all the 
        child nodes. */
    RPointerArray<CMdECondition> iChildren;

	TUint32 iOptimizationFlags;
    };

#endif  // __MDELOGICCONDITION_H__
