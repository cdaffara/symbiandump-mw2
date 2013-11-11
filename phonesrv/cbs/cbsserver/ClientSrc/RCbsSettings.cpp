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
* Description:  This module contains the implementation of RCbsSettings class 
    		 member functions.
*
*/


// INCLUDE FILES

#include "CbsServerConstants.h"
#include "RCbsTopicMessages.h"
#include "RCbsSettings.h"
#include "RCbs.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
RCbsSettings::RCbsSettings()
    : iNotifyEventPtr( NULL, 0, 0 )
    {
    }

// -----------------------------------------------------------------------------
// RCbsSettings::Open
// Creates a subsession to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt RCbsSettings::Open( 
    RCbs& aServer )
    {
    // Open the subsession.
    const TIpcArgs args( TIpcArgs::ENothing );
    return CreateSubSession( aServer, ECbsCreateSettingsSubsession, args );
    }
// -----------------------------------------------------------------------------
// RCbsSettings::Close
// Closes the subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt RCbsSettings::Close()
    {
    // Close the subsession.
    RSubSessionBase::CloseSubSession( ECbsCloseSettingsSubsession );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RCbsSettings::GetReceptionStatus
// Requests the reception status from the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsSettings::GetReceptionStatus( 
    TBool& aStatus )
    {
    // Send request to the server. Get the current reception status.
    aStatus = EFalse;
    TPckgBuf< TBool > pckg( aStatus );
    const TIpcArgs args( &pckg );

    SendReceive( ECbsGetReceptionStatus, args );

    aStatus = pckg();
    }

// -----------------------------------------------------------------------------
// RCbsSettings::SetReceptionStatus
// Changes the reception status to aStatus.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsSettings::SetReceptionStatus( 
    TBool aStatus )
    {  
    // Change the reception status
    TPckgBuf< TBool > pckg( aStatus );
    const TIpcArgs args( &pckg );
    return SendReceive( ECbsSetReceptionStatus, args );
    }

// -----------------------------------------------------------------------------
// RCbsSettings::GetTopicDetectionStatus
// Requests the current topic detection status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void RCbsSettings::GetTopicDetectionStatus( 
    TBool& aStatus )
    {
    // Get the current topic detection status.
    TPckgBuf< TBool > pckg;
    const TIpcArgs args( &pckg );
    SendReceive( ECbsGetTopicDetectionStatus, args );
    aStatus = pckg();
    }

// -----------------------------------------------------------------------------
// RCbsSettings::SetTopicDetectionStatus
// Sets a new topic detection status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsSettings::SetTopicDetectionStatus( 
    TBool aStatus )
    {
    // Change the topic detection status.
    TPckgBuf< TBool > pckg( aStatus );
    const TIpcArgs args( &pckg );
    return SendReceive( ECbsSetTopicDetectionStatus, args );
    }

// -----------------------------------------------------------------------------
// RCbsSettings::GetLanguages
// Returns the preferred languages in aLanguages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void RCbsSettings::GetLanguages( 
    TCbsSettingsLanguages& aLanguages )
    {
    // Get the preferred languages
    TPckgBuf< TCbsSettingsLanguages > pckg;
    const TIpcArgs args( &pckg );
    SendReceive( ECbsGetLanguages, args );
    aLanguages = pckg();
    }

// -----------------------------------------------------------------------------
// RCbsSettings::SetLanguages
// Sets a new set of languages to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
TInt RCbsSettings::SetLanguages( 
    const TCbsSettingsLanguages& aLanguages )
    {
    // Change the preferred languages
    TPckgBuf< TCbsSettingsLanguages > pckg( aLanguages );
    const TIpcArgs args( &pckg );
    return SendReceive( ECbsSetLanguages, args );
    }

// -----------------------------------------------------------------------------
// RCbsSettings::NotifySettingsChanged
// Requests the server to notify the client whenever any settings 
// will be changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void RCbsSettings::NotifySettingsChanged( 
    TRequestStatus& aStatus, 
    TCbsSettingsEvent& aEvent )
    {
    // Set up the notify! 
    iNotifyEventPtr.Set( reinterpret_cast<TText8*>( &aEvent ), 
                         sizeof( TCbsSettingsEvent ),
                         sizeof( TCbsSettingsEvent ) );

    // Server writes through the pointer descriptor
    const TIpcArgs args( &iNotifyEventPtr );
    SendReceive( ECbsNotifySettingsChanged, args, aStatus );
    }

// -----------------------------------------------------------------------------
// RCbsSettings::NotifySettingsChangedCancel
// Cancels the request to notify the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void RCbsSettings::NotifySettingsChangedCancel()
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    SendReceive( ECbsNotifySettingsChangedCancel, args );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File  
