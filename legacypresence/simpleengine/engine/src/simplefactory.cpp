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
* Description:    Simple Engine
*
*/




// INCLUDE FILES

#include <e32std.h>

// own simple
#include "simplecommon.h"
#include "simplexmlfactory.h"
#include "simplefactory.h"

#include "simplenamespace.h"
#include "simpleelement.h"
#include "simpledocument.h"
#include "simplefilter.h"
#include "simpleconnection.h"
#include "simplepublisher.h"
#include "simplewatcher.h"
#include "simplewinfowatcher.h"
#include "simpleengineimhandler.h"

#include "msimplenamespace.h"
#include "msimpleelement.h"
#include "msimpledocument.h"
#include "msimplefilterdocument.h"
#include "msimpleconnection.h"
#include "msimplepublisher.h"
#include "msimplewatcher.h"
#include "msimplewinfowatcher.h"
#include "simpleinstantmessage.h"

class MSimpleConnectionObserver;
class MSimplePublishObserver;
class MSimpleWatcherObserver;
class MSimpleWinfoObserver;
class MSimplePublishObserver;

class MSimpleContent;


// ================= MEMBER FUNCTIONS =======================
//

EXPORT_C MSimpleNamespace* TSimpleFactory::NewNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    return TSimpleXmlFactory::NewNamespaceL( aPrefix, aUri );
    }

EXPORT_C MSimpleElement* TSimpleFactory::NewElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return TSimpleXmlFactory::NewElementL( aNsUri, aLocalName );
    }

EXPORT_C MSimpleDocument* TSimpleFactory::NewDocumentL( )
    {
    return TSimpleXmlFactory::NewDocumentL( );
    }

EXPORT_C MSimpleDocument* TSimpleFactory::NewDocumentL( const TDesC8& aXml )
    {
    return TSimpleXmlFactory::NewDocumentL( aXml );
    }

EXPORT_C MSimpleFilterDocument* TSimpleFactory::NewFilterDocumentL()
    {
    return TSimpleXmlFactory::NewFilterDocumentL();
    }

EXPORT_C MSimpleConnection* TSimpleFactory::NewConnectionL(
    MSimpleConnectionObserver& aObserver)
    {
    return CSimpleConnection::NewL( aObserver );
    }

EXPORT_C MSimplePublisher* TSimpleFactory::NewPublisherL(
    MSimpleConnection& aConn,
    MSimplePublishObserver& aObserver )
    {
    return CSimplePublisher::NewL( aConn, aObserver );
    }

EXPORT_C MSimpleInstantMessage* TSimpleFactory::NewInstantMessageL(
    MSimpleConnection& aConn,
    MSimpleInstantMsgObserver& aObserver )
    {
    return CSimpleEngineImHandler::NewL( aConn, aObserver );
    }

EXPORT_C MSimpleWatcher* TSimpleFactory::NewWatcherL(
    MSimpleConnection& aConn,
    MSimpleWatcherObserver& aObserver )
    {
    return CSimpleWatcher::NewL( aConn, aObserver );
    }

EXPORT_C MSimpleWinfoWatcher* TSimpleFactory::NewWinfoWatcherL(
    MSimpleConnection& aConn,
    MSimpleWinfoObserver& aObserver )
    {
    return CSimpleWinfoWatcher::NewL( aConn, aObserver );
    }
    
EXPORT_C MSimpleFilterDocument* TSimpleFactory::NewFilterDocumentL( const TDesC8& aXml )
    {
    return TSimpleXmlFactory::NewFilterDocumentL( aXml );
    }    
    
EXPORT_C MSimpleContent* TSimpleFactory::NewContentL( 
    const TDesC8& aContentID, const TDesC8& aContentType )
    {
    return TSimpleXmlFactory::NewContentL( 
        aContentID, aContentType );    
    }
      




