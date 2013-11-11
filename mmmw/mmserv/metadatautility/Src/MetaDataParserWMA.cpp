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
* Description:  This class implements a wma parser 
*
*/




// INCLUDE FILES
#include	"MetaDataParserWMA.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

#include <stdlib.h>
#include <s32mem.h>

// CONSTANTS
/***
 *  File Properties Objects:
 *  Object ID,          GUID,   128 [bits]
 *  Object Size,        QWORD,  64 
 *  File ID,            GUID,   128 
 *  File Size,          QWORD,  64
 *  Creation Date,      QWORD,  64
 *  Data Packets Count, QWORD,  64
 *  Play Duration,      QWORD,  64
 *  Send Duration,      QWORD,  64
 *  Preroll,            QWORD,  64
 *  Flags,              DWORD,  32 
 */
const TInt KDurationOffset  = 64;       // duration offset in File Property object
const TInt KPrerollOffset   = 80;       // preRoll offset in File Property object
const TInt KFlagsOffset     = 88;       // flags offset in File Property object
const TInt KSampleRateOffset = 82;      // sample rate offset in stream properties object
const TInt KBitRateOffset   = 86;       // bit rate offset in stream properties object


// ASF Header Object GUIDs 
  
_LIT8 (KASFContentDescriptionObject, "75B22633668E11CFA6D900AA0062CE6C");
_LIT8 (KASFExtendedContentDescriptionObject, "D2D0A440E30711D297F000A0C95EA850");
_LIT8 (KASFHeaderObject, "75B22630668E11CFA6D900AA0062CE6C");
_LIT8 (KASFFilePropertiesObject, "8CABDCA1A94711CF8EE400C00C205365");
_LIT8 (KASFHeaderExtensionObject, "5FBF03B5A92E11CF8EE300C00C205365");
_LIT8 (KASFMetadataLibraryObject, "44231C94949849D1A1411D134E457054");
_LIT(KWMAlbumTitle, "WM/AlbumTitle\0");
_LIT(KWMPicture,	"WM/Picture\0");
_LIT(KWMText,		"WM/Text\0");
_LIT(KWMComposer,	"WM/Composer\0");
_LIT(KWMGenre,		"WM/Genre\0");
_LIT(KWMYear,		"WM/Year\0");
_LIT(KWMYear1,		"WM/OriginalReleaseYear\0"); 
_LIT(KWMOriginalArtist,	"WM/OriginalArtist\0");
_LIT(KWMTrackNumber,	"WM/TrackNumber\0");
_LIT(KWMUniqueFileIdentifier,	"WM/UniqueFileIdentifier\0");
_LIT(KWMAudioFileURL,	"WM/AudioFileURL\0");
_LIT(KWMSharedUserRating, "WM/SharedUserRating\0");
_LIT(KWMDate, "WM/OriginalReleaseTime\0");
_LIT(KWMAlbumArtist, "WM/AlbumArtist\0");

