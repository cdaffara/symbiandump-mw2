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
* Description:  This class implements an 3gp metadata parser as specified in
*                www.3gpp.org (specification 3GPP TS 26.244 V6.0.0).
*
*/



// INCLUDE FILES
#include <centralrepository.h>
#include "MetadataUtilityCRKeys.h"
#include	"MetaDataParser3gp.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// CONSTANTS
// ('udta'-box pecification found in www.3gpp.org, 3GPP TS 26.244)
const TUint32 K3gpMetaTitle 		= 0x7469746c; 	// 'titl'
const TUint32 K3gpMetaDescription 	= 0x64736370;	// 'dscp'
const TUint32 K3gpMetaCopyright		= 0x63707274;	// 'cprt'
const TUint32 K3gpMetaPerformer		= 0x70657266;	// 'perf'
const TUint32 K3gpMetaAuthor		= 0x61757468;	// 'auth'
const TUint32 K3gpMetaGenre			= 0x676e7265;	// 'gnre'
const TUint32 K3gpMetaRating		= 0x72746e67;	// 'rtng'
const TUint32 K3gpMetaAlbum			= 0x616c626d;	// 'albm' 
const TUint32 K3gpMetaYear			= 0x79727263;	// 'yrrc'



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParser3gp::CMetaDataParser3gp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParser3gp::CMetaDataParser3gp()
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParser3gp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParser3gp::ConstructL()
    {
    // Check for Cenrep key
	CRepository *metadataRepository = CRepository::NewL(KCRUidMetadataUtility);
	TInt err = KErrNone;
	iVFKK = EFalse;
	err = metadataRepository->Get(KMetadataUtilityVFKKSpecificMapping, iVFKK);
	if(err)
		{
		iVFKK = EFalse;	
		}
	delete metadataRepository;
    }

// -----------------------------------------------------------------------------
// CMetaDataParser3gp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParser3gp* CMetaDataParser3gp::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::NewL"));
#endif
	CMetaDataParser3gp* self = new( ELeave ) CMetaDataParser3gp;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParser3gp::~CMetaDataParser3gp()
	{
	
	}

// -----------------------------------------------------------------------------
// CMetaDataParser3gp::ParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParser3gp::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields,
	CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::ParseL"));
#endif
	iContainer = &aContainer;
	TInt err = KErrNone; // ignore err, as some entry may be extracted without exception
	if ( aWantedFields.Count() == 0 )
		{
		TRAP(err, GetAssetBoxL(K3gpMetaTitle, EMetaDataSongTitle));
		TRAP(err, GetAssetBoxL(K3gpMetaDescription, EMetaDataComment));
		TRAP(err, GetAssetBoxL(K3gpMetaCopyright, EMetaDataCopyright));
		if(iVFKK)
			{
			TRAP(err, GetAssetBoxL(K3gpMetaAuthor, EMetaDataArtist));	
			}
		else // as per the spec
			{
			TRAP(err, GetAssetBoxL(K3gpMetaPerformer, EMetaDataArtist));
			}
		TRAP(err, GetAssetBoxL(K3gpMetaAuthor, EMetaDataComposer));	
		TRAP(err, GetAssetBoxL(K3gpMetaGenre, EMetaDataGenre));
		TRAP(err, GetDurationL(EMetaDataDuration));
		TRAP(err, GetAssetBoxL(K3gpMetaRating, EMetaDataRating));
		TRAP(err, GetAssetBoxL(K3gpMetaAlbum, EMetaDataAlbum));
		TRAP(err, GetAssetBoxL(K3gpMetaAlbum, EMetaDataAlbumTrack));
		TRAP(err, GetAssetBoxL(K3gpMetaYear, EMetaDataYear));
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
					TRAP(err, GetAssetBoxL(K3gpMetaTitle, EMetaDataSongTitle));
					break;
				case EMetaDataComment:
					TRAP(err, GetAssetBoxL(K3gpMetaDescription, EMetaDataComment));
					break;
				case EMetaDataCopyright:
					TRAP(err, GetAssetBoxL(K3gpMetaCopyright, EMetaDataCopyright));
					break;
				case EMetaDataArtist:
					if(iVFKK)
						{
						TRAP(err, GetAssetBoxL(K3gpMetaAuthor, EMetaDataArtist));	
						}
					else // as per the spec
						{
						TRAP(err, GetAssetBoxL(K3gpMetaPerformer, EMetaDataArtist));
						}
					break;
				case EMetaDataComposer:
					TRAP(err, GetAssetBoxL(K3gpMetaAuthor, EMetaDataComposer));
					break;
				case EMetaDataGenre:
					TRAP(err, GetAssetBoxL(K3gpMetaGenre, EMetaDataGenre));
					break;
				case EMetaDataDuration:
					TRAP(err, GetDurationL(EMetaDataDuration));
					break;
				case EMetaDataRating:
					TRAP(err, GetAssetBoxL(K3gpMetaRating, EMetaDataRating));
					break;
				case EMetaDataAlbum:
					TRAP(err, GetAssetBoxL(K3gpMetaAlbum, EMetaDataAlbum));
					break;
				case EMetaDataAlbumTrack:
					TRAP(err, GetAssetBoxL(K3gpMetaAlbum, EMetaDataAlbumTrack));
					break;
				case EMetaDataYear:
					TRAP(err, GetAssetBoxL(K3gpMetaYear, EMetaDataYear));
					break;
				default:
					break;
				}
			}
		}
	
	//check inline ID32 metadata
    TRAP_IGNORE( GetID32L() );
 	}

