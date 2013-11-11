/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Property conditions for boolean property values
*
*/


#ifndef __MDEBOOLPROPERTYCONDITION_H__
#define __MDEBOOLPROPERTYCONDITION_H__


#include <mdepropertycondition.h>

/* Forward declarations. */
class CMdEPropertyDef;
class CMdCSerializationBuffer;

/**
 * Property conditions for boolean property values. The condition is true 
 * when the property's value matches the condition's boolean value.
 */
NONSHARABLE_CLASS(CMdEBoolPropertyCondition) : public CMdEPropertyCondition
	{
	public:

	/**
	 *  First-phase constructor
	 */
	static CMdEBoolPropertyCondition* NewL( const CMdEPropertyDef& aPropertyDef, 
            TBool aValue );

	/**
	 *  First-phase constructor
	 */
	static CMdEBoolPropertyCondition* NewLC( const CMdEPropertyDef& aPropertyDef, 
            TBool aValue );

	/**
	 * Destructor.
	 */
	virtual ~CMdEBoolPropertyCondition();


	/* Methods. */

    /**
     * Returns the boolean condition value.
     */
    IMPORT_C TBool Value() const;

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
    
private:
	/**
	 * Constructor. 
	 *
	 * @param aValue  boolean value
	 */
	CMdEBoolPropertyCondition(const CMdEPropertyDef& aPropertyDef, 
                              TBool aValue);

	/**
	 * Second-phase constructor.
	 */
	void ConstructL();

private:

    /** The boolean value to compare against. */
    TBool iValue;
    };

#endif  // __MDEBOOLPROPERTYCONDITION_H__
