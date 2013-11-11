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
#include "simplexmlfactory.h"

#include "simplenamespace.h"
#include "simpleelement.h"
#include "simpledocument.h"
#include "simplefilter.h"
#include "simplepresencelist.h"
#include "simplewinfo.h"
#include "simplecontent.h"

#include "msimplenamespace.h"
#include "msimpleelement.h"
#include "msimpledocument.h"
#include "msimplefilterdocument.h"
#include "msimplepresencelist.h"
#include "msimplewinfo.h"

// ================= MEMBER FUNCTIONS =======================
//

EXPORT_C MSimpleNamespace* TSimpleXmlFactory::NewNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    return CSimpleNamespace::NewL( aPrefix, aUri );
    }

EXPORT_C MSimpleElement* TSimpleXmlFactory::NewElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return CSimpleElement::NewL( aNsUri, aLocalName );
    }

EXPORT_C MSimpleDocument* TSimpleXmlFactory::NewDocumentL( )
    {
    return CSimpleDocument::NewL( );
    }

EXPORT_C MSimpleDocument* TSimpleXmlFactory::NewDocumentL( const TDesC8& aXml )
    {
    return CSimpleDocument::NewL( aXml );
    }
    
EXPORT_C MSimpleDocument* TSimpleXmlFactory::NewDocumentInMultiPartL( 
    const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart )
    {
    return CSimpleDocument::NewInMultiPartL( aData, aBoundary, aStart );
    }    

EXPORT_C MSimpleFilterDocument* TSimpleXmlFactory::NewFilterDocumentL()
    {
    return CSimpleFilter::NewL();
    }
    
EXPORT_C MSimplePresenceList* TSimpleXmlFactory::NewPresenceListL(
    const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart )
    {
    return CSimplePresenceList::NewL( aData, aBoundary, aStart );
    } 
    
EXPORT_C MSimpleWinfo* TSimpleXmlFactory::NewWinfoL( )
    {
    return CSimpleWinfo::NewL( );      
    }    
    
EXPORT_C MSimpleWinfo* TSimpleXmlFactory::NewWinfoL( const TDesC8& aXml )
    {
    return CSimpleWinfo::NewL( aXml );      
    }
    
EXPORT_C MSimpleFilterDocument* TSimpleXmlFactory::NewFilterDocumentL( const TDesC8& aXml )
    {
    return CSimpleFilter::NewL( aXml );
    }    
    
EXPORT_C MSimpleContent* TSimpleXmlFactory::NewContentL( 
        const TDesC8& aContentID, const TDesC8& aContentType )
    {
    return CSimpleContent::NewL( aContentID, aContentType );
    }

