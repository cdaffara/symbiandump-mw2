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
* Description:    SIMPLE data name space
*
*/




#ifndef M_simplenamespace_H
#define M_simplenamespace_H

#include <e32std.h>

/**
 *  MSimpleNamespace
 *
 *  SIMPLE data namespace
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimpleNamespace
    {

public:

    /**
     * Destroy this element entity
     *
     * @since S60 3.2
     */
    virtual void Close() = 0;

    /**
     * Getter for the URI of the namespace object.
     * @since S60 3.2
     * @return  The URI
     */
    virtual TPtrC8 URI() = 0;

    /**
     * Getter for the prefix of the namespace object.
     * @since S60 3.2
     * @return  The prefix
     */
    virtual TPtrC8 Prefix() = 0;

    /**
     * Setter for the URI of the namespace object.
     * @since S60 3.2
     * @param aUri  The URI.
     */
    virtual void SetUriL(const TDesC8& aUri)  = 0;

    /**
     * Setter for the prefix of the namespace object.
     * @since S60 3.2
     * @param aPrefix   The prefix.
     */
    virtual void SetPrefixL(const TDesC8& aPrefix) = 0;

    };

#endif

// End of File
