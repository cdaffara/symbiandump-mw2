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
* Description:  This module contains the implementation of CCbsRecDecoder class 
*                member functions.
*    
*                CCCbsRecDecoder handles decoding of message contents.
*
*                CbsServer receives messages stored in 8-bit descriptors
*                from ETel. This class converts them into 16-bit descriptors
*                and decodes the message representation into UCS-2 (which
*                is used by Symbian OS internally). 
*
*/


// INCLUDE FILES

#include <e32svr.h>
#include <charconv.h>
#include "CbsServerConstants.h"
#include "CCbsRecDecoder.h"
#include "CCbsMessage.h"
#include "CCbsRecMessage.h"
#include "CCbsRecWcdmaMessage.h"
#include "CbsLogger.h"

// CONSTANTS

// Max characters in generated unicode 
const TInt KMaxCharsInGeneratedUnicode = 128;   

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMcnSession::CCbsMcnSession
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecDecoder::CCbsRecDecoder()
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecDecoder::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::ConstructL()
    {
    iConverter = CCnvCharacterSetConverter::NewL();
    User::LeaveIfError( iFs.Connect() );
    }     

// -----------------------------------------------------------------------------
// CCbsRecDecoder::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsRecDecoder* CCbsRecDecoder::NewL()
    {
    CCbsRecDecoder* self = new(ELeave) CCbsRecDecoder;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CCbsRecDecoder::~CCbsRecDecoder()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecDecoder::~CCbsRecDecoder()");    
    iFs.Close();
    delete iConverter;
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecDecoder::~CCbsRecDecoder()");
    }

// -----------------------------------------------------------------------------
// CCbsRecDecoder::DecodeL
// Decodes 7-bit, 8-bit and 16-bit representations into UCS-2.
// If the message has a language indication prefixed
// in the message body, the indication is removed.
// Compressed messages are not supported.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::DecodeL( 
    CCbsMessage& aMessage )
    {
    if ( aMessage.IsCompressed() )
        {
        User::Leave( KErrNotSupported );
        }

    // Convert message into plaintext.
    DoDecodeL( aMessage );

    // 8-bit representation not needed anymore.
    aMessage.ReleaseEightBitRepresentation();

    // Determine language of this message (from header or content),
    aMessage.ResolveLanguage();

    // Remove language indication, if present, from message.
    aMessage.RemoveLanguageIndicationFromBodyL();
    }

// -----------------------------------------------------------------------------
// CCbsRecDecoder::DoDecodeL
// Decodes the given message's content. 
// Decodes aMessage's 7-bit/Unicode representation
// into internal Unicode representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::DoDecodeL( 
    CCbsMessage& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecDecoder::DoDecodeL()");
    
    TCbsRecAlphabet alphabet( aMessage.Alphabet() );
    if ( alphabet == ECbsRecAlphabetDefault || 
         alphabet == ECbsRecAlphabetUnspecified )
        {
        DefaultAlphabetDecodeL( aMessage );
        }
    else if ( alphabet == ECbsRecAlphabet8bit )
        {
        EightbitAlphabetDecodeL( aMessage );
        }
    else
        {
        UnicodeDecodeL( aMessage );
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecDecoder::DoDecodeL()");
    }
    
// -----------------------------------------------------------------------------
// CCbsRecDecoder::UnicodeDecodeL
// Decodes 8-bit and Unicode message representations.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::UnicodeDecodeL( 
    CCbsMessage& aMsg )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecDecoder::UnicodeDecodeL()");
    
    TPtrC8 contents( aMsg.Contents8() );

    // Reserve enough space for the actual contents plus
    // the CR key
    aMsg.ReserveContentSizeL( contents.Length()+1 );

    // Check if the message is preceded with language.
    if( aMsg.DCS() == DCS_MPLI_UCS2 )
        {
        // The language is encoded as 7-bit USSD. Unpack that
        // and add CR after the language to match the default
        // alphabet case.
        TBuf<3> language;
        language.Copy( contents.Left(2) );
        language[1] <<= 1;
        language[1] |= (language[0] & 0x80) >> 7;
        language[0] &= 0x7f;
        language.Append( EKeyEnter );
        aMsg.AppendContent( language );

        // Skip the language indication
        contents.Set( contents.Mid(2) );
        }

    // The rest of the message is UCS2 encoded Unicode. 
    // Make sure the byte order is correct.
    HBufC* text = HBufC::NewL( contents.Length() / 2 );
    CleanupStack::PushL( text );

    TPtr ptr( text->Des() );
    TInt length( contents.Length() );

    // If the length is an odd number, remove the last character.
    if ( length & 0x01 )
        {
        length -= 1;
        }

    for ( TInt i( 0 ); i < length; i += 2 )
        {
        ptr.Append( ( contents[ i ] << 8 ) + contents[ i + 1 ] );
        }

    aMsg.AppendContent( RemoveTrailingCR( *text ) );
    CleanupStack::PopAndDestroy();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecDecoder::UnicodeDecodeL()");
    }
   
