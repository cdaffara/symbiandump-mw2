/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles plugins / threads
*
*/


//  INCLUDE FILES
#include "cccepluginmanager.h"
#include "mccedtmfobserver.h"
#include "ccceplugin.h"
#include "cccelogger.h"
#include "cccecallcontainer.h"
#include "ccceemergencycall.h"
#include "cccecall.h"
#include "mccpdtmfprovider.h"
#include "cconvergedcallprovider.h"
#include "icmapi.h"
#include "cccespsettingshandler.h"



// ======== MEMBER FUNCTIONS ========
// -----------------------------------------------------------------------------
// NewL()
// -----------------------------------------------------------------------------
//
CCCEPluginManager* CCCEPluginManager::NewL( 
    CCCECallContainer& aCallContainer,
    CCCETransferController& aTransferController )
    {
    CCCEPluginManager* self = new (ELeave) CCCEPluginManager( 
        aCallContainer, 
        aTransferController );
        
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCCEPluginManager() 
// -----------------------------------------------------------------------------
//
CCCEPluginManager::CCCEPluginManager( 
    CCCECallContainer& aCallContainer,
    CCCETransferController& aTransferController ) : 
        CActive( EPriorityStandard ),
        iCallContainer( aCallContainer ),
        iTransferController( aTransferController )
    {
    iPluginArray.Reset();
    }

// -----------------------------------------------------------------------------
// ConstructL()
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::ConstructL()
    {
#if 0
// capabilities still todo here

    static _LIT_SECURITY_POLICY_PASS( KAllowAllPolicy );
    static _LIT_SECURITY_POLICY_C2( KICMPolicy, 
                                    ECapabilityNetworkControl,
                                    ECapabilityWriteDeviceData);    

        
    // define first property to be integer type
    TInt err = RProperty::Define( KPSUidICMIncomingCall, 
        KPropertyKeyICMPluginUID, RProperty::EInt, KAllowAllPolicy, KICMPolicy );
#else
    // define first property to be integer type
    TInt err = RProperty::Define( KPSUidICMIncomingCall, 
        KPropertyKeyICMPluginUID, RProperty::EInt );
#endif
        
    if ( err != KErrAlreadyExists )
        {
        User::LeaveIfError( err );
        }
        
    User::LeaveIfError( iProperty.Attach( KPSUidICMIncomingCall,
        KPropertyKeyICMPluginUID ) );
    CActiveScheduler::Add(this);
    RunL();

    iIdle = CIdle::NewL( EPriorityIdle );
    iIdle->Start( TCallBack(DoAfterBoot,this) );
    
    iSPSettings = CCCESPSettingsHandler::NewL( *this );
    }

// -----------------------------------------------------------------------------
// ~CCCEPluginManager()
// -----------------------------------------------------------------------------
//
CCCEPluginManager::~CCCEPluginManager()
    {
    delete iSPSettings;
    
    if( iIdle )
        {
        iIdle->Cancel();
        delete iIdle;
        iIdle = NULL;
        }

    iPluginArray.ResetAndDestroy();
    iPluginArray.Close();
    
    iPluginsToClose.ResetAndDestroy();
    iPluginsToClose.Close();
    
    iAlternativeEmergencyPlugins.Reset();
    iAlternativeEmergencyPlugins.Close();
    
    Cancel();
    iProperty.Close();
    
    REComSession::FinalClose();
    
    }

// -----------------------------------------------------------------------------
// RunL()
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::RunL()
    {
    // TODO If there is two calls arriving same time. How ICM can handle it?
    
    CCELOGSTRING("CCCEPluginManager::RunL()" );
    // resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe( iStatus );
    SetActive();
    // property updated, get new value

    TInt keyValue;
    if ( iProperty.Get( keyValue ) == KErrNotFound )
        {
        // property deleted, do necessary actions here...
        }
    else
        {
        if( keyValue )
            {
            TUid pluginId = TUid::Uid(keyValue);

            CCELOGSTRING2("CCCEPluginManager::RunL: IncomingCallAPI = %d",
                pluginId.iUid );
            // use new value ...
            GetPluginL( pluginId );
            }
        }
    }

// -----------------------------------------------------------------------------
// SetObserver()
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::SetObserver( const MCCEObserver& aObserver )
    {
    iObserver = &aObserver;
    }
    
// -----------------------------------------------------------------------------
// SetObserver()
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::SetEmergencyCallObserver(MCCPCallObserver& aEmergencyCallObserver)
    {
    iEmergencyCallObserver = &aEmergencyCallObserver;
    }

// -----------------------------------------------------------------------------
// DoAfterBoot()
// -----------------------------------------------------------------------------
//
TInt CCCEPluginManager::DoAfterBoot( TAny* aPluginManager )
    {
    CCELOGSTRING("CCCEPluginManager::DoAfterBoot()" );
    
    CCCEPluginManager* manager = 
        static_cast< CCCEPluginManager* >( aPluginManager );
        TRAP_IGNORE(manager->LoadBootPluginsL());
   
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// Remove unneeded plug-ins
// -----------------------------------------------------------------------------
//
TInt CCCEPluginManager::RemovePlugins( TAny* aPluginManager )
    {
    CCELOGSTRING("CCCEPluginManager::RemovePlugins()" );
    
    CCCEPluginManager* manager = 
        static_cast< CCCEPluginManager* >( aPluginManager );
        TRAP_IGNORE(manager->iPluginsToClose.ResetAndDestroy());
   
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// ServiceEnabledL
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::ServiceEnabledL( TUid aImplementationUid )
    {
    CCELOGSTRING("CCCEPluginManager::ServiceEnabledL" );
    CConvergedCallProvider* provider = GetPluginL( aImplementationUid );
    
    if( provider )
        {
        CCCEPlugin* plugin( NULL );
        for( TInt i=0; i<iPluginArray.Count(); i++ )
            {
            TUid oldPlugin = iPluginArray[i]->Type();
           
            if( oldPlugin.iUid == aImplementationUid.iUid )
                {
                plugin =  iPluginArray[i];
                // set plugin to stay in memory
                plugin->SetReleaseWhenIdle( EFalse );
                i = iPluginArray.Count();
                }
            }
        }
    else
        {
        User::Leave( KErrGeneral );
        }
    }

// -----------------------------------------------------------------------------
// ServiceDisabledL
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::ServiceDisabledL( TUid aImplementationUid )
    {
    CCELOGSTRING("CCCEPluginManager::ServiceDisabledL" );
    CCCEPlugin* plugin( NULL );
    for( TInt i=0; i<iPluginArray.Count(); i++ )
        {
        TUid oldPlugin = iPluginArray[i]->Type();
       
        if( oldPlugin.iUid == aImplementationUid.iUid )
            {
            plugin =  iPluginArray[i];
            i = iPluginArray.Count();
            }
        }

    if( !plugin )
        {
        CCELOGSTRING("CCCEPluginManager:: No plugin found" );
        User::Leave( KErrNotFound );
        }
    else if( iCallContainer.CountCalls( aImplementationUid ) )
        {
        CCELOGSTRING("CCCEPluginManager:: Plugin found with ongoing calls" );
        plugin->SetReleaseWhenIdle( ETrue );
        }
    else
        {
        CCELOGSTRING("CCCEPluginManager:: Plugin found" );
        plugin->SetReleaseWhenIdle( ETrue );
        RemovePlugin( aImplementationUid );
        }
    }

// -----------------------------------------------------------------------------
// Load EBootstrapCallProvider marked or enabled plugins
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::LoadBootPluginsL()
    {
    //List implementations
#ifdef _DEBUG
    RImplInfoPtrArray implementations;
    CConvergedCallProvider::ListImplementationsL(implementations);
    CCELOGSTRING2("CCCEPluginManager::GetPluginL: %d Implementation(s) found", 
        implementations.Count() );
        
    for( TInt i=0; i<implementations.Count(); i++ )
        {
        CImplementationInformation *info = implementations[i];
        
        CCELOGSTRING3("CCCEPluginManager::GetPluginL: Uid = %d, Name = %S", 
            info->ImplementationUid().iUid, &info->DisplayName() );
        CCELOGSTRING3("CCCEPluginManager::GetPluginL: ->RomBased = %d, RomOnly = %d", 
            info->RomBased(), info->RomOnly() );
        }
  
    implementations.ResetAndDestroy();
    implementations.Close();
#endif // _DEBUG
    
    RIdArray serviceIDArray;
    CleanupClosePushL( serviceIDArray );
    iSPSettings->GetServicesL( serviceIDArray );
    const TInt serviceCount = serviceIDArray.Count();
    iPrimaryEmergencyCallPlugin = NULL;
    TInt err = KErrNone;
    
    CCELOGSTRING2("CCCEPluginManager::LoadBootPluginsL: Service count: %d", serviceCount );
    for( TInt service = 0; service < serviceCount; service++ )
        {
        CCELOGSTRING2("CCCEPluginManager::LoadBootPluginsL: Processing service: %d", service );
        CCELOGSTRING2("CCCEPluginManager::LoadBootPluginsL: ServiceID: %d", serviceIDArray[service] );
        
        TRAP( err, LoadBootPluginL( serviceIDArray[service] ) )
        
        CCELOGSTRING2("CCCEPluginManager::LoadBootPluginsL: Result %i", err );
        }

    CleanupStack::PopAndDestroy();
    }

// -----------------------------------------------------------------------------
// Load EBootstrapCallProvider marked or enabled plugin
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::LoadBootPluginL( TInt aServiceId )
    {
    const TUid id = iSPSettings->ImplementationUidL( aServiceId );
    CCELOGSTRING2("CCCEPluginManager::LoadBootPluginL: Plugin id: %i", id );
    
    const TInt mask = iSPSettings->CapabilitiesL( aServiceId );
    CCELOGSTRING2("CCCEPluginManager::LoadBootPluginL: Service mask: %d", mask );
    
    const TBool enabled = iSPSettings->IsEnabledL( aServiceId );
    CCELOGSTRING2("CCCEPluginManager::LoadBootPluginL: VoIP enabled: %i", enabled );
    
    const TBool emergencyCall = mask & ESupportsEmergencyCall;
    CCELOGSTRING2(
        "CCCEPluginManager::LoadBootPluginL: Support EmergencyCall: %i", emergencyCall );
    
    if( mask & EBootstrapCallProvider || enabled )
        {
        CCELOGSTRING("CCCEPluginManager::LoadBootPluginL: Bootable plugin found" );
        ServiceEnabledL( id );
        }

    if( emergencyCall )
        {
        if( !iPrimaryEmergencyCallPlugin )
            {
            CCCEPlugin* plugin( NULL );
            for( TInt i=0; i<iPluginArray.Count(); i++ )
                {
                TUid oldPlugin = iPluginArray[i]->Type();
                
                if( oldPlugin.iUid == id.iUid )
                    {
                    plugin =  iPluginArray[i];
                    i = iPluginArray.Count();
                    }
                }
            
            if( plugin )
                {
                CCELOGSTRING("CCCEPluginManager::LoadBootPluginL: Initialise primary emergency call" );
                plugin->InitialiseEmergencyCallL( *iEmergencyCallObserver );
                iPrimaryEmergencyCallPlugin = plugin;
                }
            else
                {
                CCELOGSTRING("CCCEPluginManager::LoadBootPluginL: append plugin to alternative emergency array" );
                AddToAlternativeEmergencyArray( id );   
                }
            }
        else
            {
            CCELOGSTRING("CCCEPluginManager::LoadBootPluginL: append plugin to alternative emergency array" );
            AddToAlternativeEmergencyArray( id );   
            } 
        }
    }

// -----------------------------------------------------------------------------
// DoCancel()
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::DoCancel()
    {
    CCELOGSTRING("CCCEPluginManager::DoCancel()" );
    iProperty.Cancel();
    }
    
// -----------------------------------------------------------------------------
// Return Primary emergency call 
// -----------------------------------------------------------------------------
//    
MCCPEmergencyCall* CCCEPluginManager::PrimaryEmergencyCall()
    {
     MCCPEmergencyCall* call (NULL);
     if( iPrimaryEmergencyCallPlugin )
        {
        call =  iPrimaryEmergencyCallPlugin->GetEmergencyCall();
        }
    return call;      
    }

// -----------------------------------------------------------------------------
// Return Primary emergency call plugin uid
// -----------------------------------------------------------------------------
//    
TUid CCCEPluginManager::PrimaryEmergencyCallUid() const
    {
    TUid id( KNullUid );
    
    if( iPrimaryEmergencyCallPlugin )
        {
        id = iPrimaryEmergencyCallPlugin->Type();
        }
    
    return id;
    }
    
// -----------------------------------------------------------------------------
// Get array of know emergency plugins
// -----------------------------------------------------------------------------
//  
RArray<TUid>& CCCEPluginManager::AlternativeEmergencyPlugins()
    {
    CCELOGSTRING("CCCEPluginManager::AlternativeEmergencyPlugins ");
    RImplInfoPtrArray implementations;
    TRAP_IGNORE( CConvergedCallProvider::ListImplementationsL(implementations) );
    iAlternativeEmergencyPlugins.Reset();
        
    for( TInt i=0; i<implementations.Count(); i++ )
        {
        CImplementationInformation *info = implementations[i];
        CCELOGSTRING2("CCCEPluginManager::AlternativeEmergencyPlugins: Found %d ", info->ImplementationUid());
        AddToAlternativeEmergencyArray(info->ImplementationUid());
        }
         
    implementations.ResetAndDestroy();
    implementations.Close();  
 
    return iAlternativeEmergencyPlugins;
    }

// -----------------------------------------------------------------------------
// Add new plugin to array. Do not allow duplicates
// -----------------------------------------------------------------------------
//
void CCCEPluginManager::AddToAlternativeEmergencyArray( TUid aUid )
    {
    if( iPrimaryEmergencyCallPlugin &&
        ( iPrimaryEmergencyCallPlugin->Type().iUid == aUid.iUid ) )
        {
        CCELOGSTRING("CCCEPluginManager::AddToAlternativeEmergencyArray: Is Primary emergency plugin ");
        }
    else if( iAlternativeEmergencyPlugins.Find( aUid ) == KErrNotFound )
        {
        CCELOGSTRING("CCCEPluginManager::AddToAlternativeEmergencyArray: New plugin. Append to array ");    
        iAlternativeEmergencyPlugins.Append( aUid );  // return value ignored..
        }    
    else
        {
        CCELOGSTRING("CCCEPluginManager::AddToAlternativeEmergencyArray: Is already in Alternative Plugins ");    
        }
    
    }
// -----------------------------------------------------------------------------
// Return protocol interface object of wanted service ID
// -----------------------------------------------------------------------------
// 
CConvergedCallProvider* CCCEPluginManager::GetPluginL( TUint32 aServiceId )
    {
    CCELOGSTRING2("CCCEPluginManager::GetPluginL: Get plugin for servce id %d", aServiceId);
    TUid id( KNullUid );
    
    id = iSPSettings->ImplementationUidL( aServiceId );

    if( !id.iUid )
        {
        CCELOGSTRING("CCCEPluginManager::GetPluginL: PluginId not found");
        User::Leave( KErrNotFound );
        }
    
    return GetPluginL( id );
    }
    
// -----------------------------------------------------------------------------
// Return protocol interface object of wanted type.
// -----------------------------------------------------------------------------
//
CConvergedCallProvider* CCCEPluginManager::GetPluginL( const TUid& aType )
    {
    CCCEPlugin* plugin = NULL;
   
   //Is plugin started? 
    CCELOGSTRING2("CCCEPluginManager::GetPluginL: %d Implementation(s) running", 
        iPluginArray.Count() );

    // Checked if ie. VoIP Plugins is supported
    iSPSettings->IsPluginSupportedL( aType );
    
    for( TInt i=0; i<iPluginArray.Count(); i++ )
        {
        TUid oldPlugin = iPluginArray[i]->Type();
        CCELOGSTRING2("CCCEPluginManager::GetPluginL: existing plugin = %d",
            oldPlugin.iUid );
            
        if( oldPlugin.iUid == aType.iUid )
            {
            CCELOGSTRING("CCCEPluginManager::GetPluginL: Plugin found" );
            plugin =  iPluginArray[i];
            }
        }
 
    if (!plugin)
        {
        //Start plugin
        CCELOGSTRING("CCCEPluginManager::GetPluginL: New Plugin" );
        CCELOGSTRING2("CCCEPluginManager::GetPluginL: Plugin Uid = %d",
            aType.iUid );
        plugin = CCCEPlugin::NewL(
            aType, const_cast<MCCEObserver&>(*iObserver),
                 iCallContainer, *this, iTransferController );
                 
        CleanupStack::PushL(plugin);
        CCELOGSTRING("CCCEPluginManager::GetPluginL: Append to array" );
        iPluginArray.AppendL( plugin );
        CleanupStack::Pop();
        }
        
   return plugin->GetPluginL();
   }

// -----------------------------------------------------------------------------
// remove protocol interface object of wanted type.
// -----------------------------------------------------------------------------
//   
void CCCEPluginManager::RemovePlugin( const TUid& aType )
    {
    CCELOGSTRING2("CCCEPluginManager::RemovePlugin: %d Implementation(s) running", 
        iPluginArray.Count() );
        
    TInt pluginCount = iPluginArray.Count();
    for (TInt a = pluginCount-1; a >= 0 ; a--)
        {
        if ( iPluginArray[a]->Type() == aType && 
             iPluginArray[a]->ReleaseWhenIdle() )
            {
            // keep track that only one instance of pointer is in array
            iPluginsToClose.InsertInAddressOrder(iPluginArray[a]);
            CCELOGSTRING("CCCEPluginManager::RemovePlugin scheduled") 
            
            if (!iIdle->IsActive())
                {
                iIdle->Start( TCallBack(RemovePlugins,this) );
                }
            iPluginArray.Remove(a);
            iPluginArray.Compress();
            break;
            }
        }
    }
   
    
// ---------------------------------------------------------------------------
// Returns ETrue if plugin can be relesed if idle
// ---------------------------------------------------------------------------
//
TBool CCCEPluginManager::ReleaseWhenIdle( const TUid aType )
    {
    
    TBool release = EFalse;     
    TInt pluginCount = iPluginArray.Count();
    for (TInt a = 0; a < pluginCount; a++)
        {
        if ( iPluginArray[a]->Type() == aType )
            {
            release = iPluginArray[a]->ReleaseWhenIdle();
            break;
            }
        }
        
    CCELOGSTRING2("CCCEPluginManager::ReleaseWhenIdle  returning %d ", (TInt) release);
    return release;
    }

// -----------------------------------------------------------------------------
// Return array of dtmf providers.
// -----------------------------------------------------------------------------
//
MCCPDTMFProvider& CCCEPluginManager::DtmfProviderL( 
    const MCCPDTMFObserver& aObserver,
    TUid aImplementationUid ) const
    {
    CCCEPlugin* plugin( NULL );
    
    // performance note: a loop is nothing but keeping count in a register, decrementing it's
    // value after each iteration and jumping back.
    // But, calling RPointerArray.Count() is expensive.. that's why const TInt count
    const TInt count = iPluginArray.Count();
    for( TInt i=0; i < count; i++ )
        {
        if( iPluginArray[i]->Type().iUid == aImplementationUid.iUid )
            {
            plugin =  iPluginArray[i];
            i = count;
            }
        }
  
    if ( !plugin )
        {
        User::Leave( KErrNotFound );
        }
   
    return plugin->GetDtmfProviderL( aObserver );
    }

// -----------------------------------------------------------------------------
// From class MPluginObserver.
// 
// -----------------------------------------------------------------------------
//

void CCCEPluginManager::CCPPluginDiedEvent(TUid aPluginId, TInt /*aDeathType*/, TInt /*aReason*/)
    {

    CCELOGSTRING2("CCCEPluginManager::CCPPluginDiedEvent: %d", aPluginId.iUid );
       
    RPointerArray<CCCECall> calls;
    
    TInt count = iCallContainer.GetCall(aPluginId, calls);
    CCELOGSTRING2("CCCEPluginManager::CCPPluginDiedEvent: %d calls found!", count);
    for (TInt i = 0; i < count; i++)
        {
        calls[i]->ErrorOccurred( ECCPServerFailure, &calls[i]->GetCCPCall() );
        calls[i]->CallStateChanged( MCCPCallObserver::ECCPStateIdle, &calls[i]->GetCCPCall() );
        }
        
    calls.Close();
    
    CCELOGSTRING("CCCEPluginManager::CCPPluginDiedEvent: Plugin calls terminated");

    TInt pluginCount = iPluginArray.Count();
    for (TInt a = 0; a < pluginCount; a++)
        {
        if ( iPluginArray[a]->Type() == aPluginId )
            {
            if( KErrNone == iPluginsToClose.Append(iPluginArray[a]))
                {          
                if (!iIdle->IsActive())
                    {
                    iIdle->Start( TCallBack(RemovePlugins,this) );
                    }
                iPluginArray.Remove(a);
                iPluginArray.Compress();
                return;
                }
            }
        }
    }
// -----------------------------------------------------------------------------
// From class MPluginObserver.
// 
// -----------------------------------------------------------------------------
//    

void CCCEPluginManager::CCPPluginInitialisationFailed(TUid aPluginUid, TInt /*aError*/)
    {
 
    CCELOGSTRING2("CCCEPluginManager::CCPPluginInitialisationFailed: %d", aPluginUid.iUid );
    if (iCallContainer.CountCalls(aPluginUid))
        {
        // there is active call, cannot close.
        return;
        }
        
    TInt pluginCount = iPluginArray.Count();
    for (TInt a = 0; a < pluginCount; a++)
        {
        if ( iPluginArray[a]->Type() == aPluginUid )
            {
            if( KErrNone == iPluginsToClose.Append(iPluginArray[a]))
                {          
                if (!iIdle->IsActive())
                    {
                    iIdle->Start( TCallBack(RemovePlugins,this) );
                    }
                iPluginArray.Remove(a);
                iPluginArray.Compress();
                return;
                }
            }
        }
    }
    
// end of file
