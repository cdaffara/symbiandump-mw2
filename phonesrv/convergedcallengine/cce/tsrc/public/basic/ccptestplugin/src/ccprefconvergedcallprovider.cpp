/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCP reference plug-in implementation.
*
*/



//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <e32def.h>
#include <badesca.h>
#include <e32debug.h>
#include <mccpobserver.h>
#include <mccpcsobserver.h>
#include <mccpcallobserver.h>
#include "ccprefcall.h"
#include "ccprefemergencycall.h"
#include "ccprefdtmfprovider.h"
#include "ccprefextensionprovider.h"
#include "ccprefconvergedcallprovider.h"
#include "ccprefconferencecall.h"
#include "ccprefcontroller.h"
#include "ccpreftransferprovider.h"
#include "ccptestplugindefs.h"
#include "ccptestpluginlogger.h"

/** Max number of call objects */
const TInt KCCECallObjects = 8;
// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider, default constructor
// -----------------------------------------------------------------------------
//
CCPRefConvergedCallProvider::CCPRefConvergedCallProvider() : 
    iServiceId(9999),
    iObserver(NULL),
    iSsObserver(NULL),
    iEmergencyCallPtr(NULL),
    iExtensionProvider(NULL), 
    iDTMFProvider(NULL),
    iConferenceCallProvider(NULL),
    iCalledParty(NULL),
	iIsExtIfSupported(ETrue),
	iExtIfLeaveValue(KErrNone)
    
    {
    }

// -----------------------------------------------------------------------------
// ~CCPRefConvergedCallProvider, default destructor
// -----------------------------------------------------------------------------
//
CCPRefConvergedCallProvider::~CCPRefConvergedCallProvider()
    {
    /* Calls should have been already deleted in CCPRefCall::Release..
    */
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::~CCPRefConvergedCallProvider");
    CCPTESTPLUGINLOG1("[TESTPLUGIN] Deleting plugin: %d", iPluginUid );
    CCPTESTPLUGINLOG1("[TESTPLUGIN] Remaining calls: %d", iCallArray.Count() );
     
    iCallArray.ResetAndDestroy();
    iCallArray.Close();
    
    delete iDTMFProvider; 
    delete iCalledParty;
    delete iExtensionProvider;
    delete iEmergencyCallPtr;
    
    delete iPluginCtrl;
    }


// -----------------------------------------------------------------------------
// CsNewL
// -----------------------------------------------------------------------------
//
EXPORT_C CCPRefConvergedCallProvider* CCPRefConvergedCallProvider::CsNewL()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::CsNewL");
    CCPRefConvergedCallProvider* self = new( ELeave ) CCPRefConvergedCallProvider();

    CleanupStack::PushL( self );
    self->ConstructL( 
        KPluginCsImplementationUid, 
        CCPCall::ECallTypeCSVoice, 
            KPSUidEventCsRequestValue );
        
    CleanupStack::Pop(self);
    
    return self;
    }
    
// -----------------------------------------------------------------------------
// VccNewL
// -----------------------------------------------------------------------------
//
EXPORT_C CCPRefConvergedCallProvider* CCPRefConvergedCallProvider::VccNewL()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::VccNewL");
    CCPRefConvergedCallProvider* self = new( ELeave ) CCPRefConvergedCallProvider();

    CleanupStack::PushL( self );
    self->ConstructL( 
        KPluginVccImplementationUid, 
        CCPCall::ECallTypeCSVoice, 
        KPSUidEventCsRequestValue );
        
    CleanupStack::Pop(self);
    
    return self;
    }
    
// -----------------------------------------------------------------------------
// PsNewL
// -----------------------------------------------------------------------------
//
EXPORT_C CCPRefConvergedCallProvider* CCPRefConvergedCallProvider::PsNewL()
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::PsNewL");
    CCPRefConvergedCallProvider* self = new( ELeave ) CCPRefConvergedCallProvider();

    CleanupStack::PushL( self );
    self->ConstructL( 
        KPluginPsImplementationUid, 
        CCPCall::ECallTypePS, 
        KPSUidEventPsRequestValue );
        
    CleanupStack::Pop(self);
    
    return self;
    }


// -----------------------------------------------------------------------------
// ConstructL
// -----------------------------------------------------------------------------
//
void CCPRefConvergedCallProvider::ConstructL( 
    TUint aUid, CCPCall::TCallType aCallType, TInt aRequestValue )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ConstructL IN");
    iPluginUid.iUid = aUid;
    iCallType = aCallType;
    
    iDTMFProvider = CCPRefDTMFProvider::NewL(*this);
    iExtensionProvider = CCPRefExtensionProvider::NewL(*this);
    
    iPluginCtrl = CCPRefController::NewL( 
        *this, iPluginUid, iCallType, aRequestValue );
    
    iCalledParty = HBufC::NewL(KMaxAddressName);
        
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ConstructL OUT");
    }
    



