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
* Description:  This file contains the header file of the CCbsMessage class.
*
*/



#ifndef CCBSMESSAGE_H
#define CCBSMESSAGE_H

//  INCLUDES
#include <etelmm.h>
#include "CbsReceiverTypes.h"
#include "CbsCommon.h"
#include <cbsmcncommon.h>

//  CONSTANTS  
// If this constant is set to iPLMN.iMCC, iPLMN does not contain network info
_LIT( KRecMessageNoNetworkInfo, "-1"  );

/// Defines association element (ISO639-code, language)
struct TCbsIso639ToLangMapElement
    {
    const TLitC< 3 > iCode;
    const TCbsDbLanguage iLang;

    TCbsIso639ToLangMapElement::TCbsIso639ToLangMapElement( 
        const TLitC< 3 > aCode, 
        TCbsDbLanguage aLang )
        :iCode( aCode ), 
        iLang( aLang ) { }
    };

// ISO639 codes defining recognized languages
_LIT( KIso639Danish, "DA" );
_LIT( KIso639German, "DE" );
_LIT( KIso639Greek, "EL" );
_LIT( KIso639English, "EN" );
_LIT( KIso639Spanish, "ES" );
_LIT( KIso639Finnish, "FI" );
_LIT( KIso639French, "FR" );
_LIT( KIso639Italian, "IT" );
_LIT( KIso639Dutch, "NL" );
_LIT( KIso639Norwegian, "NO" );
_LIT( KIso639Portuguese, "PT" );
_LIT( KIso639Swedish, "SV" );
_LIT( KIso639Turkish, "TR" );
_LIT( KIso639Hungarian, "HU" );
_LIT( KIso639Polish, "PL" );
_LIT( KIso639Czech, "CS" );
_LIT( KIso639Hebrew, "HE" );
_LIT( KIso639Arabic, "AR" );
_LIT( KIso639Russian, "RU" );
_LIT( KIso639Icelandic, "IS" );
// Amount of languages that are mapped to ISO639 codes.
const TInt KNumberOfLanguageMappings = 20;

// CB message header size in bytes.
const TInt KHeaderLength = 6;

// Offset in CB message to index header in characters. 
const TInt KIndexHeaderPosition = 0;

// Index header length in characters (e.g., "EI1<cr><lf>")
const TInt KIndexHeaderLength = 5;              // including trailing <cr><lf>

// Offset to language indication, if applicable
const TInt KLanguageIndicationPosition = 0;

// Fixed length of language indication preceding message body
const TInt KLanguageIndicationLength = 3;

// Fixed length of language indication without trailing CR.
const TInt KLanguageIndicationLengthWithoutCR = 2;

// Geographical scope of CB message: cell wide / immediate display
const TInt KGeographicalScopeCellImmediate = 0x00;

// Geographical scope of CB message: operator wide / normal display
const TInt KGeographicalScopePLMNNormal = 0x01;

// Geographical scope of CB message: location area wide / normal display
const TInt KGeographicalScopeLACNormal = 0x02;

// Geographical scope of CB message: cell wide / normal display
const TInt KGeographicalScopeCellNormal = 0x03;

// Message code of an index message.
const TUint16 KIndexMessageCode = 0x02AA; // 1010101010b

const TUint8 KPageParameterSingle = 0x11;   // this page: 1, total pages: 1

// ETSI DCS specification
const TUint8 DCS_MASK_GENERAL_DATA_CODING_INDICATION = 0xcc;
const TUint8 DCS_MASK_MESSAGE_HANDLING = 0xf4;
const TUint8 DCS_MASK_LANGUAGE_INDICATION_PRECEDES = 0xff;
const TUint8 DCS_GDCI_8BIT = 0x44;
const TUint8 DCS_GDCI_UCS2 = 0x48;
const TUint8 DCS_DCMH_8BIT = 0xf4;
const TUint8 DCS_MPLI_UCS2 = 0x11;
const TUint16 KMessageCodeMask = 0x3FF0;
const TUint8 DCS_MASK_UDH = 0x9c;
const TUint8 DCS_UDH_8BIT = 0x94;

const TUint8 DCS_MASK_CLASS0 = 0xd3;
const TUint8 DCS_CLASS0 = 0x50;

// Message identifier value indicating that the message contains 
// a base station identity.
const TUint16 KCbsBaseStationId = 50;       

