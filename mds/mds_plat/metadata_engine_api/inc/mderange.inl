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
* Description:
*
*/
#ifndef MDERANGE_INL_
#define MDERANGE_INL_

/**
 * The TMdERange template is used for representing value ranges of various 
 * types. A range is composed of an upper limit, a lower limit, and type 
 * that describe how the limits are to be interpreted. 
 *
 * Value ranges are used with CMdERangePropertyCondition nodes to define 
 * a range of values that the condition matches. Basic operations such as 
 * "greater than", "equals to", and "between" are all implemented using 
 * value ranges. There are also more complex ranges that can be used in 
 * loops.
 *
 * TMdERange is the base template for many different kind of value ranges.
 * However, all ranges can be processed with the Min(), Max(), and Type() 
 * methods, regardless of any extra data defined by a specialized range type.
 */
template <class T>
class TMdERange
    {
public:

    /* Methods. */

    /**
     * Returns the lower limit of the range.
     *
     * @return  The minimum value.
     */
    inline const T& Min() const
        {
        return iMin;
        }

    /**
     * Returns the upper limit of the range.
     *
     * @return  The maximum value.
     */
    inline const T& Max() const
        {
        return iMax;
        }

    /**
     * Returns the type of the range.
     *
     * @return  type
     */
    inline const TMdERangeType& Type() const
        {
        return iType;
        }

    /**
     * Sets the lower limit of the range.
     *
     * @param aMin  The minimum value.
     */
    inline void SetMin(const T& aMin)
        {
        iMin = aMin;
        }

    /**
     * Sets the upper limit of the range.
     *
     * @param aMax  The maximum value.
     */
    inline void SetMax(const T& aMax)
        {
        iMax = aMax;
        }
    
    /**
     * Sets the type of the range.
     *
     * @param aType  The new type.
     */
    inline void SetType(TMdERangeType aType)
        {
        iType = aType;
        }

    /**
     * Get required size of serialized buffer when this is serialized.
     *
     * @return required size of serialized buffer
     */
    TUint32 RequiredBufferSize()
    	{
     	return TMdESerializeRange::RequiredBufferSize( sizeof(T) );
    	}

    /**
     * Serialize own data to serialized buffer (correct position must be set 
     * before calling) and return new position of serialized buffer.
	 *
     * @param aBuffer  serialized buffer.
     */
    void SerializeL(CMdCSerializationBuffer& aBuffer)
    	{
    	TMdESerializeRange::SerializeL( aBuffer, iType, &iMin, &iMax, sizeof(T) );
    	}

    /* Constructor. */
    TMdERange(const T& aMin, const T& aMax, TMdERangeType aType = EMdERangeTypeAny)
        : iType(aType), iMin(aMin), iMax(aMax)
        {
        }

	/**
     * Test if value is in the range.
     *
     * @param aValue Tested value.
	 */
    TBool InRange(const T& aValue)
        {
        switch( iType )
        	{        	
        	case EMdERangeTypeAny:
        		{
        		// always match
        		return ETrue;
        		}
        	case EMdERangeTypeEqual:
        		{
        		return ( aValue == iMin );
        		}
			case EMdERangeTypeNotEqual:
				{
				return ( aValue != iMin );
				}
			case EMdERangeTypeLess:
				{
				return ( aValue < iMax );
				}
			case EMdERangeTypeLessOrEqual:
				{
				return ( aValue <= iMax );
				}
			case EMdERangeTypeGreater:
				{
				return ( aValue > iMin );
				}
			case EMdERangeTypeGreaterOrEqual:
				{
				return ( aValue >= iMin );
				}
			case EMdERangeTypeBetween:
				{
				// edges belong to range
				return ( ( iMin <= aValue ) && ( aValue <= iMax ) );
				}
			case EMdERangeTypeNotBetween:
				{
				// edges belong to range
				return ( ( iMin > aValue ) || ( aValue > iMax ) );
				}
			default:
				{
				// should never happen
				return EFalse; 
				}
        	}
	    }

private:

    /** Type. */
    TMdERangeType iType;

    /** Beginning of the range. */
    T iMin;

    /** End of the range. */
    T iMax;
    };


/**
 * Value range that covers everything.
 */
template <class T>
class TMdEAny : public TMdERange<T>
    {
public:
    TMdEAny()
        : TMdERange<T>(T(0), T(0), EMdERangeTypeAny) {}
    };


