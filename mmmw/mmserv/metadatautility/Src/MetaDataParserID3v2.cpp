/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides the base class for ID3v2 parsers.
*
*/



// INCLUDE FILES
#include	"MetaDataParserID3v2.h"
#include	"MetaDataParserID3v22.h"
#include	"MetaDataParserID3v23.h"
#include	"MetaDataParserID3v24.h"
#include 	"MetaDataSourceDescriptor.h"

#include    <barsc.h>
#include    <barsread.h>
#include	<topcharacterset.rsg>
#include 	<data_caging_path_literals.hrh>

// CONSTANTS
// (ID3v2 specification found in www.id3.org)
const TInt KID3v2HeaderLength 			= 10;
const TInt KID3v2TagLength 				= 3;

const TInt KMinimumConfidenceRequired = 90;
const TInt KMinimumJapaneseConfidenceRequired = 75;
const TUint KParseFromMemory	= 1000000; 

//Album Type
const TInt KPictureTypeOther				= 0x00;
const TInt KPictureTypePixel                = 0x01;
const TInt KPictureTypeFileIcon             = 0x02;
const TInt KPictureTypeFrontCover			= 0x03;
const TInt KPictureTypeBackCover            = 0x04;

// Album Type priority
const TInt K1stAlbumArt = KPictureTypeOther;
const TInt K2ndAlbumArt = KPictureTypeFrontCover; 
const TInt K3rdAlbumArt = KPictureTypeBackCover;

