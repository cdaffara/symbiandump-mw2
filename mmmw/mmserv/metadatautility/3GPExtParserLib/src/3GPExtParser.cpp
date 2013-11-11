/* ==============================================================================
*  Name        : 3GPExtParser.cpp
*  Part of     : MetaDataUtility
*  Description : This class implements an 3GPExt metadata parser
*  Version     : 9
*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
* ==============================================================================
*/


// INCLUDE FILES
#include "3GPExtParser.h"
#include "MetaDataID3v1Genre.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// CONSTANTS

const TUint32 K3gpMetaMeta	= 0x6d657461;	// 'meta'
const TInt K3gpMetaLength = 1;


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3GPExtParser::C3GPExtParser
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3GPExtParser::C3GPExtParser(MP4Handle& aMP4Handle): iMP4Handle(aMP4Handle), ilst(NULL, 0)
	{
    }

// -----------------------------------------------------------------------------
// C3GPExtParser::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3GPExtParser::ConstructL()
    {
    K3GPExtMetaTitle.Format(K3GPExtMetaTitleFormat, 0xA9);
    K3GPExtMetaArtist.Format(K3GPExtMetaArtistFormat, 0xA9);
	K3GPExtMetaComposer.Format(K3GPExtMetaComposerFormat, 0xA9);
	K3GPExtMetaAlbum.Format(K3GPExtMetaAlbumFormat, 0xA9);
	K3GPExtMetaComment.Format(K3GPExtMetaCommentFormat, 0xA9);
	K3GPExtMetaCustomGenre.Format(K3GPExtMetaCustomGenreFormat, 0xA9);
	K3GPExtMetaYear.Format(K3GPExtMetaYearFormat, 0xA9);
    }

