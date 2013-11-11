/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


#include <e32base.h>
#include <bautils.h>
#include <syslangutil.h>
#include <PathInfo.h>
#include <data_caging_path_literals.hrh>
#include <mdsplaylisttopcharacterset.rsg>

#include "harvesteraudioplaylistparser.h"

#include "mdsutils.h"
#include "harvesterlog.h"

_LIT( KMimeTypeM3U,   "audio/x-mpegurl" );
_LIT( KExtensionM3U,    "m3u" );

_LIT( KMDSPlaylistCharacterSetRscFile, "mdsplaylisttopcharacterset.rsc" );

const TInt KMDSArrayGranularity = 12;

// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::NewL
// -----------------------------------------------------------------------------
//
CHarvesterAudioPlaylistParser* CHarvesterAudioPlaylistParser::NewL()
    {
    CHarvesterAudioPlaylistParser* self = new ( ELeave ) CHarvesterAudioPlaylistParser();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );

	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CHarvesterAudioPlaylistParser::~CHarvesterAudioPlaylistParser()
    {
    delete iTopCharacterSet;
    
    delete iAvailableCharacterSet;
    
    iFs.Close();
    }


// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::CHarvesterAudioPlaylistParser
// -----------------------------------------------------------------------------
//
CHarvesterAudioPlaylistParser::CHarvesterAudioPlaylistParser() 
    {
    }


// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::ConstructL
// -----------------------------------------------------------------------------
//
void CHarvesterAudioPlaylistParser::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    iAvailableCharacterSet = CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableL( iFs );
    iTopCharacterSet =
        new (ELeave) CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>( KMDSArrayGranularity );
    GenerateTopCharacterSetsL();
    iM3UParser = CHarvesterM3UPlaylistParser::NewL( iFs, iAvailableCharacterSet, iTopCharacterSet );
    }


// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::ParseMimeType
// -----------------------------------------------------------------------------
//
TBool CHarvesterAudioPlaylistParser::ParseMimeType( 
		const TDesC& aFileName, TDes& aMimeType )
    {
    TPtrC ext;
    if( MdsUtils::GetExt( aFileName, ext ) )
    	{
    	if( ext == KExtensionM3U )
    	    {
    	    aMimeType = KMimeTypeM3U;
    	    return ETrue;
    	    }
    	}

    aMimeType = KNullDesC;
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CHarvesterAudioPlaylistParser::ParseL( const TDesC& aFileName,
		RPointerArray<HBufC>& aUriArray, TPtrC aMimeType )
    {
    if( aMimeType == KMimeTypeM3U )
        {
        return iM3UParser->ParseL( aFileName, aUriArray );
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::ResetL
// -----------------------------------------------------------------------------
//
void CHarvesterAudioPlaylistParser::Reset()
    {
    iM3UParser->Reset();
    }

// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::GenerateTopCharacterSetsL
// -----------------------------------------------------------------------------
//
void CHarvesterAudioPlaylistParser::GenerateTopCharacterSetsL()
     {
     CArrayFixFlat<TInt>* installedLanguages = NULL;
     SysLangUtil::GetInstalledLanguages( installedLanguages );
     CleanupStack::PushL( installedLanguages );
     
     TFileName fileName;
     
     TParsePtrC memory( PathInfo::RomRootPath() );
     fileName.Copy( memory.Drive() );
     fileName.Append( KDC_RESOURCE_FILES_DIR );
     fileName.Append( KMDSPlaylistCharacterSetRscFile );

     TRAPD( err, iRscFile.OpenL( iFs, fileName ) );
     
     // if there is no resource file, then there is no top character set list
     if( err )
         {
         CleanupStack::PopAndDestroy( installedLanguages );
         return;
         }

     const TInt count( installedLanguages->Count() );
     for( TInt i=0; i < count; i++ )
          {
          SelectCharacterSetsForLanguageL( (*installedLanguages)[i] );
          }
     iTopCharacterSet->Compress();
     installedLanguages->Reset();
     CleanupStack::PopAndDestroy( installedLanguages );
     iRscFile.Close();
     }

// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::SelectCharacterSetsForLanguageL
// -----------------------------------------------------------------------------
//
void CHarvesterAudioPlaylistParser::SelectCharacterSetsForLanguageL( TInt aLanguage )
     {
     switch( aLanguage )
          {
          case ELangEnglish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ENGLISH_CHAR_SET );
               break;
          case ELangFrench:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_FRENCH_CHAR_SET );
               break;
          case ELangGerman:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_GERMAN_CHAR_SET );
               break;
          case ELangTurkish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_TURKISH_CHAR_SET );
               break;
          case ELangFinnish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_FINNISH_CHAR_SET );
               break;
          case ELangSwedish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SWEDISH_CHAR_SET );
               break;
          case ELangRussian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_RUSSIAN_CHAR_SET );
               break;
          case ELangArabic:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ARABIC_CHAR_SET );
               break;
          case ELangHebrew:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_HEBREW_CHAR_SET );
               break;
          case ELangFarsi:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_FARSI_CHAR_SET );
               break;
          case ELangItalian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ITALIAN_CHAR_SET );
               break;
          case ELangPolish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_POLISH_CHAR_SET );
               break;
          case ELangHungarian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_HUNGARIAN_CHAR_SET );
               break;
          case ELangSpanish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SPANISH_CHAR_SET );
               break;
          case ELangDutch:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_DUTCH_CHAR_SET );
               break;
          case ELangPortuguese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_PORTUGUESE_CHAR_SET );
               break;
          case ELangAmerican:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ENGLISH_CHAR_SET );
               break;
          case ELangCanadianFrench:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_FRENCH_CHAR_SET );
               break;
          case ELangBrazilianPortuguese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_PORTUGUESE_CHAR_SET );
               break;
          case ELangLatinAmericanSpanish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SPANISH_CHAR_SET );
               break;
          case ELangLatvian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_LATVIAN_CHAR_SET );
               break;
          case ELangGreek:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_GREEK_CHAR_SET );
               break;
          case ELangEstonian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ESTONIAN_CHAR_SET );
               break;
          case ELangLithuanian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_LITHUANIAN_CHAR_SET );
               break;
          case ELangRomanian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ROMANIAN_CHAR_SET );
               break;
          case ELangUkrainian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_UKRAINIAN_CHAR_SET );
               break;
          case ELangBulgarian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_BULGARIAN_CHAR_SET );
               break;
          case ELangCroatian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_CROATIAN_CHAR_SET );
               break;
          case ELangSerbian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SERBIAN_CHAR_SET );
               break;
          case ELangIndonesian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_INDONESIAN_CHAR_SET );
               break;
          case ELangMalay:
          case ELangTagalog:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_MALAY_CHAR_SET );
               break;
          case ELangIcelandic:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ICELANDIC_CHAR_SET );
               break;
          case ELangDanish:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_DANISH_CHAR_SET );
               break;
          case ELangNorwegian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_NORWEGIAN_CHAR_SET );
               break;
          case ELangHindi:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_INDIAN_CHAR_SET );
               break;
          case ELangUrdu:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_URDU_CHAR_SET );
               break;
          case ELangCzech:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_CZECH_CHAR_SET );
               break;
          case ELangSlovak:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SLOVAK_CHAR_SET );
               break;
          case ELangSlovenian:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_SLOVENIAN_CHAR_SET );
               break;
          case ELangTaiwanChinese:
          case ELangHongKongChinese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_TAIWAN_HK_CHINESE_CHAR_SET );
               break;
          case ELangPrcChinese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_CHINESE_CHAR_SET );
               break;
          case ELangEnglish_Taiwan:
          case ELangEnglish_Prc:
          case ELangEnglish_Japan:
          case ELangEnglish_Thailand:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_ENGLISH_CHAR_SET );
               break;
          case ELangJapanese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_JAPANESE_CHAR_SET );
               break;
          case ELangThai:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_THAI_CHAR_SET );
               break;
          case ELangVietnamese:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_VIETNAMESE_CHAR_SET );
               break;
          case ELangMalay_Apac:
               ReadCharacterSetResourceL( R_MDS_PLAYLIST_MALAY_CHAR_SET );
               break;
          default:
               break;
          }
     }

// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::ReadCharacterSetResourceL
// -----------------------------------------------------------------------------
//
void CHarvesterAudioPlaylistParser::ReadCharacterSetResourceL( TInt aResourceId )
     {
     TResourceReader rscReader;                       // Resource reader
     HBufC8* rscBuf;                                      // Buffer where resource is read

     rscBuf = iRscFile.AllocReadL( aResourceId );
     rscReader.SetBuffer( rscBuf );
     CleanupStack::PushL( rscBuf );

     TUint characterSetElementId;
     TInt numCharacterSetElements = rscReader.ReadInt16();
     TUint elemId;
     CCnvCharacterSetConverter::SCharacterSet elem;

     for( TInt i = 0; i < numCharacterSetElements; i++ )
          {
          characterSetElementId = rscReader.ReadInt32();
          const TInt count( iAvailableCharacterSet->Count() );
          for( TInt j = 0; j < count; j++ )
               {
               elem = (*iAvailableCharacterSet)[j];
               elemId = elem.Identifier();
               if ( elemId == characterSetElementId && !IsInTopCharacterSet(characterSetElementId) )
                    {
                    iTopCharacterSet->AppendL( elem );
                    }
               }
          }

     CleanupStack::PopAndDestroy( rscBuf );
     }     
     
// -----------------------------------------------------------------------------
// CHarvesterAudioPlaylistParser::IsInTopCharacterSet
// -----------------------------------------------------------------------------
//
TBool CHarvesterAudioPlaylistParser::IsInTopCharacterSet( TUint aCharacterSetId )
     {
     const TInt count( iTopCharacterSet->Count() ); 
     for( TInt i = 0; i < count; i++ )
          {
          if( (*iTopCharacterSet)[i].Identifier() == aCharacterSetId )
               {
               return ETrue;
               }
          }
     return EFalse;
     }

// End of file
