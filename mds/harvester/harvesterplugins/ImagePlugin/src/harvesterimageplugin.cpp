/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester image plugin
*
*/


#include <e32base.h>
#include <e32math.h>
#include <locationdatatype.h>
#include <harvesterdata.h>
#include <pathinfo.h>

#include "harvesterlog.h"
#include "mdsutils.h"
#include "harvesterexifutil.h"
#include "harvesterimageplugin.h"
#include "mdeobjectwrapper.h"
#include "mdscommoninternal.h"

using namespace MdeConstants;

// image mime types
_LIT( KJpegMime, "image/jpeg" );
_LIT( KJpeg2000Mime, "image/jp2" );
_LIT( KJpeg2000_2Mime, "image/jpx" );
_LIT( KTiffMime, "image/tiff" );
_LIT( KMsmMime, "application/x-msmetafile" );
_LIT( KGifMime, "image/gif" );
_LIT( KXbmpMime, "image/x-bmp" );
_LIT( KBmpMime, "image/bmp" );
_LIT( KPngMime, "image/png" );
_LIT( KMbmMime, "image/x-epoc-mbm" );
_LIT( KOtaMime, "image/vnd.nokia.ota-bitmap" );
_LIT( KXotaMime, "image/x-ota-bitmap" );
_LIT( KWbmpMime, "image/vnd.wap.wbmp" );
_LIT( KWmfMime, "image/x-wmf" );
_LIT( KOtbMime, "image/vnd.nokia.ota-bitmap" );

_LIT( KExtJpg, "JPG" );
_LIT( KExtJpeg, "JPEG" );
_LIT( KExtJp2, "JP2" );
_LIT( KExtJ2k, "J2K" ); 
_LIT( KExtJpx, "JPX" ); 
_LIT( KExtJpf, "JPF" );
_LIT( KExtMbm, "MBM" ); 
_LIT( KExtPng, "PNG" );
_LIT( KExtGif, "GIF" );
_LIT( KExtBmp, "BMP" );
_LIT( KExtTif, "TIF" );
_LIT( KExtTiff, "TIFF" );
_LIT( KExtOta, "OTA" );
_LIT( KExtWbmp, "WBMP" );
_LIT( KExtWmf, "WMF" ); 
_LIT( KExtOtb, "OTB" ); 

#ifdef MDS_MULTIPLE_STAGE_IMAGE_PROCESSING
const TUid KBGPSUid = { 0x0ADC2480 };
#endif

CHarvesterImagePluginPropertyDefs::CHarvesterImagePluginPropertyDefs() : CBase(),
    iCreationDatePropertyDef( NULL )
	{
	}

void CHarvesterImagePluginPropertyDefs::ConstructL( CMdEObjectDef& aObjectDef )
	{
    SetByObjectDefL( aObjectDef );
	}

CHarvesterImagePluginPropertyDefs* CHarvesterImagePluginPropertyDefs::NewL()
    {
    CHarvesterImagePluginPropertyDefs* self = 
        new (ELeave) CHarvesterImagePluginPropertyDefs();
    return self;
    }

