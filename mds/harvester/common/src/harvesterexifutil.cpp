/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Exif-utilities for harvester
*
*/

#include "harvesterexifutil.h"

#include "mdsutils.h"
#include "mdeobjectdef.h"
#include "mdeconstants.h"
#include "mdeproperty.h"
#include "tz.h"
#include <ExifModify.h>
#include "OstTraceDefinitions.h"

#ifdef OST_TRACE_COMPILER_IN_USE
#include "harvesterexifutilTraces.h"
#endif



using namespace MdeConstants;

const TUint16 KIdColorSpace = 0xA001;
const TUint16 KIdResolutionUnit = 0x0128;
const TUint16 KIdYbCrPositioning = 0x0213;
const TUint16 KIdImageDescription = 0x010E;
const TUint16 KIdCopyright = 0x8298;
const TUint16 KIdUserComment = 0x9286;
const TUint16 KIdDateTime = 0x0132;
const TUint16 KIdDateTimeOriginal = 0x9003;
const TUint16 KIdDateTimeDigitized = 0x9004;
const TUint16 KIdDateTimeModified = 0x132;
const TUint16 KIdFNumber = 0x829D;
const TUint16 KIdMake = 0x010F;
const TUint16 KIdModel = 0x0110;
const TUint16 KIdFocalLength = 0x920A;
const TUint16 KIdFocalLengthIn35mm = 0xA405;
const TUint16 KIdSamplesPerPixel = 0x0115;
const TUint16 KIdISOSpeedRatings = 0x8827;
const TUint16 KIdComponentsConfig = 0x9101;
const TUint16 KIdArtist = 0x013B;
const TUint16 KIdPixelXDimension = 0xA002;
const TUint16 KIdPixelYDimension = 0xA003;
const TUint16 KIdRelatedSoundFile = 0xA004;
const TUint16 KIdFocalPlaneResolutionUnit = 0xA210;
const TUint16 KIdFocalPlaneXResolution = 0xA20E;
const TUint16 KIdFocalPlaneYResolution = 0xA20F;

const TUint16 KIdExposureTime = 0x829A;
const TUint16 KIdApertureValue = 0x9202;
const TUint16 KIdExposureBias = 0x9204;
const TUint16 KIdMeteringMode = 0x9207;
const TUint16 KIdShutterSpeed = 0x9201;
const TUint16 KIdXResolution = 0x011A;
const TUint16 KIdYResolution = 0x011B;
const TUint16 KIdWhiteBalance = 0xA403;
const TUint16 KIdExposureProgram = 0x8822;
const TUint16 KIdFlash = 0x9209;
const TUint16 KIdOrientation = 0x112;

const TUint16 KIdGpsLatitudeRef = 0x1;
const TUint16 KIdGpsLatitude = 0x2;
const TUint16 KIdGpsLongitudeRef = 0x3;
const TUint16 KIdGpsLongitude = 0x4;
const TUint16 KIdGpsAltitudeRef = 0x5;
const TUint16 KIdGpsAltitude = 0x6;
const TUint16 KIdGpsMeasureMode = 0xA;
const TUint16 KIdGpsDop = 0xB;
const TUint16 KIdGpsMapDatum = 0x12;

_LIT( KExifDateTimeFormat, "%F%Y:%M:%D %H:%T:%S\0" );
const TInt KDateBufferSize = 20;
const TInt KCoordinateBufferSize = 24;

// This is needed for exif description field
_LIT8( KAsciiCodeDesignation,     "\x41\x53\x43\x49\x49\x00\x00\x00");
_LIT8( KJisCodeDesignation,     "\x4A\x49\x53\x00\x00\x00\x00\x00");
_LIT8( KUnicodeCodeDesignation, "\x55\x4E\x49\x43\x4F\x44\x45\x00");
_LIT8( KUnknownCodeDesignation, "\x00\x00\x00\x00\x00\x00\x00\x00");

_LIT8( KNorth, "N\0" );
_LIT8( KSouth, "S\0" );
_LIT8( KEast, "E\0" );
_LIT8( KWest, "W\0" );
_LIT8( KMeasureMode2, "2\0" );
_LIT8( KMeasureMode3, "3\0" );
_LIT8( KMapDatum, "WGS-84\0");

CHarvesterExifUtil::CHarvesterExifUtil() :
    iSession( NULL ), iDefaultNamespace( NULL )
{
}


EXPORT_C CHarvesterExifUtil::~CHarvesterExifUtil()
{
}

