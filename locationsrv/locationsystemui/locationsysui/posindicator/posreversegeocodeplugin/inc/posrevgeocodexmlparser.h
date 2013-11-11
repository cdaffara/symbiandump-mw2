/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Declaration of XML parser class.
*
*/

#ifndef POSREVGEOCODEXMLPARSER_H
#define POSREVGEOCODEXMLPARSER_H

#include <e32base.h>
#include <xml/contenthandler.h> 
#include <xml/parser.h>
using namespace Xml;

// Forward declarations
class CPosLandmark;

/**
 * MPosRevGeoCodeXmlObserver, an observer to CXmlHandler class.
 */
class MPosRevGeoCodeXmlObserver
    {
public:
    /*
     * Signyfys the completion of parsing of the output data.
     * 
     * @param aError error status of parsing part.
     */    
    virtual void OnParseCompletedL( TInt aError ) = 0;
    };

/**
 * This class parses XML script and fills the information into landmarks structure. 
 */
class CPosRevGeoCodeXmlParser : public CBase,
                                public MContentHandler
    {
    /*
     * Category of the address information. This is used for
     * internal parsing procedure.
     */
    enum TLocationInfoType
        {
        ENone = 0,
        ECountryName,
        EState,
        ECity,
        EDistrict,
        EPostalCode,
        EThoroughfareName,
        EThoroughfareNumber
        };
public:
    /**
     * Factory function to create the instance of CPosRevGeoCodeXmlParser object.
     */
    static CPosRevGeoCodeXmlParser* NewL( MPosRevGeoCodeXmlObserver& aObserver );
    
    /**
     * Destructor
     */
    virtual ~CPosRevGeoCodeXmlParser();
    
public: 
    /**
     * Starts parsing XML provided through buffer. 
     */
    void StartParsingL( HBufC8* aXmlInfo,CPosLandmark* aLandmarkInfo );
    
private:
    /**
     * Constructor
     */
    CPosRevGeoCodeXmlParser( MPosRevGeoCodeXmlObserver& aObserver );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();
    
private: // from MContentHandler 
    void OnStartDocumentL( const RDocumentParameters &aDocParam, TInt aErrorCode );
    void OnEndDocumentL( TInt aErrorCode );
    void OnStartElementL( const RTagInfo &aElement, const RAttributeArray &aAttributes, TInt aErrorCode );
    void OnEndElementL( const RTagInfo &aElement, TInt aErrorCode );
    void OnContentL( const TDesC8 &aBytes, TInt aErrorCode );
    void OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri, TInt aErrorCode );
    void OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode );
    void OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode );
    void OnSkippedEntityL( const RString &aName, TInt aErrorCode );
    void OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData, TInt aErrorCode);
    void OnError( TInt aErrorCode );
    
    TAny *GetExtendedInterface( const TInt32 aUid );
    
private://Data members
    /**
     * Reference to XML observer
     */
    MPosRevGeoCodeXmlObserver& iObserver;
    
    /**
     * Pointer to parser
     * Own
     */
    CParser*             iParser;
    
    /**
     * Buffer the contain XML to parsed
     */
    HBufC8*              iBuffer;
    
    /**
     * Pointer to client's landmarks object.
     */
    CPosLandmark*        iClientLandmark;
    
    /**
     * Current parsing element
     */
    TLocationInfoType     iCurrentElement;
    
    /**
     * Boolean to represent thoroughfare.
     */
    TBool iThoroughfare;
    };

#endif /* POSREVGEOCODEXMLPARSER_H */