// -----------------------------------------------------------------------------
// InitializeL
// -----------------------------------------------------------------------------
//
void CCPRefConvergedCallProvider::InitializeL( const MCCPObserver& aObserver, const MCCPSsObserver& aSsObserver )
    {    
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::InitializeL IN");
    TInt value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::InitializeL"),value ) )
    	{
	   	CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::InitializeL, was configured to LEAVE=%d", value );
    	User::Leave(value);
    	}
       
    iObserver = const_cast<MCCPObserver*>(&aObserver);
    
    iSsObserver = const_cast<MCCPSsObserver*>(&aSsObserver);
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::InitializeL OUT");
    }
    
    


// -----------------------------------------------------------------------------
// Creates a new call object
// Call is deleted via CCE calling MCCPCall::Release
// -----------------------------------------------------------------------------
//
MCCPCall* CCPRefConvergedCallProvider::NewCallL( const CCCPCallParameters& aParameters,
									             const TDesC& aRecipient,
									             const MCCPCallObserver& aObserver )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::NewCallL");

    TInt value;
    TBool leave = EFalse;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::NewCallL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewCallL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}
   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewCallL, configured to return=%d", value );
    	return (MCCPCall*)value;
    	}

    // Store recipient
    TBufC<100> temp(aRecipient);
    TPtr iCalledPartyPtr( iCalledParty->Des() );
    iCalledPartyPtr = temp;
        
    /* Check recipient and observer
    */
    if( aRecipient==KNullDesC )
        {
        User::Leave(KErrArgument);
        }
    
    // Create new call object
    CCPRefCall* newCall = CCPRefCall::NewL( this,
        aRecipient,
        const_cast<MCCPCallObserver*>(&aObserver),
        iPluginUid );
    
    newCall->SetParameters( aParameters );

	// Store the new call to a pointer array
    iCallArray.Append( newCall );

    return newCall;
    }
    
    
    
 // -----------------------------------------------------------------------------
// One emergency call used
//
// -----------------------------------------------------------------------------
//
MCCPEmergencyCall* CCPRefConvergedCallProvider::NewEmergencyCallL( TUint32 /*aServiceId*/,
								                                   const TDesC& /*aAddress*/,
                                                                   const MCCPCallObserver& aObserver )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::NewEmergencyCallL");

    TInt value;
    TBool leave = EFalse;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::NewEmergencyCallL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewEmergencyCallL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}

   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewEmergencyCallL, configured to return=%d", value );
    	return (MCCPEmergencyCall*)value;
    	}

    
    _LIT( KEmergencyNumber, "define default emergency call number/address or fetch it from somewhere...");

    /* In this case we have only one instance of an emergency call that is created at first request
    *  subsequent calls only set the observer again
    */
    if( !iEmergencyCallPtr )
        {
        // create new emergency call, could also be pre-done e.g. in ConstructL 
        // and just returning a pointer to that here
        iEmergencyCallPtr = CCPRefEmergencyCall::NewL( this, 
                                                       KEmergencyNumber, 
                                                       const_cast<MCCPCallObserver*>(&aObserver) );
        }
     else
        {
        iEmergencyCallPtr->SetObserver(const_cast<MCCPCallObserver*>(&aObserver));
        }

    return iEmergencyCallPtr;
    }




// -----------------------------------------------------------------------------
// Creates a new conference call object
// 
// -----------------------------------------------------------------------------
//
MCCPConferenceCall* CCPRefConvergedCallProvider::NewConferenceL( const TUint32 /*aServiceId*/,
                                                                 const MCCPConferenceCallObserver& aObserver )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::NewConferenceL");
    
    TInt value;
    TBool leave = EFalse;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::NewConferenceL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewConferenceL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}
   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::NewConferenceL, configured to return=%d", value );
    	return (MCCPConferenceCall*)value;
    	}


    if ( iConferenceCallProvider && iConferenceCallProvider->IsReleased() )
    	{
    	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::NewConferenceL -> IsReleased() = true, Leave(KErrNotReady)");
    	User::Leave(KErrNotReady);
    	}
    else
    	{
    	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::NewConferenceL -> IsReleased() = false");
    	}
         
    // Only one conference call object. 
    delete iConferenceCallProvider; iConferenceCallProvider = NULL; // null for codescanner 
    
    // Create new call object
    iConferenceCallProvider = new (ELeave) CCPRefConferenceCall(*this);

    TRAPD( err, iConferenceCallProvider->AddObserverL(aObserver) )
    
    if( err )
        {
        delete iConferenceCallProvider;
        iConferenceCallProvider = NULL;
        User::Leave( err );
        }
        
    return iConferenceCallProvider;
    }
    
    

