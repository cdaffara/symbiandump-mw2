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


#ifndef __MDEORDERRULE_H__
#define __MDEORDERRULE_H__


#include <e32base.h>

#include <mdccommon.h>

/* Forward declarations. */
class CMdEPropertyDef;
class CMdCSerializationBuffer;

/**
 * TMdEOrderRule defines a rule that is used for sorting the results of a
 * query. A query may use multiple order rules.
 */
class TMdEOrderRule
    {
public:

    /* Constructors. */

    /**
     * Constructor.
     *
     * @param aType       Type of the order rule.
     * @param aAscending  <code>ETrue</code>, if the rule order is 
     *                    ascending.
     */        
    IMPORT_C TMdEOrderRule(TOrderRuleType aType, TBool aAscending = ETrue);   

    /**
     * Constructor.
     *
     * @param aPropertyDef  Property definition.
     * @param aAscending    <code>ETrue</code>, if the rule order is 
     *                      ascending.
     */        
    IMPORT_C TMdEOrderRule(const CMdEPropertyDef& aPropertyDef, 
                           TBool aAscending = ETrue);


    /* Methods. */

    /**
     * Returns the type of the order rule.
     *
     * @return  Order rule type constant.
     */
    IMPORT_C TOrderRuleType Type() const;

    /**
     * Sets the type of the order rule.
     *
     * @param aType  Order rule type constant.
     */
    IMPORT_C void SetType(TOrderRuleType aType);

    /**
     * Determines whether the ordering is ascending.
     *
     * @return  <code>ETrue</code> ascending order,
     *          <code>EFalse</code> descending order.
     */
    IMPORT_C TBool Ascending() const;

    /**
     * Sets the order rule to ascending or descending order.
     *
     * @param aAscending  <code>ETrue</code> ascending order,
     *                    <code>EFalse</code> descending order.
     */
    IMPORT_C void SetAscending(TBool aAscending = ETrue);

    /**
     * Returns the property definition associated with the order rule.
     *
     * @return  Property definition.  <code>NULL</code>, if no property
     *          definition has been associated with the rule.
     */
    IMPORT_C const CMdEPropertyDef* PropertyDef() const;

    /**
     * Associates a property definition with the order rule.
     *
     * @param aPropertyDef  Property definition.
     */
    IMPORT_C void SetPropertyDef(const CMdEPropertyDef& aPropertyDef);

    /**
     * Sets text sorting case sensitive or case insensitive.
     *
     * @param aCaseSensitive <code>ETrue</code> case sensitive sort.
                             <code>EFalse</code> case insensitive sort.
     */
    IMPORT_C void SetCaseSensitive(TBool aCaseSensitive);

    /**
     * Returns type of text comparison, whether case sensitive
     * or case insensitive.
     *
     * @return <code>ETrue</code> case sensitive sort.
     *         <code>EFalse</code> case insensitive sort.
     */
    IMPORT_C TBool CaseSensitive();

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

    /** Type of the order rule. */
    TOrderRuleType iType;
    
    /** Optional. */
    const CMdEPropertyDef* iPropertyDef;
    
    /** Flag for ascending order. */
    TBool iAscending;

    /** Flag for text sorting. Default value is EFalse */
    TBool iCaseSensitive;
   };        

#endif  // __MDEORDERRULE_H__
