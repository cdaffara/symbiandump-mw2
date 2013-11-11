/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsRecWcdmaMessage 
*                class member functions.
*    
*                CCbsRecWcdmaMessage represents a WCDMA message page generated 
*                by CCbsRecEtelMonitor. CCbsRecWcdmaMessage is constructed from a
*                CB message page received from ETel server.   
*
*
*/



// INCLUDE FILES

#include <e32svr.h>
#include "CCbsRecWcdmaMessage.h"
#include "CbsServerPanic.h"
#include "CbsLogger.h"

// The empty message length. 1 octet is for page parameter 
const TInt KCbsRecWcdmaMessageMinLength = 1;
// The maximum message length
const TInt KCbsRecWcdmaMessageMaxLength = 82;
// The value that use for parse the page parameter octet.
const TInt KCbsRecWcdmaPageOfNumberMaskForLow4 = 0x000f;


// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::CCbsRecWcdmaMessage
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecWcdmaMessage::CCbsRecWcdmaMessage( 
    const RMobileBroadcastMessaging::TMobileBroadcastAttributesV2& aAttributes )    
    : iWcdmaAttributes( aAttributes )
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::CCbsRecWcdmaMessage
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecWcdmaMessage::CCbsRecWcdmaMessage()
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecWcdmaMessage::ConstructL( 
    const TDes8& aMessagePage,    // ETel received msg page
    const RMobileBroadcastMessaging::TMobileBroadcastAttributesV2& aAttributes )
    {
    // Message attributes
    iTotalNumberOfPages = 
     ( aAttributes.iNumberOfPages & KCbsRecWcdmaPageOfNumberMaskForLow4 ) ?
     ( aAttributes.iNumberOfPages & KCbsRecWcdmaPageOfNumberMaskForLow4 ) : 1;

    iMessageType = aAttributes.iMessageType;
    iMessageIdentifier = aAttributes.iMessageId;
    iSerialNumber = aAttributes.iSerialNum;
    iDCS = aAttributes.iDCS;

    // 16-bit representation not prepared.
    iMessage = NULL;
    // A buffer for the message contents.
    iMessage8 = HBufC8::NewL( RMobileBroadcastMessaging::KWCdmaBroadcastPageSize );    
    TPtr8 messagePtr = iMessage8->Des();
    messagePtr.Trim();

    TPtrC8 sourceMessagePtr;
    TInt msgLength = aMessagePage.Length();
    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::\
                   ConstructL(): msgLength: %d", msgLength );

    // Message from ETel for 3G has no header and CTSY will append the page 
    // parameter to the last of message. So the non-empty message's length
    // must more than 1 octet. The max length for one message page must
    // less than 82 octets. Additional octets will be omitted.
    if ( msgLength > KCbsRecWcdmaMessageMinLength )
        {
        // Number of this page. Taken form the last byte of the message. 
        // Max 15, therefore masked with 0xF
        iPageParameter = 
            ( aMessagePage[ msgLength - KCbsRecWcdmaMessageMinLength ] 
            & KCbsRecWcdmaPageOfNumberMaskForLow4 ) ?
            ( aMessagePage[ msgLength - KCbsRecWcdmaMessageMinLength ] 
              & KCbsRecWcdmaPageOfNumberMaskForLow4 ) : 1;
        
        sourceMessagePtr.Set( aMessagePage.Left( msgLength - 
                    KCbsRecWcdmaMessageMinLength) );

        if ( KCbsRecWcdmaMessageMaxLength < sourceMessagePtr.Length() )
            {
            sourceMessagePtr.Set( aMessagePage.Left( 
                        KCbsRecWcdmaMessageMaxLength ) );
            }
        }

    // Copy the TCbs page contents into this buffer.
    messagePtr.Copy( sourceMessagePtr );

    // Indicate that we have no network info available
    iNetworkInfo.iCountryCode = KRecMessageNoNetworkInfo;

    // Set network mode
    iNetworkMode = ECbsNetworkWcdma;

    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::ConstructL(): \
                   iTotalNumberOfPages: %d", iTotalNumberOfPages );
    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::ConstructL(): \
                   iSerialNumber: %d", iSerialNumber );
    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::ConstructL(): \
                   iMessageIdentifier: %d", iMessageIdentifier );
    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::ConstructL(): \
                   iDCS: %d", iDCS );
    CBSLOGSTRING2("CBSSERVER: CCbsRecWcdmaMessage::ConstructL(): \
                   iPageParameter: %d", iPageParameter );
    }

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecWcdmaMessage::ConstructL( const CCbsMessage& aMessage )
    {
    // Message attributes
    iTotalNumberOfPages = aMessage.TotalPages();
    iMessageType = (TUint8) aMessage.MessageType();
    iMessageIdentifier = aMessage.TopicNumber();
    iSerialNumber = aMessage.Key();
    iDCS = aMessage.DCS();
    // Number of this page, extracted from last octet of the message data
    iPageParameter = aMessage.PageParameter();

    iLanguage = aMessage.Language();

    // A buffer for the message contents.
    if ( aMessage.Message() )
        {
        iMessage = HBufC::NewL( aMessage.Contents().Length() );
        iMessage->Des().Copy( aMessage.Contents() );
        }

    if ( aMessage.Message8() )
        {
        iMessage8 = HBufC8::NewL( aMessage.Contents8().Length() );
        TPtr8 messagePtr = iMessage8->Des();
        messagePtr.Copy( aMessage.Contents8() );
        }
    }
   
// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::NewL
// Two-phased constructor.
// Constructs a new CCbsRecWcdmaMessage based on TDes8& and message attributes
// (which is a descriptor) received from EPOC Telephony Server.
// -----------------------------------------------------------------------------
//
CCbsRecWcdmaMessage* CCbsRecWcdmaMessage::NewL( 
    const TDes8& aMessagePage,  // page received from EPOC Telephony Server 
    const RMobileBroadcastMessaging::TMobileBroadcastAttributesV2& aAttributes ) 
    {
    CCbsRecWcdmaMessage* self = new ( ELeave ) CCbsRecWcdmaMessage( aAttributes );
    CleanupStack::PushL( self );
    self->ConstructL( aMessagePage, aAttributes );
    CleanupStack::Pop( self );
    return self;
    } 

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::NewL
// Two-phased constructor.
// Constructs a new CCbsRecWcdmaMessage based on an existing message.
// -----------------------------------------------------------------------------
//
CCbsRecWcdmaMessage* CCbsRecWcdmaMessage::NewL( const CCbsMessage& aMessage )
    {
    CCbsRecWcdmaMessage* self = new ( ELeave ) CCbsRecWcdmaMessage();
    CleanupStack::PushL( self );
    self->ConstructL( aMessage );
    CleanupStack::Pop( self );
    return self;
    }

   
// Destructor
CCbsRecWcdmaMessage::~CCbsRecWcdmaMessage()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecWcdmaMessage::~CCbsRecWcdmaMessage()");
    delete iMessage;
    delete iMessage8;
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecWcdmaMessage::~CCbsRecWcdmaMessage()");
    }
    
// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::ThisPage
// Returns the page number of this page in the message page sequence
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TUint CCbsRecWcdmaMessage::ThisPage() const
    {
    return PageParameter();    
    }

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::TotalPages
// Returns the total number of pages in this page's message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TUint CCbsRecWcdmaMessage::TotalPages() const
    {
    return iTotalNumberOfPages;
    }

// -----------------------------------------------------------------------------
// CCbsRecWcdmaMessage::MessageType
// Returns the message type of this message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsRecWcdmaMessage::MessageType() const
    {
    return iMessageType;
    }

//  End of File  
