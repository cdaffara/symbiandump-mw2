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
* Description:  This module contains the implementation of CCbsSetFilterSettingHandler 
*                class member functions
*
*/


//  INCLUDE FILES
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>
#include <mmlist.h>
#include "CCbsSetFilterSettingHandler.h"
#include "CCbsRecEtel.h"
#include "CCbsRecEtelMonitor.h"
#include "CbsServerConstants.h"
#include "CbsServerPanic.h"
#include "CbsLogger.h"


// ==================== LOCAL FUNCTIONS =====================================


// ================= MEMBER FUNCTIONS =======================================

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::CCbsSetFilterSettingHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsSetFilterSettingHandler::CCbsSetFilterSettingHandler( 
    CCbsRecEtel& aEtel, 
    MCbsEtelMessaging& aMessaging 
    ) : CActive( EPriorityStandard ),
    iEtel( aEtel ),
    iSmsMessaging( aMessaging )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsSetFilterSettingHandler::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsSetFilterSettingHandler* CCbsSetFilterSettingHandler::NewL( 
    CCbsRecEtel& aEtel, 
    MCbsEtelMessaging& aMessaging )
    {
    CCbsSetFilterSettingHandler* self = 
        new ( ELeave ) CCbsSetFilterSettingHandler( aEtel, aMessaging );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
// Destructor
CCbsSetFilterSettingHandler::~CCbsSetFilterSettingHandler()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsSetFilterSettingHandler::~CCbsSetFilterSettingHandler()"); 
    Cancel();
    CBSLOGSTRING("CBSSERVER: <<< CCbsSetFilterSettingHandler::~CCbsSetFilterSettingHandler()"); 
    }

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::SetFilterSetting
// Requests ETel to change the filter setting.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
void CCbsSetFilterSettingHandler::SetFilterSetting( 
    RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting )
    {
    if ( IsActive() )
        {
        Cancel();
        }
    iSmsMessaging.SetFilterSetting( iStatus, aSetting );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::RunL
// Called after ETel has completed the SetFilterSetting request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSetFilterSettingHandler::RunL()   
    {
    if ( iStatus != KErrNone )
        {        
        // Causes a call to RunError()
        User::Leave( iStatus.Int() );
        }
    }

// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::DoCancel
// Cancels an outstanding SetFilterSetting request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSetFilterSettingHandler::DoCancel()   
    {
    iSmsMessaging.SetFilterSettingCancel();
    }
    
// -----------------------------------------------------------------------------
// CCbsSetFilterSettingHandler::RunError
// Is called if RunL() leaves.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
TInt CCbsSetFilterSettingHandler::RunError( TInt aError )
    {
    CBSLOGSTRING2("CBSSERVER: >>> CCbsSetFilterSettingHandler::RunError(): Error code: %d", aError ); 
    CBSLOGSTRING("CBSSERVER: <<< CCbsSetFilterSettingHandler::RunError(), returning KErrNone." );
    
    // To prevent ARMV5 compiler warnings
    if ( aError )
        {
        aError = KErrNone;
        }
    return KErrNone; 
    }


// ================= OTHER EXPORTED FUNCTIONS ===============================


//  End of File 
