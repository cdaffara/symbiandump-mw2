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


#ifndef __MDETEXTPROPERTYCONDITION_H__
#define __MDETEXTPROPERTYCONDITION_H__


#include <mdepropertycondition.h>


/* Forward declarations. */
class CMdEPropertyDef;


/**
 * Instances of CMdETextPropertyCondition are associated with a specific 
 * property definition.
 */
NONSHARABLE_CLASS(CMdETextPropertyCondition) : public CMdEPropertyCondition
	{
public:

	/* Constructors and destructor. */

    /** 
     * Constructs a new text property condition node.
     *
     * @param aPropertyDef    Property definition.
     * @param aCompareMethod  Text comparison method.
     * @param aText           Text to look for.
     */
    static CMdETextPropertyCondition* NewL(const CMdEPropertyDef& aPropertyDef,
                                           TTextPropertyConditionCompareMethod aCompareMethod,
                                           const TDesC& aText);

    /** 
     * Constructs a new text property condition node, leaving it on the 
     * cleanup stack.
     *
     * @param aPropertyDef    Property definition.
     * @param aCompareMethod  Text comparison method.
     * @param aText           Text to look for.
     */
    static CMdETextPropertyCondition* NewLC(const CMdEPropertyDef& aPropertyDef,
                                            TTextPropertyConditionCompareMethod aCompareMethod,
                                            const TDesC& aText);

	/**
	 * Destructor.
	 */
	virtual ~CMdETextPropertyCondition();


	/* Methods. */
    
    /**
     * Returns the text of the condition.
     *
     * @return  Text descriptor.
     */
    const TDesC& Text() const;

    /**
     * Returns the comparison method of the condition.
     *
     * @return  Comparison method.
     */
    TTextPropertyConditionCompareMethod CompareMethod() const;

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
   
protected:

   	/**
	 * Constructor. 
	 *
	 * @param aType  type of the condition node
	 */
	CMdETextPropertyCondition(const CMdEPropertyDef& aPropertyDef,
                              TTextPropertyConditionCompareMethod aCompareMethod);


    /**
     * Second-phase constuctor. 
     */
    void ConstructL(const TDesC& aText);


private:

    /* Private methods. */


private:

    /** The comparison method used when searching for the string. */
    TTextPropertyConditionCompareMethod iCompareMethod;

    /** The string to look for. */
    HBufC* iText;
    };

#endif  // __MDETEXTPROPERTYCONDITION_H__
