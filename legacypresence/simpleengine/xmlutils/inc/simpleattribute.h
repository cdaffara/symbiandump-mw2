/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   SIMPLE attribute
*
*/




#ifndef simpleattribute_H
#define simpleattribute_H

#include <e32std.h>
// #include "simplecommon.h"
#include "msimpleattribute.h"

// FORWARD DECLARATION
class CSenBaseAttribute;


/**
 *  CSimpleAttribute
 *
 *  SIMPLE attribute.
 *
 *  @lib simplexmlutils
 *  @since S60 v3.1
 */

class CSimpleAttribute : public CBase, public MSimpleAttribute
    {

public:


    /**
     * Two-phased constructor.
     * @since S60 v3.1
     * @param aName:    Name of the attribute
     * @param aValue:   Attribute's value.
     * @return CSimpleAttribute
     */
    static CSimpleAttribute* NewL(
        const TDesC8& aName,
        const TDesC8& aValue );

    /**
     * Two-phased constructor.
     * @since S60 v3.1
     */
    static CSimpleAttribute* NewL(
        CSenBaseAttribute* aAttr );

    /**
     * Destructor.
     */
    virtual ~CSimpleAttribute();

    /**
     * @return Base class implementation
     */
    CSenBaseAttribute* BaseElement();

// from base class MSimpleAttribute

    /**
     * From MSimpleAttribute
     * Getter for the attribute name.
     * @since Series60 3.1
     * @return the attribute name.
     */
    const TDesC8& Name();

    /**
     * From MSimpleAttribute
     * Getter for the attribute value.
     * @since Series60 3.1
     * @return the attribute value.
     */
    const TDesC8& Value();

    /**
     * From MSimpleAttribute
     * Getter for the attribute value in UNICODE.
     * @since Series60 3.1
     * @return the attribute value. Ownership is transferred.
     */
    HBufC* ValueUniLC();

    /**
     * From MSimpleAttribute
     * Setter for the attribute value.
     * This makes conversion from 16-bit to 8-bit.
     *
     * @since Series60 3.1
     * @param aValue: value to be set
     */
    void SetValueL( const TDesC& aValue );

private:

    /**
     * Two-phase constructor
     * @param aName:    Name of the attribute
     * @param aValue:   Attribute's value.
     */
    void ConstructL(
        const TDesC8& aName,
        const TDesC8& aValue );

    /**
     * Two-phase constructor
     * @param aAttr:    Base element
     */
    void ConstructL(
        CSenBaseAttribute* aAttr );

    /**
     * constructor
     */
    CSimpleAttribute();


private: // data

    /**
     * Base element when created from scratch
     * Own.
     */
    CSenBaseAttribute*    iBase;

    /**
     * Base element when only reference to base element
     * Not own.
     */
    CSenBaseAttribute*    iRef;

    };

#endif      // simpleattribute_H

// End of File
