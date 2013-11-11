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
* Description:  Represent property condition
*
*/

#ifndef __MDEPROPERTYCONDITION_H__
#define __MDEPROPERTYCONDITION_H__

#include <mdecondition.h>

/* Forward declarations. */
class CMdEPropertyDef;
class CMdCSerializationBuffer;

/**
 * CMdEPropertyCondition is the base class for all property conditions.
 * Instances of CMdEPropertyCondition and its subclasses are always associated
 * with a property definition.
 */
NONSHARABLE_CLASS(CMdEPropertyCondition) : public CMdECondition
	{
public:

    /* Types. */
    
	/**
	 * Two-phased constructor.
	 */
	static CMdEPropertyCondition* NewL( const CMdEPropertyDef& aPropertyDef );

   /**
    * Two-phased constructor.
    */
   static CMdEPropertyCondition* NewLC( const CMdEPropertyDef& aPropertyDef );

    
	/**
	 * Destructor.
	 */
	virtual ~CMdEPropertyCondition();


	/* Methods. */

    /** 
     * Returns the property definition of the condition.
     *
     * @return  Property definition.
     */
    IMPORT_C const CMdEPropertyDef& PropertyDef() const;

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
    virtual TUint32 RequiredBufferSize() const;
   
    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    virtual void SerializeL(CMdCSerializationBuffer& aBuffer,
    		TMdCOffset& aFreespaceOffset) const;
   
protected:

	/**
	 * Constructor for a condition that tests the existence of properties.
	 *
	 * @param aPropertDef  Property definition.
	 */
	CMdEPropertyCondition(const CMdEPropertyDef& aPropertyDef);

    /**
     * Second-phase constructor.
     */
    void ConstructL();

    /**
	 * Constructor that sets the condition type as well.
	 *
	 * @param aType  Type of the condition node.
	 */
	CMdEPropertyCondition(TConditionType aConditionType, 
                          const CMdEPropertyDef& aPropertyDef);

protected:

    /** The property this condition operates on. */
    const CMdEPropertyDef& iPropertyDef;
    };

#endif  // __MDEPROPERTYCONDITION_H__