void CHarvesterImagePluginPropertyDefs::SetByObjectDefL(CMdEObjectDef& aObjectDef)
    {
    CMdENamespaceDef& nsDef = aObjectDef.NamespaceDef();

    // Image property definitions
    CMdEObjectDef& objectDef = nsDef.GetObjectDefL( Object::KBaseObject );
    iCreationDatePropertyDef = &objectDef.GetPropertyDefL( Object::KCreationDateProperty );
    iLastModifiedDatePropertyDef = &objectDef.GetPropertyDefL( Object::KLastModifiedDateProperty );
    iSizePropertyDef = &objectDef.GetPropertyDefL( Object::KSizeProperty );
    iTimeOffsetPropertyDef = &objectDef.GetPropertyDefL( Object::KTimeOffsetProperty );
    iItemTypePropertyDef = &objectDef.GetPropertyDefL( Object::KItemTypeProperty );
    iDefaultFolderPropertyDef = &objectDef.GetPropertyDefL( Object::KInDefaultFolder );

    // Media property definitions
    CMdEObjectDef& mediaDef = nsDef.GetObjectDefL( MediaObject::KMediaObject );
    iWidthPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KWidthProperty );
    iHeightPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KHeightProperty );
    iDescriptionPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KDescriptionProperty );
    iCommentPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KCommentProperty );
    iReleaseDatePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KReleaseDateProperty );
    iCopyrightPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KCopyrightProperty );
    iCaptureDatePropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KCaptureDateProperty );
    iResolutionUnitPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KResolutionUnitProperty );
    iArtistPropertyDef = &mediaDef.GetPropertyDefL( MediaObject::KArtistProperty );

    // Image property definitions
    CMdEObjectDef& imageDef = nsDef.GetObjectDefL( Image::KImageObject );
    iPixelYDimensionPropertyDef = &imageDef.GetPropertyDefL( Image::KPixelYDimensionProperty );
    iPixelXDimensionPropertyDef = &imageDef.GetPropertyDefL( Image::KPixelXDimensionProperty );
    iBitsPerSamplePropertyDef = &imageDef.GetPropertyDefL( Image::KBitsPerSampleProperty );
    iFrameCountPropertyDef = &imageDef.GetPropertyDefL( Image::KFrameCountProperty );
    iDateTimeOriginalPropertyDef = &imageDef.GetPropertyDefL( Image::KDateTimeOriginalProperty );
    iDateTimeDigitizedPropertyDef = &imageDef.GetPropertyDefL( Image::KDateTimeDigitizedProperty );
    iDateTimePropertyDef = &imageDef.GetPropertyDefL( Image::KDateTimeProperty );
    iWhiteBalancePropertyDef = &imageDef.GetPropertyDefL( Image::KWhiteBalanceProperty );
    iFlashPropertyDef = &imageDef.GetPropertyDefL( Image::KFlashProperty );
    iExposureProgramPropertyDef = &imageDef.GetPropertyDefL( Image::KExposureProgramProperty );
    iMakePropertyDef = &imageDef.GetPropertyDefL( Image::KMakeProperty );
    iModelPropertyDef = &imageDef.GetPropertyDefL( Image::KModelProperty );
    iOrientationPropertyDef = &imageDef.GetPropertyDefL( Image::KOrientationProperty );
    iXResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KXResolutionProperty );
    iYResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KYResolutionProperty );
    iYCbCrPositioningPropertyDef = &imageDef.GetPropertyDefL( Image::KYCbCrPositioningProperty );
    iExposureTimePropertyDef = &imageDef.GetPropertyDefL( Image::KExposureTimeProperty );
    iFNumberPropertyDef = &imageDef.GetPropertyDefL( Image::KFNumberProperty );
    iExifVersionPropertyDef = &imageDef.GetPropertyDefL( Image::KExifVersionProperty );
    iShutterSpeedValuePropertyDef = &imageDef.GetPropertyDefL( Image::KShutterSpeedValueProperty );
    iApertureValuePropertyDef = &imageDef.GetPropertyDefL( Image::KApertureValueProperty );
    iFocalLengthPropertyDef = &imageDef.GetPropertyDefL( Image::KFocalLengthProperty );
    iFlashPixVersionPropertyDef = &imageDef.GetPropertyDefL( Image::KFlashPixVersionProperty );
    iColourSpacePropertyDef = &imageDef.GetPropertyDefL( Image::KColourSpaceProperty );
    iISOSpeedRatingsPropertyDef = &imageDef.GetPropertyDefL( Image::KISOSpeedRatingsProperty );
    iComponentsConfigurationPropertyDef = &imageDef.GetPropertyDefL( Image::KComponentsConfigurationProperty );
    iExposureBiasValuePropertyDef = &imageDef.GetPropertyDefL( Image::KExposureBiasValueProperty );
    iSamplesPerPixelPropertyDef = &imageDef.GetPropertyDefL( Image::KSamplesPerPixelProperty );
    iThumbCompressionPropertyDef = &imageDef.GetPropertyDefL( Image::KThumbCompressionProperty );
    iThumbXResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KThumbXResolutionProperty );
    iThumbYResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KThumbYResolutionProperty );
    iThumbResolutionUnitPropertyDef = &imageDef.GetPropertyDefL( Image::KThumbResolutionUnitProperty );
    iFocalLengthIn35mmFilmPropertyDef = &imageDef.GetPropertyDefL( Image::KFocalLengthIn35mmFilmProperty );
    iMeteringModePropertyDef = &imageDef.GetPropertyDefL( Image::KMeteringModeProperty );
    iRelatedSoundFilePropertyDef = &imageDef.GetPropertyDefL( Image::KRelatedSoundFileProperty );
    iFocalPlaneResolutionUnitPropertyDef = &imageDef.GetPropertyDefL( Image::KFocalPlaneResolutionUnitProperty );
    iFocalPlaneXResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KFocalPlaneXResolutionProperty );
    iFocalPlaneYResolutionPropertyDef = &imageDef.GetPropertyDefL( Image::KFocalPlaneYResolutionProperty );
    iDraftPropertyDef = &imageDef.GetPropertyDefL( Image::KDraftProperty );
    }

/**
* Default constructor
*/
CHarvesterImagePlugin::CHarvesterImagePlugin() : CHarvesterPlugin(),
    iExifUtil( NULL ), iDecoder( NULL ), iPropDefs( NULL )
	{
	}

/**
* Construction
* @return Harvester image plugin
*/
CHarvesterImagePlugin* CHarvesterImagePlugin::NewL()
	{
	WRITELOG( "CHarvesterImagePlugin::NewL()" );
	CHarvesterImagePlugin* self = new (ELeave) CHarvesterImagePlugin();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	
	return self;
	}

/**
* Destruction
*/
CHarvesterImagePlugin::~CHarvesterImagePlugin()
	{
	WRITELOG( "CHarvesterImagePlugin::~CHarvesterImagePlugin()" );
	
	if (iDecoder)
		{
		iDecoder->Reset();
	    delete iDecoder;
	    iDecoder = NULL;
		}   
    
    if (iExifUtil)
		{
		delete iExifUtil;
		iExifUtil = NULL;
		}
   	
    delete iPropDefs;
    iPropDefs = NULL;
    
    delete iPhoneImagesPath;
    iPhoneImagesPath = NULL;
    delete iMmcImagesPath;
    iMmcImagesPath = NULL;
    
    iMimeTypeMappings.Close();
    iFbs.Disconnect();
	}