// -----------------------------------------------------------------------------
// C3GPExtParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C C3GPExtParser* C3GPExtParser::NewL(MP4Handle& aMP4Handle)
    {
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::NewL"));
#endif
	C3GPExtParser* self = new( ELeave ) C3GPExtParser (aMP4Handle);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
EXPORT_C C3GPExtParser::~C3GPExtParser()
	{
	if(iChunkStatus)
		{
		iChunk.Close();
		}
	}

// -----------------------------------------------------------------------------
// 3GPExtParser::GetilstBoxesL
// -----------------------------------------------------------------------------
//
EXPORT_C void C3GPExtParser::GetilstBoxesL(const TDesC8 &/*aBox*/, TMetaDataFieldId aFieldId, HBufC** aBuf)
{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::GetilstBoxesL"));
#endif
	TUint32 offset = 0;
	TUint32 size = 0;
	switch(aFieldId)
		{
			case EMetaDataSongTitle:
				offset = iTitleOffset;
				size = iTitleSize;
				break;
			case EMetaDataArtist:
				offset = iArtistOffset;
				size = iArtistSize;
				break;
			case EMetaDataAlbum:
				offset = iAlbumOffset;
				size = iAlbumSize;
				break;
			case EMetaDataComposer:
				offset = iComposerOffset;
				size = iComposerSize;
				break;
			case EMetaDataComment:
				offset = iCommentOffset;
				size = iCommentSize;
				break;
			case EMetaDataRating:
				offset = iCustomGenreOffset;
				size = iCustomGenreSize;
				break;
			case EMetaDataYear:
				offset = iYearOffset;
				size = iYearSize;
				break;
			case EMetaDataGenre:
				offset = iGenreOffset;
				size = iGenreSize;
				break;
			case EMetaDataAlbumTrack:
				offset = iTrackNumberOffset;
				size = iTrackNumberSize;
				break;
			case EMetaDataJpeg:
				offset = iCoverOffset;
				size = iCoverSize;
				break;
			default:
				break;
		
		}
		
	if(size <= 0 || size >= (KMaxTInt/2))
		{
		User::Leave(KErrNotFound); // no data
		}
		
	TInt length = ilst.Length();
	if (length < 0 || length < (offset + size))
	    {
		User::Leave(KErrNotFound); // no data
	    }
		
	// actual data ptr
	TPtrC8 data = ilst.Mid(offset, size);
	if(aFieldId != EMetaDataGenre && aFieldId != EMetaDataAlbumTrack && aFieldId != EMetaDataJpeg)
		{ // string meta data
		*aBuf = HBufC::NewL(size);
		TPtr data16Ptr( (*aBuf)->Des() );
		data16Ptr.Copy(data);
		}
	else if(aFieldId == EMetaDataGenre)
		{ // 0x XXXX ID3v1 Genre + 1
		TInt16 num = 0;
		for(TInt i = 0 ; i <= 1; i++)
			{	
			num <<= 8;
			num |= data[i];
			}
		num -= 1; // getting it to ID3
		if((num >= 0 && num <= 125) || num == 199)
			{
			*aBuf = HBufC::NewL(KMaxGenreSize);
			TPtr genrePtr = (*aBuf)->Des();
			MapID3GenreToString(num, genrePtr);
			}
		}
	else if(aFieldId == EMetaDataAlbumTrack)
		{
		*aBuf = HBufC::NewL(6);
		TUint num = 0;
		for(TInt i = 0 ; i <= 1; i++)
			{	
			num <<= 8;
			num |= data[i+2];
			}
		TBuf<6> data16;
		data16.NumUC(num);
		(**aBuf) = data16;
		}
	else if(aFieldId == EMetaDataJpeg)
		{
		TPtrC8 imageMime1 = data.Mid(6, 4);
		TPtrC8 imageMime2 = data.Mid(1, 3);
		_LIT8(KJFIF, "JFIF");
		_LIT8(KPNG, "PNG");
		if(imageMime1.Compare(KJFIF) != 0 && imageMime2.Compare(KPNG) != 0)
			{
			// only JPEG and PNG supported
			User::Leave(KErrNotFound); // no data
			}
		if(imageMime1.Compare(KJFIF) == 0)
			{
			TPtrC8 pic = data.Mid(0, size);
			TInt length = pic.Length();
			if ( length )
				{
				*aBuf = HBufC::NewL( length );
				TPtr unicode( (*aBuf)->Des() );
				unicode.Copy( pic );
				}
			}
		else if(imageMime2.Compare(KPNG) == 0)
			{
			TPtrC8 pic = data.Mid(0, size);
			TInt length = pic.Length();
			if ( length )
				{
				*aBuf = HBufC::NewL( length );
				TPtr unicode( (*aBuf)->Des() );
				unicode.Copy( pic );
				}	
			}
		}
}
// -----------------------------------------------------------------------------
// 3GPExtParser::GetilstBoxesL for 8-bit descriptor fields 
// -----------------------------------------------------------------------------
//
EXPORT_C void C3GPExtParser::GetilstBoxesL(const TDesC8 &/*aBox*/, TMetaDataFieldId aFieldId, HBufC8** aBuf)
{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::GetilstBoxesL 8 bit version"));
#endif
	TUint32 offset = 0;
	TUint32 size = 0;
	switch(aFieldId)
		{
			case EMetaDataSongTitle:
				offset = iTitleOffset;
				size = iTitleSize;
				break;
			case EMetaDataArtist:
				offset = iArtistOffset;
				size = iArtistSize;
				break;
			case EMetaDataAlbum:
				offset = iAlbumOffset;
				size = iAlbumSize;
				break;
			case EMetaDataComposer:
				offset = iComposerOffset;
				size = iComposerSize;
				break;
			case EMetaDataComment:
				offset = iCommentOffset;
				size = iCommentSize;
				break;
			case EMetaDataRating:
				offset = iCustomGenreOffset;
				size = iCustomGenreSize;
				break;
			case EMetaDataYear:
				offset = iYearOffset;
				size = iYearSize;
				break;
			case EMetaDataGenre:
				offset = iGenreOffset;
				size = iGenreSize;
				break;
			case EMetaDataAlbumTrack:
				offset = iTrackNumberOffset;
				size = iTrackNumberSize;
				break;
			case EMetaDataJpeg:
				offset = iCoverOffset;
				size = iCoverSize;
				break;
			default:
				break;
		
		}
		
	if(size <= 0 || size >= (KMaxTInt/2))
		{
		User::Leave(KErrNotFound); // no data
		}
		
	TInt length = ilst.Length();
	if (length < 0 || length < (offset + size))
	    {
		User::Leave(KErrNotFound); // no data
	    }
		
	// actual data ptr
	TPtrC8 data = ilst.Mid(offset, size);
	if(aFieldId != EMetaDataGenre && aFieldId != EMetaDataAlbumTrack && aFieldId != EMetaDataJpeg)
		{ // string meta data
		*aBuf = HBufC8::NewL(size);
		TPtr8 unicode( (*aBuf)->Des() );
		unicode.Copy(data);
		}
	if(aFieldId == EMetaDataJpeg)
		{
		TPtrC8 imageMime1 = data.Mid(6, 4);
		TPtrC8 imageMime2 = data.Mid(1, 3);
		_LIT8(KJFIF, "JFIF");
		_LIT8(KPNG, "PNG");
		if(imageMime1.Compare(KJFIF) != 0 && imageMime2.Compare(KPNG) != 0)
			{
			// only JPEG and PNG supported
			User::Leave(KErrNotFound); // no data
			}
		if(imageMime1.Compare(KJFIF) == 0)
			{
			TPtrC8 pic = data.Mid(0, size);
			TInt length = pic.Length();
			if ( length )
				{
				*aBuf = HBufC8::NewL( length );
				TPtr8 unicode( (*aBuf)->Des() );
				unicode.Copy( pic );
				}
			}
		else if(imageMime2.Compare(KPNG) == 0)
			{
			TPtrC8 pic = data.Mid(0, size);
			TInt length = pic.Length();
			if ( length )
				{
				*aBuf = HBufC8::NewL( length );
				TPtr8 unicode( (*aBuf)->Des() );
				unicode.Copy( pic );
				}	
			}
		}
}

// -----------------------------------------------------------------------------
// C3GPExtParser::Is3GPExtMetadataL
// -----------------------------------------------------------------------------
//
EXPORT_C TBool C3GPExtParser::Is3GPExtMetadataL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::3GPExtMetadataL"));
#endif
	HBufC8* frame = HBufC8::NewLC( K3gpMetaLength );
	TUint8* buffer = CONST_CAST(TUint8*, frame->Ptr());

	MP4Err err;
	mp4_u8 udtaLocation = MP4_UDTA_MOOV;
	mp4_u32 bufferSize = K3gpMetaLength;
	mp4_u32 atomIndex = 0;

	err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, K3gpMetaMeta, buffer,
			bufferSize, atomIndex);
	if (bufferSize > (KMaxTInt / 2))
	    {
		User::Leave(KErrNotFound); // no data	    
	    }
	    
	if ( err == MP4_UDTA_NOT_FOUND )
		{
		if ( udtaLocation == MP4_UDTA_NONE || udtaLocation == MP4_UDTA_MOOV )
			{
#ifdef _DEBUG
	RDebug::Print(_L("3GPExtParser::Parse3GPExtL - NotFound"));
#endif
			CleanupStack::PopAndDestroy();  // frame
			return EFalse;
			}
		else
			{
			if ( udtaLocation & MP4_UDTA_AUDIOTRAK )
				{
				udtaLocation = MP4_UDTA_AUDIOTRAK;
				}
			else 	// MP4_UDTA_VIDEOTRAK
				{
				udtaLocation = MP4_UDTA_VIDEOTRAK;
				}
			}
#ifdef _DEBUG
	RDebug::Print(_L("3GPExtParser::Parse3GPExtL - New Location [%d]"), udtaLocation);
#endif
		err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, K3gpMetaMeta, buffer,
			bufferSize, atomIndex);
		}

	if ( err != MP4_OK )
		{
		if ( err == MP4_OUTPUT_BUFFER_TOO_SMALL )
			{
			iChunk.Close();
	
			User::LeaveIfError(iChunk.CreateLocal(bufferSize, bufferSize, EOwnerProcess));
			iChunkStatus = ETrue;
			buffer = iChunk.Base();
			err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, K3gpMetaMeta, buffer, bufferSize, atomIndex);
			if(err != MP4_OK)
				{
				CleanupStack::PopAndDestroy();  // frame
				return EFalse;
				}
			}
		else
			{
			CleanupStack::PopAndDestroy();  // frame
			return EFalse;
			//User::Leave( TranslateMP4Err(err) );
			}
		}

	TPtr8 des(buffer, bufferSize, bufferSize);
	
	// HDLR BOX
	_LIT8(KHdlr, "hdlr");
	TInt hdlrOffset = 16; // header box at constant offset
	TPtrC8 hdlrCheck = des.MidTPtr(hdlrOffset, 4);
	if(hdlrCheck !=  KHdlr)
	{ 
		CleanupStack::PopAndDestroy();  // frame
		return EFalse; // not 3GPExt
	}
	hdlrOffset -= 4; // hdlr box begins at size, 4 bytes back
	
	// get hdlr size
	TPtrC8 sizeHdlrDesc = des.Mid(hdlrOffset, 4);
	TInt32 sizeHdlr = 0;
	for(TInt i = 0 ; i <= 3; i++)
		{	
		sizeHdlr <<= 8;
		sizeHdlr |= sizeHdlrDesc[i];
		}
	if(sizeHdlr <= 0)
	    {
		CleanupStack::PopAndDestroy();  // frame
		return EFalse; // not 3GPExt
	    }
	
	// hdlr ptr
	if (des.Length() < (hdlrOffset + sizeHdlr))
	    {
	    CleanupStack::PopAndDestroy();  // frame
	    return EFalse;  // data corrupted
	    }	        
	TPtrC8 hdlr = des.Mid(hdlrOffset, sizeHdlr);
	
	// match mdirappl
	/**** Comment out the following lines - no need to test for "mdirappl" string	
	TPtrC8 text = hdlr.Mid(16, 8); // at 16 bytes from hdlr box
	_LIT8(KMdirappl, "mdirappl");
	if(text.Compare(KMdirappl) != 0)
	{
		CleanupStack::PopAndDestroy();  	// frame
		return EFalse; 						// not 3GPExt
	}
	*****/   // End comment out "mdirappl" 	

	// iLST BOX
	_LIT8(KIlst, "ilst");
	TInt ilstOffset = hdlrOffset + sizeHdlr + 4;
	if (ilstOffset > bufferSize)
	    {
		CleanupStack::PopAndDestroy();  // frame
		return EFalse; // not 3GPExt	    
	    }
	    
	TPtrC8 ilstCheck = des.MidTPtr(ilstOffset, 4);
	if(ilstCheck !=  KIlst)
	    {
	    ilstOffset = des.Find(KIlst);
	    if(ilstOffset == KErrNotFound)
	        {
	        CleanupStack::PopAndDestroy();  // frame
	        return EFalse; // no list box
	        }
	    }
	ilstOffset -=4; // ilst box begins at size, 4 bytes back
	
	// get ilst size
	TPtrC8 sizeIlstDesc = des.Mid(ilstOffset, 4);
	TUint32 sizeIlst = 0;
	for(TInt i = 0 ; i <= 3; i++)
		{	
		sizeIlst <<= 8;
		sizeIlst |= sizeIlstDesc[i];
		}
	if(sizeIlst <= 0 || sizeIlst > bufferSize - ilstOffset)
	{
		CleanupStack::PopAndDestroy();  // frame
		return EFalse; // no list box
	}
	
	// ilst 
	TPtrC8 tmp = des.Mid(ilstOffset, sizeIlst);
	const TUint8* tmpPtr = tmp.Ptr();
	ilst.Set(tmpPtr, sizeIlst);
	iExists = ETrue;
	
	ParseIlstBoxesL();
	CleanupStack::PopAndDestroy();  // frame	
	return ETrue;
	}