EXPORT_C CHarvesterExifUtil* CHarvesterExifUtil::NewLC()
{
	CHarvesterExifUtil* self = new (ELeave)CHarvesterExifUtil();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

EXPORT_C CHarvesterExifUtil* CHarvesterExifUtil::NewL()
{
	CHarvesterExifUtil* self=CHarvesterExifUtil::NewLC();
	CleanupStack::Pop( self ); // self;
	return self;
}

void CHarvesterExifUtil::ConstructL()
{

}

EXPORT_C void CHarvesterExifUtil::SetSession( CMdESession* aSession )
	{
	iSession = aSession;
	if ( !iDefaultNamespace && aSession )
		{
    	TRAP_IGNORE( iDefaultNamespace = &iSession->GetDefaultNamespaceDefL() );
		}
	}


EXPORT_C TBool CHarvesterExifUtil::IsValidExifData(TPtr8 aPtr)
	{
	WRITELOG( "CHarvesterExifUtil::IsValidExifData start" );
	OstTrace0( TRACE_NORMAL, CHARVESTEREXIFUTIL_ISVALIDEXIFDATA, "CHarvesterExifUtil::IsValidExifData start" );	
	
	CExifRead* reader = NULL;
	
	TRAPD(err, reader = CExifRead::NewL(aPtr, CExifRead::ENoJpeg | CExifRead::ENoTagChecking));
	if (err != KErrNone || !reader )
		{
		WRITELOG1( "CHarvesterExifUtil::IsValidExifData - error code: %d", err );
		OstTrace1( TRACE_NORMAL, DUP1_CHARVESTEREXIFUTIL_ISVALIDEXIFDATA, "CHarvesterExifUtil::IsValidExifData -error code:%d", err );

		return EFalse;
		}
	
	delete reader;
	reader = NULL;
	
	WRITELOG( "CHarvesterExifUtil::IsValidExifData end" );	
	OstTrace0( TRACE_NORMAL, DUP2_CHARVESTEREXIFUTIL_ISVALIDEXIFDATA, "CHarvesterExifUtil::IsValidExifData end" );
	
	return ETrue;
	}

void CHarvesterExifUtil::StripNulls( HBufC& aString )
	{
	_LIT( KNull, "\0" );
	_LIT( KSpace, " " );
	
	TInt pos( 0 );
    while( (pos = aString.Find( KNull ) ) != KErrNotFound )
    	{
    	aString.Des().Replace( pos, 1, KSpace );
    	}
    aString.Des().TrimAll();
	}

HBufC16* CHarvesterExifUtil::ReadExifTagL( const CExifRead& aReader, TExifIfdType aIFD, TUint16 aTagID )
	{
	HBufC16* destination = NULL;
    if( aReader.TagExists( aTagID, aIFD ) )
    	{
        const CExifTag* tag = aReader.GetTagL( aIFD, aTagID );
    	destination = CnvUtfConverter::ConvertToUnicodeFromUtf8L( tag->Data() );
    	StripNulls( *destination );    	
    	}	
    return destination;
	}

EXPORT_C TInt CHarvesterExifUtil::ReadExifDataL( CHarvestData& aHd, CFileData& aFileData )
    {
    WRITELOG( "CHarvesterExifUtil::ReadExifDataL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTEREXIFUTIL_READEXIFDATAL, "CHarvesterExifUtil::ReadExifDataL" );
    

    CExifRead* reader = CExifRead::NewL(
    		aFileData.iImageData->Des(), CExifRead::ENoJpeg | CExifRead::ENoTagChecking);
    CleanupStack::PushL(reader);
    
    // Getting description
    aHd.iDescription16 = ReadExifTagL( *reader, EIfd0, KIdImageDescription );

    // Getting UserComment
    ReadUserCommentL( aHd, reader ); 
          
    // Getting copyright
    aHd.iCopyright16 = ReadExifTagL( *reader, EIfd0, KIdCopyright );
    
    // Artist
    aHd.iArtist = ReadExifTagL( *reader, EIfd0, KIdArtist );
    
    // Getting whitebalance
    aHd.iStoreWhiteBalance = reader->GetWhiteBalance( aHd.iWhiteBalance ) == KErrNone;
 
    // Getting aHd.iFlash    
    aHd.iStoreFlash = reader->GetFlash( aHd.iFlash ) == KErrNone;
    
    // Getting exposure
    aHd.iStoreExposureProgram = reader->GetExposureProgram( aHd.iExposureProgram ) == KErrNone;
                
    // Getting width
    if ( reader->TagExists( KIdPixelXDimension, EIfdExif ) )
        {
        // PixelXDimension tag should be found in EXIF according to the standard.
        reader->GetPixelXDimension(aHd.iImageWidthExif);
        }
    
    // Getting height
    if ( reader->TagExists( KIdPixelYDimension, EIfdExif ) )
        {
        // PixelYDimension tag should be found in EXIF according to the standard.
        reader->GetPixelYDimension(aHd.iImageHeightExif);
        }
    
    // Getting aFileData.iModified date
    if ( reader->TagExists(KIdDateTime, EIfd0) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadExifDataL() - getting last aFileData.iModified date (exif)" );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTEREXIFUTIL_READEXIFDATAL, "CHarvesterExifUtil::ReadExifDataL - getting last aFileData.iModified date (exif)" );
        
        aHd.iDateModified8 = reader->GetDateTimeL();
        }
    
    // Getting original date
    if ( reader->TagExists(KIdDateTimeOriginal, EIfdExif) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadExifDataL() - getting original date (exif)" );
        OstTrace0( TRACE_NORMAL, DUP2_CHARVESTEREXIFUTIL_READEXIFDATAL, "CHarvesterExifUtil::ReadExifDataL - getting original date (exif)" );
        
        aHd.iDateOriginal8 = reader->GetDateTimeOriginalL();
        }
        
    // Getting date & time digitized
    if ( reader->TagExists(KIdDateTimeDigitized, EIfdExif) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadExifDataL() - getting digitized date (exif)" );
        OstTrace0( TRACE_NORMAL, DUP3_CHARVESTEREXIFUTIL_READEXIFDATAL, "CHarvesterExifUtil::ReadExifDataL - getting digitized date (exif)" );
        
        aHd.iDateDigitized8 = reader->GetDateTimeDigitizedL();
        }            
    
    //Getting camera maker
    aHd.iMake = ReadExifTagL( *reader, EIfd0, KIdMake );
    
    //Getting camera aHd.iModel
    aHd.iModel = ReadExifTagL( *reader, EIfd0, KIdModel );
    
    //Getting aHd.iOrientation
    aHd.iStoreOrientation = reader->GetOrientation( aHd.iOrientation ) == KErrNone;
    
    //Getting X Resolution
    ReadXResolutionL( aHd, reader ); 
    
    //Getting Y Resolution
    ReadXResolutionL( aHd, reader ); 
    
    //Getting resolution unit (mandatory tag)
    reader->GetResolutionUnit( aHd.iResolutionUnit );
    
    //Getting YCbCr Positioning
    aHd.iStoreYCbCrPositioning = reader->GetYCbCrPositioning( aHd.iYCbCrPositioning ) == KErrNone;
        
    //Getting exposure bias value
    ReadExposureBiasL( aHd, reader ); 
    
    //Getting exposure time
    ReadExposureTimeL( aHd, reader ); 
        
    //Getting FNumber
    ReadFNumberL( aHd, reader );
    
    //Getting Exif version
    aHd.iStoreExifVersion = reader->GetExifVersion( aHd.iExifVersion ) == KErrNone;           
        
    //Getting FlashPix version
    aHd.iStoreFlashPixVersion = reader->GetFlashPixVersion( aHd.iFlashPixVersion ) == KErrNone;
    
    // Shutter speed
    ReadShutterSpeedL( aHd, reader );
        
    //Getting aHd.iAperture
    ReadApertureValueL( aHd, reader );
    
    //Getting focal length
    ReadFocalLengthL( aHd, reader );
    
    // Getting focal length in 35 mm
    ReadFocalLength35mmL( aHd, reader );
        
    aHd.iStoreColourSpace = reader->GetColorSpace( aHd.iColourSpace ) == KErrNone;
    
    aHd.iStoreThumbCompression = reader->GetThumbnailCompression( aHd.iThumbCompression ) == KErrNone;
    
	TUint32 numerator = 0;
    TUint32 denominator = 0;
	
    //Getting thumbnail X resolution    
    TInt error = reader->GetThumbnailXResolution( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreThumbXResolution = ETrue;
        aHd.iThumbXResolution = 0.0f;
        if ( denominator > 0)
            {
            aHd.iThumbXResolution = numerator / denominator;
            }
        }
    
    //Getting thumbnail Y resolution
    error = reader->GetThumbnailYResolution( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreThumbYResolution = ETrue;
        aHd.iThumbYResolution = 0.0f;
        if ( denominator > 0 )
            {
            aHd.iThumbYResolution = numerator / denominator;
            }
        }
        
    aHd.iStoreThumbResolutionUnit =
    	reader->GetThumbnailResolutionUnit( aHd.iThumbResolutionUnit ) == KErrNone;
       
    // Bits per sample and Samples per pixel not recorded in JPEG Exif.
    if ( reader->TagExists( KIdSamplesPerPixel, EIfd0 ) )
        {
        const CExifTag* tag = reader->GetTagL( EIfd0, KIdSamplesPerPixel );
        TPtrC8 tagData = tag->Data();
        
        aHd.iSamplesPerPixel = MdsUtils::ToUInt16L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        aHd.iStoreSamplesPerPixel = ETrue;
        
        WRITELOG1( "CHarvesterExifUtil::ReadExifDataL() - samples per pixel: %d", aHd.iSamplesPerPixel );
        OstTrace1( TRACE_NORMAL, DUP4_CHARVESTEREXIFUTIL_READEXIFDATAL, "CHarvesterExifUtil::ReadExifDataL - samples per pixel: %d", aHd.iSamplesPerPixel );
        
        }
    
    //Getting ISO speed rating.
    if ( reader->TagExists(KIdISOSpeedRatings, EIfdExif) )
        {
        HBufC8* iso8 = reader->GetIsoSpeedRatingsL();
        
        if ( iso8 )
            {
            aHd.iStoreIsoSpeedRating = ETrue;
            aHd.iIsoSpeedRating = iso8->Des()[0];
            delete iso8;
            iso8 = NULL;
            }    
        }
    
    //Getting components configuration
    if ( reader->TagExists( KIdComponentsConfig, EIfdExif ) )
        {
        const CExifTag* tag = reader->GetTagL(
        		EIfdExif, KIdComponentsConfig );
        TPtrC8 tagData = tag->Data();
        
        aHd.iComponentsConfiguration = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        aHd.iStoreComponentsConfig = ETrue;
        }
    
    // Getting thumbnail compression
    aHd.iStoreThumbCompression =
    	reader->GetThumbnailCompression( aHd.iThumbCompression ) == KErrNone;
        
    // Getting metering mode
    aHd.iStoreMeteringMode = reader->GetMeteringMode( aHd.iMeteringMode ) ==KErrNone;
    
    // Getting related soundfile
    aHd.iRelatedSoundFile = ReadExifTagL( *reader, EIfdExif, KIdRelatedSoundFile );
        
    // Getting focal plane resolution unit
    if ( reader->TagExists(KIdFocalPlaneResolutionUnit, EIfdExif) )
        {
        const CExifTag* tag = reader->GetTagL(
        		EIfdExif, KIdFocalPlaneResolutionUnit );
        TPtrC8 tagData = tag->Data();
        
        aHd.iFocalPlaneResolutionUnit = MdsUtils::ToUInt16L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        aHd.iStoreFocalPlaneResolutionUnit = ETrue;
        }
    
    // Getting focal plane X resolution
    ReadFocalXPlaneResolutionL( aHd, reader );

        
    // Getting focal plane Y resolution
    ReadFocalYPlaneResolutionL( aHd, reader );

    // Get GPS tags
    TBool latitudeExists = EFalse;
    
    // latitude
    ReadGPSLatitudeL( aHd, reader, latitudeExists );

    // longitude
    ReadGPSLongitudeL( aHd, reader, latitudeExists );
    
    // altitude
    ReadGPSAltitudeL( aHd, reader );
        
    CleanupStack::PopAndDestroy( reader );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Time converting
// ---------------------------------------------------------------------------
//
EXPORT_C TTime CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL(
		const TDesC8& aDateTime )
    {
    WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL()" );
    OstTrace0( TRACE_NORMAL, CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL" );    
    
    TDateTime datetime( 0, EJanuary, 0, 0, 0, 0, 0 );
    TBuf<4> text;
    
    // Year
    TPtrC8 textPart = aDateTime.Left(4);
    TLex8 lex( textPart );
    TInt number = 0;
    TInt error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get year" );
        OstTrace0( TRACE_NORMAL, DUP1_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get year" );
        
        User::Leave( error );
        }
    datetime.SetYear( number );
    
    // Month
    TPtrC8 textPart2 = aDateTime.Mid( 5,2 );
    lex.Assign( textPart2 );
    error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get month" );
        OstTrace0( TRACE_NORMAL, DUP2_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get month" );
        
        User::Leave( error );
        }        
    number--;
    TMonth month = (TMonth) number;
    datetime.SetMonth( month );
    
    // Day
    TPtrC8 textPart3 = aDateTime.Mid( 8,2 );
    lex.Assign( textPart3 );
    error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get date" );
        OstTrace0( TRACE_NORMAL, DUP3_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get date" );
        
        User::Leave( error );
        }
    datetime.SetDay( number - 1 );
    
    // Hours
    TPtrC8 textPart4 = aDateTime.Mid( 11,2 );
    lex.Assign( textPart4 );
    error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get hours" );
        OstTrace0( TRACE_NORMAL, DUP4_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get hours" );
        
        User::Leave( error );
        }    
    datetime.SetHour( number );
    
    // Minutes
    TPtrC8 textPart5 = aDateTime.Mid( 14,2 );
    lex.Assign( textPart5 );
    error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get minutes" );
        OstTrace0( TRACE_NORMAL, DUP5_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get minutes" );
        
        User::Leave( error );
        }
    datetime.SetMinute( number );
    
    // Seconds
    TPtrC8 textPart6 = aDateTime.Mid( 17,2 );
    lex.Assign( textPart6 );
    error = lex.Val( number );
    if ( error != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL() - couldn't get seconds" );
        OstTrace0( TRACE_NORMAL, DUP6_CHARVESTEREXIFUTIL_CONVERTEXIFDATETIMETOSYMBIANTIMEL, "CHarvesterExifUtil::ConvertExifDateTimeToSymbianTimeL - couldn't get seconds" );
        
        User::Leave( error );
        }
    datetime.SetSecond( number );
    
    TTime time( datetime );

    return time;
    }

void CHarvesterExifUtil::AddPropertyL( CMdEObjectDef& aObjectDef, CMdEObject& aMdeObject,
		const TDesC& aProperty, TUint16 aValue )
	{
	CMdEPropertyDef& propDef = aObjectDef.GetPropertyDefL( aProperty );    
	CMdEProperty* mdeProp = NULL;
	
	aMdeObject.Property( propDef, mdeProp, 0 );
    if ( !mdeProp )
        {
        aMdeObject.AddUint16PropertyL( propDef, aValue );
        }	
	}

void CHarvesterExifUtil::AddPropertyL( CMdEObjectDef& aObjectDef, CMdEObject& aMdeObject,
		   const TDesC& aProperty, TUint32 aValue )
{
	CMdEPropertyDef& propDef = aObjectDef.GetPropertyDefL( aProperty );    
	CMdEProperty* mdeProp = NULL;
	
	aMdeObject.Property( propDef, mdeProp, 0 );
	if ( !mdeProp )
		{
		aMdeObject.AddUint32PropertyL( propDef, aValue );
		}	
}

void CHarvesterExifUtil::SetExifDefaultsL( CMdEObject& aMdeObject, CExifModify& aExifModify )
	{
    const TUint32 KPixPerResolution = 72;
    const TUint32 KPixPerResDenm = 1;
    const TUint16 KResUnitInch = 2;
    const TUint16 KYCbCrPositioning = 1;
    const TUint8  KCompConf1st = 1;
    const TUint8  KCompConf2nd = 2;
    const TUint8  KCompConf3rd = 3;
    const TUint8  KCompConf4rd = 0;
    const TUint16 KColorSpaceRGB = 1;

    CMdEObjectDef& imageDef = iDefaultNamespace->GetObjectDefL( Image::KImageObject );
    
    aExifModify.SetXResolutionL( KPixPerResolution, KPixPerResDenm );
    aExifModify.SetYResolutionL( KPixPerResolution, KPixPerResDenm );

    AddPropertyL( imageDef, aMdeObject, MediaObject::KResolutionUnitProperty,
    		      KResUnitInch );
    aExifModify.SetResolutionUnitL( KResUnitInch );

    AddPropertyL( imageDef, aMdeObject, Image::KYCbCrPositioningProperty,
    		      KYCbCrPositioning );
    aExifModify.SetYCbCrPositioningL( KYCbCrPositioning );

    TUint32 compUint32( 0 );
    TUint8* components = (TUint8*) &compUint32;
    *(components + 3) = KCompConf4rd;
    *(components + 2) = KCompConf3rd;
    *(components + 1) = KCompConf2nd;
          *components = KCompConf1st;
    
    AddPropertyL( imageDef, aMdeObject, Image::KComponentsConfigurationProperty,
    		      compUint32 );
    aExifModify.SetComponentsConfigurationL( KCompConf1st, KCompConf2nd,
    										 KCompConf3rd, KCompConf4rd );

    AddPropertyL( imageDef, aMdeObject, Image::KColourSpaceProperty, KColorSpaceRGB );
    aExifModify.SetColorSpaceL( KColorSpaceRGB );
	}

HBufC8* CHarvesterExifUtil::GetPropertyValueLC( const CMdEPropertyDef& aPropDef,
		const CMdEProperty& aProperty )
	{
	switch( aPropDef.PropertyType() )
		{
		case EPropertyReal32:
			{
			TUint32 denominator = 1; 
			if( aPropDef.Name().CompareF( Image::KExposureTimeProperty ) == 0 )
				{
				denominator = 1000000;
				}
			else if( aPropDef.Name().CompareF( Image::KApertureValueProperty ) == 0 )
				{
				denominator = 100;
				}
			else if( aPropDef.Name().CompareF( Image::KExposureBiasValueProperty ) == 0 )
				{
				denominator = 100;
				}
			else if( aPropDef.Name().CompareF( Image::KShutterSpeedValueProperty ) == 0 )
				{
				denominator = 1000;
				}
			else if( aPropDef.Name().CompareF( Image::KFNumberProperty ) == 0 )
				{
				denominator = 10;
				}			
			TUint32 value = TUint32( aProperty.Real32ValueL() * (TReal32)denominator );

			HBufC8* buf8 = HBufC8::NewLC( 2 * sizeof(TUint32) );
			TPtr8 ptr = buf8->Des();
			ptr.Append( (TUint8*)&value, sizeof(TUint32) );
			ptr.Append( (TUint8*)&denominator, sizeof(TUint32) );
			
			return buf8;
			}
		case EPropertyTime:
			{
	        TTime time = aProperty.TimeValueL();
	        if( aPropDef.Name().CompareF( Image::KDateTimeProperty ) == 0 )
	        	{
	        	RTz timezone;
	        	CleanupClosePushL( timezone );
	        	User::LeaveIfError( timezone.Connect() );
	        	timezone.ConvertToLocalTime( time );
	        	CleanupStack::PopAndDestroy( &timezone );
	        	}
	        HBufC* buf = HBufC::NewLC( KDateBufferSize );
	        TPtr ptr = buf->Des();
	        time.FormatL( ptr, KExifDateTimeFormat );
	        HBufC8* buf8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( ptr );
	        CleanupStack::PopAndDestroy( buf );
	        CleanupStack::PushL( buf8 );
	        return buf8;
			}
		case EPropertyText:
			{
			TPtrC text = aProperty.TextValueL();
			if( aPropDef.Name().CompareF( MediaObject::KCommentProperty ) == 0 )
				{
		        const TUint16 bufLength = KUnicodeCodeDesignation.iTypeLength + text.Size();
		        HBufC8* commentBuf = HBufC8::NewLC( bufLength );
		        TPtr8 commentPtr = commentBuf->Des();
		        commentPtr.Append( KUnicodeCodeDesignation );
		        commentPtr.Append( (TUint8*)text.Ptr(), text.Size() );
		        return commentBuf;
				}
			HBufC8* buf = CnvUtfConverter::ConvertFromUnicodeToUtf8L( text );
			CleanupStack::PushL( buf );
	        return buf;
			}
		case EPropertyUint16:
			{
        	TUint16 value = aProperty.Uint16ValueL();
        	HBufC8* buf = HBufC8::NewLC( sizeof(value) );
        	TPtr8 ptr = buf->Des();
        	ptr.Copy( (TUint8*)(&value), sizeof(value) );
			return buf;
			}
		default:
			User::Leave( KErrGeneral );
		}
	    return NULL;
	}

CExifTag::TExifTagDataType CHarvesterExifUtil::ExifTagDataType( TUint16 aTagID, const CMdEPropertyDef& aPropDef )
	{
	if( aTagID == KIdUserComment )
		{
		return CExifTag::ETagUndefined;   		
		}
	if( aTagID == KIdShutterSpeed || aTagID == KIdExposureBias )
		{
		return CExifTag::ETagSrational;
		}
	switch( aPropDef.PropertyType() )
		{
		case EPropertyBool:
		case EPropertyInt8:
		case EPropertyUint8:
			return CExifTag::ETagByte;
		case EPropertyInt16:
		case EPropertyUint16:
			return CExifTag::ETagShort;
		case EPropertyUint32:
			return CExifTag::ETagLong;
		case EPropertyInt32:
			return CExifTag::ETagSlong;
		case EPropertyReal32:
			return CExifTag::ETagRational;
		case EPropertyTime:
			return CExifTag::ETagAscii;
		case EPropertyText:
			return CExifTag::ETagAscii;
		default:
			return CExifTag::ETagUndefined;
		}
	}

TBool CHarvesterExifUtil::CompareTag( TPtrC8 aMdeData, const CExifTag* aTag )
	{
	if( aTag->TagInfo().iDataType == CExifTag::ETagRational )
		{
		TUint32 denominator;
		TUint32 value;
        TPtrC8 ptr( aTag->Data() );
        memcpy( &value, ptr.Ptr(), sizeof(value) );
        memcpy( &denominator, ptr.Ptr()+sizeof(value), sizeof(denominator) );

    	TReal32 tagValue = 0.0f;
    	if ( denominator != 0 )
    		{
    		tagValue = (TReal32)value / (TReal32)denominator;
    		}
        ptr.Set( aMdeData );
        memcpy( &value, ptr.Ptr(), sizeof(value) );
        memcpy( &denominator, ptr.Ptr()+sizeof(value), sizeof(denominator) );

    	TReal32 mdeValue = 0.0f;
    	if ( denominator != 0 )
    		{
    		mdeValue = (TReal32)value / (TReal32)denominator;
    		}
    	return Abs( tagValue - mdeValue ) > 0.01f;
		}
	else
		{
		return aMdeData.CompareF( aTag->Data() ) != 0; 
		}
	}

TBool CHarvesterExifUtil::ModifyExifTagL( CMdEObject& aMdeObject, CExifModify& aExifModify,
		const TDesC& aProperty, TExifIfdType aIFD, TUint16 aTagID, TBool aRemove )
	{
	CMdEObjectDef& imageDef = iDefaultNamespace->GetObjectDefL( Image::KImageObject );
	CMdEPropertyDef& propDef = imageDef.GetPropertyDefL( aProperty );    
	CMdEProperty* mdeProp = NULL;
	TBool exifChanged = EFalse;
	
	aMdeObject.Property( propDef, mdeProp, 0 );
	TBool tagExists = EFalse;
	if( aRemove )
		{
		tagExists = aExifModify.Reader()->TagExists( aTagID, aIFD );
		}
	if ( mdeProp )
        {
        HBufC8* mdedata = GetPropertyValueLC( propDef, *mdeProp );
        TPtrC8 ptr = mdedata->Des();
        TBool change = EFalse;
        const CExifTag* tag = NULL;
        TRAP_IGNORE( tag = aExifModify.Reader()->GetTagL( aIFD, aTagID ) );
        if( !tag ) //create new exif tag
        	{
        	change = ETrue;
        	}
        else if( CompareTag( ptr, tag ) )
            {
            change = ETrue;
            }
        if( change )
        	{
        	CExifTag::TExifTagDataType type = ExifTagDataType( aTagID, propDef );
        	TInt len = 1;
        	if( type == CExifTag::ETagUndefined || type == CExifTag::ETagAscii  )
        		{
        		len = ptr.Length();
        		}
        	TExifTagInfo info( aTagID, type, len  );
            aExifModify.SetTagL( aIFD, info, ptr );
            exifChanged = ETrue;        	
        	}
        CleanupStack::PopAndDestroy( mdedata );
        }
	else if( tagExists ) // remove from exif
        {
        aExifModify.DeleteTag( aIFD, aTagID );
        exifChanged = ETrue;
        }
    return exifChanged;
	}

// ---------------------------------------------------------------------------
// ComposeExifData
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CHarvesterExifUtil::ComposeExifDataL( CMdEObject& aMdeObject, TPtr8 aImagePtr, HBufC8*& aModified )
	{

	if ( !iSession )
		{
		User::Leave( KErrSessionClosed );
		}
	
    WRITELOG1( "CHarvesterExifUtil::ComposeExifData() - Compose Start Object ID: %d", aMdeObject.Id() );
    TBool exifChanged = EFalse;

    // 2. try to init EXIF data from image file's data
    CExifModify* modifyExif = NULL;
    TInt exifError = KErrNone;
    TRAP( exifError, modifyExif = CExifModify::NewL( aImagePtr, 
    		CExifModify::EModify, CExifModify::ENoJpegParsing ) );

    // 3. Is this image format supported?
    if ( exifError == KErrNotSupported )
        {
        WRITELOG( "CHarvesterExifUtil::ComposeExifData() - Image format not supported (!jpeg)" );
        User::Leave( exifError );
        }

    // 4. if image does not contain EXIF data try to create it
    if ( exifError != KErrNone )
        {
        WRITELOG( "CHarvesterExifUtil::ComposeExifData() - Image doesn't contain EXIF data" );
        modifyExif = CExifModify::NewL( aImagePtr, 
        		CExifModify::ECreate, CExifModify::ENoJpegParsing );
        CleanupStack::PushL( modifyExif );
        SetExifDefaultsL( aMdeObject, *modifyExif );
        CleanupStack::Pop( modifyExif );
        exifChanged = ETrue;
        }
    CleanupStack::PushL( modifyExif );
    const CExifRead* readExif = modifyExif->Reader();
    CMdEObjectDef& imageDef = iDefaultNamespace->GetObjectDefL( Image::KImageObject );
    
    // Set pixel X dimension tag (mandatory)
    TBool changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KWidthProperty, EIfdExif, KIdPixelXDimension);
    exifChanged = (exifChanged | changed);

    // Set pixel Y dimension tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KHeightProperty, EIfdExif, KIdPixelYDimension);
    exifChanged = (exifChanged | changed);

    // Set white balance tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KWhiteBalanceProperty, EIfdExif, KIdWhiteBalance, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set flash tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFlashProperty, EIfdExif, KIdFlash, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set exposure program tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KExposureProgramProperty, EIfdExif, KIdExposureProgram, ETrue );
    exifChanged = (exifChanged | changed);

    // Set description tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KDescriptionProperty, EIfd0, KIdImageDescription, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set user comment tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KCommentProperty, EIfdExif, KIdUserComment, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set copyright tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KCopyrightProperty, EIfd0, KIdCopyright, ETrue );
    exifChanged = (exifChanged | changed);

    // Set DateTimeOriginal tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KDateTimeOriginalProperty, EIfdExif, KIdDateTimeOriginal, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set DateTimeDigitized tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KDateTimeDigitizedProperty, EIfdExif, KIdDateTimeDigitized, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set DateTime (_image_ modified) tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KDateTimeProperty, EIfd0, KIdDateTimeModified, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set maker tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KMakeProperty, EIfd1, KIdMake, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set model tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KModelProperty, EIfd1, KIdModel, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set orientation tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KOrientationProperty, EIfd0, KIdOrientation , ETrue );
    exifChanged = (exifChanged | changed);

    // Set YCbCrPositioning tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KYCbCrPositioningProperty, EIfd1, KIdYbCrPositioning, EFalse );
    exifChanged = (exifChanged | changed);
    
    // Set resolution unit tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		MediaObject::KResolutionUnitProperty, EIfd1, KIdResolutionUnit, EFalse );
    exifChanged = (exifChanged | changed);
    
    // Set ISO speed tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KISOSpeedRatingsProperty, EIfdExif, KIdISOSpeedRatings, ETrue );
    exifChanged = (exifChanged | changed);

    // Set related soundfile tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KRelatedSoundFileProperty, EIfdExif, KIdRelatedSoundFile, ETrue );
    exifChanged = (exifChanged | changed);    
    
    // Set exposure time tag (recommended)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KExposureTimeProperty, EIfdExif, KIdExposureTime, ETrue );
    exifChanged = (exifChanged | changed);   
        
    // Set aperture value tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KApertureValueProperty, EIfdExif, KIdApertureValue, ETrue );
    exifChanged = (exifChanged | changed);  
    
    // Set colour space tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
		Image::KColourSpaceProperty, EIfdExif, KIdColorSpace , EFalse );
    exifChanged = (exifChanged | changed);
        
    // Set exposure bias tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KExposureBiasValueProperty, EIfdExif, KIdExposureBias, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set metering mode tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KMeteringModeProperty, EIfdExif, KIdMeteringMode, ETrue );
    exifChanged = (exifChanged | changed);

    // Set shutter speed tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KShutterSpeedValueProperty, EIfdExif, KIdShutterSpeed, ETrue );
    exifChanged = (exifChanged | changed);

    // Set X resolution tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KXResolutionProperty, EIfd0, KIdXResolution, EFalse );
    exifChanged = (exifChanged | changed); 

    // Set Y resolution tag (mandatory)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KYResolutionProperty, EIfd0, KIdYResolution, EFalse );
    exifChanged = (exifChanged | changed); 
    
    // Set F number tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFNumberProperty, EIfdExif, KIdFNumber, ETrue );
    exifChanged = (exifChanged | changed);     
    
    // Set focal length tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFocalLengthProperty, EIfdExif, KIdFocalLength, ETrue );
    exifChanged = (exifChanged | changed);  
    
    // Set focal length in 35 mm film tag (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFocalLengthIn35mmFilmProperty, EIfdExif, KIdFocalLengthIn35mm, ETrue );
    exifChanged = (exifChanged | changed);   

    // Set focal plane resolution unit (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFocalPlaneResolutionUnitProperty, EIfdExif, KIdFocalPlaneResolutionUnit, ETrue );
    exifChanged = (exifChanged | changed);   

    // Set focal plane X resolution (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFocalPlaneXResolutionProperty, EIfdExif, KIdFocalPlaneXResolution, ETrue );
    exifChanged = (exifChanged | changed);
    
    // Set focal plane Y resolution (optional)
    changed = ModifyExifTagL( aMdeObject, *modifyExif,
    		Image::KFocalPlaneYResolutionProperty, EIfdExif, KIdFocalPlaneYResolution, ETrue );
    exifChanged = (exifChanged | changed);
    
    TUint16 uint16Value( 0 );
    TUint32 uint32Value( 0 );
    
    // Set components configuration tag (mandatory)
    const CMdEPropertyDef& componentsDef = imageDef.GetPropertyDefL(
     		Image::KComponentsConfigurationProperty );
        {
        CMdEProperty* componentsProp = NULL;
        aMdeObject.Property( componentsDef, componentsProp, 0 );

        if ( componentsProp )
            {
            TUint32 componentsValue = componentsProp->Uint32ValueL();
            TUint8* components = (TUint8*) &componentsValue;
            const TUint8 KComponent4th = *(components + 3);
            const TUint8 KComponent3rd = *(components + 2);
            const TUint8 KComponent2nd = *(components + 1);
            const TUint8 KComponent1st = *components;
            
            TUint8 exifComponent4th( 0 );
            TUint8 exifComponent3rd( 0 );
            TUint8 exifComponent2nd( 0 );
            TUint8 exifComponent1st( 0 );
            
            exifError = readExif->GetComponentsConfiguration(
              	exifComponent1st, exifComponent2nd, exifComponent3rd, exifComponent4th );
            if ( exifError != KErrNone || 
                 exifComponent1st != KComponent1st || exifComponent2nd != KComponent2nd ||
                 exifComponent3rd != KComponent3rd || exifComponent4th != KComponent4th )
                {
                modifyExif->SetComponentsConfigurationL(
                		KComponent1st, KComponent2nd, KComponent3rd, KComponent4th );
                exifChanged = ETrue;
                }
            }
        }        
        
	// Set thumbnail X resolution tag (mandatory)
    const CMdEPropertyDef& thumbXDef = imageDef.GetPropertyDefL(
        Image::KThumbXResolutionProperty );
        {
        CMdEProperty* thumbXProp = NULL;
        aMdeObject.Property( thumbXDef, thumbXProp, 0 );

        if ( thumbXProp )
            {
            const TUint32 thumbX = thumbXProp->Uint32ValueL();

            TUint32 exifDenominator = 0;
            exifError = readExif->GetThumbnailXResolution( uint32Value, exifDenominator );
            TUint32 exifThumbXResol = 0;
            if ( exifDenominator > 0 )
                {
                exifThumbXResol = uint32Value / exifDenominator;
                }

            if ( exifError != KErrNone || exifThumbXResol != thumbX )
                {
                const TUint32 KDenominator = 1;
                modifyExif->SetThumbnailXResolutionL( thumbX, KDenominator );
                exifChanged = ETrue;
                }
            }
        }

	// Set thumbnail Y resolution tag (mandatory)
    const CMdEPropertyDef& thumbYDef = imageDef.GetPropertyDefL(
      		Image::KThumbYResolutionProperty );
        {
        CMdEProperty* thumbYProp = NULL;
        aMdeObject.Property( thumbYDef, thumbYProp, 0 );

        if ( thumbYProp )
            {
            TUint32 thumbY = TUint32( thumbYProp->Uint32ValueL() );
            TUint32 exifDenominator = 0;
            exifError = readExif->GetThumbnailYResolution( uint32Value, exifDenominator );
            TUint32 exifThumbYResol = 0;
            if ( exifDenominator > 0 )
                {
                exifThumbYResol = uint32Value / exifDenominator;
                }
            
            if ( exifError != KErrNone || exifThumbYResol != thumbY )
                {
                const TUint32 KDenominator = 1;
                modifyExif->SetThumbnailYResolutionL( thumbY, KDenominator );
                exifChanged = ETrue;
                }
            }
        }

	// Set thumbnail resolution unit tag (mandatory)
    const CMdEPropertyDef& thumbResolutionUnitDef = imageDef.GetPropertyDefL(
        	Image::KThumbResolutionUnitProperty );
        {
        CMdEProperty* thumbResolutionUnitProp = NULL;            
        aMdeObject.Property( thumbResolutionUnitDef, thumbResolutionUnitProp, 0 );

        if ( thumbResolutionUnitProp )
            {
            exifError = readExif->GetThumbnailResolutionUnit( uint16Value );
            const TUint16 thumbnailResolutionUnitValue =
             	thumbResolutionUnitProp->Uint16ValueL();
            if ( exifError != KErrNone || uint16Value != thumbnailResolutionUnitValue )
                {
                modifyExif->SetThumbnailResolutionUnitL( thumbnailResolutionUnitValue );
                exifChanged = ETrue;
                }
            }
        }
        
    if ( exifChanged )
        {
        WRITELOG( "CHarvesterExifUtil::ComposeExifData() - write exif to buffer" );
        aModified = modifyExif->WriteDataL( aImagePtr );
        }
    CleanupStack::PopAndDestroy( modifyExif );

    WRITELOG1( "CHarvesterExifUtil::ComposeExifData() - Compose End Object ID: %d", aMdeObject.Id() );

	return KErrNone;
	}

// ---------------------------------------------------------------------------
// ComposeLocation
// ---------------------------------------------------------------------------
//
EXPORT_C void CHarvesterExifUtil::ComposeLocationL( CMdEObject* aLocation, TPtr8 aImagePtr, HBufC8*& aModified )
    {
    CMdEProperty* latitudeProperty = NULL;
    CMdEProperty* longitudeProperty = NULL;
    CMdEProperty* altitudeProperty = NULL;
    CMdEProperty* qualityProperty = NULL;
    
    CMdEObjectDef& locationDef = iDefaultNamespace->GetObjectDefL( Location::KLocationObject );

    aLocation->Property( locationDef.GetPropertyDefL(
    		Location::KLatitudeProperty ), latitudeProperty, 0 );
    aLocation->Property( locationDef.GetPropertyDefL(
    		Location::KLongitudeProperty ), longitudeProperty, 0 );
    aLocation->Property( locationDef.GetPropertyDefL(
    		Location::KAltitudeProperty ), altitudeProperty, 0 );
    aLocation->Property( locationDef.GetPropertyDefL(
    		Location::KQualityProperty ), qualityProperty, 0 );

    CExifModify* exifWriter = CExifModify::NewL( aImagePtr, 
    		CExifModify::EModify, CExifModify::ENoJpegParsing );
    CleanupStack::PushL( exifWriter );
    const CExifRead* exifReader = exifWriter->Reader();

    TBool exifChanged = EFalse;
    TBool changed = EFalse;
    const TReal KAngleSecond = 1.0 / 3600.0;
    TInt exifError( 0 );
    // location data (all fields are optional)
    // latitude
    const CExifTag* exifLatitudeRefTag = NULL;
    TRAP( exifError, exifLatitudeRefTag = exifReader->GetTagL( EIfdGps, KIdGpsLatitudeRef ) );
    if ( latitudeProperty )
        {
        TBuf8<2> south( KSouth );
        TBuf8<2> north( KNorth );
        TReal64 latitude = latitudeProperty->Real64ValueL();

        TReal64 exifLatitude( 0.0 );
        TBuf8<2> exifLatitudeRef;
        if ( exifError == KErrNone )
            {
            TPtrC8 exifLatitudeRefBuf( exifLatitudeRefTag->Data() );
            exifLatitudeRef.Append( exifLatitudeRefBuf.Ptr(), 2 );

            if ( latitude < 0 )
                {
                if ( south.Compare(exifLatitudeRef) )
                    {
                    changed = ETrue;
                    }
                }
            else
                {
                if ( north.Compare(exifLatitudeRef) )
                    {
                    changed = ETrue;
                    }
                }

            if ( !changed )
                {
                TBuf8<KCoordinateBufferSize> exifLatitudeBuf;
                const CExifTag* exifLatitudeTag = NULL;
                TRAPD( err, exifLatitudeTag = exifReader->GetTagL( EIfdGps, KIdGpsLatitude ) );
                if ( err == KErrNone )
                    {
                    TPtrC8 exifLatitudeTagBuf( exifLatitudeTag->Data() );
                    exifLatitudeBuf.Append( exifLatitudeTagBuf.Ptr(), KCoordinateBufferSize );
                    MdsUtils::ConvertFromDegreesToDecimalL( exifLatitudeBuf, exifLatitude );
                    }
                else
                    {
                    exifError = err;
                    }
                }
            }

        if ( exifError != KErrNone || changed || Abs(exifLatitude - latitude) > KAngleSecond )
            {
            // latitude ref (N/S)
            if ( latitude < 0 )
                {
                exifWriter->SetTagL( EIfdGps, TExifTagInfo(
                		KIdGpsLatitudeRef, CExifTag::ETagAscii, 2 ), south );
                latitude = -latitude;
                }
            else
                {
                exifWriter->SetTagL( EIfdGps, TExifTagInfo(
                		KIdGpsLatitudeRef, CExifTag::ETagAscii, 2 ), north );
                }

            TBuf8<KCoordinateBufferSize> latitudeBuf;
            MdsUtils::ConvertFromDecimalToDegreesL( latitude, latitudeBuf );
            exifWriter->SetTagL( EIfdGps, TExifTagInfo(
            		KIdGpsLatitude, CExifTag::ETagRational, 3 ), latitudeBuf );
            exifChanged = ETrue;
            }
        }
    else if ( exifError == KErrNone )
        {
        exifWriter->DeleteTag( EIfdGps, KIdGpsLatitudeRef );
        exifWriter->DeleteTag( EIfdGps, KIdGpsLatitude );
        exifChanged = ETrue;
        }

    changed = EFalse;
    
    // longitude
    const CExifTag* exifLongitudeRefTag = NULL;
    TRAP( exifError, exifLongitudeRefTag = exifReader->GetTagL(
    		EIfdGps, KIdGpsLongitudeRef ) );
    if ( longitudeProperty )
        {
        TBuf8<2> west( KWest );
        TBuf8<2> east( KEast );        
        TReal64 longitude = longitudeProperty->Real64ValueL();
        
        TReal64 exifLongitude( 0.0 );
        TBuf8<2> exifLongitudeRef;
        if ( exifError == KErrNone )
            {
            TPtrC8 exifLongitudeRefBuf( exifLongitudeRefTag->Data() );
            exifLongitudeRef.Append( exifLongitudeRefBuf.Ptr(), 2 );

            if ( longitude < 0 )
                {
                if ( west.Compare(exifLongitudeRef) )
                    {
                    changed = ETrue;
                    }
                }
            else
                {
                if ( east.Compare(exifLongitudeRef) )
                    {
                    changed = ETrue;
                    }
                }

            if ( !changed )
                {
                TBuf8<KCoordinateBufferSize> exifLongitudeBuf;
                const CExifTag* exifLongitudeTag = NULL;
                TRAPD( err, exifLongitudeTag = exifReader->GetTagL( EIfdGps, KIdGpsLongitude ) );
                if ( err == KErrNone )
                    {
                    TPtrC8 exifLongitudeTagBuf( exifLongitudeTag->Data() );
                    exifLongitudeBuf.Append( exifLongitudeTagBuf.Ptr(), KCoordinateBufferSize );
                    MdsUtils::ConvertFromDegreesToDecimalL( exifLongitudeBuf, exifLongitude );
                    if ( exifLongitudeRef.Compare( KWest ) == 0 )
                        {
                        exifLongitude = -exifLongitude;
                        }
                    }
                else
                    {
                    exifError = err;
                    }
                }
            }

        if ( exifError != KErrNone || changed || Abs(exifLongitude - longitude) > KAngleSecond )
            {
            // longitude ref (E/W)
            if ( longitude < 0 )
                {
                exifWriter->SetTagL( EIfdGps,
                		TExifTagInfo( KIdGpsLongitudeRef, CExifTag::ETagAscii, 2 ), west );
                longitude = -longitude;
                }
            else
                {
                exifWriter->SetTagL( EIfdGps,
                		TExifTagInfo( KIdGpsLongitudeRef, CExifTag::ETagAscii, 2 ), east );
                }
            
            TBuf8<KCoordinateBufferSize> longitudeBuf;
            MdsUtils::ConvertFromDecimalToDegreesL( longitude, longitudeBuf );
            exifWriter->SetTagL( EIfdGps,
            		TExifTagInfo( KIdGpsLongitude, CExifTag::ETagRational, 3 ), longitudeBuf );
            exifChanged = ETrue;
            }
        }
    else if ( exifError == KErrNone )
        {
        exifWriter->DeleteTag( EIfdGps, KIdGpsLongitudeRef );
        exifWriter->DeleteTag( EIfdGps, KIdGpsLongitude  );
        exifChanged = ETrue;
        }

    changed = EFalse;
    
    // altitude
    const CExifTag* exifAltitudeRefTag = NULL;
    TRAP( exifError, exifAltitudeRefTag = exifReader->GetTagL( EIfdGps, KIdGpsAltitudeRef ) );
    if ( altitudeProperty )
        {
        TReal64 altitude = altitudeProperty->Real64ValueL();

        TBuf8<8> altitudeBuf;
        const TInt32 KAltDenominator = 100;
        altitude *= KAltDenominator;

        TUint8 altitudeRef = 0;
        TBuf8<1> altRefBuf;
        
        if ( altitude < 0 )
            {
            altitudeRef = 1;
            altitude = -altitude;
            }        	
        
        altRefBuf.Append( &altitudeRef, 1 );

        TInt32 exifAltitudeValue( 0 );
        TInt32 exifAltDenominator( 0 );
        TReal64 exifAltitude = 0.0f;
        TBuf8<1> exifAltitudeRef;
        if ( exifError == KErrNone )
            {
            TPtrC8 exifAltitudeRefBuf( exifAltitudeRefTag->Data() );
            exifAltitudeRef.Append( exifAltitudeRefBuf.Ptr(), 1 );
            const CExifTag* exifAltitudeTag = NULL;
            TRAPD( err, exifAltitudeTag = exifReader->GetTagL( EIfdGps, KIdGpsAltitude ) );
            if ( err == KErrNone )
                {
                TPtrC8 exifAltitudeTagBuf( exifAltitudeTag->Data() );
                memcpy( &exifAltitudeValue, exifAltitudeTagBuf.Ptr(),
                		sizeof(exifAltitudeValue) );
                memcpy( &exifAltDenominator,
                		exifAltitudeTagBuf.Ptr()+sizeof(exifAltitudeValue),
                		sizeof(exifAltDenominator) );
                }
            else
                {
                exifError = err;
                }
            exifAltitude = (TReal64)exifAltitudeValue;
            }

        if ( exifError != KErrNone || exifAltitudeRef.Compare(altRefBuf) ||
        		Abs(altitude - exifAltitude) > KAngleSecond )
            {
            exifWriter->SetTagL( EIfdGps, TExifTagInfo(
            		KIdGpsAltitudeRef, CExifTag::ETagByte, 1 ), altRefBuf );

            TInt32 tmpAlt = (TInt32) altitude;
            altitudeBuf.Append( (TUint8*) &tmpAlt, 4 );
            altitudeBuf.Append( (TUint8*) &KAltDenominator, 4 );
            exifWriter->SetTagL( EIfdGps, TExifTagInfo(
            		KIdGpsAltitude, CExifTag::ETagRational, 1 ), altitudeBuf );
            exifChanged = ETrue;

            changed = EFalse;            
            }
        // measure mode
        const CExifTag* exifMeasureModeTag = NULL;
        TRAPD( err, exifMeasureModeTag = exifReader->GetTagL( EIfdGps, KIdGpsMeasureMode ) );
        if ( err == KErrNone )
        	{
        	
        	TBuf8<2> exifMeasureMode;
            TPtrC8 exifMeasureModeBuf( exifMeasureModeTag->Data() );
            exifMeasureMode.Append( exifMeasureModeBuf.Ptr(), 2 );
        	
            if (altitude == 0)
            	{
            	if (exifMeasureMode.Compare(KMeasureMode3))
            		{
            		changed = ETrue;
            		}
            	}
            else
            	{
            	if (exifMeasureMode.Compare(KMeasureMode2))
            		{
            		changed = ETrue;
            		}
            	}
        	}
        else
            {
            exifError = err;
            }
        
        if (err != KErrNone || changed)
        	{
        	if (altitude == 0)
            	{
            	exifWriter->SetTagL( EIfdGps, TExifTagInfo(
            		KIdGpsMeasureMode, CExifTag::ETagAscii, 2 ), KMeasureMode2 );
            	}
        	else
        		{
        		exifWriter->SetTagL( EIfdGps, TExifTagInfo(
                		KIdGpsMeasureMode, CExifTag::ETagAscii, 2 ), KMeasureMode3 );
        		}
        	}
        }
    else if ( exifError == KErrNone )
        {
        exifWriter->DeleteTag( EIfdGps, KIdGpsAltitudeRef  );
        exifWriter->DeleteTag( EIfdGps, KIdGpsAltitude  );
        exifWriter->DeleteTag( EIfdGps, KIdGpsMeasureMode  );
        exifChanged = ETrue;
        } 
    
    changed = EFalse;
    
    // quality, DOP value
    const CExifTag* exifQualityTag = NULL;
    TRAPD( err, exifQualityTag = exifReader->GetTagL( EIfdGps, KIdGpsDop ) );
    if (qualityProperty)
    	{
        CMdEReal32Property* qualityReal = static_cast<CMdEReal32Property*>(qualityProperty);
        TReal32 quality = qualityReal->Value();
        const TInt32 KQualityDenominator = 10;
        TBuf8<8> qualityBuf;
        TInt32 exifQualityValue( 0 );
        TInt32 exifQualityDenominator( 0 );  
        TReal32 exifQuality (0.0f);
        quality = quality * KQualityDenominator;
        
	    if ( err == KErrNone )
	        {
                TPtrC8 exifQualityTagBuf( exifQualityTag->Data() );
                memcpy( &exifQualityValue, exifQualityTagBuf.Ptr(),
                		sizeof(exifQualityValue) );
                memcpy( &exifQualityDenominator,
                		exifQualityTagBuf.Ptr()+sizeof(exifQualityValue),
                		sizeof(exifQualityDenominator) );
	        }
        else
            {
            exifError = err;
            }
	    
	    if (exifQualityDenominator > 0)
	    	{
	    	exifQuality = (TReal32)exifQualityValue;
	    	}
	    
	    if (exifError != KErrNone || Abs(quality - exifQuality) > 0.1f)
	    	{
	    	
	    	TInt32 tmpQuality = (TInt32) quality;
	    	qualityBuf.Append( (TUint8*) &tmpQuality, 4 );
	    	qualityBuf.Append( (TUint8*) &KQualityDenominator, 4 );
            exifWriter->SetTagL( EIfdGps, TExifTagInfo(
            		KIdGpsDop, CExifTag::ETagRational, 1 ), qualityBuf );
            exifChanged = ETrue;	    	
	    	}	    
    	}
    
    const CExifTag* exifDatumTag = NULL;
    TRAP( err, exifDatumTag = exifReader->GetTagL( EIfdGps, KIdGpsMapDatum ) );
    TBuf8<7> mapdatum( KMapDatum );

    
    if ( err == KErrNone )
        {        
        if (exifDatumTag->Data().Compare(mapdatum))
    		{
    		changed = ETrue;
    		}        	
        }
    else
        {
        exifError = err;
        }
    
    if (exifError != KErrNone || changed)
    	{        		
		exifWriter->SetTagL( EIfdGps, TExifTagInfo(
			KIdGpsMapDatum, CExifTag::ETagAscii, 7 ), mapdatum );
    	}
    
    // write the EXIF data to the image
    if ( exifChanged )
        {
        aModified = exifWriter->WriteDataL( aImagePtr );
        }
    CleanupStack::PopAndDestroy( exifWriter );
    }

void CHarvesterExifUtil::ReadUserCommentL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting UserComment
    if ( aReader->TagExists(KIdUserComment, EIfdExif ) )
        {
        TUint16 KMaxCommentLength = 256;
        
        HBufC8* comment = aReader->GetUserCommentL();
        CleanupStack::PushL( comment );
        if( comment->Length() >= 8 )
            {
            TBuf8<8> commentFormat = comment->Mid( 0,8 );
            
            TUint16 commentLength = comment->Length();
            
            TPtrC8 userPtr = comment->Mid( 8, commentLength > KMaxCommentLength ?
                    KMaxCommentLength - 8 : commentLength - 8 );
            
            if ( commentFormat.Compare(KUnicodeCodeDesignation) == 0 )
                {
                WRITELOG( "CHarvesterExifUtil::ReadUserCommentL() - comment, Unicode encoding" );
                aHd.iComment16 = HBufC::NewL( userPtr.Length() );
                TPtr ptr = aHd.iComment16->Des();
                TPtrC16 ptr16( (TUint16*)(userPtr.Ptr()), (userPtr.Size()/2) );
                ptr.Copy( ptr16 );
                }
            else if ( commentFormat.Compare(KAsciiCodeDesignation) == 0 ||
                      commentFormat.Compare(KUnknownCodeDesignation) == 0 ||
                      commentFormat.Compare(KJisCodeDesignation) == 0 )
                {
                aHd.iComment16 = CnvUtfConverter::ConvertToUnicodeFromUtf8L( userPtr ); 
                }
            else
                {
                WRITELOG( "CHarvesterExifUtil::ReadUserCommentL() - unknown comment encoding" );    
                }
            if( aHd.iComment16 )
                {
                StripNulls( *(aHd.iComment16) );
                }
            }
        CleanupStack::PopAndDestroy( comment );
        comment = NULL;
        }
    }

void CHarvesterExifUtil::ReadXResolutionL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting X Resolution
    TUint32 numerator = 0;
    TUint32 denominator = 0;
    const TInt error = aReader->GetXResolution( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreXResolution = ETrue;
        aHd.iXResolution = 0.0f;
        if ( denominator > 0 )
            {
            aHd.iXResolution = (TReal32) numerator / (TReal32) denominator;
            }
        }
    }

