/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for ALS control
*
*/



// INCLUDE FILES
#include    <rmmcustomapi.h>
#include    <startupdomainpskeys.h>
#include    <centralrepository.h>
#include    <sssettingsprivatepskeys.h> // sssettings pub&sub keys. 
#include    <sssettingsprivatecrkeys.h> // sssettings centrep keys. 
#include    <PSVariables.h> 

#include    "csssettingsalsnotifier.h" 
#include    "csssettingsactiveobject.h" 
#include    "sssettingslogger.h" 

// ================= MEMBER FUNCTIONS =======================
// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::CSSSettingsAlsNotifier
// -----------------------------------------------------------------------------
//
CSSSettingsAlsNotifier::CSSSettingsAlsNotifier( 
    RMobilePhone& aMobilePhone,
    RMmCustomAPI& aCustomPhone,
    MCenRepNotifyHandlerCallback& aAlsNotifyHandler )
: CActive( EPriorityStandard ), 
  iMobilePhone( aMobilePhone ),
  iCustomPhone( aCustomPhone ),
  iAlsNotifyHandler( &aAlsNotifyHandler )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::ConstructL
// -----------------------------------------------------------------------------
// 
void CSSSettingsAlsNotifier::ConstructL()
    {
    iRepository = CRepository::NewL ( KCRUidSupplementaryServiceSettings );
    iCenRepNotifyHandler = CCenRepNotifyHandler::NewL(
            *this, 
            *iRepository, 
            CCenRepNotifyHandler::EIntKey, 
            KSettingsAlsLine );
    iCenRepNotifyHandler->StartListeningL();

    TInt error(KErrNone);
    error = RProperty::Define(
        KPSUidSSConfig,
        KSettingsPPSupportsALS,
        RProperty::EInt );
    if( ( error != KErrNone ) && ( error != KErrAlreadyExists ) )
        {
        User::Leave ( error );
        }
    error = RProperty::Define(
        KPSUidSSConfig,
        KSettingsSIMSupportsALS,
        RProperty::EInt );
    if( ( error != KErrNone ) && ( error != KErrAlreadyExists ) )
        {
        User::Leave ( error );
        }

    iPPSupportsAlNotifier = CSSSettingsActiveObject::NewL(
        KPSUidSSConfig,
        KSettingsPPSupportsALS,
        *this );
    iSimSupportsAlsNotifier = CSSSettingsActiveObject::NewL(
        KPSUidSSConfig,
        KSettingsSIMSupportsALS,
        *this );
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::NewL
// -----------------------------------------------------------------------------
//
CSSSettingsAlsNotifier* CSSSettingsAlsNotifier::NewL(
    RMobilePhone& aMobilePhone,
    RMmCustomAPI& aCustomPhone,
    MCenRepNotifyHandlerCallback& aAlsNotifyHandler )
    {
    CSSSettingsAlsNotifier* self = 
        new ( ELeave ) CSSSettingsAlsNotifier(  
            aMobilePhone, 
            aCustomPhone, 
            aAlsNotifyHandler );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::~CSSSettingsAlsNotifier
// -----------------------------------------------------------------------------
// 
CSSSettingsAlsNotifier::~CSSSettingsAlsNotifier()
    {
    delete iPPSupportsAlNotifier;
    delete iSimSupportsAlsNotifier;

    if( iCenRepNotifyHandler )
        {
        iCenRepNotifyHandler->StopListening();
        }
    delete iCenRepNotifyHandler;
    delete iRepository;
    
    Cancel();
    // Do close after active request has been cancelled.
    iMobilePhone.Close();
    iCustomPhone.Close();
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::RunL
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::RunL()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::RunL");
    TInt error = iStatus.Int();
    if (( error == KErrCancel ) || ( error == KErrNotSupported ))
        {
        return;
        }

    // Reset the request:
    RMobilePhone::TMobilePhoneALSLine newLine = iSimActiveAls;
    iMobilePhone.NotifyALSLineChange( iStatus, iSimActiveAls );
    SetActive();
    
    if ( error == KErrNone )
        {
        TSSSettingsAlsValue newValue = ESSSettingsAlsNotSupported;
        if ( newLine == RMobilePhone::EAlternateLinePrimary )
            {
            newValue = ESSSettingsAlsPrimary;
            }
        else if ( newLine == RMobilePhone::EAlternateLineAuxiliary )
            {
            newValue = ESSSettingsAlsAlternate;
            }
        else
            {
            newValue = ESSSettingsAlsNotSupported;
            }

        // Inform observer.
        iAlsNotifyHandler->HandleNotifyInt(
            KSettingsAlsLine,
            newValue );
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::RunL");
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::DoCancel
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::DoCancel()
    {
    iMobilePhone.CancelAsyncRequest( EMobilePhoneNotifyALSLineChange );    
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::NotifyAlsChange
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::NotifyAlsChange()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::NotifyAlsChange");
    TInt error = EnsureAlsStatus();
    if ( error == KErrNone )
        {
        error = iPPSupportsAlNotifier->NotifyKeyChange();
        
        if ( error == KErrNone || error == KErrAlreadyExists )
            {
            error = iSimSupportsAlsNotifier->NotifyKeyChange();
            if ( error != KErrNone && error != KErrAlreadyExists )
                {
                iPPSupportsAlNotifier->CancelNotify();
                }
            }   
        if ( error == KErrNone || error == KErrAlreadyExists )
            {
            // Add notifier to Etel side if supported
            if ( !IsActive() && iSimSupportAls )
                {
                iMobilePhone.NotifyALSLineChange( iStatus, iSimActiveAls );
                SetActive();

                // Everything was ok, so inform that.
                error = KErrNone;
                }
            }
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::NotifyAlsChange");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::CancelNotify
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::CancelNotify()
    {
    iCenRepNotifyHandler->StopListening();
    iPPSupportsAlNotifier->CancelNotify();
    iSimSupportsAlsNotifier->CancelNotify();
    Cancel();
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::SetAlsValue
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::SetAlsValue( TSSSettingsAlsValue aValue )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::SetAlsValue");
    TInt error = EnsureAlsStatus();
    if ( error != KErrNone )
        {
        return error;
        }
        // If ALS not supported...
    if ( !iPPSupportAls && !iSimSupportAls && !iAlsCSPSupport )
        {
        aValue = ESSSettingsAlsNotSupported;
        __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::SetAlsValue: aValue: %d", aValue);
        return KErrNone;
        }
    
    // Check if aValue is corrupted.
    if (( aValue < ESSSettingsAlsNotSupported ) || 
        ( aValue > ESSSettingsAlsAlternate ))
        {
        return KErrArgument;
        }

    // If ALS is supported on SIM, use that.
    if ( iSimSupportAls )
        {
        __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::SetAlsValue: SIM ALS supported");
        // Write to ETel
        RMobilePhone::TMobilePhoneALSLine simAlsLine;
        switch ( aValue )
            {
            case ESSSettingsAlsPrimary:
                simAlsLine = RMobilePhone::EAlternateLinePrimary;
                break;
            case ESSSettingsAlsAlternate:
                simAlsLine = RMobilePhone::EAlternateLineAuxiliary;
                break;
            default:
                return KErrNotSupported;
            }
        TRequestStatus status;
        iMobilePhone.SetALSLine( status, simAlsLine );
        User::WaitForRequest( status );
        error = status.Int();
        }
    else
        {
        error = iRepository->Set( KSettingsAlsLine, aValue );
        __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::SetAlsValue: aValue: %d", aValue);
        }

    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::SetAlsValue");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::GetAlsValue
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::GetAlsValue( TSSSettingsAlsValue& aValue )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::GetAlsValue");
    TInt error = EnsureAlsStatus();
    if ( error != KErrNone )
        {
        return error;
        }

    // If ALS not supported...
    if ( !iPPSupportAls && !iSimSupportAls && !iAlsCSPSupport )
        {
        aValue = ESSSettingsAlsNotSupported;
        __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsValue: aValue: %d", aValue);
        return KErrNone;
        }
        
    // Check if aValue is corrupted.
    if (( aValue < ESSSettingsAlsNotSupported ) || 
        ( aValue > ESSSettingsAlsAlternate ))
        {
        return KErrArgument;
        }

    // If ALS is supported on SIM, use that.
    if ( iSimSupportAls )
        {   
        // Get from ETel;
        RMobilePhone::TMobilePhoneALSLine simAlsLine;
        error = iMobilePhone.GetALSLine( simAlsLine );
        if ( error != KErrNone )
            {
            return error;
            }
        switch ( simAlsLine )
            {
            case RMobilePhone::EAlternateLineAuxiliary:
                aValue = ESSSettingsAlsAlternate;
                break;
            case RMobilePhone::EAlternateLinePrimary:
                aValue = ESSSettingsAlsPrimary;
                break;
            default:
                __ASSERT_DEBUG( EFalse , Panic( 
                    SSSettingsPanicConflictInAls ) );
                aValue = ESSSettingsAlsNotSupported;
                error = KErrCorrupt;
                break;
            }
        __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsValue: SimAls supported: aValue: %d", aValue);
        }
    else
        {
        error = iRepository->Get( KSettingsAlsLine, (TInt&)aValue );
         
        if ( error != KErrNone )
            {
            return error;
            }
        
        TInt usedLine = aValue;

        // If used SIM doesn't support ALS set ESSSettingsAlsPrimary line to use.
        if ( error == KErrNone && usedLine == ESSSettingsAlsNotSupported )
            {
            // Set default value
            error = SetAlsValue( ESSSettingsAlsPrimary );
            if ( error == KErrNone )
                {
                aValue = ESSSettingsAlsPrimary;
                }
            }
        else if (( aValue < ESSSettingsAlsNotSupported ) || 
                 ( aValue > ESSSettingsAlsAlternate ))
            {
            error = KErrCorrupt;
            }
        __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsValue: aValue: %d", aValue);
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::GetAlsValue");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::GetAlsSupport
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::GetAlsSupport( 
    TBool& aPPSupport, TBool& aSimSupport, TBool& aCSPSupport, TInt& aCSPError )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::GetAlsSupport");
    TInt error = EnsureAlsStatus();

    if( error != KErrNone )
        {
        __SSSLOGSTRING("[SSS]     If we aren't able to get ALS status we can not support it");
        // If we aren't able to get ALS status we can not support it.
        aPPSupport = EFalse;
        aSimSupport = EFalse;
        aCSPSupport = EFalse;
        aCSPError = iAlsCSPError;
        return;
        }

    aPPSupport = iPPSupportAls;
    aSimSupport = iSimSupportAls;
    aCSPSupport = iAlsCSPSupport;
    aCSPError = iAlsCSPError;
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsSupport: iPPSupportAls: %d", iPPSupportAls);
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsSupport: iSimSupportAls: %d", iSimSupportAls);
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::GetAlsSupport: iAlsCSPSupport: %d", iAlsCSPSupport);
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::GetAlsSupport");
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::CheckAlsSupportInProductProfile
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::CheckAlsSupportInProductProfile( 
    TBool& aIsAlsSopportedInPP )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::CheckAlsSupportInProductProfile");
    RMmCustomAPI::TAlsSupport ppAlsSupport;

    TInt error = iCustomPhone.CheckAlsPpSupport( ppAlsSupport );

    if ( ( error == KErrNone ) &&
         ( ppAlsSupport == RMmCustomAPI::EAlsSupportOn ) )
        {
        aIsAlsSopportedInPP = ETrue;
        }
    else
        {
        aIsAlsSopportedInPP = EFalse;
        }
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::CheckAlsSupportInProductProfile: aIsAlsSopportedInPP: %d", aIsAlsSopportedInPP);
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::CheckAlsSupportInProductProfile");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::CheckAlsSupportInSim
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::CheckAlsSupportInSim(
    TBool& aIsAlsSopportedInSim )
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::CheckAlsSupportInSim");
    RMobilePhone::TMobilePhoneALSLine simAlsSupport;
    TInt error = iMobilePhone.GetALSLine( simAlsSupport );
 
    if (( error != KErrNone ) ||
        ( simAlsSupport == RMobilePhone::EAlternateLineUnknown ) || 
        ( simAlsSupport == RMobilePhone::EAlternateLineNotAvailable ))
        {
        aIsAlsSopportedInSim = EFalse;
        }
    else
        {
        aIsAlsSopportedInSim = ETrue;
        }
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::CheckAlsSupportInSim: aIsAlsSopportedInSim: %d", aIsAlsSopportedInSim);
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::CheckAlsSupportInSim");
    return error;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::EnsureAlsStatus
// -----------------------------------------------------------------------------
//
TInt CSSSettingsAlsNotifier::EnsureAlsStatus()
    {
    RMobilePhone::TCspCPHSTeleservices params = ( RMobilePhone::TCspCPHSTeleservices )0;
    TInt error(KErrNone);
    TInt simState(0);
    TBool savePPSimAls(0);
    TBool saveSimAls(0);

    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::EnsureAlsStatus");    
    if ( iAlsStatusChecked )
       {
       __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::EnsureAlsStatus - Already checked: %d", iAlsStatusChecked);  
       return KErrNone;
       }
   
    error = RProperty::Get( KPSUidStartup, KPSSimStatus, simState );
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::EnsureAlsStatus - KPSUidSIMStatusValue: %d", simState);  
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::EnsureAlsStatus - KPSUidSIMStatusValue Error: %d", error);

    if ( error == KErrNone )
        {
        // Make sure that SIM state is OK otherwise parameters coming from SIM are mess.
        if ( simState ==ESimUsable )
            {
            // Get Als support from Product Profile.
            error = CheckAlsSupportInProductProfile( iPPSupportAls );
            if ( error == KErrNone )
                {
                savePPSimAls = ETrue;
                }            
            if ( error != KErrNone )
                {
                return error;
                }
                
            // Get Als support from SIM.  
            error = CheckAlsSupportInSim( iSimSupportAls );
            if ( error == KErrNone )
                {
                saveSimAls = ETrue;
                }                
            if ( ( error != KErrNone ) && ( error != KErrNotFound ) )
                {
                return error;
                } 
                
            // Open connection RCustomerServiceProfileCache.
            RCustomerServiceProfileCache alsProfileCache;
            error = alsProfileCache.Open();
            if ( error != KErrNone )
                {
                return error;
                } 
                
            // Get ALS CSP bit from cache.
            iAlsCSPError = alsProfileCache.CspCPHSTeleservices( params );
            
            if ( iAlsCSPError == KErrNone || iAlsCSPError == KErrNotSupported )
                {
                // Checks CSP ALS support.
                iAlsCSPSupport = CheckIfAlsCSPSupported( params );
                }
            // If ETEL/TSY responses with error KErrNotReady/KErrServerBusy then CSP value is not correct one
            // and iAlsStatusChecked is set to EFalse.      
            else if ( iAlsCSPError == KErrNotReady || iAlsCSPError == KErrServerBusy ) 
                {
                iAlsStatusChecked = EFalse;
                alsProfileCache.Close();
                return iAlsCSPError;
                } 
            // Close connection RCustomerServiceProfileCache.
            alsProfileCache.Close();   

            // Save PP and SIM als values to P&S.
            if ( savePPSimAls )
               {
               error = RProperty::Set( KPSUidSSConfig, KSettingsPPSupportsALS, iPPSupportAls );
               if ( error != KErrNone )
                   {
                   return error;
                   }
               }
             
            if ( saveSimAls )
               {
               error = RProperty::Set( KPSUidSSConfig, KSettingsSIMSupportsALS, iSimSupportAls );
               if ( error != KErrNone )
                   {
                   return error;
                   }
               }
                                
            iAlsStatusChecked = ETrue;
            }
        else
            {
            iAlsStatusChecked = EFalse;    
            }
        }    
    __SSSLOGSTRING1("[SSS]--> CSSSettingsAlsNotifier::EnsureAlsStatus: iAlsStatusChecked: %d", iAlsStatusChecked);
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::EnsureAlsStatus");
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CSSSettingsAlsNotifier::CheckIfAlsCSPSupported
// ---------------------------------------------------------------------------
// 
TBool CSSSettingsAlsNotifier::CheckIfAlsCSPSupported(
    const RMobilePhone::TCspCPHSTeleservices aContainer ) const
    {
    TBool supported(EFalse);

    if ( ( aContainer & RMobilePhone::KCspALS ) != 0 )
        {       
        supported = ETrue;  
        }
    return supported;
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::HandleNotifyInt
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::HandleNotifyInt( TUint32 aId, TInt /*aNewValue*/ )
    {
    __SSSLOGSTRING("[SSS] -->CSSSettingsAlsNotifier::HandleNotifyInt");
    if ( aId == KSettingsAlsLine )
        {
        iAlsStatusChecked = EFalse;
        EnsureAlsStatus();

        // Inform others
        TSSSettingsAlsValue newLine = ESSSettingsAlsNotSupported;
        if ( GetAlsValue( newLine ) == KErrNone )
            {
            iAlsNotifyHandler->HandleNotifyInt(
                KSettingsAlsLine,
                newLine );
            }
        }
    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::HandleNotifyInt");
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::HandleNotifyError
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::HandleNotifyError( TUint32 aId, TInt /*aError*/,
    CCenRepNotifyHandler* /*aHandler*/ )
    {
    if ( aId == KSettingsAlsLine )
        {
        // Here you could call the below, if you are sure that it do not cause 
        // eternal loop
        // TRAPD( error, aHandler->StartListeningL() );
        }
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::HandlePubSubNotify
// -----------------------------------------------------------------------------
//
void CSSSettingsAlsNotifier::HandlePubSubNotify( const TUid aUid, const TUint32 aKeyId )
    {
    if ( aUid == KPSUidSSConfig )
        {
        if ( ( aKeyId == KSettingsPPSupportsALS ) || 
             ( aKeyId == KSettingsSIMSupportsALS ) )
            {
            
            if ( iAlsStatusChecked == EFalse )
                {
                EnsureAlsStatus();
                }
            
            // Inform others
            TSSSettingsAlsValue newValue = ESSSettingsAlsNotSupported;
            if ( GetAlsValue( newValue ) == KErrNone )
                {
                iAlsNotifyHandler->HandleNotifyInt( 
                    KSettingsAlsLine,
                    newValue );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CSSSettingsAlsNotifier::HandleRefresh
// -----------------------------------------------------------------------------
TInt CSSSettingsAlsNotifier::HandleRefresh()
    {
    __SSSLOGSTRING("[SSS]--> CSSSettingsAlsNotifier::HandleRefresh");
    TInt error = CheckAlsSupportInProductProfile( iPPSupportAls );
    if ( error == KErrNone )
        {
        error = RProperty::Set( 
            KPSUidSSConfig, 
            KSettingsPPSupportsALS, 
            iPPSupportAls );
        }
        
    if ( error != KErrNone )
        {
        return error;
        }
    
    error = CheckAlsSupportInSim( iSimSupportAls );
    if ( error == KErrNone )
        {
        error = RProperty::Set(
            KPSUidSSConfig,
            KSettingsSIMSupportsALS,
            iSimSupportAls );
        }
        
    if ( error != KErrNone )
        {
        return error;
        }

    // Inform others
    TSSSettingsAlsValue newLine;
    error = GetAlsValue( newLine );
    if ( error == KErrNone )
        {
        SetAlsValue( newLine );
        }

    __SSSLOGSTRING("[SSS] <--CSSSettingsAlsNotifier::HandleRefresh");
    return error;
    }

//  End of File  
