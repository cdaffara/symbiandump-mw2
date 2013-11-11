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
* Description:  This module contains the implementation of CCbsSettings class 
*                member functions. 
*    
*                The server-side CbsClient subsession for modifying settings.
*
*/


// INCLUDE FILES

#include "CbsCommon.h"
#include "CbsServerPanic.h"
#include "CbsServerConstants.h"
#include "CCbsSettings.h"
#include "CCbsDbImpSettings.H"
#include "CCbsRecEtel.h"
#include "CCbsServer.h"
#include "CbsLogger.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsSettings::CCbsSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsSettings::CCbsSettings( 
    CCbsSession& aSession, 
    CCbsDbImpSettings& aSettings, 
    CCbsRecEtel& aReceiver )
    : CCbsObject( aSession ), 
    iSettings( aSettings ), 
    iReceiver( aReceiver )
    {
    }

// -----------------------------------------------------------------------------
// CCbsSettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsSettings::ConstructL()
    {
    // Add itself as an observer.
    iSettings.AddObserverL( this );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsSettings* CCbsSettings::NewL( 
    CCbsSession& aSession, 
    CCbsDbImpSettings& aSettings, 
    CCbsRecEtel& aReceiver )
    {
    CCbsSettings* self = 
        new ( ELeave ) CCbsSettings( aSession, aSettings, aReceiver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    
    return self;
    }

// Destructor
CCbsSettings::~CCbsSettings()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsSettings::~CCbsSettings()"); 
    iSettings.RemoveObserver( this );
    CBSLOGSTRING("CBSSERVER: <<< CCbsSettings::~CCbsSettings()"); 
    }

// -----------------------------------------------------------------------------
// CCbsSettings::HandleRequestsL
// Handles the requests for this subsession.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsSettings::HandleRequestsL( 
    const RMessage2& aMessage )
    {    
    CBSLOGSTRING("CBSSERVER: >>> CCbsSettings::HandleRequestsL()");
    
    TBool requestHandled( ETrue );
       
    // Check needed capabilities    
    TBool hasCapWrite( EFalse );
    hasCapWrite = aMessage.HasCapability( ECapabilityWriteDeviceData );
    
    CBSLOGSTRING2("CBSSERVER: CCbsSettings::HandleRequestsL(), caps checked: %d", hasCapWrite );
    
    // Get the request
    TInt function = aMessage.Function();
    
    // If caps are not ok, leave
    if ( ( function == ECbsSetReceptionStatus || 
        function == ECbsSetTopicDetectionStatus ||
        function == ECbsSetLanguages ) &&
        !hasCapWrite )
        {
        CBSLOGSTRING("CBSSERVER: CCbsSettings::HandleRequestsL(): Caps NOT OK, leaving with KErrPermissionDenied...");
        User::Leave( KErrPermissionDenied );
        }               
        
    // Handle the requests for the subsession
    switch ( function )
        {
        case ECbsCloseSettingsSubsession:           
            CloseSettings();
            aMessage.Complete( KErrNone );
            break;

        case ECbsGetReceptionStatus:
            GetReceptionStatusL();
            break;

        case ECbsSetReceptionStatus:            
            SetReceptionStatusL();
            break;

        case ECbsGetTopicDetectionStatus:
            GetTopicDetectionStatusL();
            break;

        case ECbsSetTopicDetectionStatus:
            SetTopicDetectionStatusL();
            break;

        case ECbsGetLanguages:
            GetLanguagesL();
            break;

        case ECbsSetLanguages:
            SetLanguagesL();
            break;

        case ECbsNotifySettingsChanged:
            NotifySettingsChanged();
            break;

        case ECbsNotifySettingsChangedCancel:            
            NotifySettingsChangedCancel();
            break;
        default:
            requestHandled = EFalse;
            break;
        }

    CBSLOGSTRING2("CBSSERVER: <<< CCbsSettings::HandleRequestsL(), returning requestHandled: %d", requestHandled );
    return requestHandled;
    }