void CHarvesterExifUtil::ReadYResolutionL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting Y Resolution
    TUint32 numerator = 0;
    TUint32 denominator = 0;
    const TInt error = aReader->GetYResolution( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreYResolution = ETrue;
        aHd.iYResolution = 0.0f;
        if ( denominator > 0 )
            {
            aHd.iYResolution = (TReal32) numerator / (TReal32) denominator;
            }
        }
    }

void CHarvesterExifUtil::ReadExposureBiasL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting exposure bias
    TInt32 num( 0 );
    TInt32 deno( 0 );
    const TInt error = aReader->GetExposureBiasValue( num, deno );
    if ( error == KErrNone )
        {
        aHd.iStoreExposureBias = ETrue;
        aHd.iExposureBias = 0.0f;
        if ( deno != 0 )
            {
            aHd.iExposureBias = (TReal32) num / (TReal32) deno;    
            }
        }
    }

void CHarvesterExifUtil::ReadExposureTimeL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting exposure time
    TUint32 numerator = 0;
    TUint32 denominator = 0;
    const TInt error = aReader->GetExposureTime( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreExposureTime = ETrue;
        aHd.iExposureTime = 0.0f;
        if ( denominator > 0 )
            {
            aHd.iExposureTime = (TReal32) numerator / (TReal32) denominator;
            }
        }
    }

