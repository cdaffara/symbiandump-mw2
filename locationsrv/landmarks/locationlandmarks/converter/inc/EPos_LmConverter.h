/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     Common declarations for Landmarks Content Format Converter
*
*/


#ifndef LMCONVERTER_H
#define LMCONVERTER_H

//  INCLUDES
#include <e32def.h>
#include <e32std.h>

_LIT( KPosLmConverterPanic, "LmConverter" );

// DATA TYPES

enum TPosXmlTagType
    {
    EPosXmlNothing              = 0x00000000,
    EPosXmlUnknown              = 0x80000000,
    /*
     * Branches/parents
     *
     * Parents values are hard coded and should not be used for storing
     * permanently in a file or db, since these can change with new defined
     * branches/parent tags.
     */
    EPosXmlRoot                 = 0x40000000,
    EPosXmlLmCollection         = 0x20000000,
    EPosXmlMetadata             = 0x10000000,
    EPosXmlLandmark             = 0x08000000,
    EPosXmlLmCoordinates        = 0x04000000,
    EPosXmlLmAddressInfo        = 0x02000000,
    EPosXmlLmMediaLink          = 0x01000000,
    EPosXmlLmCategory           = 0x00800000,
	EPosXmlDocument           	= 0x00400000,//added by mahendran
	EPosXmlFolder				= 0x00200000,
    EPosXmlLmLastTree           = EPosXmlLmCategory,

    /*
     * Leafs/children
     *
     * Leafs values are initiated from a defined value. Values should not be
     * used for storing permanently in a file or db,
     * since these can change with new defined branches/parent tags.
     */
    EPosXmlLmLeafsStart         = 0x00000001,
    EPosXmlLmCollName,
    EPosXmlLmCollDescription,
    EPosXmlLmName,
    EPosXmlLmDescription,
    EPosXmlLmDescription2,
    EPosXmlLmCoverageRadius,

    EPosXmlCoordLatitude,
    EPosXmlCoordLongitude,
    EPosXmlCoordAltitude,
    EPosXmlCoordHorizontalAcc,
    EPosXmlCoordVerticalAcc,
    EPosXmlCoordTimestamp,
	
    EPosXmlAddrCountry,
    EPosXmlAddrCountryCode,
    EPosXmlAddrState,
    EPosXmlAddrCounty,
    EPosXmlAddrCity,
    EPosXmlAddrDistrict,
    EPosXmlAddrPostalCode,
    EPosXmlAddrCrossing1,
    EPosXmlAddrCrossing2,
    EPosXmlAddrStreet,
    EPosXmlAddrBuildingName,
    EPosXmlAddrBuildingZone,
    EPosXmlAddrBuildingFloor,
    EPosXmlAddrBuildingRoom,
    EPosXmlAddrExtension,
    EPosXmlAddrPhoneNumber,

    EPosXmlMediaLinkName,
    EPosXmlMediaLinkMime,
    EPosXmlMediaLinkUrl,

    EPosXmlCategoryId,
    EPosXmlCategoryName,
    
    EPosXmlVersion,
    EPosXmlpoint,
    EPosXmlpointcoordinates,
    EPosXmlAddress,
    EPosXmlPhoneNumber,
    EPosXmlAddrPlaceId, //Addition of Place id RM-RIM[]
    };

const TInt KPosStringLength = 40;
const TInt KPosLongStringLength = 400;

const TInt KPosCountryCodeLength = 2;
/** User equivalent range error */
const TReal KPosLmDopToAccuracyFactor = 8;

// Common XML strings
_LIT(KPosXmlVersionNum10,       "1.0");
_LIT(KPosXmlVersionNum11,       "1.1");
_LIT(KPosXmlEncNameUTF8,        "UTF-8");
_LIT(KPosXmlEncNameUTT16,       "UTF-16");