/**
* 2nd phase constructor
*/
void CHarvesterImagePlugin::ConstructL()
	{
	WRITELOG( "CHarvesterImagePlugin::ConstructL()" );
	iDecoder = CBufferedImageDecoder::NewL( iFs );
	iExifUtil = CHarvesterExifUtil::NewL();
    User::LeaveIfError( iFbs.Connect() );

    iPropDefs = CHarvesterImagePluginPropertyDefs::NewL(); 

    TLinearOrder< TMimeTypeMapping<TImageMetadataHandling> > cmp(
    		TMimeTypeMapping<TImageMetadataHandling>::CompareFunction);

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJpg(), KJpegMime(), EJpegHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJpeg(), KJpegMime(), EJpegHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtGif(), KGifMime(), EGifHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtBmp(), KBmpMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJpf(), KJpeg2000_2Mime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJpx(), KJpeg2000_2Mime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJp2(), KJpeg2000Mime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtJ2k(), KJpeg2000Mime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtMbm(), KMbmMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtOta(), KOtaMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtPng(), KPngMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtTif(), KTiffMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtTiff(), KTiffMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtWbmp(), KWbmpMime(), EOtherHandling ), cmp ) );

    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
    		KExtWmf(), KWmfMime(), EOtherHandling ), cmp ) );
    
    User::LeaveIfError( iMimeTypeMappings.InsertInOrder( TMimeTypeMapping<TImageMetadataHandling>(
            KExtOtb(), KOtbMime(), EOtherHandling ), cmp ) );

    TFileName images = PathInfo::ImagesPath();
    
    TFileName phonePath = PathInfo::PhoneMemoryRootPath();
    phonePath.Append( images );
    iPhoneImagesPath = phonePath.AllocL();

    TFileName mmcPath = PathInfo::MemoryCardRootPath();
    mmcPath.Append( images );
    iMmcImagesPath = mmcPath.Right( mmcPath.Length() - 1 ).AllocL();
	}

void CHarvesterImagePlugin::HarvestL( CHarvesterData* aHD )
	{
	WRITELOG( "CHarvesterImagePlugin::HarvestL()" );
    CMdEObject& mdeObject = aHD->MdeObject();

    CFileData* fileData = CFileData::NewL();
    CleanupStack::PushL( fileData );
    CHarvestData* harvestData = CHarvestData::NewL();
    CleanupStack::PushL( harvestData );
        
    TInt errorCode( KErrNone );
    TRAPD( error, errorCode = GatherDataL( mdeObject, *fileData, *harvestData ) );
    if ( error == KErrNone && (errorCode == KErrNone || errorCode == KErrCompletion ) ) // ok, something got harvested
        {
        if ( mdeObject.Id() == 0 || mdeObject.Placeholder() ) // is a new object or placeholder
            {
            TRAP_IGNORE( HandleObjectPropertiesL( *harvestData, *fileData, *aHD, ETrue ) );
            mdeObject.SetPlaceholder( EFalse );
            }
        else   // not a new object
            {
            TRAP_IGNORE( HandleObjectPropertiesL( *harvestData, *fileData, *aHD, EFalse ) );
            }
        
        if( harvestData->iStoreGpsLatitudeAndLongitude ||
        	harvestData->iStoreGpsAltitude )
        	{
        	TLocationData* locData = new (ELeave) TLocationData();

        	if( harvestData->iStoreGpsAltitude )
        		{
        		locData->iPosition.SetCoordinate( 
        				harvestData->iGpsLatitude, harvestData->iGpsLongitude, 
        				harvestData->iGpsAltitude);
        		}
        	else
        		{
        		locData->iPosition.SetCoordinate( 
        				harvestData->iGpsLatitude, harvestData->iGpsLongitude);
        		}

        	// remove unnessesary values
        	locData->iNetworkInfo.iAreaKnown = EFalse;
        	
        	TRealX nanX;
        	nanX.SetNaN();
        	nanX.GetTReal( locData->iQuality );
        	
        	aHD->SetLocationData( locData );
        	}
        }
    else if( error != KErrNone)
        {
        WRITELOG1( "CHarvesterImagePlugin::HarvestL() - other error: %d", error );
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( error, convertedError );
        aHD->SetErrorCode( convertedError );
        WRITELOG1( "CHarvesterImagePlugin::HarvestL() - returning: %d", convertedError );
        }
    else
        {
        WRITELOG1( "CHarvesterImagePlugin::HarvestL() - other error: %d", errorCode );
        TInt convertedError = KErrNone;
        MdsUtils::ConvertTrapError( errorCode, convertedError );
        aHD->SetErrorCode( convertedError );
        WRITELOG1( "CHarvesterImagePlugin::HarvestL() - returning: %d", convertedError );
        }

    // Delete image data.
    CleanupStack::PopAndDestroy( 2, fileData );
	}