void CHarvesterExifUtil::ReadFNumberL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting FNumber
    if ( aReader->TagExists(KIdFNumber, EIfdExif) )
        {
        const CExifTag* tag = aReader->GetTagL( EIfdExif, KIdFNumber );
        TPtrC8 tagData = tag->Data();
        
        TUint32 numerator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        TUint32 denominator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() + 4 ) );
        if ( denominator == 0 )
            {
            denominator = 1;
            }
        
        aHd.iFNumber = (TReal32) numerator / (TReal32) denominator;
        aHd.iStoreFNumber = ETrue;
        WRITELOG1( "CHarvesterExifUtil::ReadFNumberL() - fnumber %f", aHd.iFNumber );
        }
    }

void CHarvesterExifUtil::ReadShutterSpeedL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting shutter speed value
    TInt32 num( 0 );
    TInt32 deno( 0 );
    const TInt error = aReader->GetShutterSpeedValue( num, deno );
    if ( error == KErrNone )
        {
        aHd.iStoreShutterSpeed = ETrue;
        aHd.iShutterSpeed = 0.0f;
        if ( deno != 0 )
            {
            aHd.iShutterSpeed = (TReal32) num / (TReal32) deno;
            }
        
        WRITELOG1( "CHarvesterExifUtil::ReadExifDataL() - shutter speed %f", aHd.iShutterSpeed );
        }
    }