// -----------------------------------------------------------------------------
// 3GPExtParser::MapID3v1GenreToString()
// -----------------------------------------------------------------------------
//
void C3GPExtParser::ParseIlstBoxesL()
{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::ParseIlstBoxesL"));
#endif

	TUint32 sizeIlst = ilst.Length();
	TUint32 offset = 8; // first tag
	while (offset < sizeIlst)	
		{
		TUint32 tagSize = 0;
		TPtrC8 tagSizeDes = ilst.Mid(offset, 4);
		for(TInt i = 0 ; i <= 3; i++)
			{	
			tagSize <<= 8;
			tagSize |= tagSizeDes[i];
			}
		TPtrC8 tagName = ilst.Mid(offset + 4, 4);
		TUint32 dataSize = 0;
		TPtrC8 dataSizeDesc = ilst.Mid(offset + 8, 4);
		for(TInt i = 0 ; i <= 3; i++)
			{	
			dataSize <<= 8;
			dataSize |= dataSizeDesc[i];
			}
		if(dataSize > 16)
			{
			if(!tagName.Compare(K3GPExtMetaTitle))
				{ 
				iTitleOffset = offset + 8 + 16;
				iTitleSize = dataSize - 16;
				}
			else if(!tagName.Compare(K3GPExtMetaArtist))
				{
				iArtistOffset = offset + 8 + 16;
				iArtistSize = dataSize - 16;
				}
			else if(!tagName.Compare(K3GPExtMetaComposer))
				{
				iComposerOffset = offset + 8 + 16;
				iComposerSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaAlbum))
				{
				iAlbumOffset = offset + 8 + 16;
				iAlbumSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaComment))
				{
				iCommentOffset = offset + 8 + 16;
				iCommentSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaYear))
				{
				iYearOffset = offset + 8 + 16;
				iYearSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaCustomGenre))
				{
				iCustomGenreOffset = offset + 8 + 16;
				iCustomGenreSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaGenre))
				{
				iGenreOffset = offset + 8 + 16;
				iGenreSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaTrack))
				{
				iTrackNumberOffset = offset + 8 + 16;
				iTrackNumberSize = dataSize - 16;
				}	
			else if(!tagName.Compare(K3GPExtMetaJpeg))
				{
				iCoverOffset = offset + 8 + 16;
				iCoverSize = dataSize - 16;
				}	
			else // default
				{
				// unwanted tag, ignore
				}
			}
		if (tagSize <= 0)
		    {
		    #ifdef _DEBUG
	            RDebug::Print(_L("C3GPExtParser::ParseIlstBoxesL - break"));
            #endif

		    break;
		    }
		else
		    {
			offset += tagSize;
		    }
		}
}

