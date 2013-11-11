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
* Description:  This class defines the ID3v1 genre 
*
*/



#ifndef METADATAID3V1GENRE_H
#define METADATAID3V1GENRE_H


// CONSTANTS
const TInt KMaxGenreSize = 25;

_LIT(KGenreBlues, "Blues");
_LIT(KGenreClassicRock, "Classic Rock");
_LIT(KGenreCountry, "Country");
_LIT(KGenreDance, "Dance");
_LIT(KGenreDisco, "Disco");
_LIT(KGenreFunk, "Funk");
_LIT(KGenreGrunge, "Grunge");
_LIT(KGenreHipHop, "Hip-Hop");
_LIT(KGenreJazz, "Jazz");
_LIT(KGenreMetal, "Metal");
_LIT(KGenreNewAge, "New Age");
_LIT(KGenreOldies, "Oldies");
_LIT(KGenreOther, "Other");
_LIT(KGenrePop, "Pop");
_LIT(KGenreRB, "R&B");
_LIT(KGenreRap, "Rap");
_LIT(KGenreReggae, "Reggae");
_LIT(KGenreRock, "Rock");
_LIT(KGenreTechno, "Techno");
_LIT(KGenreIndustrial, "Industrial");
_LIT(KGenreAlternative, "Alternative");
_LIT(KGenreSka, "Ska");
_LIT(KGenreDeathMetal, "Death Metal");
_LIT(KGenrePranks, "Pranks");
_LIT(KGenreSoundtrack, "Soundtrack");
_LIT(KGenreEuroTechno, "Euro-Techno");
_LIT(KGenreAmbient, "Ambient");
_LIT(KGenreTripHop, "Trip-Hop");
_LIT(KGenreVocal, "Vocal");
_LIT(KGenreJazzFunk, "Jazz+Funk");
_LIT(KGenreFusion, "Fusion");
_LIT(KGenreTrance, "Trance");
_LIT(KGenreClassical, "Classical");
_LIT(KGenreInstrumental, "Instrumental");
_LIT(KGenreAcid, "Acid");
_LIT(KGenreHouse, "House");
_LIT(KGenreGame, "Game");
_LIT(KGenreSoundClip, "Sound Clip");
_LIT(KGenreGospel, "Gospel");
_LIT(KGenreNoise, "Noise");
_LIT(KGenreAlternRock, "AlternRock");
_LIT(KGenreBass, "Bass");
_LIT(KGenreSoul, "Soul");
_LIT(KGenrePunk, "Punk");
_LIT(KGenreSpace, "Space");
_LIT(KGenreMeditative, "Meditative");
_LIT(KGenreInstrumentalPop, "Instrumental Pop");
_LIT(KGenreInstrumentalRock, "Instrumental Rock");
_LIT(KGenreEthnic, "Ethnic");
_LIT(KGenreGothic, "Gothic");
_LIT(KGenreDarkwave, "Darkwave");
_LIT(KGenreTechnoIndustrial, "Techno-Industrial");
_LIT(KGenreElectronic, "Electronic");
_LIT(KGenrePopFolk, "Pop-Folk");
_LIT(KGenreEurodance, "Eurodance");
_LIT(KGenreDream, "Dream");
_LIT(KGenreSouthernRock, "Southern Rock");
_LIT(KGenreComedy, "Comedy");
_LIT(KGenreCult, "Cult");
_LIT(KGenreGangsta, "Gangsta");
_LIT(KGenreTop40, "Top 40");
_LIT(KGenreChristianRap, "Christian Rap");
_LIT(KGenrePopFunk, "Pop/Funk");
_LIT(KGenreJungle, "Jungle");
_LIT(KGenreNativeAmerican, "Native American");
_LIT(KGenreCabaret, "Cabaret");
_LIT(KGenreNewWave, "New Wave");
_LIT(KGenrePsychadelic, "Psychadelic");
_LIT(KGenreRave, "Rave");
_LIT(KGenreShowtunes, "Showtunes"); 
_LIT(KGenreTrailer, "Trailer");
_LIT(KGenreLoFi, "Lo-Fi");
_LIT(KGenreTribal, "Tribal");
_LIT(KGenreAcidPunk, "Acid Punk");
_LIT(KGenreAcidJazz, "AcidJazz");
_LIT(KGenrePolka, "Polka");
_LIT(KGenreRetro, "Retro");
_LIT(KGenreMusical, "Musical");
_LIT(KGenreRockRoll, "Rock & Roll");
_LIT(KGenreHardRock, "Hard Rock");
_LIT(KGenreFolk, "Folk");
_LIT(KGenreFolkRock, "Folk-Rock");
_LIT(KGenreNationalFolk, "National Folk");
_LIT(KGenreSwing, "Swing");
_LIT(KGenreFastFusion, "Fast Fusion");
_LIT(KGenreBebob, "Bebob");
_LIT(KGenreLatin, "Latin");
_LIT(KGenreRevival, "Revival");
_LIT(KGenreCeltic, "Celtic");
_LIT(KGenreBluegrass, "Bluegrass");
_LIT(KGenreAvantgarde, "Avantgarde");
_LIT(KGenreGothicRock, "Gothic Rock");
_LIT(KGenreProgressiveRock, "Progressive Rock");
_LIT(KGenrePsychedelicRock, "Psychedelic Rock");
_LIT(KGenreSymphonicRock, "Symphonic Rock");
_LIT(KGenreSlowRock, "Slow Rock");
_LIT(KGenreBigBand, "Big Band");
_LIT(KGenreChorus, "Chorus");
_LIT(KGenreEasyListening, "Easy Listening");
_LIT(KGenreAcoustic, "Acoustic");
_LIT(KGenreHumour, "Humour");
_LIT(KGenreSpeech, "Speech");
_LIT(KGenreChanson, "Chanson");
_LIT(KGenreOpera, "Opera");
_LIT(KGenreChamberMusic, "Chamber Music");
_LIT(KGenreSonata, "Sonata");
_LIT(KGenreSymphony, "Symphony");
_LIT(KGenreBootyBass, "Booty Bass");
_LIT(KGenrePrimus, "Primus");
_LIT(KGenrePornGroove, "Porn Groove");
_LIT(KGenreSatire, "Satire");
_LIT(KGenreSlowJam, "Slow Jam");
_LIT(KGenreClub, "Club");
_LIT(KGenreTango, "Tango");
_LIT(KGenreSamba, "Samba");
_LIT(KGenreFolklore, "Folklore");
_LIT(KGenreBallad, "Ballad");
_LIT(KGenrePowerBallad, "Power Ballad");
_LIT(KGenreRhythmicSoul, "Rhythmic Soul");
_LIT(KGenreFreestyle, "Freestyle");
_LIT(KGenreDuet, "Duet");
_LIT(KGenrePunkRock, "Punk Rock");
_LIT(KGenreDrumSolo, "Drum Solo");
_LIT(KGenreAcapella, "A capella");
_LIT(KGenreEuroHouse, "Euro-House");
_LIT(KGenreDanceHall, "Dance Hall");
_LIT(KGenrePodcast, "Podcast");

#endif      // METADATAID3V1GENRE_H

// End of File
