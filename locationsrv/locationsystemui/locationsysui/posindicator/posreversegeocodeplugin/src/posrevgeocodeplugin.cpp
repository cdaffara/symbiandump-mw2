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
* Description: Implementation of CPosRevGeoCodePlugin class.
*
*/

#include "posrevgeocodeplugin.h"
#include "posrevgeocodelogger.h"

#include <lbsposition.h>

//Maximum length for the response buffer size
const int KMaxResponseLength = 2048;
//Authentication token for the Maps server
//This authentication in specific to our photo-tagging feature
_LIT8( KAuthenticationToken, "88dedc61fb43c924aa39a873dadd41f4" );
//Static RefererURL which shows that request has been made from this app
_LIT8( KRefererURL, "nokia_positioningIndicator" );

//Language option for the REST request
_LIT8( KDefaultLanguage, "eng" );


//Format of the HTTP request for the reverse geocode
_LIT8( KRequestFormat, "http://loc.mobile.maps.svc.ovi.com/geocoder/rgc/1.0?token=%S&referer=%S&n=10&lat=%f&long=%f&lg=%S&output=xml" );


// http://www.loc.gov/marc/languages/
// These are the nokia language id converted to MARC language strings.
 static const char Marc_Table[ 104 ][ 4 ] = {
        "   ", // dummy
        "ENG", // 1 English
        "FRE", // 2 French
        "GER", // 3 German
        "SPA", // 4 Spanish
        "ITA", // 5 Italian
        "SWE", // 6 Swedish
        "DAN", // 7 Danish
        "NOR", // 8 Norwegian
        "FIN", // 9 Finnish
        "010", // 10 American
        "011", // 11 Swiss French
        "GSW", // 12 Swiss German
        "POR", // 13 Portuguese
        "TUR", // 14 Turkish
        "ICE", // 15 Icelandic
        "RUS", // 16 Russian
        "HUN", // 17 Hungarian
        "DUT", // 18 Dutch
        "019", // 19 Flemish
        "020", // 20 Australian English
        "021", // 21 Belgian French
        "022", // 22 Austrian German
        "023", // 23 New Zealand English
        "FRE", // 24 International French
        "CZE", // 25 Czech
        "SLO", // 26 Slovak
        "POL", // 27 Polish
        "SLV", // 28 Slovenian
        "029", // 29 TaiwanChinese
        "CHT", // 30 HongKongChinese
        "CHI", // 31 PeoplesRepublicOfChina Chinese
        "JPN", // 32 Japanese
        "THA", // 33 Thai
        "AFR", // 34 Afrikaans
        "ALB", // 35 Albanian
        "AMH", // 36 Amharic
        "ARA", // 37 Arabic
        "ARM", // 38 Armenian
        "TGL", // 39 Tagalog
        "BEL", // 40 Belarusian
        "BEN", // 41 Bengali
        "BUL", // 42 Bulgarian
        "BUR", // 43 Burmese
        "CAT", // 44 Catalan
        "SCR", // 45 Croatian
        "046", // 46 Canadian English
        "ENG", // 47 International English
        "048", // 48 SouthAfrican English
        "EST", // 49 Estonian
        "PER", // 50 Persian (Farsi)
        "051", // 51 Canadian French
        "GAE", // 52 Scots Gaelic
        "GEO", // 53 Georgian
        "GRE", // 54 Greek
        "055", // 55 Cyprus Greek
        "GUJ", // 56 Gujarati
        "HEB", // 57 Hebrew
        "HIN", // 58 Hindi
        "IND", // 59 Bahasa indonesia
        "GLE", // 60 Irish
        "061", // 61 Swiss Italian
        "KAN", // 62 Kannada
        "KAZ", // 63 Kazakh
        "KHM", // 64 Khmer
        "KOR", // 65 Korean
        "LAO", // 66 Lao
        "LAV", // 67 Latvian
        "LIT", // 68 Lithuanian
        "MAC", // 69 Macedonian
        "070", // 70 Bahasa Malaysia
        "MAL", // 71 Malayalam
        "MAR", // 72 Marathi
        "MOL", // 73 Moldavian
        "MON", // 74 Mongolian
        "NNO", // 75 Norwegian Nynorsk
        "076", // 76 Brazilian Portuguese
        "PAN", // 77 Punjabi
        "RUM", // 78 Romanian
        "SCC", // 79 Serbian
        "SNH", // 80 Sinhalese
        "SOM", // 81 Somali
        "082", // 82 International Spanish
        "083", // 83 LatinAmerican Spanish
        "SWA", // 84 Swahili
        "085", // 85 Finland Swedish
        "TAJ", // 86 Tajik
        "TAM", // 87 Tamil
        "TEL", // 88 Telugu
        "TIB", // 89 Tibetan
        "TIR", // 90 Tigrinya
        "091", // 91 Cyprus Turkish
        "TUK", // 92 Turkmen
        "UKR", // 93 Ukrainian
        "URD", // 94 Urdu
        "UZB", // 95 Uzbek
        "VIE", // 96 Vietnamese
        "WEL", // 97 Welsh
        "ZUL", // 98 Zulu
        "UND", // 99 Other
        "UND", // 100 Undef
        "UND", // 101 Undef
        "BAQ", // 102 Basque
        "103", // 103 Galician
    };

