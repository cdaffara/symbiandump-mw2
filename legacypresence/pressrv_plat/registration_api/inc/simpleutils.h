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
* Description:    SIMPLE Engine data document utilities
*
*/




#ifndef simpleutils_H
#define simpleutils_H

#include <e32std.h>

// Namespaces supported
_LIT8 ( KSimpleNsDefault, "urn:ietf:params:xml:ns:pidf" );
_LIT8 ( KSimpleNsPDM, "urn:ietf:params:xml:ns:pidf:data-model" );
_LIT8 ( KSimpleNsRPID, "urn:ietf:params:xml:ns:pidf:rpid" );
_LIT8 ( KSimpleNsOP, "urn:ietf:params:xml:ns:pidf:oma-pres" );
_LIT8 ( KSimpleNsGP, "urn:ietf:params:xml:ns:pidf:gepriv10" );
_LIT8 ( KSimpleNsCL, "urn:ietf:params:xml:ns:pidf:gepriv10:civilLoc" );
_LIT8 ( KSimpleNsGML, "urn:opengis:specification:gml:schema-xsd:feature:v3.0" );

_LIT8 ( KSimpleDefault, "" );
_LIT8 ( KSimplePDM, "pdm" );
_LIT8 ( KSimpleRPID, "rpid" );
_LIT8 ( KSimpleOP, "op" );
_LIT8 ( KSimpleGP, "gp" );
_LIT8 ( KSimpleCL, "cl" );
_LIT8 ( KSimpleGML, "gml" );

// Other namespaces, for <filter-set> and <watcherinfo>
_LIT8 ( KSimpleNsFilter, "urn:ietf:params:xml:ns:simple-filter" );
_LIT8 ( KSimpleNsWinfo, "urn:ietf:params:xml:ns:watcherinfo" );

_LIT8 ( KSimpleNsRlmi, "urn:ietf:params:xml:ns:rlmi" );


/**
 * SIMPLE Engine data document utility methods
 *
 * @lib simpleenine
 * @since S60 3.2
 */
class TSimpleUtils
    {

public:

    /**
     * Namespace URI solver
     * @since S60 3.2
     * @param aPrefix namespace prefix
     * @leaves with KErrNotFound if unsupported prefix given
     * @return namespace URI
     */
    IMPORT_C static TPtrC8 NsUriL(
        const TDesC8& aPrefix );

    /**
     * Namespace prefix solver
     * @since S60 3.2
     * @param aURI namespace URI
     * @leaves with KErrNotFound if unsupported URI given, see valid ones above.
     * @return namespace prefix.
     */
    IMPORT_C static TPtrC8 NsPrefixL(
        const TDesC8& aURI );
    };

#endif

// End of File