#ifdef __WINDOWS_MEDIA
_LIT(KWMProvider, "WM/Provider\0");
#endif
_LIT8 (KASFStreamPropertiesObject, "B7DC0791A9B711CF8EE600C00C205365");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::CMetaDataParserWMA
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParserWMA::CMetaDataParserWMA(
	CMetaDataSource& aSource )
	:	iSource(aSource),
		iCharacterSetId(0)
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ConstructL()
    {
	if ( ValidateL() )
		{
		User::LeaveIfError(iFs.Connect());
		// Get list of charconv supported character sets
		iCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL(iFs);
		}
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserWMA::ConstructL - Done"));
#endif
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParserWMA* CMetaDataParserWMA::NewL(
	CMetaDataSource& aSource )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserWMA::NewL"));
#endif
	CMetaDataParserWMA* self = new( ELeave ) CMetaDataParserWMA(aSource);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParserWMA::~CMetaDataParserWMA()
    {
    delete iHeaderData;
    delete iCharacterSet;
    iFs.Close();
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserWMA::ParseL"));
#endif
	iContainer = &aContainer;

	if(iContentDescriptionObjectExists)
		{
		ParseContentDescriptionObject();
		}
	if(iExtendedContentDescriptionObjectExists)
		{
		ParseExtendedContentDescriptionObjectL();
		}
	if(iHeaderExtensionObjectExists)
		{
		ParseHeaderExtensionObjectL();
		if(iMetadataLibraryObjectExists)
			{
			ParseMetadataLibraryObjectL();
			}
		}	
	TInt err = KErrNone; // ignore err, as some entry may be extracted without exception
	if ( aWantedFields.Count() == 0 )
		{
		TRAP(err, GetTitleL());
		TRAP(err, GetAuthorL());
		TRAP(err, GetCopyrightL());
		TRAP(err, GetCommentL());
		TRAP(err, GetJpegL());
		TRAP(err, GetExtContDesEntryL(EMetaDataRating, iSharedUserRatingOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataAlbum, iAlbumTitleOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataComment, iTextOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataComposer, iComposerOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataGenre, iGenreOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataYear, iYearOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataOriginalArtist, iOriginalArtistOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataAlbumTrack, iTrackNumberOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataUniqueFileIdentifier, iUniqueFileIdentifierOffset));
		TRAP(err, GetExtContDesEntryL(EMetaDataUrl, iAudioFileURLOffset));
        TRAP(err, GetExtContDesEntryL(EMetaDataAlbumArtist, iAlbumArtistOffset));
		TRAP(err, GetDurationL());
		TRAP(err, GetExtContDesEntryL(EMetaDataDate, iDateOffset));
	#ifdef __WINDOWS_MEDIA
		TRAP(err, GetExtContDesEntryL(EMetaDataVendor, iProviderOffset));
	#endif
		TRAP(err, GetSampleRateL());
		TRAP(err, GetBitRateL());
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
					TRAP(err, GetAuthorL());
					break;
				case EMetaDataCopyright:
					TRAP(err, GetCopyrightL());
					break;
				case EMetaDataRating:
					TRAP(err, GetExtContDesEntryL(EMetaDataRating, iSharedUserRatingOffset));
					break;
				case EMetaDataJpeg:
					TRAP(err, GetJpegL());
					break;
				case EMetaDataAlbum:
					TRAP(err, GetExtContDesEntryL(EMetaDataAlbum, iAlbumTitleOffset));
					break;
				case EMetaDataComment:
					TRAP(err, GetExtContDesEntryL(EMetaDataComment, iTextOffset));
					break;
				case EMetaDataComposer:
					TRAP(err, GetExtContDesEntryL(EMetaDataComposer, iComposerOffset));
					break;
				case EMetaDataGenre:
					TRAP(err, GetExtContDesEntryL(EMetaDataGenre, iGenreOffset));
					break;
				case EMetaDataYear:
					TRAP(err, GetExtContDesEntryL(EMetaDataYear, iYearOffset));
					break;
				case EMetaDataOriginalArtist:
					TRAP(err, GetExtContDesEntryL(EMetaDataOriginalArtist, iOriginalArtistOffset));
					break;
				case EMetaDataAlbumTrack:
					TRAP(err, GetExtContDesEntryL(EMetaDataAlbumTrack, iTrackNumberOffset));
					break;
				case EMetaDataUniqueFileIdentifier:
					TRAP(err, GetExtContDesEntryL(EMetaDataUniqueFileIdentifier, iUniqueFileIdentifierOffset));
					break;
				case EMetaDataUrl:
					TRAP(err, GetExtContDesEntryL(EMetaDataUrl, iAudioFileURLOffset));
					break;
				case EMetaDataDuration:
					TRAP(err, GetDurationL());
					break;
				case EMetaDataDate:
					TRAP(err, GetExtContDesEntryL(EMetaDataDate, iDateOffset));
					break;
                case EMetaDataAlbumArtist:
                    TRAP(err, GetExtContDesEntryL(EMetaDataAlbumArtist, iAlbumArtistOffset));
					break;
			#ifdef __WINDOWS_MEDIA
				case EMetaDataVendor:
					TRAP(err, GetExtContDesEntryL(EMetaDataVendor, iProviderOffset));
			#endif
				case EMetaDataSampleRate:
					TRAP(err, GetSampleRateL());
					break;
				case EMetaDataBitRate:
					TRAP(err, GetBitRateL());
					break;
				default:
					break;
				}
			}
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserWMA::ValidateL()
	{
	TInt size;
	iExists = EFalse;
	User::LeaveIfError( iSource.Size( size ) );
	if ( size <= 0 )
		{
		return EFalse;
		}

	// ASF_Header_Object GUID 128 bits.
	TBuf8<32> header; 
	iSource.ReadL(header, 16);
	if(header.Length() < 16)
		{
		return EFalse;
		}
	FormatGUID(header);
	if(header !=  KASFHeaderObject)
		{
		return EFalse;
		}
	// read header object size.
	iSource.ReadL(header, 8);
	TInt headerSize = ConvertToInt64(header);
	if(headerSize <= 30)
		{
		return EFalse; // header object is empty.
		}
	if(headerSize >= size) //Header Size is too high
		{
		return EFalse; // header size is more than the file size.
		}
	// read header object
	// 2~31 = 2 GB, size of header would not be greater than this,
	// also, HBufC does not have a NewL with TInt64 as arguement.
	iHeaderData = HBufC8::NewL(headerSize); 
	TPtr8 headerPtr = iHeaderData->Des(); 
	iSource.ReadL(headerPtr, headerSize - 24);
	
	TBuf8<4> objects = iHeaderData->Mid(0, 4);
	TInt noOfObjects = ConvertToInt32(objects);
	if(noOfObjects <= 0)
		{
		iExists = EFalse;
		return EFalse; // no objects. 
		}
        
	TInt objOffset = 6;
	if(objOffset+16 > iHeaderData->Length()) //Header Size is too small
		{
		return EFalse;
		} 
	TBuf8<32> objGUID = iHeaderData->Mid(objOffset, 16);
	FormatGUID(objGUID);
	TBool loop = ETrue;
	TInt objectCount = 0;
	while (loop)
		{
		if(!iContentDescriptionObjectExists && objGUID == KASFContentDescriptionObject)
			{
			iContentDescriptionObjectExists = ETrue;
			iContentDescriptionOffset = objOffset;
			}
		if(!iFilePropertiesObjectExists && objGUID == KASFFilePropertiesObject)
			{
			iFilePropertiesObjectExists = ETrue; // must exist
			iFilePropertiesOffset = objOffset;
			}
		if(!iExtendedContentDescriptionObjectExists && objGUID == 
			KASFExtendedContentDescriptionObject)
			{
			iExtendedContentDescriptionObjectExists = ETrue;
			iExtendedContentDescriptionOffset = objOffset;
			}
		if(!iHeaderExtensionObjectExists && objGUID == 
			KASFHeaderExtensionObject)
			{
			iHeaderExtensionObjectExists = ETrue;
			iHeaderExtensionOffset = objOffset;
			}	
		if(!iStreamPropertiesObjectExists && objGUID == 
			KASFStreamPropertiesObject)
			{
			iStreamPropertiesObjectExists = ETrue;
			iStreamPropertiesOffset = objOffset;
			}	
		TBuf8<8> size = iHeaderData->Mid(objOffset + 16, 8); 
		TInt objSize = ConvertToInt64(size); // upper 32 bits?
		if(0 > objSize)
			{
			return EFalse;
			}
		objOffset += objSize;
		if(objOffset >= headerSize - 30 || 
			(iContentDescriptionObjectExists && iFilePropertiesObjectExists 
			&& iExtendedContentDescriptionObjectExists && iHeaderExtensionObjectExists 
			&& iStreamPropertiesObjectExists) )
			{
			loop = EFalse;
			}
		else
			{
			objGUID = iHeaderData->Mid(objOffset, 16);
			FormatGUID(objGUID);
			objectCount++;

			if (objectCount == noOfObjects)
				{
				iExists = EFalse;
				return EFalse;	// gone through all objects		
				}
			}
		}
	if(iFilePropertiesObjectExists) 
		{
		iExists = ETrue;
		}
	return iExists;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::FormatGUID
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::FormatGUID(TDes8 &aGUID)
    {
	TBuf8<16> copyGUID(aGUID);
	TInt i;
	for(i = 0; i < 4; i++)
		{
		copyGUID[i] = aGUID[3-i];
		}
	for(i = 4; i < 6; i++)
		{
		copyGUID[i] = aGUID[9 - i];
		}
	for (i = 6; i < 8; i++)
		{
		copyGUID[i] = aGUID[13 - i];
		}
	for(i = 8; i < 16 ; i++)
		{
		copyGUID[i] = aGUID[i];
		}
	aGUID.Delete(0, 32);
	for(i = 0; i <16; i++)
		{
        aGUID.AppendNumFixedWidthUC(copyGUID[i], EHex, 2);
		}
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConvertToInt64
// -----------------------------------------------------------------------------
//
TInt64 CMetaDataParserWMA::ConvertToInt64(TDesC8& aDes)
    {
	TInt64 num = 0;
	TInt i;
	for(i = 7 ; i >= 0; i--)
		{
		num <<= 8;
		num |= aDes[i];
		}
	return num;
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConvertToInt32
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserWMA::ConvertToInt32(TDesC8& aDes)
    {
	TInt num = 0;
	for(TInt i = 3 ; i >= 0; i--)
		{	
		num <<= 8;
		num |= aDes[i];
		}
	return num;
    }


// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConvertToInt16
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserWMA::ConvertToInt16(TDesC8& aDes)
    {
	TInt num = 0;
	for(TInt i = 1 ; i >= 0; i--)
		{	
		num <<= 8;
		num |= aDes[i];
		}
	return num;
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConvertDes8toDes16
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ConvertDes8toDes16(const TDesC8& aDes8,TDes16& aDes16)
	{
	for(TInt i = 0 ; i < aDes8.Length(); i+=2)
		{
		if(aDes16.MaxLength() <= i/2)
			{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserWMA::ConvertDes8toDes16: i/2[%d] >= aDes16's MaxLength[%d]."),
			i/2, aDes16.MaxLength());
#endif
			return;
			}
		aDes16.Append(aDes8[i]);
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetTitleL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetTitleL()
	{
	if (iTitleLength <= 0)
		{
		return;
		}
	TInt offset = iContentDescriptionOffset + 34;
	if(iTitleLength - 2 <= 0 || offset + iTitleLength > iHeaderData->Length())
		{
		return; // empty title or corrupt title length
		}
	TPtrC8 title = iHeaderData->Mid(offset, iTitleLength - 2);
	HBufC* data16 = HBufC::NewLC( iTitleLength - 2);
	TPtr titleUnicode( data16->Des() );
	if ( ConvertToUnicodeL(title, titleUnicode) == KErrNone )
		{
		iContainer->AppendL( EMetaDataSongTitle, titleUnicode );
		}
	CleanupStack::PopAndDestroy();  // data16
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetArtistL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetAuthorL()
	{
	if (iAuthorLength <= 0)
		{
		return;
		}
	TInt offset = iContentDescriptionOffset + 34 + iTitleLength;
	if(iAuthorLength - 2 <= 0 || offset + iAuthorLength > iHeaderData->Length())
		{
		return; // empty author or corrupt author length
		}
	TPtrC8 author = iHeaderData->Mid(offset, iAuthorLength - 2);
	HBufC* data16 = HBufC::NewLC( iAuthorLength - 2);
	TPtr authorUnicode( data16->Des() );
	if ( ConvertToUnicodeL(author, authorUnicode) == KErrNone )
		{
		iContainer->AppendL( EMetaDataArtist, authorUnicode );
		}
	CleanupStack::PopAndDestroy();  // data16
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetCopyrightL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetCopyrightL()
	{
	if (iCopyrightLength <= 0)
		{
		return;
		}
	TInt offset = iContentDescriptionOffset + 34 + iTitleLength + iAuthorLength;
	if(iCopyrightLength - 2 <= 0 || offset + iCopyrightLength > iHeaderData->Length())
		{
		return; // empty or corrupt length
		}
	TPtrC8 copyright = iHeaderData->Mid(offset, iCopyrightLength - 2);
	HBufC* data16 = HBufC::NewLC( iCopyrightLength - 2);
	TPtr copyrightUnicode( data16->Des() );
	if ( ConvertToUnicodeL(copyright, copyrightUnicode) == KErrNone )
		{
		iContainer->AppendL( EMetaDataCopyright, copyrightUnicode );
		}
	CleanupStack::PopAndDestroy();  // data16
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetCommentL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetCommentL()
	{
	if (iDescriptionLength <= 0)
		{
		return;
		}
	TInt offset = iContentDescriptionOffset + 34 + iTitleLength + iAuthorLength + iCopyrightLength;
	if(iDescriptionLength - 2 <= 0 || offset + iDescriptionLength > iHeaderData->Length())
		{
		return; // empty comment or corrupt comment length
		}
	TPtrC8 comments = iHeaderData->Mid(offset, iDescriptionLength - 2);
	HBufC* data16 = HBufC::NewLC( iDescriptionLength - 2);
	TPtr commentsUnicode( data16->Des() );
	if ( ConvertToUnicodeL(comments, commentsUnicode) == KErrNone )
		{
		iContainer->AppendL( EMetaDataComment, commentsUnicode );
		}
	CleanupStack::PopAndDestroy();  // data16
	}


// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetJpegL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetJpegL()
    {
	if (iPictureOffset <= 0)
	    {
        return;
        }
	
	TInt offset = iPictureOffset;
	if(!iMetadatLibraryObjectJpegExists)
	    {
		TPtrC8 dataType = iHeaderData->Mid(offset, 2);
		offset += 2;
		TInt dataTypeInt = ConvertToInt16(dataType);
		if(dataTypeInt != 1)
			{
			return;
			}
		
		TPtrC8 lengthData = iHeaderData->Mid(offset, 2);
		offset += 2;
		TInt length	= ConvertToInt16(lengthData);
		if(length <= 0)
			{
			return;
			}		
		}
	TPtrC8 picData = iHeaderData->Mid(offset, 1);
	offset += 1;
	TInt picType = 0;
	picType |= picData[0];
	if(picType != 3)
	    {
		return; // only Front Album Cover supported
        }
	
	TPtrC8 picLengthData = iHeaderData->Mid(offset, 4);
	offset += 4;
	TInt picLength = ConvertToInt32(picLengthData);
	if(picLength <= 0)
	    {
		return;
        }
	
	_LIT8(KNULL, "\0\0");
	TPtrC8 data = iHeaderData->Mid(offset, picLength);
	TInt pos = data.Find(KNULL);
	if(pos != 0)
	    {
		pos++; // for unicode coding for strings. 
        }
	// check mime type
	if(pos != KErrNotFound)
	    {
		HBufC8 *mimeType = iHeaderData->Mid(offset, pos + 2).AllocLC();
		offset += pos + 2;
		HBufC* name16 = HBufC::NewLC( (pos+2)/2);
		TPtr mimeType16( name16->Des() );
		ConvertDes8toDes16(*mimeType, mimeType16);
		_LIT(KJPEG, "image/jpeg\0");
		_LIT(KJPG, "image/jpg\0");
		if(mimeType16.Compare(KJPEG) != 0 && mimeType16.Compare(KJPG) != 0)
		    {
			CleanupStack::PopAndDestroy(2, mimeType);
			return; // only JPEG & JPG supported
		    }
		CleanupStack::PopAndDestroy(2); // mimeType16, mimeType
        }
		
	// skip the picture description
	TPtrC8 picDesc = iHeaderData->Mid(offset, picLength);
	pos = picDesc.Find(KNULL);
	if(pos != 0)
	    {
		pos++; // for unicode coding for strings. 
        }
	offset += pos + 2;
	
	// picture data 
	TPtrC8 pic8 = iHeaderData->Mid(offset, picLength);	
	iContainer->AppendL( EMetaDataJpeg, pic8 );
    }


// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetExtContDesEntryL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParserWMA::GetExtContDesEntryL(TMetaDataFieldId aFieldId, TInt aOffset)
    {
	TBool ret = EFalse;
	if(iExtendedContentDescriptionCount == 0)
		{
		return ret;
		}
	TPtrC8 dataType = iHeaderData->Mid(aOffset, 2);
	TInt dataTypeInt = ConvertToInt16(dataType);
	if(dataTypeInt == 0x00)
		{
		TPtrC8 lengthData = iHeaderData->Mid(aOffset + 2, 2);
		TInt length	= ConvertToInt16(lengthData);
		if(length <= 0)
			{
			return ret;
			}
		TPtrC8 data = iHeaderData->Mid(aOffset + 4, length - 2);
		HBufC* data16 = HBufC::NewLC(length - 2);
		TPtr dataUnicode( data16->Des() );
		if ( ConvertToUnicodeL(data, dataUnicode) == KErrNone )
			{
			iContainer->AppendL( aFieldId, dataUnicode );
			ret = ETrue;
			}
		CleanupStack::PopAndDestroy();  // data16
		}
	else if(dataTypeInt == 0x03)
		{
		TPtrC8 lengthData = iHeaderData->Mid(aOffset + 2, 2);
		TInt length	= ConvertToInt16(lengthData);
		if(length != 4)
			{
			return ret;
			}
		TPtrC8 data = iHeaderData->Mid(aOffset + 4, length);
		TInt dword = ConvertToInt32(data);
		const TInt KMaxIntBufferSize = 10;
		HBufC* data16 = HBufC::NewLC(KMaxIntBufferSize);
		TPtr dataString( data16->Des() );
		dataString.Num(dword);
		iContainer->AppendL( aFieldId, dataString );
		CleanupStack::PopAndDestroy();  // data16
		ret = ETrue;
		}
	return ret;
    }

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetDurationL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetDurationL()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetDuration()"));
#endif    
    TInt offset = iFilePropertiesOffset + KFlagsOffset;
    TPtrC8 flags = iHeaderData->Mid(offset, 4);
    TInt broadcastBit = (TInt) (flags[0] & 0x01);
    if(broadcastBit == 1)
        {
        return; // duration not valid.
        }
    
    offset = iFilePropertiesOffset + KDurationOffset;
    TPtrC8 duration8 = iHeaderData->Mid(offset, 8);     // 100 nanosec units
    TReal durationSec = ((TReal)ConvertToInt64(duration8)) / 10000000; // seconds
    
    offset = iFilePropertiesOffset + KPrerollOffset;
    TPtrC8 preRoll8 = iHeaderData->Mid(offset, 8);      // millisec units
    TReal preRollSec = ((TReal)ConvertToInt64(preRoll8)) / 1000; // seconds
    TReal sec = durationSec - preRollSec;               // not include preroll

    TBuf16<10> des16;	
    TRealFormat format(9, 3);                           // width=9, decimal place=3
    // Use fixed format, and do not use Triads
    format.iType = KRealFormatFixed | KDoNotUseTriads; 
    des16.Num(sec, format);	                            // convert to string
	
	iContainer->AppendL( EMetaDataDuration, des16 );
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetDuration(), duration=%f"), sec);
#endif 	
	
	}



// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ConvertToUnicodeL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParserWMA::ConvertToUnicodeL(
	const TDesC8& aDesc,
	TDes16& aUnicode )
	{
	TPtrC8 unicodeData;
	TUint characterSetId = 0;
	CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
	TInt state = CCnvCharacterSetConverter::KStateDefault;

	// ASF (WMA) file format has UTF-16 LittleEndian characters.
	characterSetId = KCharacterSetIdentifierUnicodeLittle;
	unicodeData.Set( aDesc );
	
	charSetConv->PrepareToConvertToOrFromL(characterSetId, *iCharacterSet, iFs);
	TInt err = charSetConv->ConvertToUnicode(aUnicode, unicodeData, state);

#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserID3v24::ConvertToUnicode :-> Tag Size[%d] Unicode Tag Size[%d]Bytes Unconverted[%d]"),
		unicodeData.Length(), aUnicode.Length(), err);
#endif

	CleanupStack::PopAndDestroy();	// charSetConv
	return err;
	}


// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ParseContentDescriptionObject
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ParseContentDescriptionObject()
	{
	TBuf8<2> data = iHeaderData->Mid(iContentDescriptionOffset + 24, 2);
	iTitleLength = ConvertToInt16(data); 
	data = iHeaderData->Mid(iContentDescriptionOffset + 26, 2);
	iAuthorLength = ConvertToInt16(data); 
	data = iHeaderData->Mid(iContentDescriptionOffset + 28, 2);
	iCopyrightLength = ConvertToInt16(data); 
	data = iHeaderData->Mid(iContentDescriptionOffset + 30, 2);
	iDescriptionLength = ConvertToInt16(data); 
	data = iHeaderData->Mid(iContentDescriptionOffset + 32, 2);
	iRatingLength = ConvertToInt16(data);
	return;
	}


// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ParseExtendedContentDescriptionObjectL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ParseExtendedContentDescriptionObjectL()
	{
	TBuf8<2> data  =  iHeaderData->Mid(iExtendedContentDescriptionOffset + 24, 2);
	iExtendedContentDescriptionCount = ConvertToInt16(data);
	TInt offset = iExtendedContentDescriptionOffset + 26;
	for(TInt i = 0 ; i < iExtendedContentDescriptionCount; i ++)
		{
		TBuf8<2> data1  =  iHeaderData->Mid(offset, 2);
		offset += 2;
		TInt nameLength = ConvertToInt16(data1);
		if(nameLength > 0 && iHeaderData->Length() >= (offset+nameLength))
			{
			HBufC8 *name = iHeaderData->Mid(offset, nameLength).AllocLC();
			offset += nameLength;
			HBufC* data16 = HBufC::NewLC(nameLength/2 == 0 ? 1 : nameLength/2);
			TPtr name16( data16->Des() );
			ConvertDes8toDes16(*name, name16);
			
			if(!name16.Compare(KWMAlbumTitle))
				{
				iAlbumTitleOffset = offset; 
				}
			else if(!name16.Compare(KWMPicture))
				{
				iPictureOffset = offset; 
				}
			else if(!name16.Compare(KWMText))
				{
				iTextOffset = offset; 
				}
			else if(!name16.Compare(KWMComposer))
				{
				iComposerOffset = offset; 
				}
			else if(!name16.Compare(KWMGenre))
				{
				iGenreOffset = offset; 
				}
			else if(!name16.Compare(KWMYear) || !name16.Compare(KWMYear1))
				{
				iYearOffset = offset; 
				}
			else if(!name16.Compare(KWMOriginalArtist))
				{
				iOriginalArtistOffset = offset; 
				}
			else if(!name16.Compare(KWMTrackNumber))
				{
				iTrackNumberOffset = offset; 
				}
			else if(!name16.Compare(KWMUniqueFileIdentifier))
				{
				iUniqueFileIdentifierOffset = offset; 
				}
			else if(!name16.Compare(KWMAudioFileURL))
				{
				iAudioFileURLOffset = offset; 
				}
			else if(!name16.Compare(KWMSharedUserRating))
				{
				iSharedUserRatingOffset = offset; 
				}
			else if(!name16.Compare(KWMDate))
				{
				iDateOffset = offset; 
				}
            else if(!name16.Compare(KWMAlbumArtist))
                {
                iAlbumArtistOffset = offset; 
                }
		#ifdef __WINDOWS_MEDIA
			else if(!name16.Compare(KWMProvider))
				{
				iProviderOffset = offset; 
				}
		#endif

			offset += 2;
			TBuf8<2> valueLengthData  =  iHeaderData->Mid(offset, 2);
			offset += 2;
			TInt valueLength = ConvertToInt16(valueLengthData);
			offset += valueLength;
			CleanupStack::PopAndDestroy(2); //data16, name
			if(iHeaderData->Length() < offset)
				{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserWMA::ParseExtendedContentDescriptionObjectL: offset[%d] > iHeaderData's Length[%d]."),
			offset, iHeaderData->Length());
#endif
				return;
				}
			}
		}
	}

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ParseMetadataLibraryObjectL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ParseMetadataLibraryObjectL()
	{
	TBuf8<8> objectSizedata  =  iHeaderData->Mid(iMetadataLibraryOffset + 16, 8);
	iMetadataLibraryObjectSize = ConvertToInt64(objectSizedata);
	TBuf8<2> recordsCountdata  =  iHeaderData->Mid(iMetadataLibraryOffset + 24, 2);
	iMetadataLibraryDescriptionCount = ConvertToInt16(recordsCountdata);
	TInt offset = iMetadataLibraryOffset + 26;
	for(TInt i = 0 ; i < iMetadataLibraryDescriptionCount; i ++)
		{
		offset += 4; // skipping LanguageID (2 Bytes) and StreamID (2 Bytes) fields
		//Reading Name Length 
		TBuf8<2> nameLengthdata  =  iHeaderData->Mid(offset, 2);
		offset += 2;
		TInt nameLength = ConvertToInt16(nameLengthdata);
		TBuf8<2> dataTypedata  =  iHeaderData->Mid(offset, 2);
		offset += 2;
		TInt dataType = ConvertToInt16(dataTypedata);
		TBuf8<4> dataLengthdata  =  iHeaderData->Mid(offset, 4);
		offset += 4;
		TInt32 dataLength = ConvertToInt32(dataLengthdata);
		
		if(nameLength > 0)
			{
			HBufC8 *name = iHeaderData->Mid(offset, nameLength).AllocLC();
			offset += nameLength;
			HBufC* data16 = HBufC::NewLC( nameLength/2);
			TPtr name16( data16->Des() );
			ConvertDes8toDes16(*name, name16);
			if(!name16.Compare(KWMPicture))
				{
				iPictureOffset = offset;
				iMetadatLibraryObjectJpegExists = ETrue; 
				}
			offset += dataLength;
			CleanupStack::PopAndDestroy(2); //data16, name
			}
		}
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParserWMA::ParseHeaderExtensionObjectL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::ParseHeaderExtensionObjectL()
	{
	TInt length = iHeaderData->Length();
    if ( iHeaderExtensionOffset + 46 > length)
        {
        return; 
        }
        
	TBuf8<8> objectSizedata  =  iHeaderData->Mid(iHeaderExtensionOffset + 16, 8);
	iHeaderExtensionObjectSize = ConvertToInt64(objectSizedata);
	TBuf8<4> headerExtensiondata  =  iHeaderData->Mid(iHeaderExtensionOffset + 42, 4);
	
	TInt32 headerExtensionDataSize = ConvertToInt32(headerExtensiondata);
	if(headerExtensionDataSize != iHeaderExtensionObjectSize - 46)
	{
		return;
	}
	TInt objOffset = iHeaderExtensionOffset + 46;
	
    if ((objOffset < 0) || (objOffset + 16 > length))   // verify objOffset is not negative
        {
        return; 
        }	
	TBuf8<32> objGUID = iHeaderData->Mid(objOffset, 16);
	FormatGUID(objGUID);
	TBool loop = ETrue;
	while (loop)
		{
		if(!iMetadataLibraryObjectExists && objGUID == 
			KASFMetadataLibraryObject)
			{
			iMetadataLibraryObjectExists = ETrue;
			iMetadataLibraryOffset = objOffset;
			}
				
        if (objOffset + 24 > length)
            {
            return; 
            }	
			
		TBuf8<8> size = iHeaderData->Mid(objOffset + 16, 8); 
		TInt objSize = ConvertToInt64(size); // upper 32 bits?
		if(objSize == 0)
			{
			return;
			}
		objOffset += objSize;
		
        if ((objOffset < 0) || (objOffset + 16 > length))
            {
            return; 
            }				
		objGUID = iHeaderData->Mid(objOffset, 16);
		FormatGUID(objGUID);
		TInt val = objOffset - iHeaderExtensionOffset; 
		if((val > 0) && (val < iHeaderExtensionObjectSize))
			{
			loop = ETrue;
			}
		else
			{
			loop = EFalse;
			}
		}
	}	

// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetSampleRateL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetSampleRateL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetSampleRateL"));
#endif    
    if(!iStreamPropertiesObjectExists)
        {
        return;
        }
    TInt offset = iStreamPropertiesOffset + KSampleRateOffset;
    TPtrC8 sampleRate = iHeaderData->Mid(offset, 4);
    if(offset+4 > iHeaderData->Length())        //Header Size is too small
        {
        return ;
        } 
    
    TInt sampleRateValue = ConvertToInt32(sampleRate); 
    TBuf16<20> des16;   
    des16.Num(sampleRateValue);                 // convert to string
    
    iContainer->AppendL( EMetaDataSampleRate, des16 );
    
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetSampleRateL(), SampleRate=%S"), &des16);
#endif      
    }
	
// -----------------------------------------------------------------------------
// CMetaDataParserWMA::GetBitRateL
// -----------------------------------------------------------------------------
//
void CMetaDataParserWMA::GetBitRateL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetBitRateL"));
#endif    
    if(!iStreamPropertiesObjectExists)
	    {
        return;
	    }
    TInt offset = iStreamPropertiesOffset + KBitRateOffset;
    if(offset+4 > iHeaderData->Length()) //Header Size is too small
        {
        return ;
        } 
    TPtrC8 byteRate = iHeaderData->Mid(offset, 4);      // byte rate

    TInt bitRateValue = ConvertToInt32(byteRate) * 8;  // multiply by 8 to get bit rate
    TBuf16<20> des16;   
    des16.Num(bitRateValue);                           // convert to string
    
    iContainer->AppendL( EMetaDataBitRate, des16 );
#ifdef _DEBUG
    RDebug::Print(_L("CMetaDataParserWMA::GetBitRateL(), bitRate=%S"), &des16);
#endif    
    }

//  End of File
