/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defines all constants and data structures used by the Radio
*                Server and its clients.
*
*/



#ifndef RADIOSERVERDATA_H
#define RADIOSERVERDATA_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// CONSTANTS & DATA TYPES

/**
 *  Radio Server error definitions.
 *  In addition to the standard system error code, the following error codes may
 *  be returned.
 */
typedef TInt TRadioServerError;
const TRadioServerError KRadioServErrHardwareFaulty              = -12050;
const TRadioServerError KRadioServErrTuning                      = -12051;
const TRadioServerError KRadioServErrFrequencyOutOfBandRange     = -12052;
const TRadioServerError KRadioServErrFrequencyNotValid           = -12053;
const TRadioServerError KRadioServErrNotPermittedInCurrentState  = -12054;
const TRadioServerError KRadioServErrDuplicateRequest            = -12055;
const TRadioServerError KRadioServErrAntennaNotConnected	     = -12056;
const TRadioServerError KRadioServErrFmTransmitterActive	     = -12057;
const TRadioServerError KRadioServErrOfflineMode                 = -12058;

/**
* RadioServer panic codes
*/
enum TRadioServerPanic
	{
	ERadioPanicIllegalFunction,		// Illegal function called via client-server interface
	ERadioPanicBadHandle,
	ERadioPanicBadDescriptor,
	ERadioPanicPostConditionViolation
	};

// RadioServer Publish & Subscribe property category
const TUid KRadioServerPropertyCategory = {0x10207A7E};
// RadioServer monitor state key, defined as TBool
// ETrue to indicate that Radio is playing; client should now start to monitor radio activities.
// EFalse to indicate that Radio is stopped and there are no primary clients connected.
const TInt KRadioServPsMonitorState = 1;

/**
* Tuner type.
*/
enum TRsTuner
    {
    ERsTunerFm	= 0x01,		// FM tuner
    ERsTunerAm	= 0x04		// AM tuner
    };

/**
* Frequency range. The actual range comes from the adaptation. The following range
* specified in the comment is just a approximate guideline.
*/
enum TRsFrequencyRange
    {
    ERsRangeFmEuroAmerica	= 0x01,		// European and American FM frequency range (87.5 - 108 MHz)
    ERsRangeFmJapan			= 0x02,		// Japanese FM frequency range (76 - 90 MHz)
    ERsRangeAm				= 0x04		// AM (535 - 1605 kHz)
    };

// RadioServer balace
const TInt KRadioServerBalanceMin	= 0;
const TInt KRadioServerBalanceMax	= 100;

/**
*  Tuner capabilities class defines the capabilities of the tuner on the
*  device, as retrieved using the function GetTunerCapabilities.
*
*  @since Series 60 4.0
*/
class TRsTunerCapabilities
	{
	public:
		// Tuner Function bit flags - may be extended in future
		enum TRsTunerFunctions
			{
			// The tuner can be used when the device is in offline mode
			ETunerFunctionAvailableInOfflineMode	= 0x01,
			// RDS supported
			ETunerFunctionRds						= 0x02,
			// Device supports dual tuner
			ETunerFunctionDualTuner          		= 0x04
			};

	public:
		// Bitfield (as defined by TRsTunerBand) with the bits of the supported tuner bands
		TUint32 iFrequencyRange;
		// Bitfield (as defined by TRsTunerFunctions) with the bits of the supported functions set
		TUint32 iCapabilities;
		// Reserved for future
		TUint32 iAdditionalFunctions1;
		// Reserved for future
		TUint32 iAdditionalFunctions2;
	};

/**
* Player state
*/
enum TRsPlayerState
	{
	ERsPlayerIdle,
	ERsPlayerPlaying
	};