// ---------------------------------------------------------------------------
// CHarvesterImagePlugin::GetMimeType (from CHarvesterPlugin)
// ---------------------------------------------------------------------------
//    
void CHarvesterImagePlugin::GetMimeType( const TDesC& aUri, TDes& aMimeType )
    {
    aMimeType.Zero();
    
    TPtrC ext;
    if( !MdsUtils::GetExt( aUri, ext ) )
        {
        return;
        }

    TMimeTypeMapping<TImageMetadataHandling> finder(ext);
    TLinearOrder< TMimeTypeMapping<TImageMetadataHandling> > cmp(
            TMimeTypeMapping<TImageMetadataHandling>::CompareFunction);

    const TInt pos = iMimeTypeMappings.FindInOrder( finder, cmp );

    if ( pos == KErrNotFound )
        {
        return;
        }

    aMimeType = iMimeTypeMappings[pos].iMimeType;
    }

// ---------------------------------------------------------------------------
// GatherData
// ---------------------------------------------------------------------------
//    
TInt CHarvesterImagePlugin::GatherDataL( CMdEObject& aMetadataObject,
    CFileData& aFileData, CHarvestData& aHarvestData )
    {
#ifdef _DEBUG
    TTime dStart, dStop;
    dStart.UniversalTime();
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterImagePlugin::GatherData() start %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif
    
    WRITELOG( "CHarvesterImagePlugin::GatherData()" );

    TBool dataExtracted( aMetadataObject.Id() == 0 || aMetadataObject.Placeholder() );
    aFileData.iImageDef = &aMetadataObject.Def();
    
    if( dataExtracted )
        {
        CMdEProperty* prop = NULL;
        CMdEObjectDef& objectDef = *aFileData.iImageDef;
		
        InitPropDefsL( objectDef );        
        
        aMetadataObject.Property( *iPropDefs->iSizePropertyDef, prop );
        if( prop )
            {
            aFileData.iFileSize  = prop->Uint32ValueL();
            }
        else
            {
            dataExtracted = EFalse;
            }

        aMetadataObject.Property( *iPropDefs->iLastModifiedDatePropertyDef, prop );
        if( prop )
            {
            aFileData.iModified  = prop->TimeValueL();
            }
        else
            {
            dataExtracted = EFalse;
            }
        }

    const TDesC& uri = aMetadataObject.Uri();
    
    if( !dataExtracted )
        {
        TEntry entry;
        const TInt errorcode = iFs.Entry( uri, entry );
        
        if ( errorcode != KErrNone )
            {
            return errorcode; // metadata cannot be gathered!
            }
        
        aFileData.iModified = entry.iModified;
        aFileData.iFileSize = (TUint)entry.iSize;
        }

    TPtrC imageFile = uri;

    aFileData.iUri = &imageFile;
    aFileData.iFrameCount = 1;

    TRAPD( readError, DataFromImageFileL( aFileData ) );

    if ( readError != KErrNone )
        {
        WRITELOG1( "CHarvesterImagePlugin::GatherData() - error reading image data, error: %d", readError );
        return KErrCompletion; // metadata item still can be created, thus KErrCompletion
        }

    TInt err = KErrNone;
    if ( aFileData.iExifSupported )
        {
        TRAP( err, iExifUtil->ReadExifDataL( aHarvestData, aFileData ) );
        }
    if( !aFileData.iExifSupported || err != KErrNone )
        {
        // Exif couldn't be found. Open the image with ICL decoder instead.
        WRITELOG( "CHarvesterImagePlugin::GatherData() - Exif could not be read. Using ICL." );

        TPtr8 imageDataPtr = aFileData.iImageData->Des();
        TRAP( err, iDecoder->OpenL(imageDataPtr, aFileData.iMime8,
                CImageDecoder::TOptions( CImageDecoder::EPreferFastDecode | CImageDecoder::EOptionIgnoreExifMetaData ) ) );

        if ( err != KErrNone )
            {
            WRITELOG1( "CHarvesterImagePlugin::GatherData() - ERROR: Decoder could not open image data! Code %d", err );
#ifdef _DEBUG
            if( err == KErrInUse || err == KErrLocked )
                {
                TPtrC fileName( uri.Mid(2) );
                WRITELOG1( "CHarvesterImagePlugin :: Checking open file handles to %S", &fileName );

                CFileList* fileList = 0;
                TOpenFileScan fileScan( iFs );

                fileScan.NextL( fileList );   
  
                while ( fileList )   
                    {
                    const TInt count( fileList->Count() );
                    for (TInt i = 0; i < count; i++ )   
                        {   
                        if ( (*fileList)[i].iName == uri.Mid(2) )
                            {
                            TFullName processName;
                            TFindThread find(_L("*"));
                            while( find.Next( processName ) == KErrNone )
                                {
                                RThread thread;
                                TInt err = thread.Open( processName );
     
                                if ( err == KErrNone )
                                    {
                                    if ( thread.Id().Id() ==  fileScan.ThreadId() )
                                        {
                                        processName = thread.Name();
                                        thread.Close();
                                        WRITELOG1( "CHarvesterImagePlugin:: %S has a file handle open", &processName );
                                        break;
                                        }
                                    thread.Close();
                                    }
                                }
                            }
                        }
                    fileScan.NextL( fileList );   
                    } 
                }
#endif
            iDecoder->Reset();
            return KErrCompletion; // metadata item still can be created, thus KErrCompletion
            }

        WRITELOG( "CHarvesterImagePlugin::GatherData() - Image decoder has opened the file." );
        
        if ( !iDecoder->ValidDecoder() )
        	{
        	// read all remaining data from file
        	TInt64 additionalDataSize = aFileData.iFileSize - aFileData.iImageDataSize;
        	if( additionalDataSize < 0 )
        	    {
        	    additionalDataSize = 0;
        	    }
 
            HBufC8* additionalData = HBufC8::NewLC( additionalDataSize );
			if ( additionalData )
				{
				TPtr8 addPtr = additionalData->Des();
				TInt readStatus = iFs.ReadFileSection(
						*aFileData.iUri, aFileData.iImageDataSize, addPtr, additionalDataSize );
				if ( readStatus != KErrNone )
					{
					CleanupStack::PopAndDestroy( additionalData );
					iDecoder->Reset();
					return KErrCompletion;
					}
				
				// append all remaining data to image decoder
				TRAP( err, iDecoder->AppendDataL( addPtr ) );
				if ( err == KErrNone )
					{
					TRAP( err, iDecoder->ContinueOpenL() );
					}
				CleanupStack::PopAndDestroy( additionalData );
				}

            if ( err || !iDecoder->ValidDecoder() )
            	{
            	WRITELOG( "CHarvesterImagePlugin::GatherData() - ERROR: no valid decoder" );
            	iDecoder->Reset();
            	return KErrCompletion; // metadata item still can be created, thus KErrCompletion
            	}
            }

        // Get image width, frame count, height and bits per pixel from image decoder.
        const TFrameInfo info = iDecoder->FrameInfo( 0 );
        const TSize imageSize = info.iOverallSizeInPixels;
        const TInt framecount = iDecoder->FrameCount();
        aFileData.iFrameCount = framecount;
        aFileData.iImageWidth = imageSize.iWidth;
        aFileData.iImageHeight = imageSize.iHeight;
        aFileData.iBitsPerPixel = info.iBitsPerPixel;
        
        iDecoder->Reset();
        }

    WRITELOG( "CHarvesterImagePlugin::GatherData() - end" );

#ifdef _DEBUG
    dStop.UniversalTime();
    WRITELOG1( "CHarvesterImagePlugin::GatherData() end %d us", (TInt)dStop.MicroSecondsFrom(dStart).Int64() );
#endif   

    return KErrNone;
    }

