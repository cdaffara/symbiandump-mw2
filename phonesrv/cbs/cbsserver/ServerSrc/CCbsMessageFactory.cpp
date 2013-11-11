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
* Description:  Implementation of CCbsMessageFactory class.
*
*/


// INCLUDE FILES
#include <featmgr.h>

#include "CCbsMessageFactory.h"
#include "CCbsRecMessage.h"
#include "CCbsRecWcdmaMessage.h"
#include "CCbsRecEtel.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsMessageFactory::CCbsMessageFactory
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMessageFactory::CCbsMessageFactory( CCbsRecEtel& aRecEtel )
: iRecEtel( aRecEtel )
    {    
    }

// Destructor    
CCbsMessageFactory::~CCbsMessageFactory()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessageFactory::~CCbsMessageFactory()");
    CBSLOGSTRING("CBSSERVER: <<< CCbsMessageFactory::~CCbsMessageFactory()");
    }

// -----------------------------------------------------------------------------
// CCbsMessageFactory::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsMessageFactory* CCbsMessageFactory::NewL( CCbsRecEtel& aRecEtel )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessageFactory::NewL()");

    CCbsMessageFactory* self = new ( ELeave ) CCbsMessageFactory( aRecEtel );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsMessageFactory::NewL()");
    return self;
    }

// -----------------------------------------------------------------------------
// CCbsMessageFactory::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsMessageFactory::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CCbsMessageFactory::CreateMessageL
// Creates a CB message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CCbsMessage* CCbsMessageFactory::CreateMessageL( const TDes8& aMessagePage,
    const RMobileBroadcastMessaging::TMobileBroadcastAttributesV2& aAttributes,
    TCbsMessageType& aMsgType )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsMessageFactory::CreateMessageL()");

    CCbsMessage* currentMessage;
    currentMessage = NULL;
    
    if ( aAttributes.iFormat == RMobileBroadcastMessaging::EFormatWcdmaTpdu )
        {
        // Accept WCDMA messages only if the feature is supported.
        FeatureManager::InitializeLibL();
        
        if ( FeatureManager::FeatureSupported( KFeatureIdFf3gCellbroadcast ) )
            {
            CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): WCDMA msg...");
            currentMessage = CCbsRecWcdmaMessage::NewL( 
                aMessagePage, aAttributes );        
            CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): WCDMA msg created OK.");

            iAttributes = aAttributes;

            // Set the message type
            aMsgType = ECbsMessageWcdma;
            }
        else
            {
            CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): WCDMA messages not supported, currentMessage set to NULL.");
            currentMessage = NULL;
            }
        FeatureManager::UnInitializeLib();
        }
    else if ( aAttributes.iFormat == RMobileBroadcastMessaging::EFormatGsmTpdu ||
        aAttributes.iFormat == RMobileBroadcastMessaging::EFormatUnspecified )
        {        
        CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): GSM msg...");
        currentMessage = CCbsRecMessage::NewL( aMessagePage );        
        CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): GSM msg created OK.");

        // Set the message type
        aMsgType = ECbsMessageGsm;
        }
    else
        {        
        CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): Unknown msg type, currentMessage set to NULL.");
        currentMessage = NULL;
        }

    if ( currentMessage )
        {
        // Livecast messages are handled separately
        if ( currentMessage->IsLivecastMessage() )
            {        
            CBSLOGSTRING("CBSSERVER: CCbsMessageFactory::CreateMessageL(): This is a LC msg, setting type to ECbsMessageLivecast.");
            // Set the message type
            aMsgType = ECbsMessageLivecast;    
            }
        }
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsMessageFactory::CreateMessageL(), returning currentMessage");
    return currentMessage;    
    }

// -----------------------------------------------------------------------------
// CCbsMessageFactory::CreateMessageL
// Creates a CB message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
CCbsMessage* CCbsMessageFactory::CreateMessageL( const CCbsMessage& aMessage )
    {
    CCbsMessage* message;
    message = NULL;
    
    if ( iAttributes.iFormat == RMobileBroadcastMessaging::EFormatWcdmaTpdu )
        {
        message = CCbsRecWcdmaMessage::NewL( aMessage );
        }
    else if ( iAttributes.iFormat == RMobileBroadcastMessaging::EFormatGsmTpdu ||
        iAttributes.iFormat == RMobileBroadcastMessaging::EFormatUnspecified )
        {
        message = CCbsRecMessage::NewL( aMessage );
        }

    return message;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
