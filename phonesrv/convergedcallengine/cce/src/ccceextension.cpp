/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles Extension interfaces
*
*/


//  INCLUDE FILES
#include "cccepluginmanager.h"
#include "ccceextension.h"
#include "cccelogger.h"


#include "mcceextensionobserver.h"
#include "mccpextensionprovider.h"

#include "cconvergedcallprovider.h"


// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// NewL()
// -----------------------------------------------------------------------------
//
CCCEExtension* CCCEExtension::NewL( CCCEPluginManager& aPluginManager )
    {
    CCCEExtension* self = new (ELeave) CCCEExtension( aPluginManager );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCCEDtmf()
// -----------------------------------------------------------------------------
//
CCCEExtension::CCCEExtension( CCCEPluginManager& aPluginManager ):
    iPluginManager( aPluginManager )
    {

    }

// -----------------------------------------------------------------------------
// ConstructL()
// -----------------------------------------------------------------------------
//
void CCCEExtension::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// ~CCCEExtension()
// -----------------------------------------------------------------------------
//
CCCEExtension::~CCCEExtension()
    {
    }

// -----------------------------------------------------------------------------
// AddObserverL( MCCEDtmfObserver& aObserver )
// -----------------------------------------------------------------------------
//
void CCCEExtension::AddObserverL( const MCCEExtensionObserver& aObserver )
    {
    // add support for more than one observer later on. Only one is used at the moment. Last one 
    // given is used.
    iMCCEExtensionObserver = const_cast<MCCEExtensionObserver*>(&aObserver);
    }

// -----------------------------------------------------------------------------
// RemoveObserver( MCCEDtmfObserver& aObserver )
// -----------------------------------------------------------------------------
//
TInt CCCEExtension::RemoveObserver( const MCCEExtensionObserver& /*aObserver*/ )
    {
    // add support for more than one observer later on. Only one is used at the moment. Last one 
    // given is used.

    if( !iMCCEExtensionObserver )
        {
        return KErrNotFound;
        }
        
    iMCCEExtensionObserver = NULL;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RequestCommand()
// -----------------------------------------------------------------------------
//
TInt CCCEExtension::RequestCommand( TUint32 aServiceId,
                                    TInt aRequestId, 
                                    TDesC8& aData )
    {
    CCELOGSTRING3("CCCEExtension::RequestCommand(): Service id=%d Request id=%d", aServiceId, aRequestId );

    CConvergedCallProvider* plugin = GetExtIfPlugin(aServiceId);

    if( !plugin )
        {
        CCELOGSTRING("CCCEExtension::RequestCommand(): Plug-in for service-id was not found. Returning KErrNotFound" );
        return KErrNotFound;
        }

    MCCPExtensionProvider* extif = NULL;
    TRAPD( err, extif = plugin->ExtensionProviderL(*this) );

    if( err!=KErrNone )
        {
        CCELOGSTRING2("CCCEExtension::RequestCommand(): Plug-in was found, but extension interface get failed with error=%d", err );
        return err;
        }
   
    if( extif )
        {
        extif->RequestCommand(aRequestId,aData);
        }
    else
        {
        CCELOGSTRING("CCCEExtension::RequestCommand() Plug-in extension interface was found, but extensions are not supported. Returning KErrNotSupported" );
        return KErrNotSupported;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CancelRequest()
// -----------------------------------------------------------------------------
//
TInt CCCEExtension::CancelRequest( TUint32 aServiceId,
                                   TInt aRequestId )
    {
    CCELOGSTRING3("CCCEExtension::CancelRequest(): Service id=%d Request id=%d", aServiceId, aRequestId );

    CConvergedCallProvider* plugin = GetExtIfPlugin(aServiceId);

    if( !plugin )
        {
        CCELOGSTRING("CCCEExtension::CancelRequest(): Plug-in for service-id was not found. Returning KErrNotFound" );
        return KErrNotFound;
        }

    MCCPExtensionProvider* extif = NULL;
    TRAPD( err, extif = plugin->ExtensionProviderL(*this) );

    if( err!=KErrNone )
        {
        CCELOGSTRING2("CCCEExtension::CancelRequest(): Plug-in was found, but extension interface get failed with error=%d", err );
        return err;
        }
    
    if( extif )
        {
        extif->CancelRequest(aRequestId);
        }
    else
        {
        CCELOGSTRING("CCCEExtension::CancelRequest() Plug-in extension interface was found, but extensions are not supported. Returning KErrNotSupported" );
        return KErrNotSupported;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// HandleExtensionEvents()
// -----------------------------------------------------------------------------
//
void CCCEExtension::HandleExtensionEvents( TUint32 aServiceId,
                                           TInt aEvent,
                                           TInt aStatus )
    {
    CCELOGSTRING3("CCCEExtension::HandleExtensionEvents(): Service id=%d Event=%d", aServiceId, aEvent );
    CCELOGSTRING2("CCCEExtension::HandleExtensionEvents(): Status=%d", aStatus );

    if( iMCCEExtensionObserver )
        {
        iMCCEExtensionObserver->HandleExtensionEvents(aServiceId,aEvent,aStatus);
        }
    else
        {
        CCELOGSTRING("CCCEExtension::HandleExtensionEvents(): No observer was set, ignoring no event was notified!" );
        }
    }

// -----------------------------------------------------------------------------
// HandleExtensionEvents()
// -----------------------------------------------------------------------------
//
CConvergedCallProvider* CCCEExtension::GetExtIfPlugin( TUint32 aServiceId )
    {
    CConvergedCallProvider* provider = NULL;
    
    TRAP_IGNORE( provider = iPluginManager.GetPluginL( aServiceId ) );

    return provider;
    }


// end of file
