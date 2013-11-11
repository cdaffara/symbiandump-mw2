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
* Description:    SIMPLE Engine XML utils factory
*
*/




#ifndef simplexmlfactory_H
#define simplexmlfactory_H

#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATION
class MSimpleDocument;
class MSimpleWinfo;
class MSimpleElement;
class MSimpleNamespace;
class MSimpleFilterDocument;
class MSimplePresenceList;
class MSimpleContent;

/**
 * SIMPLE Engine XML utils factory
 *
 * @lib simplexmlutils 
 * @since S60 3.2
 */ 
class TSimpleXmlFactory
    {

public:

    /**
     * Create new simple Element entity.
     * @since S60 3.2
     * @return MSimpleElement, ownership is transferred.
     */
    IMPORT_C static MSimpleElement* NewElementL(
        const TDesC8& aNsUri,
        const TDesC8& aLocalName );

    /**
     * Create new simple NameSpace entity.
     * @since S60 3.2
     * @param aPrefix prefix to be used
     * @param aUri URI
     * @return MSimpleNameSpace, ownership is transferred.
     */
    IMPORT_C static MSimpleNamespace* NewNamespaceL(
        const TDesC8& aPrefix,
        const TDesC8& aUri );

    /**
     * Create new simple Document entity from scratch.
     * @since S60 3.2
     * @return MSimpleDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleDocument* NewDocumentL( );

    /**
     * Create new simple Document entity from xml-document.
     * @since S60 3.2
     * @param aXml a valid XML document in PIDF format.
     * @return MSimpleDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleDocument* NewDocumentL( const TDesC8& aXml );
    
    /**
     * Create new simple Document entity from MIME multipart
     * @since S60 3.2
     * @param aData MIME multipart data
     * @param aBoundary MIME multipart boundary
     * @param aStart MIME multipart start content-id
     * @return MSimpleDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleDocument* NewDocumentInMultiPartL(
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );        

    /**
     * Create new simple FilterDocument entity.
     * @since S60 3.2
     * @return MSimpleFilterDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleFilterDocument* NewFilterDocumentL();
    
    /**
     * Create new simple FilterDocument entity.
     * @since S60 3.2
     * @param aXml a valid XML document in PIDF format.     
     * @return MSimpleFilterDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleFilterDocument* NewFilterDocumentL( const TDesC8& aXml );    

    /**
     * Create new presence list (resource list)
     * @since S60 3.2
     * @param aData MIME multipart data
     * @param aBoundary MIME multipart boundary
     * @param aStart MIME multipart start content-id
     * @return MSimplePresenceList, ownership is transferred.
     */
    IMPORT_C static MSimplePresenceList* NewPresenceListL(
        const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart );

    /**
     * Create new winfo document
     * @since S60 3.2
     * @return MSimpleWinfo, ownership is transferred.
     */
    IMPORT_C static MSimpleWinfo* NewWinfoL();
    
    /**
     * Create new winfo document
     * @since S60 3.2
     * @param aXml XML document
     * @return MSimpleWinfo, ownership is transferred.
     */
    IMPORT_C static MSimpleWinfo* NewWinfoL( const TDesC8& aXml );
    
    IMPORT_C static MSimpleContent* NewContentL( 
        const TDesC8& aContentID, const TDesC8& aContentType );    
    };


#endif      // simplexmlfactory_H

// End of File
