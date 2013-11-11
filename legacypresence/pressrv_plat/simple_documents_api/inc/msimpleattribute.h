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
* Description:    SIMPLE data attribute
*
*/




#ifndef M_simpleattribute_H
#define M_simpleattribute_H

#include <e32std.h>

// FORWARD DECLARATION


/**
 *  MSimpleAttribute
 *
 *  SIMPLE data attribute
 *
 *  @lib simplexmlutils
 *  @since S60 v3.2
 */

class MSimpleAttribute
    {

public:

    /**
     * Getter for the attribute name.
     * @since S60 3.2
     * @return the attribute name.
     */
    virtual const TDesC8& Name() = 0;

    /**
     * Getter for the attribute value.
     * @since S60 3.1
     * @return the attribute value, UTF-8
     */
    virtual const TDesC8& Value() = 0;

    /**
     * Getter for the attribute value in UNICODE.
     * @since S60 3.2
     * @return the attribute value. Ownership is transferred.
     */
    virtual HBufC* ValueUniLC() = 0;

    /**
     * Setter for the attribute value.
     *
     * @since S60 3.2
     * @param aValue: value to be set, UNICODE
     */
    virtual void SetValueL( const TDesC& aValue ) = 0;

    };

#endif

// End of File