// -----------------------------------------------------------------------------
// 3GPExtParser::MapID3v1GenreToString()
// -----------------------------------------------------------------------------
//
void C3GPExtParser::MapID3GenreToString(TInt aNum, TDes& aGenrePtr)
{
#ifdef _DEBUG
	RDebug::Print(_L("C3GPExtParser::MapID3GenreToString"));
#endif

	switch(aNum)
			{
				case 0:
					aGenrePtr.Append(KGenreBlues);
					break;
				case 1:
					aGenrePtr.Append(KGenreClassicRock);
					break;
				case 2:
					aGenrePtr.Append(KGenreCountry);
					break;
				case 3:
					aGenrePtr.Append(KGenreDance);
					break;
				case 4:
					aGenrePtr.Append(KGenreDisco);
					break;
				case 5:
					aGenrePtr.Append(KGenreFunk);
					break;
				case 6:
					aGenrePtr.Append(KGenreGrunge);
					break;
				case 7:
					aGenrePtr.Append(KGenreHipHop);
					break;
				case 8:
					aGenrePtr.Append(KGenreJazz);
					break;
				case 9:
					aGenrePtr.Append(KGenreMetal);
					break;
				case 10:
					aGenrePtr.Append(KGenreNewAge);
					break;
				case 11:
					aGenrePtr.Append(KGenreOldies);
					break;
				case 12:
					aGenrePtr.Append(KGenreOther);
					break;
				case 13:
					aGenrePtr.Append(KGenrePop);
					break;
				case 14:
					aGenrePtr.Append(KGenreRB);
					break;
				case 15:
					aGenrePtr.Append(KGenreRap);
					break;
				case 16:
					aGenrePtr.Append(KGenreReggae);
					break;
				case 17:
					aGenrePtr.Append(KGenreRock);
					break;
				case 18:
					aGenrePtr.Append(KGenreTechno);
					break;
				case 19:
					aGenrePtr.Append(KGenreIndustrial);
					break;
				case 20:
					aGenrePtr.Append(KGenreAlternative);
					break;
				case 21:
					aGenrePtr.Append(KGenreSka);
					break;
				case 22:
					aGenrePtr.Append(KGenreDeathMetal);
					break;
				case 23:
					aGenrePtr.Append(KGenrePranks);
					break;
				case 24:
					aGenrePtr.Append(KGenreSoundtrack);
					break;
				case 25:
					aGenrePtr.Append(KGenreEuroTechno);
					break;
				case 26:
					aGenrePtr.Append(KGenreAmbient);
					break;
				case 27:
					aGenrePtr.Append(KGenreTripHop);
					break;
				case 28:
					aGenrePtr.Append(KGenreVocal);
					break;
				case 29:
					aGenrePtr.Append(KGenreJazzFunk);
					break;
				case 30:
					aGenrePtr.Append(KGenreFusion);
					break;
				case 31:
					aGenrePtr.Append(KGenreTrance);
					break;
				case 32:
					aGenrePtr.Append(KGenreClassical);
					break;
				case 33:
					aGenrePtr.Append(KGenreInstrumental);
					break;
				case 34:
					aGenrePtr.Append(KGenreAcid);
					break;
				case 35:
					aGenrePtr.Append(KGenreHouse);
					break;
				case 36:
					aGenrePtr.Append(KGenreGame);
					break;
				case 37:
					aGenrePtr.Append(KGenreSoundClip);
					break;
				case 38:
					aGenrePtr.Append(KGenreGospel);
					break;
				case 39:
					aGenrePtr.Append(KGenreNoise);
					break;
				case 40:
					aGenrePtr.Append(KGenreAlternRock);
					break;
				case 41:
					aGenrePtr.Append(KGenreBass);
					break;
				case 42:
					aGenrePtr.Append(KGenreSoul);
					break;
				case 43:
					aGenrePtr.Append(KGenrePunk);
					break;
				case 44:
					aGenrePtr.Append(KGenreSpace);
					break;
				case 45:
					aGenrePtr.Append(KGenreMeditative);
					break;
				case 46:
					aGenrePtr.Append(KGenreInstrumentalPop);
					break;
				case 47:
					aGenrePtr.Append(KGenreInstrumentalRock);
					break;
				case 48:
					aGenrePtr.Append(KGenreEthnic);
					break;
				case 49:
					aGenrePtr.Append(KGenreGothic);
					break;
				case 50:
					aGenrePtr.Append(KGenreDarkwave);
					break;
				case 51:
					aGenrePtr.Append(KGenreTechnoIndustrial);
					break;
				case 52:
					aGenrePtr.Append(KGenreElectronic);
					break;
				case 53:
					aGenrePtr.Append(KGenrePopFolk);
					break;
				case 54:
					aGenrePtr.Append(KGenreEurodance);
					break;
				case 55:
					aGenrePtr.Append(KGenreDream);
					break;
				case 56:
					aGenrePtr.Append(KGenreSouthernRock);
					break;
				case 57:
					aGenrePtr.Append(KGenreComedy);
					break;
				case 58:
					aGenrePtr.Append(KGenreCult);
					break;
				case 59:
					aGenrePtr.Append(KGenreGangsta);
					break;
				case 60:
					aGenrePtr.Append(KGenreTop40);
					break;
				case 61:
					aGenrePtr.Append(KGenreChristianRap);
					break;
				case 62:
					aGenrePtr.Append(KGenrePopFunk);
					break;
				case 63:
					aGenrePtr.Append(KGenreJungle);
					break;
				case 64:
					aGenrePtr.Append(KGenreNativeAmerican);
					break;
				case 65:
					aGenrePtr.Append(KGenreCabaret);
					break;
				case 66:
					aGenrePtr.Append(KGenreNewWave);
					break;
				case 67:
					aGenrePtr.Append(KGenrePsychadelic);
					break;
				case 68:
					aGenrePtr.Append(KGenreRave);
					break;
				case 69:
					aGenrePtr.Append(KGenreShowtunes);
					break;
				case 70:
					aGenrePtr.Append(KGenreTrailer);
					break;
				case 71:
					aGenrePtr.Append(KGenreLoFi);
					break;
				case 72:
					aGenrePtr.Append(KGenreTribal);
					break;
				case 73:
					aGenrePtr.Append(KGenreAcidPunk);
					break;
				case 74:
					aGenrePtr.Append(KGenreAcidJazz);
					break;
				case 75:
					aGenrePtr.Append(KGenrePolka);
					break;
				case 76:
					aGenrePtr.Append(KGenreRetro);
					break;
				case 77:
					aGenrePtr.Append(KGenreMusical);
					break;
				case 78:
					aGenrePtr.Append(KGenreRockRoll);
					break;
				case 79:
					aGenrePtr.Append(KGenreHardRock);
					break;
				case 80:
					aGenrePtr.Append(KGenreFolk);
					break;
				case 81:
					aGenrePtr.Append(KGenreFolkRock);
					break;
				case 82:
					aGenrePtr.Append(KGenreNationalFolk);
					break;
				case 83:
					aGenrePtr.Append(KGenreSwing);
					break;
				case 84:
					aGenrePtr.Append(KGenreFastFusion);
					break;
				case 85:
					aGenrePtr.Append(KGenreBebob);
					break;
				case 86:
					aGenrePtr.Append(KGenreLatin);
					break;
				case 87:
					aGenrePtr.Append(KGenreRevival);
					break;
				case 88:
					aGenrePtr.Append(KGenreCeltic);
					break;
				case 89:
					aGenrePtr.Append(KGenreBluegrass);
					break;
				case 90:
					aGenrePtr.Append(KGenreAvantgarde);
					break;
				case 91:
					aGenrePtr.Append(KGenreGothicRock);
					break;
				case 92:
					aGenrePtr.Append(KGenreProgressiveRock);
					break;
				case 93:
					aGenrePtr.Append(KGenrePsychedelicRock);
					break;
				case 94:
					aGenrePtr.Append(KGenreSymphonicRock);
					break;
				case 95:
					aGenrePtr.Append(KGenreSlowRock);
					break;
				case 96:
					aGenrePtr.Append(KGenreBigBand);
					break;
				case 97:
					aGenrePtr.Append(KGenreChorus);
					break;
				case 98:
					aGenrePtr.Append(KGenreEasyListening);
					break;
				case 99:
					aGenrePtr.Append(KGenreAcoustic);
					break;
				case 100:
					aGenrePtr.Append(KGenreHumour);
					break;
				case 101:
					aGenrePtr.Append(KGenreSpeech);
					break;
				case 102:
					aGenrePtr.Append(KGenreChanson);
					break;
				case 103:
					aGenrePtr.Append(KGenreOpera);
					break;
				case 104:
					aGenrePtr.Append(KGenreChamberMusic);
					break;
				case 105:
					aGenrePtr.Append(KGenreSonata);
					break;
				case 106:
					aGenrePtr.Append(KGenreSymphony);
					break;
				case 107:
					aGenrePtr.Append(KGenreBootyBass);
					break;
				case 108:
					aGenrePtr.Append(KGenrePrimus);
					break;
				case 109:
					aGenrePtr.Append(KGenrePornGroove);
					break;
				case 110:
					aGenrePtr.Append(KGenreSatire);
					break;
				case 111:
					aGenrePtr.Append(KGenreSlowJam);
					break;
				case 112:
					aGenrePtr.Append(KGenreClub);
					break;
				case 113:
					aGenrePtr.Append(KGenreTango);
					break;
				case 114:
					aGenrePtr.Append(KGenreSamba);
					break;
				case 115:
					aGenrePtr.Append(KGenreFolklore);
					break;
				case 116:
					aGenrePtr.Append(KGenreBallad);
					break;
				case 117:
					aGenrePtr.Append(KGenrePowerBallad);
					break;
				case 118:
					aGenrePtr.Append(KGenreRhythmicSoul);
					break;
				case 119:
					aGenrePtr.Append(KGenreFreestyle);
					break;
				case 120:
					aGenrePtr.Append(KGenreDuet);
					break;
				case 121:
					aGenrePtr.Append(KGenrePunkRock);
					break;
				case 122:
					aGenrePtr.Append(KGenreDrumSolo);
					break;
				case 123:
					aGenrePtr.Append(KGenreAcapella);
					break;
				case 124:
					aGenrePtr.Append(KGenreEuroHouse);
					break;
				case 125:
					aGenrePtr.Append(KGenreDanceHall);
					break;
				case 199:
					aGenrePtr.Append(KGenrePodcast);
					break;
				default:
					break;
			}
	}
		
			