/**
* RDS Programme Type.
*
* The interpretation of values of this type depends on the origin of the RDS
* broadcast: in North America, a slightly different standard, RBDS, is used.
* These PTY codes are defined by static values KRsRbdsPtyXxxxx, for example
* KRsRbdsPtySoftRock.
*
* Elsewhere, including Europe, the RDS standard is used. In these areas, the
* PTY codes are defined by static values KRsRdsPtyXxxxx, for example
* KRsRdsPtyChildrensProgrammes.
*
* In all other important aspects, the European RDS and North American RBDS
* standards are identical.
*/
typedef TInt TRsRdsProgrammeType;

const static TRsRdsProgrammeType KRsRdsPtyNone = 0;
const static TRsRdsProgrammeType KRsRdsPtyNews = 1;
const static TRsRdsProgrammeType KRsRdsPtyCurrentAffairs = 2;
const static TRsRdsProgrammeType KRsRdsPtyInformation = 3;
const static TRsRdsProgrammeType KRsRdsPtySport = 4;
const static TRsRdsProgrammeType KRsRdsPtyEducation = 5;
const static TRsRdsProgrammeType KRsRdsPtyDrama = 6;
const static TRsRdsProgrammeType KRsRdsPtyCulture = 7;
const static TRsRdsProgrammeType KRsRdsPtyScience = 8;
const static TRsRdsProgrammeType KRsRdsPtyVariedSpeech = 9;
const static TRsRdsProgrammeType KRsRdsPtyPopMusic = 10;
const static TRsRdsProgrammeType KRsRdsPtyRockMusic = 11;
const static TRsRdsProgrammeType KRsRdsPtyEasyListening = 12;
const static TRsRdsProgrammeType KRsRdsPtyLightClassical = 13;
const static TRsRdsProgrammeType KRsRdsPtySeriousClassical = 14;
const static TRsRdsProgrammeType KRsRdsPtyOtherMusic = 15;
const static TRsRdsProgrammeType KRsRdsPtyWeather = 16;
const static TRsRdsProgrammeType KRsRdsPtyFinance = 17;
const static TRsRdsProgrammeType KRsRdsPtyChildrensProgrammes = 18;
const static TRsRdsProgrammeType KRsRdsPtySocialAffairs = 19;
const static TRsRdsProgrammeType KRsRdsPtyReligion = 20;
const static TRsRdsProgrammeType KRsRdsPtyPhoneIn = 21;
const static TRsRdsProgrammeType KRsRdsPtyTravel = 22;
const static TRsRdsProgrammeType KRsRdsPtyLeisure = 23;
const static TRsRdsProgrammeType KRsRdsPtyJazzMusic = 24;
const static TRsRdsProgrammeType KRsRdsPtyCountryMusic = 25;
const static TRsRdsProgrammeType KRsRdsPtyNationalMusic = 26;
const static TRsRdsProgrammeType KRsRdsPtyOldiesMusic = 27;
const static TRsRdsProgrammeType KRsRdsPtyFolkMusic = 28;
const static TRsRdsProgrammeType KRsRdsPtyDocumentary = 29;
const static TRsRdsProgrammeType KRsRdsPtyAlarmTest = 30;
const static TRsRdsProgrammeType KRsRdsPtyAlarm = 31;

