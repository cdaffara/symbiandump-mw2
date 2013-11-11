/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: AlwaysOnline server command operation
*
*/



// INCLUDE FILES
#include "HandleServerCommandOperation.h"
#include "AlwaysOnlineManagerLogging.h"

// CONSTANTS
_LIT8( KAOCmdStart, "AOCmdStart:" );
_LIT8( KAOCmdEnd, ":AOCmdEnd" );
_LIT8( KColon, ":" );
// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAOServerCommandHandler::CAOServerCommandHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAOServerCommandHandler::CAOServerCommandHandler( 
    CMsvSession& aMsvSession,
    TInt aPriority,
    TRequestStatus& aObserverRequestStatus,
    CAlwaysOnlineEComInterface& aPlugin,
    TManagerServerCommands aCommand )
    : 
    CMsvOperation(aMsvSession, aPriority, aObserverRequestStatus),
    iPlugin( aPlugin ),
    iCommand( aCommand )    
    {
    }

// -----------------------------------------------------------------------------
// CAOServerCommandHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAOServerCommandHandler::ConstructL( const TDes8& aParameter)
    {
    CActiveScheduler::Add(this);

    // Copy the contents from the passed parameter into member variable,
    // so that the contents of the parameter remains in safe memory area.
    iParameterBuf = aParameter.AllocL();

    StartOperationL();
    KAOMANAGER_LOGGER_WRITE("CAOServerCommandHandler::ConstructL() Operation Started");
    }

// ----------------------------------------------------
// CAOServerCommandHandler::StartOperationL
// ----------------------------------------------------
//
void CAOServerCommandHandler::StartOperationL()
    {
    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOServerCommandHandler::StartOperationL() Creating async call for HandleServerCommand(). TManagerServerCommand: %d", iCommand);
    
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    }

// -----------------------------------------------------------------------------
// CAOServerCommandHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAOServerCommandHandler* CAOServerCommandHandler::NewL( 
    CMsvSession& aMsvSession,
    TRequestStatus& aObserverRequestStatus,
    TManagerServerCommands aCommand,
    TDes8& aParameter,
    CAlwaysOnlineEComInterface& aPlugin)
    {
    CAOServerCommandHandler* self = new( ELeave ) CAOServerCommandHandler( 
        aMsvSession, 
        CActive::EPriorityIdle,
        aObserverRequestStatus,
        aPlugin,
        aCommand );

    KAOMANAGER_LOGGER_WRITE_FORMAT("CAOServerCommandHandler created to address 0x%x", self);
    
    CleanupStack::PushL( self );
    self->ConstructL( aParameter );
    CleanupStack::Pop( self );

    return self;
    }

    
// ----------------------------------------------------
// ~CAOServerCommandHandler
// ----------------------------------------------------
//
CAOServerCommandHandler::~CAOServerCommandHandler()
    {
    Cancel();
    delete iParameterBuf;
    }

// ----------------------------------------------------
// CAOServerCommandHandler::ProgressL
// ----------------------------------------------------
//
const TDesC8& CAOServerCommandHandler::ProgressL()
    {
    return iProgress;
    }

// ----------------------------------------------------
// CAOServerCommandHandler::FinalProgress
// ----------------------------------------------------
//
const TDesC8& CAOServerCommandHandler::FinalProgress()
    {
    return iFinalProgress;
    }

// ----------------------------------------------------
// CAOServerCommandHandler::DoCancel
// ----------------------------------------------------
void CAOServerCommandHandler::DoCancel()
    {
    KAOMANAGER_LOGGER_WRITE("CAOServerCommandHandler::DoCancel() Operation Cancelled");
    TRequestStatus* status = &iObserverRequestStatus;
    User::RequestComplete( status, KErrCancel );
    }


// ----------------------------------------------------
// CAOServerCommandHandler::RunL
// ----------------------------------------------------
void CAOServerCommandHandler::RunL()
    {    
    KAOMANAGER_LOGGER_WRITE("CAOServerCommandHandler::RunL() Command Issued!");
    TAny* response = iPlugin.HandleServerCommandL( iCommand, iParameterBuf );
    TRequestStatus* status = &iObserverRequestStatus;
    User::RequestComplete( status, KErrNone );
    SetFinalProgressFromResult( response );
    }

// ----------------------------------------------------------------------------
// CAOServerCommandHandler::RunError()
// ----------------------------------------------------------------------------
//
TInt CAOServerCommandHandler::RunError( TInt aError )
    {
    // A leave occurred in plugin command handler. Complete watcher
    // with error.
    TRequestStatus* status = &iObserverRequestStatus;
    User::RequestComplete( status, aError );
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CAOServerCommandHandler::SetFinalProgressFromResult()
// ----------------------------------------------------------------------------
//  
void CAOServerCommandHandler::SetFinalProgressFromResult( TAny* aResponse )
    {
    iFinalProgress.Zero();

    iFinalProgress.Append( KAOCmdStart );
    iFinalProgress.AppendNum( iPlugin.InstanceUid().iUid );
    iFinalProgress.Append( KColon );
    iFinalProgress.AppendNum( iCommand );
    iFinalProgress.Append( KColon );

    // aResponse should be handled based on command.
    HandleResponse( aResponse );

    iFinalProgress.Append( KAOCmdEnd );
    }

// ----------------------------------------------------------------------------
// CAOServerCommandHandler::HandleResponse()
// ----------------------------------------------------------------------------
//  
void CAOServerCommandHandler::HandleResponse( TAny* aResponse )
    {
    switch ( iCommand )
        {
        case EAOManagerSuicideQuery:
            // aResponse should contain either 0 or 1.
            iFinalProgress.AppendNum( reinterpret_cast<TBool>( aResponse ) );
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAOServerCommandHandler::HandleResponse() EAOManagerSuicideQuery, aResponse %d", aResponse);
            break;
        default:
            // Other commands doesn't respond anything useful.
            iFinalProgress.AppendNum( reinterpret_cast<TInt>( aResponse ) );
            KAOMANAGER_LOGGER_WRITE_FORMAT("CAOServerCommandHandler::HandleResponse() default, aResponse %d", aResponse);
            break;
        }
    }
//  End of File  
