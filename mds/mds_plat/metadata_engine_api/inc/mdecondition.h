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
* Description:  Base class for all search condition tree nodes.
*
*/


#ifndef __MDECONDITION_H__
#define __MDECONDITION_H__


#include <e32base.h>

#include <mdccommon.h>

// Forward declaration
class CMdCSerializationBuffer;

/**
 * Base class for all search condition tree nodes.
 */
NONSHARABLE_CLASS(CMdECondition) : public CBase
	{
public:

	/* Destructor. */

	/**
	 * Destructor.
	 */
	virtual ~CMdECondition();


	/* Methods. */

    /**
	 * Returns the type of the condition.
	 *
	 * @return  Condition type.
	 */
	IMPORT_C TConditionType Type() const;

    /**
	 * Determines whether the condition is negated.
	 *
	 * @return  <code>ETrue</code>, if the condition is negated.  
     *          Otherwise <code>EFalse</code>.
	 */
	IMPORT_C TBool Negate() const;

    /**
	 * Sets the negate flag. The negate flag negates the meaning of the 
     * condition like a boolean NOT operator.
	 *
	 * @param aNegate  <code>ETrue</code> to negate.
	 */
	IMPORT_C void SetNegate(TBool aNegate);

    /**
     * Determines whether the condition node has been locked. Attempting to 
     * modify locked conditions causes a panic.
     *
     * @return  <code>ETrue</code>, if the condition is locked. Otherwise
     *          <code>EFalse</code>.
     */
    IMPORT_C TBool Locked() const;     

    /**
     * Locks the condition to prevent changes. Attempting to change a locked
     * condition causes a panic.
     *
     * @param aLocked  <code>ETrue</code> to lock; <code>EFalse</code> to 
     *                 unlock.
     */
    IMPORT_C virtual void SetLocked(TBool aLocked = ETrue);
    
    /**
     * Get possible query's internal optimization flags from condition.
     *
     * @param aFlaga query's internal optimization flags
     * @return last level internal optimization flags
     */
    virtual TUint32 InternalQueryOptimizationFlags(TUint32& aFlags) = 0;

    /**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
     */
    virtual TUint32 RequiredBufferSize() const = 0;

    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling).
	 *
     * @param aBuffer  serialized buffer.
     */
    virtual void SerializeL(CMdCSerializationBuffer& aBuffer, 
    		TMdCOffset& aFreespaceOffset) const = 0;

	/* Constructors. */

	/**
	 * Constructor. 
	 *
	 * @param aType  Type of the condition node.
	 */
	CMdECondition(TConditionType aType);
	
protected:
	/**
	 * Second-phase constructor.
	 */
	void ConditionConstruct();


	/* Utility methods. */
     
    /**
     * Panics if the condition is not locked.
     *
     * @panic  TMdEPanic::EConditionLocked  Condition is locked against
     *         changes.
     */
    void AssertNotLocked() const;

protected:

    /** Type of the condition. */
    TConditionType iType;

    /** The negate flag negates the result of the condition: if the 
        condition evaluates to True, the result will really be False. */
    TBool iNegated;
    
    /** Locked conditions can't be modified. */
    TBool iLocked;
    };

#endif  // __MDECONDITION_H__