// Timer interval
const TInt KTimeOutInterval = 60000000;  // 60 seconds


// ECOM implementation specifics
static const TImplementationProxy implTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x2002E693, CPosRevGeoCodePlugin::NewL )
    }; 


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(implTable) / sizeof(TImplementationProxy);

    return implTable;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::NewL
//------------------------------------------------------------------------------
CPosRevGeoCodePlugin* CPosRevGeoCodePlugin::NewL( MPosReverseGeocodeObserver& aObserver )
    {
    FUNC("CPosRevGeoCodePlugin::NewL");
    CPosRevGeoCodePlugin *self = new (ELeave) CPosRevGeoCodePlugin( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::CPosRevGeoCodePlugin
//------------------------------------------------------------------------------
CPosRevGeoCodePlugin::CPosRevGeoCodePlugin( MPosReverseGeocodeObserver& aObserver ):
	 																					iObserver( aObserver ),
	 																					iHTTPClientEngine ( NULL ),
                                            iXmlParser ( NULL ),                                           
                                            iConnectionTimer( NULL ),
                                            iXMLBuf ( NULL )        
    {
    FUNC("CPosRevGeoCodePlugin::CPosRevGeoCodePlugin");
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::~CPosRevGeoCodePlugin
//------------------------------------------------------------------------------
CPosRevGeoCodePlugin::~CPosRevGeoCodePlugin()
    {
    FUNC("CPosRevGeoCodePlugin::~CPosRevGeoCodePlugin");
     
    delete iConnectionTimer;
    iConnectionTimer = NULL;
    
    delete iXMLBuf;
    iXMLBuf = NULL;
    
    delete iXmlParser;
    iXmlParser = NULL;
    
    delete iHTTPClientEngine;
    iHTTPClientEngine = NULL;
    FUNC("out>>CPosRevGeoCodePlugin");
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::GetAddressByCoordinateL
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::GetAddressByCoordinateL( CPosLandmark& aLandmark )
    {
    FUNC("CPosRevGeoCodePlugin::GetAddressByCoordinateL");
    // Store the landmark object.
    iClientLandmark = &aLandmark;
    
    TLocality locality;
    User::LeaveIfError( aLandmark.GetPosition( locality ) );
    TReal64 latitude = locality.Latitude();
    TReal64 longitude = locality.Longitude();
    
    iAuthCode.Copy( KAuthenticationToken );
    iRefURL.Copy( KRefererURL );
    
    GetLanguageForTheRequest( iLang );
    
    //Form the request URI
    iQueryString.Format( KRequestFormat, &iAuthCode, &iRefURL, latitude, longitude, &iLang );
    
    iHTTPClientEngine->IssueHTTPGetL( iQueryString );
    
    if ( iConnectionTimer && iConnectionTimer->IsActive() )
        {
        iConnectionTimer->Cancel();
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::Cancel()
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::Cancel()
    {
    if( iHTTPClientEngine->IsActive() )
        {
        iHTTPClientEngine->CancelTransaction();
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::ConstructL
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::ConstructL()
    {
    FUNC("CPosRevGeoCodePlugin::ConstructL");
    iXMLBuf = HBufC8::NewL( KMaxResponseLength );

    iXmlParser = CPosRevGeoCodeXmlParser::NewL( *this );
    
    iHTTPClientEngine = CPosRevGeoCodeHTTPClientEngine::NewL( *this );
    
    iConnectionTimer = CPosRevGeoCodeConnectionTimer::NewL(*this);
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::OnParseCompletedL
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::OnParseCompletedL( TInt aError )
    {
    FUNC("CPosRevGeoCodePlugin::OnParseCompletedL");
    StartTimer();
    iObserver.ReverseGeocodeComplete( aError );
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::ClientEvent
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::ClientEvent( const THttpStatus& aEvent )
    {
    FUNC("CPosRevGeoCodePlugin::ClientEvent");
    TInt error;
    switch( aEvent )
        {
        case EHttpTxCompleted:
            //Reading is done
            //Parse the String and get the restults
            if( iXmlParser )
            {
             TRAP_IGNORE( iXmlParser->StartParsingL( iXMLBuf,iClientLandmark ) );
            }
            break;
        //All these cases will in turn lead to
        //generic failure due to connection/Tx related problems    
        case EHttpConnectionFailure:
            // May happen if Socket connection fails
            error = KErrCouldNotConnect;
            iObserver.ReverseGeocodeComplete( error );
            break;
        case EHttpTxFailed:
        case EHttpMhfRunError:
            // May happen if Socket connection fails
            // Complete the RGC with generic error.
            error = KErrGeneral;
            iObserver.ReverseGeocodeComplete( error );
            break;
        case EHttpTxCancelled:
            //On Cancellation of request.
            error = KErrCancel;
            iObserver.ReverseGeocodeComplete( error );
            break;
        case EHttpAuthFailed:
            //On authentication failed.
            error = KErrPermissionDenied;
            iObserver.ReverseGeocodeComplete( error );
            break;
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::ClientBodyReceived
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::ClientBodyReceived( const TDesC8& aBodyData )
    {
    FUNC("CPosRevGeoCodePlugin::ClientBodyReceived");
    //Dump the contents here
    TPtr8 ptr = iXMLBuf->Des();
    ptr.Zero();
    ptr.Append( aBodyData );
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::GetLanguageForTheRequest
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::GetLanguageForTheRequest( TDes8& aLanguage )
    {
    FUNC("CPosRevGeoCodePlugin::GetLanguageForTheRequest");
    // Get the current phone langauge  
    TInt phoneLangIndex = User::Language();

    // Get the converted language 
    if (  phoneLangIndex < sizeof( Marc_Table ) / sizeof( Marc_Table[ 0 ]  ) )
        {
        aLanguage = (const TUint8*) Marc_Table[ phoneLangIndex ];
        }
    else
        {
         //By default language will be Eng
        aLanguage.Copy( KDefaultLanguage );
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::CloseConnection
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::CloseConnection()
    {
    FUNC("CPosRevGeoCodePlugin::CloseConnection");
    if( iHTTPClientEngine )
        {
        iHTTPClientEngine->CloseConnection();
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::StartTimer
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::StartTimer()
    {
    FUNC("CPosRevGeoCodePlugin::StartTimer");
    if( iConnectionTimer )
        {
        iConnectionTimer->StartTimer( KTimeOutInterval );
        }
    }

//------------------------------------------------------------------------------
// CPosRevGeoCodePlugin::HandleTimedoutEvent
//------------------------------------------------------------------------------
void CPosRevGeoCodePlugin::HandleTimedoutEvent(TInt /*aErrorCode*/)
    {
    FUNC("CPosRevGeoCodePlugin::HandleTimedoutEvent");
    CloseConnection();
    }

//end of file