///---------------------------------------------------------------------------
/// DataFromImageFile
///---------------------------------------------------------------------------
void CHarvesterImagePlugin::DataFromImageFileL( CFileData& aFileData )
    {
    WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL()" );
    
    TPtrC ext;
    if( !MdsUtils::GetExt( *aFileData.iUri, ext ) )
    	{
    	User::Leave( KErrNotSupported );
    	}

	TMimeTypeMapping<TImageMetadataHandling> finder(ext);
	TLinearOrder< TMimeTypeMapping<TImageMetadataHandling> > cmp(
			TMimeTypeMapping<TImageMetadataHandling>::CompareFunction);

	const TInt pos = iMimeTypeMappings.FindInOrder( finder, cmp );

	TImageMetadataHandling handler( EOtherHandling ); 

	if ( pos != KErrNotFound )
		{
		handler = iMimeTypeMappings[pos].iHandler;
		}

    switch( handler )
    	{
    	case EJpegHandling:
			{
			aFileData.iJpeg = ETrue;
			const TInt K64Kb = 65536;
			aFileData.iImageDataSize = K64Kb;
			WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - read first 64Kb from JPEG" );
			break;
			}
    	case EGifHandling:
    		{
    		aFileData.iJpeg = EFalse;
    		aFileData.iImageDataSize = aFileData.iFileSize;
			WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - read whole GIF file" );
    		break;
    		}
    	default:
    		{
    		aFileData.iJpeg = EFalse;
			const TInt K4Kb = 4096;
			aFileData.iImageDataSize = K4Kb; 
			WRITELOG1( "CHarvesterImagePlugin::DataFromImageFileL() - read first %d bytes from image", aFileData.iImageDataSize );
    		}
    	}	

    aFileData.iImageData = HBufC8::NewL( aFileData.iImageDataSize );
    TPtr8 myImagePtr = aFileData.iImageData->Des();

    const TInt readStatus = iFs.ReadFileSection(
    		*aFileData.iUri, 0, myImagePtr, aFileData.iImageDataSize );
    if ( readStatus != KErrNone )
        {
        WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - ERROR: file could not be read!" );
        User::Leave( readStatus );
        }

    // extension was in mapping list, so use that 
	if ( pos != KErrNotFound )
		{
		aFileData.iMime8.Copy( iMimeTypeMappings[pos].iMimeType );
		aFileData.iMime16.Copy( aFileData.iMime8 );
		}
	// otherwise try to recognize using image decoder
    else
        {
        CImageDecoder::GetMimeTypeDataL( myImagePtr, aFileData.iMime8 );
        aFileData.iMime16.Copy( aFileData.iMime8 );

        // Check if MIME type is supported
        User::LeaveIfError( CheckIfMimeSupported( aFileData.iMime16 ) );
        }

    // If is jpeg get EXIF data
    if ( aFileData.iJpeg )
        {
        if( iExifUtil->IsValidExifData(myImagePtr) )
            {
            WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - exif data found!" );
            aFileData.iExifSupported = ETrue;
            }
        else
            {
            WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - no exif data!" );
            aFileData.iImageDataSize = aFileData.iFileSize;
            aFileData.iExifSupported = EFalse;
            }
        }
    
    WRITELOG( "CHarvesterImagePlugin::DataFromImageFileL() - reading IMAGE file done!" );
    }