const static TRsRdsProgrammeType KRsRbdsPtyNone = 0;
const static TRsRdsProgrammeType KRsRbdsPtyNews = 1;
const static TRsRdsProgrammeType KRsRbdsPtyInformation = 2;
const static TRsRdsProgrammeType KRsRbdsPtySports = 3;
const static TRsRdsProgrammeType KRsRbdsPtyTalk = 4;
const static TRsRdsProgrammeType KRsRbdsPtyRock = 5;
const static TRsRdsProgrammeType KRsRbdsPtyClassicRock = 6;
const static TRsRdsProgrammeType KRsRbdsPtyAdultHits = 7;
const static TRsRdsProgrammeType KRsRbdsPtySoftRock = 8;
const static TRsRdsProgrammeType KRsRbdsPtyTop40 = 9;
const static TRsRdsProgrammeType KRsRbdsPtyCountry = 10;
const static TRsRdsProgrammeType KRsRbdsPtyOldies = 11;
const static TRsRdsProgrammeType KRsRbdsPtySoft = 12;
const static TRsRdsProgrammeType KRsRbdsPtyNostalgia = 13;
const static TRsRdsProgrammeType KRsRbdsPtyJazz = 14;
const static TRsRdsProgrammeType KRsRbdsPtyClassical = 15;
const static TRsRdsProgrammeType KRsRbdsPtyRhythmAndBlues = 16;
const static TRsRdsProgrammeType KRsRbdsPtySoftRhythmAndBlues = 17;
const static TRsRdsProgrammeType KRsRbdsPtyLanguage = 18;
const static TRsRdsProgrammeType KRsRbdsPtyReligiousMusic = 19;
const static TRsRdsProgrammeType KRsRbdsPtyReligiousTalk = 20;
const static TRsRdsProgrammeType KRsRbdsPtyPersonality = 21;
const static TRsRdsProgrammeType KRsRbdsPtyPublic = 22;
const static TRsRdsProgrammeType KRsRbdsPtyCollege = 23;
const static TRsRdsProgrammeType KRsRbdsPtyUnassigned1 = 24;
const static TRsRdsProgrammeType KRsRbdsPtyUnassigned2 = 25;
const static TRsRdsProgrammeType KRsRbdsPtyUnassigned3 = 26;
const static TRsRdsProgrammeType KRsRbdsPtyUnassigned4 = 27;
const static TRsRdsProgrammeType KRsRbdsPtyUnassigned5 = 28;
const static TRsRdsProgrammeType KRsRbdsPtyWeather = 29;
const static TRsRdsProgrammeType KRsRbdsPtyEmergencyTest = 30;
const static TRsRdsProgrammeType KRsRbdsPtyEmergency = 31;


/**
 *  Table 1: Class codes for RTplus content types
 *  Defined in Radiotext plus (RTplus) Specification Version 2.1
 *  RDS Forum 2006-07-21 - R06/040_1
 */
enum TRsRdsRTplusClass
    {
    // Category: Item (0 - 11)
    ERsRTplusItemDummy = 0,
    ERsRTplusItemTitle,
    ERsRTplusItemAlbum,
    ERsRTplusItemTrackNumber,
    ERsRTplusItemArtist,
    ERsRTplusItemComposition,
    ERsRTplusItemMovement,
    ERsRTplusItemConductor,
    ERsRTplusItemComposer,
    ERsRTplusItemBand,
    ERsRTplusItemComment,
    ERsRTplusItemGenre,
    // Category: Info (12 - 30)
    ERsRTplusInfoNews,
    ERsRTplusInfoNewsLocal,
    ERsRTplusInfoStockMarket,
    ERsRTplusInfoSport,
    ERsRTplusInfoLottery,
    ERsRTplusInfoHoroscope,
    ERsRTplusInfoDailyDiversion,
    ERsRTplusInfoHealth,
    ERsRTplusInfoEvent,
    ERsRTplusInfoSzene,
    ERsRTplusInfoCinema,
    ERsRTplusInfoTV,
    ERsRTplusInfoDateTime,
    ERsRTplusInfoWeather,
    ERsRTplusInfoTraffic,
    ERsRTplusInfoAlarm,
    ERsRTplusInfoAdvisertisement,
    ERsRTplusInfoURL,
    ERsRTplusInfoOther,
    // Category: Programme (31 - 40)
    ERsRTplusStationNameShort,
    ERsRTplusStationNameLong,
    ERsRTplusProgramNow,
    ERsRTplusProgramNext,
    ERsRTplusProgramPart,
    ERsRTplusProgramHost,
    ERsRTplusProgramEditorialStaff,
    ERsRTplusStationFrequency,
    ERsRTplusProgramHomepage,
    ERsRTplusProgramSubChannel,
    // Category: Interactivity (41 - 53)
    ERsRTplusPhoneHotline,
    ERsRTplusPhoneStudio,
    ERsRTplusPhoneOther,
    ERsRTplusSmsStudio,
    ERsRTplusSmsOther,
    ERsRTplusEmailHotline,
    ERsRTplusEmailStudio,
    ERsRTplusEmailOther,
    ERsRTplusMmsOther,
    ERsRTplusChat,
    ERsRTplusChatCenter,
    ERsRTplusVoteQuestion,
    ERsRTplusVoteCenter,
    // Category: Reserved for future (54 - 55)
    ERsRTplusRfuClass54,
    ERsRTplusRfuClass55,
    // Category: Private classes (56 - 58)
    ERsRTplusPrivateClass56,
    ERsRTplusPrivateClass57,
    ERsRTplusPrivateClass58,
    // Category: Descriptor (59 - 63)
    ERsRTplusPlace,
    ERsRTplusAppointment,
    ERsRTplusIdentifier,
    ERsRTplusPurchase,
    ERsRTplusGetData
    };