// -----------------------------------------------------------------------------
// CMetaDataParser3gp::GetAssetBoxL
// -----------------------------------------------------------------------------
//
void CMetaDataParser3gp::GetAssetBoxL(
	TUint32 aBoxType,
	TMetaDataFieldId aFieldId )
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::GetAssetBoxL [%d]"), aBoxType);
#endif

	HBufC8* frame = HBufC8::NewLC( K3gpMetaLength );
	TUint8* buffer = CONST_CAST(TUint8*, frame->Ptr());

	MP4Err err;
	mp4_u8 udtaLocation = MP4_UDTA_MOOV;
	mp4_u32 bufferSize = K3gpMetaLength;
	mp4_u32 atomIndex = 0;

	err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, aBoxType, buffer,
			bufferSize, atomIndex);
	if ( err == MP4_UDTA_NOT_FOUND )
		{
		if ( udtaLocation == MP4_UDTA_NONE || udtaLocation == MP4_UDTA_MOOV )
			{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::GetAssetBoxL - NotFound"));
#endif
			CleanupStack::PopAndDestroy();  // frame
			return;
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
	RDebug::Print(_L("CMetaDataParser3gp::GetAssetBoxL - New Location [%d]"), udtaLocation);
#endif
		err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, aBoxType, buffer,
			bufferSize, atomIndex);
		}

	if ( err != MP4_OK )
		{
		if ( err == MP4_OUTPUT_BUFFER_TOO_SMALL )
			{
			CleanupStack::PopAndDestroy();  // frame
			frame = NULL;
			frame = HBufC8::NewLC( bufferSize );
			buffer = CONST_CAST(TUint8*, frame->Ptr());
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::GetAssetBoxL - Buffer re-alloc"));
#endif
			err = MP4ParseGetUserDataAtom(iMP4Handle, udtaLocation, aBoxType, buffer,
					bufferSize, atomIndex);
			User::LeaveIfError(TranslateMP4Err(err));
			}
		else
			{
			CleanupStack::PopAndDestroy();  // frame
			return;
			//User::Leave( TranslateMP4Err(err) );
			}
		}

#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3gp::GetAssetBoxL - Found"));
#endif

	TPtr8 des(buffer, bufferSize, bufferSize);
	// Skip 12 bytes for year - refer to standard
	if(aFieldId == EMetaDataYear)
		{
		des = des.MidTPtr(12);
		if(des.Length() < 2)
			{
			// corrpupt tag
			CleanupStack::PopAndDestroy();  // frame
			return;
			}
		TBuf8<2> yearData;
		yearData.Copy(des.MidTPtr(0, 2));
		TUint yearInt = 0; 
		for(TInt i = 0; i < 2; i++)
			{
			yearInt <<= 8;
			yearInt |= yearData[i]; 
			}
		TBuf<5> year;
		year.AppendNum(yearInt);
		iContainer->AppendL( EMetaDataYear, year );
		CleanupStack::PopAndDestroy();  // frame
		return;
		}
	// Skip 14 bytes for other tags - refer to standard
	des = des.MidTPtr(14);
	if(aFieldId == EMetaDataRating)
		{
		// Skip 8 more bytes for rtng - refer to standard
		des = des.MidTPtr(8); 
		}
	if(aFieldId == EMetaDataAlbumTrack)
		{
		// check if album track exists
		if(des[des.Length() - 1] != '\0')
			{
			TBuf8<1> trackData;
			trackData.Copy(des.MidTPtr(des.Length() - 1,1));
			TUint trackInt = 0;
			trackInt |= trackData[0]; 
			TBuf<3> track;
			track.AppendNum(trackInt);
			iContainer->AppendL( EMetaDataAlbumTrack, track );
			CleanupStack::PopAndDestroy();  // frame
			return;
			}
		}
	if(aFieldId == EMetaDataAlbum)
		{
		// check if album track exists
		if(des[des.Length() - 1] != '\0')
			{
			des = des.MidTPtr(0,des.Length() - 2);
			}
		}
	TInt length = des.Length();
	if ( length )
		{
		HBufC* data16 = HBufC::NewLC( length );
		TPtr unicode( data16->Des() );
		if ( ConvertToUnicodeL(des, unicode) == KErrNone )
			{
			iContainer->AppendL( aFieldId, unicode );
			}
		CleanupStack::PopAndDestroy();  // data16
		}

	CleanupStack::PopAndDestroy();  // frame
	}

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParser3gp::ValidateL()
	{
	mp4_u32 audioLength, audioType, timeScale, averateBitRate;
	mp4_u8 framesPerSample;

	MP4Err err;
	err = MP4ParseRequestAudioDescription(iMP4Handle, &audioLength, &audioType,
			&framesPerSample, &timeScale, &averateBitRate);
	if ( err == MP4_OK )
		{
		iExists = ETrue;
		return ETrue;
		}

	return EFalse;
	}

// -----------------------------------------------------------------------------
// CMetaDataParserMP4::GetID32L
// -----------------------------------------------------------------------------
//
void CMetaDataParser3gp::GetID32L()
    {
    MP4Err err = MP4ParseGetID32Location( iMP4Handle, iID32Offset );
    User::LeaveIfError(TranslateMP4Err(err));
    }


//  End of File