// ---------------------------------------------------------------------------
// HandleNewObjectL
// ---------------------------------------------------------------------------
//    
void CHarvesterImagePlugin::HandleObjectPropertiesL(
		CHarvestData& aHd,
		CFileData& aFileData,
		CHarvesterData& aHarvesterData,
		TBool aIsAdd )
    {
    WRITELOG( "CHarvesterImagePlugin::HandleObjectPropertiesL() - New MdE object" );
    
    CMdEObject& mdeObject = aHarvesterData.MdeObject();
	
    InitPropDefsL( mdeObject.Def() );

    TTimeIntervalSeconds timeOffsetSeconds = User::UTCOffset();

    TTime localModifiedDate = aFileData.iModified + timeOffsetSeconds;
    
    if( ! mdeObject.Placeholder() )
    	{
        // Object - Creation date
	    if ( aFileData.iExifSupported && aHd.iDateOriginal8 )
	        {
	        TTime originalTime = iExifUtil->ConvertExifDateTimeToSymbianTimeL(
	        		aHd.iDateOriginal8->Des() );
	        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCreationDatePropertyDef, &originalTime, aIsAdd );
	        }
	    else if ( aIsAdd )
	        {
	        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCreationDatePropertyDef, &localModifiedDate, aIsAdd );
	        }
	    
	    // Object - last aFileData.iModified date
	    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iLastModifiedDatePropertyDef, &aFileData.iModified, aIsAdd );
	    
	    // Object - Size
	    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iSizePropertyDef, &aFileData.iFileSize, aIsAdd );
	    
	    // Item Type
	    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iItemTypePropertyDef, &aFileData.iMime16, aIsAdd );

	    const TDesC& uri = mdeObject.Uri();
	    if( uri.FindF( iMmcImagesPath->Des()) != KErrNotFound ||
	        uri.FindF( iPhoneImagesPath->Des()) != KErrNotFound ||
	        uri.FindF( KDCIMFolder ) != KErrNotFound )
	        {
	        TBool inDefaultFolder( ETrue );
	        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );
	        }
	    else
	        {
	        TBool inDefaultFolder( EFalse );
	        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDefaultFolderPropertyDef, &inDefaultFolder, aIsAdd );    
	        }    	
    	}
    
    if( aFileData.iJpeg )
    	{
    	// Time offset
    	TInt16 timeOffsetMinutes = timeOffsetSeconds.Int() / 60;
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iTimeOffsetPropertyDef, &timeOffsetMinutes, aIsAdd );
    	}
    
    // MediaObject - Width
    if ( aFileData.iExifSupported )
        {
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iWidthPropertyDef, &aHd.iImageWidthExif, aIsAdd );

        // If pixelXDimension tag is found, save its value to both Width and PixelXDimension in DB.
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iPixelXDimensionPropertyDef, &aHd.iImageWidthExif, aIsAdd );
        }
    else if (aFileData.iImageWidth != 0)
        {
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iWidthPropertyDef, &aFileData.iImageWidth, aIsAdd );
        }
    
    // MediaObject - Height
    if ( aFileData.iExifSupported )
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iHeightPropertyDef, &aHd.iImageHeightExif, aIsAdd );
        
        // If pixelYDimension tag is found, save its value to both Height and PixelYDimension in DB.
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iPixelYDimensionPropertyDef, &aHd.iImageHeightExif, aIsAdd );
        }
    else if (aFileData.iImageHeight != 0)
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iHeightPropertyDef, &aFileData.iImageHeight, aIsAdd );
        } 

#ifdef MDS_MULTIPLE_STAGE_IMAGE_PROCESSING
    TBool draftVal = ETrue;
    if( aIsAdd )
        {
        if( aHarvesterData.Origin() != MdeConstants::Object::ECamera )
            {
            draftVal = EFalse;
            }
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDraftPropertyDef, &draftVal, aIsAdd );
        }
    else
        {
        if( aHarvesterData.ClientId() == KBGPSUid )
            {
            draftVal = EFalse;
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDraftPropertyDef, &draftVal, aIsAdd );
            }
        }