// -----------------------------------------------------------------------------
// 3GPExtParser::TranslateMP4Err
// -----------------------------------------------------------------------------
//
TInt C3GPExtParser::TranslateMP4Err(MP4Err aError)
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParserMP4::TranslateMP4Err [%d]"), aError);
#endif
	TInt err;
	switch ( aError )
		{
		case MP4_OK:
			err = KErrNone;
			break;
		case MP4_OUT_OF_MEMORY:
			err = KErrNoMemory;
			break;
		case MP4_NOT_AVAILABLE:
			err = KErrNotReady;
			break;
		case MP4_FILE_ERROR:
			err = KErrBadHandle;
			break;
		case MP4_INVALID_TYPE:
			err = KErrNotSupported;
			break;
		case MP4_TIMESCALE_NOT_SET:
			err = KErrNotReady;
			break;
		case MP4_NOT_STREAMABLE:
		case MP4_NO_REQUESTED_FRAME:
		case MP4_CANT_SEEK:
		case MP4_INVALID_INPUT_STREAM:
		case MP4_NO_FRAME:
			err = KErrArgument;
			break;
		case MP4_ERROR:
		case MP4_FILE_MODE:
		case MP4_BUFFER_TOO_SMALL:
		case MP4_END_OF_VIDEO:
		case MP4_METADATA_ERROR:
		case MP4_NO_VIDEO:
		case MP4_NO_AUDIO:
			err = KErrGeneral;
			break;
		default:
			err = KErrGeneral;
			break;
		}
	return err;
	}
//  End of File
