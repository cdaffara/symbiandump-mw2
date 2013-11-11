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
* Description:  This class implements an ID3v1 and v1.1 parser as specified in
*                www.id3.org.
*
*/



// INCLUDE FILES
#include	"MetaDataParserID3v1.h"
#include <centralrepository.h>
#include "MetadataUtilityCRKeys.h"
#include    <barsc.h>
#include    <barsread.h>
#include 	<data_caging_path_literals.hrh>
#include	<topcharacterset.rsg>
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// CONSTANTS
// (ID3v1 specification found in www.id3.org)
_LIT8( KID3v1TagHeader, "TAG" );
const TInt KID3v1TagHeaderLength = 3;

const TInt KOffsetTitle 		= 3;
const TInt KOffsetArtist		= 33;
const TInt KOffsetAlbum 		= 63;
const TInt KOffsetYear 			= 93;
const TInt KOffsetComment 		= 97;
const TInt KOffsetAlbumTrack 	= 125;
const TInt KOffsetGenre 		= 127;

const TInt KLengthTitle 		= 30;
const TInt KLengthArtist		= 30;
const TInt KLengthAlbum 		= 30;
const TInt KLengthYear 			= 4;
const TInt KLengthComment 		= 30;
const TInt KLengthAlbumTrack 	= 1;
const TInt KLengthGenre 		= 1;