#endif
    
    // Image - Bits per Sample
    if (aFileData.iBitsPerPixel != 0)
        {
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iBitsPerSamplePropertyDef, &aFileData.iBitsPerPixel, aIsAdd );
        }

    // Image - Framecount
    CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFrameCountPropertyDef, &aFileData.iFrameCount, aIsAdd );

    // If is jpeg write EXIF data
    if ( aFileData.iExifSupported )
        {
       	// MediaObject - Description
        if( aHd.iDescription16 && aHd.iDescription16->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDescriptionPropertyDef, aHd.iDescription16, aIsAdd );
            }     	

        // MediaObject - Comment (user comment)
        if( aHd.iComment16 && aHd.iComment16->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCommentPropertyDef, aHd.iComment16, aIsAdd );
            }
        
        // MediaObject - Release date
    	CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iReleaseDatePropertyDef, &localModifiedDate, aIsAdd );

        // MediaObject - Copyright
        if( aHd.iCopyright16 && aHd.iCopyright16->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCopyrightPropertyDef, aHd.iCopyright16, aIsAdd );
            }
        
        // Data & time original
        if ( aHd.iDateOriginal8 )
            {
            TTime originalTime = iExifUtil->ConvertExifDateTimeToSymbianTimeL( aHd.iDateOriginal8->Des() );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDateTimeOriginalPropertyDef, &originalTime, aIsAdd );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCaptureDatePropertyDef, &originalTime, aIsAdd );
            if( originalTime.Int64() == 0 )
                {
                CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCreationDatePropertyDef, &localModifiedDate, EFalse );
                }
            else
                {
                CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iCreationDatePropertyDef, &originalTime, EFalse );
                }
            }
        
        // Date & time digitized
        if ( aHd.iDateDigitized8 )
            {
            TTime digitizedTime = iExifUtil->ConvertExifDateTimeToSymbianTimeL( aHd.iDateDigitized8->Des() );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDateTimeDigitizedPropertyDef, &digitizedTime, aIsAdd );
            }

        // Date & time aFileData.iModified (DateTime tag)
        if ( aHd.iDateModified8 )
            {
            TTime modifiedTime = iExifUtil->ConvertExifDateTimeToSymbianTimeL(
            		aHd.iDateModified8->Des() );
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iDateTimePropertyDef, &modifiedTime, aIsAdd );
            }
        
        // Artist    
        if( aHd.iArtist && aHd.iArtist->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iArtistPropertyDef, aHd.iArtist, aIsAdd );
            }

        // Image - White balance
        if ( aHd.iStoreWhiteBalance )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iWhiteBalancePropertyDef, &aHd.iWhiteBalance, aIsAdd );
            }
         
        // Image - Flash
        if ( aHd.iStoreFlash )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFlashPropertyDef, &aHd.iFlash, aIsAdd );
            }
        
        // Image - Exposure program                
        if ( aHd.iStoreExposureProgram )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iExposureProgramPropertyDef, &aHd.iExposureProgram, aIsAdd );
            }     
         
         // Make string   
        if( aHd.iMake && aHd.iMake->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iMakePropertyDef, aHd.iMake, aIsAdd );
            }
            
        // Model string
        if( aHd.iModel && aHd.iModel->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iModelPropertyDef, aHd.iModel, aIsAdd );
            }
                    
        // Orientation
        if ( aHd.iStoreOrientation )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iOrientationPropertyDef, &aHd.iOrientation, aIsAdd );
            }
            
        // X resolution    
        if ( aHd.iStoreXResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iXResolutionPropertyDef, &aHd.iXResolution, aIsAdd );
            }
            
        // Y resolution    
        if ( aHd.iStoreYResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iYResolutionPropertyDef, &aHd.iYResolution, aIsAdd );
            }
            
        // Resolution unit
        CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iResolutionUnitPropertyDef, &aHd.iResolutionUnit, aIsAdd );
        
        // YCbCrPositioning
        if ( aHd.iStoreYCbCrPositioning )    
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iYCbCrPositioningPropertyDef, &aHd.iYCbCrPositioning, aIsAdd );
            }
        
        // Exposure time                
        if ( aHd.iStoreExposureTime )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iExposureTimePropertyDef, &aHd.iExposureTime, aIsAdd );
            }
            
        // F number
        if ( aHd.iStoreFNumber )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFNumberPropertyDef, &aHd.iFNumber, aIsAdd );
            }
            
        // EXIF version                
        if ( aHd.iStoreExifVersion )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iExifVersionPropertyDef, &aHd.iExifVersion, aIsAdd );
            }
        
        //     Shutter speed
        if ( aHd.iStoreShutterSpeed )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iShutterSpeedValuePropertyDef, &aHd.iShutterSpeed, aIsAdd );
            }
            
        // Aperture    
        if ( aHd.iStoreAperture )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iApertureValuePropertyDef, &aHd.iAperture, aIsAdd );
            }
        
        // Focal length        
        if ( aHd.iStoreFocalLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFocalLengthPropertyDef, &aHd.iFocalLength, aIsAdd );
            }
            
        // FlashPix version    
        if ( aHd.iStoreFlashPixVersion )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFlashPixVersionPropertyDef, &aHd.iFlashPixVersion, aIsAdd );
            }
            
        // Colour space            
        if ( aHd.iStoreColourSpace )    
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iColourSpacePropertyDef, &aHd.iColourSpace, aIsAdd );
            }
            
        // ISO speed rating            
        if ( aHd.iStoreIsoSpeedRating )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iISOSpeedRatingsPropertyDef, &aHd.iIsoSpeedRating, aIsAdd );
            }
        
        // Components configuration    
        if ( aHd.iStoreComponentsConfig )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iComponentsConfigurationPropertyDef, &aHd.iComponentsConfiguration, aIsAdd );
            }
        
        // Exposure bias value        
        if ( aHd.iStoreExposureBias )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iExposureBiasValuePropertyDef, &aHd.iExposureBias, aIsAdd );
            }
            
        // Samples per pixel    
        if ( aHd.iStoreSamplesPerPixel )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iSamplesPerPixelPropertyDef, &aHd.iSamplesPerPixel, aIsAdd );
            }
            
        // Thumbnail compression                
        if ( aHd.iStoreThumbCompression )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iThumbCompressionPropertyDef, &aHd.iThumbCompression, aIsAdd );
            }
            
        // Thumbnail X resolution    
        if ( aHd.iStoreThumbXResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iThumbXResolutionPropertyDef, &aHd.iThumbXResolution, aIsAdd );
            }
            
        // Thumbnail Y resolution            
        if ( aHd.iStoreThumbYResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iThumbYResolutionPropertyDef, &aHd.iThumbYResolution, aIsAdd );
            }
            
        // Thumbnail resolution unit    
        if ( aHd.iStoreThumbResolutionUnit )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iThumbResolutionUnitPropertyDef, &aHd.iThumbResolutionUnit, aIsAdd );
            }
        
        // Focal length in 35 mm    
        if ( aHd.iStoreFocalLengthIn35 )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFocalLengthIn35mmFilmPropertyDef, &aHd.iFocalLengthIn35mm, aIsAdd );
            }
            
        // Metering mode                
        if ( aHd.iStoreMeteringMode )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iMeteringModePropertyDef, &aHd.iMeteringMode, aIsAdd );
            }
            
        // Related soundfile            
        if ( aHd.iRelatedSoundFile && aHd.iRelatedSoundFile->Length() > 0 && aHd.iRelatedSoundFile->Length() < iMaxTextLength )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iRelatedSoundFilePropertyDef, aHd.iRelatedSoundFile, aIsAdd );
            }
        
        // Focal plane resolution unit
        if ( aHd.iStoreFocalPlaneResolutionUnit )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFocalPlaneResolutionUnitPropertyDef, &aHd.iFocalPlaneResolutionUnit, aIsAdd );
            }
            
        // Focal plane X resolution
        if ( aHd.iStoreFocalPlaneXResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFocalPlaneXResolutionPropertyDef, &aHd.iFocalPlaneXResolution, aIsAdd );
            }
            
        // Focal plane Y resolution
        if ( aHd.iStoreFocalPlaneYResolution )
            {
            CMdeObjectWrapper::HandleObjectPropertyL(mdeObject, *iPropDefs->iFocalPlaneYResolutionPropertyDef, &aHd.iFocalPlaneYResolution, aIsAdd );
            }
        WRITELOG( "CHarvesterImagePlugin::HandleObjectPropertiesL() - All EXIF tags added." );
        }
    }