void CHarvesterExifUtil::ReadApertureValueL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting aHd.iAperture
    TUint32 numerator = 0;
    TUint32 denominator = 0;
    const TInt error = aReader->GetApertureValue( numerator, denominator );
    if ( error == KErrNone )
        {
        aHd.iStoreAperture = ETrue;
        aHd.iAperture = 0.0f;
        if ( denominator > 0 )
            {
            aHd.iAperture = (TReal32) numerator / (TReal32) denominator;
            }
      
        WRITELOG1( "CHarvesterExifUtil::ReadExifDataL() - aHd.iAperture %f", aHd.iAperture );
        }
    }

void CHarvesterExifUtil::ReadFocalLengthL( CHarvestData& aHd, CExifRead* aReader )
    {
    //Getting focal length
    TUint32 numerator = 0;
    TUint32 denominator = 0;
    if ( aReader->TagExists(KIdFocalLength, EIfdExif) )
        {                
        const CExifTag* tag = aReader->GetTagL( EIfdExif, KIdFocalLength );
        TPtrC8 tagData = tag->Data();
        
        numerator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        denominator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() + 4 ) );
        
        if ( denominator == 0 )
            {
            denominator = 1;
            }
        
        aHd.iFocalLength = (TReal32) numerator / (TReal32) denominator;
        aHd.iStoreFocalLength = ETrue;
        
        WRITELOG1( "CHarvesterExifUtil::ReadExifDataL() - focal length %f", aHd.iFocalLength );
        }
    }

