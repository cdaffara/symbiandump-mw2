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
* Description:  This module contains the implementation of CCbsRecMessage class 
*                member functions.
*    
*                CCbsRecMessage represents a message page generated by CCbsRecEtelMonitor.
*                CCbsRecMessage is constructed from a CB message page received from 
*                ETel server.   
*
*/



// INCLUDE FILES
#include "CCbsRecMessage.h"
#include "CbsServerPanic.h"
#include "CbsLogger.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CCbsRecMessage::CCbsRecMessage
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecMessage::CCbsRecMessage()
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecMessage::ConstructL( 
    const TDes8& aMessagePage )    // ETel received msg page
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecMessage::ConstructL() (TDes8 from ETel)");

    // Retrieve values for message header information.
    iSerialNumber = TUint16( ( aMessagePage[ 0 ] << 8 ) | aMessagePage [ 1 ] );
    iMessageIdentifier = TUint16( ( aMessagePage[ 2 ] << 8 ) | aMessagePage[ 3 ] );
    iDCS = aMessagePage[ 4 ];    
    iPageParameter = aMessagePage[ 5 ];

    if ( ThisPage() == 0 || TotalPages() == 0 )
        {
        iPageParameter = KPageParameterSingle;
        }

    CBSLOGSTRING2("CBSSERVER: CCbsRecMessage::ConstructL(): iSerialNumber: %d", iSerialNumber );
    CBSLOGSTRING2("CBSSERVER: CCbsRecMessage::ConstructL(): iMessageIdentifier: %d", iMessageIdentifier );
    CBSLOGSTRING2("CBSSERVER: CCbsRecMessage::ConstructL(): iDCS: %d", iDCS );
    CBSLOGSTRING2("CBSSERVER: CCbsRecMessage::ConstructL(): iPageParameter: %d", iPageParameter );

    // 16-bit representation not prepared.
    iMessage = NULL;

    // A buffer for the message contents.
    iMessage8 = HBufC8::NewL( RMobileBroadcastMessaging::KGsmBroadcastDataSize );
    TPtr8 messagePtr = iMessage8->Des();
	TPtrC8 sourceMessagePtr;

	if ( ThisPage() == 1 && IsLivecastMessage () ) 
		{
		sourceMessagePtr.Set( aMessagePage.Right( aMessagePage.Length() ) );
		}
	else
		{
		sourceMessagePtr.Set( aMessagePage.Right( aMessagePage.Length() - KHeaderLength ) );
		}

    // Copy the TCbs page contents into this buffer.
    messagePtr.Copy( sourceMessagePtr );

    // Indicate that we have no network info available
    iNetworkInfo.iCountryCode = KRecMessageNoNetworkInfo;

    // Set network mode
    iNetworkMode = ECbsNetworkGsm;

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecMessage::ConstructL() (TDes8 from ETel)");
    }
    
// -----------------------------------------------------------------------------
// CCbsRecMessage::ConstructL
// Symbian 2nd phase copyconstructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecMessage::ConstructL( 
    const CCbsMessage& aMessagePage )
    {
    iSerialNumber = aMessagePage.Key();
    iMessageIdentifier = aMessagePage.TopicNumber();
    iDCS = aMessagePage.DCS();
    iPageParameter = aMessagePage.PageParameter();

    iLanguage = aMessagePage.Language();

    // A buffer for the message contents.
    if ( aMessagePage.Message() != NULL )
        {
        iMessage = HBufC::NewL( aMessagePage.Contents().Length() );
        iMessage->Des().Copy( aMessagePage.Contents() );
        }

    if ( aMessagePage.Message8() != NULL )
        {
        iMessage8 = HBufC8::NewL( aMessagePage.Contents8().Length() );
        TPtr8 messagePtr = iMessage8->Des();
        messagePtr.Copy( aMessagePage.Contents8() );
        }
    }
    
// -----------------------------------------------------------------------------
// CCbsRecMessage::NewL
// Two-phased constructor.
// Constructs a new CCbsRecMessage based on TDes8&
// (which is a descriptor) received from EPOC Telephony Server.
// -----------------------------------------------------------------------------
//
CCbsRecMessage* CCbsRecMessage::NewL( 
    const TDes8& aMessagePage ) // page received from EPOC Telephony Server 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecMessage::NewL() (TDes8 from ETel)");

    CCbsRecMessage* self = new ( ELeave ) CCbsRecMessage();
    CleanupStack::PushL( self );
    self->ConstructL( aMessagePage );
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecMessage::NewL() (TDes8 from ETel)");
    return self;
    } 

// -----------------------------------------------------------------------------
// CCbsRecMessage::NewL
// Constructs a copy of the message given as parameter.
// -----------------------------------------------------------------------------
//
CCbsRecMessage* CCbsRecMessage::NewL( 
    const CCbsMessage& aMessagePage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecMessage::NewL() (copy CCbsMessage)");

    CCbsRecMessage* self = new ( ELeave ) CCbsRecMessage;
    CleanupStack::PushL( self );
    self->ConstructL( aMessagePage );
    CleanupStack::Pop();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecMessage::NewL() (copy CCbsMessage)");
    return self;
    }
    
// Destructor
CCbsRecMessage::~CCbsRecMessage()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecMessage::~CCbsRecMessage()");
    delete iMessage;
    delete iMessage8;
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecMessage::~CCbsRecMessage()");
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::ThisPage
// Returns the page number of this page in the message page sequence
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TUint CCbsRecMessage::ThisPage() const
    {
    return PageParameter() >> 4;
    }

// -----------------------------------------------------------------------------
// CCbsRecMessage::TotalPages
// Returns the total number of pages in this page's message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TUint CCbsRecMessage::TotalPages() const
    {
    return PageParameter() & 0xF;    // 1111b
    }


// ============ PRIVATE FUNCTIONS ==========================

// -----------------------------------------------------------------------------
// CCbsRecMessage::MessageType
// Returns the message type of this message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsRecMessage::MessageType() const
    {
    return KErrNotSupported;
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//  End of File  