_LIT8( KID3v2Tag, "ID3" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::CMetaDataParserID3v2
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserID3v2::CMetaDataParserID3v2(
	CMetaDataSource& aSource )
	:	iSource(&aSource),
        iParseFromDescriptor(EFalse),
        iAlbumType(-1),
        iAlbumPtr(NULL)
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::CreateID3v2ParserL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserID3v2* CMetaDataParserID3v2::CreateID3v2ParserL(
	CMetaDataSource& aSource )
    {
    return CreateID3v2ParserL( aSource, 0 );
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::CreateID3v2ParserL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserID3v2* CMetaDataParserID3v2::CreateID3v2ParserL(
	CMetaDataSource& aSource, TUint aOffset )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v2::CreateID3v2ParserL"));
#endif
	TInt version = VersionL(aSource, aOffset);

	CMetaDataParserID3v2* parser = NULL;
	switch ( version )
		{
		case 2:
			parser = STATIC_CAST( CMetaDataParserID3v2*, CMetaDataParserID3v22::NewL(aSource) );
			break;
		case 3:
			parser = STATIC_CAST( CMetaDataParserID3v2*, CMetaDataParserID3v23::NewL(aSource) );
			break;
		case 4:
			parser = STATIC_CAST( CMetaDataParserID3v2*, CMetaDataParserID3v24::NewL(aSource) );
			break;
		default:	// KErrNotFound
			break;
		}
	
	if( parser )
	    {
	    parser->SetID32Offset( aOffset );
	    }
	
	return parser;
    }

// Destructor
CMetaDataParserID3v2::~CMetaDataParserID3v2()
	{
	if(iAutoDetectChinese || iAutoDetectRussian || iAutoDetectJapanese)
		{
		delete iCharacterSet;
		delete iTopCharacterSet;
		}
	if(iParseFromDescriptor)
		{
		delete iTag;
		delete iSourceDes;
		}
	if ( iAlbumPtr )
		{
		delete iAlbumPtr;
		}

    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v2::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v2::ParseL"));
#endif
	iContainer = &aContainer;
	PrepareToParseL();
	CRepository *metadataRepository = CRepository::NewL(KCRUidMetadataUtility);
	TInt err = KErrNone;
	err = metadataRepository->Get(KMetadataUtilityAutoDetectChineseChars, iAutoDetectChinese);
	if(err)
	{
		iAutoDetectChinese = EFalse;	
	}
	err = metadataRepository->Get(KMetadataUtilityAutoDetectJapaneseChars, iAutoDetectJapanese);
	if(err)
	{
		iAutoDetectJapanese = EFalse;	
	}
	delete metadataRepository;
	TLanguage lang = User::Language();
	if(lang == ELangRussian || lang == ELangUkrainian)	
		{
		iAutoDetectRussian = ETrue;	
		}
	if(iAutoDetectChinese || iAutoDetectRussian || iAutoDetectJapanese)
		{
		CreateCharacterSetsL();
		}
	// choose if want to parse from descriptor
	if(iFrameDataSize < KParseFromMemory) // 1Mb
		{
		iTag = HBufC8::NewL(iFrameDataSize);
		TPtr8 des( iTag->Des() );
		iSource->ReadL(iID32Offset, des);
		
		iSourceDes = CMetaDataSourceDescriptor::NewL(des);
		iSource = iSourceDes;
		iParseFromDescriptor = ETrue;
		}
	if ( aWantedFields.Count() == 0 )
		{
		ParseFramesL();
		}
	else
		{
		ParseFramesL(aWantedFields);
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::ParseFramesL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v2::ParseFramesL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v2::ParseFramesL"));
#endif
	TMetaDataFieldId fieldId;
	TInt frameSize;
	TInt frameHeaderSize = 0;
	while ( iFrameOffset < ( iFrameDataSize - 1 ) )
		{
		GetNextFieldL(fieldId, frameHeaderSize, frameSize);

		if ( frameHeaderSize == 0 )
			{
			// We have hit the padding -> no more fields to read
            break;
            }

        if ( frameSize < 0 || frameSize > ( iFrameDataSize - iFrameOffset ) )
            {
            // Invalid frame size -> stop
            break;
            }
		TInt err = KErrNone; // ignore err, as some entry may be extracted without exception
		if ( fieldId != EUnknownMetaDataField )
			{
			switch ( fieldId )
				{
				case EMetaDataSongTitle:
				case EMetaDataArtist:
				case EMetaDataAlbum:
				case EMetaDataAlbumTrack:
				case EMetaDataComposer:
				case EMetaDataCopyright:
				case EMetaDataOriginalArtist:
                case EMetaDataAlbumArtist:
					TRAP(err, GetTextInfoL( fieldId, frameSize ));
					break;

				case EMetaDataGenre:
					TRAP(err, GetGenreL( frameSize ));
					break;

				case EMetaDataYear:
					TRAP(err, GetTextYearL( frameSize ));
					break;

				case EMetaDataComment:
					TRAP(err, GetCommentL( frameSize ));
					break;

				case EMetaDataUrl:
					TRAP(err, GetUrlL( frameSize ));
					break;
				
				case EMetaDataUserUrl:
					TRAP(err, GetUserUrlL( frameSize ));
					break;
					
				case EMetaDataJpeg:
				    // check for 1st album art
				    if (iAlbumType != K1stAlbumArt)
				        {
	                    TRAP(err,GetJpegL( frameSize ));				        
				        }
					break;

				case EMetaDataDuration:
					TRAP(err, GetDurationL( frameSize ));
					break;
				
				case EMetaDataDate:
					TRAP(err, GetTextDateL( frameSize ));
					break;
						
				case EMetaDataRating:
					TRAP(err, GetRatingL( frameSize ));
					break;
							
				default:
					break;
				}
			}
		iFrameOffset += (frameSize + frameHeaderSize);
		}
    
    // retrieve the album art
    TRAPD (error, RetrieveAlbumArtL());
    if (error)
        {
        #ifdef _DEBUG
            RDebug::Print(_L("CMetaDataParserID3v2::ParseFramesL - RetrieveAlbumArt leave"));
        #endif        
        }
  
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::ParseFramesL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v2::ParseFramesL(
	const RArray<TMetaDataFieldId>& aWantedFields )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v2::ParseFramesL - 2"));
#endif
	TMetaDataFieldId fieldId;
	TInt frameSize;
	TInt frameHeaderSize;
	while ( iFrameOffset < ( iFrameDataSize - 1 ) )
		{
		GetNextFieldL(fieldId, frameHeaderSize, frameSize);

		if ( frameHeaderSize == 0 )
			{
			// We have hit the padding -> no more fields to read
            break;
            }

        if ( frameSize < 0 || frameSize > ( iFrameDataSize - iFrameOffset ) )
            {
            // Invalid frame size -> stop
            break;
            }
		TInt err = KErrNone; 
		if ( fieldId != EUnknownMetaDataField )
			{
            // Look for it in the wanted field array
            TInt count( aWantedFields.Count() );
            for ( TInt i = 0; i < count; i++ )
                {
                if ( aWantedFields[ i ] == fieldId )
                    {
					switch ( fieldId )
						{
						case EMetaDataSongTitle:
						case EMetaDataArtist:
						case EMetaDataAlbum:
						case EMetaDataAlbumTrack:
						case EMetaDataComposer:
						case EMetaDataCopyright:
						case EMetaDataOriginalArtist:
		                case EMetaDataAlbumArtist:
							TRAP(err, GetTextInfoL( fieldId, frameSize ));
							break;

						case EMetaDataGenre:
							TRAP(err, GetGenreL( frameSize ));
							break;

						case EMetaDataYear:
							TRAP(err, GetTextYearL( frameSize ));
							break;

						case EMetaDataComment:
							TRAP(err, GetCommentL( frameSize ));
							break;

						case EMetaDataUrl:
							TRAP(err, GetUrlL( frameSize ));
							break;

						case EMetaDataJpeg:
		                    // check for 1st album art
		                    if (iAlbumType != K1stAlbumArt)
		                        {
		                        TRAP(err,GetJpegL( frameSize ));                        
		                        }
							break;
									
						case EMetaDataUserUrl:
							TRAP(err, GetUserUrlL( frameSize ));
							break;
							
						case EMetaDataDuration:
							TRAP(err, GetDurationL( frameSize ));
							break;
						
						case EMetaDataDate:
							TRAP(err, GetTextDateL( frameSize ));
							break;
							
						case EMetaDataRating:
							TRAP(err, GetRatingL( frameSize ));
							break;
						
						default:
							break;
						}
					}
				}
			}
		iFrameOffset += (frameSize + frameHeaderSize);
		}
	
	// retrieve the album art
    TRAPD (error, RetrieveAlbumArtL());
    if (error)
        {
        #ifdef _DEBUG
            RDebug::Print(_L("CMetaDataParserID3v2::ParseFramesL-2 - RetrieveAlbumArt leave"));
        #endif        
        }
 
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::RetrieveAlbumArtL
// -----------------------------------------------------------------------------
//	
void CMetaDataParserID3v2::RetrieveAlbumArtL()
    {
    // If album art exists, add to container
    if ( iAlbumPtr != NULL )
        {
        TPtr8 des( iAlbumPtr->Des() );       
        TPtrC8 pic = des.Mid(iAlbumOffset);
        TInt length = pic.Length();
        if ( length )
            {
            iContainer->AppendL( EMetaDataJpeg, pic );
            }
         
        // reset the album related variables
        iAlbumType = -1;
        iAlbumOffset = 0;
        delete iAlbumPtr;
        iAlbumPtr = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::Handlev2GetGenreL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v2::HandleV2GetGenreL( TInt aSize, TInt aKID3v2FrameHeaderLength, TInt aKID3v2GenreNameLength )
	{
	#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v2::GetGenreL"));
	#endif

	HBufC8* frame = HBufC8::NewLC( aSize );
	TPtr8 des( frame->Des() );

	iSource->ReadL( iFrameOffset + aKID3v2FrameHeaderLength, des, aSize );

	if ( des.Length() < aSize )
		{
		// Partial frame
		User::Leave( KErrCorrupt );
		}

	TInt encoding = (TInt) (des.Left(1))[0];
	TPtrC8 info = StripTrailingZeroes( des.Mid(1), encoding);
	TInt length = info.Length();

	if(length <= 0)
		{
		CleanupStack::PopAndDestroy();  // frame
		return;
		}
	else 
		{
		length = (length > aKID3v2GenreNameLength) ? length : aKID3v2GenreNameLength;
		}

	HBufC8* genreData = HBufC8::NewLC( length );
	TPtr8 temp( genreData->Des() );
	temp.Copy(info);

	if ( encoding == 0 )
		{
		   if ( temp[0] == KID3v2format )
			{
			//temp.Delete(0,1); 
			TLex8 lex(temp.Ptr() + 1);
			TInt genreID = 0;
			TInt err = lex.Val(genreID);	
			if(err != KErrNone)
				{
					CleanupStack::PopAndDestroy(2);  // genreData, frame
					return;
				}
			if(genreID >= 0 && genreID <= 125 || genreID == 199)
				{
				temp.FillZ(0);
				MapID3GenreToStringL(genreID,temp);
				}
			}
			
			if( length )
			 {
			 HBufC* data16 = HBufC::NewLC( length );
		 	 TPtr unicode( data16->Des() );
		 	 if ( ConvertToUnicodeL(encoding, temp, unicode) == KErrNone )
				{
					iContainer->AppendL( EMetaDataGenre, unicode );
				}
			 CleanupStack::PopAndDestroy(data16);  // data16
			 }
		}
	else // non ASCII 
		{
		HBufC* dataGen16 = HBufC::NewLC( length );
		TPtr unicodeGen( dataGen16->Des() );
		if ( ConvertToUnicodeL(encoding, temp, unicodeGen) == KErrNone )
			{
			if (temp.Length() <= 2)
			    {
		        CleanupStack::PopAndDestroy(3);  // dataGen16, genreData, frame
				return;			    
			    }
			temp = temp.Mid(2);
			if(temp[0] == KID3v2format)
				{
				TLex16 lex(unicodeGen.Ptr()+1);
				TInt val=0;
				TInt err = lex.Val(val);
				if(err != KErrNone)
					{
					CleanupStack::PopAndDestroy(3);  // dataGen16, genreData, frame
					return;
					}
					
				if(val >= 0 && val <= 125 || val == 199)
					{
					unicodeGen.FillZ(0);
					MapID3GenreToStringL(val,unicodeGen);
					}
				}
			iContainer->AppendL( EMetaDataGenre, unicodeGen );				
			}
		CleanupStack::PopAndDestroy(dataGen16);  // dataGen16
		}
	CleanupStack::PopAndDestroy(genreData);  // genreData
	CleanupStack::PopAndDestroy(frame);  // frame	
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::VersionL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserID3v2::VersionL(
	CMetaDataSource& aSource, TUint aOffset )
	{
	TInt size = 0;
	aSource.Size( size );
	if ( size < KID3v2HeaderLength )
		{
		// This isn't ID3v2
		return KErrNotFound;
		}

	TBuf8<KID3v2HeaderLength> header;
	aSource.ReadL( aOffset, header );
	// ID3v2 header consists of following parts:
    // - identifier "ID3", 3 bytes
    // - version, 2 bytes
    // - flags, 1 byte
    // - data length, 4 bytes

	if ( header.Left( KID3v2TagLength ).Compare( KID3v2Tag ) != 0 )
		{
		return KErrNotFound;
		}

	TInt frameSize = 0;
	for ( TInt i = 6; i < 10; i++ )
		{
		frameSize <<= 7;
		frameSize |= header[i] & 0x7f;
		}

	if ( size < ( frameSize + KID3v2HeaderLength ) )
		{
		// Partial ID3v2 tag
		User::Leave( KErrCorrupt );
		}

	return (TInt) header[3];
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::CreateCharacterSetsL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v2::CreateCharacterSetsL()
	{
	// Get list of charconv supported character sets
	iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
	iTopCharacterSet = new (ELeave) CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>(12);
	GenerateTopCharacterSetsL();
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::DetectCharacterSetL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserID3v2::DetectCharacterSetL(
	const TDesC8& aDesc)
	{
	TInt confidence = 0;
	TInt highestConfidence = 0;
	TUint charSetId;
	TUint highestConfidencecharSetId = 0;

	CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
	TInt count = iTopCharacterSet->Count();
	for ( TInt i=0; i < iTopCharacterSet->Count(); i++)
		{
		charSetId = iTopCharacterSet->At(i).Identifier();
		charSetConv->ConvertibleToCharSetL(confidence, charSetId, *iTopCharacterSet, aDesc);
		if ( confidence > highestConfidence )
			{
			highestConfidence = confidence;
			highestConfidencecharSetId = charSetId;
			}
		}
	CleanupStack::PopAndDestroy(charSetConv);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::DetectCharacterSetL :-> Confidence[%d] CharSetId[%x]"),
	        highestConfidence, highestConfidencecharSetId);
#endif

	if ( (highestConfidence >= KMinimumConfidenceRequired) || (iAutoDetectJapanese && highestConfidence >= KMinimumJapaneseConfidenceRequired ))
		{
		iCharacterSetId = highestConfidencecharSetId;
		return KErrNone;
		}
	else
		{
		return KErrNotFound;
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::AutoDetectL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserID3v2::AutoDetectL(const TDesC8& aDesc,
	TDes16& aUnicode)
{
	if ( iCharacterSetId == 0 )
		{
		if ( DetectCharacterSetL(aDesc) == KErrNotFound )
			{
			return KErrNotFound;
			}
		}

	CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
	TInt state = CCnvCharacterSetConverter::KStateDefault;
	TInt numOfUnconvertibleChars = 0;

	charSetConv->PrepareToConvertToOrFromL(iCharacterSetId, *iCharacterSet, iFs);
	TInt retVal = charSetConv->ConvertToUnicode(aUnicode, aDesc, state, numOfUnconvertibleChars);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::ConvertToUnicode :-> Tag Size[%d] Unicode Tag Size[%d]Bytes Unconverted[%d] retVal[%d]"),
		aDesc.Length(), aUnicode.Length(), numOfUnconvertibleChars, retVal);
#endif
	if ( retVal < 0 )
		{
		CleanupStack::PopAndDestroy();	// charSetConv
		return retVal;
		//return KErrGeneral;
		}

	if ( retVal > 0 || numOfUnconvertibleChars > 0 )
		{
		// This is different character set. Need to auto detect again
		if ( DetectCharacterSetL(aDesc) == KErrNotFound )
			{
			CleanupStack::PopAndDestroy();	// charSetConv
			return KErrNotFound;
			}
		state = CCnvCharacterSetConverter::KStateDefault;
		numOfUnconvertibleChars = 0;
		charSetConv->PrepareToConvertToOrFromL(iCharacterSetId, *iCharacterSet, iFs);
		retVal = charSetConv->ConvertToUnicode(aUnicode, aDesc, state, numOfUnconvertibleChars);
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::ConvertToUnicode :-> Tag Size[%d] Unicode Tag Size[%d]Bytes Unconverted[%d] retVal[%d]"),
		aDesc.Length(), aUnicode.Length(), numOfUnconvertibleChars, retVal);
#endif
        if ( retVal != 0 || numOfUnconvertibleChars > 0 )
			{
	        CleanupStack::PopAndDestroy();	// charSetConv
	        return KErrGeneral;
			}
		}
	CleanupStack::PopAndDestroy();	// charSetConv
	return KErrNone;
}



// -----------------------------------------------------------------------------
// CMetaDataParserID3v2::Version()
// -----------------------------------------------------------------------------
//
TID3Version CMetaDataParserID3v2::ID3Version()
{
	return EID3Version2;
}


//-----------------------------------------------------------------------------
// CMetaDataParserID3v2::NeedRetrieveAlbumArt( TInt aPicType )
//-----------------------------------------------------------------------------
//
TBool CMetaDataParserID3v2::NeedRetrieveAlbumArt( TInt aPicType )
{
    TBool ret = EFalse;
    
    switch (iAlbumType)
        {
        case K1stAlbumArt : 
            // return EFalse, no need to retrieve another album art
            break;
        case K2ndAlbumArt : 
            if (aPicType == K1stAlbumArt)
            {
                ret = ETrue;            
            }
            break;
        case K3rdAlbumArt : 
            if ((aPicType == K1stAlbumArt) ||
                (aPicType == K2ndAlbumArt))
            {
                ret = ETrue;            
            }
            break;
        default:
            // There is no album art, or it is a priority album art
            if ((iAlbumType == -1) ||
                (aPicType == K1stAlbumArt) ||
                (aPicType == K2ndAlbumArt) ||
                (aPicType == K3rdAlbumArt))
            {
                ret = ETrue;            
            }
            // if the old one is bad, and the new one is good, use it
            else
            {
                if ( IgnoreAlbumType(iAlbumType) && !IgnoreAlbumType(aPicType) )
                {   
                    // get the new one
                    ret = ETrue;            
                }
            }

        }
    
    return ret;
}


//-----------------------------------------------------------------------------
// CMetaDataParserID3v2::IgnoreAlbumType( TInt aPicType )
//-----------------------------------------------------------------------------
//
TBool CMetaDataParserID3v2::IgnoreAlbumType (TInt aPicType)
{
    if (aPicType == KPictureTypePixel || aPicType == KPictureTypeFileIcon)
        {
        return ETrue;
        }
   
    return EFalse;
      
}
//  End of File