// Information on Data Coding Scheme values.
// Refer to ETSI GSM 03.38 for details.
enum 
    {
    // Bits 4-7 0000: Language specified with bits 0-3, default alphabet.
    ECbsRecLanguageGerman = 0,
    ECbsRecLanguageEnglish = 1,
    ECbsRecLanguageItalian = 2,
    ECbsRecLanguageFrench = 3,
    ECbsRecLanguageSpanish = 4,
    ECbsRecLanguageDutch = 5,
    ECbsRecLanguageSwedish = 6,
    ECbsRecLanguageDanish = 7,
    ECbsRecLanguagePortuguese = 8,
    ECbsRecLanguageFinnish = 9,
    ECbsRecLanguageNorwegian = 10,
    ECbsRecLanguageGreek = 11,
    ECbsRecLanguageTurkish = 12,
    ECbsRecLanguageHungarian = 13,
    ECbsRecLanguagePolish = 14,
    ECbsRecLanguageUnspecified = 15,
    // Bits 4-7 0001: Message preceded by language indication, 
    // default or UCS2 alphabet.
    ECbsRecDCSDefaultMsgPrecededByLanguage = 16,
    ECbsRecDCSUCS2MsgPrecededByLanguage = 17,
    // Bits 4-7 0010: Czech language, reservations for European languages
    // using the default alphabet.
    ECbsRecLanguageCzech = 32,
    ECbsRecLanguageHebrew = 33,
    ECbsRecLanguageArabic = 34,
    ECbsRecLanguageRussian = 35,
    ECbsRecLanguageIcelandic = 36,
    // Bits 4-7 0011: Reserved for European languages using the default
    // alphabet, with unspecified handling at the MS.
    // Max value of language enum, this MUST remain as last
    ECbsRecLanguageMax = 100
    };

// Message types, GSM, WCDMA, Livecast
enum TCbsMessageType
    {
    ECbsMessageGsm,
    ECbsMessageWcdma,
    ECbsMessageLivecast,
    ECbsMessageTypeUnspecified
    };


//  CLASS DECLARATION 