void CHarvesterExifUtil::ReadFocalLength35mmL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting focal length in 35 mm
    if ( aReader->TagExists(KIdFocalLengthIn35mm, EIfdExif) )
        {
        const CExifTag* tag = aReader->GetTagL( EIfdExif, KIdFocalLengthIn35mm );
        TPtrC8 tagData = tag->Data();
        
        aHd.iFocalLengthIn35mm = MdsUtils::ToUInt16L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        
        aHd.iStoreFocalLengthIn35 = ETrue;
        
        WRITELOG1( "CHarvesterExifUtil::ReadExifDataL() - focal length in 35 mm: %f", aHd.iFocalLengthIn35mm );
        }
    }

void CHarvesterExifUtil::ReadFocalXPlaneResolutionL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting focal plane X resolution
    if ( aReader->TagExists(KIdFocalPlaneXResolution, EIfdExif) )
        {
        const CExifTag* tag = aReader->GetTagL(
                EIfdExif, KIdFocalPlaneXResolution );
        TPtrC8 tagData = tag->Data();
        
        TUint32 numerator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        TUint32 denominator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() + 4 ) );
        if ( denominator == 0 )
            {
            denominator = 1;
            }
        
        aHd.iFocalPlaneXResolution = (TReal32) numerator / (TReal32) denominator;
        aHd.iStoreFocalPlaneXResolution = ETrue;
        }
    }

