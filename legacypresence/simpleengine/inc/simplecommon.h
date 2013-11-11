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
* Description:    Abstract base class for WV engine service APIs.
*
*/



#ifndef SimpleCommon_H
#define SimpleCommon_H


// INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KRangeSize = 100;  // range of operation ids for each client handle
    
const TUint KSimpleOK = 200;  // return code for SIP
const TUint KSimpleAccepted = 202;  // return code for SIP

_LIT8 ( KSimpleApplicationType, "application");
_LIT8 ( KSimpleMultiType, "multipart");
_LIT8 ( KSimplePidfSubType, "pidf+xml");
_LIT8 ( KSimpleListSubType, "rlmi+xml");
_LIT8 ( KSimpleMultipartSubType, "related");
_LIT8 ( KSimpleWinfoSubType, "watcherinfo+xml");
_LIT8 ( KSimpleDocumentType, "application/pidf+xml");
_LIT8 ( KSimpleListType, "application/rlmi+xml");
_LIT8 ( KSimpleWinfoType, "application/watcherinfo+xml");
_LIT8 ( KSimpleSubscribeSubType, "simple-filter+xml");
_LIT8 ( KSimpleMultipartType, "multipart/related");
_LIT8 ( KSimplePRESENCE, "PRESENCE");
_LIT8 ( KSimplePUBLISH, "PUBLISH" );
_LIT8 ( KSimpleNOTIFY, "NOTIFY" );
_LIT8 ( KSimpleSTATUS, "STATUS" );
_LIT8 ( KSimpleSipIfMatch, "SIP-If-Match");
_LIT8 ( KSimpleETag, "SIP-ETag");
_LIT8 ( KSimplePrivacy, "Privacy");
_LIT8 ( KSimplePIdentity, "P-Preferred-Identity");

_LIT8 ( KSimpleMediaType, "text" );     // the 1st part of content type of IM
_LIT8 ( KSimpleMediaSubType, "plain" ); // the 2nd part of content type of IM

// OMA data specifications
_LIT8 ( KSimpleXmlStart, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
_LIT8 ( KDocumentLocalName, "presence" );
_LIT8 ( KFilterLocalName, "filter-set" );
_LIT8 ( KWinfoLocalName, "watcherinfo" );
_LIT8 ( KRlmiLocalName, "list" );

// DATA TYPES

enum TSimpleSipState
    {
    /** Connection initializing */
    ESimpleSipIdle=1,
    /** Connection active */
    ESimpleSipActive,
    /** Connection suspended */
    ESimpleSipSuspend,
    /** Connection is inactive */
    ESimpleSipInactive,
    /** Connection is permanently unavailable */
    ESimpleSipUnavailable
    };
    
    
// Notice: Remove the hard coded values later, and values not needed
namespace NSimpleDocument
    {
    _LIT8( KBoundary, "--%S\r\n");
    _LIT8( KEndBoundary, "--%S--"); 
    _LIT8( KSimpleBoundary, "simple-1" ); // Notice: static now. 
    _LIT8( KSimpleAvatarContent8, "simpleavatar.nokia.com" ); // notice: static now, supports only one 
    _LIT8( KSimpleAvatarCID8, "<simpleavatar.nokia.com>" ); // notice: static now, supports only one            
    // const TInt boundarySize = 4;
    const TInt KSimpleBoundarySize = 8;  
    // const TInt KContentTypeSize_only = 14; // Length of "content-type: "
    namespace NSimpleMulti
        {                              
                          // 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123
        _LIT8( KContentType, "Content-Type: multipart/related; type=\"application/pidf+xml\"; start=\"<1.simple.nokia.com>\"; boundary=\"%S\"\r\n"); 
        _LIT8( KContentTypeFullValue, "multipart/related; type=\"application/pidf+xml\"; start=\"<1.simple.nokia.com>\"; boundary=\"simple-1\"\r\n");
        _LIT8( KContentTypeValue, "multipart/related; type=\"application/pidf+xml\"; start=\"<1.simple.nokia.com>\"; boundary=\"%S\"\r\n");                
        _LIT8( KBoundaryHeader, "boundary=--%S\r\n");        
        const TInt KContentTypeSize  = 85;         // 93 -  8 = 85
        const TInt KBoundaryHeaderSize = 13;        
        }    
    namespace NSimpleRoot
        {                 // 12345678901234567890123456789012345678
        _LIT8( KContentType, "Content-Type: application/pidf+xml\r\n");
        _LIT8( KCID, "Content-ID: <1.simple.nokia.com>\r\n");    // notice: use tag and not static id  
                  // 12345678901234567890123456789012345678                                          
        const TInt KContentTypeSize = 36;  // 38 - 2 
        const TInt KCIDSize = 34; // 36-2 
        }
    namespace NSimpleContent
        {        
        _LIT8( KContentType, "Content-Type: %S\r\n");
        _LIT8( KCID,         "Content-ID: <%S>\r\n");    // notice: use tag and not static id  
        _LIT8( KContentEncoding,  "Content-Transfer-Encoding: BASE64\r\n");          
                  // 12345678901234567890123456789012345678                          
        const TInt KContentTypeSize = 16;  // 20 - 4 
        const TInt KCIDSize = 16; // 20-4 
        const TInt KContentEncodingSize = 35;                 
        }
    }


#endif

// End of File