_LIT(KPosXmlDeclStart,          "<?xml");
_LIT(KPosXmlDeclEnd,            "?>");
_LIT(KPosXmlSpace,              " ");
_LIT(KPosXmlEq,                 "=");
_LIT(KPosXmlQuotationMark,      "\"");
_LIT(KPosXmlNewLine,            "\r\n");
_LIT(KPosXmlSlash,              "/");
_LIT(KPosXmlVersion,            "version");
_LIT(KPosXmlEncoding,           "encoding");

_LIT(KPosXmlTab,                "\t");
_LIT(KPosXmlSTagStart,          "<");
_LIT(KPosXmlETagStart,          "</");
_LIT(KPosXmlNameSpacePrefix,    "lm:");
_LIT(KPosXmlTagEnd,             ">");

_LIT(KPosXmlLmNSAttName,        "xmlns:lm");
_LIT(KPosXmlXsiNSAttName,       "xmlns:xsi");
_LIT(KPosXmlSchemaLocationAttName, "xsi:schemaLocation");

_LIT(KPosXmlXsiNSAttValue,      "http://www.w3.org/2001/XMLSchema-instance");
_LIT(KPosXmlSchemaLocationAttValue,
    "http://www.nokia.com/schemas/location/landmarks/");
_LIT(KPosXmlVersionAttValue,    "1/0");
_LIT(KPosXmlSchemaAttValue,     "lmx.xsd");

// LMX xml strings
_LIT(KPosXmlLmxRoot,            "lmx");

_LIT(KPosXmlLmCollection,       "landmarkCollection");
_LIT(KPosXmlLmCollName,         "name");
_LIT(KPosXmlLmCollDescription,  "description");

_LIT(KPosXmlLm,                 "landmark");
_LIT(KPosXmlLmName,             "name");
_LIT(KPosXmlLmDescription,      "description");
_LIT(KPosXmlLmCoordinates,      "coordinates");
_LIT(KPosXmlLmCoverageRadius,   "coverageRadius");
_LIT(KPosXmlLmAddressInfo,      "addressInfo");
_LIT(KPosXmlLmMediaLink,        "mediaLink");
_LIT(KPosXmlLmCategory,         "category");

_LIT(KPosXmlAddrCountry,        "country");
_LIT(KPosXmlAddrCountryCode,    "countryCode");
_LIT(KPosXmlAddrState,          "state");
_LIT(KPosXmlAddrCounty,         "county");
_LIT(KPosXmlAddrCity,           "city");
_LIT(KPosXmlAddrDistrict,       "district");
_LIT(KPosXmlAddrPostalCode,     "postalCode");
_LIT(KPosXmlAddrCrossing1,      "crossing1");
_LIT(KPosXmlAddrCrossing2,      "crossing2");
_LIT(KPosXmlAddrStreet,         "street");
_LIT(KPosXmlAddrBuildingName,   "buildingName");
_LIT(KPosXmlAddrBuildingZone,   "buildingZone");
_LIT(KPosXmlAddrBuildingFloor,  "buildingFloor");
_LIT(KPosXmlAddrBuildingRoom,   "buildingRoom");
_LIT(KPosXmlAddrExtension,      "extension");
_LIT(KPosXmlAddrPhoneNumber,    "phoneNumber");
_LIT(KPosXmlAddrPlaceID,            "placeid");

_LIT(KPosXmlCoordLatitude,      "latitude");
_LIT(KPosXmlCoordLongitude,     "longitude");
_LIT(KPosXmlCoordAltitude,      "altitude");
_LIT(KPosXmlCoordHorizontalAcc, "horizontalAccuracy");
_LIT(KPosXmlCoordVerticalAcc,   "verticalAccuracy");
_LIT(KPosXmlCoordTimeStamp,     "timeStamp");

_LIT(KPosXmlMediaLinkName,      "name");
_LIT(KPosXmlMediaLinkMime,      "mime");
_LIT(KPosXmlMediaLinkUrl,       "url");