const TInt KMinimumConfidenceRequired = 90;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::CMetaDataParserID3v1
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserID3v1::CMetaDataParserID3v1(
	CMetaDataSource& aSource )
	:	iSource(aSource),
		iCharacterSetId(0)
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::ConstructL()
    {
	iTagData.FillZ( KID3v1TagLength );
	if ( ValidateL() )
		{
		User::LeaveIfError(iFs.Connect());
		// Get list of charconv supported character sets
		iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
		iTopCharacterSet = new (ELeave) CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>(12);
		GenerateTopCharacterSetsL();
    	}
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::ConstructL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserID3v1* CMetaDataParserID3v1::NewL(
	CMetaDataSource& aSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::NewL"));
#endif
	CMetaDataParserID3v1* self = new( ELeave ) CMetaDataParserID3v1(aSource);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserID3v1::~CMetaDataParserID3v1()
	{
	if ( iExists )
		{
		delete iCharacterSet;
		delete iTopCharacterSet;
		iFs.Close();
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v1::ParseL"));
#endif
	iContainer = &aContainer;
	TInt err = KErrNone;
	if ( aWantedFields.Count() == 0 )
		{
		TRAP(err, GetArtistL()); // ignore errors, coz other entries may be 
		TRAP(err, GetTitleL()); // extracted wihout exceptions.
		TRAP(err, GetAlbumL());
		TRAP(err, GetYearL());
		TRAP(err, GetCommentL());
		TRAP(err, GetAlbumTrackL());
		TRAP(err, GetGenreL());
		}
	else
		{
		// Look for it in the wanted field array
		TInt count( aWantedFields.Count() );
		for ( TInt i = 0; i < count; i++ )
			{
			switch ( aWantedFields[ i ] )
				{
				case EMetaDataSongTitle:
					TRAP(err, GetTitleL());
					break;
				case EMetaDataArtist:
					TRAP(err, GetArtistL());
					break;
				case EMetaDataAlbum:
					TRAP(err, GetAlbumL());
					break;
				case EMetaDataYear:
					TRAP(err, GetYearL());
					break;
				case EMetaDataComment:
					TRAP(err, GetCommentL());
					break;
				case EMetaDataAlbumTrack:
					TRAP(err, GetAlbumTrackL());
					break;
				case EMetaDataGenre:
					TRAP(err, GetGenreL());
					break;
				default:
					break;
				}
			}
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserID3v1::ValidateL()
	{
	// ID3v1 tags are always 128 bytes long
	TInt size;
	User::LeaveIfError( iSource.Size( size ) );
	if ( size < KID3v1TagLength )
		{
		// This isn't ID3v1
		return EFalse;
		}

	iSource.ReadL( size - KID3v1TagLength, iTagData );
	// Search for the ID3v1 header in the tag data
	if ( iTagData.Left( KID3v1TagHeaderLength ).Compare( KID3v1TagHeader ) != 0 )
		{
		return EFalse;
		}

	iExists = ETrue;
	return ETrue;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetTitleL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetTitleL()
	{
	TPtrC8 title = StripTrailingZeroes( iTagData.Mid( KOffsetTitle, KLengthTitle ) );
	if ( title.Length() )
		{
		TBuf<KLengthTitle> titleUnicode;
		if ( ConvertToUnicodeL(title, titleUnicode) == KErrNone )
			{
			iContainer->AppendL( EMetaDataSongTitle, titleUnicode );
			}
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetArtistL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetArtistL()
	{
	TPtrC8 artist = StripTrailingZeroes( iTagData.Mid( KOffsetArtist, KLengthArtist ) );
	if ( artist.Length() )
		{
		TBuf<KLengthArtist> artistUnicode;
		if ( ConvertToUnicodeL(artist, artistUnicode) == KErrNone )
			{
			iContainer->AppendL( EMetaDataArtist, artistUnicode );
			}
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetAlbumL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetAlbumL()
	{
	TPtrC8 album = StripTrailingZeroes( iTagData.Mid( KOffsetAlbum, KLengthAlbum ) );
	if ( album.Length() )
		{
		TBuf<KLengthAlbum> albumUnicode;
		if ( ConvertToUnicodeL(album, albumUnicode) == KErrNone )
			{
			iContainer->AppendL( EMetaDataAlbum, albumUnicode );
			}
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetYearL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetYearL()
	{
	TBuf<KLengthYear> year;
	year.Copy( StripTrailingZeroes( iTagData.Mid( KOffsetYear, KLengthYear ) ) );
	if ( year.Length() )
		{
		iContainer->AppendL( EMetaDataYear, year );
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetCommentL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetCommentL()
	{
	TInt commentLength = 0;
	// To be compliant with ID3 v1.1
	if ( iTagData[KOffsetAlbumTrack] == 0 && iTagData[KOffsetAlbumTrack + 1] != 0 )
		{
		commentLength = 28;
		}
	else
		{
		commentLength = KLengthComment;
		}
	TPtrC8 comment = StripTrailingZeroes( iTagData.Mid( KOffsetComment, commentLength ) );
	if ( comment.Length() )
		{
		TBuf<KLengthComment> commentUnicode;
		if ( ConvertToUnicodeL(comment, commentUnicode) == KErrNone )
			{
			iContainer->AppendL( EMetaDataComment, commentUnicode );
			}
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetAlbumTrackL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetAlbumTrackL()
	{
	// To be compliant with ID3 v1.1
	if ( iTagData[KOffsetAlbumTrack] == 0 && iTagData[KOffsetAlbumTrack + 1] != 0 )
		{
		TBuf<3> track;	// 1 byte, but can be upto 3 bytes in when converted to ascii
		TUint num = iTagData.Mid( KOffsetAlbumTrack + 1, KLengthAlbumTrack )[0];
		track.NumUC( num );
		iContainer->AppendL( EMetaDataAlbumTrack, track );
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::GetGenreL
// -----------------------------------------------------------------------------
//
void CMetaDataParserID3v1::GetGenreL()
	{
		TUint num = iTagData.Mid( KOffsetGenre, KLengthGenre )[0];
		TBufC<KMaxGenreSize> genre;
		TPtr genrePtr = genre.Des();
		if(num <= 125 || num == 199)
			{
			MapID3GenreToStringL(num, genrePtr);
			iContainer->AppendL( EMetaDataGenre, genre );
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::ConvertToUnicodeL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserID3v1::ConvertToUnicodeL(
	const TDesC8& aDesc,
	TDes16& aUnicode )
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
        if ( retVal != 0 || numOfUnconvertibleChars > 0)
			{
	        CleanupStack::PopAndDestroy();	// charSetConv
	        return KErrGeneral;
			}
		}

	CleanupStack::PopAndDestroy();	// charSetConv
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::DetectCharacterSetL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserID3v1::DetectCharacterSetL(
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
   		confidence, iCharacterSetId);
#endif
	if ( highestConfidence < KMinimumConfidenceRequired )
		{
		return KErrNotFound;
		}
	else
		{
		iCharacterSetId = highestConfidencecharSetId;
		return KErrNone;
		}
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserID3v1::Version()
// -----------------------------------------------------------------------------
//
TID3Version CMetaDataParserID3v1::ID3Version()
{
	return EID3Version1;
}

//  End of File