void CHarvesterExifUtil::ReadFocalYPlaneResolutionL( CHarvestData& aHd, CExifRead* aReader )
    {
    // Getting focal plane Y resolution
    if ( aReader->TagExists(KIdFocalPlaneYResolution, EIfdExif) )
        {
        const CExifTag* tag = aReader->GetTagL(
                EIfdExif, KIdFocalPlaneYResolution );
        TPtrC8 tagData = tag->Data();
        
        TUint32 numerator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() ) );
        TUint32 denominator = MdsUtils::ToUInt32L( CONST_CAST( TUint8*, tagData.Ptr() + 4 ) );
        if ( denominator == 0 )
            {
            denominator = 1;
            }
        
        aHd.iFocalPlaneYResolution = (TReal32) numerator / (TReal32) denominator;
        aHd.iStoreFocalPlaneYResolution = ETrue;
        }
    }

void CHarvesterExifUtil::ReadGPSLatitudeL( CHarvestData& aHd, 
                                                                       CExifRead* aReader, 
                                                                       TBool& aLatitude )
    {
    const TInt KCoordinateBufferSize = 24;
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLatitudeL() - trying to read GPS Latitude" );
    
    // Getting GPS latitude   
    if ( aReader->TagExists(KIdGpsLatitudeRef, EIfdGps) &&
            aReader->TagExists(KIdGpsLatitude, EIfdGps) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadGPSLatitudeL() - GPS Latitude found" );
        
        TReal64 latitude = 0.0;
        const CExifTag* refTag = aReader->GetTagL(
                EIfdGps, KIdGpsLatitudeRef );
        TBuf8<3> latitudeRef = refTag->Data();
        const CExifTag* latitudeTag = aReader->GetTagL(
                EIfdGps, KIdGpsLatitude );
        TBuf8<KCoordinateBufferSize> latitudeBuf = latitudeTag->Data();
        MdsUtils::ConvertFromDegreesToDecimalL( latitudeBuf, latitude );
        if ( latitudeRef == KSouth )
            {
            latitude = -latitude;
            }
        aHd.iGpsLatitude = latitude;
        aLatitude = ETrue;
        }
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLatitudeL() - trying to read GPS Latitude - end" );
    }