_LIT(KPosXmlCategoryId,         "id");
_LIT(KPosXmlCategoryName,       "name");

// GPX xml strings
_LIT(KPosXmlGpxRoot,            "gpx");
_LIT(KPosXmlGpxCollName,        "name");
_LIT(KPosXmlGpxCollDesc,        "desc");
_LIT(KPosXmlGpxMetadata,        "metadata"); // v1.1

_LIT(KPosXmlGpxWaypoint,        "wpt");
_LIT(KPosXmlGpxLmName,          "name");
_LIT(KPosXmlGpxLmDescription,   "desc");
_LIT(KPosXmlGpxLmComment,       "cmt");

_LIT(KPosXmlGpxLmLatitude,      "lat");
_LIT(KPosXmlGpxLmLongitude,     "lon");
_LIT(KPosXmlGpxLmAltitude,      "ele");

_LIT(KPosXmlGpxUrl,             "url"); // v1.0
_LIT(KPosXmlGpxUrlName,         "urlname"); // v1.0
_LIT(KPosXmlGpxLink,            "link"); // v1.1
_LIT(KPosXmlGpxLinkHref,        "href"); // v1.1
_LIT(KPosXmlGpxLinkText,        "text"); // v1.1
_LIT(KPosXmlGpxLinkType,        "type"); // v1.1

_LIT(KPosXmlGpxCategory,        "type");

_LIT(KPosXmlGpxHdop,            "hdop");
_LIT(KPosXmlGpxVdop,            "vdop");


// KML xml strings
_LIT(KPosXmlKmlRoot,            "kml");
_LIT(KPosXmlKmlCollName,        "name");
_LIT(KPosXmlKmlCollDesc,        "description");
_LIT(KPosXmlKmlMetadata,        "Metadata"); 
_LIT(KPosXmlKmlDocument,        "Document");
_LIT(KPosXmlKmlFolder,          "Folder");
_LIT(KPosXmlKmlPlacemark,       "Placemark");
_LIT(KPosXmlKmlLmName,          "name");
_LIT(KPosXmlKmlLmDescription,   "description");
//_LIT(KPosXmlGpxLmComment,       "cmt");

_LIT(KPosXmlKmlLmlocation,      "Point");
_LIT(KPosXmlKmlLmCoordinates,      "coordinates");
_LIT(KPosXmlKmlLmAddress,      "address");
_LIT(KPosXmlKmlLmPhonenumber,      "phoneNumber");

