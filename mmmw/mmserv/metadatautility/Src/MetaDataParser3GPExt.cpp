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
* Description:  This class implements an 3GPExt metadata parser
*
*/



// INCLUDE FILES
#include	"MetaDataParser3GPExt.h"
#include	"3GPExtParser.h"
#ifdef _DEBUG
#include	<e32svr.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::CMetaDataParser3GPExt
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParser3GPExt::CMetaDataParser3GPExt()
	{
    }

// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMetaDataParser3GPExt::ConstructL()
    {
    i3GPExtParser = C3GPExtParser::NewL(iMP4Handle);
    }

// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMetaDataParser3GPExt* CMetaDataParser3GPExt::NewL()
    {
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3GPExt::NewL"));
#endif
	CMetaDataParser3GPExt* self = new( ELeave ) CMetaDataParser3GPExt;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CMetaDataParser3GPExt::~CMetaDataParser3GPExt()
	{
	delete i3GPExtParser;
	}

// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::ConvertAndAppendL
// -----------------------------------------------------------------------------
void CMetaDataParser3GPExt::ConvertAndAppendL(
    HBufC8** aBuf8, TMetaDataFieldId aFieldId)
	{
    CleanupStack::PushL(*aBuf8);
	if ((*aBuf8) && (*aBuf8)->Length() > 0)
        {
        TInt length = (*aBuf8)->Length();
        HBufC* data16 = HBufC::NewLC(length);
        TPtr16 titleUnicode(data16->Des());
        TPtrC8 title((*aBuf8)->Des());
        if ( ConvertToUnicodeL(title, titleUnicode) == KErrNone )
            {
            iContainer->AppendL(aFieldId, data16->Des());
            }
        CleanupStack::PopAndDestroy(data16);  // data16
        }
    CleanupStack::PopAndDestroy(*aBuf8);   // *aBuf8
    (*aBuf8) = NULL;
    }


// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::ParseL
// -----------------------------------------------------------------------------
void CMetaDataParser3GPExt::ParseL(
	const RArray<TMetaDataFieldId>& aWantedFields, 
	CMetaDataFieldContainer& aContainer)
	{
	
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3GPExt::ParseL"));
#endif
	iContainer = &aContainer;
	TInt err = KErrNone; // ignore err as some entry may be extracted without exception
	HBufC* buf = NULL;
	HBufC8* buf8 = NULL; 
	
	if ( aWantedFields.Count() == 0 )
        {
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaTitleFormat, EMetaDataSongTitle, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataSongTitle);
            }

        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaArtistFormat, EMetaDataArtist, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataArtist);
            }
		
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaComposerFormat, EMetaDataComposer, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataComposer);
            }

        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaAlbumFormat, EMetaDataAlbum, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataAlbum);
            }
            
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaCommentFormat, EMetaDataComment, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataComment);
            }

        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaGenre, EMetaDataGenre, &buf));
        if (KErrNone == err)
            {
            AppendL(&buf, EMetaDataGenre);
            }
		
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaYearFormat, EMetaDataYear, &buf));
        if (KErrNone == err)
            {
            AppendL(&buf, EMetaDataYear);
            }

        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaCustomGenreFormat, EMetaDataRating, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataGenre);
            }
			
		TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaTrack, EMetaDataAlbumTrack, &buf));
        if (KErrNone == err)
            {
            AppendL(&buf, EMetaDataAlbumTrack);
            }

        TRAP(err, GetDurationL(EMetaDataDuration));
		
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaArtistFormat, EMetaDataAlbumArtist, &buf8));
        if (KErrNone == err)
            {
            ConvertAndAppendL(&buf8, EMetaDataAlbumArtist);
            }
        
        TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaJpeg, EMetaDataJpeg, &buf8));
        if (KErrNone == err)
            {
            AppendL(&buf8, EMetaDataJpeg);
            }
		}
				
    else
        {
        // Look in the wanted field array
        TInt count( aWantedFields.Count() );
        for ( TInt i = 0; i < count; i++ )
            {
            switch ( aWantedFields[ i ] )
                {
                case EMetaDataSongTitle:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaTitleFormat, EMetaDataSongTitle, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataSongTitle);
                        }					
                    break;
					
                case EMetaDataComment:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaCommentFormat, EMetaDataComment, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataComment);
                        }					
                    break;
					
                case EMetaDataAlbumTrack:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaTrack, EMetaDataAlbumTrack, &buf));
                    if (KErrNone == err)
                        {
                        AppendL(&buf, EMetaDataAlbumTrack);
                        }					
                    break;
					
                case EMetaDataArtist:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaArtistFormat, EMetaDataArtist, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataArtist);
                        }					
                    break;
					
                case EMetaDataComposer:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaComposerFormat, EMetaDataComposer, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataComposer);
                        }					
                    break;
					
                case EMetaDataAlbum:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaAlbumFormat, EMetaDataAlbum, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataAlbum);
                        }					
                    break;
					
                case EMetaDataGenre:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaGenre, EMetaDataGenre, &buf));
                    if (KErrNone == err)
                        {
                        AppendL(&buf, EMetaDataGenre);
                        }					
                    break;
					
                case EMetaDataDuration:
                    TRAP(err, GetDurationL(EMetaDataDuration));
                    break;
					
                case EMetaDataYear:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaYearFormat, EMetaDataYear, &buf));
                    if (KErrNone == err)
                        {
                        AppendL(&buf, EMetaDataYear);
                        }					
                    break;
					
                case EMetaDataRating:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaCustomGenreFormat, EMetaDataRating, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataGenre);
                        }					
                    break;
					
                case EMetaDataAlbumArtist:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaArtistFormat, EMetaDataAlbumArtist, &buf8));
                    if (KErrNone == err)
                        {
                        ConvertAndAppendL(&buf8, EMetaDataAlbumArtist);
                        }                   
                    break;
                    
                case EMetaDataJpeg:
                    TRAP(err, i3GPExtParser->GetilstBoxesL(K3GPExtMetaJpeg, EMetaDataJpeg, &buf8));
                    if (KErrNone == err)
                        {
                        AppendL(&buf8, EMetaDataJpeg);
                        }					
                    break;
					
                default:
                    break;
                }
            }
        } 
               
	}

// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::ValidateL
// -----------------------------------------------------------------------------
//
TBool CMetaDataParser3GPExt::ValidateL()
	{
#ifdef _DEBUG
	RDebug::Print(_L("CMetaDataParser3GPExt::ValidateL"));
#endif
	mp4_u32 audioLength, audioType, timeScale, averateBitRate;
	mp4_u8 framesPerSample;

	MP4Err err;
	err = MP4ParseRequestAudioDescription(iMP4Handle, &audioLength, &audioType,
			&framesPerSample, &timeScale, &averateBitRate);
	if ( err == MP4_OK  && i3GPExtParser->Is3GPExtMetadataL())
		{
		iExists = ETrue;
		return ETrue;
		}

	return EFalse;
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::AppendL
// -----------------------------------------------------------------------------
//
void CMetaDataParser3GPExt::AppendL(HBufC** aBuf, TMetaDataFieldId aFieldId)
	{
	CleanupStack::PushL(*aBuf);
    if((*aBuf) && (*aBuf)->Length() > 0)
        {
        iContainer->AppendL(aFieldId, (*aBuf)->Des()); // copy to container
        }
    CleanupStack::PopAndDestroy(); // *aBuf
    (*aBuf) = NULL;
    }
	
// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::AppendL
// -----------------------------------------------------------------------------
//
void CMetaDataParser3GPExt::AppendL(HBufC8** aBuf, TMetaDataFieldId aFieldId)
	{
	CleanupStack::PushL(*aBuf);
    if((*aBuf) && (*aBuf)->Length() > 0)
        {
        iContainer->AppendL(aFieldId, (*aBuf)->Des());  // copy to container
        }
    CleanupStack::PopAndDestroy(); // *aBuf
    (*aBuf) = NULL;
    }	


// -----------------------------------------------------------------------------
// CMetaDataParser3GPExt::ConvertToUnicodeL
// -----------------------------------------------------------------------------
//
TInt CMetaDataParser3GPExt::ConvertToUnicodeL(const TDesC8& aDesc, TDes16& aUnicode )
	{
	TPtrC8 unicodeData;
  	TUint characterSetId = 0;
	CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
	TInt state = CCnvCharacterSetConverter::KStateDefault;

	// M4A file format has UTF-8 BigEndian characters.
	characterSetId = KCharacterSetIdentifierUtf8;
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
	
//  End of File
