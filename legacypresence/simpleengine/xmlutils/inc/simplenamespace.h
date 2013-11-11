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
* Description:   SIMPLE namespace
*
*/




#ifndef simplenamespace_H
#define simplenamespace_H

#include <e32std.h>
#include "msimplenamespace.h"

/**
 *  CSimpleNamespace
 *
 *  SIMPLE data namespacw
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class CSimpleNamespace : public CBase, public MSimpleNamespace
    {

public:

    /**
     * Two-phased constructor.
     * @since S60 3.2
     */
    static CSimpleNamespace* NewL(
        const TDesC8& aPrefix,
        const TDesC8& aUri );

    /**
     * Destructor.
     */
    virtual ~CSimpleNamespace();

// from base class MSimpleNamespace

    /**
     * From MSimpleNamespace
     * Destroy this element entity
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * From MSimpleNamespace
     * Getter for the URI of the namespace object.
     *
     * @since S60 3.2     
     * @return  The URI
     */
    TPtrC8 URI();

    /**
     * From MSimpleNamespace
     * Getter for the prefix of the namespace object.
     *
     * @since S60 3.2     
     * @return  The prefix
     */
    TPtrC8 Prefix();

    /**
     * From MSimpleNamespace
     * Setter for the URI of the namespace object.
     *
     * @since S60 3.2     
     * @param aUri  The URI.
     */
    void SetUriL(const TDesC8& aUri);

    /**
     * From MSimpleNamespace
     * Setter for the prefix of the namespace object.
     *
     * @since S60 3.2     
     * @param aPrefix   The prefix.
     */
    void SetPrefixL(const TDesC8& aPrefix);

private:

    void ConstructL(
        const TDesC8& aPrefix,
        const TDesC8& aUri);

    /**
     * contructor
     */
    CSimpleNamespace( );

private:

    /**
     * The actual namespace entity.
     * Own.
     */
    HBufC8* iPrefix;
    
    /**
     * URI.
     * Own.
     */    
    HBufC8* iUri;

    };

#endif      // simplenamespace_H

// End of File
