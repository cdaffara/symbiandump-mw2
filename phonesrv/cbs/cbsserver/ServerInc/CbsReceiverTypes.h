/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains enumerations and typedefs used by the receiver.
*
*/


#ifndef CBSRECEIVERTYPES_H
#define CBSRECEIVERTYPES_H

// INCLUDES

#include <s32std.h>

// CONSTANTS

// Alphabet/coding indication (GSM 03.38)
enum TCbsRecAlphabet
    {
    // 7-bit alphabet coding
    ECbsRecAlphabetDefault,
    // 8-bit alphabet
    ECbsRecAlphabet8bit,
    // UCS2 (Unicode) alphabet coding
    ECbsRecAlphabetUCS2,
    // Unspecified coding; Use default coding instead.
    ECbsRecAlphabetUnspecified
    };

// Language indication (GSM 03.38)
enum TCbsRecLanguageIndication
    {
    // Language indicated in message page header.
    ECbsRecLanguageIndicationHeader,
    // Message content preceded by language indication.
    ECbsRecLanguageIndicationBody
    };

enum TCbsRecGeographicalScope 
    {
    ECbsRecGeographicalScopeCell,
    ECbsRecGeographicalScopeLAC,
    ECbsRecGeographicalScopePLMN
    };

// DATA TYPES

/**
*   This represents the Data Coding Scheme of a CBS message.
*
*   DCS defines the language, encoding and compression of the message.
*
*   Refer to GSM specification 03.38 for details.
*/
typedef TUint8 TCbsRecDCS;

/**
*   This represents the Page Parameter of a CBS message.
*
*   Coding starts at bit value 0001 for a page value of 1. 0000 is reserved.
*
*   Bits 0-3 indicate total pages in this message (1..15).
*   Bits 4-7 indicate the page number of this message in the current message 
*            sequence (1..15).
*/
typedef TUint8 TCbsRecPageParameter;

// These types comprise the Serial Number field of the CBS header (16 bits).
typedef TUint16 TCbsRecMessageCode;            // actually 10 bits
typedef TUint8 TCbsRecUpdateNumber;            // actually 4 bits

#endif        // CBSRECEIVERTYPES_H
