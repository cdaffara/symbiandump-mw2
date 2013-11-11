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
* Description:  Represents value range of property condition
*
*/


#ifndef __MDERANGEPROPERTYCONDITION_H__
#define __MDERANGEPROPERTYCONDITION_H__


#include <mdepropertycondition.h>
#include <mderange.h>

/* Forward declarations. */
class CMdEPropertyDef;
class CMdCSerializationBuffer;

class TMdESerializeRangePropertyCondition
{
public:
	static void SerializeL( CMdCSerializationBuffer& aBuffer, 
			const CMdEPropertyCondition& aPropertyCondition, 
			TMdERangeType aType, const void* aMin, const void* aMax, 
			TInt aValueSize, TMdCOffset& aFreespaceOffset );
	static TUint32 RequiredBufferSize( TInt aValueSize );
};

/**
 * Instances of CMdERangePropertyCondition are associated with a specific 
 * property definition.
 */
template <TConditionType KConditionType, class T>
NONSHARABLE_CLASS(CMdERangePropertyCondition) : public CMdEPropertyCondition
	{
public:
	/* Constructors and destructor. */

	/**
	 * Two-phased constructor.
	 */
	static CMdERangePropertyCondition* NewL( const CMdEPropertyDef& aPropertyDef,
			const TMdERange<T>& aValueRange)
		{
		CMdERangePropertyCondition* self =
				CMdERangePropertyCondition::NewLC( aPropertyDef, aValueRange );
	    CleanupStack::Pop( self );
	    return self;
		}
	
	/**
	* Two-phased constructor.
	*/
	static CMdERangePropertyCondition* NewLC( const CMdEPropertyDef& aPropertyDef,
			const TMdERange<T>& aValueRange)
		{
		CMdERangePropertyCondition* self =
				new ( ELeave ) CMdERangePropertyCondition( aPropertyDef, aValueRange );
	    CleanupStack::PushL( self );
	    self->ConstructL();
	    return self;
		}


	/**
	 * Destructor.
	 */
	virtual ~CMdERangePropertyCondition()
        {
        }


	/* Methods. */
	
	/**
	 * Returns the range of the property condition.
	 *
	 * @return  Range.
	 */
	inline const TMdERange<T>& Range() const
	    {
	    return iRange;
	    }

    /**
     * Get possible query's internal optimization flags from condition.
     *
     * @param aFlaga query's internal optimization flags
     * @return last level internal optimization flags
     */
    TUint32 InternalQueryOptimizationFlags(TUint32& aFlags)
    	{
    	return CMdEPropertyCondition::InternalQueryOptimizationFlags(aFlags);
    	}

    /**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
     */
    TUint32 RequiredBufferSize() const
    	{
    	return TMdESerializeRangePropertyCondition::RequiredBufferSize( 
    			sizeof(T) );
    	}

    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    void SerializeL(CMdCSerializationBuffer& aBuffer, 
    		TMdCOffset& aFreespaceOffset) const
    	{
    	TMdESerializeRangePropertyCondition::SerializeL( aBuffer, *this, iRange.Type(), 
    			&iRange.Min(), &iRange.Max(), sizeof(T), aFreespaceOffset );
    	}

protected:

	/* Utility methods. */


private:

    /* Private methods. */
	/**
	 * Constructor. 
	 *
	 * @param aType  type of the condition node
	 */
    CMdERangePropertyCondition(const CMdEPropertyDef& aPropertyDef,
        const TMdERange<T>& aValueRange)
        : CMdEPropertyCondition(KConditionType, aPropertyDef), 
          iRange(aValueRange)
        {
        }

private:

    /** The value range that is valid for the condition. */
    TMdERange<T> iRange;
    };


/* The actual RangePropertyConditions. */

#define MDE_DEFINE_RANGE_CONDITION(RangeType) \
    typedef CMdERangePropertyCondition<EConditionTypeProperty##RangeType##Range, T##RangeType> CMdE##RangeType##RangePropertyCondition;

MDE_DEFINE_RANGE_CONDITION(Int)
MDE_DEFINE_RANGE_CONDITION(Int64)
MDE_DEFINE_RANGE_CONDITION(Uint)
MDE_DEFINE_RANGE_CONDITION(Real)
MDE_DEFINE_RANGE_CONDITION(Time)

#undef MDE_DEFINE_RANGE_CONDITION
 
#endif  // __MDERANGEPROPERTYCONDITION_H__