// URL constants
const TInt KUrlMaxTextEncodingLength = 768;
const TInt KUrlMaxDescriptionEncodingLength = 12288;
const TInt KUrlMaxCategoryEncodingLength = 372;
const TInt KRealWidth = 50;
_LIT( KUrlHex , "%x" );
_LIT8( KUrlSchemeHttp, "http" );
_LIT8( KUrlHostFull,   "www.maps.ovi.com" );
_LIT( KUrlQueryStart, "/?" );
_LIT( KUrlQueryDelimeter, "&" );
_LIT( KUrlLatitudeParam,  "la=" );
_LIT( KUrlLongitudeParam, "lo=" );
_LIT( KUrlAltitudeParam,  "a=" );
_LIT( KUrlEscapeCharacter, "%" );
_LIT( KUrlNameParam,      "n=" );
_LIT( KUrlDescriptionParam, "d=" );
_LIT( KUrlHouseNumberParam, "sn=" );
_LIT( KUrlStreetParam,      "s=" );
_LIT( KUrlCityParam,        "c=" );
_LIT( KUrlStateParam,       "sp=" );
_LIT( KUrlCountryParam,     "cr=" );
_LIT( KUrlPostalCodeParam,  "pz=" );
_LIT( KUrlTelephoneParam,   "t=" );
_LIT( KUrlWebAddressParam,  "w=" );
_LIT( KUrlPositionAccuracyParam,  "pa=" );
_LIT( KUrlAltitudeAccuracyParam,  "aa=" );
_LIT( KUrlSpeedParam,        "v=" ); 
_LIT( KUrlHeadingParam,      "h=" );
_LIT( KUrlCategoryParam,    "cat=" );
_LIT( KUrlWebParam,         "w=" ); 
_LIT( KUrlPidParam,     "pid=" );
_LIT( KUrlTimeStampParam, "ts=" );
// constants used for encoding the timestamp
_LIT( KYearFormat, "%04d" );
_LIT( KFormatMicroSeconds, "%06d" );
_LIT( KFormat, "%02d" );
_LIT( KTimeMicroSecondSeparator, "." );
_LIT( KDateTimeSeparator,   ":" );
// constants used during the parsing of the URL
const TUint KParamSeperator='&';
_LIT8( KUrlQueryStartDelimeter, "/?" );
_LIT8( KUrlSchemeHttps, "https" );
_LIT8( KUrlHost,   "maps.ovi.com" );
_LIT8( KUrlHostDelimeter,   "://" );
_LIT8( KUrlLocationDelimeter, "??" );
_LIT( KLocationDelimeter, "??" );
_LIT8( KUrlParameterDelimeter, "&" );
_LIT( KUrlParameterValueDelimeter, "=" );
_LIT8( KUrlLatitude, "la" );
_LIT8( KUrlLongitude, "lo" );
_LIT8( KUrlAltitude, "a" );
_LIT8( KUrlPositionAccuracy,  "pa" );
_LIT8( KUrlAltitudeAccuracy, "aa" );
_LIT8( KUrlSpeed, "v" );
_LIT8( KUrlHeading, "h" );
_LIT8( KUrlName, "n" );
_LIT8( KUrlStreet, "s" );
_LIT8( KUrlHouseNumber, "sn" );
_LIT8( KUrlPostalCode, "pz" );
_LIT8( KUrlCity, "c" );
_LIT8( KUrlState, "sp" );
_LIT8( KUrlCountry, "cr" );
_LIT8( KUrlTelephone, "t" );
_LIT8( KUrlWebUrl, "w" );
_LIT8( KUrlDescription, "d" );
_LIT8( KUrlPlaceId, "pid" );
_LIT8( KUrlTimestamp, "ts" );
_LIT8( KUrlCategory, "cat" );
_LIT8( KUrlDateTimeDelimeter,":" );
_LIT8( KUrlTimeDelimeter, "." );




/*_LIT(KPosXmlGpxLmLongitude,     "lon");
_LIT(KPosXmlGpxLmAltitude,      "ele");

_LIT(KPosXmlGpxUrl,             "url"); // v1.0
_LIT(KPosXmlGpxUrlName,         "urlname"); // v1.0
_LIT(KPosXmlGpxLink,            "link"); // v1.1
_LIT(KPosXmlGpxLinkHref,        "href"); // v1.1
_LIT(KPosXmlGpxLinkText,        "text"); // v1.1
_LIT(KPosXmlGpxLinkType,        "type"); // v1.1

_LIT(KPosXmlGpxCategory,        "type");

_LIT(KPosXmlGpxHdop,            "hdop");
_LIT(KPosXmlGpxVdop,            "vdop");*/

#ifdef _DEBUG

#define LOG( a )                { RDebug::Print( _L( a ) ); }
#define LOG1( a, b )            { RDebug::Print( _L( a ), b ); }
#define LOG2( a, b, c )         { RDebug::Print( _L( a ), b, c ); }
#define LOG3( a, b, c, d )      { RDebug::Print( _L( a ), b, c, d ); }
#define LOG4( a, b, c, d, e )   { RDebug::Print( _L( a ), b, c, d, e ); }

#else // _DEBUG

// Release version - no logging
#define LOG( a)
#define LOG1( a, b )
#define LOG2( a, b, c)
#define LOG3( a, b, c, d)
#define LOG4( a, b, c, d, e )

#endif

#endif      // LMCONVERTER_H

// End of File
