/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles dtmf
*
*/


//  INCLUDE FILES
#include "cccedtmf.h"
#include "mccpdtmfprovider.h"
#include "cccecall.h"
#include "cccelogger.h"
#include "cccecallcontainer.h"
#include "cccepluginmanager.h"

// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// NewL()
// -----------------------------------------------------------------------------
//
CCCEDtmf* CCCEDtmf::NewL( 
    CCCECallContainer& aCallContainer,
    CCCEPluginManager& aPluginManager )
    {
    CCCEDtmf* self = new (ELeave) CCCEDtmf( aCallContainer, aPluginManager );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCCEDtmf() 
// -----------------------------------------------------------------------------
//
CCCEDtmf::CCCEDtmf( 
    CCCECallContainer& aCallContainer,
    CCCEPluginManager& aPluginManager ):
    iCallContainer( aCallContainer ),
    iPluginManager( aPluginManager )
    {

    }

// -----------------------------------------------------------------------------
// ConstructL()
// -----------------------------------------------------------------------------
//
void CCCEDtmf::ConstructL()
    {
    CCELOGSTRING("CCCEDtmf::ConstructL");
    }

// -----------------------------------------------------------------------------
// ~CCCEDtmf()
// -----------------------------------------------------------------------------
//
CCCEDtmf::~CCCEDtmf()
    {
    CCELOGSTRING("CCCEDtmf::~CCCEDtmf");

    while( iObservers.Count() )
        {
        CCELOGSTRING("CCCEDtmf::~CCCEDtmf: There are dtmf observers in the array. Should be removed before dtor!");
        iObservers.Remove ( 0 );
        iObservers.Compress();
        }

    iObservers.Close();
    }

// -----------------------------------------------------------------------------
// GetDtmfProviders()
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::GetDtmfProviders( RPointerArray<MCCPDTMFProvider>& aProviders ) const
    {
    CCELOGSTRING("CCCEDtmf::GetDtmfProviders");
    RPointerArray<MCCECallInfo> calls;
    // Gets calls
    TInt err = KErrNotFound;
    MCCPDTMFProvider* provider;
    provider = NULL;
    err = iCallContainer.GetActiveOrConnectingCalls( calls );
    
    // Insert desired dtmf providers into array
    for( TInt i = 0; i < calls.Count(); i++ )
        {
        TRAPD( error, provider = 
            &iPluginManager.DtmfProviderL( *this, calls[i]->ImplementationUid() );
            );
            
        if( !error && provider )
            {
            // Ref-to-Ptr -> ownership not transfered. 
            err = aProviders.InsertInAddressOrder( provider ); 
            }
        }
        
    calls.Close();
    
    // notifies only if all providers fail to support 
    if( aProviders.Count()==0 )
        {
        err = KErrNotSupported;
        }
    return err;
    }


// -----------------------------------------------------------------------------
// CCCEDtmf::StopDtmfTone()
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::StopDtmfTone()
    {
    CCELOGSTRING("CCCEDtmf::StopDtmfTone");
    RPointerArray<MCCPDTMFProvider> dtmfproviders;

    TInt err = GetDtmfProviders( dtmfproviders );
    // Send request to desired dtmf providers
    for( TInt i = 0; i < dtmfproviders.Count(); i++ )
        {
        dtmfproviders[i]->StopDtmfTone();
        }
    dtmfproviders.Close();
    
    return err;
    }

// -----------------------------------------------------------------------------
// CCCEDtmf::StartDtmfTone( TChar /* aTone */ )
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::StartDtmfTone( TChar aTone )
    {
    CCELOGSTRING2("CCCEDtmf::StartDtmfTone %d",(TInt)aTone);
    RPointerArray<MCCPDTMFProvider> dtmfproviders;

    TInt err = GetDtmfProviders( dtmfproviders );
    // Send request to desired dtmf providers
    for( TInt i = 0; i < dtmfproviders.Count(); i++ )
        {
        dtmfproviders[i]->StartDtmfTone( aTone );
        }
        
    dtmfproviders.Close();
    
    return err;
    }
// -----------------------------------------------------------------------------
// CCCEDtmf::SendDtmfToneString( TDesC& aTones )
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::SendDtmfToneString( const TDesC& aTones )
    {
    CCELOGSTRING("CCCEDtmf::SendDtmfToneString");
    RPointerArray<MCCPDTMFProvider> dtmfproviders;

    TInt err = GetDtmfProviders( dtmfproviders );
    // Send request to desired dtmf providers
    for( TInt i = 0; i < dtmfproviders.Count(); i++ )
        {
       err = dtmfproviders[i]->SendDtmfToneString( aTones );
        }
        
    dtmfproviders.Close();
    
    return err;
    }
  
// -----------------------------------------------------------------------------
// CCCEDtmf::ContinueStringSending( TBool aContinue )
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::ContinueStringSending( TBool aContinue )
    {
    CCELOGSTRING2("CCCEDtmf::SendDtmfToneString %d",aContinue);
    RPointerArray<MCCPDTMFProvider> dtmfproviders;

    TInt err = GetDtmfProviders( dtmfproviders );
    // Send request to desired dtmf providers
    for( TInt i = 0; i < dtmfproviders.Count(); i++ )
        {
        err = dtmfproviders[i]->ContinueDtmfStringSending( aContinue );
        }
        
    dtmfproviders.Close();
    
    return err;
    }
 
// -----------------------------------------------------------------------------
// CCCEDtmf::CancelDtmfStringSending()
// -----------------------------------------------------------------------------
//
void CCCEDtmf::CancelDtmfStringSending()
    {
    CCELOGSTRING("CCCEDtmf::CancelDtmfStringSending");
    RPointerArray<MCCPDTMFProvider> dtmfproviders;

    GetDtmfProviders( dtmfproviders );
    // Send request to desired dtmf providers
    for( TInt i = 0; i < dtmfproviders.Count(); i++ )
        {
        dtmfproviders[i]->CancelDtmfStringSending();
        }
        
    dtmfproviders.Close();
    }

// -----------------------------------------------------------------------------
// CCCEDtmf::SetDtmfObserver( MCCEDtmfObserver& aObserver )
// -----------------------------------------------------------------------------
//
void CCCEDtmf::SetObserver( const MCCEDtmfObserver& aObserver )
    {
    CCELOGSTRING("CCCEDtmf::SetObserver. Will be deprecated, start using AddObserver instead!");

    // deprecated, not supported anymore!!! Remove from API later on!
    
    // 26.11.2007 support for old way SetObserver. Remove this when changes to phone are done!!!
    TRAP_IGNORE(AddObserverL(aObserver));
    } 
    
// -----------------------------------------------------------------------------
// CCCEDtmf::AddObserverL( MCCEDtmfObserver& aObserver )
// -----------------------------------------------------------------------------
//
void CCCEDtmf::AddObserverL( const MCCEDtmfObserver& aObserver )
    {
    CCELOGSTRING("CCCEDtmf::AddObserverL");

    if( iObservers.Find( &aObserver )==KErrNotFound )
        {
        iObservers.AppendL( &aObserver );
        }
    else
        {
        // already added, ignore 
    CCELOGSTRING("CCCEDtmf::AddObserverL. Observer to be added already existed. Operation ignored!");
        }    
    }    

// -----------------------------------------------------------------------------
// CCCEDtmf::RemoveObserver( MCCEDtmfObserver& aObserver )
// -----------------------------------------------------------------------------
//
TInt CCCEDtmf::RemoveObserver( const MCCEDtmfObserver& aObserver )
    {
    CCELOGSTRING("CCCEDtmf::RemoveObserver");
    TInt index = iObservers.Find( &aObserver ); 
    
    if( index!=KErrNotFound )
        {
        iObservers.Remove ( index );
        iObservers.Compress();
        }
    else
        {
        return KErrNotFound;
        }

    return KErrNone;
    } 

// ---------------------------------------------------------------------------
// From class MCCPDTMFObserver
// ---------------------------------------------------------------------------
//
void CCCEDtmf::HandleDTMFEvent( const TCCPDtmfEvent aEvent,
                                const TInt aError,
                                const TChar aTone ) const
    {
    CCELOGSTRING ("CCCEDtmf::HandleDTMFEvent");
    CCELOGSTRING2 ("    HandleDTMFEvent, event = %d ",aEvent );
    CCELOGSTRING2 ("    HandleDTMFEvent, error = %d ",aError );
    CCELOGSTRING2 ("    HandleDTMFEvent, tone = %d ",(TInt)aTone );
    
    MCCEDtmfObserver::TCCEDtmfEvent event = (MCCEDtmfObserver::TCCEDtmfEvent)(aEvent);
    
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        // notify all observers
            const_cast<MCCEDtmfObserver*>(iObservers[i])->HandleDTMFEvent( event,
                                                                       aError, 
                                                                       aTone );
        }
    }