/**
*  The RDS Capabilities class defines the capabilities of the RDS control on the
*  device, as retrieved using the function GetCapabilities.
*
*  @since Series 60 3.2
*/
class TRsRdsCapabilities
	{
	public:
		/// RDS Function Bit Flags
		enum TRsRdsFeatures
			{
			ERsRdsProgrammeIdentification	= 0x001,
			ERsRdsProgrammeType				= 0x002,
			ERsRdsProgrammeService 			= 0x004,
			ERsRdsRadioText 				= 0x008,
			ERsRdsRadioTextPlus				= 0x010,
			ERsRdsClockTime 				= 0x020,
			ERsRdsTrafficProgramme 			= 0x040,
			ERsRdsTrafficAnnouncement 		= 0x080,
			ERsRdsAlternateFrequency 		= 0x100
			};
	public:
		// Bitfield as defined by TRsRdsFeatures with the bits of the supported functions set
		TUint32 iRdsFunctions;
		// Reserved for future
		TUint32 iAdditionalFunctions1;
		// Reserved for future
		TUint32 iAdditionalFunctions2;
	};

/**
*  This class provides RDS features that can be enabled or disabled. Used in
*  function SubscribeRdsData.
*
*  @since Series 60 3.2
*/
class TRsRdsData
	{
	public:
		// RDS Function Bit Flags
		enum TRsRdsFeatures
			{
			ERsRdsProgrammeIdentification	= 0x001,
			ERsRdsProgrammeType				= 0x002,
			ERsRdsProgrammeService 			= 0x004,
			ERsRdsRadioText 				= 0x008,
			ERsRdsRadioTextPlus				= 0x010,
			ERsRdsClockTime 				= 0x020,
			ERsRdsTrafficProgramme 			= 0x040,
			ERsRdsTrafficAnnouncement 		= 0x080,
			ERsRdsAlternateFrequency 		= 0x100
			};

	// A value indicating a set containig all RDS data encapsulated by class TRdsData.
	static const TUint32 KRsAllRdsData = ( ERsRdsProgrammeIdentification | ERsRdsProgrammeType | ERsRdsProgrammeService
										 | ERsRdsRadioText|ERsRdsRadioTextPlus | ERsRdsClockTime
										 | ERsRdsTrafficProgramme |ERsRdsTrafficAnnouncement | ERsRdsAlternateFrequency);

	public:
		// Bitfield as defined by TRsRdsFeatures with the bits of the supported functions set
		TUint32 iRdsFunctions;
		// Reserved for future
		TUint32 iAdditionalFunctions1;
		// Reserved for future
		TUint32 iAdditionalFunctions2;
	};

// Programme Service name, 8 characters
typedef TBuf16<8> TRsRdsPSName;
// Radio Text, 64 characters
typedef TBuf16<64> TRsRdsRadioText;

typedef CArrayFixFlat<TInt>            TFreqList;
typedef CArrayFixFlat<TRsRdsPSName>    TPsNameList;
//typedef CArrayFixFlat<TRsRdsRTplusTag> TRtPlusTagsList;

/**
 *  RadioServer settings data.
 */