// -----------------------------------------------------------------------------
// Release call
//
// -----------------------------------------------------------------------------
//
TInt CCPRefConvergedCallProvider::ReleaseCall( MCCPCall& aCalltoRemove )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ReleaseCall");

    TInt value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::ReleaseCall"),value ) )
    	{
    	return value;
    	}
    
    return DeleteCall(static_cast<CCPRefCall&>(aCalltoRemove));
    }
									


// -----------------------------------------------------------------------------
// Release emergency call
// -----------------------------------------------------------------------------
TInt CCPRefConvergedCallProvider::ReleaseEmergencyCall( MCCPEmergencyCall& /* aCalltoRemove */ )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ReleaseEmergencyCall");
    
    delete iEmergencyCallPtr;
    iEmergencyCallPtr = NULL;

    TInt value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::ReleaseEmergencyCall"),value ) )
    	{
    	return value;
    	}
    
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// Release conference call
// -----------------------------------------------------------------------------
TInt CCPRefConvergedCallProvider::ReleaseConferenceCall( MCCPConferenceCall& aCalltoRemove )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ReleaseConferenceCall");
    
    TInt value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::ReleaseConferenceCall"),value ) )
    	{
    	return value;
    	}
    
    if( static_cast<CCPRefConferenceCall*>(&aCalltoRemove) == iConferenceCallProviderTemp )
        {
        delete iConferenceCallProviderTemp;
        iConferenceCallProviderTemp = NULL;
        return KErrNone;
        }
        
    while ( iConferenceCallProvider->iConfCallArray.Count() > 0 )
    	{
    	// Find from call array
    	TInt ccpCall = iCallArray.Find( (CCPRefCall*)iConferenceCallProvider->iConfCallArray[0]);
    		
    	// Remove from conf call array
    	if ( ccpCall != KErrNotFound )
    		{
	    	//iConferenceCallProvider->RemoveConfCall( *iCallArray[ccpCall] );
	    	iConferenceCallProvider->iConfCallArray.Remove(0);
    		}
    	else
    		{
    		User::Panic( _L("ReleaseConferenceCall,  call not found in iCallArray"), KErrGeneral );
    		}
    	
    	/* Disconnect the call, either via Disconnecting state
    	*  or straight to Idle
    	*/
    	switch ( iCallArray[ccpCall]->State() )
    		{
    		case MCCPCallObserver::ECCPStateRinging:
    		case MCCPCallObserver::ECCPStateQueued:
    		case MCCPCallObserver::ECCPStateConnecting:
    		case MCCPCallObserver::ECCPStateForwarding:   			
    			iCallArray[ccpCall]->SetNewState( MCCPCallObserver::ECCPStateIdle );
    			break;
    		
    		case MCCPCallObserver::ECCPStateConnected:
    		case MCCPCallObserver::ECCPStateHold:
    		case MCCPCallObserver::ECCPStateTransferring:
    			iCallArray[ccpCall]->HangUp();
    			break;
    				
    		default:
    			break;
    		}
		}
	    
  	// Reset the conference call array
    iConferenceCallProvider->iConfCallArray.Reset();
    
    // Only one conference call object.
    delete iConferenceCallProvider; iConferenceCallProvider = NULL; // null for codescanner 
    
    return KErrNone;
    }
	
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
const TUid& CCPRefConvergedCallProvider::Uid() const
    {
    CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::Uid() -> %d", iPluginUid );

    TUid value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::Uid"),value.iUid ) )
    	{
    	return value;
    	}
    
    return iPluginUid;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TUint32 CCPRefConvergedCallProvider::Caps() const
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::Caps()");

    TUint32 value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::Caps"),value ) )
    	{
    	return value;
    	}
    
    return iCaps;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
MCCPDTMFProvider* CCPRefConvergedCallProvider::DTMFProviderL( const MCCPDTMFObserver& aObserver )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::DTMFProviderL()");

    TInt value;
    TBool leave = EFalse;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::DTMFProviderL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::DTMFProviderL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}

   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::DTMFProviderL, configured to return=%d", value );
    	return (MCCPDTMFProvider*)value;
    	}
    

    if( iDTMFProvider )
        {
        iDTMFProvider->AddObserverL(aObserver);
        }

    return iDTMFProvider;
    }