/**
 * TMdEEquals defines a range that contains only a single point. Thus it
 * functions as an equivalence operator.
 */
template <class T> 
class TMdEEqual : public TMdERange<T>
    {
public:
    TMdEEqual(const T& aValue) 
        : TMdERange<T>(aValue, aValue, EMdERangeTypeEqual) {}
    };

/**
 * Range that contains all values except for one point.
 */
template <class T> 
class TMdENotEqual : public TMdERange<T>
    {
public:
    TMdENotEqual(const T& aValue) 
        : TMdERange<T>(aValue, aValue, EMdERangeTypeNotEqual) {}
    };

/**
 * Less-than range.
 */
template <class T> 
class TMdELess : public TMdERange<T>
    {
public:
    TMdELess(const T& aLimit) 
        : TMdERange<T>(T(0), aLimit, EMdERangeTypeLess) {}
    };


/**
 * Less-than-or-equal-to range.
 */
template <class T> 
class TMdELessEqual : public TMdERange<T>
    {
public:
    TMdELessEqual(const T& aLimit) 
        : TMdERange<T>(T(0), aLimit, EMdERangeTypeLessOrEqual) {}
    };


/**
 * Greater-than range.
 */
template <class T> 
class TMdEGreater : public TMdERange<T>
    {
public:
    TMdEGreater(const T& aLimit) 
        : TMdERange<T>(aLimit, T(0), EMdERangeTypeGreater) {}
    };


/**
 * Greater-than-or-equal-to range.
 */
template <class T> 
class TMdEGreaterEqual : public TMdERange<T>
    {
public:
    TMdEGreaterEqual(const T& aLimit) 
        : TMdERange<T>(aLimit, T(0), EMdERangeTypeGreaterOrEqual) {}
    };


/**
 * Between range.  The upper and lower limits are inclusive by default.
 */
template <class T> 
class TMdEBetween : public TMdERange<T>
    {
public:
    TMdEBetween(const T& aMin, const T& aMax) 
        : TMdERange<T>(aMin, aMax, EMdERangeTypeBetween) {}
    };


/**
 * Not-between range.  The upper and lower limits are inclusive by 
 * default.
 */
template <class T> 
class TMdENotBetween : public TMdERange<T>
    {
public:
    TMdENotBetween(const T& aMin, const T& aMax) 
        : TMdERange<T>(aMin, aMax, EMdERangeTypeNotBetween) {}
    };


/* Types that will be used in practice. */

/** Value range of type TInt. */
typedef TMdERange<TInt> TMdEIntRange;

/** Value range of type TUint. */
typedef TMdERange<TUint> TMdEUintRange;

/** Value range of type TInt64. */
typedef TMdERange<TInt64> TMdEInt64Range;

/** Value range of type TReal. */
typedef TMdERange<TReal> TMdERealRange;

/** Value range of type TTime. */
typedef TMdERange<TTime> TMdETimeRange;


/** Macro for defining the real typenames. Makes four versions of each
    range type. */
#define MDE_DEFINE_ACTUAL_RANGE_TYPES(RangeName) \
    typedef TMdE##RangeName<TInt> TMdEInt##RangeName; \
    typedef TMdE##RangeName<TUint> TMdEUint##RangeName; \
    typedef TMdE##RangeName<TInt64> TMdEInt64##RangeName; \
    typedef TMdE##RangeName<TReal> TMdEReal##RangeName; \
    typedef TMdE##RangeName<TTime> TMdETime##RangeName;

MDE_DEFINE_ACTUAL_RANGE_TYPES(Any)
MDE_DEFINE_ACTUAL_RANGE_TYPES(Equal)
MDE_DEFINE_ACTUAL_RANGE_TYPES(NotEqual)
MDE_DEFINE_ACTUAL_RANGE_TYPES(Less)
MDE_DEFINE_ACTUAL_RANGE_TYPES(LessEqual)
MDE_DEFINE_ACTUAL_RANGE_TYPES(Greater)
MDE_DEFINE_ACTUAL_RANGE_TYPES(GreaterEqual)
MDE_DEFINE_ACTUAL_RANGE_TYPES(Between)
MDE_DEFINE_ACTUAL_RANGE_TYPES(NotBetween)

#undef MDE_DEFINE_ACTUAL_RANGE_TYPES   

#endif /*MDERANGE_INL_*/