class TRsSettingsData
	{
	public:
		TInt iError;
		TInt iData1;
		TInt iData2;
		TInt iData3;
	};
/**
 *  RadioServer Radio Text+ object data.
 */
class TRsRdsRTPlusObjectData
	{
	public:
		TRsRdsRadioText aRtObj1;
		TUint32 iContentType1;
		TRsRdsRadioText aRtObj2;
		TUint32 iContentType2;
	};
  
/**
 *  RadioServer character conversion table data.
 *  Conversion from 8-bit RDS character set to 16-bit UNICODE
 */
static const TUint16 TRsRdsCharConvData[ ] = {
        // Convert all non displayble control characters (< 0x20) to UNICODE spaces 0x0020,
        // to avoid incorrect output in display.
        0x0020, // 0x00 NUL -> <space>
        0x0020, // 0x01 SOH -> <space>
        0x0020, // 0x02 STX -> <space>
        0x0020, // 0x03 ETX -> <space>
        0x0020, // 0x04 EOT -> <space>
        0x0020, // 0x05 ENQ -> <space>
        0x0020, // 0x06 ACK -> <space>
        0x0020, // 0x07 BEL -> <space>
        0x0020, // 0x08 BS -> <space>
        0x0020, // 0x09 TAB -> <space>
        0x0020, // 0x0A LF -> <space>
        0x0020, // 0x0B VT -> <space>
        0x0020, // 0x0C FF -> <space>
        0x0020, // 0x0D CR -> <space>
        0x0020, // 0x0E SO -> <space>
        0x0020, // 0x0F SI -> <space>
        0x0020, // 0x10 DLE -> <space>
        0x0020, // 0x11 DC1 -> <space>
        0x0020, // 0x12 DC2-> <space>
        0x0020, // 0x13 DC3 -> <space>
        0x0020, // 0x14 DC4 -> <space>
        0x0020, // 0x15 NAK -> <space>
        0x0020, // 0x16 SYN -> <space>
        0x0020, // 0x17 ETB -> <space>
        0x0020, // 0x18 CAN -> <space>
        0x0020, // 0x19 EM -> <space>
        0x0020, // 0x1A SUB -> <space>
        0x0020, // 0x1B ESC -> <space>
        0x0020, // 0x1C FS -> <space>
        0x0020, // 0x1D GS -> <space>
        0x0020, // 0x1E RS -> <space>
        0x0020, // 0x1F US -> <space>
        // end of non displayable control characters
        0x0020, // 0x20 RDS 8bit <space> to UNICODE <space>, first displayable character
        0x0021, // 0x21 !
        0x0022, // 0x22 "
        0x0023, // 0x23 #
        0x00A4, // 0x24 'sun'
        0x0025, // 0x25 %
        0x0026, // 0x26 &
        0x0027, // 0x27 '
        0x0028, // 0x28 (
        0x0029, // 0x29 )
        0x002A, // 0x2A *
        0x002B, // 0x2B +
        0x002C, // 0x2C ,
        0x002D, // 0x2D -
        0x002E, // 0x2E .
        0x002F, // 0x2F /
        0x0030, // 0x30 0
        0x0031, // 0x31 1
        0x0032, // 0x32 2
        0x0033, // 0x33 3
        0x0034, // 0x34 4
        0x0035, // 0x35 5
        0x0036, // 0x36 6
        0x0037, // 0x37 7
        0x0038, // 0x38 8
        0x0039, // 0x39 9
        0x003A, // 0x3A :
        0x003B, // 0x3B ;
        0x003C, // 0x3C <
        0x003D, // 0x3D =
        0x003E, // 0x3E >
        0x003F, // 0x3F ?
        0x0040, // 0x40 @
        0x0041, // 0x41 A
        0x0042, // 0x42 B
        0x0043, // 0x43 C
        0x0044, // 0x44 D
        0x0045, // 0x45 E
        0x0046, // 0x46 F
        0x0047, // 0x47 G
        0x0048, // 0x48 H
        0x0049, // 0x49 I
        0x004A, // 0x4A J
        0x004B, // 0x4B K
        0x004C, // 0x4C L
        0x004D, // 0x4D M
        0x004E, // 0x4E N
        0x004F, // 0x4F O
        0x0050, // 0x50 P
        0x0051, // 0x51 Q
        0x0052, // 0x52 R
        0x0053, // 0x53 S
        0x0054, // 0x54 T
        0x0055, // 0x55 U
        0x0056, // 0x56 V
        0x0057, // 0x57 W
        0x0058, // 0x58 X
        0x0059, // 0x59 Y
        0x005A, // 0x5A Z
        0x005B, // 0x5B [
        0x005C, // 0x5C '\'
        0x005D, // 0x5D ]
        0x0336, // 0x5E horizontal line centered
        0x005F, // 0x5F _
        0x01C1, // 0x60 double pipes centered
        0x0061, // 0x61 a
        0x0062, // 0x62 b
        0x0063, // 0x63 c
        0x0064, // 0x64 d
        0x0065, // 0x65 e
        0x0066, // 0x66 f
        0x0067, // 0x67 g
        0x0068, // 0x68 h
        0x0069, // 0x69 i
        0x006A, // 0x6A j
        0x006B, // 0x6B k
        0x006C, // 0x6C l
        0x006D, // 0x6D m
        0x006E, // 0x6E n
        0x006F, // 0x6F o
        0x0070, // 0x70 p
        0x0071, // 0x71 q
        0x0072, // 0x72 r
        0x0073, // 0x73 s
        0x0074, // 0x74 t
        0x0075, // 0x75 u
        0x0076, // 0x76 v
        0x0077, // 0x77 w
        0x0078, // 0x78 x
        0x0079, // 0x79 y
        0x007A, // 0x7A z
        0x007B, // 0x7B {
        0x007C, // 0x7C pipe
        0x007D, // 0x7D }
        0x00AF, // 0x7E horizontal line up
        0x0020, // 0x7F DEL (control character), set it to <space>
        0x00E1, // 0x80 á
        0x00E0, // 0x81 à
        0x00E9, // 0x82 é
        0x00E8, // 0x83 è
        0x00ED, // 0x84 í
        0x00EC, // 0x85 ì
        0x00F3, // 0x86 ó
        0x00F2, // 0x87 ò
        0x00FA, // 0x88 ú
        0x00F9, // 0x89 ù
        0x00D1, // 0x8A Ñ
        0x00C7, // 0x8B Ç
        0x015E, // 0x8C S with curve under
        0x00DF, // 0x8D double S
        0x00A1, // 0x8E ! upside down
        0x0132, // 0x8F 'IJ'
        0x00E2, // 0x90 â
        0x00E4, // 0x91 ä
        0x00E9, // 0x92 ê
        0x00EB, // 0x93 ë
        0x00EE, // 0x94 î
        0x00EF, // 0x95 ï
        0x00F4, // 0x96 ô
        0x00F6, // 0x97 ö
        0x00FB, // 0x98 û
        0x00FC, // 0x99 ü
        0x00F1, // 0x9A ñ        
        0x00E7, // 0x9B ç
        0x015F, // 0x9C s with curve under
        0x011F, // 0x9D g with curve above
        0x0131, // 0x9E i without dot
        0x0133, // 0x9F 'ij'
        0x00AA, // 0xA0 superscript a underlined
        0x03B1, // 0xA1 alfa-sign
        0x00A9, // 0xA2 copyright
        0x2030, // 0xA3 per mille sign
        0x01E6, // 0xA4 G with curve above
        0x011B, // 0xA5 e with curve above
        0x0148, // 0xA6 n with curve above
        0x0151, // 0xA7 o with tilted double quotes
        0x03C0, // 0xA8 pi sign
        0x20A0, // 0xA9 'CE' sign
        0x00A3, // 0xAA £
        0x0024, // 0xAB $
        0x2190, // 0xAC left arrow
        0x2191, // 0xAD up arrow
        0x2192, // 0xAE right arrow
        0x2193, // 0xAF down arrow
        0x00BA, // 0xB0 superscript o underlined
        0x00B9, // 0xB1 superscript 1
        0x00B2, // 0xB2 superscript 2
        0x00B3, // 0xB3 superscript 3
        0x00B1, // 0xB4 plus/minus
        0x0130, // 0xB5 I with dot
        0x0144, // 0xB6 n with right stroke
        0x0171, // 0xB7 u with tilted double quotes
        0x00B5, // 0xB8 mju
        0x00BF, // 0xB9 question mark upside down
        0x00F7, // 0xBA division sign
        0x00B0, // 0xBB degrees sign
        0x00BC, // 0xBC 1/4 sign
        0x00BD, // 0xBD 1/2 sign
        0x00BE, // 0xBE 3/4 sign
        0x00A7, // 0xBF §
        0x00C1, // 0xC0 Á
        0x00C0, // 0xC1 À
        0x00C9, // 0xC2 É
        0x00C8, // 0xC3 È
        0x00CD, // 0xC4 Í
        0x00CC, // 0xC5 Ì
        0x00D3, // 0xC6 Ó
        0x00D2, // 0xC7 Ò
        0x00DA, // 0xC8 Ú
        0x00D9, // 0xC9 Ù
        0x0158, // 0xCA R with curve above
        0x010C, // 0xCB C with curve above
        0x0160, // 0xCC S with curve above
        0x017D, // 0xCD Z with curve above
        0x0110, // 0xCE D with stroke
        0x013F, // 0xCF L with dot
        0x00C2, // 0xD0 Â
        0x00C4, // 0xD1 Ä
        0x00CA, // 0xD2 Ê
        0x00CB, // 0xD3 Ë
        0x00CE, // 0xD4 Î
        0x00CF, // 0xD5 Ï
        0x00D4, // 0xD6 Ô
        0x00D6, // 0xD7 Ö
        0x00DB, // 0xD8 Û
        0x00DC, // 0xD9 Ü
        0x0159, // 0xDA r with curve above
        0x010D, // 0xDB c with curve above
        0x0161, // 0xDC s with curve above
        0x017E, // 0xDD z with curve above
        0x0111, // 0xDE d with stroke
        0x0140, // 0xDF l with dot
        0x00C3, // 0xE0 Ã
        0x00C5, // 0xE1 Å
        0x00C6, // 0xE2 AE
        0x0152, // 0xE3 OE
        0x0177, // 0xE4 y with curve above
        0x00DD, // 0xE5 Ý
        0x00D5, // 0xE6 Õ
        0x00D8, // 0xE7 O with stroke
        0x00DE, // 0xE8 'P'-look-a-like
        0x014A, // 0xE9 'NJ'
        0x0154, // 0xEA R with stroke
        0x0106, // 0xEB C with stroke
        0x015A, // 0xEC S with stroke
        0x0179, // 0xED Z with stroke
        0x0166, // 0xEE T with stroke
        0x00F0, // 0xEF 'O'-look-a-like
        0x00E3, // 0xF0 ã
        0x00E5, // 0xF1 å
        0x00E6, // 0xF2 ae
        0x0153, // 0xF3 oe
        0x0175, // 0xF4 w with stroke
        0x00FD, // 0xF5 ý
        0x00F5, // 0xF6 õ
        0x00F8, // 0xF7 o with stroke
        0x00FE, // 0xF8 'p'-look-a-like
        0x014B, // 0xF9 'nj'
        0x0155, // 0xFA r with stroke
        0x0107, // 0xFB c with stroke
        0x015B, // 0xFC s with stroke
        0x017A, // 0xFD z with stroke
        0x0167, // 0xFE t with stroke
        0x0020  // 0xFF Should not happen, but just in case set to <space>
                //      to avoid out of table access.
};

#endif      // RADIOSERVERDATA_H

// End of File
