/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the data structure and message code
*                definitions needed by both the server and the client
*                interfaces at compile time.
*
*/


#ifndef SATSOPCODES_H
#define SATSOPCODES_H

//  INCLUDES

#include <e32base.h>
#include <languages.hrh>    // Contains new language definitions for Series 60.

// CONSTANTS
// Contains the full path name of the executable to be loaded.
_LIT( KSatServerNameAndPath, "z:\\sys\\bin\\SatServer.exe" );

// A version must be specified when creating a session with the server.
const TUint KSatServerMajorVersionNumber = 0;
const TUint KSatServerMinorVersionNumber = 1;
const TUint KSatServerBuildVersionNumber = 1;

// Number of message slots
const TInt KSatMessageSlots = 42;

// Max number of message arguments
const TInt KSatMaxMessageArguments = 4;

// DATA TYPES

// Operation codes used in message passing between client and server
enum TSatServerRequest
    {
    ESatSNone, // Illegal service request.
    // Server test commands
    ESatSDisplayText,
    ESatSNotifyTest,
    ESatSNotifyTestCancel,
    ESatSShutdown,
    // Proactive commands (asynchronous)
    ESatSProactiveDisplayText,
    ESatSProactiveGetInkey,
    ESatSProactiveGetInput,
    ESatSProactiveSetUpMenu,
    ESatSProactiveSelectItem,
    ESatSProactivePlayTone,
    ESatSProactiveRefresh,
    ESatSProactiveQuery, // for Send SM and Set Up Call
    ESatSProactiveEvent, // for server event notification
    ESatSProactiveNotification,

    // Envelope commands (synchronous)
    ESatSActiveMenuSelection,

    // Command responses (synchronous)
    ESatSProactiveDisplayTextResponse,
    ESatSProactiveGetInkeyResponse,
    ESatSProactiveGetInputResponse,
    ESatSProactiveSetUpMenuResponse,
    ESatSProactiveSelectItemResponse,
    ESatSProactivePlayToneResponse,
    ESatSProactiveQueryResponse,
    ESatSProactiveEventResponse,
    ESatSProactiveNotificationResponse,

    ESatSProactiveRefreshResponse,

    // Session indicators (for internal use)
    ESatSOpenUiSubSession,
    ESatSGetSIMAppStatus,
    ESatSCloseUiSubSession,

    // Refresh handling
    ESatSOpenRefreshSubSession,
    ESatSCloseRefreshSubSession,
    ESatSRefreshAllowed,
    ESatSNotifyAllowRefresh,
    ESatSNotityAllowRefreshCancel,
    ESatSNotifyRefresh,
    ESatSCancelNotifyRefresh,
    ESatSRefreshEFRead,

    // Icon handling
    ESatSOpenIconSubSession,
    ESatSCloseIconSubSession,
    ESatSGetIconInfo,
    ESatSGetIconInstance,
    ESatSIconInstanceGot,

    // SetUpIdleMode response
    ESatSIdleModeResponse,

    ESatSSessionTerminated
    };

// String array to convert Symbian TLanguage enums to
// ISO 639-1 language codes.
//
// See "http://lcweb.loc.gov/standards/iso639-2/englangn.html"
typedef const TText*  TLangString;
struct SLangString
    {
    TInt iId;
    TLangString iString;
    };

