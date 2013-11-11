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
* Description:  Implementation of CCbsMessage class.
*
*/


// INCLUDE FILES
#include "CbsServerPanic.h"
#include "CCbsMessage.h"
#include "CbsLogger.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMessage::CCbsMessage
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMessage::CCbsMessage()
    {
    }

// Destructor    
CCbsMessage::~CCbsMessage()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessage::~CCbsMessage()");
    CBSLOGSTRING("CBSSERVER: <<< CCbsMessage::~CCbsMessage()");
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Alphabet
// Returns the alphabet set of this message.
// Decides the alphabet set of this message by examining DCS field. 
// Note that DCS value "unspecified" isn't used anymore.
// All messages are considered to have "default" representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecAlphabet CCbsMessage::Alphabet() const
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessage::Alphabet()");
    CBSLOGSTRING2("CBSSERVER: CCbsMessage::Alphabet(): iDCS: %d", iDCS );
    
    TCbsRecAlphabet alphabet( ECbsRecAlphabetUnspecified );   

    // Check for Message preceded by language indication/UCS2 and
    // General Data Coding Indication/UCS2
    if ( 
        ( iDCS & DCS_MASK_LANGUAGE_INDICATION_PRECEDES ) == DCS_MPLI_UCS2 || 
        ( iDCS & DCS_MASK_GENERAL_DATA_CODING_INDICATION ) == DCS_GDCI_UCS2 )
        {
        alphabet = ECbsRecAlphabetUCS2;
        }
    // Check for 8-bit DCSs
    else if ( 
        ( iDCS & DCS_MASK_GENERAL_DATA_CODING_INDICATION ) == DCS_GDCI_8BIT ||
        ( iDCS & DCS_MASK_MESSAGE_HANDLING )== DCS_DCMH_8BIT ||
        ( iDCS & DCS_MASK_UDH ) == DCS_UDH_8BIT )
        {
        alphabet = ECbsRecAlphabet8bit;
        }
     // Else assume that alphabet is default
    else
        {
        alphabet = ECbsRecAlphabetDefault;
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsMessage::Alphabet(), returning alphabet.");
    return alphabet;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::AppendContent
// Appends another message's content to this message's content.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::AppendContent( 
    const TDesC& aMsg )
    {
    iMessage->Des().Append( aMsg );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::AppendContent8
// Appends another message's content to this message's content.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::AppendContent8( 
    const TDesC8& aMsg )
    {
    iMessage8->Des().Append( aMsg );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::CellId
// Returns identifier of the cell in which the phone was when this  message was received.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TUint CCbsMessage::CellId() const
    {
    return iLocationArea.iCellId;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Contents
// Returns a pointer descriptor to the Unicode contents of the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
const TDesC& CCbsMessage::Contents() const
    {
    __ASSERT_DEBUG( iMessage != 0, CbsServerPanic( ECbsMsgRepUnprepared ) );
    return *iMessage;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Contents8
// Returns a pointer descriptor to 8-bit descriptor representation
// of the message body. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
const TDesC8& CCbsMessage::Contents8() const
    {
    __ASSERT_DEBUG( iMessage8 != 0, CbsServerPanic( ECbsMsgRepUnprepared ) );
    return *iMessage8;
    }


// -----------------------------------------------------------------------------
// CCbsMessage::DCS
// Returns the Data Coding Scheme of this message page.
// Refer to GSM specification 03.38 for details on DCS.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TCbsRecDCS CCbsMessage::DCS() const
    {
    return iDCS;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::GeographicalScope
// Returns the geographical scope of the message.
// This is a 16-bit value containing Geographical 
// Scope (2 bits), Message Code (10 bits) and Update Number (4 bits).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecGeographicalScope CCbsMessage::GeographicalScope() const
    {
    TInt gs( Key() >> 14 );
    if ( gs == KGeographicalScopeCellImmediate || 
         gs == KGeographicalScopeCellNormal )
        {
        return ECbsRecGeographicalScopeCell;
        }
    else if ( gs == KGeographicalScopePLMNNormal ) 
        {
        return ECbsRecGeographicalScopePLMN;
        }
    else
        {
        return ECbsRecGeographicalScopeLAC;
        }
    }

// -----------------------------------------------------------------------------
// CCbsMessage::GetPLMN
// Returns operator information.
// On return, aNetworkInfo contains identity of the network
// in which the phone was when this message was received.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsMessage::GetPLMN( 
    RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo ) const
    {
    aNetworkInfo.iCountryCode = iNetworkInfo.iCountryCode;
    aNetworkInfo.iNetworkId = iNetworkInfo.iNetworkId;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::IsChildSubindex
// Returns boolean value indicating if this message is a child subindex message.
// Child subindex messages have message code 1010101010b and
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CCbsMessage::IsChildSubindex() const
    {
    return IsIndexMessage() && ( TopicNumber() != 0 );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::IsClass0
// Returns ETrue, if this message is a Class 0 message according to 
// ETSI GSM specification 03.38.
// Message is of class 0 if the 6th and 7th bit of Geographical Scope
// are both zero.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsMessage::IsClass0() const
    {
    // Examine bits 0, 1, 4, 6 and 7 of DCS.
    TUint8 class0( DCS() );
    class0 &= DCS_MASK_CLASS0;
    // Check Class 0 indication.
    return class0 == DCS_CLASS0; 
    }

// -----------------------------------------------------------------------------
// CCbsMessage::IsCompressed
// Returns ETrue, if the message is compressed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsMessage::IsCompressed() const
    {
    // The CB message is compressed only if the value of DCS is 011xxxxx.
    return ( TUint8( iDCS >> 5 ) == 3 );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::IsIndexMessage
// Returns boolean value indicating if this is the index message.  
// Index messages have message code 1010101010b.
// Refer to GSM specifications to for details.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CCbsMessage::IsIndexMessage() const
    {
    return MessageCode() == KIndexMessageCode;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Key
// Returns the key of the message (serial number).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbMessageKey CCbsMessage::Key() const
    {
    return TCbsDbMessageKey( iSerialNumber );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::LAC
// Returns location area information.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TUint CCbsMessage::LAC() const
    {
    return iLocationArea.iLocationAreaCode;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Language
// Returns the language specified in this message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbLanguage CCbsMessage::Language() const
    {
    __ASSERT_DEBUG( iLanguage != -1, 
        CbsServerPanic( ECbsLanguageNotResolved ) );

    return iLanguage;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::LanguagePrecedingMessage
// Determines and returns the language of this message
// based on first two characters of message content.
// Assumes that the message content contains language
// indication and that the content has been decoded
// into UCS-2 representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbLanguage CCbsMessage::LanguagePrecedingMessage() const
    {
    /// Mapping of ISO639 codes to server internal language representation
    const TCbsIso639ToLangMapElement KLanguageMap[ KNumberOfLanguageMappings ] = 
        {
        TCbsIso639ToLangMapElement( KIso639Danish, ECbsDanish ),
        TCbsIso639ToLangMapElement( KIso639German, ECbsGerman ),
        TCbsIso639ToLangMapElement( KIso639Greek, ECbsGreek ),
        TCbsIso639ToLangMapElement( KIso639English, ECbsEnglish ),
        TCbsIso639ToLangMapElement( KIso639Spanish, ECbsSpanish ),
        TCbsIso639ToLangMapElement( KIso639Finnish, ECbsFinnish ),
        TCbsIso639ToLangMapElement( KIso639French, ECbsFrench ),
        TCbsIso639ToLangMapElement( KIso639Italian, ECbsItalian ),
        TCbsIso639ToLangMapElement( KIso639Dutch, ECbsDutch ),
        TCbsIso639ToLangMapElement( KIso639Norwegian, ECbsNorwegian ),
        TCbsIso639ToLangMapElement( KIso639Portuguese, ECbsPortuguese ),
        TCbsIso639ToLangMapElement( KIso639Swedish, ECbsSwedish ),
        TCbsIso639ToLangMapElement( KIso639Turkish, ECbsTurkish ),
        TCbsIso639ToLangMapElement( KIso639Hungarian, ECbsHungarian ),
        TCbsIso639ToLangMapElement( KIso639Polish, ECbsPolish ),
        TCbsIso639ToLangMapElement( KIso639Czech, ECbsCzech ),
        TCbsIso639ToLangMapElement( KIso639Hebrew, ECbsHebrew ),
        TCbsIso639ToLangMapElement( KIso639Arabic, ECbsArabic ),
        TCbsIso639ToLangMapElement( KIso639Russian, ECbsRussian ),
        TCbsIso639ToLangMapElement( KIso639Icelandic, ECbsIcelandic )
        };

    __ASSERT_DEBUG( iMessage != NULL , CbsServerPanic( ECbsMsgRepUnprepared ) );

    TPtrC isoCode( 
        Contents().Left( KLanguageIndicationLengthWithoutCR ) );

    for ( TInt i( 0 ); i<KNumberOfLanguageMappings; i++ )
        {
        TCbsIso639ToLangMapElement element( KLanguageMap[i] );
        TPtrC code( element.iCode );
        if ( code == isoCode )
            {
            TCbsDbLanguage language( element.iLang );
            return language;
            }
        }
    return ECbsOther;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::LanguageWithDefaultAlphabet
// Determines and returns the language of this message
// based on DCS value. Assumes that the same DCS value
// indicates, that the language can be determined in this
// way (and not from message content).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbLanguage CCbsMessage::LanguageWithDefaultAlphabet() const
    {
    TCbsDbLanguage language;
    switch ( iDCS )
        {
        // These cases handle DCS values with bits 0000xxxx.
        // All languages use the default alphabet.
        case ECbsRecLanguageGerman: 
            language = ECbsGerman; 
            break;
        case ECbsRecLanguageEnglish: 
            language = ECbsEnglish; 
            break;
        case ECbsRecLanguageItalian: 
            language = ECbsItalian; 
            break;
        case ECbsRecLanguageFrench: 
            language = ECbsFrench; 
            break;
        case ECbsRecLanguageSpanish: 
            language = ECbsSpanish; 
            break;
        case ECbsRecLanguageDutch: 
            language = ECbsDutch; 
            break;
        case ECbsRecLanguageSwedish: 
            language = ECbsSwedish; 
            break;
        case ECbsRecLanguageDanish: 
            language = ECbsDanish; 
            break;
        case ECbsRecLanguagePortuguese: 
            language = ECbsPortuguese; 
            break;
        case ECbsRecLanguageFinnish: 
            language = ECbsFinnish; 
            break;
        case ECbsRecLanguageNorwegian: 
            language = ECbsNorwegian; 
            break;
        case ECbsRecLanguageGreek: 
            language = ECbsGreek; 
            break;
        case ECbsRecLanguageTurkish: 
            language = ECbsTurkish; 
            break;
        case ECbsRecLanguageHungarian: 
            language = ECbsHungarian; 
            break;
        case ECbsRecLanguagePolish: 
            language = ECbsPolish; 
            break;
        case ECbsRecLanguageUnspecified: 
            language = ECbsOther; 
            break;
        case ECbsRecLanguageCzech:
            language = ECbsCzech;
            break;
        case ECbsRecLanguageHebrew:
            language = ECbsHebrew;
            break;
        case ECbsRecLanguageArabic:
            language = ECbsArabic;
            break;
        case ECbsRecLanguageRussian:
            language = ECbsRussian;
            break;
        case ECbsRecLanguageIcelandic:
            language = ECbsIcelandic;
            break;
        default:
            language = ECbsOther;
            break;
        }
    return language;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Message
// Returns the 16-bit message content
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CCbsMessage::Message() const
    {
    return iMessage;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::Message8
// Returns the 8-bit representation of the message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* CCbsMessage::Message8() const
    {
    return iMessage8;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::MessageCode
// Returns the message code of the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecMessageCode CCbsMessage::MessageCode() const
    {
    return TCbsRecMessageCode( ( TUint16( Key() & KMessageCodeMask ) ) >> 4 );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::MessageClass
// Returns the class of the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsMessage::MessageClass() const
    {
	TUint8 msgclass;
	msgclass = TUint8( iDCS >> 4 );
	TInt messageClass( 0 );

    if ( msgclass == 0x05 || msgclass == 0x07 || msgclass == 0x0f ) // 0000 0101 or 0000 0111 or 0000 1111
		{
			msgclass = 0x03; // 0000 0011

			// Lets find out last two bits
			msgclass = TUint8( msgclass & iDCS );
			switch ( msgclass )
			{
				case 1:
					messageClass = 1;
					break;
				case 2:
					messageClass = 2;
					break;
				case 3:
					messageClass = 3;
					break;
				case 0:
				default:
					messageClass = 0;
					break;
			}
		}
	return messageClass;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::MessagePrecededByLanguageIndication
// Returns ECbsRecLanguageIndicationBody if message's content is preceded 
// by language indication (see GSM 03.38).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecLanguageIndication CCbsMessage::MessagePrecededByLanguageIndication() const
    {
    TCbsRecLanguageIndication indication( ECbsRecLanguageIndicationHeader );
    if ( iDCS == 0x10 || iDCS == 0x11 )
        {
        indication = ECbsRecLanguageIndicationBody;
        }
    return indication;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::NetworkMode
// Returns current network mode (GSM/WCDMA)
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsNetworkMode CCbsMessage::NetworkMode() const
    {
    return iNetworkMode;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::PageParameter
// Returns the Page Parameter of the message page
//
// Page Parameter is a 8-bit field describing the number
// of pages in a message and the index of this page in a
// sequence of pages comprising a message.
// For single-page messages this value is 00010001b. 
// Value 0 (zero) is reserved with coding beginning at 0001b.
// Thus there can be 1..15 pages in one message.
// Total and this page -fields can be accessed with 
// methods TotalPages() and ThisPage().
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecPageParameter CCbsMessage::PageParameter() const
    {
    return iPageParameter;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::ReleaseEightBitRepresentation
// Frees the memory allocated for 8-bit representation of message content.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void CCbsMessage::ReleaseEightBitRepresentation()
    {
    delete iMessage8;
    iMessage8 = NULL;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::RemoveIndexHeaderL
// Removes any index header prefix from the message body.
//    
// This method assumes that the index header is contained in the
// first line of message body. This line, including trailing <cr><lf>
// is removed.
//
// Index header prefix is assumed to contain "EIn<cr><lf>", where n
// specifies the version number this index message claims to 
// follow and <cr><lf> is a carriage return + line feed.
//  
// It is also assumed that any language indication prefix has
// already been removed prior to this operation.
//
// Message must be in UCS-2 representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::RemoveIndexHeaderL() 
    {
    __ASSERT_DEBUG( IsIndexMessage(), CbsServerPanic( ECbsNotIndexMessage ) );

    if ( iMessage->Length() < KIndexHeaderLength )
        {
        User::Leave( KErrUnderflow );   // Msg too short to contain ix header.
        }

    iMessage->Des().Delete( KIndexHeaderPosition, KIndexHeaderLength );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::RemoveLanguageIndicationFromBodyL
// Removes language indication field from the message body.
//   
// Presence of an indication is verified with the DCS value 
// according to ETSI specifications. Message is assumed to
// contain valid indication, i.e., three bytes preceding the message
// body specifying the language code. 
//  
// The correctness of this operation is not checked. If the
// indication is invalid, the message body is likely to either have
// a couple of characters too many or few.
// 
// Message is assumed to be decoded into UCS-2.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::RemoveLanguageIndicationFromBodyL()
    {
    if ( MessagePrecededByLanguageIndication() == 
        ECbsRecLanguageIndicationBody )
        {
        if ( iMessage->Length() < KLanguageIndicationPosition + 
            KLanguageIndicationLength )
            {
            User::Leave( KErrUnderflow );
            }

        iMessage->Des().Delete( KLanguageIndicationPosition, 
            KLanguageIndicationLength );
        }
    }

// -----------------------------------------------------------------------------
// CCbsMessage::RequiresImmediateDisplay
// Returns ETrue, if this message requires to be displayed immediately.
//       	   
// The CB message has to be displayed immediately
// if either it is flagged as a Class 0 message (see GSM 03.38)
// or has a geographical scope of the type "Immediate" 
// (see GSM 03.41).
//  
// This function always returns EFalse for Base station ID messages 
// (message identifier = 50).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsMessage::RequiresImmediateDisplay() const
    {
    return IsClass0();
    }

// -----------------------------------------------------------------------------
// CCbsMessage::ReserveContentSizeL
// Increases the space allocated for message content.
// Reallocates message content to match parameter aLength.
// Should be used before AppendContent()-call.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::ReserveContentSizeL( 
    TInt aLength )
    {
    if ( iMessage == NULL )
        {
        iMessage = HBufC::NewL( aLength );
        }
    else if ( ( iMessage ) && ( aLength < iMessage->Length() ) )                
        {
        User::Leave( KErrArgument );
        }
    else
        {
        iMessage = iMessage->ReAllocL( aLength );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsMessage::ReserveContentSize8L
// Increases the space allocated for message content.
// Reallocates message content to match parameter aLength.
// Should be used before AppendContent()-call.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::ReserveContentSize8L( 
    TInt aLength )
    {
    if ( iMessage8 == NULL )
        {
        iMessage8 = HBufC8::NewL( aLength );
        }
    else if ( ( iMessage8 ) && ( aLength < iMessage8->Length() ) )                
        {
        User::Leave( KErrArgument );
        }
    else
        {
        iMessage8 = iMessage8->ReAllocL( aLength );
        }    
    }

// -----------------------------------------------------------------------------
// CCbsMessage::ResolveLanguage
// Determines language of this message so that calls to Language() 
// will return the correct value.
//
// Language information may be determined from Data Coding Scheme
// of message header or from the first characters in message body.
// For the latter to succeed, the message has to be decoded into UCS-2.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsMessage::ResolveLanguage()
    {
    if ( iDCS <= ECbsRecLanguageUnspecified ||      // 0000xxxx
        ( iDCS >= ECbsRecLanguageCzech && iDCS < ECbsRecLanguageMax ) )
        {
        // Default alphabet, bits 0-3 specify language.
        iLanguage = LanguageWithDefaultAlphabet();
        }
    else if ( MessagePrecededByLanguageIndication() == 
        ECbsRecLanguageIndicationBody )
        {
        iLanguage = LanguagePrecedingMessage();
        }
    else
        {
        iLanguage = ECbsOther;
        }
    }

// -----------------------------------------------------------------------------
// CCbsMessage::SetNetworkInfo
// Sets the network information of this message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsMessage::SetNetworkInfo(
    const RMobilePhone::TMobilePhoneNetworkInfoV1& aNetworkInfo,
    const RMobilePhone::TMobilePhoneLocationAreaV1& aArea )
    {
    iNetworkInfo = aNetworkInfo;
    iLocationArea = aArea;
    }

// -----------------------------------------------------------------------------
// CCbsMessage::TopicNumber
// Returns the 16-bit Message Identifier containing the topic number.	
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsDbTopicNumber CCbsMessage::TopicNumber() const
    {
    return TCbsDbTopicNumber( iMessageIdentifier );
    }

// -----------------------------------------------------------------------------
// CCbsMessage::UpdateNumber
// Returns the update number of the message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCbsRecUpdateNumber CCbsMessage::UpdateNumber() const
    {
    return TCbsRecUpdateNumber( Key() & 0xF );            // 00001111b
    }


// -----------------------------------------------------------------------------
// CCbsMessage::IsLivecastMessage
// Checks if this message is a Livecast message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool CCbsMessage::IsLivecastMessage() const
    {
    TBool isLivecastMsg( EFalse );

    // Check if this message is a Livecast message (just check the topic number)
    if ( TopicNumber() >= KCbsLivecastTopicMin && 
        TopicNumber() <= KCbsLivecastTopicMax )
        {
        // This is a Livecast msg
        isLivecastMsg = ETrue;
        }

    return isLivecastMsg;
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
