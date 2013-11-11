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
* Description:  This class provides the base class for metadata parsers.
*                This class is the highest class in parser hierarchy.
*
*/



// INCLUDE FILES
#include "MetaDataParser.h"
#include <syslangutil.h>
#include <languages.hrh>
#include <topcharacterset.rsg>
#include <data_caging_path_literals.hrh>
#include <stdlib.h>
#include <bautils.h>

#include "MetaDataParser.h"
#include "MetaDataSourceFile.h"
#include "MetaDataFieldContainer.h"

_LIT(KCharacterSetRscFile, "TopCharacterSet.rsc");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMetaDataParser::CMetaDataParser
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMetaDataParser::CMetaDataParser()
	:	iContainer(NULL),
		iExists(EFalse),
		iCharacterSet(NULL),
		iTopCharacterSet(NULL)
    {
    }

// Destructor
CMetaDataParser::~CMetaDataParser()
	{
	}

// -----------------------------------------------------------------------------
// CMetaDataParser::MetaDataExists
// -----------------------------------------------------------------------------
//
TBool CMetaDataParser::MetaDataExists()
   	{
	return iExists;
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::StripTrailingZeroes
// -----------------------------------------------------------------------------
//
TPtrC8 CMetaDataParser::StripTrailingZeroes(
	const TDesC8& aDesc, TInt aEncoding)
    {
	// Find out the actual size of data, ie without the trailing zeros
	TInt realOffset = aDesc.Length() - 1;
	if(aEncoding == 0x00 || aEncoding == 0x03)
		{
		while ( realOffset >= 0  )
			{
			if ( aDesc[realOffset] != 0x00 )
				{
				break;
				}
				realOffset--;
			}
		return aDesc.Left( realOffset + 1 );	
		}	
	else
		{
		while ( realOffset > 0  )
			{
			if ( aDesc[realOffset] != 0x00  || aDesc[realOffset-1] != 0x00)
				{
				break;
				}
				realOffset -= 2;
			}
		return aDesc.Left( realOffset + 1 );
		}
    }


// -----------------------------------------------------------------------------
// CMetaDataParser::StripTrailingZeroes
// -----------------------------------------------------------------------------
//
TPtrC CMetaDataParser::StripTrailingZeroes(
	const TDesC& aDesc)
    {
	// Find out the actual size of data, ie without the trailing zeros
	TInt realOffset = aDesc.Length() - 1;
	while ( realOffset >= 0  )
			{
			if ( aDesc[realOffset] != 0x00 )
				{
				break;
				}
				realOffset--;
			}
		return aDesc.Left( realOffset + 1 );	
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::UnicodeBOM
// -----------------------------------------------------------------------------
//
TInt CMetaDataParser::UnicodeBOM(
	const TDesC8& aUnicode )
	{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::UnicodeBOM"));
#endif
    
	TPtrC8 bomData( aUnicode.Left( 2 ) );

	TBuf8<2> BEbom; // Big endian BOM
	BEbom.Append( TChar( 0xFE ) );
	BEbom.Append( TChar( 0xFF ) );

	TBuf8<2> LEbom; // Little endian BOM
	LEbom.Append( TChar( 0xFF ) );
	LEbom.Append( TChar( 0xFE ) );

	if ( bomData.Compare( BEbom ) == 0 )
		{
		// Big Endian
		return KUnicodeBOMBigEndian;
			}
	else if ( bomData.Compare( LEbom ) == 0 )
		{
		// Little Endian
		return 	KUnicodeBOMLittleEndian;
		}
	else
		{
		return KUnicodeBOMNotFound;
		}
	}
	
// -----------------------------------------------------------------------------
// CMetaDataParser::Version()
// -----------------------------------------------------------------------------
//
TID3Version CMetaDataParser::ID3Version()
{
	return ENonID3;
}

// -----------------------------------------------------------------------------
// CMetaDataParser::MapID3v1GenreToString()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::MapID3GenreToStringL(TInt aNum, TDes& aGenrePtr)
{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::MapID3GenreToStringL"));
#endif
    
	if(aNum < 0 || aNum > 125 && aNum != 199)
		{
		return;
		}
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
// CMetaDataParser::MapID3v2GenreToString()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::MapID3GenreToStringL(TInt aNum, TDes8& aGenrePtr)
{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::MapID3GenreToStringL"));
#endif

	if(aNum < 0 || aNum > 125 && aNum != 199)
		{
		return;
		}
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
// CMetaDataParser::GenerateTopCharacterSets()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::GenerateTopCharacterSetsL()
	{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::GenerateTopCharacterSetsL"));
#endif

	TInt result = 0;
	CArrayFixFlat<TInt>* installedLanguages = NULL;
	SysLangUtil::GetInstalledLanguages(installedLanguages);
	CleanupStack::PushL(installedLanguages);
	
	TFileName* resourceFileName=new(ELeave) TFileName;
	CleanupStack::PushL(resourceFileName);
	Dll::FileName(*resourceFileName); // Get the drive letter of dll
	
	TParse* parse=new(ELeave) TParse;
	CleanupStack::PushL(parse);
	User::LeaveIfError(parse->SetNoWild(KCharacterSetRscFile, &KDC_RESOURCE_FILES_DIR, resourceFileName));
	resourceFileName->Copy(parse->FullName());
    #ifdef _DEBUG
         RDebug::Print(_L("CMetaDataParser::GenerateTopCharacterSetsL, resourceFileName=%S"), resourceFileName);
    #endif
	
	// Resource file should always exist in the same drive as dll
	if (BaflUtils::FileExists(iFs, *resourceFileName))
		{
		TRAPD(err, iRscFile.OpenL(iFs, *resourceFileName));
		result = err;
		}
	
	CleanupStack::PopAndDestroy(parse);	  
	CleanupStack::PopAndDestroy(resourceFileName);
	  
	// if there is no resource file, then there is no top character set list
	if (result)
	    {
        #ifdef _DEBUG
	        RDebug::Print(_L("CMetaDataParser::GenerateTopCharacterSetsL-unable to find resource file"));
        #endif
	        
		CleanupStack::PopAndDestroy(installedLanguages);
		return;
	    }

	for (TInt i=0; i < installedLanguages->Count(); i++)
		{
		SelectCharacterSetsForLanguageL(installedLanguages->At(i));
		}
	iTopCharacterSet->Compress();
	installedLanguages->Reset();
	CleanupStack::PopAndDestroy(installedLanguages);
	iRscFile.Close();
	}


// -----------------------------------------------------------------------------
// CMetaDataParser::SelectCharacterSetsForLanguage()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::SelectCharacterSetsForLanguageL(TInt aLanguage)
	{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::SelectCharacterSetsForLanguageL, aLanguage=%d"), aLanguage);
#endif

	switch ( aLanguage )
		{
		case ELangEnglish:
			ReadCharacterSetResourceL(R_ENGLISH_CHAR_SET);
			break;
		case ELangFrench:
			ReadCharacterSetResourceL(R_FRENCH_CHAR_SET);
			break;
		case ELangGerman:
			ReadCharacterSetResourceL(R_GERMAN_CHAR_SET);
			break;
		case ELangTurkish:
			ReadCharacterSetResourceL(R_TURKISH_CHAR_SET);
			break;
		case ELangFinnish:
			ReadCharacterSetResourceL(R_FINNISH_CHAR_SET);
			break;
		case ELangSwedish:
			ReadCharacterSetResourceL(R_SWEDISH_CHAR_SET);
			break;
		case ELangRussian:
			ReadCharacterSetResourceL(R_RUSSIAN_CHAR_SET);
			break;
		case ELangArabic:
			ReadCharacterSetResourceL(R_ARABIC_CHAR_SET);
			break;
		case ELangHebrew:
			ReadCharacterSetResourceL(R_HEBREW_CHAR_SET);
			break;
		case ELangFarsi:
			ReadCharacterSetResourceL(R_FARSI_CHAR_SET);
			break;
		case ELangItalian:
			ReadCharacterSetResourceL(R_ITALIAN_CHAR_SET);
			break;
		case ELangPolish:
			ReadCharacterSetResourceL(R_POLISH_CHAR_SET);
			break;
		case ELangHungarian:
			ReadCharacterSetResourceL(R_HUNGARIAN_CHAR_SET);
			break;
		case ELangSpanish:
			ReadCharacterSetResourceL(R_SPANISH_CHAR_SET);
			break;
		case ELangDutch:
			ReadCharacterSetResourceL(R_DUTCH_CHAR_SET);
			break;
		case ELangPortuguese:
			ReadCharacterSetResourceL(R_PORTUGUESE_CHAR_SET);
			break;
		case ELangAmerican:
			ReadCharacterSetResourceL(R_ENGLISH_CHAR_SET); // Maybe an American characterset exist?
			break;
		case ELangCanadianFrench:
			ReadCharacterSetResourceL(R_FRENCH_CHAR_SET);
			break;
		case ELangBrazilianPortuguese:
			ReadCharacterSetResourceL(R_PORTUGUESE_CHAR_SET);
			break;
		case ELangLatinAmericanSpanish:
			ReadCharacterSetResourceL(R_SPANISH_CHAR_SET);
			break;
		case ELangLatvian:
			ReadCharacterSetResourceL(R_LATVIAN_CHAR_SET);
			break;
		case ELangGreek:
			ReadCharacterSetResourceL(R_GREEK_CHAR_SET);
			break;
		case ELangEstonian:
			ReadCharacterSetResourceL(R_ESTONIAN_CHAR_SET);
			break;
		case ELangLithuanian:
			ReadCharacterSetResourceL(R_LITHUANIAN_CHAR_SET);
			break;
		case ELangRomanian:
			ReadCharacterSetResourceL(R_ROMANIAN_CHAR_SET);
			break;
		case ELangUkrainian:
			ReadCharacterSetResourceL(R_UKRAINIAN_CHAR_SET);
			break;
		case ELangBulgarian:
			ReadCharacterSetResourceL(R_BULGARIAN_CHAR_SET);
			break;
		case ELangCroatian:
			ReadCharacterSetResourceL(R_CROATIAN_CHAR_SET);
			break;
		case ELangSerbian:
			ReadCharacterSetResourceL(R_SERBIAN_CHAR_SET);
			break;
		case ELangIndonesian:
			ReadCharacterSetResourceL(R_INDONESIAN_CHAR_SET);
			break;
		case ELangMalay:
		case ELangTagalog:
			ReadCharacterSetResourceL(R_MALAY_CHAR_SET);
			break;
		case ELangIcelandic:
			ReadCharacterSetResourceL(R_ICELANDIC_CHAR_SET);
			break;
		case ELangDanish:
			ReadCharacterSetResourceL(R_DANISH_CHAR_SET);
			break;
		case ELangNorwegian:
			ReadCharacterSetResourceL(R_NORWEGIAN_CHAR_SET);
			break;
		case ELangHindi:
			ReadCharacterSetResourceL(R_INDIAN_CHAR_SET);
			break;
		case ELangUrdu:
			ReadCharacterSetResourceL(R_URDU_CHAR_SET);
			break;
		case ELangCzech:
			ReadCharacterSetResourceL(R_CZECH_CHAR_SET);
			break;
		case ELangSlovak:
			ReadCharacterSetResourceL(R_SLOVAK_CHAR_SET);
			break;
		case ELangSlovenian:
			ReadCharacterSetResourceL(R_SLOVENIAN_CHAR_SET);
			break;
		case ELangTaiwanChinese:
		case ELangHongKongChinese:
			ReadCharacterSetResourceL(R_TAIWAN_HK_CHINESE_CHAR_SET);
			break;
		case ELangPrcChinese:
			ReadCharacterSetResourceL(R_CHINESE_CHAR_SET);
			break;
		case KLangTaiwanEnglish:
		case KLangPrcEnglish:
		case KLangJapaneseEnglish:
		case KLangThaiEnglish:
		case KLangHongKongEnglish:
			ReadCharacterSetResourceL(R_ENGLISH_CHAR_SET);
			break;
		case ELangJapanese:
			ReadCharacterSetResourceL(R_JAPANESE_CHAR_SET);
			break;
		case ELangThai:
			ReadCharacterSetResourceL(R_THAI_CHAR_SET);
			break;
		case ELangVietnamese:
			ReadCharacterSetResourceL(R_VIETNAMESE_CHAR_SET);
			break;
		case KLangApacMalay:
			ReadCharacterSetResourceL(R_MALAY_CHAR_SET);
			break;
        case ELangKorean:
            ReadCharacterSetResourceL(R_KOREAN_CHAR_SET);
            break;			
		default:
		    #ifdef _DEBUG
	            RDebug::Print(_L("CMetaDataParser::SelectCharacterSetsForLanguageL, aLanguage=%d"),aLanguage );
            #endif
			ReadCharacterSetResourceL(R_ENGLISH_CHAR_SET);
			break;
		}

	}

// -----------------------------------------------------------------------------
// CMetaDataParser::ReadCharacterSetResourceL()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::ReadCharacterSetResourceL(TInt aResourceId)
	{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::ReadCharacterSetResourceL, ResourceID=%d"), aResourceId);
#endif

	TResourceReader rscReader;			        // Resource reader
	HBufC8* rscBuf;						        // Buffer where resource is read

	rscBuf = iRscFile.AllocReadL(aResourceId);
	rscReader.SetBuffer(rscBuf);
	CleanupStack::PushL(rscBuf);

	TUint characterSetElementId;
	TInt numCharacterSetElements = rscReader.ReadInt16();
	TUint elemId;
	CCnvCharacterSetConverter::SCharacterSet elem;

	for (TInt i = 0; i < numCharacterSetElements; i++)
		{
		characterSetElementId = rscReader.ReadInt32();
		for (TInt j = 0; j < iCharacterSet->Count(); j++ )
			{
			elem = iCharacterSet->At(j);
			elemId = elem.Identifier();
			if ( elemId == characterSetElementId && !IsInTopCharacterSet(characterSetElementId) )
				{
				iTopCharacterSet->AppendL(elem);
				}
			}
		}

	CleanupStack::PopAndDestroy(1); // rscBuf
	}

// -----------------------------------------------------------------------------
// CMetaDataParser::IsInTopCharacterSet()
// -----------------------------------------------------------------------------
//
TBool CMetaDataParser::IsInTopCharacterSet(TUint aCharacterSetId)
	{
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::IsInTopCharacterSet, aCharacterSetId=%d"), aCharacterSetId);
#endif
	
	for (TInt i = 0; i < iTopCharacterSet->Count(); i++)
		{
		if ( iTopCharacterSet->At(i).Identifier() == aCharacterSetId )
			{
            #ifdef _DEBUG
			    RDebug::Print(_L("CMetaDataParser::IsInTopCharacterSet-found char set"));
            #endif
			
			return ETrue;
			}
		}
	return EFalse;
	}
// -----------------------------------------------------------------------------
// CMetaDataParser::SetID32Offset()
// -----------------------------------------------------------------------------
//
void CMetaDataParser::SetID32Offset( TUint aOffset )
    {
    iID32Offset = aOffset;
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::ID32Offset()
// -----------------------------------------------------------------------------
//
TUint CMetaDataParser::ID32Offset()
    {
    return iID32Offset;
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::CommonParseL
// -----------------------------------------------------------------------------
//
void CMetaDataParser::CommonParseL(
    CMetaDataSourceFile* aSource,
    const RArray<TMetaDataFieldId>& aWantedFields,
    CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::CommonParseL"));
#endif

    GetProtectedL(aSource, aContainer);
    ParseL(aWantedFields, aContainer);
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::CommonParseDesL
// -----------------------------------------------------------------------------
//
void CMetaDataParser::CommonParseDesL(
    const RArray<TMetaDataFieldId>& aWantedFields,
    CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::CommonParseDesL"));
#endif

     //Since this is for descriptor "source", by default it is not a DRM Protected file
     _LIT(KNoProtection, "0");
     aContainer.AppendL( EMetaDataProtected, KNoProtection );
           
     //Continue parsing the file
     ParseL(aWantedFields, aContainer);
    }

// -----------------------------------------------------------------------------
// CMetaDataParser::GetProtectedL
// -----------------------------------------------------------------------------
//
void CMetaDataParser::GetProtectedL( CMetaDataSourceFile* aSource,
        CMetaDataFieldContainer& aContainer )
    {
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::GetProtectedL"));
#endif

    TBool fileProtected = aSource->FileProtected();
    TBuf16<5> desc16;
    desc16.AppendNum(fileProtected);

    aContainer.AppendL( EMetaDataProtected, desc16 );
    
#ifdef _DEBUG
     RDebug::Print(_L("CMetaDataParser::GetProtectedL, Protected=%S "), &desc16);
#endif
    
    }

//  End of File