// -----------------------------------------------------------------------------
// CCbsSettings::TopicDetectionStatusChangedIndL
// Called when the topic detection is changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::TopicDetectionStatusChangedIndL()
    {
    // Notify the client in case there is a pending notification request.
    NotifyClientL( ECbsModifiedTopicDetectionStatus );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::ReceptionStatusChangedIndL
// Called when reception status is changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::ReceptionStatusChangedIndL()
    {
    // Notify the client in case there is a pending notification request.
    NotifyClientL( ECbsModifiedReceptionStatus );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::LanguagesChangedIndL
// Called when reception status is changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::LanguagesChangedIndL()
    {
    // Notify the client in case there is a pending notification request.
    NotifyClientL( ECbsModifiedLanguages );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::LimitedReceptionStatusChangedIndL
// Called when reception status is changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::LimitedReceptionStatusChangedIndL()
    {
    // Notify the client in case there is a pending notification request.
    NotifyClientL( ECbsModifiedLimitedReceptionStatus );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::CleanupTimeChangedIndL
// Called when clean up time of read messages is changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::CleanupTimeChangedIndL()
    {
    // Notify the client in case there is a pending notification request.
    NotifyClientL( ECbsModifiedCleanupTime );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::CloseSettings
// Close the subsession.
// Note that the method will delete itself, so the object is no
// longer valid after the call.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::CloseSettings()
    {
    // Removes the object.
    Session().Server().DeleteObjectByHandle( Message().Int3() );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::GetReceptionStatusL
// Passes the reception status from the database to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::GetReceptionStatusL()
    {
    // First, get the reception status from the database.
    TBool receptionStatus( EFalse );
    iSettings.GetReceptionStatus( receptionStatus );

    // Write the reception status to the client side.
    TPckgBuf< TBool > statusPckg( receptionStatus );
    Message().WriteL( 0, statusPckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::SetReceptionStatusL
// Change the reception status to the requested one.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::SetReceptionStatusL()
    {
    // Read the reception status from the client side and then
    // change the current status to the requested one.
    TPckgBuf< TBool > statusPckg( EFalse );
    Message().ReadL( 0, statusPckg );

    // Change the status.
    TBool status( statusPckg() );
    iSettings.SetReceptionStatusL( status );
    iReceiver.ApplyStateChangesL();

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::GetTopicDetectionStatusL
// Return the current topic detection status to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::GetTopicDetectionStatusL()
    {
    // First, get the topic detection status.
    TBool topicDetectionStatus( EFalse );
    iSettings.GetTopicDetectionStatus( topicDetectionStatus );

    // Write the topic detection status to the client side.
    TPckgBuf< TBool > statusPckg( topicDetectionStatus );
    Message().WriteL( 0, statusPckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::SetTopicDetectionStatusL
// Change the topic detection status to the requested one.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::SetTopicDetectionStatusL()
    {
    // Read the topic detection status from the client side and then
    // change the current status to the requested one.
    TPckgBuf< TBool > statusPckg( EFalse );
    Message().ReadL( 0, statusPckg );
 
    iSettings.SetTopicDetectionStatusL( statusPckg() );  
    iReceiver.ApplyStateChangesL();

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::GetLanguagesL
// Return the preferred languages to the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::GetLanguagesL()
    {
    // Get the language settings.
    TCbsDbLanguages language;
    iSettings.GetLanguages( language );
    
    // Write the preferred languages to the client side.
    TPckgBuf< TCbsSettingsLanguages > languagePckg( language );
    Message().WriteL( 0, languagePckg );

    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::SetLanguagesL
// Sets preferred languages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::SetLanguagesL()
    {
    // Read the preferred languages from the client side and then
    // change the current languages to the requested.
    TPckgBuf< TCbsSettingsLanguages > languagePckg;
    Message().ReadL( 0, languagePckg );

    // Update db's language settings.
    TCbsDbLanguages languages( languagePckg() );
    iSettings.SetLanguagesL( languages );
    
    // Complete the request.
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::NotifySettingsChanged
// Make request to notify when settings are changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::NotifySettingsChanged()
    {
    // If there is already a pending request, cancel it.
    if ( iIsMessage )
        {
        NotifySettingsChangedCancel();
        }

    // And then save the message.
    iMessage = Message();
    iIsMessage = ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsSettings::NotifySettingsChangedCancel
// Cancel the request to notify when settings are changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::NotifySettingsChangedCancel()
    {               
    if ( iIsMessage )
        {
        iMessage.Complete( KErrCancel );
        }

    iIsMessage = EFalse;
    Message().Complete( KErrNone );
    }

// -----------------------------------------------------------------------------
// CCbsSettings::NotifyClientL
// Notifies the client (if there is a pending request).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsSettings::NotifyClientL( 
    TCbsSettingsEvent aEvent )
    {
    // If there is a pending request, then process it.
    if ( iIsMessage )
        {
        // Make a pointer descriptor
        TPckg<TCbsSettingsEvent> eventPckg( aEvent );
        iMessage.WriteL( 0, eventPckg );
                
        iMessage.Complete( KErrNone );
        iIsMessage = EFalse;
        }       
    }


// ========================== OTHER EXPORTED FUNCTIONS =========================
//  End of File  

