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
* Description:  This module contains the implementation of CCbsEtelMessaging
*                class member functions.
*   
*                CCbsEtelMessaging acts as a wrapper to RMobileBroadcastMessaging instance. 
*
*/



// INCLUDE FILES
    /*******************************
    *	Series 60 Customer / Etel
    *	Series 60 Etel API
    *******************************/
#include <mmretrieve.h>
#include "CCbsEtelMessaging.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::CCbsEtelMessaging
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsEtelMessaging::CCbsEtelMessaging()
    {
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsEtelMessaging::ConstructL()
    {    
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsEtelMessaging* CCbsEtelMessaging::NewL()
    {
    CCbsEtelMessaging* self = new ( ELeave ) CCbsEtelMessaging;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    
    return self;
    }

    
// Destructor
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
CCbsEtelMessaging::~CCbsEtelMessaging()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsEtelMessaging::~CCbsEtelMessaging()");
    iMessaging.Close();
    CBSLOGSTRING("CBSSERVER: <<< CCbsEtelMessaging::~CCbsEtelMessaging()");
    }   

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::Open
// Opens an ETel messaging session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
TInt CCbsEtelMessaging::Open( 
    RMobilePhone& aPhone )
    {
    return iMessaging.Open( aPhone );
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::Close
// Closes ETel messaging session.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
void CCbsEtelMessaging::Close()
    {
    iMessaging.Close();
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::ReceiveMessage
// Requests forwarding of the next received CB message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
void CCbsEtelMessaging::ReceiveMessage(
    TRequestStatus& aReqStatus, 
    TDes8& aMsgData, 
    TDes8& aMsgAttributes )
    {
    iMessaging.ReceiveMessage( aReqStatus, aMsgData, aMsgAttributes );
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::ReceiveMessageCancel
// Cancels an outstanding ReceiveMessage() request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
void CCbsEtelMessaging::ReceiveMessageCancel()
    {
    // Use the base class method to cancel    
    iMessaging.CancelAsyncRequest( EMobileBroadcastMessagingReceiveMessage );
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::SetFilterSetting
// Sets CBS receive mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
void CCbsEtelMessaging::SetFilterSetting(
    TRequestStatus& aReqStatus, 
    RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting ) const
    {
    iMessaging.SetFilterSetting( aReqStatus, aSetting );
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::SetFilterSettingCancel
// Cancels active SetFilterSetting request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
void CCbsEtelMessaging::SetFilterSettingCancel()
    {
    // Use the base class method to cancel
    iMessaging.CancelAsyncRequest( EMobileBroadcastMessagingSetFilterSetting );
    }

// -----------------------------------------------------------------------------
// CCbsEtelMessaging::GetFilterSetting
// Retrieves the current CBS receive mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
        /*******************************
        *	Series 60 Customer / Etel
        *	Series 60 Etel API
        *******************************/
TInt CCbsEtelMessaging::GetFilterSetting(
    RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter& aSetting )
    {
    return iMessaging.GetFilterSetting( aSetting );
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//	End of File  
