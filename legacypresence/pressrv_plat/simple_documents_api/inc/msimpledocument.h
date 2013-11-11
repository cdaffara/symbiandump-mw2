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
* Description:    SIMPLE data document
*
*/




#ifndef M_simpledocument_H
#define M_simpledocument_H

#include <e32std.h>
#include <s32strm.h>
#include <msimpleelement.h>

// FORWARD DECLARATION
class MSimpleNamespace;
class MSimpleContent;


/**
 *  MSimpleDocument
 *
 *  SIMPLE data document
 *
 *  @lib simplexmlutils
 *  @since S60 3.2
 */

class MSimpleDocument : public MSimpleElement
    {

public:

    /**
     * Entity's URI getter. URI is specified in a factory method
     *
     * @since S60 3.2
     * @return URI, ownership is not transferred.
     */
    virtual const TDesC8* EntityURI() = 0;

    /**
     * Entity's URI setter.
     *
     * @since S60 3.2
     * @param aValue entity URI
     * @return URI
     */
    virtual void SetEntityURIL( const TDesC8& aValue ) = 0;

    /**
     * Document's default namespace accessor. It is PIDF namespace.
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
     * Gets all namespaces of this element in an array.
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
    
    /**
     * Get direct contents
     * @param aContents array of contents [out]. 
     *        This is walid till the AddDirectContentL method.
     */ 
    virtual void GetDirectContentsL( 
        RPointerArray<MSimpleContent>& aContents ) = 0;
    
    /**
     * Add content
     * @param aContent content, the data content is copied into document
     * @param aCopyContent ETrue if buffer of each content is copied or 
     *        ownership is transferred just without copy.     
     */    
    virtual void AddDirectContentL( MSimpleContent& aContent, TBool aCopyContent ) = 0;
          

    };

#endif

// End of File