const SLangString KLangStrings[] =
    {
    { ELangEnglish,                 _S( "en" ) },
    { ELangFrench,                  _S( "fr" ) },
    { ELangGerman,                  _S( "de" ) },
    { ELangSpanish,                 _S( "es" ) },
    { ELangItalian,                 _S( "it" ) },
    { ELangSwedish,                 _S( "sv" ) },
    { ELangDanish,                  _S( "da" ) },
    { ELangNorwegian,               _S( "no" ) },
    { ELangFinnish,                 _S( "fi" ) },

    // Following three languages are not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangAmerican,                _S( "en" ) },
    { ELangSwissFrench,             _S( "fr" ) },
    { ELangSwissGerman,             _S( "de" ) },

    { ELangPortuguese,              _S( "pt" ) },
    { ELangTurkish,                 _S( "tr" ) },
    { ELangIcelandic,               _S( "is" ) },
    { ELangRussian,                 _S( "ru" ) },
    { ELangHungarian,               _S( "hu" ) },
    { ELangDutch,                   _S( "nl" ) },

    // Following five languages are not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangBelgianFlemish,          _S( "nl" ) },
    { ELangAustralian,              _S( "en" ) },  // ISO 639-2 "aus"
    { ELangBelgianFrench,           _S( "fr" ) },
    { ELangAustrian,                _S( "de" ) },
    { ELangNewZealand,              _S( "en" ) },

    { ELangInternationalFrench,     _S( "fr" ) },
    { ELangCzech,                   _S( "cs" ) },
    { ELangSlovak,                  _S( "sk" ) },
    { ELangPolish,                  _S( "pl" ) },
    { ELangSlovenian,               _S( "sl" ) },

    // Following three languages are not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangTaiwanChinese,           _S( "zh" ) },
    { ELangHongKongChinese,         _S( "zh" ) },
    { ELangPrcChinese,              _S( "zh" ) },

    { ELangJapanese,                _S( "ja" ) },
    { ELangThai,                    _S( "th" ) },
    { ELangAfrikaans,               _S( "af" ) },
    { ELangAlbanian,                _S( "sq" ) },
    { ELangAmharic,                 _S( "am" ) },

    { ELangArabic,                  _S( "ar" ) },
    { ELangArmenian,                _S( "hy" ) },
    { ELangTagalog,                 _S( "tl" ) },
    { ELangBelarussian,             _S( "be" ) },
    { ELangBengali,                 _S( "bn" ) },
    { ELangBulgarian,               _S( "bg" ) },
    { ELangBurmese,                 _S( "my" ) },
    { ELangCatalan,                 _S( "ca" ) },
    { ELangCroatian,                _S( "hr" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangCanadianEnglish,         _S( "en" ) },

    { ELangInternationalEnglish,    _S( "en" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangSouthAfricanEnglish,     _S( "en" ) },

    { ELangEstonian,                _S( "et" ) },
    { ELangFarsi,                   _S( "fa" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangCanadianFrench,          _S( "fr" ) },

    { ELangScotsGaelic,             _S( "gd" ) },
    { ELangGeorgian,                _S( "ka" ) },
    { ELangGreek,                   _S( "el" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangCyprusGreek,             _S( "el" ) },

    { ELangGujarati,                _S( "gu" ) },
    { ELangHebrew,                  _S( "he" ) },
    { ELangHindi,                   _S( "hi" ) },
    { ELangIndonesian,              _S( "id" ) },
    { ELangIrish,                   _S( "ga" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangSwissItalian,            _S( "it" ) },

    { ELangKannada,                 _S( "kn" ) },
    { ELangKazakh,                  _S( "kk" ) },
    { ELangKhmer,                   _S( "km" ) },
    { ELangKorean,                  _S( "ko" ) },
    { ELangLao,                     _S( "lo" ) },
    { ELangLatvian,                 _S( "lv" ) },
    { ELangLithuanian,              _S( "lt" ) },
    { ELangMacedonian,              _S( "mk" ) },
    { ELangMalay,                   _S( "ms" ) },
    { ELangMalayalam,               _S( "ml" ) },
    { ELangMarathi,                 _S( "mr" ) },
    { ELangMoldavian,               _S( "mo" ) },
    { ELangMongolian,               _S( "mn" ) },
    { ELangNorwegianNynorsk,        _S( "nn" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangBrazilianPortuguese,     _S( "pt" ) },

    { ELangPunjabi,                 _S( "pa" ) },
    { ELangRomanian,                _S( "ro" ) },
    { ELangSerbian,                 _S( "sr" ) },
    { ELangSinhalese,               _S( "si" ) },
    { ELangSomali,                  _S( "so" ) },
    { ELangInternationalSpanish,    _S( "es" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangLatinAmericanSpanish,    _S( "es" ) },

    { ELangSwahili,                 _S( "sw" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangFinlandSwedish,          _S( "sv" ) },

    { ELangReserved1,               _S( "en" ) },
    { ELangTamil,                   _S( "ta" ) },
    { ELangTelugu,                  _S( "te" ) },
    { ELangTibetan,                 _S( "bo" ) },
    { ELangTigrinya,                _S( "ti" ) },

    // Following language is not listed in ISO 639-1 specification.
    // Decisions based on parent language.
    { ELangCyprusTurkish,           _S( "tr" ) },

    { ELangTurkmen,                 _S( "tk" ) },
    { ELangUkrainian,               _S( "uk" ) },
    { ELangUrdu,                    _S( "ur" ) },
    { ELangReserved2,               _S( "en" ) },
    { ELangVietnamese,              _S( "vi" ) },
    { ELangWelsh,                   _S( "cy" ) },
    { ELangZulu,                    _S( "zu" ) },

    // Series 60 added language codes.
    { KLangTaiwanEnglish,           _S( "en" ) },
    { KLangHongKongEnglish,         _S( "en" ) },
    { KLangPrcEnglish,              _S( "en" ) },
    { KLangJapaneseEnglish,         _S( "en" ) },
    { KLangThaiEnglish,             _S( "en" ) },
    };

#define KNUMLANGSTRINGS ( sizeof( KLangStrings ) / sizeof( SLangString ) )

#endif      // SATSOPCODES_H

// End of File