void CHarvesterExifUtil::ReadGPSLongitudeL( CHarvestData& aHd, 
                                                                          CExifRead* aReader, 
                                                                          TBool& aLatitude )
    {
    const TInt KCoordinateBufferSize = 24;
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLongitudeL() - trying to read GPS Longitude" );
    
    // Getting GPS longitude
    if ( aReader->TagExists(KIdGpsLongitudeRef, EIfdGps) &&
            aReader->TagExists(KIdGpsLongitude, EIfdGps) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadGPSLatitudeL() - GPS Longitude found" );
    
        TReal64 longitude = 0.0;
        const CExifTag* refTag = aReader->GetTagL(
                EIfdGps, KIdGpsLongitudeRef );
        TBuf8<3> longitudeRef = refTag->Data();
        const CExifTag* longitudeTag = aReader->GetTagL(
                EIfdGps, KIdGpsLongitude );
        TBuf8<KCoordinateBufferSize> longitudeBuf = longitudeTag->Data();
        MdsUtils::ConvertFromDegreesToDecimalL( longitudeBuf, longitude );
        if ( longitudeRef == KWest )
            {
            longitude = -longitude;
            }
        aHd.iGpsLongitude = longitude;
        if ( aLatitude )
            {
            aHd.iStoreGpsLatitudeAndLongitude = ETrue;
            }
        }
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLongitudeL() - trying to read GPS Longitude - end" );
    }

void CHarvesterExifUtil::ReadGPSAltitudeL( CHarvestData& aHd, CExifRead* aReader )
    {
    const TInt KAltitudeBufferSize = 8;
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLongitudeL() - trying to read GPS Altitude" );
    
    // Getting GPS altitude
    if ( aReader->TagExists(KIdGpsAltitudeRef, EIfdGps) &&
            aReader->TagExists(KIdGpsAltitude, EIfdGps) )
        {
        WRITELOG( "CHarvesterExifUtil::ReadGPSLatitudeL() - GPS Altitude found" );
    
        const CExifTag* refTag = aReader->GetTagL(
                EIfdGps, KIdGpsAltitudeRef );
        TBuf8<2> altitudeRef = refTag->Data();
        const CExifTag* altitudeTag = aReader->GetTagL(
                EIfdGps, KIdGpsAltitude );
        TBuf8<KAltitudeBufferSize> altitudeBuf = altitudeTag->Data();
        TInt32 altitude = MdsUtils::ToUInt32L(
                CONST_CAST( TUint8*, altitudeBuf.Left(4).Ptr() ) );
        TInt32 denominator =  MdsUtils::ToUInt32L(
                CONST_CAST( TUint8*, altitudeBuf.Right(4).Ptr() ) );
        TInt8 ref = *((TUint8*) altitudeRef.Ptr());

        if ( ref == 1 )
            {
            altitude = -altitude;
            }

        aHd.iGpsAltitude = 0.0f;
        if ( denominator != 0 )
            {
            aHd.iGpsAltitude = (TReal64)altitude / (TReal64)denominator;
            }

        aHd.iStoreGpsAltitude = ETrue;
        }
    
    WRITELOG( "CHarvesterExifUtil::ReadGPSLongitudeL() - trying to read GPS Altitude - end" );
    }

// End of file
