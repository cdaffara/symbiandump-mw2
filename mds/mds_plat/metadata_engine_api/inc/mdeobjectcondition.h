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


#ifndef __MDEOBJECTCONDITION_H__
#define __MDEOBJECTCONDITION_H__


#include <mdecondition.h>
#include <mderange.h>

/* Forward declarations. */
class CMdEObjectDef;


/**
 * Query condition for objects.
 */
NONSHARABLE_CLASS(CMdEObjectCondition) : public CMdECondition
	{
public:

	/* Constructors and destructor. */
/**
 * Two-phased constructor.
 */

	static CMdEObjectCondition* NewL( TObjectConditionCompareMethod aCompareMethod,
			TItemId aObjectId = KNoId, 
			const CMdEObjectDef* aObjectDef = NULL,
			TInt64 aObjectGuidHigh = 0, TInt64 aObjectGuidLow = 0 );

	static CMdEObjectCondition* NewLC( TObjectConditionCompareMethod aCompareMethod,
			TItemId aObjectId = KNoId,
			const CMdEObjectDef* aObjectDef = NULL,
			TInt64 aObjectGuidHigh = 0, TInt64 aObjectGuidLow = 0 );

	static CMdEObjectCondition* NewL( TObjectConditionCompareMethod aCompareMethod,
			const RArray<TItemId>* aObjectIds = NULL,
			const TDesC* aString = NULL,
			const TMdEUintRange* aRange = NULL );

	static CMdEObjectCondition* NewLC( TObjectConditionCompareMethod aCompareMethod,
			const RArray<TItemId>* aObjectIds = NULL,
			const TDesC* aString = NULL,
			const TMdEUintRange* aRange = NULL );


	/**
	 * Destructor.
	 */
	virtual ~CMdEObjectCondition();

	/* Methods. */

    /**
     * Returns the object definition of the condition.
     *
     * @return  Object definition. <code>NULL</code>, if not defined.
     */
	IMPORT_C const CMdEObjectDef* ObjectDef() const;

    /**
     * Returns the object ID number of the condition.
     *
     * @return  Object ID. Zero, if not defined.
     */
    IMPORT_C TItemId ObjectId() const;

    /**
     * Returns the object ID numbers of the condition.
     *
     * @return  Object IDs. <code>NULL</code>, if not defined.
     */
    IMPORT_C const RArray<TItemId>* ObjectIds() const;

    /**
     * Returns the string of the condition.
     *
     * @return  String descriptor. <code>NULL</code>, if not defined.
     */
    IMPORT_C const TDesC* String() const;

    /**
     * Returns the comparison method of the condition.
     *
     * @return  Comparison method.
     */
    IMPORT_C TObjectConditionCompareMethod CompareMethod() const;

    /**
     * Returns the level of confidentiality of the condition.
     *
     * @return  Confidentiality level of condition.
     */
    IMPORT_C TObjectConditionConfidentialityLevel ConfidentialityLevel() const;

    /**
     * Sets the confidentiality level for the condition.
     * Default level is ELevelNormal.
     *
     * @param  aLevel focuses the search to confidential or
     *         non-confidential  objects or both of them.
     */
    IMPORT_C void SetConfidentialityLevel( TObjectConditionConfidentialityLevel aLevel );

    /**
     * Determines whether the condition requires the object
     * to be a present object or not.
     *
     * @return  <code>ETrue</code>, if not present objects are requested;
     *          <code>EFalse</code> otherwise.
     */
    IMPORT_C TBool NotPresent() const;

    /** 
     * Sets whether the condition requires the object
     * to be a present object or not.
     *
     * @param aNotPresent  <code>ETrue</code>, if the object should 
     *                                         be a not present object.
     *                     <code>EFalse</code> otherwise.
     */
    IMPORT_C void SetNotPresent(TBool aNotPresent);

    /**
     * Determines whether the condition requires the object
     * to be a not placeholder object.
     *
     * @return  <code>ETrue</code>, if not placeholder objects are excluded;
     *          <code>EFalse</code> otherwise.
     */
    IMPORT_C TBool NotPlaceholder() const;

    /** 
     * Sets whether the condition requires the object
     * to be a not placeholder object.
     *
     * @param aNotPlaceholder  <code>ETrue</code>, if the object should 
     *                                             be a not placeholder object.
     *                         <code>EFalse</code> otherwise.
     */
    IMPORT_C void SetNotPlaceholder(TBool aNotPlaceholder);

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
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    void SerializeL(CMdCSerializationBuffer& aBuffer,
    		TMdCOffset& aFreespaceOffset) const;
    
    /**
     * Determines whether the condition requires the object
     * to be a not placeholder object.
     *
     * @return  <code>ETrue</code>, if not placeholder objects are excluded;
     *          <code>EFalse</code> otherwise.
     */
    IMPORT_C TBool PlaceholderOnly() const;

    /** 
     * Sets whether the condition requires the object
     * to be a not placeholder object.
     *
     * @param aNotPlaceholder  <code>ETrue</code>, if the object should 
     *                                             be a not placeholder object.
     *                         <code>EFalse</code> otherwise.
     */
    IMPORT_C void SetPlaceholderOnly( TBool aPlaceholderOnly );

private:

	/**
	 * Constructor.
	 *
     * @param aCompareMethod  Comparison method.
     * @param aObjectId  ID number of an object.
	 * @param aObjectDef     Object definition.
     * @param aObjectGuidHigh  object's GUID high part.
     * @param aObjectGuidLow   object's GUID low part.
	 */
	CMdEObjectCondition( TObjectConditionCompareMethod aCompareMethod,
				TItemId aObjectId, const CMdEObjectDef* aObjectDef,
				TInt64 aObjectGuidHigh, TInt64 aObjectGuidLow );

	/**
	 * Constructor.
	 *
     * @param aCompareMethod  Comparison method.
	 */
	CMdEObjectCondition( TObjectConditionCompareMethod aCompareMethod );

    /**
     * Second-phase constructor.
     */
    void ConstructL();

    /**
     * Second-phase constructor.
     * 
     * @param aString         Text string to look for.
     * @param aObjectIds      object IDs to look for.
     */
    void ConstructL( const RArray<TItemId>* aObjectIds,
    		const TDesC* aString, const TMdEUintRange* aRange );

private:

    /** Object definition to match. <code>NULL</code>, if not applicable. */
    const CMdEObjectDef* iObjectDef;

    /** The object's ID. */
    TItemId iObjectId;

    /** The objects' ID. */
    RArray<TItemId>* iObjectIds;

    /** String to match. <code>NULL</code>, if not applicable. */
    HBufC* iString;

    /** Compare method for the string. */
    TObjectConditionCompareMethod iCompareMethod;

    /** 
     * Object condition flags 
     * not present 
     * not placeholder
     */
    TUint32 iFlags;

    /** 
     * Determines confidentiality level of result object.
     * By default level is ELevelNormal, which means that
     * query returns only non-confidential objects as result.
     */
    TObjectConditionConfidentialityLevel iConfidentialityLevel;

    /** Object's GUID high part. */
    TInt64 iGuidHigh;

    /** Object's GUID low part. */
    TInt64 iGuidLow;
    
    TMdEUintRange* iRange;
    TBool iPlaceholdersOnly;
    };

#endif  // __MDEOBJECTCONDITION_H__