// -----------------------------------------------------------------------------
// CCbsRecDecoder::DefaultAlphabetDecodeL
// Decodes 7-bit message representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::DefaultAlphabetDecodeL( 
    CCbsMessage& aMsg )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecDecoder::DefaultAlphabetDecodeL()");
    
    // Check that the converter is available
    CCnvCharacterSetConverter* characterSetConverter = 
        CCnvCharacterSetConverter::NewLC(); // on CS

    CCnvCharacterSetConverter::TAvailability availability = 
        characterSetConverter->PrepareToConvertToOrFromL(
            KCharacterSetIdentifierSms7Bit, iFs );

    if ( availability == CCnvCharacterSetConverter::ENotAvailable )
        {
        User::Leave( KErrNotFound );
        }

    // Allocate a buffer for the cleartext message
    TBuf8< KCbsMaxCharsInPage + 1 > realMsg;

    // Get a pointer to the encoded message
    TPtrC8 msgPartOfEtelMsg = aMsg.Contents8();

    // Length of the cleartext message.
    TInt messageLength( ( msgPartOfEtelMsg.Length() * 8 ) / 7 );

    static const TUint mask_table[ 8 ] = 
			    { 0x7F, 0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F };
    TUint8  si,di;            // Indexes
    TInt  tmp_modulo;         // Temporary; to improve efficiency
    si = 0;

    // The location of the current 7-bit character determines the
    // action to be taken. Only every 7th character is not divided into
    // two bytes. All other characters will have to be contructed by
    // combining bits of two consequent bytes.

    for ( di = 0; di < messageLength; di++ )
	    {
	    TUint num2 = 0;
	    tmp_modulo = di % 8;
	    switch ( tmp_modulo )
	        {
	        case 0:
		        num2 = msgPartOfEtelMsg[ si ] & 0x7F;
		        realMsg.Append( num2 );
		        break;
	        case 7:
		        num2 = ( msgPartOfEtelMsg[ si ] >> 1 ) & 0x7F;
		        realMsg.Append( num2 );
		        si++;
		        break;
	        default:
	            num2 = msgPartOfEtelMsg[ si ] >> ( 8 - ( tmp_modulo ) );
	            num2 &= mask_table[ tmp_modulo ];
	            num2 |= msgPartOfEtelMsg[ si + 1 ] << ( tmp_modulo );
	            num2 &= 0x7F;
	            realMsg.Append( num2 );
	            si++;
	        break;
	        }//switch( tmp_modulo )
	    }//for

    TBuf16<KMaxCharsInGeneratedUnicode> generatedUnicode;
    TInt state( CCnvCharacterSetConverter::KStateDefault ); // has to be ref.
    
    // Remove all trailing control characters from the end of the Cell info msg
    TBuf8<KCbsMaxCharsInPage> cleanedMsg;
    if ( aMsg.TopicNumber() == KCellInfoTopic )
        {
        cleanedMsg = RemoveTrailingControlChars( realMsg );
        User::LeaveIfError( characterSetConverter->ConvertToUnicode(
            generatedUnicode, cleanedMsg, state ) );
        }
    else
        {
        User::LeaveIfError( characterSetConverter->ConvertToUnicode(
            generatedUnicode, realMsg, state ) );
        } 
    
    CleanupStack::PopAndDestroy(); // characterSetConverter
    
    aMsg.ReserveContentSizeL( messageLength );
    aMsg.AppendContent( RemoveTrailingCR( generatedUnicode ) );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecDecoder::DefaultAlphabetDecodeL()");
    }
    
// -----------------------------------------------------------------------------
// CCbsRecDecoder::EightbitAlphabetDecodeL
// Decodes 8-bit message representation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecDecoder::EightbitAlphabetDecodeL( 
    CCbsMessage& aMsg )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecDecoder::EightbitAlphabetDecodeL()");
    
    // Copy 8-bit representation to 16-bit
    HBufC* hbuf = HBufC::NewL( aMsg.Contents8().Length() );
    CleanupStack::PushL( hbuf );
    
    TPtr16 ptr16 = hbuf->Des();     
    ptr16.Copy( aMsg.Contents8() );        
    
    // Reserve enough space for the actual contents    
    aMsg.ReserveContentSizeL( aMsg.Contents8().Length() );  
    aMsg.AppendContent( RemoveTrailingCR( *hbuf ) );
    
    CleanupStack::PopAndDestroy( hbuf );
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecDecoder::EightbitAlphabetDecodeL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecDecoder::RemoveTrailingCR
// Removes the trailing CRs from a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CCbsRecDecoder::RemoveTrailingCR( 
    const TDesC& aText )
    {
    TInt i( aText.Length() );
    for ( ; i > 0 && aText[ i - 1 ] == EKeyEnter; i-- )
        {
        // nothing
        }

    return aText.Left( i );
    }
    
// -----------------------------------------------------------------------------
// CCbsRecDecoder::RemoveTrailingControlChars
// Removes the trailing control characters from a message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC8 CCbsRecDecoder::RemoveTrailingControlChars( 
    const TDesC8& aText )
    {
    TInt i( aText.Length() );
    for ( ; i > 0 && ( aText[ i - 1 ] == EKeyEnter || aText[ i - 1 ] == EKeyLineFeed ); i-- )
        {
        // nothing
        }

    return aText.Left( i );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  
