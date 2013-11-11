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
* Description:    SIMPLE Engine presence list meta data
*
*/




#ifndef M_simplemeta_H
#define M_simplemeta_H

#include <e32std.h>
#include <msimpleelement.h>

// FORWARD DECLARATION
class MSimpleNamespace;

/**
 *  MSimpleMeta
 *
 *  SIMPLE Engine presence list meta data
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimpleMeta : public MSimpleElement
    {

public:

    /**
     * Meta data namespace accessor.
     *
     * @since S60 3.2
     * @return default namespace URI
     */
    virtual TPtrC8 DefaultNamespace() = 0;

    /**
     * Add new newspace
     *
     * @since S60 3.2
     * @param aPrefix:  Namespace prefix
     * @param aUri: Namespace URI
     */
    virtual void AddNamespaceL( const TDesC8& aPrefix,
                                const TDesC8& aUri) = 0;
    /**
     * Gets all extra namespaces of this element in an array.
     *
     * @since S60 3.2
     * @return array of namespaces. Array will be empty if element has
     *         no namespaces.
     *         Do NOT call ResetAndDestroy() for the array,
     *         you may call Reset() only. The array is valid only till
     *         the next call of this method in this entity.
     *         AddNamespaceL() modifications are not visible without
     *         new call of this method.
     */
    virtual RPointerArray<MSimpleNamespace>& NamespacesL() = 0;

    /**
     * Externalize the document into write stream.
     *
     * @since S60 3.2
     * @param aStream write stream
     */
    virtual void ExternalizeL( RWriteStream& aStream ) = 0;

    };

#endif

// End of File
