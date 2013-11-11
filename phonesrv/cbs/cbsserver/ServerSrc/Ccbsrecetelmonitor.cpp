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
* Description:  This module contains the implementation of CCbsRecEtelMonitor class 
*                member functions.    
*
*/



// INCLUDE FILES

#include <e32math.h>
#include "CbsServerConstants.h"
#include "CCbsRecEtel.h"
#include "CCbsRecEtelMonitor.h"
#include "CCbsRecMessage.h"
#include "CCbsRecWcdmaMessage.h"
#include "CCbsMessageFactory.h"
#include "CbsServerPanic.h"
#include "CCbsLivecastHandler.h"
#include <featmgr.h>
#include <bldvariant.hrh> // for feature definitions
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::CCbsRecEtelMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsRecEtelMonitor::CCbsRecEtelMonitor( 
    CCbsRecEtel& aEtel, 
    MCbsEtelMessaging& aMessaging,
    CCbsMessageFactory& aFactory,
    CCbsLivecastHandler& aLivecastHandler )
    : CActive( EPriorityStandard ),
    iEtel( aEtel ), 
    iSmsMessaging( aMessaging ),        
    iMsgAttributesPckg( iAttributes ),   // Message attributes
    iFactory( aFactory ),
    iLivecastHandler( aLivecastHandler ),
    iNewstickerSupported( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecEtelMonitor::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtelMonitor::ConstructL()");

    CActiveScheduler::Add( this );    

    // Sets up TLS, must be done before FeatureManager is used.
    FeatureManager::InitializeLibL();

    // Check if Newsticker feature is supported
    if ( FeatureManager::FeatureSupported( KFeatureIdNewsticker ) )
        {
        iNewstickerSupported = ETrue;
        CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::ConstructL(): Newsticker supported.");
        }
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::ConstructL(): Newsticker NOT supported.");
        }

    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
    
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsRecEtelMonitor* CCbsRecEtelMonitor::NewL( 
    CCbsRecEtel& aEtel, 
    MCbsEtelMessaging& aSmsMessaging,
    CCbsMessageFactory& aFactory,
    CCbsLivecastHandler& aLivecastHandler )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtelMonitor::NewL()");

    CCbsRecEtelMonitor* self = 
        new ( ELeave ) CCbsRecEtelMonitor( aEtel, aSmsMessaging, aFactory, aLivecastHandler );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::NewL()");
    return self;
    }
    
// Destructor
CCbsRecEtelMonitor::~CCbsRecEtelMonitor()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtelMonitor::~CCbsRecEtelMonitor()");
    Cancel();
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::~CCbsRecEtelMonitor()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::IssueRequest
// Requests EPOC Telephony Server to forward next received CB message to this object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtelMonitor::IssueRequest() 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtelMonitor::IssueRequest()");

    if ( !IsActive() )
        {
        iMsgData.FillZ();
        iSmsMessaging.ReceiveMessage( iStatus, iMsgData, iMsgAttributesPckg );
        SetActive(); 
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::IssueRequest()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::RunL
// Becomes active when EPOC Telephony Server has copied a CB message to 
// iMsgData. The message is then given to CCbsRecEtel for
// processing.
// Finally this function re-issues the ReceiveMessage request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtelMonitor::RunL()
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsRecEtelMonitor::RunL(), iStatus: %d", iStatus.Int() );

    if ( iStatus == KErrNone )
        {
        CCbsMessage* currentMessage = NULL;
        TCbsMessageType msgType( ECbsMessageTypeUnspecified );
		
        CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): Calling CreateMessageL()..." );
        currentMessage = iFactory.CreateMessageL( iMsgData, iAttributes, msgType );        
        CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): CreateMessageL() called OK." );

        if ( currentMessage )
            {            
            if ( msgType == ECbsMessageLivecast )
                {				
                CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): LC message" );

				if ( iNewstickerSupported )
					{
	                // Ownership of currentMessage transferred to iLivecastHandler
	                CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): Calling HandleLivecastMessageL()..." );
	                TInt lcResult( KErrNone );
	                TRAP( lcResult, iLivecastHandler.HandleLivecastMessageL( currentMessage ) );                
	                CBSLOGSTRING2("CBSSERVER: CCbsRecEtelMonitor::RunL(): HandleLivecastMessageL() finished, result: %d", lcResult );
					}
                else 
                	{
                	CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): NewsSticker is not supported" );
                	}
                }
            else
                {				
                CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): Normal message" );

                // Ownership of currentMessage transferred to iEtel
                CBSLOGSTRING("CBSSERVER: CCbsRecEtelMonitor::RunL(): Calling HandleMessageReceivedL()..." );
                TInt result( KErrNone );
                TRAP( result, iEtel.HandleMessageReceivedL( currentMessage ) );                
                CBSLOGSTRING2("CBSSERVER: CCbsRecEtelMonitor::RunL(): HandleMessageReceivedL() finished, result: %d", result );
                }            
            }

        // Renew the request
        IssueRequest();
        }
	CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::RunL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtelMonitor::DoCancel
// Cancels an outstanding ReceiveMessage request.  
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtelMonitor::DoCancel()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtelMonitor::DoCancel()" );
    iSmsMessaging.ReceiveMessageCancel();
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtelMonitor::DoCancel()" );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File  