//---------------------------------------------------------------------------
// CheckIfMimeSupported
//---------------------------------------------------------------------------
//
TInt CHarvesterImagePlugin::CheckIfMimeSupported(const TDesC& aMimeBuf)
    {
    if ( MdsUtils::Compare(KJpegMime, aMimeBuf) == 0
            || MdsUtils::Compare(KJpeg2000Mime, aMimeBuf) == 0
            || MdsUtils::Compare(KJpeg2000_2Mime, aMimeBuf) == 0
            || MdsUtils::Compare(KPngMime, aMimeBuf) == 0
            || MdsUtils::Compare(KGifMime, aMimeBuf) == 0
            || MdsUtils::Compare(KBmpMime, aMimeBuf) == 0
            || MdsUtils::Compare(KMsmMime, aMimeBuf) == 0
            || MdsUtils::Compare(KXbmpMime, aMimeBuf) == 0
            || MdsUtils::Compare(KWbmpMime, aMimeBuf) == 0
            || MdsUtils::Compare(KMbmMime, aMimeBuf) == 0
            || MdsUtils::Compare(KTiffMime, aMimeBuf) == 0
            || MdsUtils::Compare(KOtaMime, aMimeBuf) == 0
            || MdsUtils::Compare(KXotaMime, aMimeBuf) == 0
            || MdsUtils::Compare(KWmfMime, aMimeBuf) == 0 )

    	{
        return KErrNone;
        }
    
    return KErrNotSupported;
    }

void CHarvesterImagePlugin::InitPropDefsL( CMdEObjectDef& aObjectDef )
    {
    if( !iPropDefs->iCreationDatePropertyDef )
        {
        iPropDefs->SetByObjectDefL( aObjectDef );
        // Prefetch max text lengt for validity checking
        iMaxTextLength = iPropDefs->iCopyrightPropertyDef->MaxTextLengthL();
        }
    }