// -----------------------------------------------------------------------------
// Do not set test config checking here. Special case.
// -----------------------------------------------------------------------------
//
MCCPExtensionProvider* 
CCPRefConvergedCallProvider::ExtensionProviderL( const MCCPExtensionObserver& aObserver ) 
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ExtensionProviderL()");
    
    TInt value;
    TBool leave = EFalse;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::ExtensionProviderL"),value, &leave ) )
    	{
    	if( leave )
    		{
	   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::ExtensionProviderL, was configured to LEAVE=%d", value );
    		User::Leave(value);
    		}

   		CCPTESTPLUGINLOG1("[TESTPLUGIN] CCPRefConvergedCallProvider::ExtensionProviderL, configured to return=%d", value );
    	return (MCCPExtensionProvider*)value;
    	}
    
    if( iExtensionProvider )
        {
        iExtensionProvider->AddObserverL(aObserver);
        }

    return iExtensionProvider;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CCPRefConvergedCallProvider::GetLifeTime( TDes8& aLifeTimeInfo )
	{
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::GetLifeTime()");

    TBool value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::GetLifeTime"),value ) )
    	{
    	return value;
    	}
    	
    //RDebug::Print(_L("[TESTPLUGIN] aLifeTimeInfo max size=%d"), aLifeTimeInfo.MaxSize() );
	_LIT8( KReturnLifetime, "47285710728469827493");
    aLifeTimeInfo = KReturnLifetime;
    
    return ETrue;
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CCPRefConvergedCallProvider::GetCSInfo( CSInfo& aCSInfo )
	{
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::GetCSInfo()");

	
	aCSInfo.iSerialNumber = _L("test serial nbr 123456789");
	
	// sw version
	aCSInfo.iSWVersion = _L("test sw version nbr 123456789");

	// PhoneModel
	aCSInfo.iPhoneModel = _L("test phone nbr 123456789");

	// Manufacturer name
	aCSInfo.iManufacturerIdSize = _L("test manufacturer id size 123456789");


    TBool value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::GetCSInfo"),value ) )
    	{
    	return value;
    	}
    
    return ETrue;
	}



// -----------------------------------------------------------------------------
// new method, not in CCP API. Called from CCPRefCall::Release
// -----------------------------------------------------------------------------
//
TInt CCPRefConvergedCallProvider::DeleteCall( CCPRefCall& aCallToDelete )
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::DeleteCall()");
    
    CCPTESTPLUGINLOG1("[TESTPLUGIN] Call count before delete = %d", iCallArray.Count() );
    
    /* Delete the call from Call Array
    */  
    TInt callToDelete = iCallArray.Find(&aCallToDelete);
    
    if ( callToDelete == KErrNotFound )
    	{
    	CCPTESTPLUGINLOG("[TESTPLUGIN] Delete failed");
    	return KErrNotFound;
    	}
    else
    	{
    	// iTransferProvider->iActiveTransferCall is HangUp inside this delete
    	CCPTESTPLUGINLOG("[TESTPLUGIN] Deleting CCP call");
    	delete iCallArray[callToDelete];
    	iCallArray.Remove(callToDelete);
    	iCallArray.Compress();
     	}
     	
    /* Delete the call from Conference Call Array
    */
    if( iConferenceCallProvider )
        {
        iConferenceCallProvider->RemoveConfCall( aCallToDelete );
        }
    
    if ( iCallArray.Count() == 0 )
    	{
    	iDTMFProvider->ClearDtmfString();
    	}
     	
    CCPTESTPLUGINLOG1("[TESTPLUGIN] Call count after delete = %d", iCallArray.Count() );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// new method, not in CCP API. Return service id
// -----------------------------------------------------------------------------
//
TUint32 CCPRefConvergedCallProvider::ServiceId() const 
    {
    CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::ServiceId()");

    TUint32 value;
    if( iConfigurationHandler.IsActionConfigured( _L8("CConvergedCallProvider::ServiceId"),value ) )
    	{
    	return value;
    	}
    
    return iServiceId;
    }







// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider::CreateMtCall
// -----------------------------------------------------------------------------
//
void CCPRefConvergedCallProvider::CreateMtCall(TInt aServiceID)
	{
	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::CreateMtCall");
			
	_LIT(KRecipient, "mtcall@123.12.32.22");

	CCPRefCall* mtcall = CCPRefCall::NewL( this,
                                           KRecipient,
                                           NULL, 		// There isn't observer yet
                                           iPluginUid );
                                           
	mtcall->SetMobileOriginated(EFalse);

    // Store the new call
    CleanupStack::PushL(mtcall);
    iCallArray.AppendL(mtcall);
    CleanupStack::Pop(mtcall);
    
    // iIsMobileOriginated is automatically set to EFalse
    
    CCCPCallParameters* callParams;
    TRAP_IGNORE( callParams = CCCPCallParameters::NewL() );
    __ASSERT_ALWAYS( callParams, User::Panic( _L("[TESTPLUGIN] CCPRefController::PluginInitiatedCSMoCall()"), -4) );
    callParams->SetServiceId( aServiceID );
    callParams->SetCallType( iCallType );

    
    mtcall->SetParameters( *callParams );
    delete callParams;
      
    iObserver->IncomingCall( mtcall );
	}


// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider::IncomingTransfer
// -----------------------------------------------------------------------------
//
void CCPRefConvergedCallProvider::IncomingTransfer( TInt aOriginatorCallID, TBool aAttended, TInt aServiceID )
	{
	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::IncomingTransfer");
	
	_LIT( KTransferAddress, "TransferTargetAddress" );
	
	CCPRefCall* transferTarget = CCPRefCall::NewL( this,
                                                   KTransferAddress,
                                                   NULL, 		// No observer yet
                                                   iPluginUid );
                                           
	transferTarget->SetMobileOriginated(ETrue);
	
    CleanupStack::PushL(transferTarget);
    iCallArray.AppendL(transferTarget);
    CleanupStack::Pop(transferTarget);
    
    CCCPCallParameters* callParams;
    TRAP_IGNORE( callParams = CCCPCallParameters::NewL() );
    __ASSERT_ALWAYS( callParams, User::Panic( _L("[TESTPLUGIN] CCPRefController::PluginInitiatedCSMoCall()"), -4) );
    callParams->SetServiceId( aServiceID );
    callParams->SetCallType( iCallType );

    transferTarget->SetParameters( *callParams );
    delete callParams;
    
    // Set iTransferTarget
    transferTarget->iTransferProvider->SetTransferTarget(KTransferAddress);
    	
	iObserver->CallCreated( transferTarget, iCallArray[aOriginatorCallID -1], aAttended );
	
	}

// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider::CountRemainingCalls 
// -----------------------------------------------------------------------------
//
void CCPRefConvergedCallProvider::CountRemainingCalls()
	{
	CCPTESTPLUGINLOG("[TESTPLUGIN] CCPRefConvergedCallProvider::CountRemainingCalls");
	CCPTESTPLUGINLOG1("[TESTPLUGIN] Number of calls = %d", iCallArray.Count() );

    if( iConferenceCallProvider )
        {
        CCPTESTPLUGINLOG1(
            "[TESTPLUGIN] Number of conference calls = %d", iConferenceCallProvider->iConfCallArray.Count() );
        }
	}

// -----------------------------------------------------------------------------
// CCPRefConvergedCallProvider::ConfigurationHandler
// -----------------------------------------------------------------------------
//
CConfigurationHandler& CCPRefConvergedCallProvider::ConfigurationHandler()
	{
	return iConfigurationHandler;
	}

// -----------------------------------------------------------------------------
// Get call object by call id
// -----------------------------------------------------------------------------
//
CCPRefCall* CCPRefConvergedCallProvider::GetCall( TInt aCallId ) const
    {
    if( iCallArray.Count()>aCallId )
        {
        return iCallArray[aCallId];
        }
    else
        {
        return NULL;
        }
    }

// -----------------------------------------------------------------------------
// TerminateAllCalls
// -----------------------------------------------------------------------------
//    
void CCPRefConvergedCallProvider::TerminateAllCalls()
    {   
    for( TInt i = 0; i < KCCECallObjects; i++ )
        {
        CCPRefCall* call = GetCall( i );
        
        if( call && 
            !call->IsInConference(*call) )
            {
            // Quiet hangup
            switch( call->State() )
                {
                case MCCPCallObserver::ECCPStateDialling:
                    call->Cancel();
                    break;
                case MCCPCallObserver::ECCPStateAnswering:
                case MCCPCallObserver::ECCPStateRinging:
                case MCCPCallObserver::ECCPStateQueued:
                    call->Reject();
                    break;
                default:
                    call->HangUp();
                    break;
                }
            }
        
        }

    if( iConferenceCallProvider && 
            iConferenceCallProvider->iConfCallArray.Count() > 0 )
        {
        iConferenceCallProvider->HangUp();
        }

    }

