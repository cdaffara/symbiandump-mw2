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
* Description:  
*
*/


// INCLUDE FILES
#include "CCbsLivecastHandler.h"
#include "CCbsRecMessage.h"
#include "CCbsRecWcdmaMessage.h"
#include "CCbsRecEtel.h"
#include "CCbsRecCollector.h"
#include "CCbsReceiverHelper.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsLivecastHandler::CCbsLivecastHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsLivecastHandler::CCbsLivecastHandler( CCbsRecEtel& aRecEtel )
: iRecEtel( aRecEtel )
    {    
    }

// Destructor    
CCbsLivecastHandler::~CCbsLivecastHandler()
    {
    CBSLOGSTRING("CBSSERVER: >>> ~CCbsLivecastHandler::~CCbsLivecastHandler()");
    CBSLOGSTRING("CBSSERVER: <<< ~CCbsLivecastHandler::~CCbsLivecastHandler()");
    }

// -----------------------------------------------------------------------------
// CCbsLivecastHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsLivecastHandler* CCbsLivecastHandler::NewL( CCbsRecEtel& aRecEtel )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsLivecastHandler::NewL()");

    CCbsLivecastHandler* self = new ( ELeave ) CCbsLivecastHandler( aRecEtel );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsLivecastHandler::NewL()");
    return self;
    }

// -----------------------------------------------------------------------------
// CCbsLivecastHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsLivecastHandler::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CCbsLivecastHandler::HandleLivecastMessageL
// Processes the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
void CCbsLivecastHandler::HandleLivecastMessageL( 
    CCbsMessage* aLivecastMessage )
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsLivecastHandler::HandleLivecastMessageL()");

    // Take ownership
    CleanupStack::PushL( aLivecastMessage );

    // Check if this message must be rejected:
    // 1. Is message compressed
    // 2. Is message defined as 'Class 2' or 'Class 3' message

	TInt msgclass ( aLivecastMessage->MessageClass() );

	if ( aLivecastMessage->IsCompressed() ||
		msgclass == 3 || msgclass == 2 )
        {		
        CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Rejected, compressed/msg class.");

        // Reject the message
        CleanupStack::PopAndDestroy(); // aLivecastMessage
        }
    else
        {
        // Add network information to message
        TInt error = iRecEtel.AddNetworkInfo( *aLivecastMessage );

		CBSLOGSTRING2("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): AddNetworkInfo() error: %d", error );

        // Handle the message
        // Is message multipaged
        if ( aLivecastMessage->TotalPages() > 1 )
            {			
            CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Message multipaged.");

            // Ensure that the network info is available. 
            // If not, reject the message, because info is required to collect
            // pages of a multipaged message.
            if ( KErrNone != error )
                {				
                CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Message rejected, AddNetworkInfo() error.");
                CleanupStack::PopAndDestroy( aLivecastMessage );
                }
            else
                {				
                CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Sending msg to Collector...");

                // Ownership of aLivecastMessage transferred to collector with
                // aLivecastMessage left on the cleanup stack.                
                CCbsMessage* completedMsg = iRecEtel.Collector().CollectL( aLivecastMessage, ECbsMessageLivecast );
                
                CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Collector finished.");

                if ( completedMsg )
                    {					
                    CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Msg collected.");

                    // Message pages combined => Handle like a single-paged msg.
                    // Result code ignored.
                    CleanupStack::PushL( completedMsg );
					
                    CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Routing msg to RecEtel...");
					iRecEtel.RouteMessageL( *completedMsg );					
                    CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Routing finished.");

                    CleanupStack::PopAndDestroy( completedMsg );
                    }
                }
            }
        // Message is single paged
        else            
            {			
            CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Message single paged.");            

            iRecEtel.RouteMessageL( *aLivecastMessage );			
            CBSLOGSTRING("CBSSERVER: CCbsLivecastHandler::HandleLivecastMessageL(): Single paged RouteMessageL() OK.");

            CleanupStack::PopAndDestroy( aLivecastMessage );
            }        
        }
	CBSLOGSTRING("CBSSERVER: <<< CCbsLivecastHandler::HandleLivecastMessageL()");
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
