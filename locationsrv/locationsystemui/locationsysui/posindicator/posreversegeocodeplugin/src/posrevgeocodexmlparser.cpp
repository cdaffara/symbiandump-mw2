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
* Description: Implementation of XML parser class.
*
*/

#include "posrevgeocodexmlparser.h"
#include "posrevgeocodelogger.h"


#include <utf.h>
#include <EPos_CPosLandmark.h>

// CONSTANTS
_LIT8( KXmlMimeType, "text/xml" );
_LIT8( KCountry, "country" );
_LIT8( KState, "state" );
_LIT8( KDistrict, "district" );
_LIT8( KCity, "city" );
_LIT8( KPostalCode, "postCode" );
_LIT8( KThoroughfare, "thoroughfare" );
_LIT8( KNameTag, "name" );
_LIT8( KNumberTag, "number" );


//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::NewL
//------------------------------------------------------------------------------
CPosRevGeoCodeXmlParser* CPosRevGeoCodeXmlParser::NewL( MPosRevGeoCodeXmlObserver& aObserver )
    {
    FUNC("CPosRevGeoCodeXmlParser::NewL");
    CPosRevGeoCodeXmlParser* self = new ( ELeave ) CPosRevGeoCodeXmlParser( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::~CPosRevGeoCodeXmlParser
//------------------------------------------------------------------------------
CPosRevGeoCodeXmlParser::~CPosRevGeoCodeXmlParser()
    {
    FUNC("CPosRevGeoCodeXmlParser::~CPosRevGeoCodeXmlParser");
    delete iParser;
    delete iBuffer;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::CPosRevGeoCodeXmlParser
//------------------------------------------------------------------------------
CPosRevGeoCodeXmlParser::CPosRevGeoCodeXmlParser( MPosRevGeoCodeXmlObserver& aObserver ):
    iObserver( aObserver ),
    iParser( NULL ),
    iBuffer( NULL ),
    iThoroughfare( EFalse )
    {
    FUNC("CPosRevGeoCodeXmlParser::CPosRevGeoCodeXmlParser");
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::ConstructL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::ConstructL()
    {
    FUNC("CPosRevGeoCodeXmlParser::ConstructL");
    iParser = CParser::NewL( KXmlMimeType,*this );
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::StartParsingL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::StartParsingL( HBufC8 *aXmlInfo,
                                             CPosLandmark* aLandmarkInfo )
    {
    FUNC("CPosRevGeoCodeXmlParser::StartParsingL");
    // Store client's landmark
    iClientLandmark = aLandmarkInfo;
    
    if( iBuffer )
        {
        delete iBuffer;
        iBuffer = NULL;
        }

    iBuffer = HBufC8::NewL( aXmlInfo->Size() );
    TPtr8 ptr = iBuffer->Des();
    ptr.Copy( aXmlInfo->Ptr(),aXmlInfo->Size() );
    
    // Now, we have the whole file content in iBuffer.
    // We are ready to parse the XML content.
    iParser->ParseBeginL();
    iParser->ParseL( *iBuffer );
    
    // Since we read the whole file contents within one-shot,
    // we can call ParseEndL() right after calling ParseL().
    iParser->ParseEndL();
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnStartDocumentL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnStartDocumentL( const RDocumentParameters& /*aDocParam*/,
                                                TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnStartDocumentL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnEndDocumentL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnEndDocumentL( TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnEndDocumentL");
    iObserver.OnParseCompletedL( aErrorCode );
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnStartElementL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnStartElementL( const RTagInfo& aElement,
                                               const RAttributeArray& /*aAttributes*/,
                                               TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnStartElementL");
    if ( aErrorCode == KErrNone )
        {
        // If we find the start of an element, we write to the screen,
        // for example: "<tag>"
        
        if( !aElement.LocalName().DesC().Compare( KCountry ) )
            {
            iCurrentElement = ECountryName;
            }
        else if( !aElement.LocalName().DesC().Compare( KState ) )
            {
            iCurrentElement = EState;
            }
        else if( !aElement.LocalName().DesC().Compare( KCity ) )
            {
            iCurrentElement = ECity;
            }
        else if( !aElement.LocalName().DesC().Compare( KDistrict ) )
            {
            iCurrentElement = EDistrict;
            }
        else if( !aElement.LocalName().DesC().Compare( KPostalCode )  )
            {
            iCurrentElement = EPostalCode;
            }
        else if( !aElement.LocalName().DesC().Compare( KThoroughfare )  )
            {
            iThoroughfare = ETrue;
            }
        else if( !aElement.LocalName().DesC().Compare( KNameTag ) && iThoroughfare )
            {
            iCurrentElement = EThoroughfareName;
            }
        else if( !aElement.LocalName().DesC().Compare( KNumberTag ) && iThoroughfare )
            {
            iCurrentElement = EThoroughfareNumber;
            }
        else
            {
            // Nothing to be done here.
            }
        }
    else
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
        
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnEndElementL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnEndElementL( const RTagInfo& /*aElement*/, TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnEndElementL");
    if( aErrorCode == KErrNone )
        {
        // at the end of the tag </tag>
        //Set it to ENone
        iCurrentElement = ENone;
        iThoroughfare = EFalse;
        }
    else
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
    
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnContentL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnContentL( const TDesC8 &aBytes, TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnContentL");
    if( aErrorCode == KErrNone )
        {

        if( iCurrentElement == ENone )
            {
            //ignore if the current element is not there
            return;
            }

        // convert the content to UCS-2
        // from UTF-8        
        RBuf buffer;
        CleanupClosePushL(buffer);
        buffer.CreateL( aBytes.Length() );
        CnvUtfConverter::ConvertToUnicodeFromUtf8( buffer,aBytes );
        
        if( iCurrentElement == ECountryName )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldCountry,buffer );
            }
        else if( iCurrentElement == EState )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldState,buffer );
            }
        else if( iCurrentElement == EDistrict )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldDistrict,buffer );
            }
        else if( iCurrentElement == ECity )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldCity,buffer );
            }
        else if( iCurrentElement == EPostalCode )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldPostalCode,buffer );
            }
        else if( iCurrentElement == EThoroughfareName )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldStreet,buffer );
            }
        else if( iCurrentElement == EThoroughfareNumber )
            {
            iClientLandmark->SetPositionFieldL( EPositionFieldStreetExtension,buffer );
            }
        else    
            {
            // Nothing to be done here.
            }
        CleanupStack::PopAndDestroy( &buffer ); 
        }
    else
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
    
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnStartPrefixMappingL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnStartPrefixMappingL( const RString& /*aPrefix*/,
                                                     const RString& /*aUri*/,
                                                     TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnStartPrefixMappingL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
        
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnEndPrefixMappingL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnEndPrefixMappingL( const RString& /*aPrefix*/,
                                                   TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnEndPrefixMappingL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
    
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnIgnorableWhiteSpaceL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnIgnorableWhiteSpaceL( const TDesC8& /*aBytes*/,
                                                      TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnIgnorableWhiteSpaceL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }
    
//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnSkippedEntityL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnSkippedEntityL( const RString& /*aName*/,
                                                TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnSkippedEntityL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnProcessingInstructionL
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnProcessingInstructionL( const TDesC8& /*aTarget*/,
                                                        const TDesC8& /*aData*/, 
                                                        TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnProcessingInstructionL");
    if( aErrorCode != KErrNone )
        {
        iObserver.OnParseCompletedL( aErrorCode );
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::OnError
//------------------------------------------------------------------------------
void CPosRevGeoCodeXmlParser::OnError( TInt aErrorCode )
    {
    FUNC("CPosRevGeoCodeXmlParser::OnError");
    TRAP_IGNORE( iObserver.OnParseCompletedL( aErrorCode ) );
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodeXmlParser::GetExtendedInterface
//------------------------------------------------------------------------------
TAny* CPosRevGeoCodeXmlParser::GetExtendedInterface( const TInt32 /*aUid*/ )
    {
    FUNC("CPosRevGeoCodeXmlParser::GetExtendedInterface");
    return NULL;
    }

// End of File
