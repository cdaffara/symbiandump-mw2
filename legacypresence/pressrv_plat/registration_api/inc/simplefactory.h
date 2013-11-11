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
* Description:    Factory class for SIMPLE Engine
*
*/




#ifndef simplefactory_H
#define simplefactory_H

#include <e32std.h>
#include <e32base.h>


// FORWARD DECLARATION
class MSimpleConnection;
class MSimplePublisher;
class MSimpleWatcher;
class MSimpleWinfoWatcher;
class MSimpleDocument;
class MSimpleElement;
class MSimpleNamespace;
class MSimpleFilterDocument;

class MSimpleConnectionObserver;
class MSimplePublishObserver;
class MSimpleWatcherObserver;
class MSimpleWinfoObserver;
class MSimpleContent;
class MSimpleInstantMsgObserver;
class MSimpleInstantMessage;

/**
 * Factory class for SIMPLE Engine
 *
 * @lib simpleengine
 * @since S60 3.2
 */
class TSimpleFactory
    {

public:

    /**
     * Create new simple Element entity.
     * @since S60 3.2
     * @param aNsUri the namespace URI for this element.
     * @param aLocalName the local name for this element.
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
     * Create new simple engine Connection entity.
     * @since S60 3.2
     * @param aObserver client callback observer
     * @return MSimpleConnection, ownership is transferred.
     */
    IMPORT_C static MSimpleConnection* NewConnectionL(
        MSimpleConnectionObserver& aObserver );

    /**
     * Create new Publisher entity.
     * @since S60 3.2
     * @paraam aConn simple engine connection
     * @param aObserver client callback observer
     * @return MSimplePublisher, ownership is transferred.
     */
    IMPORT_C static MSimplePublisher* NewPublisherL(
        MSimpleConnection& aConn,
        MSimplePublishObserver& aObserver );

    /**
     * Create new InstantMessage entity.
     * @since S60 5.0
     * @param aConn simple engine connection
     * @param aObserver client callback observer
     * @return MSimpleInstantMessage, ownership is transferred.
     */
    IMPORT_C static MSimpleInstantMessage* NewInstantMessageL(
        MSimpleConnection& aConn,
        MSimpleInstantMsgObserver& aObserver );

    /**
     * Create new Watcher entity.
     * @since S60 3.2
     * @paraam aConn simple engine connection
     * @param aObserver client callback observer
     * @return MSimpleWatcher, ownership is transferred.
     */
    IMPORT_C static MSimpleWatcher* NewWatcherL(
        MSimpleConnection& aConn,
        MSimpleWatcherObserver& aObserver );

    /**
     * Create new WinfoWatcher entity.
     * @since S60 3.2
     * @paraam aConn simple engine connection
     * @param aObserver client callback observer
     * @return MSimpleWinfoWatcher, ownership is transferred.
     */
    IMPORT_C static MSimpleWinfoWatcher* NewWinfoWatcherL(
        MSimpleConnection& aConn,
        MSimpleWinfoObserver& aObserver );

    /**
     * Create new simple Document entity from scratch.
     * @since S60 3.2
     * @return MSimpleDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleDocument* NewDocumentL( );

    /**
     * Create new simple Document entity from xml-document.
     * @since S60 3.2
     * @param aXml a valid XML document containing in PIDF format.
     * @return MSimpleDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleDocument* NewDocumentL( const TDesC8& aXml );

    /**
     * Create new simple FilterDocument entity.
     * @since S60 3.2
     * @return MSimpleFilterDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleFilterDocument* NewFilterDocumentL();

    /**
     * Create new simple FilterDocument entity.
     * @since S60 3.2
     * @param aXml a valid XML document containing in PIDF format.
     * @return MSimpleFilterDocument, ownership is transferred.
     */
    IMPORT_C static MSimpleFilterDocument* NewFilterDocumentL( const TDesC8& aXml );
    
    IMPORT_C static MSimpleContent* NewContentL( 
        const TDesC8& aContentID, const TDesC8& aContentType );    

    };

#endif

// End of File