/**
*   CCbsMessage is the base class for GSM anf WCDMA message classes.
*   
*/
class CCbsMessage : public CBase
    {
    public:

        /**
        *   Virtual destructor
        */
        virtual ~CCbsMessage();

    public:
        /**
        *   Returns the type of the message (only in WCDMA)
        *
        *   @return                     Type of this message
        */
        virtual TInt MessageType() const = 0;        

        /**
        *   Returns ETrue, if this message is a child subindex message.
        *
        *   Child subindex messages have message code 1010101010b and
        *   message identifier other than 0.
        *
        *   @return                     ETrue, if the message is child subindex
        */
        virtual TBool IsChildSubindex() const;    
		
        /**
        *   Returns ETrue, if this message is a livecast message.
        *
        *
        *   @return                     ETrue, if the message is livecast message
        */

		TBool IsLivecastMessage() const;

        /**
        *   Returns the message code of the message.
        *
        *   @return                     Message Code.
        */
        TCbsRecMessageCode MessageCode() const;

        /**
        *   Returns the update number of the message.
        *
        *   @return                     Update Number.
        */
        TCbsRecUpdateNumber UpdateNumber() const;

        /**
        *   Returns the alphabet set of this message.
        *
        *   Possible values are:
        *
        *   ECbsRecAlphabetDefault      7-bit GSM encoding
        *   ECbsRecAlphabet8bit         8-bit data, unknown encoding
        *   ECbsRecAlphabetUCS2         16-bit, UCS-2 encoding
        *   ECbsRecAlphabetUnspecified  Unknown encoding
        *
        *   @return                     Alphabet
        */
        TCbsRecAlphabet Alphabet() const;

        /**
        *   Appends another message's content to this message's content.
        *
        *   @param aMsg                 Message that is appended 
        *                               to this msg.
        */
        void AppendContent( const TDesC& aMsg );

        /**
        *   Appends another 8-bit message's content to this message's content.
        *
        *   @param aMsg                 Message that is appended 
        *                               to this msg.
        */
        void AppendContent8( const TDesC8& aMsg );

        /**
        *   Returns identifier of the cell in which the phone was when this
        *   message was received.
        *
        *   @return                     Cell identifier
        */
        TUint CellId() const;

        /**
        *   Returns a pointer descriptor to the Unicode contents of the 
        *   message.
        *
        *   Panics if 16-bit presentation is not prepared.
        *
        *   @return                     Pointer descriptor to the contents of 
        *                               the message.
        */
        const TDesC& Contents() const;

        /**
        *   Returns a pointer descriptor to 8-bit descriptor representation
        *   of the message body. Panics if 8-bit representation
        *   is not prepared.
        *
        *   @return                     Pointer descriptor to the 8-bit 
        *                               representation of this message's body.
        */
        const TDesC8& Contents8() const;

        /**
        *   Returns the Data Coding Scheme of this message page.
        *
        *   Refer to GSM specification 03.38 for details on DCS.
        *
        *   @return                     Data Coding Scheme.
        */
        TCbsRecDCS DCS() const;

        /**
        *   Returns the geographical scope of the message.
        *
        *   @return                     Geographical scope.
        */
        TCbsRecGeographicalScope GeographicalScope() const;

        /**
        *   On return, aNetworkId contains identity of the network
        *   in which the phone was when this message was received.
        *
        *   @param aNetworkInfo           Returns: Operator information
        */
        void GetPLMN( RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo ) const;

        /**
        *   Returns ETrue, if this message is a Class 0 message according
        *   to ETSI GSM specification 03.38.
        */
        TBool IsClass0() const;

        /**
        *   Returns ETrue, if the message is compressed.
        *
        *   @return                     ETrue, if the message is compressed.
        */
        TBool IsCompressed() const;

        /**
        *   Returns boolean value indicating if this is the index message.
        *
        *   Index messages have message code 1010101010b.
        *
        *   Refer to GSM specifications to for details.
        *
        *   @return                     Boolean value indicating if this is 
        *                               an index message.
        */
        TBool IsIndexMessage() const;

        /**
        *   Returns the key of the message (serial number).
        *    
        *   Refer to GSM specifications for details.
        *
        *   @return                     Key of the message.
        */
        TCbsDbMessageKey Key() const;

        /**
        *   Returns the LAC (Location Area Code) of the area where the phone
        *   was when this message was received.
        *
        *   @return                     Location area code
        */
        TUint LAC() const;

        /**
        *   Returns the language in which the message is written.
        *
        *   Function will panic, if the language has not been resolved
        *   with ResolveLanguage().
        *
        *   @return                     The language of the message.
        */
        TCbsDbLanguage Language() const;

        /**
        *   Determines and returns the language of this message
        *   based on first two characters of message content.
        *   Assumes that the message content contains language
        *   indication and that the content has been decoded
        *   into UCS-2 representation.
        *
        *   @returns                    Language of the message
        */
        TCbsDbLanguage LanguagePrecedingMessage() const;

        /**
        *   Determines and returns the language of this message
        *   based on DCS value. Assumes that the same DCS value
        *   indicates, that the language can be determined in this
        *   way (and not from message content).
        *
        *   @returns                    Language of the message
        */
        TCbsDbLanguage LanguageWithDefaultAlphabet() const;
        
        /**
        *   Returns the 16-bit representation of the message
        *   contents.
        *
        *   @returns                    Message contents
        */
        HBufC* Message() const;
        
        /**
        *   Returns the 8-bit representation of the message
        *   contents.
        *
        *   @returns                    Message contents
        */
        HBufC8* Message8() const;

        /**
        *   Returns the message class of the message.
        *
        *   @return                     Message Class.
        */
		TInt MessageClass() const;

        /**
        *   Returns ECbsRecLanguageIndicationBody, if message's content is 
        *   preceded by language indication (see GSM 03.38).
        *
        *   Return values:
        *   ECbsRecLanguageIndicationHeader Language indicated by the message
        *                                   header DCS value
        *   ECbsRecLanguageIndicationBody   Language indicated by 
        *                                   a two-character prefix in message
        *                                   body.
        *
        *   @return                         Indication location.
        */
        TCbsRecLanguageIndication MessagePrecededByLanguageIndication() const;

        /**
        *   Returns the network mode (GSM/WCDMA) of this message page.
        *        
        *   @return                         Network in which this message was 
        *                                   received (GSM/WCDMA).
        */
        TCbsNetworkMode NetworkMode() const;

        /**
        *   Returns the page parameter of this message page.
        *
        *   Total and this page -fields can be accessed with 
        *   methods TotalPages() and ThisPage().
        *
        *   @return                     Page Parameter.
        */
        TCbsRecPageParameter PageParameter() const;

        /**
        *   Frees the memory allocated for 8-bit representation of message
        *   content.
        */
        void ReleaseEightBitRepresentation();

        /**
        *   Removes any index header prefix from the message body.
        *
        *   This method assumes that the index header is contained in the
        *   first line of message body. This line, including trailing <cr><lf>
        *   is removed.
        *
        *   Index header prefix is assumed to contain "EIn<cr><lf>", where n
        *   specifies the version number this index message claims to 
        *   follow and <cr><lf> is a carriage return + line feed.
        *
        *   It is also assumed that any language indication prefix has
        *   already been removed prior to this operation.
        *
        *   Message must be in UCS-2 representation.
        *
        *   Leave reasons:
        *   KErrUnderflow   Message is too short to contain index header.
        */
        void RemoveIndexHeaderL();

        /**
        *   Removes language indication field from the message body.
        *
        *   Presence of an indication is verified with the DCS value 
        *   according to ETSI specifications. Message is assumed to
        *   contain valid indication, i.e., three bytes preceding the message
        *   body specifying the language code. 
        *    
        *   The correctness of this operation is not checked. If the
        *   indication is invalid, the message body is likely to either have
        *   a couple of characters too many or few.
        *
        *   Message is assumed to be decoded into UCS-2.
        *
        *   Leave reasons:
        *   KErrUnderflow  Message is too short to contain language indication.
        */
        void RemoveLanguageIndicationFromBodyL();

        /**
        *   Returns ETrue, if this message requires to be displayed 
        *   immediately.
        *
        *   The CB message has to be displayed immediately
        *   if either it is flagged as a Class 0 message (see GSM 03.38)
        *   or has a geographical scope of the type "Immediate" 
        *   (see GSM 03.41).
        *
        *   This function always returns EFalse for Base station ID messages 
        *   (message identifier = 50). 
        *
        *   @return                     Immediate display indication.
        */
        TBool RequiresImmediateDisplay() const;

        /**
        *   Increases the space allocated for message content.
        *
        *   @param aLength              New number of characters in msg 
        *                               content.
        */
        void ReserveContentSizeL( TInt aLength );

        /**
        *   Increases the space allocated for 8-bit message content.
        *
        *   @param aLength              New number of characters in msg 
        *                               content.
        */
        void ReserveContentSize8L( TInt aLength );

        /**
        *   Determines language of this message so that calls to Language()
        *   will return the correct value.
        *
        *   Language information may be determined from Data Coding Scheme
        *   of message header or from the first characters in message body.
        *   For the latter to succeed, the message has to be decoded into 
        *   UCS-2.
        */
        void ResolveLanguage();

        /**
        *   Sets the network information of this message.
        *
        *   @param aNetworkInfo         Network information
        *   @param aArea                Area information
        */
        void SetNetworkInfo(
            const RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo,
            const RMobilePhone::TMobilePhoneLocationAreaV1& aArea );

        /**
        *   Returns the page number of this page in the message page sequence,
        *
        *   @return                     Page number of this page.
        */
        virtual TUint ThisPage() const = 0;

        /**
        *   Returns the topic number to which the message belongs. 
        *   (Message Identifier)
        *
        *   @return                     The number of the topic
        */
        TCbsDbTopicNumber TopicNumber() const;

        /**
        *   Returns the total number of pages in this page's message.
        *
        *   @return                     Total number of pages in this message
        */
        virtual TUint TotalPages() const = 0;
        
    protected:
        CCbsMessage();      

    protected:
        // Data Coding Scheme (language, encoding, compression)
        TCbsRecDCS iDCS;

        // Own: contents of the message page in 16-bit representation
        HBufC* iMessage;

        // Own: contents of the message page in 8-bit representation
        HBufC8* iMessage8;

        // Serial Number (GS, Message Code, Update Number)
        TUint16 iSerialNumber;

        // Message Identifier (Topic number)
        TUint16 iMessageIdentifier;

        // Page Parameter (this page, total pages)
        TCbsRecPageParameter iPageParameter;

        // Total number of pages
        TInt iTotalNumberOfPages;

        // Location area information
        RMobilePhone::TMobilePhoneLocationAreaV1 iLocationArea;

        // Network information
        RMobilePhone::TMobilePhoneNetworkInfoV1 iNetworkInfo;

        // Language of this message
        TCbsDbLanguage iLanguage;

        // Network in which current message was received
        TCbsNetworkMode iNetworkMode;
        };

#endif      //  CCBSMESSAGE_H
            
// End of File


